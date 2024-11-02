/* 
#AUTHOR Дмитрий Витальевич Мельник
 #MAIL i.x.c.o.n@yandex.ru 
#LICENSE Использование  только  для не коммерческих проектов, без упоминания автора. При использовании автор не несет ни какой отвественности, и не дает ни какой гарантии работы. Не ведет поддержку или исправление ошибок.  
*/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PINONESTEPDIR 34
#define PINTWOSTEPDIR 26
#define STEPONE 35
#define STEPTWO 33

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int CountStepOne = 0;
int CountStepTwo = 0;

uint32_t timer = 0;
uint32_t timer1 = 0;
uint32_t timer2 = 0;

char command[64];

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 START OK!");
  pinMode(PINONESTEPDIR, OUTPUT);
  pinMode(PINTWOSTEPDIR, OUTPUT);
  pinMode(STEPONE, OUTPUT);
  pinMode(STEPTWO, OUTPUT);

  digitalWrite(PINONESTEPDIR, HIGH);
  digitalWrite(PINTWOSTEPDIR, HIGH);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.setCursor(36, 12);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println("ESP32");
  display.setCursor(36, 32);
  display.println("START");
  display.display();
}

void loop() {

  if(millis() - timer >= 100){
    timer = millis();
    SetDisplay();

  }

  if(millis() - timer1 >= 1000){
    timer1 = millis();

  }

  if(millis() - timer2 >= 100){
    timer2 = millis();
    if(Serial.available() > 0){
        Serial.readBytesUntil('\n', command, 32);
        delay(20);
        ReadCom();
    }

  }
  
}

void StepOneDrive(int step){
  for(int i = 0; i < step; i++)
  {
    digitalWrite(STEPONE, HIGH);
    delay(1);
    digitalWrite(STEPONE, LOW);
    delay(1);
  }
  bool TempStepOneDrive = digitalRead(PINONESTEPDIR);
  if(TempStepOneDrive == false)
  {
    step = step * -1;
  }
  CountStepOne = CountStepOne + step;
}

void StepTwoDrive(int step){
  for(int i = 0; i < step; i++)
  {
    digitalWrite(STEPTWO, HIGH);
    delay(1);
    digitalWrite(STEPTWO, LOW);
    delay(1);
  }
  bool TempStepTwoDrive = digitalRead(PINTWOSTEPDIR);
  if(TempStepTwoDrive == false)
  {
    step = step * -1;
  }
  CountStepTwo = CountStepTwo + step;
}

void SetDisplay()
{
  display.clearDisplay();
  display.setCursor(36, 12);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println(CountStepOne);
  display.setCursor(36, 32);
  display.println(CountStepTwo);
  display.display();
}

void ReadCom(){
  char number[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  int numberint = 0;
  if(command[0] == 'X')
  {
    if(command[1] == '-')
    {
      digitalWrite(PINONESTEPDIR, LOW);
      number[0] = command[1];
    }
    else 
    {
      digitalWrite(PINONESTEPDIR, HIGH);
      if(command[1] >= 48 & command[1] <= 57)
      {
        number[0] = command[1];
      }
    }
    if(command[2] != '\n' && command[2] != '\r')
    {
      if(command[2] >= 48 & command[2] <= 57)
      {
        number[1] = command[2];
      }
    }
    else
    {
      number[1] = '\n';
    }
    if(command[3] != '\n' && command[3] != '\r')
    {
    if(command[3] >= 48 & command[3] <= 57)
      {
        number[2] = command[3];
      }
    }
    else
    {
      number[2] = '\n';
    }
    if(command[4] != '\n' && command[4] != '\r')
    {
    if(command[4] >= 48 & command[4] <= 57)
      {
        number[3] = command[4];
      }
    }
    else
    {
      number[3] = '\n';
    }
    numberint = atoi(number);
    Serial.println(numberint);
    if (numberint < 0)
    {
      numberint = numberint * -1;
    }
    StepOneDrive(numberint);
  }

  if(command[0] == 'Y')
  {
    if(command[1] == '-')
    {
      digitalWrite(PINTWOSTEPDIR, LOW);
      number[0] = command[1];
    }
    else 
    {
      digitalWrite(PINTWOSTEPDIR, HIGH);
      if(command[1] >= 48 & command[1] <= 57)
      {
        number[0] = command[1];
      }
    }
    if(command[2] != '\n' && command[2] != '\r')
    {
      if(command[2] >= 48 & command[2] <= 57)
      {
        number[1] = command[2];
      }
    }
    else
    {
      number[1] = 0x00;
    }
    if(command[3] != '\n' && command[3] != '\r')
    {
    if(command[3] >= 48 & command[3] <= 57)
      {
        number[2] = command[3];
      }
    }
    else
    {
      number[2] = 0x00;
    }
    if(command[4] != '\n'  && command[4] != '\r')
    {
    if(command[4] >= 48 & command[4] <= 57)
      {
        number[3] = command[4];
      }
    }
    else
    {
      number[3] = 0x00;
    }
    numberint = atoi(number);
    Serial.println(numberint);
    if (numberint < 0)
    {
      numberint = numberint * -1;
    }
    StepTwoDrive(numberint);
  }

}
