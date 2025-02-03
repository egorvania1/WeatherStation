#include <Wire.h>
#include <SPI.h>
#include <RTC.h>
#include <BME280I2C.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>
#include <GyverPower.h>

//Подключение дисплея
#define __CS 10      //Порт CS                                     
#define __DC 9       //Порт A                                    
#define __RST 8      //Порт RESET                                     
TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC, __RST); 

static DS3231 RTC; //Подключение времени 

BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
BME280::PresUnit presUnit(BME280::PresUnit_hPa);
float temp(NAN), hum(NAN), pres(NAN);

//Цвета
#define BLACK 0x0000

int TextSize = 2;          //Размер текста
int T1 = 0;
int T2 = 0;
int H = 0;
int P = 0;
int hours = 0;
int minutes = 0;
int week = 0;
int day = 0;
int month = 0;
int year = 0;

BME280I2C bme; //Подключение датчика температуры
void setup() {

  Wire.begin(); //Запускаем библиотеку для BME280
  tft.begin();  //Запускаем дисплей
  RTC.begin();  //Запускаем время

  //Калибровка сна
  power.calibrate(power.getMaxTimeout());
  power.setSleepMode(STANDBY_SLEEP);
  
  tft.fillScreen();    //Очищаем экран
  tft.setTextSize(1);  //Меняем размер текста
  tft.setCursor(3, 1); //Меняем позицию курсора

  //Ждем BME280
  while(!bme.begin())
  {
    tft.print("BME280 not found!");
    delay(1000);
    tft.clearScreen();
  }
 
   
  tft.setTextSize(TextSize);
  
      //Рисуем макет
  //Время и неделя
  tft.setCursor(27, 61); 
  tft.print(":");

  //Дата
  tft.setCursor(27, 81); 
  tft.print(".");
  tft.setCursor(63, 81);
  tft.print(".");
      //Макет нарисован

}

void loop() {
  //Температура
  bme.read(pres, temp, hum, tempUnit, presUnit);

  if (T1 != int(temp)) {
    T1 = int(temp);
    tft.fillRect(3, 1, 48, 14, BLACK);
    tft.setCursor(3, 1);
    tft.print(T1);
    tft.print((char)247);
    tft.print("C");
  }

  //Влажность
  if (H != int(hum)) {
    H = int(hum);
    tft.fillRect(72, 1, 48, 14, BLACK);
    tft.setCursor(72, 1);
    tft.print(H);
    tft.print("%");
  }

  //Давление
  if (int(P) != int(pres)) {
    P = pres;
    tft.fillRect(3, 21, 84, 14, BLACK);
    tft.setCursor(3, 21);
    tft.print(int(P * 0.75));
    tft.print("mmHg");
  }

  if (T2 != int(RTC.getTemp())) {
    T2 = int(RTC.getTemp());
    tft.fillRect(3, 41, 60, 14, BLACK);
    tft.setCursor(3, 41);
    tft.print(T2);
    tft.print((char)247);
    tft.print("C");
  }
  
  //Время
  if (hours != RTC.getHours()) {
    hours = RTC.getHours();
    tft.fillRect(3, 61, 24, 14, BLACK);
    tft.setCursor(3, 61);
    tft.print(hours);
  }
  
  if (minutes != RTC.getMinutes()) {
    minutes = RTC.getMinutes();
    tft.fillRect(39, 61, 24, 14, BLACK);
    tft.setCursor(39, 61);
    tft.print(minutes);
  }

  //Неделя
  if (week != RTC.getWeek()) {
    week = RTC.getWeek();
    tft.fillRect(81, 61, 36, 14, BLACK);
    tft.setCursor(81, 61);
    switch (week)
    {
      case 1:
        tft.print("TUE");
        break;
      case 2:
        tft.print("WED");
        break;
      case 3:
        tft.print("THU");
        break;
     case 4:
        tft.print("WED");
        break;
      case 5:
        tft.print("SAT");
       break;
      case 6:
        tft.print("SUN");
        break;
     case 7:
        tft.print("MON");
        break;
    }
  }
  
  //День Месяц Год
  if (day != RTC.getDay()) {
    day = RTC.getDay();
    tft.fillRect(3, 81, 24, 14, BLACK);
    tft.setCursor(3, 81);
    tft.print(day);
  }

  if (month != RTC.getMonth()) {
    month = RTC.getMonth();
    tft.fillRect(39, 81, 24, 14, BLACK);
    tft.setCursor(39, 81);
    tft.print(month);
  }

  if (year != RTC.getYear()) {
    year = RTC.getYear();
    tft.fillRect(75, 81, 48, 14, BLACK);
    tft.setCursor(75, 81);
    tft.print(year);
  }
  
  //Ждем смены времеми
  power.sleepDelay(60000 - RTC.getSeconds()*1000);
}
