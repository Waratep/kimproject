#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define run_bt 34
uint8_t state_run = 0;
String gpsHour;

char hexaKeys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[4] = {32, 33, 4, 27};
byte colPins[4] = {25, 26, 18, 19};
uint8_t getKeypad = 0;
Keypad keypads = Keypad( makeKeymap(hexaKeys), rowPins, colPins, 4, 4);


class Cursurs {
  private:
    uint8_t cursorX;
    uint8_t cursorY;
    uint8_t enter;
    uint8_t setting;
  public:
    Cursurs() {
      cursorX = cursorY = enter = setting = 0 ;
    }
    uint8_t getX() {
      return cursorX;
    }
    uint8_t getY() {
      return cursorY;
    }
    uint8_t getEnter() {
      return enter;
    }
    uint8_t getSetting() {
      return setting;
    }
    void setX(uint8_t x) {
      cursorX = x;
    }
    void setY(uint8_t y) {
      cursorY = y;
    }
    void setEnter(uint8_t _enter) {
      enter = _enter;
    }
    void setSetting(uint8_t sett) {
      setting = sett;
    }
};

Cursurs cs;

class Data {
  private:
    float _rate;
    float _speed;
    float _width;
    float _feed;
    uint8_t _pwm;

    uint8_t pwm_calibated_1;
    float   feed_calibated_1;
    uint8_t pwm_calibated_2;
    float   feed_calibated_2;

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

      pwm_calibated_1 = 0;
      feed_calibated_1 = 0;
      pwm_calibated_2 = 0;
      feed_calibated_2 = 0;

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
    uint8_t get_pwm()
    {
      return _pwm;
    }
    uint8_t set_pwm(uint8_t pwm)
    {
      _pwm = pwm;
    }

    uint8_t getpwm_calibated_1()
    {
      return pwm_calibated_1;
    }
    uint8_t setpwm_calibated_1(uint8_t pwm)
    {
      pwm_calibated_1 = pwm;
    }
    float getfeed_calibated_1()
    {
      return feed_calibated_1;
    }
    float setfeed_calibated_1(float feed)
    {
      feed_calibated_1 = feed;
    }
    uint8_t getpwm_calibated_2()
    {
      return pwm_calibated_2;
    }
    uint8_t setpwm_calibated_2(uint8_t pwm)
    {
      pwm_calibated_2 = pwm;
    }
    float getfeed_calibated_2()
    {
      return feed_calibated_2;
    }
    float setfeed_calibated_2(float feed)
    {
      feed_calibated_2 = feed;
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

class Menu {
  public:
    uint8_t page;
    uint8_t last_page;
    uint8_t cursur_select;

    float subpage;
    float cursur;
    Menu(){
      page          = 0;
      subpage       = 0;
      cursur        = 0;
      last_page     = 0;
      cursur_select = 0;

    }
    void menucursor(){
      clearCursor();
      updateCursor();
    }
    void pages(uint8_t _page){
      switch (_page)
      {
        case 0: page_0();                  break;
        case 1: page_1();                  break;
        case 2: main_page_1();             break;
        case 3: main_page_2();             break;
        case 4: main_page_3();             break;
        default:                           break;
      }
    }
    void clearCursor(){
      for (uint8_t i = 0 ; i < 4 ; i++) {
        lcd.setCursor(19, i);
        lcd.print(" ");
      }
    }
    void updateCursor(){
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
      else if ((uint8_t)cursur > 6 and ((uint8_t)cursur <= 7))
      {
        last_page = page;
        cursur_select = (uint8_t)cursur;
        lcd.setCursor(19, 1);
        lcd.print("<");
        page = 4;
      }
    }
    void page_0(){
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print("RATE CONTROLLER");
      lcd.setCursor(4, 2);
      lcd.print("KMITL  RC-1");
      page = 0;
      while (!cs.getEnter()) {
        Serial.println("loop in page0");
      }
      Serial.println("page0");
    }
    void page_1(){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RATE : "); lcd.print(datas.getrate_sett()); lcd.print(" kg/r");
      lcd.setCursor(0, 1);
      lcd.print("SPEED: "); lcd.print(datas.get_speed()); lcd.print(" km/r");
      lcd.setCursor(0, 2);
      lcd.print("WIDTH: "); lcd.print(datas.getwidth_sett()); lcd.print(" m");
      lcd.setCursor(0, 3);
      lcd.print("FEED : "); lcd.print(datas.get_feed()); lcd.print(" g/s");
      page = 1;
      Serial.println(datas.getrate_sett());
      Serial.println(datas.get_speed());
      Serial.println(datas.getwidth_sett());
      Serial.println(datas.get_feed());
      while (!cs.getEnter()) {
        Serial.println("loop in page1");
      }
      Serial.println("page1");

    }
    void main_page_1(){
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
    void main_page_2(){
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
    void main_page_3(){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU:PRESS TO ACT");
      lcd.setCursor(5, 1);
      lcd.print("DISPLAY");
      page = 4;
    }
    uint8_t pagechange(){
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
    uint8_t calibation(){
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("MENU:CALIBATION");
      lcd.setCursor(1, 1);
      lcd.print("FEED@");
      lcd.setCursor(8, 1);
      lcd.print(":");
      lcd.print(datas.getfeed_calibated_1());
      lcd.print(" g/s");
      lcd.setCursor(1, 2);
      lcd.print("FEED@25");
      lcd.setCursor(8, 2);
      lcd.print(":");
      lcd.print(datas.getfeed_calibated_2());
      lcd.print(" g/s");
      lcd.setCursor(1, 3);
      lcd.print("EXIT -->");
      
      uint8_t cursur = 1;
      char tmp;
      while (1)
      {
        tmp = keypads.getKey();
        if (tmp - '0' == 18) {
          cursur--;
        } else if (tmp - '0' == 19) {
          cursur++;
        }
        cursur = cursur >= 3 ? 3 : cursur;
        cursur = cursur <= 0 ? 1 : cursur;
        clearCursor();
        lcd.setCursor(19, cursur);
        lcd.print("<");
        if (cursur == 1 and tmp - '0' == 20)
        {
          float feed;
          char tmp = keypads.getKey();
          float val = 0 , counter = 0;
          while (tmp - '0' != 20)
          {
            delay(200);
            tmp = keypads.getKey();
            if(tmp and tmp - '0' != 20){
              if(counter == 0) val += (tmp - '0') * 1000;
              if(counter == 1) val += (tmp - '0') * 100;
              if(counter == 2) val += (tmp - '0') * 10;
              if(counter == 3) val += (tmp - '0') * 1;
              counter++;
            }
            Serial.print(val);
            Serial.print("  ");
            Serial.println(tmp - '0');
            feed += datas.getfeed_calibated_1();
            feed = feed < 0 ? 0 : feed;
            feed = feed > 100 ? 100 : feed;
            lcd.setCursor(1, 1);
            lcd.print("FEED@75");
            lcd.setCursor(8, 1);
            lcd.print(":");
            lcd.setCursor(9, 1);
            lcd.print("          ");
            lcd.setCursor(9, 1);
            lcd.print(val / 10);
            lcd.print(" g/s");
            feed = val / 10;

          }
          datas.setfeed_calibated_1(feed);
        }
        else if (cursur == 2 and tmp - '0' == 20)
        {
          float feed;
          char tmp = keypads.getKey();
          float val = 0 , counter = 0;
          while (tmp - '0' != 20)
          {
            delay(200);
            tmp = keypads.getKey();
            if(tmp and tmp - '0' != 20){
              if(counter == 0) val += (tmp - '0') * 1000;
              if(counter == 1) val += (tmp - '0') * 100;
              if(counter == 2) val += (tmp - '0') * 10;
              if(counter == 3) val += (tmp - '0') * 1;
              counter++;
            }            
            feed += datas.getfeed_calibated_2();
            feed = feed < 0 ? 0 : feed;
            feed = feed > 100 ? 100 : feed;
            lcd.setCursor(1, 2);
            lcd.print("FEED@");
            lcd.setCursor(8, 2);
            lcd.print(":");
            lcd.setCursor(9, 2);
            lcd.print("          ");
            lcd.setCursor(9, 2);
            lcd.print(val / 10);
            lcd.print(" g/s");
            feed = val / 10;
          }
          datas.setfeed_calibated_2(feed);
        }
        else if (cursur == 3 and tmp - '0' == 20)
        {
          break;
        }
        delay(100);
      }
      subpage = 1;
    }
    uint8_t settiing(){
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

      uint8_t cursur = 1;
      char tmp;
      while (1)
      {
        delay(200);
        tmp = keypads.getKey();
        if (tmp - '0' == 18) {
          cursur--;
        } else if (tmp - '0' == 19) {
          cursur++;
        }
        cursur = cursur >= 4 ? 4 : cursur;
        cursur = cursur <= 1 ? 1 : cursur;
        clearCursor();
        lcd.setCursor(19, cursur);
        lcd.print("<");
        if (cursur >= 4) break;
        
        if (cursur == 1 and tmp - '0' == 20){
          float rate = 0 , val = 0, tmp = 0, counter = 0;
          while (tmp - '0' != 20){
            delay(200);
            tmp = keypads.getKey();
            if(tmp and tmp - '0' != 20){
              if(counter == 0) val += (tmp - '0') * 1000;
              if(counter == 1) val += (tmp - '0') * 100;
              if(counter == 2) val += (tmp - '0') * 10;
              if(counter == 3) val += (tmp - '0') * 1;
              counter++;
            }  
            rate += datas.getrate_sett();
            rate = rate < 0 ? 0 : rate;
            rate = rate > 100 ? 100 : rate;
            lcd.setCursor(8, 1);
            lcd.print("             ");
            lcd.setCursor(8, 1);
            lcd.print(val / 10);
            lcd.print(" kg/r");

          }
          rate = val / 10;
          datas.setrate_sett(rate);
        }else if (cursur == 2 and tmp - '0' == 20){
          float rate = 0 , val = 0, tmp = 0, counter = 0 ,width = 0;
          while (tmp - '0' != 20){
            delay(200);
            tmp = keypads.getKey();
            if(tmp and tmp - '0' != 20){
              if(counter == 0) val += (tmp - '0') * 1000;
              if(counter == 1) val += (tmp - '0') * 100;
              if(counter == 2) val += (tmp - '0') * 10;
              if(counter == 3) val += (tmp - '0') * 1;
              counter++;
            }              
            width += datas.getwidth_sett();
            width = width < 0 ? 0 : width;
            width = width > 100 ? 100 : width;
            lcd.setCursor(8, 2);
            lcd.print("             ");
            lcd.setCursor(8, 2);
            lcd.print(val / 10);
            lcd.print(" m");
          }
          width = val / 10;
          datas.setwidth_sett(width);
        }else if (cursur == 3 and tmp - '0' == 20){
          float rate = 0 , val = 0, tmp = 0, counter = 0 , cal;
          while (tmp - '0' != 20){
            delay(200);
            tmp = keypads.getKey();
            if(tmp and tmp - '0' != 20){
              if(counter == 0) val += (tmp - '0') * 1000;
              if(counter == 1) val += (tmp - '0') * 100;
              if(counter == 2) val += (tmp - '0') * 10;
              if(counter == 3) val += (tmp - '0') * 1;
              counter++;
            }       
            cal += datas.getcal_sett();
            cal = cal < 0 ? 0 : cal;
            cal = cal > 100 ? 100 : cal;
            lcd.setCursor(8, 3);
            lcd.print("             ");
            lcd.setCursor(8, 3);
            lcd.print(val / 10);
            lcd.print(" g/s");
          }
          cal = val / 10;
          datas.setcal_sett(cal);
      }
    }
}
    uint8_t drain(){
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("MENU: DRAIN");
      lcd.setCursor(3, 2);
      lcd.print("DRAING....");
      char tmp;
      float val = 0 , counter = 0;
      while (tmp - '0' != 20) {
        tmp = keypads.getKey();
        if(tmp and tmp - '0' != 20){
          if(counter == 0) val += (tmp - '0') * 10;
          if(counter == 1) val += (tmp - '0') * 1;
          counter++;
        }    
        Serial.println(val);
        run_pwm(map(val,0,100,0,4095));
        delay(200);
      }
      run_pwm(0);
  
      subpage = 3;
    }
    uint8_t program(){
      //      lcd.clear();
      //      lcd.setCursor(2, 0);
      //      lcd.print("MENU: PROGRAM");
      //
      //      kp.count = 0;
      //      uint8_t cursur = kp.Keypads();
      //      uint8_t state = 0;
      //
      //      while (1)
      //      {
      //        cursur = kp.Keypads() * 1;
      //        cursur = cursur >= 8 ? 8 : cursur;
      //        cursur = cursur <= 1 ? 1 : cursur;
      //        Serial.println(cursur);
      //        clearCursor();
      //
      //        if (cursur >= 8) break;
      //        if (cursur >= 4)
      //        {
      //          cursur = cursur >= 7 ? 6 : cursur;
      //          lcd.setCursor(19, cursur - 3);
      //        }
      //        else
      //        {
      //          lcd.setCursor(19, cursur);
      //        }
      //
      //        lcd.print("<");
      //
      //        if (cursur >= 4 and state != 2)
      //        {
      //          kp.count = 4;
      //          lcd.setCursor(0, 1);
      //          lcd.print("PROGRAM:  04");
      //          lcd.setCursor(0, 2);
      //          lcd.print("PROGRAM:  05");
      //          lcd.setCursor(0, 3);
      //          lcd.print("PROGRAM:  06");
      //          state = 2;
      //        }
      //        else if (cursur <= 3 and state != 1)
      //        {
      //          kp.count = 0;
      //          lcd.setCursor(0, 1);
      //          lcd.print("PROGRAM:  01");
      //          lcd.setCursor(0, 2);
      //          lcd.print("PROGRAM:  02");
      //          lcd.setCursor(0, 3);
      //          lcd.print("PROGRAM:  03");
      //          state = 1;
      //        }
      //        if (cursur == 1 and kp.KeypadsPress())
      //        {
      //
      //          float x = (EEPROM.read(1));
      //          float y = (EEPROM.read(3));
      //          float z = (EEPROM.read(5));
      //
      //          float _x = EEPROM.read(0);
      //          float _y = EEPROM.read(2);
      //          float _z = EEPROM.read(4);
      //
      //          datas.setrate_sett((float)_x + (float)(x / 100));
      //          datas.setwidth_sett((float)_y + (float)(y / 100));
      //          datas.setcal_sett((float)_z + (float)(z / 100));
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 2 and kp.KeypadsPress())
      //        {
      //          float x = (EEPROM.read(7));
      //          float y = (EEPROM.read(9));
      //          float z = (EEPROM.read(11));
      //
      //          float _x = EEPROM.read(6);
      //          float _y = EEPROM.read(8);
      //          float _z = EEPROM.read(10);
      //
      //          datas.setrate_sett((float)_x + (float)(x / 100));
      //          datas.setwidth_sett((float)_y + (float)(y / 100));
      //          datas.setcal_sett((float)_z + (float)(z / 100));
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 3 and kp.KeypadsPress())
      //        {
      //          float x = (EEPROM.read(13));
      //          float y = (EEPROM.read(15));
      //          float z = (EEPROM.read(17));
      //
      //          float _x = EEPROM.read(12);
      //          float _y = EEPROM.read(14);
      //          float _z = EEPROM.read(16);
      //
      //          datas.setrate_sett((float)_x + (float)(x / 100));
      //          datas.setwidth_sett((float)_y + (float)(y / 100));
      //          datas.setcal_sett((float)_z + (float)(z / 100));
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 4 and kp.KeypadsPress())
      //        {
      //          float x = (EEPROM.read(19));
      //          float y = (EEPROM.read(21));
      //          float z = (EEPROM.read(23));
      //
      //          float _x = EEPROM.read(18);
      //          float _y = EEPROM.read(20);
      //          float _z = EEPROM.read(22);
      //
      //          datas.setrate_sett((float)_x + (float)(x / 100));
      //          datas.setwidth_sett((float)_y + (float)(y / 100));
      //          datas.setcal_sett((float)_z + (float)(z / 100));
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 5 and kp.KeypadsPress())
      //        {
      //          float x = (EEPROM.read(25));
      //          float y = (EEPROM.read(27));
      //          float z = (EEPROM.read(29));
      //
      //          float _x = EEPROM.read(24);
      //          float _y = EEPROM.read(26);
      //          float _z = EEPROM.read(28);
      //
      //          datas.setrate_sett((float)_x + (float)(x / 100));
      //          datas.setwidth_sett((float)_y + (float)(y / 100));
      //          datas.setcal_sett((float)_z + (float)(z / 100));
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 6 and kp.KeypadsPress())
      //        {
      //          float x = (EEPROM.read(31));
      //          float y = (EEPROM.read(33));
      //          float z = (EEPROM.read(35));
      //
      //          float _x = EEPROM.read(30);
      //          float _y = EEPROM.read(32);
      //          float _z = EEPROM.read(34);
      //
      //          datas.setrate_sett((float)_x + (float)(x / 100));
      //          datas.setwidth_sett((float)_y + (float)(y / 100));
      //          datas.setcal_sett((float)_z + (float)(z / 100));
      //          kp.count = 0;
      //          break;
      //        }
      //      }
    }
    uint8_t setDefult(){
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
      char tmp;
      while (tmp - '0' != 20){
        tmp = keypads.getKey();
      }
    }
    uint8_t save(){
      //      lcd.clear();
      //      lcd.setCursor(2, 0);
      //      lcd.print("MENU: SAVE");
      //
      //      kp.count = 0;
      //      uint8_t cursur = kp.Keypads();
      //      uint8_t state = 0;
      //
      //      while (1)
      //      {
      //        cursur = kp.Keypads() * 1;
      //        cursur = cursur >= 8 ? 8 : cursur;
      //        cursur = cursur <= 1 ? 1 : cursur;
      //        clearCursor();
      //
      //        if (cursur >= 7) break;
      //        if (cursur >= 4)
      //        {
      //          cursur = cursur >= 7 ? 6 : cursur;
      //          lcd.setCursor(19, cursur - 3);
      //        }
      //        else
      //        {
      //          lcd.setCursor(19, cursur);
      //        }
      //
      //        lcd.print("<");
      //
      //        if (cursur >= 4 and state != 2)
      //        {
      //          kp.count = 4;
      //          lcd.setCursor(0, 1);
      //          lcd.print("PROGRAM:  04");
      //          lcd.setCursor(0, 2);
      //          lcd.print("PROGRAM:  05");
      //          lcd.setCursor(0, 3);
      //          lcd.print("PROGRAM:  06");
      //          state = 2;
      //        }
      //        else if (cursur <= 3 and state != 1)
      //        {
      //          kp.count = 0;
      //          lcd.setCursor(0, 1);
      //          lcd.print("PROGRAM:  01");
      //          lcd.setCursor(0, 2);
      //          lcd.print("PROGRAM:  02");
      //          lcd.setCursor(0, 3);
      //          lcd.print("PROGRAM:  03");
      //          state = 1;
      //        }
      //        if (cursur == 1 and kp.KeypadsPress())
      //        {
      //
      //          EEPROM.write(0 , (uint8_t)datas.getrate_sett());
      //          EEPROM.write(2 , (uint8_t)datas.getwidth_sett());
      //          EEPROM.write(4 , (uint8_t)datas.getcal_sett());
      //
      //          EEPROM.write(1 , (uint8_t)(100 * ((float)datas.getrate_sett() - (uint8_t)datas.getrate_sett())));
      //          EEPROM.write(3 , (uint8_t)(100 * ((float)datas.getwidth_sett() - (uint8_t)datas.getwidth_sett())));
      //          EEPROM.write(5 , (uint8_t)(100 * ((float)datas.getcal_sett() - (uint8_t)datas.getcal_sett())));
      //
      //          EEPROM.commit();
      //
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 2 and kp.KeypadsPress())
      //        {
      //
      //          EEPROM.write(6 , (uint8_t)datas.getrate_sett());
      //          EEPROM.write(8 , (uint8_t)datas.getwidth_sett());
      //          EEPROM.write(10 , (uint8_t)datas.getcal_sett());
      //
      //          EEPROM.write(7 , (uint8_t)(100 * ((float)datas.getrate_sett() - (uint8_t)datas.getrate_sett())));
      //          EEPROM.write(9 , (uint8_t)(100 * ((float)datas.getwidth_sett() - (uint8_t)datas.getwidth_sett())));
      //          EEPROM.write(11 , (uint8_t)(100 * ((float)datas.getcal_sett() - (uint8_t)datas.getcal_sett())));
      //
      //          EEPROM.commit();
      //
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 3 and kp.KeypadsPress())
      //        {
      //          EEPROM.write(12 , (uint8_t)datas.getrate_sett());
      //          EEPROM.write(14 , (uint8_t)datas.getwidth_sett());
      //          EEPROM.write(16 , (uint8_t)datas.getcal_sett());
      //
      //          EEPROM.write(13 , (uint8_t)(100 * ((float)datas.getrate_sett() - (uint8_t)datas.getrate_sett())));
      //          EEPROM.write(15 , (uint8_t)(100 * ((float)datas.getwidth_sett() - (uint8_t)datas.getwidth_sett())));
      //          EEPROM.write(17 , (uint8_t)(100 * ((float)datas.getcal_sett() - (uint8_t)datas.getcal_sett())));
      //
      //          EEPROM.commit();
      //
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 4 and kp.KeypadsPress())
      //        {
      //          EEPROM.write(18 , (uint8_t)datas.getrate_sett());
      //          EEPROM.write(20 , (uint8_t)datas.getwidth_sett());
      //          EEPROM.write(22 , (uint8_t)datas.getcal_sett());
      //
      //          EEPROM.write(19 , (uint8_t)(100 * ((float)datas.getrate_sett() - (uint8_t)datas.getrate_sett())));
      //          EEPROM.write(21 , (uint8_t)(100 * ((float)datas.getwidth_sett() - (uint8_t)datas.getwidth_sett())));
      //          EEPROM.write(23 , (uint8_t)(100 * ((float)datas.getcal_sett() - (uint8_t)datas.getcal_sett())));
      //
      //          EEPROM.commit();
      //
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 5 and kp.KeypadsPress())
      //        {
      //          EEPROM.write(24 , (uint8_t)datas.getrate_sett());
      //          EEPROM.write(26 , (uint8_t)datas.getwidth_sett());
      //          EEPROM.write(28 , (uint8_t)datas.getcal_sett());
      //
      //          EEPROM.write(25 , (uint8_t)(100 * ((float)datas.getrate_sett() - (uint8_t)datas.getrate_sett())));
      //          EEPROM.write(27 , (uint8_t)(100 * ((float)datas.getwidth_sett() - (uint8_t)datas.getwidth_sett())));
      //          EEPROM.write(29 , (uint8_t)(100 * ((float)datas.getcal_sett() - (uint8_t)datas.getcal_sett())));
      //
      //          EEPROM.commit();
      //
      //          kp.count = 0;
      //          break;
      //        }
      //        else if (cursur == 6 and kp.KeypadsPress())
      //        {
      //          EEPROM.write(30 , (uint8_t)datas.getrate_sett());
      //          EEPROM.write(32 , (uint8_t)datas.getwidth_sett());
      //          EEPROM.write(34 , (uint8_t)datas.getcal_sett());
      //
      //          EEPROM.write(31 , (uint8_t)(100 * ((float)datas.getrate_sett() - (uint8_t)datas.getrate_sett())));
      //          EEPROM.write(33 , (uint8_t)(100 * ((float)datas.getwidth_sett() - (uint8_t)datas.getwidth_sett())));
      //          EEPROM.write(35 , (uint8_t)(100 * ((float)datas.getcal_sett() - (uint8_t)datas.getcal_sett())));
      //
      //          EEPROM.commit();
      //
      //          kp.count = 0;
      //          break;
      //        }
      //      }
    }
    float calculator(){
      //      float slope = 0.02 * (datas.getfeed_calibated_2() - datas.getfeed_calibated_1());
      //      float offset = datas.getfeed_calibated_1() - (25 * slope);
      //
      //      Serial.println("slope");
      //      Serial.println(datas.getfeed_calibated_2());
      //      Serial.println(datas.getfeed_calibated_1());
      //      Serial.println("offset");
      //      Serial.println(datas.getfeed_calibated_1());
      //      Serial.println("END");
      //
      //      float feed = datas.get_rate() * datas.get_width() * datas.get_speed() / 5.76;
      //
      //      datas.set_feed(feed);
      //      datas.setcal_sett(feed);
      //      Serial.println("feed");
      //      Serial.println(datas.get_rate());
      //      Serial.println(datas.get_width());
      //      Serial.println("offset");
      //      Serial.println(datas.get_speed());
      //      Serial.println("END");
      //
      //      float pwm = slope * feed - offset;
      //
      //      Serial.println("CAL");
      //      Serial.println(slope);
      //      Serial.println(feed);
      //      Serial.println(offset);
      //      Serial.println(pwm);
      //      Serial.println("END");
      //      if (pwm < 0) pwm = 0;
      //      return pwm;
    }
    void run_pwm(float pwm){
      const int ledPin = 12;
      const int freq = 5000;
      const int ledChannel = 0;
      const int resolution = 12;

      ledcSetup(ledChannel, freq, resolution);
      ledcAttachPin(ledPin, ledChannel);
      pwm = pwm <= 0 ? 0 : pwm;
      pwm = pwm >= 100 ? 100 : pwm;
      uint16_t buffer_pwm = map(pwm, 0, 100, 0, 4095);
      Serial.println(buffer_pwm);
      ledcWrite(ledChannel, buffer_pwm);
    }
};

Menu menu;

void setup() {

  Serial.begin(115200);
  //  Serial2.begin(38400);

  lcd.begin();
  EEPROM.begin(64);

  pinMode(run_bt, INPUT);
  
  xTaskCreate(
    taskTwo,          /* Task function. */
    "TaskTwo",        /* String with name of task. */
    20000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */

}

void loop() {
  
  char tmp = keypads.getKey();
  if (tmp - '0' == 18) {
    menu.cursur--;
  } else if (tmp - '0' == 19) {
    menu.cursur++;
  }
  menu.menucursor();
  menu.pagechange();

  if (tmp - '0' == 20) {
    switch (menu.cursur_select) {
      case 1: menu.calibation(); menu.pages(2); cs.setY(0); break;
      case 2: menu.settiing();   menu.pages(2); cs.setY(0); break;
      case 3: menu.drain();      menu.pages(2); cs.setY(0); break;
      case 4: menu.program();    menu.pages(2); cs.setY(0); break;
      case 5: menu.setDefult();  menu.pages(2); cs.setY(0); break;
      case 6: menu.save();       menu.pages(2); cs.setY(0); break;
      default:                                              break;
    }
  }
  delay(200);
}


void taskTwo( void * parameter ) {
  while (1) {
    if (Serial2.available()) {
      String s = Serial2.readStringUntil('\n');
      if (s.startsWith("$GPRMC")) {
        int posHeader = s.indexOf("$GPRMC");
        if (posHeader == 0) { // Got "$GPRMC"

          if (s.length() >= 69) {
            //            Serial.println("That's a perfectly acceptable text message");
            s.trim();
            //            Serial.println(s);
            int posHour = s.indexOf(',');// Get first occurance of comma ","
            //            Serial.print("Position of First Comma: ");
            //            Serial.println(posHour);

            gpsHour = s.substring(posHour + 1, posHour + 3); // Get GPS Hours from String
            //            Serial.print("GPS Hour: ");
            //            Serial.println(gpsHour);
          }
          else {
            //            Serial.println(s);
          }
        }
      }
    }
    vTaskDelay(1000);
  }
  vTaskDelete( NULL );
}
