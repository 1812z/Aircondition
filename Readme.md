空调控制
=================
![avatar](https://github.com/1812z/Aircondition/raw/main/%E5%88%B6%E4%BD%9C%E6%80%9D%E8%B7%AF%E5%9B%BE.png)
Aircondition  
智能空调控制，通过最新的Blinker实现小爱开关空调，设置温度等等 配置APP还能定时操作 最低15元实现智能空调 
硬件：ESP8266 ir模块 dht21 
未实现的功能：
* 各种模式
已经实现的功能:
* 风速调节 [小爱不支持超强]
* 温度调节
* 模式调节[只支持APP]
* 定时任务[APP自带功能]
* 信号检测
* 温度湿度曲线变化图

界面配置：
{¨config¨{¨headerColor¨¨transparent¨¨headerStyle¨¨dark¨¨background¨{¨img¨¨assets/img/bg/f5.jpg¨¨isFull¨»}}¨dashboard¨|{¨type¨¨btn¨¨ico¨¨fad fa-power-off¨¨mode¨Ê¨t0¨¨电源¨¨t1¨¨文本2¨¨bg¨É¨cols¨Ë¨rows¨Ë¨key¨¨BUTTON_1¨´x´Ï´y´Ë¨speech¨|{¨cmd¨¨开关电脑¨¨act¨¨tap¨}÷¨lstyle¨Ê¨clr¨¨#076EEF¨}{ßA¨num¨ßF¨实时湿度¨ßC¨fad fa-humidity¨ßUßV¨min¨É¨max¨¢1c¨uni¨´%´ßJÉßKËßLËßM¨humi¨´x´Ë´y´ÏßO|÷ßTÉ}{ßAßWßF¨实时温度¨ßC¨fad fa-thermometer-three-quarters¨ßUßVßZÉßa¤eßb´℃´ßJÉßKËßLËßM¨temp¨´x´É´y´ÏßO|÷ßTÉ}{ßA¨cha¨ßJÉ¨sty¨¨line¨ßUßV¨sty1¨ßi¨clr1¨¨#EA0909¨¨sty2¨ßi¨clr2¨¨#389BEE¨ßKÑßLÍßMßf´x´É´y´ÑßO|÷¨key0¨¨cha-humi¨¨key1¨¨cha-temp¨ßF¨湿度¨ßH¨温度¨ßTÊ}{ßA¨tex¨ßF¨三星空调控制¨ßH¨风速0为自动¨¨size¨¨16¨ßJÉßC¨fad fa-air-conditioner¨ßKÍßLËßM¨空调控制¨´x´É´y´ÉßO|÷ßTÌßUßV}{ßA¨ran¨ßF¨设定温度¨ßUßoßa¤UßZ¤HßJÉßKÏßLËßM¨Slidert¨´x´É´y´ËßO|÷ßTÍ}{ßA¨deb¨ßEÉßJÉßKÑßLÌßM¨debug¨´x´É´y´¤CßO|÷}{ßAß12ßF´风速´ßUßoßaÍßZÉßJÉßKÏßLËßM¨Sliderf¨´x´É´y´ÍßO|÷ßTÍ}{ßAßBßC¨fal fa-power-off¨ßEÊßF´扫风´ßHßIßJÉßKËßLËßM¨BUTTON_2¨´x´Ï´y´ÍßO|÷ßTÊßUßV}{ßAßBßC¨fad fa-snowflakes¨ßEÉßF¨文本1¨ßHßIßJÉßKÊßLÊßM¨BUTTON_4¨´x´Ï´y´ÏßO|÷ßTÍßUßV}{ßAßBßCßYßEÉßF´除湿´ßHßIßJÉßKÊßLÊßM¨BUTTON_5¨´x´Ð´y´ÏßO|÷ßTÍßUßV}{ßAßBßC¨fad fa-wind¨ßEÉßFß1BßHßIßJÉßKÊßLÊßM¨BUTTON_6¨´x´Ð´y´ÐßO|÷ßTÍßUßV}{ßAßBßC¨fad fa-sun¨ßEÉßFß1BßHßIßJÉßKÊßLÊßM¨BUTTON_7¨´x´Ï´y´ÐßO|÷ßTÍßUßV}{ßAßBßC¨fad fa-chart-network¨ßEÉßF´自动´ßHßIßJÉßKËßLËßM¨BUTTON_3¨´x´Í´y´ÏßO|÷ßUßV}{ßAßWßF¨信号强度¨ßC¨fad fa-signal¨ßUßVßZÉßaº0ßb´db´ßJÉßKÍßLËßM¨rssi¨´x´Í´y´ÉßO|÷ßTÊ}÷¨actions¨|¦ßP¦¨switch¨‡¨text¨‡´on´¨打开?name¨¨off¨¨关闭?name¨—÷¨triggers¨|{¨source¨ß1O¨source_zh¨¨开关状态¨¨state¨|´on´ß1R÷¨state_zh¨|´打开´´关闭´÷}÷}

