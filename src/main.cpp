#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Instanziierung

const int analogPin = 34; //Pin for the anaolog Photoelectric Sensor
const double threshold = 1000 // Threshold for the Photoelectric Sensor

int currentAnalogValue = 0; //current value of the analog Photoelectric Sensor

double runtime;           // Zeit seit Boot
double drinkTime = 0.;      //gesamtzeit des Trinkens
double drinkStart = 0.;    //Zeitpunkt bei dem Angefange wurde zu trinken
double currentTime = 0.;  //verändernde Variable die hochgeht


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

double getRuntime()
{
  return millis() / 1000.0;
}

int getanalogPin()
{
  return analogRead(analogPin);
}

void stage1()
{
  stage = 1;
  currentTime = 0;
  drinkTime = 0;
  Serial.print(">Stage:");
  Serial.println(stage);

  Serial.print(">Time:");
  Serial.println(currentTime);

  currentAnalogValue = getanalogPin();
  Serial.print(">AnalogValue:");
  Serial.println(currentAnalogValue);
  Serial.print(">DrinkTime:");
  Serial.println(drinkTime);

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.print("Trinken");
  display.display();
  delay(2000);
} 

void stage2()
{

  while(getanalogPin() > 100)
  {
    stage = 2;
    Serial.print(">Stage:");
    Serial.println(stage);
    Serial.print(">Time:");
    Serial.println(currentTime);

    currentAnalogValue = getanalogPin();
    Serial.print(">AnalogValue:");
    Serial.println(currentAnalogValue);
    Serial.print(">DrinkTime:");
    Serial.println(drinkTime);

    //double startTime = millis();
    //double endTime = millis();
    //Serial.print(">Time:");
    //Serial.println(endTime - startTime);
  }

  drinkStart = millis();
}

void stage3()
{
  currentAnalogValue = getanalogPin();
  while(currentAnalogValue < threshold || currentTime < 1)
  {
    currentAnalogValue = getanalogPin();
    stage = 3;
    Serial.print(">Stage:");
    Serial.println(stage);

    currentTime = (millis() - drinkStart)/1000.0;
    currentTime = (double)(int)(currentTime * 100) / 100;

    Serial.print(">Time:");
    Serial.println(currentTime);
    Serial.print(">AnalogValue:");
    Serial.println(currentAnalogValue);

    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.print(currentTime);
    display.display();

    //double startTime = millis();
    //double endTime = millis();
    //Serial.print(">Time:");
    //Serial.println(endTime - startTime);
  }
  
  drinkTime = (millis() - drinkStart) / 1000.0;
  drinkTime = (double)(int)(drinkTime * 100) / 100;
  Serial.print(">DrinkTime:");
  Serial.println(drinkTime);
}

void stage4()
{
  stage = 4;
  Serial.print(">Stage:");
  Serial.println(stage);

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.print(drinkTime);
  display.display();
  delay(5000);
}

void loop() {

  //runtime = millis() / 1000.0;
  //Serial.print(">Analog value:");
  //Serial.println(currentMedian);

  stage1();
  stage2();
  stage3();
  stage4();

}

