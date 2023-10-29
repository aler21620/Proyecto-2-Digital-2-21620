//*****************************************************************************
// Universidad del Valle de Guatemala
// BE3015 - Electrónica Digital 2
// Proyecto 2 - María Alejandra Rodríguez
//*****************************************************************************
//*****************************************************************************
// Librerías
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include <SPI.h>
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
#include "lcd_registers.h"

//*****************************************************************************
//  Definición de pines
//*****************************************************************************
#define boton1 PUSH1 //Definición del botón para la varibale de contador
#define boton2 PUSH2 //Definición del botón 2 para la variable de contador
#define RX_2 PD6 //Para comunicación serial con ESP32
#define TX_2 PD7 //Para comunicación serial con ESP32
#define buzz 40 //Definición del buzzer 
#define LCD_RST PD_0 //Definición de pin RESET pantalla SPI
#define LCD_DC PD_1 //Definición de pin DC pantalla SPI
#define LCD_CS PA_3 //Definición de pin CS pantalla SPI

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
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
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

//*****************************************************************************
// Variables Globales
//*****************************************************************************
uint8_t contador = 0; //Para almacenar el valor del contador de 0 a 255

//*****************************************************************************
// Configuración
//*****************************************************************************
void setup() {
  Serial.begin(9600); //Velocidad del monitor serial
  Serial.println("Se configuró Serial 0");

  //Comunicación UART2 con el ESP32, Serial (2)
  Serial2.begin(115200); //Velocidad de la comunicación 

  pinMode(boton1, INPUT_PULLUP); //Configuración del botón como entrada
  pinMode(boton2, INPUT_PULLUP); //Configuración del botón como entrada

  //EJEMPLO UART 
  //Serial.begin(115200); 
  //Serial2.begin(115200); 
}

//*****************************************************************************
// Loop
//*****************************************************************************
void loop() {
  //Condiciones para sumar o restar con los botones en la variable contador
  if (digitalRead(boton1) == LOW) {
    contador = contador + 1;
  }

  if (digitalRead(boton2) == LOW) {
    contador = contador - 1;
  }
  delay(100);

  analogWrite(verde, contador); //Asignar el valor al brillo del led verde
  Serial2.println("\n Azul"); //Enviar información sobre led azul al ESP32
  Serial2.println(voltaje2); //Enviar valor del led azul al ESP32
  Serial2.println("\n Verde"); //Enviar información sobre led verde al ESP32
  Serial2.println(contador); //Enviar valor del led verde al ESP32
  delay(250);

  //Recibir información enviada desde el ESP32
  while (Serial2.available()) { 
    uint8_t voltaje1 = Serial2.parseInt(); //Definir variable donde se almacenará el dato 
    String datos = Serial2.readStringUntil('\n'); //Leer el dato enviado desde el ESP32
    Serial.print("\n Rojo: "); //Imprimir en el monitor serial los datos del led rojo
    Serial.print(voltaje1); //Imprimir en el monitor el valor del led rojo 
    analogWrite(rojo, voltaje1); //Asignar el valor al brillo del led rojo 
  }

  //EJEMPLO UART
  //if (Serial2.available()) {
  //int inByte
}

//*****************************************************************************
// Funciones
//*****************************************************************************
