#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
// #include <GyverButton.h>
#include <ServoSmooth.h>
#include <EncButton.h>
#define CLK 8                                                                      // enc
#define DT 9                                                                       // enc
int_fast32_t pos = 0;                                                              // enc
byte lastState = 0;                                                                // enc
const int8_t increment[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0}; // enc
const char* Nstring[] = {"1. Mixer", "2. Hardener" ,"3. Mixing" ,"4. Molding" ,"5. Iposition"};
int sensStop = 8;                      // датчик кз
int sensorPin = A0; // пока не использую
int MixPin = 11;   // Два датчика Дом Стол
byte targ = 0; // flag target
int ztable = 0;     // flag z table
int cstart = 0; // flag start
int proc = 0; // rgim raboty
int posA = 1940; // Mixer
int posB = 2040; // Otverditel
unsigned long start, finished, elapsed;
float h, m, s, ms;
unsigned long over;
int32_t sensorValue = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Устанавливаем дисплей
EncButton<EB_TICK, 8, 9, 10> enc;   // энкодер с кнопкой <A, B, KEY>
#define ENC_TYPE 0                  // тип энкодера, 0 или 1
    

//------------SERVO----------------
#define AMOUNT 2 // кол-во серво
ServoSmooth servos[AMOUNT];
uint32_t servoTimer;
uint32_t turnTimer;
boolean flag;

void setup()
{
  //enc.setEncReverse(true); //Reverse enc
  pinMode(MixPin, OUTPUT);     // назначить выводу порт ввода
  digitalWrite(MixPin, LOW); // выключить миксер
  // Serial.begin(9600);
  lcd.init();
  lcd.backlight(); // Включаем подсветку дисплея
  // подключаем
  servos[0].setCurrent(1940);
  servos[1].setCurrent(2040);
  servos[0].attach(2);          //Mixer
  servos[1].attach(3);          //Otverditel
  // настраиваем макс. скорости и ускорения
  // скор. по умолч. 100 град/с
  // ускорение по умолч. 0.2
  servos[0].setSpeed(10);
  servos[0].setAccel(0.1);
  servos[1].setSpeed(10);
  servos[1].setAccel(0.1);
}
//-----------------------------------------------------------------
void loop()
{
  enc.tick();
  byte state = digitalRead(CLK) | (digitalRead(DT) << 1); // Encoder
  if (state != lastState)
  {
    pos -= increment[state | (lastState << 2)]; // Encoder
    lastState = state;
    if (pos < 0) pos = 0;
    if (pos > 8) pos = 8; // 8 / 2 = 4 pozicii
  }
  if (enc.click())
  {
    proc = pos / 2;
    switch (proc)
    {
    case 0:
      posA = 1940;
      posB = 2040;                /* code */
      digitalWrite(MixPin, HIGH); // включить миксер
      start = millis();           // сохраняет время начала, чтобы вычислить прошедшее время
      break;
    case 1:
      posA = 1940;
      posB = 1100; /* code */
      start = millis(); // сохраняет время начала, чтобы вычислить прошедшее время
      break;
    case 2:
      posA = 1940;
      posB = 2040; /* code */
      break;
    case 3:
      posA = 950;
      posB = 2040; /* code */
      break;
    case 4:
      posA = 1940;
      posB = 2040;               /* code */
      digitalWrite(MixPin, LOW); // выключить миксер
      break;
    default:
      posA = 1940;
      posB = 2040; /* code */
      break;
    }
    targ = 1;
  }
//-------------SERVO----------------------------------------------------------------------
  // каждые 20 мс
  if (millis() - servoTimer >= 20)
  { // взводим таймер на 20 мс (как в библиотеке)
    servoTimer += 20;
    for (byte i = 0; i < AMOUNT; i++)
    {
      servos[i].tickManual(); // двигаем все сервы. Такой вариант эффективнее отдельных тиков
    }
  }
if (targ == 1){
    servos[0].setTarget(posA);    //slov muve
    servos[1].setTarget(posB);
    targ = 0;
  }
if(targ == 0)
  {
    servos[0].setTarget(posA); // slov muve
    servos[1].setTarget(posB);
    servos[0].setAutoDetach(false); // retention current pozition
    servos[1].setAutoDetach(false);
  }
//----------------------------------------------------------------------------------------
  static uint32_t tmr;
  if (millis() - tmr >= 500)
  {
    tmr = millis();
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(3, 0);
    lcd.print(String(Nstring[pos / 2]));
    // _____________ТАЙМЕР_______________
    elapsed = millis() - start;
    h = int(elapsed / 3600000);
    over = elapsed % 3600000;
    m = int(over / 60000);
    over = over % 60000;
    s = int(over / 1000);
    ms = over % 1000;
    // показать таймер
    lcd.setCursor(5, 1);
    lcd.print(m, 0);
    lcd.print("m ");
    lcd.print(s, 0);
    lcd.print("s ");
    
  }
}
