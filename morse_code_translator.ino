#include<SPI.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>
Bounce debouncer;
#define sw 128 //width screen
#define sh 32 //hight screen
#define OLED_RESET            4

Adafruit_SSD1306 display(sw, sh, &Wire,-1);
#define Myinput 7
#define LED 8
char transcode[36] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9'};
byte morse[36][5] = {
    /* A‑Z */
    {0,1,3,3,3},   /* A */
    {1,0,0,0,3},     /* B */
    {1,0,1,0,3},     /* C */
    {1,0,0,3,3},    /* D */
    {0,3,3,3,3},  /* E */
    {0,0,1,0,3},     /* F */
    {1,1,0,3,3},    /* G */
    {0,0,0,0,3},     /* H */
    {0,0,3,3,3},   /* I */
    {0,1,1,1,3},     /* J */
    {1,0,1,3,3},    /* K */
    {0,1,0,0,3},     /* L */
    {1,1,3,3,3},   /* M */
    {1,0,3,3,3},   /* N */
    {1,1,1,3,3},    /* O */
    {0,1,1,0,3},     /* P */
    {1,1,0,1,3},     /* Q */
    {0,1,0,3,3},    /* R */
    {0,0,0,3,3},    /* S */
    {1,3,3,3,3},  /* T */
    {0,0,1,3,3},    /* U */
    {0,0,0,1,3},     /* V */
    {0,1,1,3,3},    /* W */
    {1,0,0,1,3},     /* X */
    {1,0,1,1,3},     /* Y */
    {1,1,0,0,3},     /* Z */

    /* 0‑9  (indices 26‑35) */
    {1,1,1,1,1},      /* 0 */
    {0,1,1,1,1},      /* 1 */
    {0,0,1,1,1},      /* 2 */
    {0,0,0,1,1},      /* 3 */
    {0,0,0,0,1},      /* 4 */
    {0,0,0,0,0},      /* 5 */
    {1,0,0,0,0},      /* 6 */
    {1,1,0,0,0},      /* 7 */
    {1,1,1,0,0},      /* 8 */
    {1,1,1,1,0}       /* 9 */
};
bool keyIsPressed() {
  debouncer.update();
  return debouncer.read() == LOW;   
}
byte  READinput(){
  
  while (!keyIsPressed()); 
  digitalWrite(LED,LOW);
  unsigned long first = millis();
  while (keyIsPressed()); 
  digitalWrite(LED, HIGH);
  unsigned long dif = millis() - first;
  
  if(dif >=400){
    return 1;

  }else if(dif >= 200 && dif < 400){
    return 0;
  }else{
    return 3;
  }
}
void getINPUT (byte code[20]){
  int k=0;
  int i=0;
  while(k!=3){
    k=READinput();
    code[i]=k;
    i++;
    if(k == 3) break;

  }

}
void translate(byte code[20],char smt[20]){
  
  int k=0;
  for (int i=0;i<36;i++){
    boolean check=true;
    for(int j=0;j<5;j++){
      if(code[j]==3 && morse[i][j]==3 ){
        break;
      }
      if(code[j]!=morse[i][j] ){
        check =false;
        break;
      }
    }
    if(check){
      smt [k]=transcode[i];
      k++;
      break;
    }
  }
  smt [k]='\0';
}
void printWord(char cracked[20]) {
  display.clearDisplay();
  display.setCursor(0, 0); 
  display.print(cracked);
  display.display();    
}
void printstart() {
  display.clearDisplay();
  display.setCursor(0, 0); 
  display.print("start clicking");
  display.display();    
}
void morseCode(){
  printstart();

  
  byte code[20];
  char smt[20];
  getINPUT(code);
  translate(code,smt);
  delay(500);
  printWord(smt);
  delay(500);
  

  
}




void setup() {
  pinMode(Myinput, INPUT_PULLUP);
  debouncer.attach(Myinput);
  debouncer.interval(10);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Wire.begin();

  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Ready");
  display.display();
  delay(1000);

}

void loop() {
  
  morseCode();
  display.clearDisplay();

}
