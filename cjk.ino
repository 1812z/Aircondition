//必看！！！！
//原创代码 作者 bilibili @1812z 博客: 1812z.top 
//Github:https://github.com/1812z/Aircondition
//关于睡眠模式和节能模式添加： 先取消注释，然后在APP里编辑界面，添加两个按钮，键名分别是 BUTTON_8 和 BUTTON_9 ,设置为开关按钮 
//能力有限，代码不是最好的，有建议或反馈请留言
//高中党，时间有限，可能回复不及时
//视频教程看我b站投稿
#define BLINKER_WIFI
#define BLINKER_MIOT_AIRCONDITION
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "ESP8266WiFi.h"
//
char auth[] = "KEY";
char ssid[] = "WIFI名字";
char pswd[] = "WIFI密码";
//
#include <ir_Samsung.h>  //修改空调型号改这里，比如你的空调是格力，那么使用下面的，并把这个添加注释
//#include <ir_Gree.h> //左边是格力空调
//
//
//


#include <Blinker.h>
const uint16_t kIrLed = 14;

IRSamsungAc ac(kIrLed);     // Set the GPIO used for sending messages.

//wifi信号检测

BlinkerNumber TEXT("rssi");

#define Slider_1 "Slidert"//APP温度控制滑条
BlinkerSlider Slider1(Slider_1);
#define Slider_2 "Sliderf"//APP风速控制滑条
BlinkerSlider Slider2(Slider_2);

uint32_t read_time = 0;
#define ACSEND \
   ac.send();  \
   digitalWrite(kIrLed,HIGH); \
   digitalWrite(16,HIGH); \
   delay(800); \
   digitalWrite(16,LOW); 

bool oState = false;
bool hsState = false;
bool vsState = false;
bool ecoState = false;//节能
bool slState = false;//睡眠
uint8_t setLevel;
uint8_t setTemp;
void miotLevel(uint8_t level)
{
    BLINKER_LOG("need set level: ", level);
    // 0:AUTO MODE, 1-3 LEVEL
   switch (level)
    {
     case 0:
     ac.setFan(kSamsungAcFanAuto);
     ACSEND
     break;
     case 1:
     ac.setFan(kSamsungAcFanLow);
     ACSEND
     break;
      case 2:
     ac.setFan(kSamsungAcFanMed);
     ACSEND
     break;
      case 3:
     ac.setFan(kSamsungAcFanHigh);
     ACSEND
     break;
     default:
     break;
    }  
    BlinkerMIOT.level(level);
    BlinkerMIOT.print();
}

void miotTemp(uint8_t temp)
{
    BLINKER_LOG("need set temp: ", temp);
    setTemp = temp;    
    ac.setTemp(setTemp);
    ACSEND
    Slider1.print(temp);
    BlinkerMIOT.temp(temp);
    BlinkerMIOT.print();
}

//APP控制
BlinkerButton Button1("BUTTON_1");
BlinkerButton Button2("BUTTON_2");
//模式控制
BlinkerButton Button3("BUTTON_3");
BlinkerButton Button4("BUTTON_4");
BlinkerButton Button5("BUTTON_5");
BlinkerButton Button6("BUTTON_6");
BlinkerButton Button7("BUTTON_7");
////以下代码为节能模式开关 如果你的空调支持自行取消注释
//BlinkerButton Button8("BUTTON_8");

 ////以下代码为睡眠模式开关 如果你的空调支持自行取消注释
//BlinkerButton Button9("BUTTON_9");

//温度APP控制
void slider1_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);  
     ac.setTemp(value);
     ACSEND
     BlinkerMIOT.temp(value);
     BlinkerMIOT.print();
}
//风速控制
void slider2_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);  
    switch (value)
    {
     case 0:
     ac.setFan(kSamsungAcFanAuto);
     ACSEND
     break;
     case 1:
     ac.setFan(kSamsungAcFanLow);
     ACSEND
     break;
      case 2:
     ac.setFan(kSamsungAcFanMed);
     ACSEND
     break;
      case 3:
     ac.setFan(kSamsungAcFanHigh);
     ACSEND
     break;
      case 4:
     ac.setFan(kSamsungAcFanTurbo);
     ACSEND
     break;  
     default:
     break;
    }  
     BlinkerMIOT.level(value);
     BlinkerMIOT.print();
}
//求教
//模式APP控制
//初始化

void miotMode(const String & mode, const String & state)
{
    // eco ECO节能模式
    // anion 负离子
    // heater 辅热功能
    // dryer 干燥功能
    // sleep 睡眠模式
    // soft 柔风功能
    // uv UV杀菌
    // unsb un-straight-blowing 防直吹
    
    BLINKER_LOG("need set mode: ", mode, ", state:", state);

    BlinkerMIOT.mode(mode, state);
}

void miotHSwingState(const String & state)
{
    BLINKER_LOG("need set HSwing state: ", state);
    // horizontal-swing
    if (state == BLINKER_CMD_ON) {
        BlinkerMIOT.hswing("on");
        BlinkerMIOT.print();

        hsState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        BlinkerMIOT.hswing("off");
        BlinkerMIOT.print();

        hsState = false;
    }
}
void heartbeat()//心跳
{ 
    TEXT.print(WiFi.RSSI()); 
}
//APP控制开关
void button1_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");
        Button1.icon("icon_1");
        Button1.color("#FF0000");
        Button1.text("电源开");
        Button1.print("on");   
         ac.on();
        ACSEND
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");
        Button1.icon("icon_1");
        Button1.color("#FF0000");
        Button1.text("电源关");
        // Button1.text("Your button name", "describe");
        Button1.print("off");
         ac.off();
        ACSEND
    }
     else {
        BLINKER_LOG("Get user setting: ", state);

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("?");
        // Button1.text("Your button name", "describe");
        Button1.print();

}
}
//APP控制开关-扫风
void button2_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");
        Button2.icon("icon_1");
        Button2.color("#FF0000");
        Button2.text("扫风开");
        Button2.print("on");   
        ac.setSwing(true);
        ACSEND
        BlinkerMIOT.vswing("on");
        BlinkerMIOT.print();
         vsState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");
        Button2.icon("icon_1");
        Button2.color("#FF0000");
        Button2.text("扫风关");
        // Button1.text("Your button name", "describe");
        Button2.print("off");
        ac.setSwing(false);
        ACSEND
        BlinkerMIOT.vswing("off");
        BlinkerMIOT.print();
         vsState = false;
    }
     else {
        BLINKER_LOG("Get user setting: ", state);

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("?");
        // Button1.text("Your button name", "describe");
        Button1.print();

}
}
//APP控制-模式
void button3_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!");
        ac.setMode(kSamsungAcAuto);
        ACSEND
    }
}
void button4_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!");  
        ac.setMode(kSamsungAcCool);
        ACSEND
    }
}
 void button5_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!"); 
        ac.setMode(kSamsungAcDry);
        ACSEND
    }
}
void button6_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!");
        ac.setMode(kSamsungAcFan);
        ACSEND
    }
}
void button7_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!"); 
        ac.setMode(kSamsungAcHeat);
        ACSEND
    }
}  
//以下代码为节能模式开关 如果你的空调支持自行取消注释
//void button8_callback(const String & state)
// 
//   BLINKER_LOG("get button state: ", state);
//   if (state == BLINKER_CMD_ON) {
//       BLINKER_LOG("Toggle on!");
//       Button8.icon("icon_1");
//       Button8.text("节能开");
//       Button8.print("on");   
//       ac.eco(on);
//       ACSEND
//       BlinkerMIOT.eco("on");
//       BlinkerMIOT.print();
//        ecoState = true;
//   }
//   else if (state == BLINKER_CMD_OFF) {
//       BLINKER_LOG("Toggle off!");
//       Button8.icon("icon_1");
//       Button8.color("#FF0000");
//       Button8.text("节能关");
//       // Button1.text("Your button name", "describe");
//       Button8.print("off");
//       ac.eco(false);
//       ACSEND
//        BlinkerMIOT.eco("off");
//        BlinkerMIOT.print();
//         ecoState = false;
//   }
//}
//void button9_callback(const String & state)
//{
//void miotecoState(const String & state)
//{
//    BLINKER_LOG("need set eco state: ", state);
//
//    if (state == BLINKER_CMD_ON) {
//        ac.eco(on);
//        ACSEND
//        BlinkerMIOT.eco("on");
//        BlinkerMIOT.print();
//        ecoState = true;
//    }
//    else if (state == BLINKER_CMD_OFF) {
//        ac.eco(off);
//        ACSEND
//        BlinkerMIOT.eco("off");
//        BlinkerMIOT.print();
//        ecoState = false;
//    }
//}


//以下代码为睡眠模式开关 如果你的空调支持自行取消注释
//  BLINKER_LOG("get button state: ", state);
//    if (state == BLINKER_CMD_ON) {
//        BLINKER_LOG("Toggle on!");
//        Button9.icon("icon_1");
//        Button9.text("睡眠开");
//        Button9.print("on");   
//        ac.sleep(on);
//        ACSEND
//        BlinkerMIOT.sleep("on");
//        BlinkerMIOT.print();
//        slState = true;
//    }
//    else if (state == BLINKER_CMD_OFF) {
//        BLINKER_LOG("Toggle off!");
//        Button9.icon("icon_1");
//        Button9.color("#FF0000");
//        Button9.text("睡眠关");
//        // Button1.text("Your button name", "describe");
//        Button9.print("off");
 //       ac.sleep(false);
//        ACSEND
//        BlinkerMIOT.sleep("off");
//        BlinkerMIOT.print();
//         slState = false;
//    }
//}
//void miotsleepState(const String & state)
//{
//    BLINKER_LOG("need set sleep state: ", state);
//
//    if (state == BLINKER_CMD_ON) {
//        ac.sleep(on);
//        ACSEND
//        BlinkerMIOT.sleep("on");
//        BlinkerMIOT.print();
//        slState = true;
//    }
//    else if (state == BLINKER_CMD_OFF) {
//        ac.sleep(off);
//        ACSEND
//        BlinkerMIOT.sleep("off");
//        BlinkerMIOT.print();
//        slState = false;
//    }
//}


void miotVSwingState(const String & state)
{
    BLINKER_LOG("need set VSwing state: ", state);
    // vertical-swing

    if (state == BLINKER_CMD_ON) {
        BlinkerMIOT.vswing("on");
        BlinkerMIOT.print();
        ac.setSwing(true);
        ACSEND
        vsState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        BlinkerMIOT.vswing("off");
        BlinkerMIOT.print();
        ac.setSwing(false);
        ACSEND
        vsState = false;
    }
}
//小爱电源控制
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        ac.on();
        ACSEND
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        ac.off();
        ACSEND
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
        oState = false;
    }
}

void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.hswing(hsState ? "on" : "off");
            BlinkerMIOT.vswing(vsState ? "on" : "off");
            BlinkerMIOT.eco("off");
            BlinkerMIOT.anion("off");
            BlinkerMIOT.heater("off");
            BlinkerMIOT.dryer("off");
            BlinkerMIOT.sleep(slState ? "on" : "off");
            BlinkerMIOT.soft("off");
            BlinkerMIOT.uv("off");
            BlinkerMIOT.unStraightBlow("off");
            BlinkerMIOT.level(setLevel);
            BlinkerMIOT.temp(setTemp);       
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.hswing(hsState ? "on" : "off");
            BlinkerMIOT.vswing(vsState ? "on" : "off");
            BlinkerMIOT.eco("off");
            BlinkerMIOT.anion("off");
            BlinkerMIOT.heater("off");
            BlinkerMIOT.dryer("off");
            BlinkerMIOT.sleep("off");
            BlinkerMIOT.soft("off");
            BlinkerMIOT.uv("off");
            BlinkerMIOT.unStraightBlow("off");
            BlinkerMIOT.level(setLevel);
            BlinkerMIOT.temp(setTemp);  
            BlinkerMIOT.print();
            break;
    }
}

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}

void setup()
{
  // 
     ac.begin();
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
//APP控制
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    //模式callback
    Button3.attach(button3_callback);
    Button4.attach(button4_callback);
    Button5.attach(button5_callback);
    Button6.attach(button6_callback);
    Button7.attach(button7_callback);
    ////以下代码为节能模式开关 如果你的空调支持自行取消注释
   // Button8.attach(button8_callback);//节能
   // ////以下代码为睡眠模式开关 如果你的空调支持自行取消注释
   // Button9.attach(button9_callback);//睡眠
    //
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachHSwing(miotHSwingState);
    BlinkerMIOT.attachVSwing(miotVSwingState);
    BlinkerMIOT.attachLevel(miotLevel);
    BlinkerMIOT.attachMode(miotMode);
    BlinkerMIOT.attachTemp(miotTemp);
    BlinkerMIOT.attachQuery(miotQuery);
    Slider1.attach(slider1_callback);//温度
    Slider2.attach(slider2_callback);//风速
     pinMode(kIrLed,OUTPUT);
    digitalWrite(kIrLed,HIGH);
     pinMode(16,OUTPUT);
    digitalWrite(16,LOW);
}

void loop()
{

    Blinker.run();
}
