空调控制
=================
求投币支持 ：https://www.bilibili.com/video/BV1XM4y1N75M/

智能空调控制，通过最新的Blinker实现小爱开关空调，设置温度等等 配置APP还能定时操作 最低15元实现智能空调 

鉴于大家空调型号不一，已经发布通用版本，common就是通用的，需要修改两处，一处是数字，firm=这里，一处是void setup这里,decode_type_t::你的空调型号，要大写，适配就变蓝色


=================
![avatar](https://github.com/1812z/Aircondition/raw/main/%E5%88%B6%E4%BD%9C%E6%80%9D%E8%B7%AF%E5%9B%BE.png)
所需的库：IRRemoteESP8266,WIFI,Blinker,Arduino,DHT sensor library 【反正少了就搜的词少点，都下载了,Doge]

自制ESP8266红外遥控器
=================

硬件：ESP8266 ir模块 dht21[可选]

不带DHt版本 界面配置相同，删除温度部分组件即可


场景控A1
=================
场景控A1专用，只能用这个 否则可能烧毁！！！

未实现的功能：

* 红外接收

 刷写以及注意事项：https://www.bilibili.com/video/BV1cA411P79G/ 
 
 不带DHt版本 界面配置相同，删除温度部分组件即可 

界面配置[普通版本无学习]：
=================
{¨config¨{¨headerColor¨¨transparent¨¨headerStyle¨¨dark¨¨background¨{¨img¨¨assets/img/bg/f5.jpg¨¨isFull¨»}}¨dashboard¨|{¨type¨¨btn¨¨ico¨¨fad fa-power-off¨¨mode¨Ê¨t0¨¨电源¨¨t1¨¨文本2¨¨bg¨É¨cols¨Ë¨rows¨Ë¨key¨¨BUTTON_1¨´x´Ï´y´Ë¨speech¨|{¨cmd¨¨开关电脑¨¨act¨¨tap¨}÷¨lstyle¨Ê¨clr¨¨#076EEF¨}{ßA¨num¨ßF¨实时湿度¨ßC¨fad fa-humidity¨ßUßV¨min¨É¨max¨¢1c¨uni¨´%´ßJÉßKËßLËßM¨humi¨´x´Ë´y´ÏßO|÷ßTÉ}{ßAßWßF¨实时温度¨ßC¨fad fa-thermometer-three-quarters¨ßUßVßZÉßa¤eßb´℃´ßJÉßKËßLËßM¨temp¨´x´É´y´ÏßO|÷ßTÉ}{ßA¨cha¨ßJÉ¨sty¨¨line¨ßUßV¨sty1¨ßi¨clr1¨¨#EA0909¨¨sty2¨ßi¨clr2¨¨#389BEE¨ßKÑßLÍßMßf´x´É´y´¤AßO|÷¨key0¨¨cha-humi¨¨key1¨¨cha-temp¨ßF¨湿度¨ßH¨温度¨ßTÊ}{ßA¨tex¨ßF¨空调控制¨ßH¨风速0为自动¨¨size¨¨16¨ßJÉßC¨fad fa-air-conditioner¨ßKÍßLËßMßw´x´É´y´ÉßO|÷ßTÌßUßV}{ßA¨ran¨ßF¨设定温度¨ßUßoßa¤UßZ¤HßJÉßKÏßLËßM¨Slidert¨´x´É´y´ËßO|÷ßTÍ}{ßAß11ßF´风速´ßUßoßaÍßZÉßJÉßKÑßLËßM¨Sliderf¨´x´É´y´ÍßO|÷ßTË}{ßAßBßC¨fal fa-power-off¨ßEÊßF´扫风´ßHßIßJÉßKËßLËßM¨BUTTON_2¨´x´Ï´y´ÑßO|÷ßTÊßUßV}{ßAßBßC¨fad fa-snowflakes¨ßEÉßF¨文本1¨ßHßIßJÉßKÊßLÊßM¨BUTTON_4¨´x´Ï´y´ÏßO|÷ßTÍßUßV}{ßAßBßCßYßEÉßF´除湿´ßHßIßJÉßKÊßLÊßM¨BUTTON_5¨´x´Ð´y´ÏßO|÷ßTÍßUßV}{ßAßBßC¨fad fa-wind¨ßEÉßFß18ßHßIßJÉßKÊßLÊßM¨BUTTON_6¨´x´Ð´y´ÐßO|÷ßTÍßUßV}{ßAßBßC¨fad fa-sun¨ßEÉßFß18ßHßIßJÉßKÊßLÊßM¨BUTTON_7¨´x´Ï´y´ÐßO|÷ßTÍßUßV}{ßAßBßC¨fad fa-chart-network¨ßEÉßF´自动´ßHßIßJÉßKËßLËßM¨BUTTON_3¨´x´Í´y´ÏßO|÷ßUßV}{ßAßWßF¨信号强度¨ßC¨fad fa-signal¨ßUßVßZÉßaº0ßb´db´ßJÉßKÍßLËßM¨rssi¨´x´Í´y´ÉßO|÷ßTÊ}{ßAßBßCß15ßEÊßF´节能´ßHßIßJÉßKËßLËßM¨BUTTON_8¨´x´É´y´ÑßO|÷ßTÊ}{ßAßBßCß15ßEÊßF´睡眠´ßHßIßJÉßKËßLËßM¨BUTTON_9¨´x´Ë´y´ÑßO|÷ßTÊ}{ßAßBßCß15ßEÊßF´超强´ßHßIßJÉßKËßLËßM¨BUTTON_10¨´x´Í´y´ÑßO|÷ßTÊ}÷¨actions¨|¦ßP¦¨switch¨‡¨text¨‡´on´¨打开?name¨¨off¨¨关闭?name¨—÷¨triggers¨|{¨source¨ß1O¨source_zh¨¨开关状态¨¨state¨|´on´ß1R÷¨state_zh¨|´打开´´关闭´÷}÷}


界面配置[带10个按钮学习，长按学习]：
=================

{¨config¨{¨headerColor¨¨transparent¨¨headerStyle¨¨dark¨¨background¨{¨img¨¨assets/img/bg/f5.jpg¨¨isFull¨»}}¨dashboard¨|{¨type¨¨btn¨¨ico¨¨fad fa-power-off¨¨mode¨Ê¨t0¨¨电源¨¨t1¨¨文本2¨¨bg¨É¨cols¨Ë¨rows¨Ë¨key¨¨BUTTON_11¨´x´É´y´Ë¨speech¨|{¨cmd¨¨开关电脑¨¨act¨¨tap¨}÷¨lstyle¨Ê¨clr¨¨#EA0909¨}{ßA¨tex¨ßF¨三星空调控制¨ßH¨按住我下滑¨¨size¨¨16¨ßJÉßC¨fad fa-air-conditioner¨ßKÍßLËßM¨空调控制¨´x´É´y´ÉßO|÷ßTÌßU¨#076EEF¨}{ßA¨ran¨ßF¨设定温度¨ßU¨#389BEE¨¨max¨¤U¨min¨¤HßJÉßKÑßLËßM¨Slidert¨´x´É´y´ÏßO|÷ßTË}{ßAßeßF¨风速¨ßUßgßhÍßiÉßJÉßKÑßLËßM¨Sliderf¨´x´É´y´ÍßO|÷ßTË}{ßAßBßC¨fal fa-power-off¨ßEÊßF¨扫风¨ßHßIßJÉßKËßLËßM¨BUTTON_12¨´x´Ï´y´ËßO|÷ßTÊßUßd}{ßAßBßC¨fad fa-snowflakes¨ßEÉßF¨制冷¨ßHßIßJÉßKËßLËßM¨BUTTON_14¨´x´Ë´y´ËßO|÷ßTÉßUßd}{ßAßBßC¨fad fa-humidity¨ßEÉßF¨除湿¨ßHßIßJÉßKËßLËßM¨BUTTON_15¨´x´Í´y´ÑßO|÷ßTÉßUßd}{ßAßBßC¨fad fa-wind¨ßEÉßF¨送风¨ßHßIßJÉßKËßLËßM¨BUTTON_16¨´x´Ï´y´ÑßO|÷ßTÉßUßd}{ßAßBßC¨fad fa-sun¨ßEÉßF¨制热¨ßHßIßJÉßKËßLËßM¨BUTTON_17¨´x´Í´y´ËßO|÷ßTÉßUßd}{ßAßBßC¨fad fa-chart-network¨ßEÉßF´自动´ßHßIßJÉßKËßLËßM¨BUTTON_13¨´x´Ë´y´ÑßO|÷ßUßd}{ßA¨num¨ßF¨信号强度¨ßC¨fad fa-signal¨ßUßdßiÉßh¢1c¨uni¨´db´ßJÉßKÍßLËßM¨rssi¨´x´Í´y´ÉßO|÷ßTÊ}{ßAßBßCßmßEÉßF¨点击发射 ¨ßHßIßJÉßKËßLËßM¨btn-0¨´x´É´y´¤AßO|÷ßUßd}{ßAßBßCßmßEÉßFß18ßHßIßJÉßKËßLËßM¨btn-1¨´x´Ë´y´¤AßO|÷ßUßd}{ßAßBßC¨fad fa-toggle-off¨ßEÊßF¨学习开关¨ßHßIßJÉßKËßLËßM¨btn-xuexi¨´x´É´y´ÑßO|÷ßTÊßUßd}{ßAßBßCßmßEÉßFß18ßHßIßJÉßKËßLËßM¨btn-4¨´x´É´y´¤CßO|÷ßUßd}{ßAßBßCßmßEÉßFß18ßHßIßJÉßKËßLËßM¨btn-5¨´x´Ë´y´¤CßO|÷ßUßd}{ßAßBßCßmßEÉßFß18ßHßIßJÉßKËßLËßM¨btn-2¨´x´Í´y´¤AßO|÷ßUßd}{ßAßBßCßmßEÉßFß18ßHßIßJÉßKËßLËßM¨btn-6¨´x´Í´y´¤CßO|÷ßUßd}{ßAßBßCßmßEÉßFß18ßHßIßJÉßKËßLËßM¨btn-3¨´x´Ï´y´¤AßO|÷ßUßd}{ßAßBßCßmßEÉßFß18ßHßIßJÉßKËßLËßM¨btn-7¨´x´Ï´y´¤CßO|÷ßUßd}{ßA¨deb¨ßEÉßJÉßKÑßLÌßM¨debug¨´x´É´y´¤E}÷¨actions¨|¦ßP¦¨switch¨‡¨text¨‡´on´¨打开?name¨¨off¨¨关闭?name¨—÷¨triggers¨|{¨source¨ß1N¨source_zh¨¨开关状态¨¨state¨|´on´ß1Q÷¨state_zh¨|´打开´´关闭´÷}÷}


