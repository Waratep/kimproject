#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x3F, 20, 4);
class Data
{
  private:
    float _rate;
    float _speed;
    float _width;
    float _feed;

    uint8_t pwm_calibated;
    float   feed_calibated;

    float rate_sett;
    float width_sett;
    float cal_sett;

    float rate_defult;
    float width_defult;
    float cal_defult;

  public:
    Data()
    {
      _rate = 0;
      _speed = 0;
      _width = 0;
      _feed = 0;

      pwm_calibated = 0;
      feed_calibated = 0;

      rate_sett = 0;
      width_sett = 0;
      cal_sett = 0;

      rate_defult = 50;
      width_defult = 1.5;
      cal_defult = 1.5;
    }
    float get_rate()
    {
      return _rate;
    }
    float set_rate(float rate)
    {
      _rate = rate;
    }
    float get_speed()
    {
      return _speed;
    }
    float set_speed(float speed)
    {
      _speed = speed;
    }
    float get_width()
    {
      return _width;
    }
    float set_width(float width)
    {
      _width = width;
    }
    float get_feed()
    {
      return _feed;
    }
    float set_feed(float feed)
    {
      _feed = feed;
    }

    uint8_t getpwm_calibated()
    {
      return pwm_calibated;
    }
    uint8_t setpwm_calibated(uint8_t pwm)
    {
      pwm_calibated = pwm;
    }
    float getfeed_calibated()
    {
      return feed_calibated;
    }
    float setfeed_calibated(float feed)
    {
      feed_calibated = feed;
    }


    float getrate_sett()
    {
      return rate_sett;
    }
    float setrate_sett(float rate)
    {
      rate_sett = rate;
    }

    float getwidth_sett()
    {
      return width_sett;
    }
    float setwidth_sett(float width)
    {
      width_sett = width;
    }

    float getcal_sett()
    {
      return cal_sett;
    }
    float setcal_sett(float cal)
    {
      cal_sett = cal;
    }

    float getrate_defult()
    {
      return rate_defult;
    }
    float setrate_defult(float rate)
    {
      rate_defult = rate;
    }

    float getwidth_defult()
    {
      return width_defult;
    }
    float setwidth_defult(float width)
    {
      width_defult = width;
    }

    float getcal_defult()
    {
      return cal_defult;
    }
    float setcal_defult(float cal)
    {
      cal_defult = cal;
    }

};

Data datas;

class Rotary
{
  public:
    uint8_t state;
    uint8_t prevState;
    float count;
    int encoderStates[4][4] = {
      {  0, -1,  1,  0 },
      {  1,  0,  0, -1 },
      { -1,  0,  0,  1 },
      {  0,  1, -1,  0 },
    };

    uint8_t _bt;
    uint8_t _s1;
    uint8_t _s2;
    uint8_t change;
    uint8_t prss;

    Rotary(uint8_t bt , uint8_t  s1 , uint8_t s2)
    {
      _bt = bt;
      _s1 = s1;
      _s2 = s2;

      pinMode(_bt, INPUT);
      pinMode(_s1, INPUT);
      pinMode(_s2, INPUT);
    }

    float rotary()
    {
      for(uint8_t i = 0 ; i < 10 ; i++)
      {
        state = (digitalRead(_s1) << 1) | digitalRead(_s2);
      }
      
      count += encoderStates[prevState][state] * 0.2;

      if (state != prevState) {
        change = 1;
      }
      prevState = state;
      return count;
    }
    uint8_t rotaryChange()
    {
      return change;
    }
    uint8_t rotaryPress()
    {
      if (!digitalRead(_bt))
      {
        while (!digitalRead(_bt));
        delay(50);
        prss = 1;
        //        Serial.println(prss);
      }
      else
      {
        prss = 0;
      }
      return prss;
    }
};

Rotary rt(36, 39, 32);

class Menu
{
  public:
    uint8_t page;
    uint8_t last_page;
    uint8_t cursur_select;

    float subpage;
    float cursur;
    Menu()
    {
      page          = 0;
      subpage       = 0;
      cursur        = 0;
      last_page     = 0;
      cursur_select = 0;

    }
    void menucursor()
    {
      clearCursor();
      updateCursor();
    }
    void pages(uint8_t _page)
    {
      switch (_page)
      {
        case 0: page_0();                  break;
        case 1: page_1();                  break;
        case 2: main_page_1(); cursur = 1; break;
        case 3: main_page_2(); cursur = 1; break;
        default:                           break;
      }
    }
    void clearCursor()
    {
      for (uint8_t i = 0 ; i < 4 ; i++) {
        lcd.setCursor(19, i);
        lcd.print(" ");
      }
    }
    void updateCursor()
    {
      if ((uint8_t)cursur >= 7) cursur = 6;
      if ((uint8_t)cursur <= 1) cursur = 1;

      if ((uint8_t)cursur > 0 and ((uint8_t)cursur <= 3))
      {
        last_page = page;
        cursur_select = (uint8_t)cursur;
        lcd.setCursor(19, cursur_select);
        lcd.print("<");
        page = 2;
      }
      else if ((uint8_t)cursur > 3 and ((uint8_t)cursur <= 6))
      {
        last_page = page;
        cursur_select = (uint8_t)cursur;
        lcd.setCursor(19, cursur_select - 3);
        lcd.print("<");
        page = 3;
      }
    }
    void page_0()
    {
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("RATECONTROLLER");
      lcd.setCursor(4, 2);
      lcd.print("KMITL  RC-1");
      page = 0;
    }
    void page_1()
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RATE : 00.0 kg/r");
      lcd.setCursor(0, 1);
      lcd.print("SPEED:  0.0 km/r");
      lcd.setCursor(0, 2);
      lcd.print("WIDTH:  0.0 m");
      lcd.setCursor(0, 3);
      lcd.print("FEED :  0.  g/s");
      page = 1;
    }
    void main_page_1()
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU:PRESS TO ACT");
      lcd.setCursor(5, 1);
      lcd.print("CALIBATEION");
      lcd.setCursor(5, 2);
      lcd.print("SETTING");
      lcd.setCursor(5, 3);
      lcd.print("DRAIN");
      page = 2;
    }
    void main_page_2()
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU:PRESS TO ACT");
      lcd.setCursor(5, 1);
      lcd.print("PROGRAM");
      lcd.setCursor(5, 2);
      lcd.print("SET DEFULT");
      lcd.setCursor(5, 3);
      lcd.print("SAVE");
      page = 3;
    }
    uint8_t pagechange()
    {
      if (page != last_page)
      {
        pages(page);
        return 1;
      }
      else
      {
        return 0;
      }
    }
    uint8_t calibation()
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("MENU:CALIBATION");
      lcd.setCursor(1, 1);
      lcd.print("%PWM    :");
      lcd.print(datas.getpwm_calibated());
      lcd.print("%");
      lcd.setCursor(1, 2);
      lcd.print("FEED@");
      lcd.print(datas.getpwm_calibated());
      lcd.setCursor(9, 2);
      lcd.print(":");
      lcd.print(datas.getfeed_calibated());
      lcd.print(" g/s");
      lcd.setCursor(3, 3);
      lcd.print("EXIT MENU");
      
      uint8_t cursur = rt.rotary();
      while (1)
      {
        cursur = rt.rotary();
        cursur = cursur >= 3 ? 3 : cursur;
        cursur = cursur <= 1 ? 1 : cursur;
        clearCursor();
        lcd.setCursor(19, cursur);
        lcd.print("<");
        if (cursur == 1 and rt.rotaryPress())
        {
          uint8_t duty;
          while (!rt.rotaryPress())
          {
            duty = rt.rotary();
            Serial.println(rt.rotary());
            duty += datas.getpwm_calibated();
            duty = duty < 0 ? 0 : duty;
            duty = duty > 100 ? 100 : duty;
            lcd.setCursor(10, 1);
            lcd.print("    ");
            lcd.setCursor(10, 1);
            lcd.print(duty);
            lcd.print("%");

          }
          datas.setpwm_calibated(duty);
          rt.count = 0;
        }
        else if (cursur == 2 and rt.rotaryPress())
        {
          uint8_t feed;
          while (!rt.rotaryPress())
          {
            feed = rt.rotary();
            Serial.println(rt.rotary());
            feed += datas.getfeed_calibated();
            feed = feed < 0 ? 0 : feed;
            feed = feed > 100 ? 100 : feed;
            lcd.setCursor(1, 2);
            lcd.print("FEED@");
            lcd.print(datas.getpwm_calibated());
            lcd.setCursor(9, 2);
            lcd.print(":");
            lcd.setCursor(10, 2);
            lcd.print("          ");
            lcd.setCursor(10, 2);
            lcd.print(feed);
            lcd.print(" g/s");
          }
          datas.setfeed_calibated(feed);
          rt.count = 0;
        }
        else if (cursur == 3 and rt.rotaryPress())
        {
          break;
        }
      }
      subpage = 1;
    }
    uint8_t settiing()
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("MENU: SETTING");
      lcd.setCursor(1, 1);
      lcd.print("RATE : ");
      lcd.print(datas.getrate_sett());
      lcd.print(" kg/r");
      lcd.setCursor(1, 2);
      lcd.print("WIDTH: ");
      lcd.print(datas.getwidth_sett());
      lcd.print(" m");
      lcd.setCursor(1, 3);
      lcd.print("CAL. : ");
      lcd.print(datas.getcal_sett());
      lcd.print(" g/s");

      uint8_t cursur = rt.rotary();
      while (1)
      {
        cursur = rt.rotary();
        cursur = cursur >= 4 ? 4 : cursur;
        cursur = cursur <= 1 ? 1 : cursur;
        clearCursor();
        lcd.setCursor(19, cursur);
        lcd.print("<");
        if (cursur >= 4) break;
        if (cursur == 1 and rt.rotaryPress())
        {
          uint8_t rate;
          while (!rt.rotaryPress())
          {
            Serial.println(rt.rotary());
            rate = rt.rotary();
            rate += datas.getrate_sett();
            rate = rate < 0 ? 0 : rate;
            rate = rate > 100 ? 100 : rate;
            lcd.setCursor(8, 1);
            lcd.print("             ");
            lcd.setCursor(8, 1);
            lcd.print(rate);
            lcd.print(" kg/r");

          }
          datas.setrate_sett(rate);
          Serial.println(rate);
          rt.count = 0;
        }
        else if (cursur == 2 and rt.rotaryPress())
        {
          uint8_t width;
          while (!rt.rotaryPress())
          {
            Serial.println(rt.rotary());
            width = rt.rotary();
            width += datas.getwidth_sett();
            width = width < 0 ? 0 : width;
            width = width > 100 ? 100 : width;
            lcd.setCursor(8, 2);
            lcd.print("             ");
            lcd.setCursor(8, 2);
            lcd.print(width);
            lcd.print(" m");
          }
          datas.setwidth_sett(width);
          Serial.println(width);
          rt.count = 0;
        }
        else if (cursur == 3 and rt.rotaryPress())
        {
          uint8_t cal;
          while (!rt.rotaryPress())
          {
            Serial.println(rt.rotary());
            cal = rt.rotary();
            cal += datas.getcal_sett();
            cal = cal < 0 ? 0 : cal;
            cal = cal > 100 ? 100 : cal;
            lcd.setCursor(8, 3);
            lcd.print("             ");
            lcd.setCursor(8, 3);
            lcd.print(cal);
            lcd.print(" g/s");
          }
          datas.setcal_sett(cal);
          Serial.println(cal);
          rt.count = 0;
        }
      }
    }
    uint8_t drain()
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("MENU: DRAIN");
      lcd.setCursor(3, 2);
      lcd.print("DRAING....");

      while (!rt.rotaryPress());

      subpage = 3;
    }
    uint8_t program()
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("MENU: PROGRAM");

      rt.count = 0;
      uint8_t cursur = rt.rotary();
      uint8_t state = 0;

      while (1)
      {
        cursur = rt.rotary();
        cursur = cursur >= 8 ? 8 : cursur;
        cursur = cursur <= 1 ? 1 : cursur;
        clearCursor();

        if (cursur >= 7) break;
        if (cursur >= 4)
        {
          cursur = cursur >= 7 ? 6 : cursur;
          lcd.setCursor(19, cursur - 3);
        }
        else
        {
          lcd.setCursor(19, cursur);
        }

        lcd.print("<");

        if (cursur >= 4 and state != 2)
        {
          rt.count = 4;
          lcd.setCursor(0, 1);
          lcd.print("PROGRAM:  04");
          lcd.setCursor(0, 2);
          lcd.print("PROGRAM:  05");
          lcd.setCursor(0, 3);
          lcd.print("PROGRAM:  06");
          state = 2;
        }
        else if (cursur <= 3 and state != 1)
        {
          rt.count = 0;
          lcd.setCursor(0, 1);
          lcd.print("PROGRAM:  01");
          lcd.setCursor(0, 2);
          lcd.print("PROGRAM:  02");
          lcd.setCursor(0, 3);
          lcd.print("PROGRAM:  03");
          state = 1;
        }
        if (cursur == 1 and rt.rotaryPress())
        {
          Serial.println(EEPROM.read(0));
          Serial.println(EEPROM.read(1));
          Serial.println(EEPROM.read(2));

          datas.setrate_sett(EEPROM.read(0));
          datas.setwidth_sett(EEPROM.read(1));
          datas.setcal_sett(EEPROM.read(2));
          rt.count = 0;
          break;
        }
        else if (cursur == 2 and rt.rotaryPress())
        {
          datas.setrate_sett(EEPROM.read(3));
          datas.setwidth_sett(EEPROM.read(4));
          datas.setcal_sett(EEPROM.read(5));
          rt.count = 0;
          break;
        }
        else if (cursur == 3 and rt.rotaryPress())
        {
          datas.setrate_sett(EEPROM.read(6));
          datas.setwidth_sett(EEPROM.read(7));
          datas.setcal_sett(EEPROM.read(8));
          rt.count = 0;
          break;
        }
        else if (cursur == 4 and rt.rotaryPress())
        {
          datas.setrate_sett(EEPROM.read(9));
          datas.setwidth_sett(EEPROM.read(10));
          datas.setcal_sett(EEPROM.read(11));
          rt.count = 0;
          break;
        }
        else if (cursur == 5 and rt.rotaryPress())
        {
          datas.setrate_sett(EEPROM.read(12));
          datas.setwidth_sett(EEPROM.read(13));
          datas.setcal_sett(EEPROM.read(14));
          rt.count = 0;
          break;
        }
        else if (cursur == 6 and rt.rotaryPress())
        {
          datas.setrate_sett(EEPROM.read(15));
          datas.setwidth_sett(EEPROM.read(16));
          datas.setcal_sett(EEPROM.read(17));
          rt.count = 0;
          break;
        }
      }
    }
    uint8_t setDefult()
    {
      datas.setrate_sett(datas.getrate_defult());
      datas.setwidth_sett(datas.getwidth_defult());
      datas.setcal_sett(datas.getcal_defult());
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU: SET DEFULT");
      lcd.setCursor(0, 1);
      lcd.print("RATE : ");
      lcd.print(datas.getrate_sett());
      lcd.print(" kg/r");
      lcd.setCursor(0, 2);
      lcd.print("WIDTH: ");
      lcd.print(datas.getwidth_sett());
      lcd.print(" m");
      lcd.setCursor(0, 3);
      lcd.print("CAL. : ");
      lcd.print(datas.getcal_sett());
      lcd.print(" g/s");

      while (!rt.rotaryPress());
    }
    uint8_t save()
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("MENU: SAVE");

      rt.count = 0;
      uint8_t cursur = rt.rotary();
      uint8_t state = 0;

      while (1)
      {
        cursur = rt.rotary();
        cursur = cursur >= 8 ? 8 : cursur;
        cursur = cursur <= 1 ? 1 : cursur;
        clearCursor();

        if (cursur >= 7) break;
        if (cursur >= 4)
        {
          cursur = cursur >= 7 ? 6 : cursur;
          lcd.setCursor(19, cursur - 3);
        }
        else
        {
          lcd.setCursor(19, cursur);
        }

        lcd.print("<");

        if (cursur >= 4 and state != 2)
        {
          rt.count = 4;
          lcd.setCursor(0, 1);
          lcd.print("PROGRAM:  04");
          lcd.setCursor(0, 2);
          lcd.print("PROGRAM:  05");
          lcd.setCursor(0, 3);
          lcd.print("PROGRAM:  06");
          state = 2;
        }
        else if (cursur <= 3 and state != 1)
        {
          rt.count = 0;
          lcd.setCursor(0, 1);
          lcd.print("PROGRAM:  01");
          lcd.setCursor(0, 2);
          lcd.print("PROGRAM:  02");
          lcd.setCursor(0, 3);
          lcd.print("PROGRAM:  03");
          state = 1;
        }
        if (cursur == 1 and rt.rotaryPress())
        {

          EEPROM.write(0 , (uint8_t)datas.getrate_sett());
          EEPROM.write(1 , (uint8_t)datas.getwidth_sett());
          EEPROM.write(2 , (uint8_t)datas.getcal_sett());
          rt.count = 0;
          break;
        }
        else if (cursur == 2 and rt.rotaryPress())
        {
          EEPROM.write(3 , (uint8_t)datas.getrate_sett());
          EEPROM.write(4 , (uint8_t)datas.getwidth_sett());
          EEPROM.write(5 , (uint8_t)datas.getcal_sett());
          rt.count = 0;
          break;
        }
        else if (cursur == 3 and rt.rotaryPress())
        {
          EEPROM.write(6 , (uint8_t)datas.getrate_sett());
          EEPROM.write(7 , (uint8_t)datas.getwidth_sett());
          EEPROM.write(8 , (uint8_t)datas.getcal_sett());
          rt.count = 0;
          break;
        }
        else if (cursur == 4 and rt.rotaryPress())
        {
          EEPROM.write(9 , (uint8_t)datas.getrate_sett());
          EEPROM.write(10 , (uint8_t)datas.getwidth_sett());
          EEPROM.write(11 , (uint8_t)datas.getcal_sett());
          rt.count = 0;
          break;
        }
        else if (cursur == 5 and rt.rotaryPress())
        {
          EEPROM.write(12 , (uint8_t)datas.getrate_sett());
          EEPROM.write(13 , (uint8_t)datas.getwidth_sett());
          EEPROM.write(14 , (uint8_t)datas.getcal_sett());
          rt.count = 0;
          break;
        }
        else if (cursur == 6 and rt.rotaryPress())
        {
          EEPROM.write(15 , (uint8_t)datas.getrate_sett());
          EEPROM.write(16 , (uint8_t)datas.getwidth_sett());
          EEPROM.write(17 , (uint8_t)datas.getcal_sett());
          rt.count = 0;
          break;
        }
      }


    }

};

Menu menu;

void setup() {

  Serial.begin(115200);
  lcd.begin();
  EEPROM.begin(64);

  xTaskCreate(
                    taskOne,          /* Task function. */
                    "TaskOne",        /* String with name of task. */
                    40000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

//  xTaskCreate(
//                    taskTwo,          /* Task function. */
//                    "TaskTwo",        /* String with name of task. */
//                    40000,            /* Stack size in bytes. */
//                    NULL,             /* Parameter passed as input of the task */
//                    2,                /* Priority of the task. */
//                    NULL);    

  menu.pages(0);
  while (!rt.rotaryPress());
  menu.pages(1);
  while (!rt.rotaryPress());
}

void loop() 
{
  menu.cursur = rt.rotary();
  menu.menucursor();
  menu.pagechange();

  if (rt.rotaryPress()) {
    switch (menu.cursur_select)
    {
      case 1: menu.calibation(); menu.pages(2); rt.count = 0; break;
      case 2: menu.settiing();   menu.pages(2); rt.count = 0; break;
      case 3: menu.drain();      menu.pages(2); rt.count = 0; break;
      case 4: menu.program();    menu.pages(2); rt.count = 0; break;
      case 5: menu.setDefult();  menu.pages(2); rt.count = 0; break;
      case 6: menu.save();       menu.pages(2); rt.count = 0; break;
      default:                                                break;
    }
  }  
}

void taskOne( void * parameter )
{
  while(1){
    Serial.println(rt.rotary());
    vTaskDelay(10);
  }
  vTaskDelete( NULL );
}
 
//void taskTwo( void * parameter)
//{
//
//  menu.pages(0);
//  while (!rt.rotaryPress());
//  menu.pages(1);
//  while (!rt.rotaryPress());
//  
//  while(1)
//  {
//    menu.cursur = rt.rotary();
//    menu.menucursor();
//    menu.pagechange();
//  
//    if (rt.rotaryPress()) {
//      switch (menu.cursur_select)
//      {
//        case 1: menu.calibation(); menu.pages(2); rt.count = 0; break;
//        case 2: menu.settiing();   menu.pages(2); rt.count = 0; break;
//        case 3: menu.drain();      menu.pages(2); rt.count = 0; break;
//        case 4: menu.program();    menu.pages(2); rt.count = 0; break;
//        case 5: menu.setDefult();  menu.pages(2); rt.count = 0; break;
//        case 6: menu.save();       menu.pages(2); rt.count = 0; break;
//        default:                                                break;
//      }
//    }
//    vTaskDelay(10);
//  }
//  vTaskDelete( NULL );
//}