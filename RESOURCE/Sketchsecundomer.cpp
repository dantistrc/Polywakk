// Добавляем нужные библиотеки
#include <SPI.h>
#include <LiquidCrystal.h>
// это пины, используемые на модуле
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);
// переменные, используемые для более чем 1 функции, должны быть объявлены здесь
unsigned long start, finished, elapsed;
boolean r = false;
// Переменные для времени отладки кнопки
long lastButtonPressTime = 0; // последний раз, когда была нажата кнопка
long debounceDelay = 50;      // время отладки; установите как можно меньше

void setup()
{
    lcd.begin(16, 2); // инициализировать ЖК (16 символов, 2 строки)

    lcd.setCursor(4, 0); // установите курсор на первый символ в строке 1 - НЕ нужен (он автоматически устанавливается на lcd.begin ()
    lcd.print("Arduino");
    lcd.setCursor(3, 1); // установите курсор на 4-й символ в строке 2
    lcd.print("StopWatch");
    delay(2000); // ждать 2 секунды
    lcd.clear(); // очистить дисплей
    lcd.setCursor(3, 0);
    lcd.print("By Conor M");
    lcd.setCursor(1, 1);
    lcd.print("Edited By Elac");
    delay(2000);
    lcd.clear();
    lcd.print("Press select for");
    lcd.setCursor(2, 1); // установите курсор на 3-й символ в строке 2
    lcd.print("Start & Stop");
}

void loop()
{
    CheckStartStop();
    DisplayResult();
}

void CheckStartStop()
{
    int x = analogRead(0);  // назначьте «x» аналоговым входам Arduino (кнопки модуля)
    if (x < 800 && x > 600) // если кнопка SELECT
    {
        if ((millis() - lastButtonPressTime) > debounceDelay)
        {

            if (r == false)
            {
                lcd.clear();
                lcd.setCursor(3, 0); // нужно
                lcd.print("Time Past");
                start = millis(); // сохраняет время начала, чтобы вычислить прошедшее время
            }
            else if (r == true)
            {
                lcd.setCursor(2, 0); // нужно
                lcd.print(" Final Time ");
            }
            r = !r;
        }
        lastButtonPressTime = millis();
    }
}

void DisplayResult()
{
    if (r == true)
    {
        finished = millis(); // сохраняет время остановки, чтобы вычислить прошедшее время.
        // объявляем переменные
        float h, m, s, ms;
        unsigned long over;

        // время
        elapsed = finished - start;

        h = int(elapsed / 3600000);
        over = elapsed % 3600000;
        m = int(over / 60000);
        over = over % 60000;
        s = int(over / 1000);
        ms = over % 1000;
        // показать результаты
        lcd.setCursor(0, 1);
        lcd.print(h, 0);
        lcd.print("h ");
        lcd.print(m, 0);
        lcd.print("m ");
        lcd.print(s, 0);
        lcd.print("s ");
        if (h < 10)
        {
            lcd.print(ms, 0);
            lcd.print("ms ");
        }
    }
}
