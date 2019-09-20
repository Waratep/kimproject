#include <string.h>
#include <ctype.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
int cont  = 0;
int bien  = 0;
int conta = 0;
int indices[13];

#define buzzer 2
#define run_bt 34
#define red 23
#define blue 5
#define run_bt 34
#define eepromsize 64

uint8_t state_run = 0;
String gpsHour;
uint8_t gpsActive = 0;
#define frate 100

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

class Save {
  private:
    uint8_t index25_1;
    uint8_t index75_1;
    uint8_t index25_2;
    uint8_t index75_2;
    uint8_t savecouter;
  public:
    int saveList;
    int saveListEdit;
    Save() {
      index25_1   = 0;
      index25_2   = 16;
      index75_1   = 32;
      index75_2   = 48;

      saveList     = 0;
      saveListEdit = 0;
    }
    float saved25(float data25, int indexlist) {
      uint8_t INT;
      uint8_t FLOAT;
      INT   = (uint8_t)(data25);
      FLOAT = (uint8_t)((data25 - INT) * 100);
      EEPROM.write(index25_1 + indexlist, INT);
      EEPROM.write(index25_2 + indexlist, FLOAT);

      EEPROM.commit();

    }
    float saved75(float data75, int indexlist) {
      uint8_t INT;
      uint8_t FLOAT;
      INT   = (uint8_t)(data75);
      FLOAT = (uint8_t)(data75 - INT) * 100;
      EEPROM.write(index75_1 + indexlist, data75);
      EEPROM.write(index75_2 + indexlist, data75);

      EEPROM.commit();

    }
    float get25() {
      return (EEPROM.read(index25_1 + saveList) + (float)EEPROM.read(index25_2 + saveList) / 100);
    }
    float load25(int index) {
      return (EEPROM.read(index25_1 + index) + (float)EEPROM.read(index25_2 + index) / 100);
    }
    float load75(int index) {
      return (EEPROM.read(index75_1 + index) + (float)EEPROM.read(index75_2 + index) / 100);
    }
};

Save save;

class Data {
  private:
    float _rate;
    float _speed;
    float _simspeed;
    float _width;
    float _simfeed;
    uint8_t _simpwm;
    float _cal25;
    float _cal75;

  public:
    Data()
    {
      _cal25 = 0;
      _cal75 = 0;
      _rate  = 0;
      _speed = 0;
      _width = 0;

      _simspeed = 0;
      _simpwm   = 0;
      _simfeed  = 0;
    }
    float get_cal25() {
      return _cal25;
    }
    float get_cal75() {
      return _cal75;
    }
    float set_cal25(float _cal) {
      _cal25 = _cal;
    }
    float set_cal75(float _cal) {
      _cal75 = _cal;
    }
    float get_simspeed() {
      return _simspeed;
    }
    float set_simspeed(float simspeed) {
      _simspeed = simspeed;
    }
    uint8_t get_simpwm() {
      return _simpwm;
    }
    uint8_t set_simpwm(uint8_t simpwm) {
      _simpwm = simpwm;
    }
    float get_simfeed() {
      return _simfeed;
    }
    float set_simfeed(float simfeed) {
      _simfeed = simfeed;
    }
    float get_rate() {
      return _rate;
    }
    float set_rate(float rate) {
      _rate = rate;
    }
    float get_speed() {
      return _speed;
    }
    float set_speed(float speed) {
      _speed = speed;
    }
    float get_width() {
      return _width;
    }
    float set_width(float width) {
      _width = width;
    }


};

Data datas;

class Menu {
  public:
    int slidingpage;
    int last_slidingpage;
    uint8_t page;
    uint8_t last_page;
    uint8_t cursur_select;
    uint8_t subpage;
    uint8_t lastsubpage;
    float cursur;
    Menu() {
      last_slidingpage = 0;
      slidingpage   = 0;
      page          = 0;
      subpage       = 0;
      cursur        = 0;
      last_page     = 0;
      cursur_select = 0;
    }
    void menucursor() {
      clearCursor();
      updateCursor();
    }
    void clearCursor() {
      for (uint8_t i = 0 ; i < 4 ; i++) {
        lcd.setCursor(19, i);
        lcd.print(" ");
      }
    }
    void updateCursor() {
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
    void main_page_1() {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU:RUNNING");
      lcd.setCursor(0, 1);
      lcd.print("RATE : ");
      lcd.print(datas.get_rate());
      lcd.setCursor(15, 1);
      lcd.print("kg/r");
      lcd.setCursor(0, 2);
      lcd.print("VEL  : ");
      lcd.print(datas.get_speed());
      lcd.setCursor(15, 2);
      lcd.print("km/h");

      page = 2;

    }
    void main_page_2() {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU:SETUP");
      lcd.setCursor(0, 1);
      lcd.print("RATE  : ");
      lcd.print(datas.get_rate());
      lcd.setCursor(15, 1);
      lcd.print("kg/r");
      lcd.setCursor(0, 2);
      lcd.print("WIDTH : ");
      lcd.print(datas.get_width());
      lcd.setCursor(15, 2);
      lcd.print("m");
      lcd.setCursor(0, 3);
      lcd.print("PROGRAM[ ]");
      lcd.setCursor(8, 3);
      lcd.print(save.saveList);
      lcd.setCursor(11, 3);
      lcd.print(save.get25());
      page = 3;
    }
    void main_page_3() {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU:SIM");
      lcd.setCursor(0, 1);
      lcd.print("VEL  : ");
      lcd.print(datas.get_simspeed());
      lcd.setCursor(15, 1);
      lcd.print("km/h");
      lcd.setCursor(0, 2);
      lcd.print("PWM  : ");
      lcd.print(datas.get_simpwm());
      lcd.setCursor(15, 2);
      lcd.print("%");
      lcd.setCursor(0, 3);
      lcd.print("FEED : ");
      lcd.print(datas.get_simfeed());
      lcd.setCursor(15, 3);
      lcd.print("kg/r");
      page = 4;
    }
    void main_page_4() {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU:DRAIN");
      lcd.setCursor(1, 2);
      lcd.print("PRESS ENT TO DRAIN");
      page = 4;
    }
    void main_subpage_2(int whocall) {
      if (whocall == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("MENU:SETUP");
        lcd.setCursor(0, 1);
        lcd.print("PROGRAM EDIT : ");
        lcd.print("    ");
        lcd.setCursor(15, 1);
        lcd.print(save.saveListEdit);
        lcd.setCursor(0, 2);
        lcd.print("ABOUT ME");
      } else if (whocall == 1) {
        lcd.setCursor(15, 1);
        lcd.print("    ");
        lcd.setCursor(15, 1);
        lcd.print(save.saveListEdit);
      }
      page = 3;
    }
    uint8_t runing(int _curcur) { 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU:RUNNING");
      lcd.setCursor(0, 1);
      lcd.print("RATE : ");
      lcd.print(datas.get_rate());
      lcd.setCursor(15, 1);
      lcd.print("kg/r");
      lcd.setCursor(0, 2);
      lcd.print("VEL  : ");
      lcd.print(datas.get_speed());
      lcd.setCursor(15, 2);
      lcd.print("km/h");

      uint8_t cursur = _curcur;
      char tmp;

      while (1) {
        tmp = keypads.getKey();
        if(tmp) {
          digitalWrite(buzzer,1);
          delay(20);
          digitalWrite(buzzer,0);
        } 
        if (tmp - '0' == 18) {
          cursur--;
        } else if (tmp - '0' == 19) {
          cursur++;
        }
        if (tmp - '0' == -6) break;
        cursur = cursur >= 2 ? 2 : cursur;
        cursur = cursur <= 0 ? 1 : cursur;
        clearCursor();
        lcd.setCursor(19, cursur);
        lcd.print("<");
        if (_curcur == 1)
        {
          float feed;
          char tmp = keypads.getKey();
          float val = 0 , counter = 0;
          while (tmp - '0' != 20)
          {
            delay(frate);
            tmp = keypads.getKey();
            if(tmp) {
              digitalWrite(buzzer,1);
              delay(20);
              digitalWrite(buzzer,0); 
            }
            if (tmp and tmp - '0' != 20) {
              if (counter == 0) val += (tmp - '0') * 1000;
              if (counter == 1) val += (tmp - '0') * 100;
              if (counter == 2) val += (tmp - '0') * 10;
              if (counter == 3) val += (tmp - '0') * 1;
              counter++;
            }
            lcd.setCursor(0, 1);
            lcd.print("RATE : ");
            lcd.print("        ");
            lcd.setCursor(7, 1);
            lcd.print(val / 10);
            lcd.setCursor(15, 1);
            lcd.print("kg/r");
          }
          datas.set_rate(val / 10);
          break;
        }
        else if (_curcur == 2)
        {
          //          float feed;
          //          char tmp = keypads.getKey();
          //          float val = 0 , counter = 0;
          //          while (tmp - '0' != 20)
          //          {
          //            delay(frate);
          //            tmp = keypads.getKey();
          //            if (tmp and tmp - '0' != 20) {
          //              if (counter == 0) val += (tmp - '0') * 1000;
          //              if (counter == 1) val += (tmp - '0') * 100;
          //              if (counter == 2) val += (tmp - '0') * 10;
          //              if (counter == 3) val += (tmp - '0') * 1;
          //              counter++;
          //            }
          //            lcd.setCursor(0, 2);
          //            lcd.print("VEL  : ");
          //            lcd.print("        ");
          //            lcd.setCursor(7, 2);
          //            lcd.print(val / 10);
          //            lcd.setCursor(15, 2);
          //            lcd.print("km/h");
          //          }
          break;
        }
        if ((tmp - '0' == -6) or (tmp - '0' == 20)) {
          break;
        }

        delay(frate);
      }
    }
    uint8_t sim(int _curcur) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MENU:SIM");
      lcd.setCursor(0, 1);
      lcd.print("VEL  : ");
      lcd.print(datas.get_simspeed());
      lcd.setCursor(15, 1);
      lcd.print("km/h");
      lcd.setCursor(0, 2);
      lcd.print("PWM  : ");
      lcd.print(datas.get_simpwm());
      lcd.setCursor(15, 2);
      lcd.print("%");
      lcd.setCursor(0, 3);
      lcd.print("FEED : ");
      lcd.print(datas.get_simfeed());
      lcd.setCursor(15, 3);
      lcd.print("kg/r");

      uint8_t cursur = _curcur;
      char tmp;

      while (1) {
        if(tmp) {
          digitalWrite(buzzer,1);
          delay(20);
          digitalWrite(buzzer,0); 
        }
        tmp = keypads.getKey();
        if (tmp - '0' == 18) {
          cursur--;
        } else if (tmp - '0' == 19) {
          cursur++;
        }
        if (tmp - '0' == -6) break;
        cursur = cursur >= 3 ? 3 : cursur;
        cursur = cursur <= 0 ? 1 : cursur;
        clearCursor();
        lcd.setCursor(19, cursur);
        lcd.print("<");
        if (_curcur == 1)
        {
          float feed;
          char tmp = keypads.getKey();
          float val = 0 , counter = 0;
          while (tmp - '0' != 20)
          {
            delay(frate);
            tmp = keypads.getKey();
            if(tmp) {
              digitalWrite(buzzer,1);
              delay(20);
              digitalWrite(buzzer,0);
              
            }
            if (tmp and tmp - '0' != 20) {
              if (counter == 0) val += (tmp - '0') * 1000;
              if (counter == 1) val += (tmp - '0') * 100;
              if (counter == 2) val += (tmp - '0') * 10;
              if (counter == 3) val += (tmp - '0') * 1;
              counter++;
            }
            lcd.setCursor(0, 1);
            lcd.print("VEL  : ");
            lcd.print("        ");
            lcd.setCursor(7, 1);
            lcd.print(val / 10);
            lcd.setCursor(15, 1);
            lcd.print("km/h");
          }
          datas.set_simspeed(val / 10);
          break;
        }
        else if (_curcur == 2)
        {
          float feed;
          char tmp = keypads.getKey();
          float val = 0 , counter = 0;
          while (tmp - '0' != 20)
          {
            delay(frate);
            tmp = keypads.getKey();
            if(tmp) {
              digitalWrite(buzzer,1);
              delay(20);
              digitalWrite(buzzer,0); 
            }
            if (tmp and tmp - '0' != 20) {
              if (counter == 0) val += (tmp - '0') * 100;
              if (counter == 1) val += (tmp - '0') * 10;
              if (counter == 2) val += (tmp - '0') * 1;
              counter++;
            }
            lcd.setCursor(0, 2);
            lcd.print("PWM  : ");
            lcd.print("        ");
            lcd.setCursor(7, 2);
            lcd.print(val / 10);
            lcd.setCursor(15, 2);
            lcd.print("%");
          }
          datas.set_simpwm(val / 10);
          break;
        }
        else if (_curcur == 3)
        {
          float feed;
          char tmp = keypads.getKey();
          float val = 0 , counter = 0;
          while (tmp - '0' != 20)
          {
            delay(frate);
            tmp = keypads.getKey();
            if(tmp) {
              digitalWrite(buzzer,1);
              delay(20);
              digitalWrite(buzzer,0);
            }
            if (tmp and tmp - '0' != 20) {
              if (counter == 0) val += (tmp - '0') * 1000;
              if (counter == 1) val += (tmp - '0') * 100;
              if (counter == 2) val += (tmp - '0') * 10;
              if (counter == 3) val += (tmp - '0') * 1;
              counter++;
            }
            lcd.setCursor(0, 3);
            lcd.print("FEED : ");
            lcd.print("      ");
            lcd.setCursor(7, 3);
            lcd.print(val / 10);
            lcd.setCursor(15, 3);
            lcd.print("km/r");
          }
          datas.set_simfeed(val / 10);
          break;
        }
        if ((tmp - '0' == -6) or (tmp - '0' == 20)) {
          break;
        }

        delay(frate);
      }
      subpage = 1;
    }
    uint8_t setups(int _curcur) {
      lcd.clear();
      if (_curcur >= 0 and _curcur <= 3) {
        lcd.setCursor(0, 0);
        lcd.print("MENU:SETUP");
        lcd.setCursor(0, 1);
        lcd.print("RATE  : ");
        lcd.print(datas.get_rate());
        lcd.setCursor(15, 1);
        lcd.print("kg/r");
        lcd.setCursor(0, 2);
        lcd.print("WIDTH : ");
        lcd.print(datas.get_width());
        lcd.setCursor(15, 2);
        lcd.print("m");
        lcd.setCursor(0, 3);
        lcd.print("PROGRAM[ ]");
        lcd.setCursor(8, 3);
        lcd.print(save.saveList);
        lcd.setCursor(11, 3);
        lcd.print(save.get25());
      } else if (_curcur >= 4 and _curcur <= 5) {
        lcd.setCursor(0, 0);
        lcd.print("MENU:SETUP");
        lcd.setCursor(0, 1);
        lcd.print("PROGRAM EDIT : ");
        lcd.print("    ");
        lcd.setCursor(15, 1);
        lcd.print(save.saveList);
        lcd.setCursor(0, 2);
        lcd.print("ABOUT ME");
      }
      uint8_t cursur = _curcur;
      char tmp;
      while (1) {
        tmp = keypads.getKey();
        if(tmp) {
          digitalWrite(buzzer,1);
          delay(20);
          digitalWrite(buzzer,0);
        }
        if (tmp - '0' == 18) {
          cursur--;
        } else if (tmp - '0' == 19) {
          cursur++;
        }
        if (tmp - '0' == -6) break;
        cursur = cursur >= 2 ? 2 : cursur;
        cursur = cursur <= 0 ? 1 : cursur;
        clearCursor();
        lcd.setCursor(19, cursur);
        lcd.print("<");
        if (_curcur == 1) {
          float feed;
          char tmp = keypads.getKey();
          float val = 0 , counter = 0;
          while (tmp - '0' != 20) {
            delay(frate);
            tmp = keypads.getKey();
            if(tmp) {
              digitalWrite(buzzer,1);
              delay(20);
              digitalWrite(buzzer,0);
            }
            if (tmp and tmp - '0' != 20) {
              if (counter == 0) val += (tmp - '0') * 1000;
              if (counter == 1) val += (tmp - '0') * 100;
              if (counter == 2) val += (tmp - '0') * 10;
              if (counter == 3) val += (tmp - '0') * 1;
              counter++;
            }
            lcd.setCursor(0, 1);
            lcd.print("RATE  : ");
            lcd.print("        ");
            lcd.setCursor(8, 1);
            lcd.print(val / 10);
            lcd.setCursor(15, 1);
            lcd.print("kg/r");
          }
          datas.set_rate(val / 10);
          break;
        }
        else if (_curcur == 2) {
          float feed;
          char tmp = keypads.getKey();
          float val = 0 , counter = 0;
          while (tmp - '0' != 20) {
            delay(frate);
            tmp = keypads.getKey();
            if(tmp) {
              digitalWrite(buzzer,1);
              delay(20);
              digitalWrite(buzzer,0);
            }
            if (tmp and tmp - '0' != 20) {
              if (counter == 0) val += (tmp - '0') * 1000;
              if (counter == 1) val += (tmp - '0') * 100;
              if (counter == 2) val += (tmp - '0') * 10;
              if (counter == 3) val += (tmp - '0') * 1;
              counter++;
            }
            lcd.setCursor(0, 2);
            lcd.print("WIDTH : ");
            lcd.print("        ");
            lcd.setCursor(8, 2);
            lcd.print(val / 10);
            lcd.setCursor(15, 2);
            lcd.print("m");
          }
          datas.set_width(val / 10);
          break;
        }
        else if (_curcur == 3) {
          Serial.println("saved!");
          lcd.setCursor(8, 3);
          lcd.print(save.saveList);
          lcd.setCursor(11, 3);
          lcd.print(save.get25());
          datas.set_cal25(save.load25(save.saveList));
          datas.set_cal75(save.load75(save.saveList));
          break;
        }
        else if (_curcur == 4) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("MENU:PROGRAM EDIT");
          lcd.setCursor(0, 1);
          lcd.print("CAL@25 : ");
          lcd.print(save.load25(save.saveListEdit));
          lcd.setCursor(0, 2);
          lcd.print("CAL@75 : ");
          lcd.print(save.load75(save.saveListEdit));
          cursur = 0;
          while (1) {
            delay(frate);
            tmp = keypads.getKey();
            if(tmp) {
              digitalWrite(buzzer,1);
              delay(20);
              digitalWrite(buzzer,0); 
            }
            if (tmp - '0' == 18) {
              cursur--;
            } else if (tmp - '0' == 19) {
              cursur++;
            }
            if (tmp - '0' == -6) break;
            cursur = cursur >= 2 ? 2 : cursur;
            cursur = cursur <= 0 ? 1 : cursur;
            clearCursor();
            lcd.setCursor(19, cursur);
            lcd.print("<");
            if (cursur == 1 and tmp - '0' == 20) {
              char tmp = keypads.getKey();
              float val = 0 , counter = 0;
              while (tmp - '0' != 20) {
                delay(frate);
                tmp = keypads.getKey();
                if(tmp) {
                  digitalWrite(buzzer,1);
                  delay(20);
                  digitalWrite(buzzer,0); 
                }
                if (tmp and tmp - '0' != 20) {
                  if (counter == 0) val += (tmp - '0') * 1000;
                  if (counter == 1) val += (tmp - '0') * 100;
                  if (counter == 2) val += (tmp - '0') * 10;
                  if (counter == 3) val += (tmp - '0') * 1;
                  counter++;
                }
                lcd.setCursor(9, 1);
                lcd.print("        ");
                lcd.setCursor(9, 1);
                lcd.print(val / 10);
              }
              save.saved25(val / 10, save.saveListEdit);
            } else if (cursur == 2 and tmp - '0' == 20) {
              char tmp = keypads.getKey();
              float val = 0 , counter = 0;
              while (tmp - '0' != 20) {
                delay(frate);
                tmp = keypads.getKey();
                if(tmp) {
                  digitalWrite(buzzer,1);
                  delay(1);
                  digitalWrite(buzzer,0); 
                }
                if (tmp and tmp - '0' != 20) {
                  if (counter == 0) val += (tmp - '0') * 1000;
                  if (counter == 1) val += (tmp - '0') * 100;
                  if (counter == 2) val += (tmp - '0') * 10;
                  if (counter == 3) val += (tmp - '0') * 1;
                  counter++;
                }
                lcd.setCursor(9, 2);
                lcd.print("        ");
                lcd.setCursor(9, 2);
                lcd.print(val / 10);
              }
              save.saved75(val / 10, save.saveListEdit);
            }
          }
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("MENU:SETUP");
          lcd.setCursor(0, 1);
          lcd.print("PROGRAM EDIT : ");
          lcd.print("    ");
          lcd.setCursor(15, 1);
          lcd.print(save.saveListEdit);
          lcd.setCursor(0, 2);
          lcd.print("ABOUT ME");
          break;
        }
        else if (_curcur == 5) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("MENU:ABOUT ME");
          lcd.setCursor(0, 1);
          lcd.print("VERSION 0.0.1");
          lcd.setCursor(0, 2);
          lcd.print("KMITL");
          lcd.setCursor(0, 3);
          lcd.print("ETC.");

          while (keypads.getKey() - '0' != -6) {
          }
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("MENU:SETUP");
          lcd.setCursor(0, 1);
          lcd.print("PROGRAM EDIT : ");
          lcd.print("    ");
          lcd.setCursor(15, 1);
          lcd.print(save.saveList);
          lcd.setCursor(0, 2);
          lcd.print("ABOUT ME");
          break;
        }
        if ((tmp - '0' == -6) or (tmp - '0' == 20)) {
          break;
        }
        delay(frate);
      }
    }
    uint8_t subpage_setup(int _curcur) {
      lcd.setCursor(8, 3);
      lcd.print(save.saveList);
      lcd.setCursor(11, 3);
      lcd.print(save.get25());
      Serial.println(save.get25());

    }
    uint8_t drain() {
//      lcd.clear();
//      lcd.setCursor(0, 0);
//      lcd.print("MENU:DRAIN");
//      lcd.setCursor(5, 2);
//      lcd.print("DRAIN....");
      char tmp;
      uint8_t state_run = 0;
      uint8_t val = 0;
      while (1) {
        tmp = keypads.getKey();
        if(tmp) {
          digitalWrite(buzzer,1);
          delay(20);
          digitalWrite(buzzer,0);
          
        }
        if (tmp - '0' == -6) {
          return 2;
//          slidingpage--;
//          break;
        }
        if (tmp - '0' == -13) {
          return 3;
//          slidingpage++;
//          break;
        }
        if (tmp - '0' == 20) break;
//        menu.slidingpage
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("MENU: DRAIN");
        lcd.setCursor(3, 2);
        lcd.print("DRAING....[");
        lcd.print(val);
        lcd.print("%]");

        if (!digitalRead(run_bt) and state_run == 0) {
          state_run = 1;
          for (uint8_t duty = 0 ; duty < 51 ; duty++) {
            tmp = keypads.getKey();
            if(tmp) {
              digitalWrite(buzzer,1);
              delay(20);
              digitalWrite(buzzer,0);
            }
            if (tmp - '0' == -6) break;
            if (tmp - '0' == 20) break;
            if (digitalRead(run_bt)) {
              delay(frate * 5);
              break;
            }
            run_pwm(duty);
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("MENU: DRAIN");
            lcd.setCursor(3, 2);
            lcd.print("DRAING....[");
            lcd.print(duty);
            lcd.print("%]");
            delay(frate * 5);
          }
          while (!digitalRead(run_bt));
        }
        else if (digitalRead(run_bt) and state_run == 1) {
          state_run = 0;
        }
        delay(frate * 2);
      }
      if (!digitalRead(run_bt)) {
        delay(frate * 5);
        while (!digitalRead(run_bt)) {
          lcd.clear();
          lcd.setCursor(4, 2);
          lcd.print("TURN OFF PWM!");
          delay(frate * 5);
        }
      }
      run_pwm(0);
      subpage = 3;
    }
    void run_pwm(float pwm) {
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
  Serial2.begin(38400, SERIAL_8N1, 16, 17);

  lcd.begin();
  EEPROM.begin(eepromsize);
  //  for(int i = 0; i < 64; i++) EEPROM.write(i,0);
  pinMode(run_bt, INPUT);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(buzzer , OUTPUT);

  xTaskCreate(
    taskTwo,          /* Task function. */
    "TaskTwo",        /* String with name of task. */
    20000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
  menu.main_page_1();
}

void loop() {

  char tmp = keypads.getKey();
  if(tmp) {
    Serial.println(tmp - '0');
    digitalWrite(buzzer,1);
    delay(20);
    digitalWrite(buzzer,0);
  }
  
  menu.last_slidingpage = menu.slidingpage;
  menu.lastsubpage      = menu.subpage;
  switch (tmp - '0') {
    case -6  :
      if (menu.slidingpage == 1 and menu.cursur == 3) {
        save.saveList--;
        if (save.saveList < 0) save.saveList = 0;
        menu.subpage_setup(save.saveListEdit);
      } else if (menu.slidingpage == 1 and menu.cursur == 4) {
        save.saveListEdit--;
        if (save.saveListEdit < 0) save.saveListEdit = 0;
        menu.main_subpage_2(1);
      } else {
        menu.slidingpage--;
      }

      break;
    case -13 :
      if (menu.slidingpage == 1 and menu.cursur == 3) {
        save.saveList++;
        if (save.saveList > 9) save.saveList = 9;
        menu.subpage_setup(save.saveList);
      } else if (menu.slidingpage == 1 and menu.cursur == 4) {
        save.saveListEdit++;
        if (save.saveListEdit > 9) save.saveListEdit = 9;
        menu.main_subpage_2(1);
      } else {
        menu.slidingpage++;
      }
      break;
    case  18 : menu.cursur--;      break;
    case  19 : menu.cursur++;      break;
    default  :                     break;
  }
  if (menu.slidingpage > 3) menu.slidingpage = 0;
  if (menu.slidingpage < 0) menu.slidingpage = 3;

  switch (menu.slidingpage) {
    case 0:
      if (menu.cursur < 0) menu.cursur = 0;
      if (menu.cursur > 2) menu.cursur = 2;
      menu.menucursor();

      if (digitalRead(run_bt) and state_run == 0) {
        float slope = 0.02 * (datas.get_cal75() - datas.get_cal25());
        float offset = datas.get_cal25() - (25 * slope);
        float feed = datas.get_rate() * datas.get_width() * datas.get_speed() / 5.76;
        float pwm = slope * feed - offset;
        menu.run_pwm(pwm);
        state_run = 1;
        Serial.println("run pwm");
      }
      else if (!digitalRead(run_bt) and state_run == 1) {
        menu.run_pwm(0);
        state_run = 0;
        Serial.println("not run pwm");
      }

      break;
    case 1:
      if (menu.cursur < 0) menu.cursur = 0;
      if (menu.cursur > 5) menu.cursur = 5;
      if (menu.cursur >= 0 and menu.cursur <= 3) menu.subpage = 0;
      if (menu.cursur >= 4 and menu.cursur <= 5) menu.subpage = 1;
      menu.menucursor();
      menu.run_pwm(0);
      break;
    case 2:
      if (menu.cursur < 0) menu.cursur = 0;
      if (menu.cursur > 3) menu.cursur = 3;
      menu.menucursor();

      if (digitalRead(run_bt) and state_run == 0) {
        float slope = 0.02 * (datas.get_cal75() - datas.get_cal25());
        float offset = datas.get_cal25() - (25 * slope);
        float feed = datas.get_rate() * datas.get_width() * datas.get_speed() / 5.76;
        float pwm = slope * feed - offset;
        menu.run_pwm(pwm);
        state_run = 1;
        Serial.println("run pwm");
      }
      else if (!digitalRead(run_bt) and state_run == 1) {
        menu.run_pwm(0);
        state_run = 0;
        Serial.println("not run pwm");
      }
      break;
    default:
      break;
  }

  if ((menu.last_slidingpage != menu.slidingpage) or (menu.subpage != menu.lastsubpage)) {
    switch (menu.slidingpage) {
      case 0: menu.main_page_1();
        break;
      case 1:
        menu.run_pwm(0);
        switch (menu.subpage) {
          case 0 : menu.main_page_2();     break;
          case 1 : menu.main_subpage_2(0); break;
          default:                         break;
        }
        break;
      case 2: menu.main_page_3();
        break;
      case 3: menu.main_page_4();
        break;
      default:
        break;
    }
  }

  if (tmp - '0' == 20 and menu.slidingpage == 0) {
    menu.runing(menu.cursur);
  } else if (tmp - '0' == 20 and menu.slidingpage == 1) {
    menu.setups(menu.cursur);
  } else if (tmp - '0' == 20 and menu.slidingpage == 2) {
    menu.sim(menu.cursur);
  } else if (menu.slidingpage == 3) {
    uint8_t tmp = menu.drain();
    if(tmp == 2) {
      menu.slidingpage--;
      menu.main_page_3();
    }
    if(tmp == 3) menu.slidingpage++;
//    menu.main_page_4();
  }

  delay(frate);
}

void taskTwo( void * parameter ) {
  while (1) {
    if (Serial2.available()) {
      String s = Serial2.readStringUntil('\n');
      if (s.startsWith("$GPRMC")) {
        int posHeader = s.indexOf("$GPRMC");
        if (posHeader == 0) { // Got "$GPRMC"
//          if (s.length() >= 69) {
            s.trim();
            int counter = 0;
            String vel  = "";
            float  vel_float = 0;
            for (int i = 0 ; i < s.length() ; i++){
              if(s[i] == ',') counter++;
              if(counter >=7  and counter < 8){
                if(s[i] != ',') vel += s[i];
              }
              Serial.print(s[i]);
            }
            Serial.println();
            if (vel == "") {
              vel_float = -1;
            }else{
              vel_float = vel.toFloat() * 1.852;
            }
            datas.set_speed(vel_float);
            Serial.println(vel_float);
            gpsActive = 1;
          }
//          else {
//            Serial.println(s);
//            gpsActive = 0;
//          }
        }
//      }
      if (datas.get_speed() != -1){
        digitalWrite(blue,LOW);
        digitalWrite(red,HIGH);
      }else{
        digitalWrite(blue,HIGH);
        digitalWrite(red,LOW);        
      }
    }
    vTaskDelay(100);
  }
  vTaskDelete( NULL );
}
