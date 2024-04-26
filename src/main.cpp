#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <vector>
using namespace std;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Instanziierung

#define lowerThreshold 2500
#define upperThreshold 3000

const int analogPin = 34; //Pin für den Analogen Feuchtigkeitssensor
double runtime;           // Zeit seit Boot
double drinkTime = 0;      //gesamtzeit des Trinkens
double drinkStart = 0;    //Zeitpunkt bei dem Angefange wurde zu trinken
double drinkEnd = 0;      //Zeitpunkt bei dem Aufgehört wurde zu trinken
double currentTime = 0;  //verändernde Variable die hochgeht


int currentAnalogValue = 0;
int lastAnalogValue = 0;
//Messwertespeicher
vector <int> analogValues;

/*Stages:
  1 = Trinkbereit
  2 = TrinkStart
  3 = Messung
  4 = Auswertung
*/
int stage = 1;


void setup() {
  Serial.begin(115200);

  //analogen Pin als Eingang definieren
  pinMode(analogPin, INPUT);

  //DisplaySetup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.clearDisplay();
	display.display(); // zeigt den Grafikpuffer auf dem OLED-Display

	// Beispiel einer Textausgabe:
	display.setTextColor(WHITE); display.setTextSize(3);
	display.setCursor(0, 0); display.println("Bier   Baron");
  // Grafikpuffer auf dem OLED-Display ausgeben:
  display.display();

  delay(3000);
  display.clearDisplay();
}

void loop() {

  
  lastAnalogValue = currentAnalogValue;
  currentAnalogValue = ( currentAnalogValue + analogRead(analogPin) ) / 2 ;

  runtime = millis() / 1000.0;
  Serial.print(">Analog value:");
  Serial.println(currentAnalogValue);

  if(stage == 1)
  {
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.print("Trinken");
    display.display();
    stage = 2;
    delay(2000);
  }
  else  if(stage == 2 && currentAnalogValue < lowerThreshold)
  {
    drinkStart = runtime;
    stage = 3;
  } 
  else if(stage == 3)
  {
    if(currentAnalogValue > upperThreshold)
    {
      drinkEnd = runtime;
      drinkTime = drinkEnd - drinkStart;
      stage = 4;
    }
    currentTime = runtime - drinkStart;
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.print(currentTime);
    display.display();

  }
  else if(stage == 4)
  {
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.print(drinkTime);
    display.display();
    stage = 1;
    delay(5000);
  }
/*
  Serial.print(">Analog value:");
  Serial.println(analogRead(analogPin));

*/
}

