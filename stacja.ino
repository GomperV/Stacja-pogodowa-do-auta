#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Wire.h> 
#include <RTClib.h>


#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

#define DHTPIN 2
#define DHTTYPE DHT22

//data
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

char daysOfTheWeek[7][12] = {"Nd.", "Pon", "Wt.", "Sr.", "Czw", "Pt.", "Sb."};


#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

LiquidCrystal_I2C lcd(0x27, 16, 2); //adres LCD
RTC_DS1307 RTC;
DHT dht(DHTPIN, DHTTYPE); //typ wyswietlacza i pin do ktorego jest podpiety
Adafruit_BMP280 bmp; // I2C



void setup()
{

  dht.begin();     // start termometru
  lcd.init();      // start LCD
  lcd.backlight(); // wlaczenie podswietlacza LCD
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

  int czasPomiaru = 1800; //co ile pomiar sredniej
  int a = 0; //czas od ostatniego pomiaru
  //float lastTemp;
  float deltaTemp;
  float first15 = 0; //srednia z pierwszych 15 min
  float last15 = 0; //srednia z kolejnych 15 min
  int czasPetli = 2;
  int ilePomiarow = czasPomiaru/czasPetli;
  String zmianaTemp;
  int humi;
  float tempC;
  bool czyFirst15 = false; //czy minelo pierwsze 15 min
  int nr = 0;
  int cisnienie;
  int data;
  
void loop()
{

  nr = nr + 1;
  a = a + czasPetli;
  delay(czasPetli*1000); // interwal miedzy pomiarami
  humi  = dht.readHumidity();    // odczyt wilgotnosci w %
  tempC = dht.readTemperature(); // odczyt temperatury w C
  if (!czyFirst15) 
  {
    first15 += tempC/ilePomiarow;
      if (a >= czasPomiaru) {
        czyFirst15 = true;
        a = 0;
        //Serial.println("Pobieranie sredniej:");
        //Serial.println(first15);
    }
  }
  else {
    last15 += tempC/ilePomiarow;
       if (a >= czasPomiaru) {
        a = 0;
        //Serial.println("Pobieranie sredniej:");
        //Serial.println(last15);
        if (last15 - first15 > 0.2) {
          zmianaTemp = "->";
          //digitalWrite(yellow, HIGH);
        }
        else if (last15 - first15 < -0.2) {
          zmianaTemp = "<-";
          //digitalWrite(yellow, LOW);
        }
        else {
          
          zmianaTemp = "<>";
          //digitalWrite(yellow, LOW);
       }
       first15 = last15;
       last15 = 0;
    }
  }
  //if (tempC >= 23.00 || tempC <= 20.00)
  //{
  //  digitalWrite(red, HIGH); 
  //  digitalWrite(green, LOW);
  //}
  //else
  //{
  //  digitalWrite(red, LOW);
  //  digitalWrite(green, HIGH); 
  //}
  lcd.clear();
  //lcd.setCursor(14, 0);
  //lcd.print("-");
/////////////////////////////////
  //DATA I CZAS//
/////////////////////////////////
   DateTime now = RTC.now();
   lcd.setCursor(10, 0);
   lcd.print(" ");
  lcd.print(now.hour(), DEC);
    lcd.print(':');
    if (now.minute() < 10) //żeby się wyświetlało 0 przed cyfrą minuty (bardzo nieprofesjonalny kod ale dziala)
    {
    lcd.setCursor(14, 0);
    lcd.print('0');
    lcd.setCursor(15, 0);
    lcd.print(now.minute(), DEC);
    }
    else {
    lcd.print(now.minute(), DEC);
    }
    lcd.print(':');
    lcd.print(now.second(), DEC);
  // lcd.setCursor(0, 1);
  // lcd.print(now.day(), DEC);
  //  lcd.print('/');
  //  lcd.print(now.month(), DEC);
  //  lcd.print('/');
  //  lcd.print(now.year(), DEC);
  //  lcd.print(' ');
//////////////////////////////////
  lcd.setCursor(8, 0);
  lcd.print(zmianaTemp);

  
  //Serial.print(lastTemp);
  ///Serial.println(zmianaTemp);
  //Serial.print("n\");
  //Serial.print(a);
  // sprawdzenie czy odczyty dzialaja
  if (isnan(humi) || isnan(tempC)) {
    lcd.setCursor(0, 0);
    lcd.print("Error odczytow");
  } 
    else 
  {
    Serial.println(" ");
    //Serial.println("Numer pomiaru:");
    Serial.print(nr);
    Serial.print(" ");
    //Temperatura w C
    lcd.setCursor(0, 0);  // wyswietlanie od 1 kolumny i 1 wiersza
    lcd.print("T:");
    lcd.print(tempC);     // wyswietl zmienna temperatury
    //Serial.println("tempC:");
    Serial.print(tempC);
    //Serial.print("n\ ");
    lcd.setCursor(6, 0);
    lcd.print((char)223); // znak °
    lcd.print("C");
    //lcd.setCursor(11, 0);
    //lcd.print("19:37");
    //Wilgotnosc w %
    lcd.setCursor(0, 1);  // wyswietlanie od 1 kolumny ale 2 wiersza
    lcd.print("W:");
    lcd.print(humi);      // wyswietl zmienna wilgotnosci
    lcd.print("%"); 
    //int cisnienie = bmp.readPressure();
    lcd.setCursor(8, 1);
    lcd.print("C:");
    lcd.print((bmp.readPressure()/100),0);      // wyswietl zmienna cisnienia w hPa
    lcd.print("hPa"); 
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure()/100); //hPa
    Serial.println(" hPa");

    if(now.minute() % 2 == 0)
    {
      lcd.setCursor(0, 1);
      lcd.print(now.day(), DEC);
      lcd.print('.');
      lcd.print(now.month(), DEC);
      lcd.print('.');
      lcd.print(now.year(), DEC);
      lcd.print(" (");
      lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
      lcd.print(") ");
    }
    
  
  }

}
