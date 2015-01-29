//Serial Relay - Arduino will patch a 
//serial link between the computer and the GPRS Shield
//at 19200 bps 8-N-1
//Computer is connected to Hardware UART
//GPRS Shield is connected to the Software UART 
 
#include <SoftwareSerial.h>
 
SoftwareSerial GPRS(7, 8);
unsigned char buffer[64]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array 

int setupStep = 0;

void setup()
{
  
  GPRS.begin(19200);               // the GPRS baud rate   
  Serial.begin(19200);             // the Serial port of Arduino baud rate.
 
}
 
void loop()
{
  if (GPRS.available())              // if date is comming from softwareserial port ==> data is comming from gprs shield
  {
    while(GPRS.available())          // reading data into char array 
    {
      buffer[count++]=GPRS.read();     // writing data into array
      if(count == 64)break;
  }
    Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    count = 0;                       // set counter of while loop to zero
 
 
  }
  
  if(setupStep < 12) {
    ConnectToInternet(setupStep);
  }
  
}
void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
    { buffer[i]=NULL;}                  // clear all index of array with command NULL
}

void ConnectToInternet(int step) {
  
  int delayT = 1000;
  
  if(step == 0) {
    GPRS.println("AT"); 
    delay(delayT);
  }
  
  if(step == 1) {
    GPRS.println("AT+CPIN?");
    delay(delayT);
  }
  
  if(step == 2) {
    GPRS.println("AT+CREG?");
    delay(delayT);
  }
  
  if(step == 3) {
    GPRS.println("AT+CGATT?");
    delay(delayT);
  }
  
  if(step == 4) {
    GPRS.println("AT+CIPSHUT");
    delay(delayT);
  }
  
  if(step == 5) {
    GPRS.println("AT+CIPSTATUS");
    delay(delayT);
  }
  
  if(step == 6) {
    GPRS.println("AT+CIPMUX=0");
    delay(delayT);
  }
  
  if(step == 7) {
    GPRS.println("AT+CSTT=\"wap.cingular\",\"WAP@CINGULARGPRS.COM\",\"CINGULAR1\"");
    delay(delayT);
  }
  
  if(step == 8) {
    GPRS.println("AT+CIICR");
    delay(delayT * 3);
  }

  if(step == 9) {
    GPRS.println("AT+CIFSR");
    delay(delayT);
  }
  
  if(step == 10) {
    GPRS.println("AT+CIPSTART=\"TCP\",\"http://pubsub.pubnub.com/subscribe/demo/pubnub_gprs/\",\"80\"");
    delay(delayT);
    delay(2000);
  }
  
  if(step == 11) {
    Serial.println("here");
    GPRS.println("AT+CIPSEND=20");
    delay(delayT);
    delay(2000);
  }
  
  setupStep++;

}

