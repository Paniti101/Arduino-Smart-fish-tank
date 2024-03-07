#define BLYNK_TEMPLATE_ID           "TMPL6C4f2dZkk"
#define BLYNK_TEMPLATE_NAME         "Water quality monitor"
#define BLYNK_PRINT Serial

char auth[] = "Rkx3aai3_BAILgRFAX2VQG0PN9mug5q8";      //Tokenใน Blynk app B
// char ssid[] = "TH_Toey";
// char pass[] = "0859322075";

char ssid[] = "realme X2 Pro";
char pass[] = "123456789"; 

// char ssid[] = "Joe_2.4G";
// char pass[] = "0909629307";

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>     //ESP32
#include <DallasTemperature.h>    //Temp
#include <OneWire.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <TridentTD_LineNotify.h>  

//Pin
#define Relay1          14    //Relay 1  หลอดไฟ
#define Turbidity       32    //Sensor Turbidity         
#define Temp            25    //Sensor_Temp  
#define TDS             3     //Sensor_TDS        ##ยังไม่ได้ต้องไฟ
#define LED             27    //LED
#define button          26    //Button
#define LINE_TOKEN "YP8m9FG29aym0sPMuOyWR9rNglcctc1VsE4tz2g5EdH"//"5unQdgWO0rpyBVRZK0fDFqxPaHq7xexxHKIIZYEaTPF"  //TOKEN Line

//Blynk pin

//pin 34,35 ใช้ไม่ได้ / 32,33,25,26,27

OneWire oneWire(Temp);
DallasTemperature sensors(&oneWire);
BlynkTimer timer;

// float volt;
// float ntu;


//Set Time Server
// WiFiUDP ntpUDP;
// const long utcOffsetInSeconds = 25200;    // setTime TH 7 ชั่วโมง * 3600 วินาที
// NTPClient timeClient(ntpUDP, "0.th.pool.ntp.org", utcOffsetInSeconds);

//Set ตัวแปร
float temperatureC;  
float PH_Value;
float Turbidity_Value;
float TDS_Value;

int relayState = HIGH;
unsigned long lastButtonPressTime = 0;  // เก็บเวลาล่าสุดที่ปุ่มถูกกด
const int debounceDelay = 1000;         // เวลาหน่วงในการป้องกันการกดซ้ำ
int sensorValue = 0;

// PH----------------------------------------------------------------------------------------------
float calibration_value = 21.34; //20.24 - 0.7
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

// Google Sheet----------------------------------------------------------------------------------------
String GAS_ID = "AKfycbyCxuXn26rcn3aUjX0OFdjSuMPqeg7da2hDIFNptYl2iAUDz_VKt_ZOwBpu9HXXSSOS"; 
const char* host = "script.google.com";
#define UPDATE_INTERVAL_HOUR  (0)
#define UPDATE_INTERVAL_MIN   (0)
#define UPDATE_INTERVAL_SEC   (30)
#define UPDATE_INTERVAL_MS    ( ((UPDATE_INTERVAL_HOUR*60*60) + (UPDATE_INTERVAL_MIN * 60) + UPDATE_INTERVAL_SEC ) * 1000 )

void update_google_sheet()
{
    Serial.print("connecting to ");
    Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClientSecure client;
    const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
    
    client.setInsecure(); // this is the magical line that makes everything work
    
    if (!client.connect(host, httpPort)) { //works!
      Serial.println("connection failed");
      return;
    }
       
    //----------------------------------------Processing data and sending data
    String url = "/macros/s/" + GAS_ID + "/exec?Temperature=";
   
    url += String(temperatureC);
    url += "&Turbidity=";
    url += String(TDS_Value);
    url += "&PH=";
    url += String(PH_Value);
    url += "&TDS=";
    url += String(Turbidity_Value);
    
    Serial.print("Requesting URL: ");
    Serial.println(url);
  
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
  
    Serial.println();
    Serial.println("closing connection");  
}

// ----------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  sensors.begin();
  Serial.println("Start Project");

  pinMode(Relay1, OUTPUT);             //Lamp
    // digitalWrite(Relay1, HIGH);

  pinMode(button, INPUT_PULLUP);      //Button Lamp
  // pinMode(button, INPUT);

  pinMode(LED, OUTPUT);               //LED
    // digitalWrite(LED, HIGH);

  pinMode(Turbidity,INPUT);             
  pinMode(Temp,INPUT);
  // pinMode(33,INPUT);
  pinMode(TDS,INPUT);

  LINE.setToken(LINE_TOKEN);
  Blynk.begin(auth, ssid, pass);    //เชื่อมต่อกับ Blynk
  // timer.setInterval(1000L, checkButton); // ตั้งค่าตัวตรวจสอบปุ่มทุก 1 วินาที
}

// ----------------------------------------------------------------------------------------------
unsigned long time_ms;
unsigned long time_1000_ms_buf;
unsigned long time_sheet_update_buf;
unsigned long time_dif;

void loop() {
  Blynk.run();
  timer.run();

  time_ms = millis();
  time_dif = time_ms - time_1000_ms_buf;

//LED
  digitalWrite(LED, HIGH); // Turn on the LED

//Temp
  sensors.requestTemperatures();    //DallasTemperature
  temperatureC = sensors.getTempCByIndex(0);  
  Serial.print("Temperature is: ");
  Serial.print(temperatureC);       //แสดงค่า อุณหภูมิ 
  Serial.println(" °C");
  
// PH
  for(int i=0;i<10;i++) {
    buffer_arr[i]=analogRead(33);
    delay(30);
  }
    for(int i=0;i<9;i++)
  {
      for(int j=i+1;j<10;j++)
    {
      if(buffer_arr[i]>buffer_arr[j])
      {  
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
      }
    }
 }
  avgval=0;
  for(int i=2;i<8;i++)
    avgval+=buffer_arr[i];
  float volt=(float)avgval*3.3/4096.0/6;  
 //Serial.print("Voltage: ");
 //Serial.println(volt);
  PH_Value = -5.70 * volt + calibration_value;

 Serial.print("pH Val: ");
 Serial.println(PH_Value);

//Turbidity
  int sensorValue = analogRead(Turbidity);
  Turbidity_Value = map(sensorValue, 0, 980, 100, 0);        //(sensorValue, 0,4095, 100, 0);
  Serial.print("sensorTurbidity = ");
  Serial.println(Turbidity_Value);

// LINE.notify("Temp sensor disconnected!");

  delay(1500);

//Button
  int buttonStates = digitalRead(button); // Read the state of the button

  if (buttonStates == LOW) { 
    digitalWrite(Relay1, LOW); // Turn on the relay
    Serial.println("เปิด Relay");
  } else {
    digitalWrite(Relay1, HIGH); // Turn off the relay
    Serial.println("ปิด Relay");
  }

//Blynk APP
  Blynk.virtualWrite(V3,Turbidity_Value);    //นำค่าtdsValueมาแสดงที่ Gauge  ใน Blynk App
  Blynk.virtualWrite(V4,PH_Value);           //นำค่าPHมาแสดง Gauge ใน Blynk App
  Blynk.virtualWrite(V5,temperatureC);       //นำค่าTempมาแสดง Labeled ใน Blynk App

  // Update data to google sheet in specific period
  time_ms = millis();
  time_dif = time_ms - time_sheet_update_buf;  
  if ( time_dif >= UPDATE_INTERVAL_MS ) // Specific period
  {
    time_sheet_update_buf = time_ms;
    update_google_sheet();
  }

}

BLYNK_WRITE(V2) {
  int buttonState = param.asInt();
  
  if (buttonState) {
    // ถ้าปุ่มถูกกด (buttonState == HIGH)
    digitalWrite(Relay1, LOW);
    relayState = LOW;
    Serial.println("เปิด Relay");
  } else {
    // ถ้าปุ่มถูกปล่อย (buttonState == LOW)
    digitalWrite(Relay1, HIGH);
    relayState = HIGH;
    Serial.println("ปิด Relay");
  }
}

//-----------------------------------------------------------------------------------------

//Line notfy//YP8m9FG29aym0sPMuOyWR9rNglcctc1VsE4tz2g5EdH

//Line แจ้งเตือนอุณหภูมิเกินที่มีผลต่อปลา

// Check the status of Temp pin (23)
  // int tempStatus = digitalRead(Temp);
  // Serial.print("Temp Status: ");
  // Serial.println(tempStatus);
  // if (tempStatus == LOW) {
  //   Serial.println("Temp sensor disconnected. Sending notification...");  
  //   LINE.notify("Temp sensor disconnected!"); 
  // }