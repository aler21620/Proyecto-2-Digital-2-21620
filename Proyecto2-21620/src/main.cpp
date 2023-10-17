//*****************************************************************************
// Universidad del Valle de Guatemala
// BE3015 - Electrónica Digital 2
// Proyecto 2 - María Alejandra Rodríguez
//*****************************************************************************
//*****************************************************************************
// Librerías
//*****************************************************************************
#include <Arduino.h>
#include "driver/ledc.h"
#include "esp_adc_cal.h"

//*****************************************************************************
//  Definición de pines
//*****************************************************************************
#define Sensor 35 
#define pwmChannel 0
#define ledRChannel 1
#define ledGChannel 2
#define ledBChannel 3
#define freqPWM 500
#define resolution 8
#define pinLedR 5
#define pinLedB 18
#define pinLedG 19
#define pinPWM 15

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
uint32_t readADC_Cal(int ADC_Raw); 
void configurarPWM(void); 
void temperatura_led(void); 

//*****************************************************************************
// Variables Globales
//*****************************************************************************
int Sensor_Raw = 0; 
float Sensor1 = 0.0;
float voltaje =0.0;

//*****************************************************************************
// Configuración
//*****************************************************************************
void setup() {
  configurarPWM();
  Serial.begin(115200);
}

//*****************************************************************************
// Loop
//*****************************************************************************
void loop() {
  temperatura_led();
  delay(1000);
}

//*****************************************************************************
// Funciones
//*****************************************************************************
uint32_t readADC_Cal(int ADC_Raw){
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}

void configurarPWM(void) {
  // Paso 1: Configurar el módulo PWM
  ledcSetup(pwmChannel, freqPWM, resolution);
  ledcSetup(ledRChannel, freqPWM, resolution);
  ledcSetup(ledGChannel, freqPWM, resolution);
  ledcSetup(ledBChannel, freqPWM, resolution);

  // Paso 2: seleccionar en qué GPIO tendremos nuestra señal PWM
  ledcAttachPin(pinPWM, pwmChannel);
  ledcAttachPin(pinLedR, ledRChannel);
  ledcAttachPin(pinLedG, ledGChannel);
  ledcAttachPin(pinLedB, ledBChannel);
}

void temperatura_led(void){
  // Leer el pin LM35_Sensor1 ADC 
  Sensor_Raw = analogRead(Sensor);  
  //Calibrar ADC y tomar el voltaje en mV 
  voltaje = readADC_Cal(Sensor_Raw);
  // TempC = función con respecto al voltaje
  Sensor1 = ((voltaje/4095)*3.25) / 0.01; //De ser necesario se multiplica por un factor para que lea correctamente la temperatura
  
  // Imprimir las lecturas, para saber si el sensor funciona 
  Serial.print("Lectura de la temperatura = ");
  Serial.print(Sensor1);
  Serial.print(" °C \n");
}