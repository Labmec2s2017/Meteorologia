#include <SoftwareSerial.h>
#include <String.h>
#include <elapsedMillis.h>
char incomingByte;
elapsedMillis waitTime;
SoftwareSerial mySerial(7, 8);


void setup()
{
  mySerial.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);    // the GPRS baud rate 
}
 
void loop()
{      
      sendtemp();
      delay(10000);
}
void sendtemp()
{
 
  mySerial.println("AT");
  waitforok();

  mySerial.println("AT+CPIN?");
  waitforok();

  mySerial.println("AT+CREG?");
  waitforok();

  mySerial.println("AT+CGATT?");
  waitforok();

  mySerial.println("AT+CIPSHUT");
  waitforshutok();

  mySerial.println("AT+CIPSTATUS");
  waitforok();

  mySerial.println("AT+CIPMUX=0");
  waitforok();

  mySerial.println("AT+CSTT=\"bam.entelpcs.cl\",\"entelpcs\",\"entelpcs\"");
 // mySerial.println("AT+CSTT=\"Free\"");
  waitforok();
 
  mySerial.println("AT+CIICR");//bring up wireless connection
  waitforok();

  mySerial.println("AT+CIPSPRT=0");
  waitforok();

  mySerial.println("AT+CIFSR");
  delay(200);
  
  mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  waitforconnectok();//waitforok();
 
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  String str="GET http://api.thingspeak.com/update?api_key=FAUJUXAVFGN39XP6&field1=0" + String(analogRead(A0))+ "&field2="+ String(analogRead(A1));//String("100");
  mySerial.println(str);//begin send data to remote serve
  ShowSerialData();
  //delay(5000);
 //ShowSerialData();
  
  mySerial.println((char)26);//sending
 delay(5000);
 ShowSerialData();

mySerial.println("AT+CSCLK=2");
  
} 
void ShowSerialData()
{
  while(mySerial.available()!=0)
Serial.write(mySerial.read());
}

bool waitFor(String searchString, int waitTimeMS) {
    Serial.println("Waiting for string");
    waitTime = 0;
    String foundText;
    while (waitTime < waitTimeMS) {
        if (!mySerial.available()) {
          // Nothing in the buffer, wait a bit
            delay(5);
            continue;
        }
        
        // Get the next character in the buffer
         incomingByte = mySerial.read();
        if (incomingByte == 0) {
          //Ignore NULL character
          continue;
        }
        Serial.print(incomingByte);
        foundText += incomingByte;
        
        if (foundText.lastIndexOf(searchString) != -1) {
            Serial.print("Found string after ");
            Serial.print(waitTime);
            Serial.println("ms.");
            return true;
        }
    }
    
    // Timed out before finding it
    Serial.println("string not found, timed out");
    return false;
}

/**
 * Wait for "OK" from the Sim800l
 */
bool waitforok() {
    return waitFor("\nOK\r\n", 3000);
}

bool waitforshutok() {
    return waitFor("\nSHUT OK\r\n", 3000);
}


bool waitforconnectok(){
  return waitFor("\nCONNECT OK\r\n",6000);
}

bool waitforsendok(){
return waitFor("\nSEND OK\r\n",6000);
}
