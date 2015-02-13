# How to connect Arduino GPRS/GSM to the Internet

When I was showing off my IOT house at CES this year about one in four people I talked to asked me if PubNub works over GSM/GPRS networks. Here at PubNub, we knew it would work, but we never tried it ourselves. Until now...

This week I got a very basic PubNub demo working over GSM/GPRS. Stephen and I are incredibly excited about the sort of opportunities this creates from PubNub. It's super useful for anything mobile (connected cars, drones) or without access to a WiFi signal (weather station, sprinklers).

## Parts List

Here's what I used:

* Arduino Uno rev 3
* Seeed Studio GPRS Shield V2
* ATT SIM Card (standalone)
* C size batteries (x5)
* C size battery holder (x5)
* Smart Phone
* Laptop

I bought the GPRS Shield from Radioshack where it was on sale. I got the ATT SIM card from Fry's electronics; they have a section for universal SIMS. It may be hard to find the standalone SIM card in other retail stores.

## Setup the SIM Card

First, you'll need to unlock your SIM card and attach a data plan to it. This is why a smart phone is in the parts list. 

Put the SIM card in your phone and read the instructions included within the package. In the case of the ATT SIM I purchased, I dialed a series of numbers on my phone to activate the card, and then configured a plan online. Make sure your mobile plan supports data transfer, not just calls and texts.

Once you've activated the SIM card, try connecting to a website on your mobile phone. If it succeeds, you have data and it'll work within your Arduino.

## Arduino GPRS Setup

Take your Arduino GPRS Shield, and plug the SIM card in. Seeed Studio has [a great tutorial about how to do this](http://www.seeedstudio.com/wiki/GPRS_Shield_V2.0#Application). Follow that tutorial but stop and return here before uploading code in "Test Setup".

![](http://www.seeedstudio.com/wiki/images/thumb/b/b9/GPRS_SIM.JPG/400px-GPRS_SIM.JPG)
![](http://www.seeedstudio.com/wiki/images/thumb/3/33/GPRS_SIM2.JPG/330px-GPRS_SIM2.JPG)

## Arduino Libraries

You'll need a whole heap of libraries to get this thing setup on Arduino, but trust me, it makes it a whole lot easier. I tried writing them myself!

You can find the specific libraries for the Seeed Studio GRPS Shield V2 [here](https://github.com/Seeed-Studio/GPRS_Shield_Suli). Pay attention to the docs, you need 3 libraries to get this baby rolling:

* [GPRS_Shield_Suli](https://github.com/Seeed-Studio/GPRS_Shield_Suli) - GPRS Module Library, 
* [SoftwareSerial](https://github.com/arduino/Arduino/tree/master/libraries/SoftwareSerial) - we use software serial to communicate with GPRS module
* [Suli_Arduino](https://github.com/Seeed-Studio/Suli) - the Seeed Unified Libray interface

Import all three of these libraries into your Arduino setup.

## Software Serial

Now for the fun part. Your shield and Arduino environment are all setup. So let's play with cell networks.

First, we need a way to talk to the SIM900 chip on the Arduino. We could use a Hardware serial and talk directly to it, but that's hard, so let's just use Arduino's software serial.

Open up the ```SIM_900_Serial_Debug``` example from your Arduino examples folder.

![](http://i.imgur.com/6XH51Fo.png)

This example is simple, it just proxies the serial data from Arduino's serial port into SIM900's. This let's us use the built in Arduino Serial debug tool to interact with our SIM900.

```
void loop(){
  if(gprs.available()){
    Serial.write(gprs.read());
  }
  if(Serial.available()){     
    gprs.write(Serial.read()); 
  }
}
```

## Call Yourself

Let's try calling ourselves with the Arduino GSM shield to test everything out.

Load the Software Serial example onto the Arduino, and open up the Serial Debugger. Make sure you power on the SIM card on the Arduino GPRS shield using the button on the side. 

> Power up the SIM900 by pressing the power button in about 2 seconds. The red LED will be on. The green one beside it will blink. If the shield join the network sucessfully, the green LED will blink every 3 seconds. 

Make sure you put your old SMS card back into your phone after testing the Arduino SIM!

Now, type the following into your Arduino Serial Window:

```
AT
```

Serial output should show something like:

```
RDY

+CFUN: 1

+CPIN: READY

+PACSP: 0

Call Ready
```

> If you can not see the messages in the serial monitor, you should click the "send new" option that will add carriage return at the end of AT command and then send AT command "AT+IPR=19200" to set the baud rate of the SIM900.

Now try calling yourself. Enter the following command, replacing ```1***8675309``` with your own number.

```
ATD1***8675309;
```

If it succeeds, a message ```ATH OK``` will show up as the picture below. Otherwise, ```No CARRIER``` will show up instead. The reason might be nonexistent phone number or incorrect command format.

## Connect to the Internet

We've successfully configured our Arduino project to work over GSM. Now let's get it connected to the Internet.

> Make sure that the baud rate of SIM900 is 9600! You can use the AT Command(AT+IPR=9600) to set it through SerialDebug

Load up the the GPRS_TCPConnection example from GPRS_Shield_Suli. This example makes a simple request to ```mbed.org/media/uploads/mbed_official/hello.txt HTTP/1.0\r\n\r\n```

However, it probably won't run when you load it on your Arduino. This is because it's not configured to work with your specific cellular network.

In my case I'm using ATT, so I needed to change the following lines to configure my gprs connection:

```
GPRS gprs(PIN_TX, PIN_RX, BAUDRATE,"cmnet");
```

This line isn't sufficient in most cases. We'll also need to supply an APN, username, and password. These parameters are omitted from the example but you can simply supply them on your own. You can find the relevant code [here](https://github.com/Seeed-Studio/GPRS_Shield_Suli/blob/master/GPRS_Shield_Arduino.h). 

```
GPRS(int tx, int rx, uint32_t baudRate = 9600, const char* apn = NULL, const char* userName = NULL, const char *passWord = NULL);
```

To connect to ATT, use the following config:

```
GPRS gprs(PIN_TX, PIN_RX, BAUDRATE, "wap.cingular", "WAP@CINGULARGPRS.COM", "CINGULAR1");
```

Now run the code. You should be able to obtain an IP and issue the request. However, the app will probably crash (SIM turns off) when the request has been issued. This might be confusing, but there's an easy fix.

## Batteries

The Seeed Studio and a variety of other shields advertise to be "plug and play" with Arduino but this is not always the case. This shield in particular doesn't have enough power from the single 5v USB supply to successfully complete TCP requests.

This is easy enough to fix though, all we need to do is give it more power! Online forums suggest adding 5 C cell batteries to power the GSM chip, which is what I have done.

![](http://i.imgur.com/EHjyBhK.jpg)

The batteries should be wired in series, with the positive end wired into the VIN input and negative wired into the GND. 

After you've added batteries, try the example again. It should connect successfully.

Great! Now we're ready to signal with PubNub.

## PubNub

Now that we're fully powered, let's get to the fun part. Load the following example onto your board, changing the ```GPRS gprs()``` configuration as mentioned previously. 

```
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
    
    Serial.println("attempting to publish");
    
    while (false == gprs.join()) {
      Serial.println("gprs join network error");
      delay(2000);
    }

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
      Serial.println("publish success");
      Serial.println(data);
    }

    gprs.send(http_cmd, sizeof(http_cmd) - 1);

    gprs.close();

  }

  int counter = 0;

  void setup() {

    Serial.println("starting");
    
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
```

This example connects to PubNub and publishes a message every second. It works just like the previous example, but instead of retrieving data this example streams information to the PubNub network where it's accessible from anywhere.

http://www.pubnub.com/console/?channel=pubnub_gprs&origin=pubsub.pubnub.com&sub=demo&pub=demo&cipher=&ssl=false&secret=&auth=

## Rest Client

You can see how the message is assembled by looking at the ```publishData()``` function.

See how the url is created here:

```
String str = "GET /publish/demo/demo/0/pubnub_gprs/0/\"" + data + "\" HTTP/1.0\r\n\r\n";
```

It may be confusing at first, but let's break it down. The url is formatted according to the [PubNub REST Push API](http://www.pubnub.com/http-rest-push-api/).

```
http://pubsub.pubnub.com
/publish
/pub-key
/sub-key
/signature
/channel
/callback
/message
```

The domain ```pubsub.pubnub.com``` is defined later in the function. The ```publish``` key refers to is the pubnub command (you can also subscribe). The ```pub-key``` and ```sub-key``` come from your own PubNub account, but you can use the ```demo``` keys for now. Don't worry about signature. 

The ```channel``` is really important. This is like an IRC channel for your PubNub devices to talk to each other.

## Show me the data

Since we don't have any other device listening for messages from the Arduino, let's use the PubNub console to observe our channel activity.

If you're using the default code above, you can use [this link](http://www.pubnub.com/console/?channel=pubnub_gprs&origin=pubsub.pubnub.com&sub=demo&pub=demo&cipher=&ssl=false&secret=&auth=) to view channel activity for your GPRS chip. 

If your demo is running you should see a count like 1, 2, 3, 4 appear in the "message" pane. 

![](http://i.imgur.com/wGLF8ee.png)

We're able to view the data coming out of the Arduino because we plugged the same publish and subscribe key into the PubNub console as we did into the Arduino. Since we're using the ```demo``` keys the channel is shared publicly, but if you create your own account your data will be privatized. 

## More info

* You can learn more about TCP connection over GPRS from [this blog post](https://vsblogs.wordpress.com/2013/11/28/tcp-connection-over-gprs-using-sim900-and-at-commands/). 
* The full Seeedstudio GPRS Shield v2 resource can be [found here](http://www.seeedstudio.com/wiki/GPRS_Shield_V2.0#Resource).
* The suggestion for adding 5 C batteries in series [came from this post](https://nateclifford.wordpress.com/2013/09/27/seeedstudiogprs-power-problems/).
