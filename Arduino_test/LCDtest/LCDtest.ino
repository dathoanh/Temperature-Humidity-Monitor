//#include <LiquidCrystal_I2C.h>
//
//// set the LCD number of columns and rows
//int lcdColumns = 16;
//int lcdRows = 2;
//
//// set LCD address, number of columns and rows
//// if you don't know your display address, run an I2C scanner sketch
//LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
//
//void setup() {
//  // initialize LCD
//  lcd.begin();
//  // turn on LCD backlight
//  lcd.backlight();
//}
//
//void loop() {
//  // set cursor to first column, first row
//  lcd.setCursor(0, 0);
//  // print message
//  lcd.print(" Duc Le");
//  delay(1000);
//  // clears the display to print new message
//  //  lcd.clear();
//  // set cursor to first column, second row
//  lcd.setCursor(0, 1);
//  lcd.print(" AKA An May 4.0!");
//  //  delay(1000);
//  //  lcd.clear();
//}
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <LiquidCrystal_I2C.h>
//
//// set the LCD number of columns and rows
//int lcdColumns = 16;
//int lcdRows = 2;
//
//// set LCD address, number of columns and rows
//// if you don't know your display address, run an I2C scanner sketch
//LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
//
//void setup(){
//  // initialize LCD
//  lcd.begin();
//  // turn on LCD backlight                      
//  lcd.backlight();
//}
//
//void loop(){
//  // set cursor to first column, first row
//  lcd.setCursor(0, 0);
//  // print message
//  lcd.print("Hello, World!");
//  delay(1000);
//  // clears the display to print new message
//  lcd.clear();
//  // set cursor to first column, second row
//  lcd.setCursor(0,1);
//  lcd.print("Hello, World!");
//  delay(1000);
//  lcd.clear(); 
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////  
#include <LiquidCrystal_I2C.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  Serial.begin(9600);
  lcd.init();
  Serial.println("Enter your name.");
  lcd.setCursor(0,0);
  lcd.print("Enter Your Name");
  lcd.backlight();
  }
  

char rx_byte = 0;
String rx_str = "";

void loop() {
  if (Serial.available() > 0) {    
    rx_byte = Serial.read();       
    
    if (rx_byte != '\n') {
      rx_str += rx_byte;
    }
    else {
      // end of string
      Serial.println(rx_str);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter Your Name");
      lcd.setCursor(0,1);
      lcd.print(rx_str);
      rx_str = "";                
      Serial.println("");
      Serial.println("Enter your name.");
    }
  }  
}
