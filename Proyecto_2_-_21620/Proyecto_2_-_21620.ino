//*****************************************************************************
// Universidad del Valle de Guatemala
// BE3015 - Electrónica Digital 2
// Proyecto 2 - María Alejandra Rodríguez
//*****************************************************************************
//*****************************************************************************
// Librerías
//*****************************************************************************
#include <SPI.h>
#include <SD.h>
/*#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"*/

//*****************************************************************************
//  Definición de pines
//*****************************************************************************
//Pines botones
#define boton1 PUSH1 //Definición del botón para la varibale de contador
#define boton2 PUSH2 //Definición del botón 2 para la variable de contador
//Pines comunicación serial
#define RX_2 PD6 //Para comunicación serial con ESP32
#define TX_2 PD7 //Para comunicación serial con ESP32
//Pin buzzer
#define buzz 40 //Definición del buzzer 
/*
//Pines pantalla
#define LCD_RST PD_0 //Definición de pin RESET pantalla SPI
#define LCD_DC PD_1 //Definición de pin DC pantalla SPI
#define LCD_CS PA_3 //Definición de pin CS pantalla SPI*/
//Pines SD
#define SCK A2
#define MOSI A5
#define MISO A4
#define CS 32

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
/*//Prototipos de función SD
void guardar(String);
//Prototipos de función que puedo utilizar con la pantalla SPI
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);*/

//*****************************************************************************
// Variables Globales
//*****************************************************************************
String nombre; //Nombre del archivo que abre o crea
float temp; //Para almacenar el valor de temperatura del sensor del ESP32
int numTones = 10;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};

//*****************************************************************************
// Configuración
//*****************************************************************************
void setup() {
  SPI.setModule(0);
  Serial.begin(115200); //Velocidad del monitor serial
  Serial.println("Se configuró Serial 0");
 
  //Comunicación UART2 con el ESP32, Serial (2)
  Serial2.begin(115200); //Velocidad de la comunicación 

  pinMode(boton1, INPUT_PULLUP); //Configuración del botón como entrada
  pinMode(boton2, INPUT_PULLUP); //Configuración del botón como entrada
  pinMode(buzz, OUTPUT); 

  // Inicializa la comunicación con la tarjeta SD
  if (!SD.begin(CS)) {
    //Indica que algo pasó y no se inicializó correctamente
    Serial.println("No se pudo inicializar la tarjeta SD.");
    return;
  }
  //Indica que se inicializó correctamente
  Serial.println("Tarjeta SD inicializada correctamente.");
}

//*****************************************************************************
// Loop
//*****************************************************************************
void loop() {
  //Boton para leer el dato del sensor
  int data = digitalRead(boton1);
  //Condiciones para sumar o restar con los botones en la variable contador
  if (data == LOW) {
    //Envío de un entero a ESP32 para que el microcontrolador sepa que debe enviar la última lectura
    Serial2.println('1');
    if(Serial2.available() > 0) {
      temp = Serial2.parseFloat();
      Serial.print("\n 🌡Tu temperatura actual es: ");
      Serial.print(temp);
      Serial.print("  °C 🌡\n");
    }
    tone(buzz, 349);
    delay(500); 
    noTone(buzz);
    tone(buzz, 523);
    delay(500); 
    noTone(buzz);
    tone(buzz, 349);
    delay(500); 
    noTone(buzz);
    tone(buzz, 330);
    delay(500); 
    noTone(buzz);
  }

  if (digitalRead(boton2) == LOW) {
    guardar("Sensor.txt");
    delay(250);
    tone(buzz, 49);
    delay(500); 
    noTone(buzz);
    tone(buzz, 349);
    delay(500); 
    noTone(buzz);
    tone(buzz, 330);
    delay(500); 
    noTone(buzz);
  }
  delay(100);
}

//*****************************************************************************
// Funciones
//*****************************************************************************
//Función para guardar el dato en la memoria SD
void guardar(String nombre) {
  File archivo = SD.open("Sensor.txt", FILE_WRITE);

  if (archivo) {
    archivo.print("🌡Tu temperatura actual es: ");
    archivo.print(temp);
    archivo.print("  °C 🌡");
    archivo.println();
    archivo.close();
    Serial.println("Datos de temperatura registrados correctamente en la SD");
  } else {
    Serial.println("No se pudo abrir el archivo para guardar datos.");
  }
}
