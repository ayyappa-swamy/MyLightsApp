/*
 * Circuit on Tiva Connected Launchpad for MyLights hardware setup:
 * For Serial3 communication :
 *    Pin PC_4 - TX of Arduino
 *    Pin PC_5 - RX of Arduino
 * Pin PD_2 - IN3 of Relay board
 * 5v - VCC of Relay board
 * GND - GND of Realy board
 * Ethernet port - Ethernet port of Wifi Router
*/

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0x00, 0x1A, 0xB6, 0x03, 0x1C, 0xF8
};

EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(PD_2, OUTPUT);
  // this check is only needed on the Leonardo:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  while (!Serial3) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  // print your local IP address:
  printIPAddress();
  server.begin();
}

void loop() {

  switch (Ethernet.maintain())
  {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("Renewed success");

      //print your local IP address:
      printIPAddress();
      break;

    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");

      //print your local IP address:
      printIPAddress();
      break;

    default:
      //nothing happened
      break;
  }
  listenForEthernetClients();
}

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
}

void listenForEthernetClients() {
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client) {                             // if you get a client,
    Serial.print("new client on port ");           // print a message out the serial port
    Serial.println(client.port());
    String currentLine = "";                // make a String to hold incoming data from the client
    boolean newConnection = true;     // flag for new connections
    unsigned long connectionActiveTimer;  // will hold the connection start time

    while (client.connected()) {       // loop while the client's connected
      if (newConnection){                 // it's a new connection, so
        connectionActiveTimer = millis(); // log when the connection started
        newConnection = false;          // not a new connection anymore
      }
      if (!newConnection && connectionActiveTimer + 1000 < millis()){ 
        // if this while loop is still active 1000ms after a web client connected, something is wrong
        break;  // leave the while loop, something bad happened
      }


      if (client.available()) {             // if there's bytes to read from the client,    
        char c = client.read();             // read a byte, then
        // This lockup is because the recv function is blocking.
        Serial.print(c);
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {  
            break;         
          } 
          else {      // if you got a newline, then clear currentLine:
            if(currentLine.lastIndexOf("/rgb/") > 0){
                String rgbRequest = currentLine.substring(currentLine.indexOf("/rgb/"), currentLine.indexOf("HTTP"));
                Serial3.println(rgbRequest);
                Serial.println("***************************\nRGB Request received: ");
                Serial.println(rgbRequest);
                Serial.println("***************************");
                break;
            }
            else if (currentLine.lastIndexOf("/bulb/") > 0){
                String bulbRequest = currentLine.substring(currentLine.indexOf("/bulb/"), currentLine.indexOf("HTTP"));
                Serial3.println(bulbRequest);
                Serial.println("***************************\nBulb Request received: ");
                Serial.println(bulbRequest);
                Serial.println("***************************");
                
                String bulbVal = bulbRequest.substring(bulbRequest.lastIndexOf('/') + 1);
                digitalWrite(PD_2,bulbVal.toInt());
                break;
            }
            else if (currentLine.lastIndexOf("/led/green/") > 0){
                String bulbRequest = currentLine.substring(currentLine.indexOf("/led/green/"), currentLine.indexOf("HTTP"));
                Serial3.println(bulbRequest);
                Serial.println("***************************\nLED Request received: ");
                Serial.println(bulbRequest);
                Serial.println("***************************");
                break;
            }
            else if (currentLine.lastIndexOf("/led/red/") > 0){
                String bulbRequest = currentLine.substring(currentLine.indexOf("/led/red/"), currentLine.indexOf("HTTP"));
                Serial3.println(bulbRequest);
                Serial.println("***************************\nLED Request received: ");
                Serial.println(bulbRequest);
                Serial.println("***************************");
                break;
            }
            currentLine = "";
          }
        }     
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    //Serial.println("client disonnected");
  }
}
