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

界面配置：
=================
{¨config¨{¨headerColor¨¨transparent¨¨headerStyle¨¨dark¨¨background¨{¨img¨¨assets/img/bg/f5.jpg¨¨isFull¨»}}¨dashboard¨|{¨type¨¨btn¨¨ico¨¨fad fa-power-off¨¨mode¨Ê¨t0¨¨电源¨¨t1¨¨文本2¨¨bg¨É¨cols¨Ë¨rows¨Ë¨key¨¨BUTTON_1¨´x´Ï´y´Ë¨speech¨|{¨cmd¨¨开关电脑¨¨act¨¨tap¨}÷¨lstyle¨Ê¨clr¨¨#076EEF¨}{ßA¨num¨ßF¨实时湿度¨ßC¨fad fa-humidity¨ßUßV¨min¨É¨max¨¢1c¨uni¨´%´ßJÉßKËßLËßM¨humi¨´x´Ë´y´ÏßO|÷ßTÉ}{ßAßWßF¨实时温度¨ßC¨fad fa-thermometer-three-quarters¨ßUßVßZÉßa¤eßb´℃´ßJÉßKËßLËßM¨temp¨´x´É´y´ÏßO|÷ßTÉ}{ßA¨cha¨ßJÉ¨sty¨¨line¨ßUßV¨sty1¨ßi¨clr1¨¨#EA0909¨¨sty2¨ßi¨clr2¨¨#389BEE¨ßKÑßLÍßMßf´x´É´y´¤AßO|÷¨key0¨¨cha-humi¨¨key1¨¨cha-temp¨ßF¨湿度¨ßH¨温度¨ßTÊ}{ßA¨tex¨ßF¨空调控制¨ßH¨风速0为自动¨¨size¨¨16¨ßJÉßC¨fad fa-air-conditioner¨ßKÍßLËßMßw´x´É´y´ÉßO|÷ßTÌßUßV}{ßA¨ran¨ßF¨设定温度¨ßUßoßa¤UßZ¤HßJÉßKÏßLËßM¨Slidert¨´x´É´y´ËßO|÷ßTÍ}{ßAß11ßF´风速´ßUßoßaÍßZÉßJÉßKÑßLËßM¨Sliderf¨´x´É´y´ÍßO|÷ßTË}{ßAßBßC¨fal fa-power-off¨ßEÊßF´扫风´ßHßIßJÉßKËßLËßM¨BUTTON_2¨´x´Ï´y´ÑßO|÷ßTÊßUßV}{ßAßBßC¨fad fa-snowflakes¨ßEÉßF¨文本1¨ßHßIßJÉßKÊßLÊßM¨BUTTON_4¨´x´Ï´y´ÏßO|÷ßTÍßUßV}{ßAßBßCßYßEÉßF´除湿´ßHßIßJÉßKÊßLÊßM¨BUTTON_5¨´x´Ð´y´ÏßO|÷ßTÍßUßV}{ßAßBßC¨fad fa-wind¨ßEÉßFß18ßHßIßJÉßKÊßLÊßM¨BUTTON_6¨´x´Ð´y´ÐßO|÷ßTÍßUßV}{ßAßBßC¨fad fa-sun¨ßEÉßFß18ßHßIßJÉßKÊßLÊßM¨BUTTON_7¨´x´Ï´y´ÐßO|÷ßTÍßUßV}{ßAßBßC¨fad fa-chart-network¨ßEÉßF´自动´ßHßIßJÉßKËßLËßM¨BUTTON_3¨´x´Í´y´ÏßO|÷ßUßV}{ßAßWßF¨信号强度¨ßC¨fad fa-signal¨ßUßVßZÉßaº0ßb´db´ßJÉßKÍßLËßM¨rssi¨´x´Í´y´ÉßO|÷ßTÊ}{ßAßBßCß15ßEÊßF´节能´ßHßIßJÉßKËßLËßM¨BUTTON_8¨´x´É´y´ÑßO|÷ßTÊ}{ßAßBßCß15ßEÊßF´睡眠´ßHßIßJÉßKËßLËßM¨BUTTON_9¨´x´Ë´y´ÑßO|÷ßTÊ}{ßAßBßCß15ßEÊßF´超强´ßHßIßJÉßKËßLËßM¨BUTTON_10¨´x´Í´y´ÑßO|÷ßTÊ}÷¨actions¨|¦ßP¦¨switch¨‡¨text¨‡´on´¨打开?name¨¨off¨¨关闭?name¨—÷¨triggers¨|{¨source¨ß1O¨source_zh¨¨开关状态¨¨state¨|´on´ß1R÷¨state_zh¨|´打开´´关闭´÷}÷}


