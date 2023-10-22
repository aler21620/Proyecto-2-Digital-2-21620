//*****************************************************************************
// Universidad del Valle de Guatemala
// BE3015 - Electrónica Digital 2
// Proyecto 2 - María Alejandra Rodríguez
// Sensor de pulso - Comunicación con TIVA C y Pantalla SPI
//*****************************************************************************
//*****************************************************************************
// Librerías
//*****************************************************************************
/*#include <Arduino.h>
#include "driver/ledc.h"
#include "esp_adc_cal.h"

//*****************************************************************************
//  Definición de pines
//*****************************************************************************
#define SensorPulso 35
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
void pulso(void);

//*****************************************************************************
// Variables Globales
//*****************************************************************************
int Sensor_Raw = 0;
float Sensor1 = 0.0;
float voltaje =0.0; 

esp_adc_cal_characteristics_t adc_chars;

// Definición de variables globales
int Sensor_Raw = 0;
float Sensor1 = 0.0;
float voltaje = 0.0;

// Variables para el cálculo del pulso
unsigned long lastPulseTime = 0;
int pulseCount = 0;

//*****************************************************************************
// Configuración
//*****************************************************************************
void setup()
{
  //configurarPWM();
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  Serial.begin(115200);
}

//*****************************************************************************
// Loop
//*****************************************************************************
void loop()
{
  pulso();
  delay(1000);
}

//*****************************************************************************
// Funciones
//*****************************************************************************
/*uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return (esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}

void configurarPWM(void)
{
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
/*
void pulso(void)
{
  // Leer el pin LM35_Sensor1 ADC
  Sensor_Raw = analogRead(SensorPulso);
  // Calibrar ADC y tomar el voltaje en mV
  voltaje = readADC_Cal(Sensor_Raw);
  // TempC = función con respecto al voltaje
  Sensor1 = ((voltaje / 4095) * 3.25) / 0.01; // De ser necesario se multiplica por un factor para que lea correctamente el pulso

  // Imprimir las lecturas, para saber si el sensor funciona
  Serial.print("♥ Tu pulso es:  ");
  Serial.print(Sensor1);
  Serial.print(" BPM ♥ \n");
}


uint32_t readADC_Cal(int ADC_Raw) {
  return esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars);
}

void pulso(void) {
  Sensor_Raw = analogRead(SensorPulso);
  voltaje = readADC_Cal(Sensor_Raw);
  Sensor1 = ((voltaje / 4095) * 3.25) / 0.01; // Ajusta según tus necesidades.

  if (detectarPulso(voltaje)) {
    int BPM = calcularBPM();
    Serial.print("BPM: ");
    Serial.println(BPM);
  }
}

bool detectarPulso(float voltage) {
  static bool pulseDetected = false;
  float voltageThreshold = 2000; // Ajusta este valor según tus necesidades.

  if (voltage > voltageThreshold && !pulseDetected) {
    pulseDetected = true;
    lastPulseTime = millis();
    return true;
  }

  if (millis() - lastPulseTime > 2000) {
    pulseDetected = false;
  }

  return false;
}

int calcularBPM() {
  return (pulseCount * 30000) / (millis() - lastPulseTime);
}*/

#include <Arduino.h>
#define SensorPulso 35
#define muestra 20 // Número de muestras para calcular el promedio

unsigned int sensorValues[muestra];
int sensorIndex = 0;
int promedioSensor = 0;
unsigned long lastPeakTime = 0;
unsigned long peakInterval = 0;
int BPM = 0;

void pulso() {
  // Leer el pin LM35_Sensor1 ADC
  int Sensor_Raw = analogRead(SensorPulso);

  // Almacenar la lectura actual en el arreglo
  sensorValues[sensorIndex] = Sensor_Raw;
  sensorIndex = (sensorIndex + 1) % muestra;

  // Calcular el promedio de las últimas muestras
  promedioSensor = 0;
  for (int i = 0; i < muestra; i++) {
    promedioSensor += sensorValues[i];
  }
  promedioSensor /= muestra;

  // Detectar picos en la señal del sensor
  static int prevSensorValue = 0;
  static int peakThreshold = 10; // Ajusta este valor según tus necesidades

  if (abs(promedioSensor - prevSensorValue) > peakThreshold) {
    // Se ha detectado un pico (latido)
    // Calcula el tiempo entre picos
    unsigned long currentTime = millis();
    peakInterval = currentTime - lastPeakTime;
    lastPeakTime = currentTime;

    // Calcula el BPM basado en el tiempo entre picos
    BPM = 60000 / peakInterval; // Convierte a BPM (pulsos por minuto)
  }

  // Actualizar el valor previo
  prevSensorValue = promedioSensor;

  // Imprimir el valor del BPM
  Serial.print("♥ Tu pulso es:  ");
  Serial.print(BPM);
  Serial.print(" BPM ♥ \n");
}

void setup() {
  Serial.begin(115200);
  lastPeakTime = millis();
}

void loop() {
  pulso();
  delay(1000); // Puedes ajustar el retardo según tus necesidades
}
