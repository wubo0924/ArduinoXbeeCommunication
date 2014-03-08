/**Created by Bo **/

#include <SPI.h>
#include <XBee.h>
#include <Time.h>

// create the XBee object
XBee xbee = XBee();


  String id;
  String readingtime;
  String reading;
  
int count_id;
int count_readingtime;
int count_reading;

String timestamp;

String test;
int ldrPin = 0;
int lightVal = 0;

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x13A200, 0x40A9C82D);
ZBTxRequest zbTx;
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

//char ids="S1ZZZ";


void setup() {
  setTime(15,45,0,24,10,13);
  Serial.begin(9600);
  Serial.flush();
  xbee.setSerial(Serial);
  
  
}

void loop() {
  
   /*caution::::::
   please comply to following structure to send data reading to coordinator
   get reading from analog pin first, then call getCurrentTime().
   make sure get reading before getCurrentTime() function.
   ::::::::::*/
   
   /*Start*
   reading=String(analogRead(0));
   id="";
   readingtime=getCurrentTime();
   sendToCoordinator(id,readingtime,reading); 
   *End*/
}

/*
* Send sensor reading to coordinator
* Call method when sending reqired
*/

void sendToCoordinator(String id,String readingtime,String reading){
    /*****assemble correct format payload****/
  char ids[id.length()+1];
  char readingtimes[readingtime.length()+1];
  char readings[reading.length()+1];
  
  id.toCharArray(ids,id.length()+1);
  readingtime.toCharArray(readingtimes,readingtime.length()+1);
  reading.toCharArray(readings,reading.length()+1);
  
  count_id = id.length();
  count_readingtime = readingtime.length();
  count_reading = reading.length();
  
  //Serial.println(count_id + count_readingtime + count_reading);
  
  uint8_t payload[count_id + count_readingtime + count_reading + 2];
  
  for(int i=0;i<count_id;i++){
    payload[i]=ids[i];
  
    test.concat(String(ids[i]));
  }
 
  payload[count_id]=';';
  test.concat(";");
  int j=0;
  
  for(int i=count_id+1;i<count_id+count_readingtime+1;i++){
    payload[i]=readingtime[j];
    test.concat(String(readingtime[j]));
    j++;
  }
  payload[count_id+count_readingtime+1]=';';
  test.concat(";");
  j=0;
  for(int i=count_id+count_readingtime+2;i<count_id+count_readingtime+2+count_reading+1;i++){
    payload[i]=readings[j];
    test.concat(String(readings[j]));
    j++;
  }
  /**********end***************/
  //Serial.println(sizeof(payload));
  for(int i=0;i<sizeof(payload);i++){
    Serial.print(char(payload[i]));
    
  }
  
  zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
  xbee.send(zbTx);

  delay(1000);
}



/*
* Get current date and time 
* Format:yyyy-mm-dd hh:mm:ss
*
*/

String getCurrentTime(){
  int y=year();
  int m=month();
  int d=day();
  int h=hour();
  int minu=minute();
  int s=second();
  //Serial.println(String(y)+"-"+String(m)+"-"+String(d)+" "+String(h)+":"+String(minu)+":"+String(s));
  
  String timeformat="";
  timeformat.concat(String(y));
  timeformat.concat("-");
  if(m<10){
    timeformat.concat("0");
    timeformat.concat(String(m));
  }else{
    timeformat.concat(String(m));
  }
  timeformat.concat("-");
  if(d<10){
    timeformat.concat("0");
    timeformat.concat(String(d));
  }else{
    timeformat.concat(String(d));
  }
  timeformat.concat("|");
  if(h<10){
    timeformat.concat("0");
    timeformat.concat(String(h));
  }else{
    timeformat.concat(String(h));
  }
  timeformat.concat(":");
  if(minu<10){
    timeformat.concat("0");
    timeformat.concat(String(minu));
  }else{
    timeformat.concat(String(minu));
  }
  timeformat.concat(":");
  if(s<10){
    timeformat.concat("0");
    timeformat.concat(String(s));
  }else{
    timeformat.concat(String(s));
  }
  //Serial.println(timeformat);
  return timeformat;
}
/*
* Conver float to String
* in order to be accepted by coordinator
*/
String floatToString(float v){
  //sizeof(v);
  char tmp[15];
  dtostrf(v,1,2, &tmp[0]);
  return tmp;
}

