const int RAIN_INT = 3;  
#define LED_PIN 13
volatile int myTotal = 0;

void setup() {
  pinMode(RAIN_INT, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(RAIN_INT), myCount, RISING);
  Serial.begin(9600);
}

void loop() {
 Serial.print("Count is: ");
 Serial.print(myTotal);
 Serial.print(" and interrupt is ");
 Serial.println(digitalRead(RAIN_INT));
 delay(1000);
 digitalWrite(LED_PIN, LOW);
}

void myCount() {
 myTotal +=1;
 digitalWrite(LED_PIN, HIGH);
}

