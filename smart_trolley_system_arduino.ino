
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
const int remove_button = 2;    
const int add_button = 3; 
const int reset_button = 4;
const int buzzer_Pin = 5; 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

struct item
{
  String item_name;
  String item_number;
  int item_price;
};

const int number_of_item = 4;
const item item_list[number_of_item]= 
  {
  //Item Name       Item RFID Number   Item Price
  {"Tata salt",     "83 1C 09 DA",        100},
  {"Milk Powder",   "25 7A 24 4F",        50},
  {"Bournvita",     "13 1F A3 0D",        20},
  {"Ball_Pen",      "E3 68 DC 12",        10},
  };

int bill_amount = 0;
int remove_buttonState = 0;  
int add_buttonState = 0;  
int reset_buttonState = 0;
int add_item_flag = 1;
int remove_item_flag = 0;
void setup() 
{
  pinMode(remove_button, INPUT);
  pinMode(reset_button, INPUT);
  pinMode(add_button, INPUT);
  pinMode(buzzer_Pin, OUTPUT);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  digitalWrite(buzzer_Pin, LOW); 
    lcd.init();
  // Turn on the Backlight
  lcd.backlight();
  lcd.clear(); 
  // Set cursor (Column, Row)
  lcd.setCursor(0, 0);  
  lcd.print("Smart Trolley"); 
  lcd.setCursor(0,1);
  lcd.print("Billing System");
  delay(2000);
  lcd.clear(); 
  // Set cursor (Column, Row)
  lcd.setCursor(0, 0);  
  lcd.print("Start purchasing"); 
  lcd.setCursor(0,1);
  lcd.print("your item");
  delay(100);
}
void loop() 
{
  remove_buttonState = digitalRead(remove_button);
  add_buttonState = digitalRead(add_button);
  reset_buttonState = digitalRead(reset_button);
  if(remove_buttonState)
  {
    add_item_flag = 0;
    remove_item_flag = 1;
    lcd.clear(); 
    // Set cursor (Column, Row)
    lcd.setCursor(0, 0);  
    lcd.print("You Can now"); 
    lcd.setCursor(0,1);
    lcd.print("Remove your item");
    delay(2000);
  }
  else if(add_buttonState)
  {
    add_item_flag = 1;
    remove_item_flag = 0;
    lcd.clear(); 
    // Set cursor (Column, Row)
    lcd.setCursor(0, 0);  
    lcd.print("You Can now"); 
    lcd.setCursor(0,1);
    lcd.print("add your item");
    delay(2000);
  }
  else if(reset_buttonState)
  {
    lcd.clear(); 
    // Set cursor (Column, Row)
    lcd.setCursor(0, 0);  
    lcd.print("Resetting"); 
    lcd.setCursor(0,1);
    lcd.print("Trolley data");
    delay(2000);
    lcd.clear(); 
    // Set cursor (Column, Row)
    lcd.setCursor(0, 0);  
    lcd.print("Start Purchasing"); 
    lcd.setCursor(0,1);
    lcd.print("Your item");
    delay(2000);
    bill_amount = 0;
  }
  
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
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  for(int i = 0 ;i < sizeof(item_list) ; i++)
  {
    if (content.substring(1) == item_list[i].item_number) //change here the UID of the card/cards that you want to give access
    {
      if(add_item_flag == 1)
      {
          bill_amount = bill_amount + item_list[i].item_price;
  //      Serial.println("you have purchased = " + item_list[i].item_name);
  //      Serial.print("Purchased item price");
  //      Serial.println(item_list[i].item_price);
  //      Serial.print("Total billing amount");
  //      Serial.println(bill_amount);
  //      delay(1000);
          digitalWrite(buzzer_Pin, HIGH);
          delay(500);
          digitalWrite(buzzer_Pin, LOW); 
          lcd.clear(); 
          // Set cursor (Column, Row)
          lcd.setCursor(0, 0);  
          lcd.print(item_list[i].item_name ); 
          lcd.setCursor(0,1);
          lcd.print(String(item_list[i].item_price)+" Rs");
          delay(2000);
      }
      else if(remove_item_flag == 1)
      {
        if(bill_amount > 0)
        {
          bill_amount = bill_amount - item_list[i].item_price;
  //      Serial.println("you have purchased = " + item_list[i].item_name);
  //      Serial.print("Purchased item price");
  //      Serial.println(item_list[i].item_price);
  //      Serial.print("Total billing amount");
  //      Serial.println(bill_amount);
  //      delay(1000);
          digitalWrite(buzzer_Pin, HIGH);
          delay(500);
          digitalWrite(buzzer_Pin, LOW);  
          lcd.clear(); 
          // Set cursor (Column, Row)
          lcd.setCursor(0, 0);  
          lcd.print(item_list[i].item_name ); 
          lcd.setCursor(0,1);
          lcd.print("removed");
          delay(2000); 
          
        }
        else
        {
          lcd.clear(); 
          // Set cursor (Column, Row)
          lcd.setCursor(0, 0);  
          lcd.print("Your trolley is" ); 
          lcd.setCursor(0,1);
          lcd.print("empty now");
          delay(2000);
        }
      }
    }
  }
  lcd.clear(); 
  // Set cursor (Column, Row)
  lcd.setCursor(0, 0);  
  lcd.print("Total Billing"); 
  lcd.setCursor(0,1);
  lcd.print(String(bill_amount)+" Rs");
  delay(2000);


} 
