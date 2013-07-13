int trigpin = 3;
int echopin = 2;
int minA = 1;
int maxA = 30;
long duration;
long distance;
int i;

/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  pinMode(3,OUTPUT);
  pinMode(2,INPUT);
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          
          if(ultradistance()) {
           int sensorReading = analogRead(A0);
           if(sensorReading < 400) {
            client.print("Soil is wet");
           } 
           else if(sensorReading < 500) {
            client.print("Soil is slightly wet"); 
           }
           else if(sensorReading < 600) {
            client.print("Soil is not so moist"); 
           }
           else {
             client.print("<img src= \"http://upload.wikimedia.org/wikipedia/commons/a/a0/Dead_plant_in_pots.jpg\" width=\"100\">");
            client.print("Soil is dry"); 
           }
          }
                  
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


 boolean ultradistance()
           {
           digitalWrite(trigpin, LOW);
           delayMicroseconds(2);
           digitalWrite(trigpin, HIGH);
           delayMicroseconds(10);
           digitalWrite(trigpin, LOW);
           duration = pulseIn(echopin, HIGH);
           distance = duration/58.2;
           if(distance <= maxA && distance >= minA) {
            return true;
           }
           else {
            return false; 
           }
           }
          
