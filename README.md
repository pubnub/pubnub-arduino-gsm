* ATT SIM Card from Fry's
* Seeed Studio GSM Card V2 from radioshack
* Activate SIM card via mobile phone
* software serial
* send text
* call phone
* https://vsblogs.wordpress.com/2013/11/28/tcp-connection-over-gprs-using-sim900-and-at-commands/
* http://www.seeedstudio.com/forum/viewtopic.php?f=23&t=5350
* https://nateclifford.wordpress.com/2013/09/27/seeedstudiogprs-power-problems/
* custom loader
* power drain

AT

OK
AT+CPIN?

+CPIN: READY

OK

AT+CREG?

+CREG: 0,1

OK
AT+CGATT?

+CGATT: 1

OK
AT+CIPSHUT

SHUT OK
AT+CIPSTATUS

OK

STATE: IP INITIAL

AT+CIPMUX=0

OK
AT+CSTT="wap.cingular","WAP@CINGULARGPRS.COM","CINGULAR1"

OK
AT+CIICR

OK
AT+CIFSR

10.99.152.119
AT+CIPSTART="TCP","http://www.downforeveryoneorjustme.com/","80"

OK

CONNECT OK
