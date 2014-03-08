#include <XBee.h>
#include <SPI.h>
#include <WiFi.h>
#include <Time.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

String value;
String id;
String readingtime;
String reading;

String finalValue;

int count_;

int request_count=0;

char ssid[] = "pocketwifi-ce1c"; //  your network SSID (name) 
char pass[] = "40761870"; 
int status = WL_IDLE_STATUS;

IPAddress server(115,146,92,166);
//char server[] = "115.146.92.166";    // name address for Google (using DNS)

WiFiClient client; 

void setup() {
 
  // start serial
  Serial.begin(9600);
  xbee.begin(Serial);
  
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(3000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
  
  Serial.println("\nStarting connection to server...");
  client.connect(server, 80);
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
    value=NULL;
    id="";
    reading="";
    readingtime="";
    finalValue="";
    count_=0;
    
    //Serial.println("----");
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) { 
      xbee.getResponse().getZBRxResponse(rx);
      //Serial.println(rx.getDataLength());
      for(int i=0;i<rx.getDataLength();i++){
        value.concat(char(rx.getData(i)));
      }
      
      
      //Serial.println(value);
      
      Serial.println("content: "+value);
      Serial.println( "/EnvisAWS/services/DataConsumer/streamDataIn?data="+ value + " HTTP/1.1"); 
      Serial.println("connected to server");
      
      client.println("GET /EnvisAWS/services/DataConsumer/streamDataIn?data="+ value + " HTTP/1.1");
      client.println("Host: rmitenvis-env.elasticbeanstalk.com");
      // client.println("Connection: close");
      client.println();
      delay(2000);
      
  // if there are incoming bytes available 
  // from the server, read them and print them:
     while (client.available()) {
       char c = client.read();
       Serial.write(c);
     }
     
     /***after every 10 reuest, close connection***/
     /*
     if(request_count<10){
       request_count++;
     }else{
       //client.stop();
       client.println("Connection: close");
       request_count=0;
     }
     /****end****/
     
  // if the server's disconnected, stop the client:
     if (!client.connected()) {
       Serial.println();
       Serial.println("disconnected from server....trying again to connect");
       client.stop();
       client.connect(server, 80);
     }
  }
    
}

void divideString(String v){
  int i=v.indexOf(';');
  id=v.substring(0,i);
  reading=v.substring(i+1,v.length());
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
