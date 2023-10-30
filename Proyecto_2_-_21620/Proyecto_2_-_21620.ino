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
#include "pitches.h"
#include <stdint.h>
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
#include "lcd_registers.h"

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

//Pines pantalla
#define LCD_RST PD_0 //Definición de pin RESET pantalla SPI
#define LCD_DC PD_1 //Definición de pin DC pantalla SPI
#define LCD_CS PA_3 //Definición de pin CS pantalla SPI
// El SPI es el 0
//MOSI va a PA_5
//MISO va a PA_4
//SCK va a PA_2

//Pines SD
#define SCK A2
#define MOSI A5
#define MISO A4
#define CS 12

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
//Prototipos de función SD
void guardar(String);
//Prototipos de función que puedo utilizar con la pantalla SPI
void LCD_Init(void);
void LCD_Clear(unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

//*****************************************************************************
// Variables Globales
//*****************************************************************************
String nombre; //Nombre del archivo que abre o crea
float temp; //Para almacenar el valor de temperatura del sensor del ESP32

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

  //Inicialización pantalla SPI
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x00);

  FillRect(0,0, 320, 240,  0x37FC);
  FillRect(0, 60, 320, 220, 0xFEC6);   
  FillRect(0,180, 320, 220,  0x37FC);   
  String text1 = "TEMPERATURA";
  LCD_Print(text1, 70, 8, 2, 0x1105, 0x37FC); 
  String text2 = "ACTUAL";
  LCD_Print(text2, 115, 30, 2, 0x1105, 0x37FC); 
  String text3 = "ALEJANDRA";
  LCD_Print(text3, 90, 190, 2, 0x1105, 0x37FC);
  String text4 = "RODRIGUEZ";
  LCD_Print(text4, 90, 212, 2, 0x1105, 0x37FC); 
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
      Serial.print("🌡Tu temperatura actual es: ");
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
    tone(buzz, 494);
    delay(500); 
    noTone(buzz);
    tone(buzz, 494);
    delay(500); 
    noTone(buzz);
    tone(buzz, 440);
    delay(500); 
    noTone(buzz);
    tone(buzz, 392);
    delay(500); 
    noTone(buzz);
    tone(buzz, 440);
    delay(500); 
    noTone(buzz);
    tone(buzz, 494);
    delay(500); 
    noTone(buzz);
    tone(buzz, 392);
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

//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_DC, OUTPUT);
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_DC, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 

//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
