#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#define SS_PIN 10
#define RST_PIN 9
#define TRIG_PIN 4
#define ECHO_PIN 3
#define LED_V 6
#define LED_R 5
#define BUZZER 7
#define RESET_BTN 8
MFRC522 rfc(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); 
int passagers = 0;
const int SEUIL_MAX = 30;
void setup() {
  pinMode(LED_V, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RESET_BTN, INPUT_PULLUP);
  Serial.begin(9600);
  SPI.begin();
  rfc.PCD_Init();
  lcd.init();
  lcd.backlight();
  afficherStatut();
}
void loop() {
  if (digitalRead(RESET_BTN) == LOW) {
    passagers = 0;
    lcd.clear();
    lcd.print("REINITIALISATION");
    lcd.setCursor(0, 1);
    lcd.print("Bus vide...");
    delay(2000);
    afficherStatut();
  }
  long distance = lireDistance();
  if (passagers >= SEUIL_MAX && distance < 50) {
    lcd.setCursor(0, 0);
    lcd.print("ATTENTE PORTE ! ");
    lcd.setCursor(0, 1);
    lcd.print("Degagez l'entree");
    delay(500);
    return; 
  }
  if (!rfc.PICC_IsNewCardPresent() || !rfc.PICC_ReadCardSerial()) {
    return;
  }
  if (passagers < SEUIL_MAX) {
    passagers++;
    digitalWrite(LED_V, HIGH);
    tone(BUZZER, 1500, 200);
    lcd.clear();
    lcd.print("ACCES VALIDE");
    lcd.setCursor(0, 1);
    lcd.print("SIEGE No: "); lcd.print(passagers);
    delay(1500);
    digitalWrite(LED_V, LOW);
  } else {
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 500, 600); 
    lcd.clear();
    lcd.print("BUS COMPLET !");
    lcd.setCursor(0, 1);
    lcd.print("STOP EMBARQUER");
    delay(2000);
    digitalWrite(LED_R, LOW);
  }
  afficherStatut();
  rfc.PICC_HaltA(); 
}
long lireDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duree = pulseIn(ECHO_PIN, HIGH);
  return duree * 0.034 / 2;
}
void afficherStatut() {
  lcd.clear();
  if (passagers >= SEUIL_MAX) {
    digitalWrite(LED_R, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("BUS EST PLEIN !");
    lcd.setCursor(0, 1);
    lcd.print("Depart ds 10s ");
  } else {
    digitalWrite(LED_R, LOW);
    int placesManquantes = SEUIL_MAX - passagers;
    lcd.setCursor(0, 0);
    lcd.print("Passagers: "); lcd.print(passagers);
    lcd.setCursor(0, 1);
    lcd.print("Places res: "); lcd.print(placesManquantes);
  }
}
