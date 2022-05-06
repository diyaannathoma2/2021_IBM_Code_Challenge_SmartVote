

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <SPI.h>
#include <RFID.h>
#define SS_PIN 53
#define RST_PIN 49
RFID rfid(SS_PIN, RST_PIN);
String rfidCard;

#define S1 7
#define S2 6
#define S3 5
#define S4 4
#define S5 3
int vote1 = 0;
int vote2 = 0;
int vote3 = 0;
int vote4 = 0;
String regVoters[2] = {"243 209 125 25"};
int c[2] = {0, 0};
int reg=0;
void setup()
{
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  lcd.begin();
  lcd.print("Electronic ");
  lcd.setCursor(0, 1);
  lcd.print(" Voting Machine ");
  delay(4000);
  digitalWrite(S1, HIGH);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  digitalWrite(S4, HIGH);
  digitalWrite(S5, HIGH);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("A");
  lcd.setCursor(5, 0);
  lcd.print("B");
  lcd.setCursor(9, 0);
  lcd.print("C");
  lcd.setCursor(13, 0);
  lcd.print("D");
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
}

void loop()
{
  lcd.setCursor(1, 0);
  lcd.print("A");
  lcd.setCursor(1, 1);
  lcd.print(vote1);
  lcd.setCursor(5, 0);
  lcd.print("B");
  lcd.setCursor(5, 1);
  lcd.print(vote2);
  lcd.setCursor(9, 0);
  lcd.print("C");
  lcd.setCursor(9, 1);
  lcd.print(vote3);
  lcd.setCursor(13, 0);
  lcd.print("D");
  lcd.setCursor(13, 1);
  lcd.print(vote4);
  if (rfid.isCard())
  {
    if (rfid.readCardSerial())
    {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      Serial.println(rfidCard);
      lcd.clear();
      lcd.print(rfidCard);
      delay(3000);
      lcd.clear();
      for (int i = 0; i < 1; i++)
      {
        if (rfidCard == regVoters[i])
        {
          reg=1;
          lcd.clear();
          lcd.print("Registered");
          delay(3000);
          lcd.clear();
          while (c[i] == 0)
          {
            if (digitalRead(S1) == 0)
            {
              vote1++;
              c[i] = 1;
            }
            else if (digitalRead(S2) == 0)
            {
              vote2++;
              c[i] = 1;
            }
            else if (digitalRead(S3) == 0)
            {
              vote3++;
              c[i] = 1;
            }
            else if (digitalRead(S4) == 0)
            {
              vote4++;
              c[i] = 1;
            }
          }
        }
      }
      if(reg==0)
      {
          lcd.clear();
          lcd.print("Not a Registered");
          lcd.setCursor(0, 1);
          lcd.print(" Voter ");
          delay(3000);
          lcd.clear();
      }
      if (digitalRead(S5) == 0)
      {
        int vote = vote1 + vote2 + vote3 + vote4;
        if (vote)
        {
          if ((vote1 > vote2 && vote1 > vote3 && vote1 > vote4))
          {
            lcd.clear();
            lcd.print("A is Winner");
            delay(3000);
            lcd.clear();
          }
          else if ((vote2 > vote1 && vote2 > vote3 && vote2 > vote4))
          {
            lcd.clear();
            lcd.print("B is Winner");
            delay(3000);
            lcd.clear();
          }
          else if ((vote3 > vote1 && vote3 > vote2 && vote3 > vote4))
          {
            lcd.clear();
            lcd.print("C is Winner");
            delay(3000);
            lcd.clear();
          }
          else if (vote4 > vote1 && vote4 > vote2 && vote4 > vote3)
          {
            lcd.setCursor(0, 0);
            lcd.clear();
            lcd.print("D is Winner");
            delay(3000);
            lcd.clear();
          }
          else if (vote4 > vote1 && vote4 > vote2 && vote4 > vote3)
          {
            lcd.setCursor(0, 0);
            lcd.clear();
            lcd.print("D is Winner");
            delay(3000);
            lcd.clear();
          }
          else
          {
            lcd.clear();
            lcd.print(" Tie Up Or ");
            lcd.setCursor(0, 1);
            lcd.print(" No Result ");
            delay(3000);
            lcd.clear();
          }
        }
        else
        {
          lcd.clear();
          lcd.print("No Voting....");
          delay(3000);
          lcd.clear();
        }
        vote1 = 0; vote2 = 0; vote3 = 0; vote4 = 0, vote = 0;
        lcd.clear();
      }
    }
    rfid.halt();
  }
  reg=0;
}
