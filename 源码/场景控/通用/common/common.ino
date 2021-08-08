//必看！！！！
//原创代码 作者 bilibili @1812z 博客: 1812z.top 
//Github:https://github.com/1812z/Aircondition
//能力有限，代码不是最好的，有建议或反馈请留言
//高中党，时间有限，可能回复不及时
//视频教程看我b站投稿
#define BLINKER_WIFI
#define BLINKER_MIOT_AIRCONDITION
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>
#include "ESP8266WiFi.h"
//
int firm = 46;  //型号改这里  型号对照图片https://img2.moeblog.vip/images/Zx9t.png 来自arduino论坛
char auth[] = "KEY";
char ssid[] = "wifi名称";
char pswd[] = "密码";
//
#include <Blinker.h>
const uint16_t kIrLed = 14;

IRac ac(kIrLed);     // Set the GPIO used for sending messages.

#define ACSEND \
   ac.sendAc(); \ 
   digitalWrite(kIrLed,HIGH); \
   digitalWrite(16,HIGH); \
   delay(100); \
   digitalWrite(16,LOW); 

   
//wifi信号检测
BlinkerNumber TEXT("rssi");

#define Slider_1 "Slidert"//APP温度控制滑条
BlinkerSlider Slider1(Slider_1);
#define Slider_2 "Sliderf"//APP风速控制滑条
BlinkerSlider Slider2(Slider_2);


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
     ac.next.fanspeed = stdAc::fanspeed_t::kAuto;
     ACSEND
     break;
     case 1:
     ac.next.fanspeed = stdAc::fanspeed_t::kLow;
     ACSEND
     break;
      case 2:
    ac.next.fanspeed = stdAc::fanspeed_t::kMedium;
     ACSEND
     break;
      case 3:
     ac.next.fanspeed = stdAc::fanspeed_t::kHigh;
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
    ac.next.degrees = setTemp; 
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
////以下代码为节能模式开关 
BlinkerButton Button8("BUTTON_8");
 ////以下代码为睡眠模式开关 
BlinkerButton Button9("BUTTON_9");
BlinkerButton Button10("BUTTON_10"); //超强
//温度APP控制
void slider1_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);  
     ac.next.degrees = value;  
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
     ac.next.fanspeed = stdAc::fanspeed_t::kAuto;
     ACSEND
     break;
     case 1:
     ac.next.fanspeed = stdAc::fanspeed_t::kLow;
     ACSEND
     break;
      case 2:
    ac.next.fanspeed = stdAc::fanspeed_t::kMedium;
     ACSEND
     break;
      case 3:
    ac.next.fanspeed = stdAc::fanspeed_t::kHigh;
     ACSEND
     break;
      case 4:
    ac.next.fanspeed = stdAc::fanspeed_t::kMax;
     ACSEND
     break;  
     default:
     break;
    }  
     BlinkerMIOT.level(value);
     BlinkerMIOT.print();
}

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
          ac.next.power = true;
        ACSEND
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");
        Button1.icon("icon_1");
        Button1.color("#FF0000");
        Button1.text("电源关");
        // Button1.text("Your button name", "describe");
        Button1.print("off");
          ac.next.power = false;
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
        ac.next.swingv = stdAc::swingv_t::kOff;
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
        ac.next.mode = stdAc::opmode_t::kAuto;
        ACSEND
    }
}
void button4_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!");  
        ac.next.mode = stdAc::opmode_t::kCool;
        ACSEND
    }
}
 void button5_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!"); 
       ac.next.mode = stdAc::opmode_t::kDry;
        ACSEND
    }
}
void button6_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!");
        ac.next.mode = stdAc::opmode_t::kFan;
        ACSEND
    }
}
void button7_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!"); 
       ac.next.mode = stdAc::opmode_t::kHeat;
        ACSEND
    }
}  
//以下代码为节能模式开关 
void button8_callback(const String & state)
 {
   BLINKER_LOG("get button state: ", state);
   if (state == BLINKER_CMD_ON) {
       BLINKER_LOG("Toggle on!");
       Button8.icon("icon_1");
      Button8.text("节能开");
       Button8.print("on");   
      ac.next.econo = true;
       ACSEND
       BlinkerMIOT.eco("on");
       BlinkerMIOT.print();
        ecoState = true;
   }
   else if (state == BLINKER_CMD_OFF) {
       BLINKER_LOG("Toggle off!");
      Button8.icon("icon_1");
       Button8.color("#FF0000");
       Button8.text("节能关");
       // Button1.text("Your button name", "describe");
       Button8.print("off");
      ac.next.econo = false;
       ACSEND
        BlinkerMIOT.eco("off");
        BlinkerMIOT.print();
         ecoState = false;
   }
}
void button9_callback(const String & state)
{

//以下代码为睡眠模式开关 如果你的空调支持自行取消注释
  BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");
        Button9.icon("icon_1");
        Button9.text("睡眠开");
        Button9.print("on");   
        ac.next.quiet = true;
        ACSEND
        BlinkerMIOT.sleep("on");
        BlinkerMIOT.print();
        slState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");
        Button9.icon("icon_1");
        Button9.color("#FF0000");
        Button9.text("睡眠关");
        // Button1.text("Your button name", "describe");
        Button9.print("off");
        ac.next.quiet = false;
       ACSEND
        BlinkerMIOT.sleep("off");
        BlinkerMIOT.print();
         slState = false;
    }
}
void button10_callback(const String & state)
{

//以下代码为超强开关
  BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");
        Button10.icon("icon_1");
        Button10.text("超强开");
        Button10.print("on");   
       ac.next.turbo = true;
        ACSEND
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");
        Button10.icon("icon_1");
        Button10.color("#FF0000");
        Button10.text("超强关");
        // Button1.text("Your button name", "describe");
        Button10.print("off");
       ac.next.turbo = false;
       ACSEND
    }
}
void miotsleepState(const String & state)
{
    BLINKER_LOG("need set sleep state: ", state);

    if (state == BLINKER_CMD_ON) {
         ac.next.quiet = true;
        ACSEND
        BlinkerMIOT.sleep("on");
        BlinkerMIOT.print();
        slState = true;
   }
    else if (state == BLINKER_CMD_OFF) {
        ac.next.quiet = false;
        ACSEND
        BlinkerMIOT.sleep("off");
        BlinkerMIOT.print();
        slState = false;
    }
}


void miotVSwingState(const String & state)
{
    BLINKER_LOG("need set VSwing state: ", state);
    // vertical-swing

    if (state == BLINKER_CMD_ON) {
        BlinkerMIOT.vswing("on");
        BlinkerMIOT.print();
        ACSEND
        vsState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        BlinkerMIOT.vswing("off");
        BlinkerMIOT.print();
        ac.next.swingv = stdAc::swingv_t::kOff;
        ACSEND
        vsState = false;
    }
}
//小爱电源控制
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
         ac.next.power = true;
        ACSEND
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
         ac.next.power = false;
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
  // AC Common 配置
    // Set up what we want to send.
  // See state_t, opmode_t, fanspeed_t, swingv_t, & swingh_t in IRsend.h for
  // all the various options.
  decode_type_t protocol = (decode_type_t)firm;
  ac.next.protocol = protocol;  
  ac.next.model = 1;  // Some A/Cs have different models. Try just the first.
  ac.next.mode = stdAc::opmode_t::kCool;  // Run in cool mode initially.
  ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
  ac.next.degrees = 25;  //默认温度
  ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
  ac.next.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
  ac.next.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left or right.
  ac.next.light = false;  // Turn off any LED/Lights/Display that we can.
  ac.next.beep = false;  // Turn off any beep from the A/C if we can.
  ac.next.econo = false;  // Turn off any economy modes if we can.
  ac.next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  ac.next.turbo = false;  // Don't use any turbo/powerful/etc modes.
  ac.next.quiet = false;  // Don't use any quiet/silent/etc modes.
  ac.next.sleep = -1;  // Don't set any sleep time or modes.
  ac.next.clean = false;  // Turn off any Cleaning options if we can.
  ac.next.clock = -1;  // Don't set any current time if we can avoid it.
  ac.next.power = false;  // Initially start with the unit off.
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
    Button8.attach(button8_callback);//节能
   // ////以下代码为睡眠模式开关 如果你的空调支持自行取消注释
    Button9.attach(button9_callback);//睡眠
     Button10.attach(button10_callback);//超强
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
