
#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Suli.h>

#define PIN_TX    7
#define PIN_RX    8
//make sure that the baud rate of SIM900 is 9600!
//you can use the AT Command(AT+IPR=9600) to set it through SerialDebug
#define BAUDRATE  9600


GPRS gprs(PIN_TX, PIN_RX, BAUDRATE, "wap.cingular", "WAP@CINGULARGPRS.COM", "CINGULAR1");

void publishData(String data) {
  
  while (false == gprs.join()) {
    Serial.println("gprs join network error");
    delay(2000);
  }

  // successful DHCP
  Serial.print("IP Address is ");
  Serial.println(gprs.getIPAddress());

  String str = "GET /publish/demo/demo/0/pubnub_gprs/0/\"" + data + "\" HTTP/1.0\r\n\r\n";
  
  // Length (with one extra character for the null terminator)
  int str_len = str.length() + 1; 

  // Prepare the character array (the buffer) 
  char http_cmd[str_len];

  // Copy it over 
  str.toCharArray(http_cmd, str_len);

  char buffer[512];

  if (false == gprs.connect(TCP, "pubsub.pubnub.com", 80)) {
    Serial.println("connect error");
  } else {
    Serial.println("connect pubnub success");
  }

  Serial.println("waiting to fetch...");
  gprs.send(http_cmd, sizeof(http_cmd) - 1);

  /*

  while (true) {

    int ret = gprs.recv(buffer, sizeof(buffer) - 1);

    if (ret <= 0) {
      Serial.println("fetch over...");
      break;
    }

    buffer[ret] = '\0';
    Serial.print("Recv: ");
    Serial.print(ret);
    Serial.print(" bytes: ");
    Serial.println(buffer);

  }
  
  */

  gprs.close();

}

int counter = 0;

void setup() {

  Serial.begin(9600);
  // use DHCP
  gprs.init();
  // attempt DHCP
  
  while(true){
    publishData(String(counter));
    counter++;
    delay(1000);
  }
  
  gprs.disconnect();

}

void loop() {

}
