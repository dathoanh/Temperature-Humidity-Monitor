/*************************************************************************/
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

/*************************************************************************/ 
#define SS_PIN D0 //D0
#define RST_PIN D3 //D3

#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

/************************************************************************/
#include "DHT.h"

#define DHTPIN 10    // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

/*************************************************************************/
String welcomeMessage = "!!!Welcome!!!";
String projectDescription = "Hardware Project";
String nameMember1 = " DaoHoanhDat ";
String studentID1  = "   1752157 ";
String nameMember2 = " LeNguyenAnhTu ";
String studentID2  = "    1751110    ";

void showTempHumid (float h, float t, float f) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Humidity,:"));
  lcd.print(h);
  lcd.setCursor(0, 1);
  lcd.print(F("Temperature:"));
  lcd.print(t);
  lcd.print(F("°C "));

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
//  Serial.print(F("°F  Heat index: "));
//  Serial.print(hic);
//  Serial.print(F("°C "));
//  Serial.print(hif);
//  Serial.println(F("°F"));
}

void setup() {
  // LCD intitialize
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.setCursor(0,0);
  lcd.print(welcomeMessage);
  lcd.setCursor(0, 1);
  lcd.print(projectDescription);
  lcd.backlight();

  // RFID initialize
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
}

void loop() {
//  delay(2000);
  // put your main code here, to run repeatedly:
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

//  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.println();
  Serial.print(" UID tag: ");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
//     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println(content);

  if (content.substring(1) == "0C 83 D3 A0") //change UID of the card that you want to give access
  {
//    Serial.println(" Access Granted ");
//    Serial.print(" Welcome ");
//    Serial.print(nameMember1);
//    Serial.println();
//    delay(1000);
//    Serial.println(" Have FUN ");
//    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Access Granted ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(nameMember1);
    lcd.setCursor(0, 1);
    lcd.print(studentID1);
    delay(2000);
    showTempHumid(h, t, f);
  }

  else if (content.substring(1) == "90 DA 93 13") //change UID of the card that you want to give access
  {
//    Serial.println(" Access Granted ");
//    Serial.print(" Welcome ");
//    Serial.print(nameMember2);
//    Serial.println();
//    delay(1000);
//    Serial.println(" Have FUN ");
//    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Access Granted ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(nameMember2);
    lcd.setCursor(0, 1);
    lcd.print(studentID2);
    delay(2000);
    showTempHumid(h, t, f);
  }
  
  else   {
    Serial.println(" Access Denied ");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Access Denied ");
    delay(2000); 
  }


  
}
