
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  OPEN CURING
//
//  The circuit:
//  * This code is made to be used with the Open Curing Controller V1
//
//  The License: 
//  * Created in 2024 by nas.craftsman https://github.com/NasCraftsman
//  * This example code is in the public domain.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LED      26      // General purpose LED
#define BUT      15      // General purpose button
#define POT      36      // General purpose Potentiometer
#define UV       25      // Control pin for UV LEDs

#define A1       27      // Stepper motor A1
#define A2       14      // Stepper motor A2
#define A3       33      // Stepper motor A3
#define A4       32      // Stepper motor A4

#define OLED_WIDTH 128 // OLED display width,  in pixels
#define OLED_HEIGHT 32 // OLED display height, in pixels



#include <TinyStepper_28BYJ_48.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

bool button=false, go=false;
double potentiometer=0;
const int STEPS_PER_REVOLUTION = 2048;
int state=0,vel=0,power=0,tim=0; 
String states[1]={"TIM  "};
String cowntdown[11]={"  10   ","   9   ","   8   ","   7   ","   6   ","   5   ","   4   ","   3   ","   2   ","   1   ","  GO!!   "};

TinyStepper_28BYJ_48 stepper;
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);


void read_UI(){
  if(!digitalRead(BUT)& !button){
    button=true;
    state++;
    if(state>0){
      state=0; 
      go=true;
    }
    Serial.print("Button Pressed! Menu:");
    Serial.println(state);
  }
  if(digitalRead(BUT)){
    button=false;
  }
  potentiometer=100-((analogRead(POT)-2450)*100)/1645; 
}

void print_Display(){
   oled.clearDisplay();          // clear display
   oled.setTextSize(3);          // text size
   oled.setTextColor(WHITE);     // text color
   oled.setCursor(0, 10);        // position to display
   oled.print(states[state]);    // text to display
   if(state==0){
      oled.print(tim);
      tim=potentiometer/3;
   }
   else if(state==1){
      oled.print(vel);
      vel=potentiometer/10;
   }
   else if(state==2){
      oled.print(power);
      power=potentiometer/10;
   }
   else{
    
   }
   oled.display();               
}
void curing(){
   Serial.println("Starting count down...");
   for(int x=0; x<11; x++){
      oled.clearDisplay();                // clear display
      oled.setTextSize(3);                // text size
      oled.setTextColor(WHITE);           // text color
      oled.setCursor(0, 10);              // position to display
      oled.println(cowntdown[x]);                   // text to display
      oled.display();  
      delay(1000);
      Serial.println(cowntdown[x]);
   }
   Serial.println("Starting curing process...");
   Serial.print("Speed: ");
   Serial.println(vel);
   Serial.print("Power: ");
   Serial.println(power);
   Serial.print("Timer: ");
   Serial.println(tim);
   
   oled.clearDisplay();                
   oled.println("UV ON!!");
   oled.display();  
   delay(1000);delay(1000);
   digitalWrite(UV, HIGH);
   stepper.setSpeedInStepsPerSecond(2048/(11-vel));
   stepper.setAccelerationInStepsPerSecondPerSecond(512);
   stepper.moveRelativeInSteps(60*tim*(2048/(11-vel))); 
   digitalWrite(UV, LOW); 
   oled.clearDisplay();
   oled.setTextSize(3);                
   oled.setTextColor(WHITE);     
   oled.println("DONE");
   oled.display();
   delay(1000);
   go=false;
}

void setup() {
   Serial.begin(115200);
   Serial.println("---------------------SETTING UP----------------------");
   pinMode(LED, OUTPUT);
   pinMode(UV, OUTPUT);
   pinMode(BUT, INPUT);
   stepper.connectToPins(A1, A2, A3, A4);
   if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      while (true);
    }
   //oled.rotation=3;
   Serial.println("-----------------------------------------------------");
   digitalWrite(LED, HIGH);
   oled.clearDisplay();                // clear display
   oled.setTextSize(3);                // text size
   oled.setTextColor(WHITE);           // text color
   oled.setCursor(0, 10);              // position to display
   oled.println("HELLO!");                   // text to display
   oled.display();  
   delay(1000);
}

void loop() {
  read_UI();
  if(go){ //start Curing 
      curing();
  }
  print_Display();
}
