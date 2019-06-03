// The bucket tested with this code is summarized here:
// http://texaselectronics.com/media/mconnect_uploadfiles/t/r/tr-525i_rainfall_user_s_manual.pdf

// Testing shows this bucket has a normally open reed switch.

#define RAIN_PIN 2          // aka interrupt pin 0 on an Arduino Uno 
#define CALC_INTERVAL 1000  // increment of measurements
#define DEBOUNCE_TIME 80    // time in milliseconds required to get through bounce noise

// http://texaselectronics.com/media/mconnect_uploadfiles/t/r/tr-525i_rainfall_user_s_manual.pdf
// Per manufatures spec on bucket being tested:

// "Average Switch closure time is 135 ms"
// "Bounce Settling Time: 0.75 ms" 

// However, my tests show that a debounce of 1 ms increments by 6 on each tip, so this 
// measuring bounce in switch via multiple interrupt calls. 

// 20 increments by 4 
// 60 increments by 2 
// 70 increments by 2 
// 75 increments by 2 
// 77 increments by 1 
// 80 increments by 1

// All tests are buggy with multiple tips or long periods of waiting.
// Sketch provided here for your own testing.

unsigned long nextCalc;
unsigned long timer;

volatile unsigned int rainTrigger = 0;
volatile unsigned long last_micros_rg;

void setup() {
  Serial.begin(9600); 
  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), countingRain, RISING); 
  
  pinMode(RAIN_PIN, INPUT);
  nextCalc = millis() + CALC_INTERVAL;
}

void loop() {
  timer = millis();
  if(timer > nextCalc) {
    nextCalc = timer + CALC_INTERVAL;
    Serial.print("Total Tips: ");
    Serial.print((float) rainTrigger);
    Serial.print("\t");
    Serial.println(sizeof((float) rainTrigger));     
  }
}

void countingRain() {
  // ATTEMPTED: Check to see if time since last interrupt call is greater than 
  // debounce time. If so, then the last interrupt call is through the 
  // noisy period of the reed switch bouncing, so we can increment by one.   
  if((long)(micros() - last_micros_rg) >= DEBOUNCE_TIME) {
   rainTrigger += 1;
   last_micros_rg = micros();
  }  
}
