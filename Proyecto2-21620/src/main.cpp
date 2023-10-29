//*****************************************************************************
// Universidad del Valle de Guatemala
// BE3015 - Electrónica Digital 2
// Proyecto 2 - María Alejandra Rodríguez
// Sensor de pulso - Comunicación con TIVA C y Pantalla SPI
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
#define SensorPulso 35 //Sensor del proyecto 
#define RX_2 16 //Para comunicación serial con TIVA
#define TX_2 17 //Para comunicación serial con TIVA
#define led 18

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
uint32_t readADC_Cal(int ADC_Raw);
void pulso(void);

//*****************************************************************************
// Variables Globales
//*****************************************************************************
int Sensor_Raw = 0;
float Sensor1 = 0.0;
float voltaje =0.0; 
int sensorValue = 0;
int prevSensorValue = 0;
bool pulseDetected = false;
unsigned long lastBeatTime = 0;
float bpm = 0;
//float factor = 0.75; 
//float maximo = 0.0; 
//int minimoEntreLatidos = 300; 
//float valorAnterior = 500; 
//int latidos = 0; 

//esp_adc_cal_characteristics_t adc_chars;

/*// Definición de variables globales
int Sensor_Raw = 0;
float Sensor1 = 0.0;
float voltaje = 0.0;

// Variables para el cálculo del pulso
unsigned long lastPulseTime = 0;
int pulseCount = 0;*/

//*****************************************************************************
// Configuración
//*****************************************************************************
void setup() {
  //esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  pinMode(led, OUTPUT);
  //Comunicación UART0 con la computadora Serial (0)
  Serial.begin(115200);
  Serial.println("Iniciando las mediciones: ");

  Serial.begin(115200); //Velocidad del ESP32
  Serial.println("Se configuró Serial 0");

  Serial2.begin(115200, SERIAL_8N1, RX_2, TX_2); //Establecer comunicación serial con TIVA


  //Ejemplo UART 
  //Serial.begin(115200);
  //Serial2.begin(115200);
}

//*****************************************************************************
// Loop
//*****************************************************************************
void loop() {
  pulso();
  delay(1000);


  //Enviar datos del potenciómetro 1 a TIVA C
  Serial2.println("\n Valor Sensor: "); 
  Serial2.println(SensorPulso);
  
  //Recibir datos de la TIVA C para colocar en la LCD
  while (Serial2.available()) {
    uint8_t voltaje2 = Serial2.parseInt(); //Definir variables donde se almacenará cada dato 
    uint8_t contador = Serial2.parseInt(); 
    String datos = Serial2.readStringUntil('\n'); //Leer los datos enviados desde TIVA C
  }

  //EJEMPLO UART 
  //if (Serial2.available() > 0) {
    //String bufferTIVA = Serial2.readStingUntil('\n');  
  //}
  //if (bufferTIVA == "m") {
    //medirSensor();
    //Serial2.println(valorSensor);
    //Serial.println(valorSensor);
    //bufferTIVA = "";
  //}
}

//*****************************************************************************
// Funciones
//*****************************************************************************
uint32_t readADC_Cal(int ADC_Raw) {
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return (esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}

void pulso(void) {
// Leer el pin LM35_Sensor1 ADC
  Sensor_Raw = analogRead(SensorPulso);
  // Calibrar ADC y tomar el voltaje en mV
  voltaje = readADC_Cal(Sensor_Raw);
  Sensor1 = 60/(voltaje/1000); // De ser necesario se multiplica por un factor para que lea correctamente el pulso

  // Imprimir las lecturas, para saber si el sensor funciona
  Serial.print("♥ Tu pulso es:  ");
  Serial.print(Sensor1);
  Serial.print(" BPM ♥ \n");
  Serial.println(voltaje);
  Serial.println(Sensor_Raw);
}

/*void pulso(void) {
  // Leer el pin LM35_Sensor1 ADC
  Sensor_Raw = analogRead(SensorPulso);
  // Calibrar ADC y tomar el voltaje en mV
  voltaje = readADC_Cal(Sensor_Raw);
  Sensor1 = 60/(voltaje/1000); // De ser necesario se multiplica por un factor para que lea correctamente el pulso

  // Imprimir las lecturas, para saber si el sensor funciona
  Serial.print("♥ Tu pulso es:  ");
  Serial.print(Sensor1);
  Serial.print(" BPM ♥ \n");
  Serial.println(voltaje);
  Serial.println(Sensor_Raw);
  /*static unsigned long tiempoLPM = millis(); 
  static unsigned long entreLatidos = millis(); 

  float valorFiltrado = factor * valorAnterior + (1 - factor) * Sensor_Raw; //Filtro pasa bajas
  float cambio = valorFiltrado - valorAnterior; 
  valorAnterior = valorFiltrado;

  if((cambio>= maximo) && (millis() > entreLatidos + minimoEntreLatidos)) {
    maximo = cambio; 
    digitalWrite(led, HIGH); 
    entreLatidos = millis(); 
    latidos++; 
  } else { 
    digitalWrite(led, LOW); 
  }

  maximo = maximo*0.97; 

  if (millis() >= tiempoLPM + 15000) {
    Serial.print("♥ Tu pulso es:  ");
    Serial.print(latidos*4);
    Serial.print(" BPM ♥ \n");
    latidos = 0; 
    tiempoLPM = millis(); 
  }

  delay(50);
}*/

/*
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
}

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
}*/
