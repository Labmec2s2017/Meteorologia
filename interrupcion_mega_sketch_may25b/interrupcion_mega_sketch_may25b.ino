//interrupcion arduino mega
#include <TimeLib.h>
#include <TimeAlarms.h>


#define Bucket_Size 0.25 // bucket size to trigger tip count 
#define RG11_Pin 21 // digital pin RG11 connected to 

volatile unsigned long tipCount; // bucket tip counter used in interrupt routine 
volatile unsigned long contactTime; // Timer to manage any contact bounce in interrupt routine 
volatile float totalRainfall; // total amount of rainfall detected 

float mmlluvia = 0; //lluvia en 1 hora
float lluviahoras[24]; //valores horarios de lluvia para cada dia

void setup() { 

tipCount = 0; 
totalRainfall = 0; 



Serial.begin(9600); 

Serial.println("\tRainfall"); 




pinMode(RG11_Pin, INPUT); 
//attachInterrupt(digitalPinToInterrupt(RG11_Pin), isr_rg, FALLING);
attachInterrupt(0, isr_rg, FALLING); 
sei();// Enable Interrupts 
} 

void loop() { 

Serial.print(totalRainfall/2); 
Serial.print("\t");
Serial.print(" mm"); 
Serial.print("\t");
Serial.println(tipCount/2);

// Add a 2 second delay. 
delay(2000); //just here to slow down the output. 

} 

// Interrupt handler routine that is triggered when the rg-11 detects rain 
void isr_rg() { 

if((millis() - contactTime) > 15 ) { // debounce of sensor signal 
tipCount++; 
totalRainfall = tipCount * Bucket_Size; 
contactTime = millis();

} 
} 
// end of rg-11 rain detection interrupt handler
//-----------------------------------------------------CODIGO-PRECIPITACION--------------------------------
//---------------------------------------------------------------------------------------------------------
void calculolluvia(){
  mmlluvia = tipCount/2 * Bucket_Size;//pasos * capacidad;//calcula mm de lluvia en 1 hora
  muestraresultadoshora();//muestra los resultados de la ultima hora
  lluviahoras[hour()]= mmlluvia; //almacena el valor de la lluvia en esa hora
  if (hour()==00){
    //lluviahoras[]={0}; //pone a cero los datos de precipitacion horaria
  }
  tipCount = 0;//pasos = 0;//pone a cero el contador de balanceos
  unsigned short hora; //ajusta la alarma para el siguiente periodo de medida
  hora = hour() + 1;
  if(hora == 23){
    hora = 00;
    
    
  }
  Alarm.alarmRepeat(hora,00,00,calculolluvia);
}


void muestraresultadoshora(){
  Serial.print("son las");
  Serial.print(hour());
  Serial.print(". Durante la última hora: ");
  Serial.print(tipCount/2);//(pasos);
  Serial.print(" pasos x 0.25 mm = ");
  Serial.print(mmlluvia);
  Serial.println("mm de precipitación");
  Serial.println(); 
}
