# [Arduino-Smart-fish-tank](https://github.com/Paniti101/Arduino-Smart-fish-tank) 
ระบบ IoT ตรวจสอบคุณภาพน้ำภายในตู้ปลา โดยแสดงผลและควบคุมผ่านแอปพลิเคชั่น blynk โดยใช้บอร์ด ESP32 ควบคุมสั่งการ 

## สารบัญ
 - [อุปกรณ์และเซนเซอร์](#%E0%B8%AD%E0%B8%B8%E0%B8%9B%E0%B8%81%E0%B8%A3%E0%B8%93%E0%B9%8C%E0%B9%81%E0%B8%A5%E0%B8%B0%E0%B9%80%E0%B8%8B%E0%B8%99%E0%B9%80%E0%B8%8B%E0%B8%AD%E0%B8%A3%E0%B9%8C)
 - [การต่อวงจร](#%E0%B8%81%E0%B8%B2%E0%B8%A3%E0%B8%95%E0%B9%88%E0%B8%AD%E0%B8%A7%E0%B8%87%E0%B8%88%E0%B8%A3)
 - [App Blynk](#app-blynk)

## อุปกรณ์และเซนเซอร์

 - Node MCU ESP32
 - Relay
 - Turbidity Sensor
 - PH Sensor
 - Temperature Sensor
 - Power Supply
 - Lamp
 - Switch

## การต่อวงจร
#### Turbidity Sensor
[![Turbidity-sensor.png](https://i.postimg.cc/0N2s5czk/Turbidity-sensor.png)](https://postimg.cc/Sn55DLL3) 

#### Temperature Sensor
[![Temp-sensor.png](https://i.postimg.cc/9QXmzWSH/Temp-sensor.png)](https://postimg.cc/YhVKswMd)

#### PH Sensor
[![PH-sensor.png](https://i.postimg.cc/3JTpSvyj/PH-sensor.png)](https://postimg.cc/Wt5httwt)

#### Relay
[![Relay.png](https://i.postimg.cc/LXmKQvpK/Relay.png)](https://postimg.cc/v12NcrT3)

#### Lamp
[![Relay-lamp.png](https://i.postimg.cc/VN6HJTmN/Relay-lamp.png)](https://postimg.cc/JsfQgpyw)

#### Switch
[![Switch.png](https://i.postimg.cc/MKnTYZ50/Switch.png)](https://postimg.cc/ZCh4bS10)

#### วงจรทั้งหมด
[![All.png](https://i.postimg.cc/nz6HRB0v/All.png)](https://postimg.cc/TLJMhL8w)

|Sensor|Pin  |
|--|--|
|Temperature Sensor  | 25 |
|PH Sensor|33|
|Turbidity Sensor|32|
|Relay|26|
|Switch|14|

## App Blynk 
[![166266.jpg](https://i.postimg.cc/hvGKGnpt/166266.jpg)](https://postimg.cc/FdwMP2MM)
