#include <Wire.h> // Library for I2C communication
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

//firebase variables
#define FIREBASE_HOST "medicine-2c770-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "QwOvQVJsKGElscxaTS697fj6u1rfsP6XNKrz8Ux0"        //Your Firebase Database Secret goes here
//#define WIFI_SSID "Airtel_9920828883"                                               //WiFi SSID to which you want NodeMCU to connect
//#define WIFI_PASSWORD "air45290"                                      //Password of your wifi network

 // Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value
int led_stat_one=0;
int led_stat_two=0;

uint8_t ledone=D6;
uint8_t ledtwo=D5;

//Variables
int statusCode,num;
const char* ssid = "realme 7 pro";
const char* passphrase = "123456789";
String st;
String content;
#define IRPin A0
#define pinA D5 
#define pinB D6
#define pinC D7

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);

//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);

//NTP Client declaration
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Setup an instance of DS1307 naming it RTC
RTC_DS1307 RTC;     

void setup()
{

  pinMode(ledone, OUTPUT);
  pinMode(ledtwo, OUTPUT);
  pinMode(IRPin, INPUT);
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  Serial.begin(9600); //Initialising if(DEBUG)Serial Monitor
  Wire.begin(); // Start the I2C
  RTC.begin();  // Init RTC
  RTC.adjust(DateTime(__DATE__, __TIME__));  // Time and date is expanded to date and time on your computer at compiletime
  Serial.print('Time and date set');
  timeClient.begin();
  timeClient.setTimeOffset(0);
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");

  //---------------------------------------- Read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");

  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);


  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
    Serial.println("Connected to Firebase");
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
  }

  Serial.println();
  Serial.println("Waiting.");
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }
}
void loop() {

  DateTime now = RTC.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(' ');
  Serial.println();
  delay(1000);
  getAlarm();
  delay(100);
  checkAlarm();
//  checkMed();

}


//----------------------------------------------- Fuctions used for WiFi credentials saving and connecting to it which you do not need to change 
bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}

void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("testWIFI", "123456789"); // ssid and password for hotspot
  WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
}

void createWebServer()
{
 {
    server.on("/", []() {

      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p>";
      content += st;
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      content = "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();

        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);

    });
  } 
}

void getAlarm(){

  int d1h,d1m,d2h,d2m,d3h,d3m;
  timeClient.update();
  int dow = timeClient.getDay();

  Serial.println("Get Alarm");
  Serial.print(dow);

  Firebase.getInt(firebaseData, "/flagStatus");
  int flagStatus = firebaseData.intData();
  Serial.println("");
  Serial.print("Flag: ");
  Serial.print(flagStatus);
  Serial.println("");

  if(flagStatus == 0){
    //Sunday
    if(dow == 0){
      Firebase.getInt(firebaseData, "/monday/dose_1Hour");
      d1h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/sunday/dose_1Minute");
      d1m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/sunday/dose_2Hour");
      d2h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/sunday/dose_2Minute");
      d2m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/sunday/dose_3Hour");
      d3h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/sunday/dose_3Minute");
      d3m = firebaseData.intData();
      Serial.println("getAlarm");
      Serial.println(d1h);
      EEPROM.write(150,d1h);
      EEPROM.write(152,d1m);
      delay(100);
      EEPROM.write(154,d2h);
      EEPROM.write(156,d2m);
      delay(100);
      EEPROM.write(158,d3h);
      EEPROM.write(160,d3m);
      delay(100);
      delay(1000);
      Firebase.setInt(firebaseData,"/flagStatus",1);
      Serial.println("Sunday Set"); 
    }
    
    //Monday
    if(dow == 1){
      Firebase.getInt(firebaseData, "/monday/dose_1Hour");
      d1h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/monday/dose_1Minute");
      d1m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/monday/dose_2Hour");
      d2h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/monday/dose_2Minute");
      d2m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/monday/dose_3Hour");
      d3h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/monday/dose_3Minute");
      d3m = firebaseData.intData();
    
      EEPROM.write(162,d1h);
      EEPROM.write(164,d1m);
      delay(100);
      EEPROM.write(166,d2h);
      EEPROM.write(168,d2m);
      delay(100);
      EEPROM.write(170,d3h);
      EEPROM.write(172,d3m);
      delay(100);
      delay(1000); 
      Firebase.setInt(firebaseData,"/flagStatus",1);
      Serial.println("Monday Set");
    }
  
    //Tuesday
    if(dow == 2){
      Firebase.getInt(firebaseData, "/tuesday/dose_1Hour");
      d1h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/tuesday/dose_1Minute");
      d1m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/tuesday/dose_2Hour");
      d2h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/tuesday/dose_2Minute");
      d2m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/tuesday/dose_3Hour");
      d3h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/tuesday/dose_3Minute");
      d3m = firebaseData.intData();
    
      EEPROM.write(174,d1h);
      EEPROM.write(176,d1m);
      delay(100);
      EEPROM.write(178,d2h);
      EEPROM.write(180,d2m);
      delay(100);
      EEPROM.write(182,d3h);
      EEPROM.write(184,d3m);
      delay(100);
      delay(1000); 
      Firebase.setInt(firebaseData,"/flagStatus",1);
      Serial.println("Tuesday Set");
    }
  
    //Wednesday
    if(dow == 3){
      Firebase.getInt(firebaseData, "/wednesday/dose_1Hour");
      d1h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/wednesday/dose_1Minute");
      d1m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/wednesday/dose_2Hour");
      d2h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/wednesday/dose_2Minute");
      d2m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/wednesday/dose_3Hour");
      d3h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/wednesday/dose_3Minute");
      d3m = firebaseData.intData();
    
      EEPROM.write(186,d1h);
      EEPROM.write(188,d1m);
      delay(100);
      EEPROM.write(190,d2h);
      EEPROM.write(192,d2m);
      delay(100);
      EEPROM.write(194,d3h);
      EEPROM.write(196,d3m);
      delay(100);
      delay(1000); 
      Firebase.setInt(firebaseData,"/flagStatus",1);
      Serial.println("Wednesday Set");
    }
  
    //Thursday
    if(dow == 4){
      Firebase.getInt(firebaseData, "/thursday/dose_1Hour");
      d1h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/thursday/dose_1Minute");
      d1m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/thursday/dose_2Hour");
      d2h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/thursday/dose_2Minute");
      d2m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/thursday/dose_3Hour");
      d3h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/thursday/dose_3Minute");
      d3m = firebaseData.intData();
    
      EEPROM.write(198,d1h);
      EEPROM.write(200,d1m);
      delay(100);
      EEPROM.write(202,d2h);
      EEPROM.write(204,d2m);
      delay(100);
      EEPROM.write(206,d3h);
      EEPROM.write(208,d3m);
      delay(100);
      delay(1000); 
      Firebase.setInt(firebaseData,"/flagStatus",1);
      Serial.println("Thursday Set");
    }
  
    //Friday
    if(dow == 5){
      Firebase.getInt(firebaseData, "/friday/dose_1Hour");
      d1h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/friday/dose_1Minute");
      d1m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/friday/dose_2Hour");
      d2h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/friday/dose_2Minute");
      d2m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/friday/dose_3Hour");
      d3h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/friday/dose_3Minute");
      d3m = firebaseData.intData();
    
      EEPROM.write(210,d1h);
      EEPROM.write(212,d1m);
      delay(100);
      EEPROM.write(214,d2h);
      EEPROM.write(216,d2m);
      delay(100);
      EEPROM.write(218,d3h);
      EEPROM.write(220,d3m);
      delay(100);
      delay(1000); 
      Firebase.setInt(firebaseData,"/flagStatus",1);
      Serial.println("Friday Set");
    }
  
    //Saturday
    if(dow == 6){
      Firebase.getInt(firebaseData, "/saturday/dose_1Hour");
      d1h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/saturday/dose_1Minute");
      d1m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/saturday/dose_2Hour");
      d2h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/saturday/dose_2Minute");
      d2m = firebaseData.intData();
    
      Firebase.getInt(firebaseData, "/saturday/dose_3Hour");
      d3h = firebaseData.intData();
      Firebase.getInt(firebaseData, "/saturday/dose_3Minute");
      d3m = firebaseData.intData();
    
      EEPROM.write(222,d1h);
      EEPROM.write(224,d1m);
      delay(100);
      EEPROM.write(226,d2h);
      EEPROM.write(228,d2m);
      delay(100);
      EEPROM.write(230,d3h);
      EEPROM.write(232,d3m);
      delay(100);
      delay(1000); 
      Firebase.setInt(firebaseData,"/flagStatus",1);
      Serial.println("Saturday Set");
    }
  }
}

void checkAlarm(){
  DateTime now = RTC.now();
  timeClient.update();
  int dow = timeClient.getDay();

  Serial.println(EEPROM.read(174));
  Serial.println(EEPROM.read(176));
  Serial.println(dow);
  //Sunday
  if(dow == 0){
    Serial.println(EEPROM.read(150));
    Serial.println(now.hour());
    Serial.println("");
    Serial.println(EEPROM.read(152));
    Serial.println(now.minute());
    if((EEPROM.read(150) == now.hour())&&(EEPROM.read(152) == now.minute())){
      Serial.println("Dose 1 Beep");
    }
  
    if((EEPROM.read(154) == now.hour())&&(EEPROM.read(156) == now.minute())){
      Serial.println("Dose 2 Beep");
    }
  
    if((EEPROM.read(158) == now.hour())&&(EEPROM.read(160) == now.minute())){
      Serial.println("Dose 3 Beep");
    }
  }

  //Monday
  if(dow == 1){
    if((EEPROM.read(162) == now.hour())&&(EEPROM.read(164) == now.minute())){
      Serial.println("Dose 1 Beep");
    }
  
    if((EEPROM.read(166) == now.hour())&&(EEPROM.read(168) == now.minute())){
      Serial.println("Dose 2 Beep");
    }
  
    if((EEPROM.read(170) == now.hour())&&(EEPROM.read(172) == now.minute())){
      Serial.println("Dose 3 Beep");
    }
  }

  //Tuesday
  if(dow == 2){
    if((EEPROM.read(174) == now.hour())&&(EEPROM.read(176) == now.minute())){
      Serial.println("Dose 1 Beep");
       checkMed();
    }
  
    if((EEPROM.read(178) == now.hour())&&(EEPROM.read(180) == now.minute())){
      Serial.println("Dose 2 Beep");
    }
  
    if((EEPROM.read(182) == now.hour())&&(EEPROM.read(184) == now.minute())){
      Serial.println("Dose 3 Beep");
    }
  }

  //Wednesday
  if(dow == 3){
    if((EEPROM.read(186) == now.hour())&&(EEPROM.read(188) == now.minute())){
      Serial.println("Dose 1 Beep");
    }
  
    if((EEPROM.read(190) == now.hour())&&(EEPROM.read(192) == now.minute())){
      Serial.println("Dose 2 Beep");
    }
  
    if((EEPROM.read(194) == now.hour())&&(EEPROM.read(196) == now.minute())){
      Serial.println("Dose 3 Beep");
    }
  }

  //Thursday
  if(dow == 4){
    if((EEPROM.read(198) == now.hour())&&(EEPROM.read(200) == now.minute())){
      Serial.println("Dose 1 Beep");
    }
  
    if((EEPROM.read(202) == now.hour())&&(EEPROM.read(204) == now.minute())){
      Serial.println("Dose 2 Beep");
    }
  
    if((EEPROM.read(206) == now.hour())&&(EEPROM.read(208) == now.minute())){
      Serial.println("Dose 3 Beep");
    }
  }

  //Friday
  if(dow == 5){
    if((EEPROM.read(210) == now.hour())&&(EEPROM.read(212) == now.minute())){
      Serial.println("Dose 1 Beep");
    }
  
    if((EEPROM.read(214) == now.hour())&&(EEPROM.read(216) == now.minute())){
      Serial.println("Dose 2 Beep");
    }
  
    if((EEPROM.read(218) == now.hour())&&(EEPROM.read(220) == now.minute())){
      Serial.println("Dose 3 Beep");
    }
  }

  //Saturday
  if(dow == 6){
    if((EEPROM.read(222) == now.hour())&&(EEPROM.read(224) == now.minute())){
      Serial.println("Dose 1 Beep");
    }
  
    if((EEPROM.read(226) == now.hour())&&(EEPROM.read(228) == now.minute())){
      Serial.println("Dose 2 Beep");
    }
  
    if((EEPROM.read(230) == now.hour())&&(EEPROM.read(232) == now.minute())){
      Serial.println("Dose 3 Beep");
    }
  }
  
}

void pinSelect(int num){

  if(num == 0){
    digitalWrite(pinC,LOW);
    delay(100);
    digitalWrite(pinB,LOW);
    delay(100);
    digitalWrite(pinA,LOW);
    delay(100);
  }

  if(num == 1){
    digitalWrite(pinC,LOW);
    delay(100);
    digitalWrite(pinB,LOW);
    delay(100);
    digitalWrite(pinA,HIGH);
    delay(100);
  }

  if(num == 2){
    digitalWrite(pinC,LOW);
    delay(100);
    digitalWrite(pinB,HIGH);
    delay(100);
    digitalWrite(pinA,LOW);
    delay(100);
  }

  if(num == 3){
    digitalWrite(pinC,LOW);
    delay(100);
    digitalWrite(pinB,HIGH);
    delay(100);
    digitalWrite(pinA,HIGH);
    delay(100);
  }

  if(num == 4){
    digitalWrite(pinC,HIGH);
    delay(100);
    digitalWrite(pinB,LOW);
    delay(100);
    digitalWrite(pinA,LOW);
    delay(100);
  }

  if(num == 5){
    digitalWrite(pinC,HIGH);
    delay(100);
    digitalWrite(pinB,LOW);
    delay(100);
    digitalWrite(pinA,HIGH);
    delay(100);
  }

  if(num == 6){
    digitalWrite(pinC,HIGH);
    delay(100);
    digitalWrite(pinB,HIGH);
    delay(100);
    digitalWrite(pinA,LOW);
    delay(100);
  }
  
}

void checkMed(){

  timeClient.update();
  int dow = timeClient.getDay();

  if(dow == 0){
    pinSelect(0);
    num = analogRead(IRPin);
    if(num >= 400){
        Serial.println("Sunday Take Medicine");
      }
    else{
      Serial.println(" Sunday Medicine Taken");
    }
    delay(100);
  }

  if(dow == 1){
    pinSelect(1);
    num = analogRead(IRPin);
    if(num >= 400){
        Serial.println("Monday Take Medicine");
      }
    else{
      Serial.println("Monday Medicine Taken");
    }
    delay(100);
  }

  if(dow == 2){
    pinSelect(2);
    num = analogRead(IRPin);
    if(num >= 780){
        Serial.println("Tuesday Take Medicine");
      }
    else{
      Serial.println("Tuesday Medicine Taken");
    }
    delay(100);
  }

  if(dow == 3){
    pinSelect(3);
    num = analogRead(IRPin);
    if(num >= 400){
        Serial.println("Take Medicine");
      }
    else{
      Serial.println("Medicine Taken");
    }
    delay(100);
  }

  if(dow == 4){
    pinSelect(4);
    num = analogRead(IRPin);
    if(num >= 400){
        Serial.println("Take Medicine");
      }
    else{
      Serial.println("Medicine Taken");
    }
    delay(100);
  }

  if(dow == 5){
    pinSelect(5);
    num = analogRead(IRPin);
    if(num >= 400){
        Serial.println("Take Medicine");
      }
    else{
      Serial.println("Medicine Taken");
    }
    delay(100);
  }

  if(dow == 6){
    pinSelect(6);
    num = analogRead(IRPin);
    if(num >= 400){
        Serial.println("Take Medicine");
      }
    else{
      Serial.println("Medicine Taken");
    }
    delay(100);
  }
}
