#define BLYNK_TEMPLATE_ID "TMPL6nYGeT0XO"
#define BLYNK_TEMPLATE_NAME "Project end"
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>      //ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <ESP_Line_Notify.h>
#include <TridentTD_LineNotify.h>
#include <ENC28J60lwIP.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

Servo servo1;

#define TdsSensorPin A0               //ขา sensor Tds
#define VREF 5.0                      //set ค่าไฟฟ้า 5v
#define SCOUNT 30
int analogBuffer[SCOUNT];
int analogBufferIndex = 0;
float tdsValue = 0, temperature = 25;

char auth[] = "NtqAPq9iHCuQFAVI408twIErdRcGfgGh";     //Tokenใน Blynk app
char ssid[] = "realme X2 Pro";
char pass[] = "123456789"; 

//Set Time Server
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 25200;                                    // setTime TH 7 ชั่วโมง * 3600 วินาที
NTPClient timeClient(ntpUDP, "0.th.pool.ntp.org", utcOffsetInSeconds);

#define LINE_TOKEN "lyKfoRLTwoi4UFIVcpFA63TrCKYN4TwVfgWQodKa7Vj"          //Token Line   Textheadได้เวลาเปลี่ยนน้ำ:

#define Relay1 D3         //ต่อขา relay ไฟตู้
#define Relay3 D1         //ต่อขา relay ปั๊มน้ำเข้า
#define Relay4 D2         //ต่อขา relay ปั๊มน้ำออก
#define servopin1 D4      //ต่อขา servo 
#define Liquid_level D8   //ต่อขา Warter sensor_Liquid_level
#define Liquid_level_2 D9 //ต่อขา Warter sensor_Liquid_level_2


WidgetLED Relay1Blynk(V5);  //led app ไฟตู้
WidgetLED Relay3Blynk(V6);  //led app น้ำเข้า  
WidgetLED Relay4Blynk(V7);  //led app น้ำออก
WidgetLED Servo1Blynk(V9);  //led app อาหารปลา
WidgetLCD LCD(V4);          //lcd app	

int Btnpin = D5;            //Btn ปั๊มน้ำ
int Btnpin2 = D6;           //Btn หลอดไฟ 
int val = 0;                

void setup() {
  Serial.begin(115200);
  pinMode(TdsSensorPin, INPUT);     //Serson TDS
  pinMode(Btnpin,INPUT);            //Btn ปั๊มน้ำ
  pinMode(Btnpin2,INPUT);           //Btn หลอดไฟ
  pinMode(Relay3,OUTPUT);           //Relay ปั๊มน้ำ in
  pinMode(Relay4,OUTPUT);           //Relay ปั๊มน้ำ out
    digitalWrite(Relay3,1);         //Relay ยังไม่ทำงานเมื่อจ่ายไฟ 
    digitalWrite(Relay4,1);         //Relay ยังไม่ทำงานเมื่อจ่ายไฟ
  pinMode(Relay1,OUTPUT);           //Relay ไฟตู้
    digitalWrite(Relay1,1);         //Relay ยังไม่ทำงาน HIGH

  pinMode(Liquid_level, INPUT);

  servo1.attach(servopin1);          //Servo
  timeClient.update();

  timeClient.begin();               //เชื่อมต่อกับ NTPClient
  Blynk.begin(auth, ssid, pass);    //เชื่อมต่อกับ Blynk
  LINE.setToken(LINE_TOKEN);        //เชื่อกับ Line

  //ตั้งเวลาแจ้งเตือนทุกวัน เมื่อค่า TDS ตรงตามเงื่อนไข
  Alarm.alarmRepeat(10, 0, 0, OnTDSValueReached);  // ตั้งเวลาแจ้งเตือนที่ 10:00 ทุกวัน
  Alarm.alarmRepeat(10, 0, 0, OnTDSValueReached_dangerous); // ตั้งเวลาแจ้งเตือนที่ 10:00 ทุกวัน   ppm=1000
  Alarm.alarmRepeat(18, 0, 0, OnTDSValueReached_dangerous); // ตั้งเวลาแจ้งเตือนที่ 18:00 ทุกวัน   ppm=1000

/*TEST*/ //ตั้งเวลาแจ้งเตือนทุกวัน เมื่อค่า TDS ตรงตามเงื่อนไข
  Alarm.alarmRepeat(16, 30, 0, OnTDSValueReached); // ตั้งเวลาแจ้งเตือนที่ 16:00 ทุกวัน 
  Alarm.alarmRepeat(16, 30, 0, OnTDSValueReached_dangerous); // ตั้งเวลาแจ้งเตือนที่ 16:00 ทุกวัน 
/*TEST*/

  setSyncInterval(86400); // กำหนดให้ปรับเวลาหนึ่งครั้งต่อวัน (ครอบคลุมทั้ง 24 ชั่วโมง)
}

void loop() {
  Blynk.run();
  timeClient.update();  // อัปเดตเวลาจากเซิร์ฟเวอร์ NTP

  val = digitalRead(Liquid_level);            //สร้างตัวแปรเก็บ ระดับน้ำ 
  int button = digitalRead(Btnpin); 
  int button2 = digitalRead(Btnpin2);

  // if(button == HIGH){                     //ปั๊มน้ำทำงานโดย SW
  //   pumpWater();
  // }

  // if(button2 == HIGH){                    //หลอดไฟทำงานโดย SW
  //   onlamp();
  // }

//Warter sensor_Liquid_level
  // Serial.println(val);
  // if (val > 100){
  //   // digitalWrite(led, LOW);
  //   // digitalWrite(buzzer, HIGH);
  //   Serial.println("No Water");
  // }
  // if (val < 100){
  //   // digitalWrite(led, HIGH);
  //   // digitalWrite(buzzer, LOW);
  //   Serial.println("Detected Water");
  // }

  // Serial.println(timeClient.getFormattedTime());     //ตัวตรวจสอบ Time server 
  // delay(1000);

  static unsigned long analogSampleTimepoint = millis();
  static unsigned long printTimepoint = millis();
  
  // Blynk.virtualWrite(V3, "ค่า :    " ,tdsValue, "    ppm");    //นำค่าtdsValueมาแสดงที่ Labeled Value ใน Blynk App
  Blynk.virtualWrite(V8,tdsValue);                               //นำค่าtdsValueมาแสดงที่ Gauge  ใน Blynk App

//Sensor TDS
  if (millis() - analogSampleTimepoint > 40U) {                 //การหน่วงเวลา ms
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);
    analogBufferIndex = (analogBufferIndex + 1) % SCOUNT;
  } 

  if (millis() - printTimepoint > 800U) {                       //การหน่วงเวลา ms
    printTimepoint = millis();
    float averageVoltage = getAverage(analogBuffer, SCOUNT) * VREF / 1024.0;        
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);            //ค่าความต่างของอุณหภูมิ
    float compensationVoltage = averageVoltage / compensationCoefficient;
    tdsValue = (133.42 * pow(compensationVoltage, 3) - 255.86 * pow(compensationVoltage, 2) + 857.39 * compensationVoltage) * 0.5;
    
    Serial.print("TDS Value: ");
    Serial.print(tdsValue, 0);
    Serial.println(" ppm");
  }

  if(tdsValue >= 600){
    LCD.clear();
    LCD.print(0,0,"Turbid : Change");       
    LCD.print(0,1,"The Water");
  } else if (tdsValue <= 590){
    LCD.clear();
    LCD.print(0,0,"Not : Turbid");
    LCD.print(0,1,"Not : Turbid");
  }
  Alarm.delay(1000);   //ให้ TimeAlarms ทำงานและตรวจสอบเวลาแจ้งเตือน

//Set Time Alarm
  if (timeClient.getHours() == 10 && timeClient.getMinutes() == 0 && timeClient.getSeconds() == 0) {
    OnTDSValueReached();
  } else if (timeClient.getHours() == 10 && timeClient.getMinutes() == 0 && timeClient.getSeconds() == 0) {
    OnTDSValueReached_dangerous();
  } else if (timeClient.getHours() == 18 && timeClient.getMinutes() == 0 && timeClient.getSeconds() == 0) {
    OnTDSValueReached_dangerous();
  }

  /*TEST*/ //Set Time Alarm
  if (timeClient.getHours() == 16 && timeClient.getMinutes() == 30 && timeClient.getSeconds() == 0) {
    OnTDSValueReached();              //คุณภาพปานกลาง
  } else if (timeClient.getHours() == 16 && timeClient.getMinutes() == 30 && timeClient.getSeconds() == 0) {
    OnTDSValueReached_dangerous();    //คุณภาพแย่
  }
  /*TEST*/
  
}

float getAverage(int data[], int count) {             // ฟังก์ชัน Sum getAverage 
  float sum = 0;
  for (int i = 0; i < count; i++) {
    sum += data[i];
  }
  return sum / count;
}

//แจ้งเตือน LINE 
/*TEST*/ 
void OnTDSValueReached() {
  if (tdsValue >= 5 && tdsValue < 30) {                //คุณภาพน้ำเกิน 600 ppm
    LINE.notify("TDS Value เกิน 30 แล้ว ควรเปลี่ยนน้ำ");
  } else if (tdsValue >= 30 && tdsValue < 50) {         //คุณภาพน้ำเกิน 800 ppm
    LINE.notify("TDS Value เกิน 50 แล้ว ควรเปลี่ยนน้ำ");
  }
}
void OnTDSValueReached_dangerous(){
  if (tdsValue >= 70) {                                //คุณภาพน้ำเกิน 1000 ppm
    LINE.notify("TDS Value เกิน 70 แล้ว ควรเปลี่ยนน้ำ");
  } 
}
/*TEST*/

//แจ้งเตือน LINE 
// void OnTDSValueReached() {
//   if (tdsValue >= 600 && tdsValue < 800) {                //คุณภาพน้ำเกิน 600 ppm
//     LINE.notify("TDS Value เกิน 600 แล้ว ควรเปลี่ยนน้ำ");
//   } else if (tdsValue >= 800 && tdsValue < 900) {         //คุณภาพน้ำเกิน 800 ppm
//     LINE.notify("TDS Value เกิน 800 แล้ว ควรเปลี่ยนน้ำ");
//   }
// }

// void OnTDSValueReached_dangerous(){
//   if (tdsValue >= 1000) {                                //คุณภาพน้ำเกิน 1000 ppm
//     LINE.notify("TDS Value เกิน 1000 แล้ว ควรเปลี่ยนน้ำ");
//   } 
// }

//ปั๊มน้ำ BLYNK Button
  BLYNK_WRITE(V2){                      //ควบคุมrelayปั๊มดูดน้ำออก
  Serial.println(param.asInt()); 
    if (param.asInt()){ 
        digitalWrite(Relay4, LOW);      //ปั๊มดูดน้ำออกทำงาน
        Relay3Blynk.on();
        LINE.notify("เริ่มต้นการเปลี่ยนถ่ายน้ำ");
        delay(5000UL);                 //หน่วงเวลาดูดน้ำออก50วิ 
    } else {
        digitalWrite(Relay4, HIGH); 
        Relay3Blynk.off();
        digitalWrite(Relay3, LOW);      //ปั๊มดูดน้ำเข้าทำงาน
        Relay4Blynk.on();
        delay(5000UL);                  //หน่วงเวลาดูดน้ำออก50วิ   
        digitalWrite(Relay3, HIGH);     //ปั๊มดูดน้ำเข้าไม่ทำงาน
        Relay4Blynk.off(); 
        LINE.notify("การเปลี่ยนถ่ายน้ำเสร็จสิ้น");
    }
  }

//ปั๊มน้ำ SW
void pumpWater()
{
  Serial.println("ทำงาน");            //เช็คการทำงาน Button
    digitalWrite(Relay4, LOW);        //ปั๊มดูดน้ำออกทำงาน
    Relay3Blynk.on();
    LINE.notify("เริ่มต้นการเปลี่ยนถ่ายน้ำ");
    delay(5000UL);                    //หน่วงเวลาดูดน้ำออก50วิ ในบิ้งเป็น Push Switch

  Serial.println("ทำงาน2");          //เช็คการทำงาน Button
    digitalWrite(Relay4, HIGH); 
    Relay3Blynk.off();
    digitalWrite(Relay3, LOW);        //ปั๊มดูดน้ำเข้าทำงาน
    Relay4Blynk.on();
    delay(5000UL);                    //หน่วงเวลาดูดน้ำออก50วิ ในบิ้งเป็น Push Switch (50000UL)

    digitalWrite(Relay3, HIGH);     //ปั๊มดูดน้ำเข้าไม่ทำงาน
    Relay4Blynk.off(); 
    LINE.notify("การเปลี่ยนถ่ายน้ำเสร็จสิ้น");
  }

//ไฟตู้ปลา BLYNK Button
  BLYNK_WRITE(V0){                      //ควบคุม Relay2 ไฟตู้ปลา
  Serial.println(param.asInt());        //แสดงออกทางหน้าจอserial monitor (1,0)
    if (param.asInt()){                 //ตรวจสอบการกดถ้ากด ให้ทำการเปิดไฟ
        digitalWrite(Relay1, LOW);      //ไม่มีไฟ = ทำงาน low to HIGH ralay no = no , nc = off
        Relay1Blynk.on();               //สั่งให้ Blynk Widget LED ไฟติด
    } else {
        digitalWrite(Relay1, HIGH);     //ตรวจสอบการกดถ้าไม่กด ให้ทำการปิดไฟ
         Relay1Blynk.off();             //สั่งให้ Blynk Widget LED ไฟดับ
    }
  }

//ไฟตู้ปลา SW
void onlamp()
{
  Serial.println("ทำงานไฟ");                   //เช็คการทำงาน Button
    digitalWrite(Relay1, LOW);                 // ถ้าปิดอยู่ ให้เปิดไฟตู้ปลา
    Relay1Blynk.on();                          // เปิด Widget LED ในแอป Blynk
  Serial.println("เปิดทำงานไฟ");              // แสดงข้อความเมื่อเปิดไฟตู้ปลา
    delay(500);

    digitalWrite(Relay1, HIGH);                // ถ้าเปิดอยู่ ให้ปิดไฟตู้ปลา
    Relay1Blynk.off();                         // ปิด Widget LED ในแอป Blynk
  Serial.println("ปิดทำงานไฟ");              // แสดงข้อความเมื่อปิดไฟตู้ปลา
}

//ให้อาหารปลา BLYNK Button
  BLYNK_WRITE(V1){                      //ควบคุมservoอาหารปลา
  Serial.println(param.asInt()); 
    if (param.asInt()){ 
        // delay(10);
        servo1.write(180);                //Servo 0 องศา
        Servo1Blynk.on();               ////led app blynk อาหารปลา on
    } else {
        delay(1000); 
        servo1.write(0);               //Servo 90 องศา
        Servo1Blynk.off();              //led app blynk อาหารปลา off
    }
  }

//ตั้งเวลาให้อาหารปลา BLYNK Time Input
  BLYNK_WRITE(V3) {
  TimeInputParam t(param);
    if (t.hasStartTime()) {
      int startHour = t.getStartHour();
      int startMinute = t.getStartMinute();
      Serial.print("เวลาที่กำหนด: ");
      Serial.print(startHour);
      Serial.print(":");
      Serial.println(startMinute);
    
    // ตั้งเวลาเริ่มต้นเพื่อเปิดอุปกรณ์ (ในกรณีนี้คือ Relay)
    if (startHour == timeClient.getHours() && startMinute == timeClient.getMinutes()) {
        servo1.write(180);                //Servo 0 องศา 
        Servo1Blynk.on();
        delay(1000); 
        servo1.write(0);               //Servo 90 องศา
        Servo1Blynk.off();
    }
  }
}


