/*************************************************************************/
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
int a = 0;

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
#include <SoftwareSerial.h>

#define RX 15
#define TX 2

SoftwareSerial sim(TX, RX);

/*************************************************************************/
String welcomeMessage = "!!!Welcome!!!";
String projectDescription = "Hardware Project";
String nameMember1 = " DaoHoanhDat ";
String studentID1  = "   1752157 ";
String member1Phone = "+84708367531"; // +880 is the country code
String nameMember2 = " LeNguyenAnhTu ";
String studentID2  = "    1751110    ";
String member2Phone = "+84946688199"; // +880 is the country code

void showTempHumid (float h, float t, float f) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F(" Humidity:"));
  lcd.print(h);
  lcd.setCursor(0, 1);
  lcd.print(F(" Tempt:"));
  lcd.print(t);
  lcd.print(F(" C "));

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C | "));
  Serial.print(f);
  Serial.println(F("°F "));
//  Serial.print(F("°F  Heat index: "));
//  Serial.print(hic);
//  Serial.print(F("°C "));
//  Serial.print(hif);
//  Serial.println(F("°F"));
}

void checkCard(){
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
}

void checkSensor(float h, float t, float f){
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}

void showPrivateInfo(String memberName, String studentID){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Access Granted ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(memberName);
    lcd.setCursor(0, 1);
    lcd.print(studentID);
    delay(2000);
}

void sendMessage(String number, float h, float t, float f){
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
//  String humidSMS = "Humidity: " + h;
//  String temptSMS = "Temperature: " + t + "°C | " + f + "°F";
  sim.print(F("Humidity: "));
  sim.println(h);
  delay(100);
  sim.print(F("Temperature: "));
  sim.print(t);
  sim.print(F("(C) | "));
  sim.print(f);
  sim.print(F("(F)"));
  delay(100);
  sim.println((char)26);
  delay(1000);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
int readButtons(int c)
// returns the button number pressed, or zero for none pressed 
// int pin is the analog pin number to read 
{
  int b;  
  if (c>540 && c<560)
  {
    b=1; // button 1 pressed
  }     
  else
    if (c>335 && c<355)
    {
      b=2; // button 2 pressed
    } 
  else b=0;      
return b;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // LCD intitialize
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  pinMode(A0, INPUT_PULLUP);
  lcd.setCursor(0,0);
  lcd.print(welcomeMessage);
  lcd.setCursor(0, 1);
  lcd.print(projectDescription);
  lcd.backlight();

  // SIM800L initialize
  Serial.println("System Started...");
  sim.begin(9600);
  delay(1000);  

  // RFID initialize
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
}

void loop() {
//  delay(2000);
// put your main code here, to run repeatedly:
//  checkCard();
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
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
    showPrivateInfo(nameMember1, studentID1);

        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" Press a button");

    while(1){
        int c = analogRead(A0); // get the analog value 
        a =  readButtons(c);
        Serial.println(c);
        delay(100);
          // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  checkSensor(h, t, f);
    if (a==1)
         {
          showTempHumid(h, t, f);
         }
    else if (a==2)
         {
          sendMessage(member1Phone, h, t, f);
         }
    delay(1000);
    if (  mfrc522.PICC_IsNewCardPresent()) 
  {
    break;
  }
    }               
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
    showPrivateInfo(nameMember2, studentID2);

        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" Press a button");

    while(1){
        int c = analogRead(A0); // get the analog value 
        a =  readButtons(c);
        Serial.println(c);
        delay(100);
          // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  checkSensor(h, t, f);     
    if (a==1)
         {
          showTempHumid(h, t, f);
         }
    else if (a==2)
         {
          sendMessage(member2Phone, h, t, f);
         }
    delay(1000);
    if (  mfrc522.PICC_IsNewCardPresent()) 
  {
    break;
  }
    }

  }
  
  else   {
    Serial.println(" Access Denied ");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Access Denied ");
    delay(2000); 
  }

//  if (sim.available() > 0)
//    Serial.write(sim.read());
 
}
