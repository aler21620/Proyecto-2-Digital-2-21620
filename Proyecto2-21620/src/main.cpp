//*****************************************************************************
// Universidad del Valle de Guatemala
// BE3015 - Electrónica Digital 2
// Proyecto 2 - María Alejandra Rodríguez
// Sensor de temperatura - Comunicación con TIVA C y Pantalla SPI
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
#define SensorTemp 35 //Sensor del proyecto 
#define RX_2 16 //Para comunicación serial con TIVA
#define TX_2 17 //Para comunicación serial con TIVA

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
uint32_t readADC_Cal(int ADC_Raw);
void temperatura(void);

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
  temperatura();
  delay(1000);


  //Enviar datos del potenciómetro 1 a TIVA C
  Serial2.println("\n Valor Sensor: "); 
  Serial2.println(Sensor1);
  
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

void temperatura(void) {
// Leer el pin LM35_Sensor1 ADC
  Sensor_Raw = analogRead(SensorTemp);
  // Calibrar ADC y tomar el voltaje en mV
  voltaje = readADC_Cal(Sensor_Raw);
  Sensor1 = ((voltaje/4095)*3.25)/0.01; // De ser necesario se multiplica por un factor para que lea correctamente la temperatura

  // Imprimir las lecturas, para saber si el sensor funciona
  Serial.print("🌡️ Tu temperatura actual es:   ");
  Serial.print(Sensor1);
  Serial.print("  °C 🌡️\n");
}