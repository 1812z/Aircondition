//必看！！！！
//半原创代码 作者 bilibili @1812z 博客: 1812z.top 
//Github:https://github.com/1812z/Aircondition
//关于睡眠模式和节能模式：大概率不支持，如果要添加看下面
//先取消注释，然后在APP里编辑界面，添加两个按钮，键名分别是 BUTTON_8 和 BUTTON_9 ,设置为开关按钮 
//能力有限，代码不是最好的，有建议或反馈请留言[每一条都会看的}
//视频教程看我b站投稿【请搭配专栏食用，视频里可能不全】
#define BLINKER_WIFI
#include <EEPROM.h>
#define BLINKER_MIOT_AIRCONDITION
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRac.h>
#include <IRutils.h>
#include <IRtext.h>
#include "ESP8266WiFi.h"
#include <LittleFS.h>
#include <FS.h>
#include <LinkedList.h>
char auth[] = "KEY";
char ssid[] = "名称";
char pswd[] = "密码";
//
#include <ir_Samsung.h>  //修改空调型号改这里，比如你的空调是格力，那么使用下面的，并把这个添加注释，而且下面空调风速模式等等前缀也要修改，比较麻烦
//#include <ir_Gree.h> //左边是格力空调
#include <Blinker.h>

//
#define TESTFS LittleFS
//红外相关
//红外指令数据格式
struct RawData{
  uint16_t bits;
  uint16_t nIndex;
  uint16_t nSize;
  uint16_t* data;
  RawData():nIndex(0),nSize(0),
  data(NULL){}
};
RawData* pCurrentRawData = NULL;
const uint16_t kCaptureBufferSize = 1024;
#if DECODE_AC
const uint8_t kTimeout = 50;
#else   // DECODE_AC
const uint8_t kTimeout = 15;
#endif  // DECODE_AC
const uint16_t kMinUnknownSize = 12;
#define LEGACY_TIMING_INFO false
bool xuexi = false;
const uint16_t kIrLed = 13;  
uint16_t RECV_PIN = 14;   
bool is_debug = false;
//当前调式的红外指令对应的按钮
int index_debug_btn = 0;
//红外指令列表
LinkedList<RawData*> myLinkedList;
IRSamsungAc ac(kIrLed);   
IRsend irsend(kIrLed);  
IRrecv irrecv(RECV_PIN, kCaptureBufferSize, kTimeout, true);
decode_results results;
//红外APP部分

BlinkerButton Button900("btn-xuexi"); //学习开关数据键名


//wifi信号检测
BlinkerNumber TEXT("rssi");

#define Slider_1 "Slidert"//APP温度控制滑条
BlinkerSlider Slider1(Slider_1);
#define Slider_2 "Sliderf"//APP风速控制滑条
BlinkerSlider Slider2(Slider_2);

uint32_t read_time = 0;
#define ACSEND \
   digitalWrite(16,HIGH); \
   pinMode(RECV_PIN,OUTPUT); \
   digitalWrite(RECV_PIN,LOW); \
   ac.send();  \
   pinMode(RECV_PIN,INPUT); \
   delay(100); \
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

// 新建按钮组件对象
#define BlinkerButton_Callback_Def(index,name)             \
  BlinkerButton Button##index(name);         \
  void button_callback##index(const String & state) { buttonx_callback(index,state);}

#define BlinkerButton_Attach_Def(index)             \
    Button##index.attach(button_callback##index);


    
BlinkerButton_Callback_Def(0,"btn-0");
BlinkerButton_Callback_Def(1,"btn-1");
BlinkerButton_Callback_Def(2,"btn-2");
BlinkerButton_Callback_Def(3,"btn-3");
BlinkerButton_Callback_Def(4,"btn-4");
BlinkerButton_Callback_Def(5,"btn-5");
BlinkerButton_Callback_Def(6,"btn-6");
BlinkerButton_Callback_Def(7,"btn-7");
BlinkerButton_Callback_Def(8,"btn-8");
BlinkerButton_Callback_Def(9,"btn-9");
BlinkerButton_Callback_Def(10,"btn-10");


//APP控制
BlinkerButton Button11("BUTTON_11");
BlinkerButton Button12("BUTTON_12");
//模式控制
BlinkerButton Button13("BUTTON_13");
BlinkerButton Button14("BUTTON_14");
BlinkerButton Button15("BUTTON_15");
BlinkerButton Button16("BUTTON_16");
BlinkerButton Button17("BUTTON_17");
////以下代码为节能模式开关 如果你的空调支持自行取消注释
//BlinkerButton Button8("BUTTON_8");
 ////以下代码为睡眠模式开关 如果你的空调支持自行取消注释
//BlinkerButton Button9("BUTTON_9");

//////////以下为红外学习代码
void button900_callback(const String & state)
{
    if(state == "on"){   
      Serial.println("开");   
      xuexi = true;
      Button900.print("on");   
      Blinker.delay(100);
       digitalWrite(kIrLed,LOW);
     pinMode(RECV_PIN,INPUT);
      Blinker.println("开始接收");
    }else if (state == "off"){   
      Serial.println("关");
      pinMode(RECV_PIN,OUTPUT);
       digitalWrite(RECV_PIN,HIGH);
      xuexi = false;
      Button900.print("off");
      Blinker.delay(100);
      Blinker.println("停止接收");
    }
}


//////////
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
void button11_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");
        Button11.icon("icon_1");
        Button11.color("#FF0000");
        Button11.text("电源开");
        Button11.print("on");   
         ac.on();
        ACSEND
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");
        Button11.icon("icon_1");
        Button11.color("#FF0000");
        Button11.text("电源关");
        // Button1.text("Your button name", "describe");
        Button11.print("off");
         ac.off();
        ACSEND
    }
     else {
        BLINKER_LOG("Get user setting: ", state);

        Button11.icon("icon_1");
        Button11.color("#FFFFFF");
        Button11.text("?");
        // Button1.text("Your button name", "describe");
        Button11.print();

}
}
//APP控制开关-扫风
void button12_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");
        Button12.icon("icon_1");
        Button12.color("#FF0000");
        Button12.text("扫风开");
        Button12.print("on");   
        ac.setSwing(true);
        ACSEND
        BlinkerMIOT.vswing("on");
        BlinkerMIOT.print();
         vsState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");
        Button12.icon("icon_1");
        Button12.color("#FF0000");
        Button12.text("扫风关");
        // Button1.text("Your button name", "describe");
        Button12.print("off");
        ac.setSwing(false);
        ACSEND
        BlinkerMIOT.vswing("off");
        BlinkerMIOT.print();
         vsState = false;
    }
     else {
        BLINKER_LOG("Get user setting: ", state);

        Button12.text("?");
        Button12.print();

}
}
//APP控制-模式
void button13_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!");
        ac.setMode(kSamsungAcAuto);
        ACSEND
    }
}
void button14_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!");  
        ac.setMode(kSamsungAcCool);
        ACSEND
    }
}
 void button15_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!"); 
        ac.setMode(kSamsungAcDry);
        ACSEND
    }
}
void button16_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!");
        ac.setMode(kSamsungAcFan);
        ACSEND
    }
}
void button17_callback(const String & state)
{
  
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Toggle on!"); 
        ac.setMode(kSamsungAcHeat);
        ACSEND
    }
}  
//按钮重置为关闭
void resetBlinkerButton(){
  for(int  i = 1;i<11;i++){
    getBlinkerButton(i).print("off");
  }
}
BlinkerButton& getBlinkerButton(const uint16_t index){
  switch(index){
    case 0:
      return Button0;
    case 1:
      return Button1;
    case 2:
      return Button2;
    case 3:
      return Button3;
    case 4:
      return Button4;
    case 5:
      return Button5;
    case 6:
      return Button6;
    case 7:
      return Button7;
    case 8:
      return Button8;
    case 9:
      return Button9;
    case 10:
      return Button10;
  }
}

//删除红外数据
void freeRawData(RawData * pRawData){
  if(pRawData){
    if(pRawData->data){
      delete[] pRawData->data;
    }
    delete pRawData;
    pRawData = NULL;
  }

}
void dataStorage(const uint16_t& bits){
  int nSize = myLinkedList.size();
  // assert(nwrite != sizeof(uint16_t));
  for(int i = 0;i< nSize ;i++){
    RawData* pRawData = myLinkedList.get(i);
    if(pRawData){
       if(bits == pRawData->bits){
          Blinker.dataStorage("sensor", pRawData->nIndex);
          Serial.println("Code:");
          Serial.println(pRawData->bits);
          Serial.print("保存操作历史：btn-");
          Serial.println(pRawData->nIndex);
          break;
       }
    }
  }

}
void resultToRawData(const decode_results * const results,LinkedList<uint16_t>& sourceCode) {
  // int nLength = getCorrectedRawLength(results);
  // pRawData->nSize = nLength;
  // Dump data
  for (uint16_t i = 1; i < results->rawlen; i++) {
    uint32_t usecs;
    for (usecs = results->rawbuf[i] * kRawTick; usecs > UINT16_MAX; usecs -= UINT16_MAX) {
      sourceCode.add(UINT16_MAX);
    }
     sourceCode.add(usecs);
  }
}

void bindRICodeToBlinkerButton(const int& index,const BlinkerButton& btn){
  Serial.println("红外指令和按钮绑定 index:"+ index );
  if(pCurrentRawData && pCurrentRawData->nSize > 0){
      RawData *tempRawData_ = myLinkedList.get(index);
      if(tempRawData_){
        Serial.println("红外指令已经存在，准备替换");
        myLinkedList.remove(index);
        //先释放原来指令的内存
        freeRawData(tempRawData_);
      }
      //保存新的指令
      tempRawData_   =  new RawData();
      tempRawData_->bits = pCurrentRawData->bits;
      tempRawData_->nIndex = index;
      tempRawData_->nSize = pCurrentRawData->nSize;
      tempRawData_->data = new uint16_t[pCurrentRawData->nSize];
      memcpy((void*)tempRawData_->data,(void*)pCurrentRawData->data,pCurrentRawData->nSize * sizeof(uint16_t));
      myLinkedList.add(index, tempRawData_);
      Serial.println("红外指令绑定完成");

  }else{
     BLINKER_LOG("invalid code, bindCurrentRICode: ", index);
  }
}
//保存红外配置文件
bool saveConfig(FS *fs){
  if (!fs->begin()) {
    Serial.println("Unable to begin(), aborting");
    return false;
  }
  Serial.println("Creating file, may take a while...");
  long start = millis();
  File f = fs->open("/irconfig.bin", "w");
  if (!f) {
    Serial.println("Unable to open file for writing, aborting");
    return false;
  }
  int nSize = myLinkedList.size();
  int nwrite = f.write((char*)&nSize, sizeof(uint16_t));
  // assert(nwrite != sizeof(uint16_t));
  for(int i = 0;i< nSize ;i++){
    RawData* pRawData = myLinkedList.get(i);
    if(pRawData){
      f.write((char*)&pRawData->bits, sizeof(uint16_t));
      f.write((char*)&pRawData->nIndex, sizeof(uint16_t));
      f.write((char*)&pRawData->nSize, sizeof(uint16_t));
      f.write((char*)pRawData->data, pRawData->nSize * sizeof(uint16_t));
    }
  }
  f.close();
  long stop = millis();
  Serial.printf("==> Time to write  chunks = %ld milliseconds\n", stop - start);
  return true;
}

//加载红外配置文件
bool loadConfig(FS *fs){
if (!fs->begin()) {
    Serial.println("Unable to begin(), aborting");
    return false;
  }
  Serial.println("Reading file, may take a while...");
  long start = millis();
  File f = fs->open("/irconfig.bin", "r");
  if (!f) {
    Serial.println("Unable to open file for reading, aborting");
    return false;
  }
  int nSize = 0;
  int nread = f.read((uint8_t*)&nSize, sizeof(uint16_t));
  // assert(nread != sizeof(uint16_t));
   Serial.println(nread);
  for(int i = 0;i< nSize ;i++){
    RawData* pRawData = new RawData();
    if(pRawData){
      f.read((uint8_t*)&pRawData->bits, sizeof(uint16_t));
      f.read((uint8_t*)&pRawData->nIndex, sizeof(uint16_t));
      f.read((uint8_t*)&pRawData->nSize, sizeof(uint16_t));
      Serial.println(pRawData->bits);
      Serial.println(pRawData->nIndex);
      Serial.println(pRawData->nSize);
      pRawData->data = new uint16_t[pRawData->nSize];
      if(pRawData->data){
        f.read((uint8_t*)pRawData->data, pRawData->nSize * sizeof(uint16_t));
        myLinkedList.add(pRawData->nIndex,pRawData);
      }
    }
  }
  f.close();
  long stop = millis();
  Serial.printf("==> Time to write  chunks = %ld milliseconds\n", stop - start);
  return true;
}

void SendIrMsg( const uint16_t* rawbuf, uint16_t nSize){
  if(rawbuf && nSize >0 ){
digitalWrite(16,HIGH);
   pinMode(RECV_PIN,OUTPUT); 
   digitalWrite(RECV_PIN,LOW); 
    IRsend irsend(kIrLed);  // Set the GPIO （D2）to be used to sending the message.
    irsend.begin();
    Serial.printf("RawData length: %u",nSize);
    Serial.println("#");
    irsend.sendRaw(rawbuf, nSize, 38);
   
    pinMode(RECV_PIN,INPUT);
    digitalWrite(16,LOW); 
  }else{
    Serial.print("illegal RawData");
  }
}

void buttont_callback(const String & state) { 
      if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");
        RawData*  pRawData = myLinkedList.get(1);
        if(pRawData){
          //rawData
          SendIrMsg(pRawData->data,pRawData->nSize);
          dataStorage(pRawData->bits);
        }else{
          BLINKER_LOG("还没有绑定指令,快去绑定指令吧！");
       }
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");
        RawData*  pRawData = myLinkedList.get(2);
        if(pRawData){
          //rawData
          SendIrMsg(pRawData->data,pRawData->nSize);
          dataStorage(pRawData->bits);
        }else{
          BLINKER_LOG("还没有绑定指令,快去绑定指令吧！");
       }

    }

}
//红外-APP内按钮
void buttonx_callback( const uint16_t index, const String & state) {
    BLINKER_LOG("get button state: ", state);
    BLINKER_LOG("index： ", index);

    if (state == BLINKER_CMD_BUTTON_PRESSED){
        BLINKER_LOG("Button pressed!");
    }
    else if (state == BLINKER_CMD_BUTTON_TAP){
        BLINKER_LOG("Toggle tap!");
        if(is_debug  && index_debug_btn == index ){
            BLINKER_LOG("IR test is_debug = true");
            SendIrMsg((uint16_t*)pCurrentRawData->data,pCurrentRawData->nSize);
            resetBlinkerButton();
          getBlinkerButton(index).text("发射成功");
          getBlinkerButton(index).print();
          Blinker.vibrate();
            // printRawData(pCurrentRawData);
        }else{
            BLINKER_LOG("IR test is_debug = false");
            RawData*  pRawData = myLinkedList.get(index);
            if(pRawData){
              //rawData
              (pRawData->data,pRawData->nSize);
              dataStorage(pRawData->bits);
              // printRawData(pRawData);
            }else{
              BLINKER_LOG("还没有绑定指令,快去绑定指令吧！");
            }
        }
    }
    else if (state == BLINKER_CMD_BUTTON_PRESSUP){
        index_debug_btn  = index;
        is_debug = !is_debug;
        if(is_debug){
          resetBlinkerButton();
          getBlinkerButton(index).text("学习模式");
          getBlinkerButton(index).print();
          Blinker.vibrate();
        }else{
          //红外指令和blinker 的按钮绑定
          bindRICodeToBlinkerButton(index,getBlinkerButton(index));
          saveConfig(&TESTFS);
          getBlinkerButton(index).print("off");
          Blinker.vibrate();
        }
      
    }
}



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

void print_decode_results(const decode_results& results){
    // Display a crude timestamp.
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
    // Check if we got an IR message that was to big for our capture buffer.
    if (results.overflow)
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    // Display the library version the message was captured with.
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_ "\n");
    // Display the basic output of what we found.
    String basic =  resultToHumanReadableBasic(&results);
    basic.replace(" ","");
    Serial.print(basic.c_str());
    // Display any extra A/C info if we have it.
    String description = IRAcUtils::resultAcToString(&results);
    description.replace(" ","");
    if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
    yield();  // Feed the WDT as the text output can take a while to print.
#if LEGACY_TIMING_INFO
    // Output legacy RAW timing info of the result.
    String results = resultToTimingInfo(&results);
    Serial.println(results.c_str());
    yield();  // Feed the WDT (again)
#endif  // LEGACY_TIMING_INFO
    // Output the results as source code
    String sourcecode = resultToSourceCode(&results);
    Serial.println(sourcecode.c_str());
    Serial.println();    // Blank line between entries
    yield();             // Feed the WDT (again)
}
void printRawData(RawData* pRawData){
  if(pRawData){
    decode_results results;
    results.rawlen = pRawData->nSize;
    results.value = pRawData->bits;
    results.rawbuf = (uint16_t *)pRawData->data;
    Serial.println("发送指令:");
    print_decode_results(results);
  }

}
void setup()
{
  // 
     ac.begin();
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
//APP控制
    Button11.attach(button11_callback);
    Button12.attach(button12_callback);
    //模式callback
    Button13.attach(button13_callback);
    Button14.attach(button14_callback);
    Button15.attach(button15_callback);
    Button16.attach(button16_callback);
    Button17.attach(button17_callback);
    Button900.attach(button900_callback);

    Blinker.begin(auth, ssid, pswd);
   
    BlinkerButton_Attach_Def(0);
    BlinkerButton_Attach_Def(1);
    BlinkerButton_Attach_Def(2);
    BlinkerButton_Attach_Def(3);
    BlinkerButton_Attach_Def(4);
    BlinkerButton_Attach_Def(5);
    BlinkerButton_Attach_Def(6);
    BlinkerButton_Attach_Def(7);
    BlinkerButton_Attach_Def(8);
    BlinkerButton_Attach_Def(9);
    BlinkerButton_Attach_Def(10);


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
     pinMode(RECV_PIN,INPUT);
    digitalWrite(kIrLed,LOW);
     pinMode(16,OUTPUT);
    digitalWrite(16,LOW);
    Blinker.delay(10);
    irrecv.enableIRIn();  
      irsend.begin();    
       //加载保存的红外信息
    loadConfig(&TESTFS);
    int nSize = myLinkedList.size();
    for(int i = 0;i< nSize ;i++){
      decode_results results;
      RawData* pRawData = myLinkedList.get(i);
      if(pRawData){
        results.bits = pRawData->bits;
        results.rawlen = pRawData->nSize;
        results.rawbuf = pRawData->data;
        print_decode_results(results);
      }
    }
}


void loop()
{
    Blinker.run();
 if (irrecv.decode(&results) && xuexi==true) {
         digitalWrite(16,HIGH);
         print_decode_results(results);
  if(pCurrentRawData != NULL) {
          //先释放原来指令的内存
          freeRawData(pCurrentRawData);
       }
       LinkedList<uint16_t> sourceCode;
      resultToRawData(&results,sourceCode);
       pCurrentRawData   =  new RawData();
      pCurrentRawData->bits = results.value;
      pCurrentRawData->nIndex = 0;
      pCurrentRawData->nSize = sourceCode.size();
      pCurrentRawData->data = new uint16_t[sourceCode.size()];
      for(int i = 0;i< sourceCode.size();i++){
         pCurrentRawData->data[i] = sourceCode.get(i);
      }
      dataStorage(results.value);
      Serial.println("已经保存为测试指令");
        irrecv.resume();
        Blinker.delay(100);
        digitalWrite(16,LOW);
      }
    
}


