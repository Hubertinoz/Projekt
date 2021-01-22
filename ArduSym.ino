#include <LiquidCrystal.h>

//Stałe
const int pResistor = A0;	// Fotorezytor pin A0
const int ledGreen = 9;	// Led pin 9
const int ledBlue = 8;// Led pin 8
const int ledRed = 6;// Led pin 6
const int PIR = 7;			// Czujnik ruchu pin 4
const int buttonPin = 13;  
int buttonState = 0;   

//Zmienne
int value;					// Wartość fotorezystora (0-1023)
long unsigned int lowIn;
long unsigned int pause = 2 * 1000;	//Czas świecenia od czasu kiedy czujnik przeszedł w stan niski
bool pirstatus = true;	//Zmienna dla czujnika
bool lowTime;

String text = "Zielony";
int color = 1;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(ledGreen, OUTPUT);	// Pin 9  jako wyjście led
   pinMode(ledBlue, OUTPUT);	// Pin 9  jako wyjście led
   pinMode(ledRed, OUTPUT);	// Pin 9  jako wyjście led
  pinMode(buttonPin, INPUT);
  
  pinMode(pResistor, INPUT);// Pin A0  jako wejście fotorezystora
  pinMode(PIR, INPUT);		// Pin 4 jako wejście czujnika ruchu
  turnOff();
  Serial.begin(9600);		// Logi do konsoli
  
  lcd.begin(16, 2);
  lcd.print("Witaj Eweryst");
  delay(2000);
  lcd.clear();
}

void turnOff()
{
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledRed, LOW);
}

void turnOn()
{
  switch (color) {
  case 1:
    turnOff();
    digitalWrite(ledGreen, HIGH);
    break;
  case 2:
    turnOff();
     digitalWrite(ledBlue, HIGH);
  	break;
  case 3:
    turnOff();
     digitalWrite(ledRed, HIGH); 
  break;
    
  default:
    break;
}
  
 
  
}

void loop() {
   buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
   color = color + 1;
   if(color==1)
     text = "Zielony";
   else if(color==2)
     text = "Niebieski";
   else if(color==3)
	 text = "Czerwony";
   else if(color==4)
		color=0;
  } 
  
  value = analogRead(pResistor);//Odczyt danych
  Serial.println(value);	//Wypisywanie wartości do konsoli

  lcd.setCursor(0, 0);
  lcd.print("Wartosc: ");
  lcd.setCursor(9, 0);
  lcd.print(value);
  lcd.setCursor(0, 1);
  lcd.print("Kolor: ");
  lcd.setCursor(7, 1);
  lcd.print(text + " ");
            
  if (digitalRead(PIR) && value < 900) { //Sprawdzanie czy wykonano ruch oraz czy jest wystarczająco ciemno
    turnOn();
    if (pirstatus) {
      //upewnienie sie czy czujnik przeszedl w stan niski aby kontynuowac
      pirstatus = false;
      delay(20);
    }
    lowTime = true;
  }

  if (digitalRead(PIR)) {
    if (lowTime) {
      lowIn = millis(); //zapisanie czasu po przejsciu ze stanu wysokiego na niski
      lowTime = false;
    }
  }
  //jesli czujnik jest w stanie niskim przez wiecej czasu niz podana pauza
  //mozna zalozyc ze nie bedzie juz wiecej ruchu
  if (!pirstatus && millis() - lowIn > pause) {
    pirstatus = true;
    turnOff();
    delay(10);
  }

}