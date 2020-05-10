/***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * IE3027: Electrónica Digital 2 - 2019
 */
//***************************************************************************************************************************************
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

#define LCD_RST PE_2
#define LCD_CS PE_3
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
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

extern uint8_t cancha[];
extern uint8_t disparorojo[];
extern uint8_t pelota[];
extern uint8_t fondo[];
extern uint8_t transformacionSS5[];

////////////////////////////////// VARIABLES CONTROLES JUGADOR 1/////////////////////////////////////////////////////////////////////////////////////////////////////
//     VARIABLES

// pelota
int CAIDA = +3;


const int DER = PA_2;
int DERECHA1 = 0;         //derecha

const int IZQ = PF_4;
int IZQUIERDA1 = 0;       //izquierda

const int ARR = PD_7;
int ARRIBA1 = 0;          // arriba

//COORDENADAS SPRITE
int POSX1 = 60;
int LINEAV = POSX1-2;
int POSY1 = 186;
int LINEAH = POSY1-1;

/////////////////////////////////VARIABLES CONTROLES JUGADOR 2///////////////////////////////////////////////////////////////////////////////////////////////////////
//VARIABLES

const int DER2 = PA_4;
int DERECHA2 = 0;         //izquierda

const int IZQ2 = PA_3;
int IZQUIERDA2 = 0;       //derecha

const int ARR2 = PD_6;
int ARRIBA2 = 0;          //arriba

//COORDENADAS SPRITE
int POSX2 = 249;//249
int LINEAV2 = POSX2-2;
int POSY2 = 186;
int LINEAH2 = POSY2-1;

/////////////////////COORDENADAS DE LA PELOTA////////////////////////////////////////////////////////////////////////////////////////////////////////0
int POSXP = 150;
int POSYP = 60;
int LINEAHP = POSYP-1;
int LINEAVP = POSXP - 2;
int DIRECCION = 0;

int bandera = 1;

int contadorgol1 = 0;
int contadorgol2=0;
String gol1="0";
String gol2= "0";
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  
  pinMode(DER, INPUT_PULLUP);
  pinMode(IZQ, INPUT_PULLUP);
  pinMode(ARR, INPUT_PULLUP);
  pinMode(DER2, INPUT_PULLUP);
  pinMode(IZQ2, INPUT_PULLUP);
  pinMode(ARR2, INPUT_PULLUP);
    
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x00);
  
   
  //FillRect(0, 0, 320, 240, 0x0000);   //0x421b

  /////////////////////////// INTRODUCCIÓN ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  LCD_Bitmap(0, 0, 320, 240, cancha);
  String text1 = "Futbol sayayin";
  LCD_Print(text1, 35, 20, 2, 0x0000, 0xffff);    //0x421b x35
  String text2 = "cargando...";
  LCD_Print(text2, 142, 200, 2, 0x0000, 0xffff);   
  
//LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
//LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
  
   
   for(int x = 0; x <50; x++){
    delay(15);
    int anim2 = (x/15)%2;
    int anim1 = (x/15)%7;
    LCD_Sprite(20,165,130,40,disparorojo,2,anim2,0,0);
    
   }
   
     
//LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    
//LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
//LCD_Bitmap(0, 0, 320, 240, cancha);
    
        
    FillRect(0, 0, 320, 240, 0x0042);   ///fondo oscuro
 
  for(int x = 0; x <319; x++){
    //LCD_Bitmap(x, 30, 72, 30, imagenSS5); //tile2 0x8410
    //LCD_Bitmap(x, 68, 16, 16, tile);
    
    //LCD_Bitmap(x, 207, 16, 16, tile);
    LCD_Bitmap(x, 223, 16, 16, piso);   /// suelo 
    x += 15;
 }

}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {

  
  LCD_Sprite(2,58,30,165,porterilla,1,0,0,0);
  LCD_Sprite(288 ,58,30,165,porterilla,1,0,1,0);
  LCD_Sprite(3,2,24,30,imagenSS5,1,0,0,0);
  LCD_Sprite(295,2,25,35,imagenSS2,1,0,0,0);     
  //LCD_Sprite(150,100,25,25,pelota,7,0,0,0);
  //LCD_Sprite(70,195,24,27,saltoSS5,2,0,0,0);
  //LCD_Sprite(250,195,24,27,saltoSS2,2,0,1,0);                            // PORTERILLAS, IMAGENES, SUELO, BALON

  DERECHA1 = digitalRead (DER);
  IZQUIERDA1 = digitalRead (IZQ);
  ARRIBA1 = digitalRead (ARR);
  DERECHA2 = digitalRead (DER2);
  IZQUIERDA2 = digitalRead (IZQ2);
  ARRIBA2 = digitalRead (ARR2);

  
 //////////MOVIMIENTO JUGADOR 1/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   IZQ  && POSX1<=POSX2-23 && POSX1>=POSY2 && POSY1<POSY2+28 && POSY1+28>POSY2
    if(DERECHA1==LOW && POSX1<260 ){                              // DERECHA && POSX1+19>=POSX2 && POSX1<POSX2+19 && POSY1<POSY2+28 && POSY1+28>POSY2
        POSX1 = POSX1 + 3;
      LINEAV = POSX1 - 1;
      LCD_Sprite(POSX1,POSY1,23,27,movimientoSS5,2,1,0,0);
      V_line( LINEAV, POSY1, 27, 0x0042);
      V_line( LINEAV-1, POSY1, 27, 0x0042);
      V_line( LINEAV-2, POSY1, 27, 0x0042);
    }
    else {
      LCD_Sprite(POSX1,POSY1,23,27,movimientoSS5,2,0,0,0);
    }                                                               ////////movimiento derecha 1

    if(IZQUIERDA1==LOW && POSX1>34 ){//&& (POSX1+24!=POSX2 && POSX1!=POSX2+19 &&POSY1!=POSY2 )){// && POSX1-24<=POSX2 && POSX1>=POSX2+19 && POSY1<=POSY2+28 && POSY1+28>=POSY2){
      bandera = 1;
      POSX1 = POSX1 - 3;
      LINEAV = POSX1 + 24;
      LCD_Sprite(POSX1,POSY1,23,27,movimientoSS5,2,1,1,0);
      V_line( LINEAV, POSY1, 27, 0x0042);
      V_line( LINEAV+1, POSY1, 27, 0x0042);
      V_line( LINEAV+2, POSY1, 27, 0x0042);
    }                                                               //////// movimiento izquierda 1
    if(ARRIBA1==LOW  && POSY1>42){
      bandera = 1;
      POSY1 = POSY1 - 3;
      LINEAH = POSY1 + 28;
      LCD_Sprite(POSX1,POSY1,24,27,saltoSS5,2,1,0,0);
      H_line( POSX1, LINEAH, 27, 0x0042);
      H_line( POSX1, LINEAH+1, 27, 0x0042);
      H_line( POSX1, LINEAH+2, 27, 0x0042);

    }
    else if(ARRIBA1=HIGH && POSY1<=192) {
      POSY1 = POSY1 + 3;
      LINEAH = POSY1 -1;
      LCD_Sprite(POSX1,POSY1,24,27,saltoSS5,2,1,0,0);
      H_line( POSX1, LINEAH, 27, 0x0042);
      H_line( POSX1, LINEAH-1, 27, 0x0042);
      H_line( POSX1, LINEAH-2, 27, 0x0042);

      }

///// MOVIMIENTO JUGADOR 2/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


     if(DERECHA2==LOW && POSX2<261){ //&& POSX2!=POSX1 && POSY2!=POSY1){
      bandera = 1;
      POSX2 = POSX2 + 3;
      LINEAV2 = POSX2 - 1;
      LCD_Sprite(POSX2,POSY2,19,28,movimientoSS2,2,1,0,0);
      V_line( LINEAV2, POSY2, 28, 0x0042);
      V_line( LINEAV2-1, POSY2, 28, 0x0042);
      V_line( LINEAV2-2, POSY2, 28, 0x0042);
      V_line( LINEAV2-3, POSY2, 28, 0x0042);
    }
    else {
      LCD_Sprite(POSX2,POSY2,19,28,movimientoSS2,2,0,1,0);
    }                                                               ////////movimiento derecha 

    if(IZQUIERDA2==LOW && POSX2>36){
      bandera = 1;
      POSX2 = POSX2 - 3;
      LINEAV2 = POSX2 + 21;
      LCD_Sprite(POSX2,POSY2,19,28,movimientoSS2,2,1,1,0);
      V_line( LINEAV2, POSY2, 28, 0x0042);
      V_line( LINEAV2+1, POSY2, 28, 0x0042);
      V_line( LINEAV2+2, POSY2, 28, 0x0042);
    }                                                                 ///////////// movimiento izquierda
    if(ARRIBA2==LOW && POSY2>41){
      bandera = 1;
      POSY2 = POSY2 - 3;
      LINEAH2 = POSY2 + 29;
      LCD_Sprite(POSX2,POSY2,24,28,saltoSS2,2,1,1,0);
      H_line( POSX2, LINEAH2, 24, 0x0042);
      H_line( POSX2, LINEAH2+1, 24, 0x0042);
      H_line( POSX2, LINEAH2+2, 24, 0x0042);
    }
    else if(ARRIBA2=HIGH && POSY2<=192) {
      POSY2 = POSY2 + 3;
      LINEAH2 = POSY2 -1;
      LCD_Sprite(POSX2,POSY2,24,28,saltoSS2,2,1,1,0);
      H_line( POSX2, LINEAH2, 28, 0x0042);
      H_line( POSX2, LINEAH2-1, 28, 0x0042);
      H_line( POSX2, LINEAH2-2, 28, 0x0042);

      }
          if (POSXP == 288){
              contadorgol1 = contadorgol1+1;
              String gool = "Gooool!!";
              LCD_Print(gool, 100, 60, 2, 0x0000, 0xffff);    //0x421b x35
              POSXP = 150;
              POSYP = 90;
              LCD_Sprite(POSXP,POSYP,25,25, pelota,7,1,0,0);
              POSYP = POSYP+CAIDA;
              LCD_Sprite(POSXP,POSYP,25,25, pelota,7,1,0,0);

                if (POSYP > 140){
                  FillRect(100, 90, 100, 100, 0x0042);  
                }

                if (contadorgol1 == 0){
                  String gol1 = "0";
                  String jugador1 = gol1;
                   LCD_Print(jugador1, 270, 20, 2, 0x0000, 0xffff);
                }
  

               if (contadorgol1 == 3){
                String gol1 = "3";
                  String jugador1 = gol1;
                   LCD_Print(jugador1, 35, 20, 2, 0x0000, 0xffff);
                   LCD_Bitmap(0, 0, 320, 240, cancha);  
                   String ganador1 = "GANA EL JUGADOR 1";
                   LCD_Print(ganador1, 35, 60, 2, 0x0000, 0xffff);

                }
                if (contadorgol1 == 2){
                  String gol1 = "2";
                  String jugador1 = gol1;
                   LCD_Print(jugador1, 35, 20, 2, 0x0000, 0xffff);
                  
                }
                if (contadorgol1 == 1){
                  String gol1 = "1";
                  String jugador1 = gol1;
                   LCD_Print(jugador1, 35, 20, 2, 0x0000, 0xffff);
                  
                }
                
                
              
              DIRECCION =0;
          }
            if (POSXP == 3){

              contadorgol2=contadorgol2+1;
              String gool = "Gooool!!";
              LCD_Print(gool, 100, 60, 2, 0x0000, 0xffff);    //0x421b x35
              POSXP = 150;
              POSYP = 90;
              LCD_Sprite(POSXP,POSYP,25,25, pelota,7,1,0,0);
              POSYP = POSYP+CAIDA;
              LCD_Sprite(POSXP,POSYP,25,25, pelota,7,1,0,0);

              if (POSYP > 140){
                  FillRect(100, 90, 100, 100, 0x0042);  
                }

              if (contadorgol2 == 0){
                  String gol2 = "0";
                  String jugador2 = gol2;
                   LCD_Print(jugador2, 270, 20, 2, 0x0000, 0xffff);
                }
                if (contadorgol2 == 3){
                String gol2 = "3";
                  String jugador2 = gol2;
                   LCD_Print(jugador2, 270, 20, 2, 0x0000, 0xffff);
                   LCD_Bitmap(0, 0, 320, 240, cancha);  
                   String ganador2 = "GANA EL JUGADOR 2";
                   LCD_Print(ganador2, 35, 60, 2, 0x0000, 0xffff);

                }
                if (contadorgol2 == 2){
                  String gol2 = "2";
                  String jugador2 = gol2;
                   LCD_Print(jugador2, 270, 20, 2, 0x0000, 0xffff);
                  
                }
                if (contadorgol2 == 1){
                  String gol2 = "1";
                  String jugador2 = gol2;
                   LCD_Print(jugador2, 270, 20, 2, 0x0000, 0xffff);
                  
                }
              
              DIRECCION =0;
          }

      //pelota
      if(DIRECCION == 0){
      if (POSYP <= 192){
                    
                    POSYP = POSYP + CAIDA;
                    LINEAHP = POSYP -1;
                    LCD_Sprite(POSXP,POSYP,25,25, pelota,7,1,0,0);
                    H_line( POSXP, LINEAHP, 25, 0x0042);
                    H_line( POSXP, LINEAHP-1, 25, 0x0042);
                    H_line( POSXP, LINEAHP-2, 25, 0x0042);


                    if ((POSX1 == POSXP && POSY1 == POSYP+24) || (POSX2 == POSXP && POSY2 == POSYP+24)){//||(POSX1 == POSXP+3 && POSY1 == POSYP+24) || (POSX1 == POSXP+6 && POSY1 == POSYP+24) || (POSX1 == POSXP+9 && POSY1 == POSYP+24) || (POSX1 == POSXP+12 && POSY1 == POSYP+24) || (POSX1 == POSXP+15 && POSY1 == POSYP+24) ||(POSX1 == POSXP+18 && POSY1 == POSYP+24) || (POSX1 == POSXP+21 && POSY1 == POSYP+24) ||(POSX1 == POSXP+24 && POSY1 == POSYP+24) ){
                      CAIDA= 0;
                      
                      }
                        
                    
                    if((POSX1 != POSXP && POSY1+24!= POSYP) && (POSX2!= POSXP && POSY2+24!= POSYP)){
                        CAIDA = 3;
                      }

      }
      }
      
      if ( POSX1+24 == POSXP && POSY1 == POSYP){

          DIRECCION = 1;
          }
       if ( POSX2-24 == POSXP && POSY2 == POSYP){

          DIRECCION = 2;
          }
                      //movimiento a la derecha == 1
                      //movimiento a la izquierda == 2
                      
        if (DIRECCION == 1){
                 LINEAVP=POSXP-1;
                 
                 
                POSXP = POSXP+3;
                LCD_Sprite(POSXP,POSYP,25,25, pelota,7,1,0,0);
                V_line( LINEAVP+1, POSYP, 25, 0x0042);
                V_line( LINEAVP, POSYP, 25, 0x0042);
                V_line( LINEAVP-1, POSYP, 25, 0x0042);

                if ((POSX2-24 == POSXP && POSY2 == POSYP) || POSX1-24 == POSXP  && POSY1 == POSYP){
                  DIRECCION =2;
                  }
                
        }

        if (DIRECCION == 2){
            LINEAVP=POSXP+24;
            POSXP = POSXP-3;
            LCD_Sprite(POSXP,POSYP,25,25, pelota,7,1,0,0);
            V_line( LINEAVP+1, POSYP, 25, 0x0042);
            V_line( LINEAVP+2, POSYP, 25, 0x0042);
            V_line( LINEAVP+3, POSYP, 25, 0x0042);

                  if ((POSX2+24 == POSXP && POSY2 == POSYP) || POSX1+24 == POSXP  && POSY1 == POSYP){
                    DIRECCION =1;
                  }
            
          
          }
                    



       

     // && (  POSY1 != POSY2 && POSX1+24!=POSX2  )
      
    


    
    
    
   
      
    
        
    
      
       

      
      
     /* int animP = (y/15)%7;
      LCD_Sprite(150,y,25,25,pelota,7,animP,0,0);*/
      
    /*if (y== 198){
    
      }*/
    
  
  /*if (SALTO1==0){
    //int P = 0;
    //delay(15);
    //int animP = (P++/15)%7;
    LCD_Sprite(150,150,25,25,pelota,7,1,0,0);
    }*/
    
    
    



  /*for(int x = 0; x <320-32; x++){
    delay(15);
    int anim2 = (x/35)%2;
    
    LCD_Sprite(x,100,23,27,movimientoSS5,2,anim2,0,1);
    V_line( x -1, 100, 27, 0x0042);}*/
    /*
    //LCD_Bitmap(x, 100, 32, 32, prueba);
    
    int anim = (x/11)%8;
    

    int anim3 = (x/11)%4;
    
    LCD_Sprite(x, 20, 16, 32, planta,8, anim,1, 0);
    V_line( x -1, 20, 32, 0x421b);
 
    //LCD_Sprite(x,100,32,32,bowser,4,anim3,0,1);
    //V_line( x -1, 100, 32, 0x421b);
 
 
    LCD_Sprite(x, 140, 16, 24, planta,2, anim2,1, 0);
    V_line( x -1, 140, 16, 0x421b);
  
    //LCD_Sprite(x, 175, 16, 32, movimientoSS5,8, anim,1, 0);
    //V_line( x -1, 175, 32, 0x421b);
  }
  for(int x = 320-32; x >0; x--){
    delay(5);
    int anim = (x/11)%8;
    int anim2 = (x/11)%2;
    
    LCD_Sprite(x,100,16,24,planta,2,anim2,0,0);
    V_line( x + 16, 100, 24, 0x421b);
    
    //LCD_Bitmap(x, 100, 32, 32, prueba);
    
    //LCD_Sprite(x, 140, 16, 16, enemy,2, anim2,0, 0);
    //V_line( x + 16, 140, 16, 0x421b);
    
    //LCD_Sprite(x, 175, 16, 32, luigi,8, anim,0, 0);
    //V_line( x + 16, 175, 32, 0x421b);

    //LCD_Sprite(x, 20, 16, 32, mario,8, anim,0, 0);
    //V_line( x + 16, 20, 32, 0x421b);
  } 
*/
}
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
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
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
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
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
/*void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
*/

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
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
      /*if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }*/
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
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}
