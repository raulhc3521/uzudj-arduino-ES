/*Librería Encoder:
 *http://www.pjrc.com/teensy/td_libs_Encoder.html
 * 
 *Librería FastLED:
 *http://fastled.io
 */
#include <FastLED.h>
#include <Encoder.h>
#include <HID-Project.h>
#include <HID-Settings.h>
#include <EEPROM.h>

/*Definición de los pines de los encoder
 * 
 *Estos pines estan pensados para funcionar con un ATmega32U4 (utilizados en Arduino Leonardo y micro) 
 *para utilizar otro microcontrolador es necesario que posea pines que trabajen con interrupciones y 
 *que sea la cantidad necesaria para el numero de encoder utilizados (2 pines por encoder)
 */
Encoder knobLeft(3, 2);         //pines para el encoder derecho
Encoder knobRight(0, 1);        //pines para el encoder izquierdo



/*Definir color de las luces de los encoder
 * Los colores se definen bajo el modelo de color HSV (Hue, Saturation y Value o en español Tono, Saturación y valor)
 * en H (Hue) definimos el tono del color deseado distribuido en valores de entre 0-255 donde 0 es rojo y al aumentar el valor cambiará de todo hasta volver al rojo en 255
 * en S (saturation) definimos la saturación deseada entre 0-255, donde 0 resulta en un blanco absoluto y 255 en el color absoluto definido en H
 * en V (Value) definimos la intensidad del color entre 0-255, donde 0 resulta en el LED apagado y 255 en el LED con maximo brillo (el brillo a su vez depende del valor de brollo definido a continuación
 */

////////// Configuración de color de los LED //////////

byte H1 = EEPROM.read(0);             //CHSV Valor de H color 1 - pink (valor entre 0-255)
byte H2 = EEPROM.read(1);             //CHSV Valor de H color 2 - cian (valor entre 0-255)
byte BrilloLED = EEPROM.read(2);      //Brillo de los LEDs, 0 es el LED apagado y 255 es el maximo de brillo
byte Vel = 5;                        //Velocidad de movimiento de los LED, tiene que ser divisor de 120 y 240 (ej: 1, 2, 3, 4, 5, 6, 8, 10, 12, 15, 20, 30, ...)

////////// FIN de la Configuración de color de los LED //////////


/*Definición cantidad de LEDs por encoder*/
const byte NUM_LEDS = 20;       //Cantidad de LEDs, en este caso son 2 encoder y cada uno con 10 LED, dando un todal de 20
const byte DATA_PIN = 7;        //Pin de datos para los LEDs (es importante que este pin funcione con interrupciones, de no ser así, puede producir algún fallo
byte IniLedEnc_1 = 0;           //Definir desde que LED y hasta que LED corresponden a cada encoder
byte FinLedEnc_1 = 9;
byte IniLedEnc_2 = 10;
byte FinLedEnc_2 = 19;

/*Efecto de doble destello de luz con movimiento
 * 
 *al modificar esto, modificaras el funcionamiento de la iluminación
 */
byte _LED1 = 4;           //LED previo color 1
byte LED1 = 5;            //LED central color 1
byte LED1_ = 6;           //LED posterior color 1
byte _LED2 = 9;           //LED previo color 2
byte LED2 = 0;            //LED central color 2
byte LED2_ = 1;           //LED posterior color 2
byte _LED3 = 14;          //LED previo color 1
byte LED3 = 15;           //LED central color 1
byte LED3_ = 16;          //LED posterior color 1
byte _LED4 = 19;          //LED previo color 2
byte LED4 = 10;           //LED central color 2
byte LED4_ = 11;          //LED posterior color 2
byte _V1 = 120;           //CHSV Valor de V anterior color 1
byte V1 = 240;            //CHSV Valor de V central color 1
byte V1_ = 120;           //CHSV Valor de V posterior color 1
byte _V2 = 120;           //CHSV Valor de V anterior color 2
byte V2 = 240;            //CHSV Valor de V central color 2
byte V2_ = 120;           //CHSV Valor de V posterior color 2
byte _V3 = 120;           //CHSV Valor de V anterior color 1
byte V3 = 240;            //CHSV Valor de V central color 1
byte V3_ = 120;           //CHSV Valor de V posterior color 1
byte _V4 = 120;           //CHSV Valor de V anterior color 1
byte V4 = 240;            //CHSV Valor de V central color 1
byte V4_ = 120;           //CHSV Valor de V posterior color 1
byte S1 = 255;            //CHSV Valor de S color 1
byte S2 = 255;            //CHSV Valor de S color 2
byte MaxV = 240;          //CHSV valor maximo de V
byte MinV = 0;            //CHSV valor minimo de V
CRGB leds[NUM_LEDS];
/* Fin del Efecto */

////////// Mapeo de los botones para modo keyboard //////////
/* Para saber que numero decimal corresponde a el caractes ASCII que necesitas, vitita la siguiente página:
 * http://www.asciitable.com
 */
 
char MapKey1 = 97;             //  97 = a
char MapKey2 = 119;            // 119 = w
char MapKey3 = 100;            // 100 = d
char MapKey4 = 32;             //  32 = space
char MapKey5 = 106;            // 228 = j
char MapKey6 = 105;            // 232 = i
char MapKey7 = 108;            // 230 = l
char MapKey8 = 120;            //  120 = x
char MapKey9 = 99;             //  99 = c
char MapKey10 = 110;           //  110 = n
char MapKey11 = 109;           //  109 = m
char MapKeyST = 10;            //  13 = enter

char MapEncLeft = 51;          //51 = Numero 3
char MapEncRight = 55;         //55 = Numero 7

////////// Mapeo de los botones FN //////////
char MapKeyFN_ST = 132;             //  132 = escape
char MapKeyFN_2 = 49;               //  49 = 1
char MapKeyFN_6 = 50;               //  50 = 2
//char MapKeyFN_1 = 129;            //  KEY_LEFT_SHIFT
//char MapKeyFN_3 = 201;            //  KEY_F8
//char MapKeyFN_4 = 179;            //  KEY_TAB
//char MapKeyFN_5 = 203;            //  KEY_F9
//char MapKeyFN_7 = 133;            //  KEY_RIGHT_SHIFT
//char MapKeyFN_8 = 132;            //  132 = escape
//char MapKeyFN_9 = 132;            //  132 = escape
//char MapKeyFN_10 = 132;           //  132 = escape
//char MapKeyFN_11 = 132;           //  132 = escape
////////// Fin del Mapeo de los botones FN //////////

////////// Fin del Mapeo de los botones para modo keyboard //////////


////////// Definición de pines para los botones //////////
byte Key1Pin = 4;             //Configurado el boton 1 en el pin 4
byte Key2Pin = 5;             //Configurado el boton 2 en el pin 5
byte Key3Pin = 6;             //Configurado el boton 3 en el pin 6
byte Key4Pin = 8;             //Configurado el boton 4 en el pin A3
byte Key5Pin = 9;             //Configurado el boton 5 en el pin A2
byte Key6Pin = 10;            //Configurado el boton 6 en el pin A1
byte Key7Pin = 16;            //Configurado el boton 7 en el pin A0
byte Key8Pin = A0;            //Configurado el boton 8 en el pin 10
byte Key9Pin = A1;            //Configurado el boton 9 en el pin 16
byte Key10Pin = A2;           //Configurado el boton 10 en el pin 14
byte Key11Pin = A3;           //Configurado el boton 11 en el pin 15
byte KeySTPin = 14;           //Configurado el boton Start en el pin 8
byte KeyFNPin = 15;           //Configurado el boton Function en el pin 9
////////// Fin de la Definición de pines para los botones //////////


////////// debounce de botones //////////
unsigned int Debounce = 20;               //Debounce de 20ms
unsigned int TurntableDelay = 100;        //Debounce de 20ms

unsigned long Key1Time;                   //Momento en el que se presionó el boton
unsigned long Key2Time;
unsigned long Key3Time;
unsigned long Key4Time;
unsigned long Key5Time;
unsigned long Key6Time;
unsigned long Key7Time;
unsigned long Key8Time;
unsigned long Key9Time;
unsigned long Key10Time;
unsigned long Key11Time;
unsigned long KeySTTime;
unsigned long KeyFNTime;
unsigned long TurntableUpTime;
unsigned long TurntableDownTime;
unsigned long TurntableLeftTime;
unsigned long TurntableRightTime;

boolean Key1State = false;                //Estado de los botones
boolean Key2State = false;
boolean Key3State = false;
boolean Key4State = false;
boolean Key5State = false;
boolean Key6State = false;
boolean Key7State = false;
boolean Key8State = false;
boolean Key9State = false;
boolean Key10State = false;
boolean Key11State = false;
boolean KeySTState = false;
boolean KeyFNState = false;
boolean TurntableUpState = false;
boolean TurntableDownState = false;
boolean TurntableLeftState = false;
boolean TurntableRightState = false;

boolean HoldTurntableUp = false;
boolean HoldTurntableDown = false;
boolean HoldTurntableLeft = false;
boolean HoldTurntableRight = false;

/* Fin del debounce de botones */

void setup() {
  pinMode(Key1Pin, INPUT_PULLUP);
  pinMode(Key2Pin, INPUT_PULLUP);
  pinMode(Key3Pin, INPUT_PULLUP);
  pinMode(Key4Pin, INPUT_PULLUP);
  pinMode(Key5Pin, INPUT_PULLUP);
  pinMode(Key6Pin, INPUT_PULLUP);
  pinMode(Key7Pin, INPUT_PULLUP);
  pinMode(Key8Pin, INPUT_PULLUP);
  pinMode(Key9Pin, INPUT_PULLUP);
  pinMode(Key10Pin, INPUT_PULLUP);
  pinMode(Key11Pin, INPUT_PULLUP);
  pinMode(KeySTPin, INPUT_PULLUP);
  pinMode(KeyFNPin, INPUT_PULLUP);
  
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BrilloLED);

  NKROKeyboard.begin();
  
}

long positionLeft  = -999;
long positionRight = -999;

void loop() {

  /////////////////////////////// Botones ///////////////////////////////

  /// Boton FN ///
  int HoldKeyFN = digitalRead(KeyFNPin);
  if ((HoldKeyFN == LOW) && (KeyFNState == false) && (KeyFNTime + Debounce <= millis())) {
    KeyFNState = true;
    KeyFNTime = millis();
  }
  if ((HoldKeyFN == HIGH) && (KeyFNState == true) && (KeyFNTime + Debounce <= millis())) {
    KeyFNState = false;
    KeyFNTime = millis();
    EEPROM.update(0, H1);
    EEPROM.update(1, H2); 
    EEPROM.update(2, BrilloLED); 
  }  
  /// Fin de Boton FN ///

  
  /// Boton 1 ///
  int HoldKey1 = digitalRead(Key1Pin);
  if ((HoldKey1 == LOW) && (Key1State == false) && (Key1Time + Debounce <= millis())) {
    if(KeyFNState == true){
      NKROKeyboard.press(KEY_LEFT_SHIFT);
      Key1State = true;
      Key1Time = millis();
    }
    else{
      NKROKeyboard.press(MapKey1);
      Key1State = true;
      Key1Time = millis();
    }
  }
  if ((HoldKey1 == HIGH) && (Key1State == true) && (Key1Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey1);
    NKROKeyboard.release(KEY_LEFT_SHIFT);
    Key1State = false;
    Key1Time = millis();
  }  
  /// Fin de Boton 1 ///


  /// Boton 2 ///
  int HoldKey2 = digitalRead(Key2Pin);
  if ((HoldKey2 == LOW) && (Key2State == false) && (Key2Time + Debounce <= millis())) {
    if(KeyFNState == true){
      NKROKeyboard.press(MapKeyFN_2);
      Key2State = true;
      Key2Time = millis();
    }
    else{
      NKROKeyboard.press(MapKey2);
      Key2State = true;
      Key2Time = millis();
    }    
  }
  if ((HoldKey2 == HIGH) && (Key2State == true) && (Key2Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey2);
    NKROKeyboard.release(MapKeyFN_2);
    Key2State = false;
    Key2Time = millis();
  }  
  /// Fin de Boton 2 ///


  /// Boton 3 ///
  int HoldKey3 = digitalRead(Key3Pin);
  if ((HoldKey3 == LOW) && (Key3State == false) && (Key3Time + Debounce <= millis())) {
    if(KeyFNState == true){
      NKROKeyboard.press(KEY_F8);
      Key3State = true;
      Key3Time = millis();
    }
    else{
      NKROKeyboard.press(MapKey3);
      Key3State = true;
      Key3Time = millis();
    }
  }
  if ((HoldKey3 == HIGH) && (Key3State == true) && (Key3Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey3);
    NKROKeyboard.release(KEY_F8);
    Key3State = false;
    Key3Time = millis();
  }  
  /// Fin de Boton 3 ///


  /// Boton 4 ///
  int HoldKey4 = digitalRead(Key4Pin);
  if ((HoldKey4 == LOW) && (Key4State == false) && (Key4Time + Debounce <= millis())) {
    if(KeyFNState == true){
      NKROKeyboard.press(KEY_TAB);
      Key4State = true;
      Key4Time = millis();
    }
    else{
      NKROKeyboard.press(MapKey4);
      Key4State = true;
      Key4Time = millis();
    }
  }
  if ((HoldKey4 == HIGH) && (Key4State == true) && (Key4Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey4);
    NKROKeyboard.release(KEY_TAB);
    Key4State = false;
    Key4Time = millis();
  }  
  /// Fin de Boton 4 ///


  /// Boton 5 ///
  int HoldKey5 = digitalRead(Key5Pin);
  if ((HoldKey5 == LOW) && (Key5State == false) && (Key5Time + Debounce <= millis())) {
    if(KeyFNState == true){
      NKROKeyboard.press(KEY_F9);
      Key5State = true;
      Key5Time = millis();
    }
    else{
      NKROKeyboard.press(MapKey5);
      Key5State = true;
      Key5Time = millis();
    }
  }
  if ((HoldKey5 == HIGH) && (Key5State == true) && (Key5Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey5);
    NKROKeyboard.release(KEY_F9);
    Key5State = false;
    Key5Time = millis();
  }  
  /// Fin de Boton 5 ///


  /// Boton 6 ///
  int HoldKey6 = digitalRead(Key6Pin);
  if ((HoldKey6 == LOW) && (Key6State == false) && (Key6Time + Debounce <= millis())) {
    if(KeyFNState == true){
      NKROKeyboard.press(MapKeyFN_6);
      Key6State = true;
      Key6Time = millis();
    }
    else{
      NKROKeyboard.press(MapKey6);
      Key6State = true;
      Key6Time = millis();
    }
  }
  if ((HoldKey6 == HIGH) && (Key6State == true) && (Key6Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey6);
    NKROKeyboard.release(MapKeyFN_6);
    Key6State = false;
    Key6Time = millis();
  }  
  /// Fin de Boton 6 ///


  /// Boton 7 ///
  int HoldKey7 = digitalRead(Key7Pin);
  if ((HoldKey7 == LOW) && (Key7State == false) && (Key7Time + Debounce <= millis())) {
    if(KeyFNState == true){
      NKROKeyboard.press(KEY_RIGHT_SHIFT);
      Key7State = true;
      Key7Time = millis();
    }
    else{
      NKROKeyboard.press(MapKey7);
      Key7State = true;
      Key7Time = millis();
    }
  }
  if ((HoldKey7 == HIGH) && (Key7State == true) && (Key7Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey7);
    NKROKeyboard.release(KEY_RIGHT_SHIFT);
    Key7State = false;
    Key7Time = millis();
  }  
  /// Fin de Boton 7 ///


  /// Boton 8 ///
  int HoldKey8 = digitalRead(Key8Pin);
  if ((HoldKey8 == LOW) && (Key8State == false) && (Key8Time + Debounce <= millis())) {
    if(KeyFNState == true){
      H1 = H1 + 5;
      if(H1 >= 255){
        H1 = 0;    
      }
      Key8State = true;
      Key8Time = millis();
      leds[LED1] = CHSV(H1,S1,V1);          //Enviar el color de los led
      leds[LED1_] = CHSV(H1,S1,V1_);
      leds[_LED1] = CHSV(H1,S1,_V1);      
      leds[LED3] = CHSV(H1,S1,V3);          //Enviar el color de los led
      leds[LED3_] = CHSV(H1,S1,V3_);
      leds[_LED3] = CHSV(H1,S1,_V3);
      FastLED.show(); 
    }
    else{
      NKROKeyboard.press(MapKey8);
      Key8State = true;
      Key8Time = millis();
    }
  }
  if ((HoldKey8 == HIGH) && (Key8State == true) && (Key8Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey8);
    Key8State = false;
    Key8Time = millis();
  }  
  /// Fin de Boton 8 ///


  /// Boton 9 ///
  int HoldKey9 = digitalRead(Key9Pin);
  if ((HoldKey9 == LOW) && (Key9State == false) && (Key9Time + Debounce <= millis())) {
    if(KeyFNState == true){
      H2 = H2 + 5;
      if(H2 >= 255){
        H2 = 0;      
      }      
      Key9State = true;
      Key9Time = millis();
      leds[LED2] = CHSV(H2,S2,V2);
      leds[LED2_] = CHSV(H2,S2,V2_);
      leds[_LED2] = CHSV(H2,S2,_V2);       
      leds[LED4] = CHSV(H2,S2,V4);
      leds[LED4_] = CHSV(H2,S2,V4_);
      leds[_LED4] = CHSV(H2,S2,_V4);
      FastLED.show(); 
    }
    else{
      NKROKeyboard.press(MapKey9);
      Key9State = true;
      Key9Time = millis();
    }
  }
  if ((HoldKey9 == HIGH) && (Key9State == true) && (Key9Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey9);
    Key9State = false;
    Key9Time = millis();
  }  
  /// Fin de Boton 9 ///


  /// Boton 10 ///
  int HoldKey10 = digitalRead(Key10Pin);
  if ((HoldKey10 == LOW) && (Key10State == false) && (Key10Time + Debounce <= millis())) {
    if(KeyFNState == true){
      if(BrilloLED > 9){
        BrilloLED = BrilloLED - 10;      
      }   
      Key10State = true;
      Key10Time = millis();
      FastLED.setBrightness(BrilloLED);
      FastLED.show(); 
    }
    else{
      NKROKeyboard.press(MapKey10);
      Key10State = true;
      Key10Time = millis();
    }
  }
  if ((HoldKey10 == HIGH) && (Key10State == true) && (Key10Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey10);
    Key10State = false;
    Key10Time = millis();
  }  
  /// Fin de Boton 10 ///


  /// Boton 11 ///
  int HoldKey11 = digitalRead(Key11Pin);
  if ((HoldKey11 == LOW) && (Key11State == false) && (Key11Time + Debounce <= millis())) {
    if(KeyFNState == true){
      if(BrilloLED < 245){
        BrilloLED = BrilloLED + 10;      
      }   
      Key11State = true;
      Key11Time = millis();
      FastLED.setBrightness(BrilloLED);
      FastLED.show(); 
    }
    else{
      NKROKeyboard.press(MapKey11);
      Key11State = true;
      Key11Time = millis();
    }
  }
  if ((HoldKey11 == HIGH) && (Key11State == true) && (Key11Time + Debounce <= millis())) {
    NKROKeyboard.release(MapKey11);
    Key11State = false;
    Key11Time = millis();
  }  
  /// Fin de Boton 11 ///


  /// Boton ST ///
  int HoldKeyST = digitalRead(KeySTPin);
  if ((HoldKeyST == LOW) && (KeySTState == false) && (KeySTTime + Debounce <= millis())) {
    if(KeyFNState == true){
      NKROKeyboard.press(MapKeyFN_ST);
      KeySTState = true;
      KeySTTime = millis();
    }
    else{
      NKROKeyboard.press(MapKeyST);
      KeySTState = true;
      KeySTTime = millis();
    }
  }
  if ((HoldKeyST == HIGH) && (KeySTState == true) && (KeySTTime + Debounce <= millis())) {
    NKROKeyboard.release(MapKeyST);
    NKROKeyboard.release(MapKeyFN_ST);
    KeySTState = false;
    KeySTTime = millis();
  }  
  /// Fin de Boton ST ///
  
  /////////////////////////////// Fin de Botones ///////////////////////////////

  
  /////////////////////////////// Tornamesa ///////////////////////////////

  /// Tornamesa UP ///
  if ((HoldTurntableUp == true) && (TurntableUpState == false) && (TurntableUpTime + Debounce <= millis())) {
    NKROKeyboard.release(KEY_DOWN_ARROW);
    NKROKeyboard.release(MapEncLeft);
    NKROKeyboard.press(MapEncLeft);
    NKROKeyboard.press(KEY_UP_ARROW);
    TurntableUpState = true;
    TurntableUpTime = millis();
  }
  if ((HoldTurntableUp == false) && (TurntableUpState == true) && (TurntableUpTime + TurntableDelay <= millis())) {
    if(HoldTurntableDown == false){
      NKROKeyboard.release(MapEncLeft); 
    }
    NKROKeyboard.release(KEY_UP_ARROW);
    TurntableUpState = false;
    TurntableUpTime = millis();
  }  
  /// Fin de Tornamesa UP ///



  /// Tornamesa DOWN ///
  if ((HoldTurntableDown == true) && (TurntableDownState == false) && (TurntableDownTime + Debounce <= millis())) {
    NKROKeyboard.release(KEY_UP_ARROW);
    NKROKeyboard.release(MapEncLeft);
    NKROKeyboard.press(MapEncLeft);
    NKROKeyboard.press(KEY_DOWN_ARROW);
    TurntableDownState = true;
    TurntableDownTime = millis();
  }
  if ((HoldTurntableDown == false) && (TurntableDownState == true) && (TurntableDownTime + TurntableDelay <= millis())) {
    if(HoldTurntableUp == false){
      NKROKeyboard.release(MapEncLeft); 
    } 
    NKROKeyboard.release(KEY_DOWN_ARROW);
    TurntableDownState = false;
    TurntableDownTime = millis();
  }  
  /// Fin de Tornamesa DOWN ///



  /// Tornamesa LEFT ///
  if ((HoldTurntableLeft == true) && (TurntableLeftState == false) && (TurntableLeftTime + Debounce <= millis())) {
    NKROKeyboard.release(MapEncRight);
    NKROKeyboard.release(KEY_RIGHT_ARROW);
    NKROKeyboard.press(MapEncRight);
    NKROKeyboard.press(KEY_LEFT_ARROW);
    TurntableLeftState = true;
    TurntableLeftTime = millis();
  }
  if ((HoldTurntableLeft == false) && (TurntableLeftState == true) && (TurntableLeftTime + TurntableDelay <= millis())) {
    if(HoldTurntableRight == false){
      NKROKeyboard.release(MapEncRight); 
    }
    NKROKeyboard.release(KEY_LEFT_ARROW);
    TurntableLeftState = false;
    TurntableLeftTime = millis();
  }  
  /// Fin de Tornamesa LEFT ///


  /// Tornamesa RIGHT ///
  if ((HoldTurntableRight == true) && (TurntableRightState == false) && (TurntableRightTime + Debounce <= millis())) {
    NKROKeyboard.release(MapEncRight);
    NKROKeyboard.release(KEY_LEFT_ARROW);
    NKROKeyboard.press(MapEncRight);
    NKROKeyboard.press(KEY_RIGHT_ARROW);
    TurntableRightState = true;
    TurntableRightTime = millis();
  }
  if ((HoldTurntableRight == false) && (TurntableRightState == true) && (TurntableRightTime + TurntableDelay <= millis())) {
    if(HoldTurntableLeft == false){
      NKROKeyboard.release(MapEncRight); 
    } 
    NKROKeyboard.release(KEY_RIGHT_ARROW);
    TurntableRightState = false;
    TurntableRightTime = millis();
  }  
  /// Fin de Tornamesa RIGHT ///

  /////////////////////////////// Fin de la Tornamesa ///////////////////////////////

  

  /////////////////////////////// Iluminación Encoder + LEDs ///////////////////////////////

  long newLeft, newRight;
  newLeft = knobLeft.read();
  newRight = knobRight.read();

  if ((newLeft == positionLeft) && (TurntableUpTime + TurntableDelay <= millis())){
    HoldTurntableUp = false;
    TurntableUpTime = millis();
  }

  if ((newLeft == positionLeft) && (TurntableDownTime + TurntableDelay <= millis())){
    HoldTurntableDown = false;
    TurntableDownTime = millis();
  }

  if ((newRight == positionRight) && (TurntableLeftTime + TurntableDelay <= millis())){
    HoldTurntableLeft = false;
    TurntableLeftTime = millis();
  }

  if ((newRight == positionRight) && (TurntableRightTime + TurntableDelay <= millis())){
    HoldTurntableRight = false;
    TurntableRightTime = millis();
  }

  

  if (newLeft < positionLeft) {    

    HoldTurntableUp = true;
    HoldTurntableDown = false;
    
    if(V1_ >= MaxV){                      //Si el Valor en CHSV del color 1 es mayor o igual que el maximo
      LED1_ ++;                         //Cambiar los LED del color 1 al siguiente LED
      LED1 ++;
      _LED1 ++;
      _V1 = MaxV;                       //Valor CHSV del color 1 por defecto al comenzar con el siguiente LED
      V1 = MaxV;
      V1_ = MinV;
      if(LED1 >= IniLedEnc_2){          //cuando se llegue al limite de LEDs volver al LED 0
        LED1 = IniLedEnc_1;
      }
      if(LED1_ >= IniLedEnc_2){
        LED1_ = IniLedEnc_1;
      }
      if(_LED1 >= IniLedEnc_2){
        _LED1 = IniLedEnc_1;
      } 
    }
    
    if(V2_ >= MaxV){                    //Si el Valor en CHSV del color 2 es mayor o igual que el maximo
      LED2_ ++;                         //Cambiar los LED del color 2 al siguiente LED
      LED2 ++;
      _LED2 ++;
      _V2 = MaxV;                       //Valor CHSV del color 2 por defecto al comenzar con el siguiente LED
      V2 = MaxV;
      V2_ = MinV;
      if(LED2 >= IniLedEnc_2){          //cuando se llegue al limite de LEDs volver al LED 0
        LED2 = IniLedEnc_1;
      }
      if(LED2_ >= IniLedEnc_2){
        LED2_ = IniLedEnc_1;
      }      
      if(_LED2 >= IniLedEnc_2){
        _LED2 = IniLedEnc_1;
      } 
    }
      
    _V1 = _V1 - Vel;                    //Cambiar el Valor CHSV de los colores segun velosidad
    V1_ = V1_ + Vel;    
    _V2 = _V2 - Vel;
    V2_ = V2_ + Vel;
  }
  /*
  
  */
  if (newLeft > positionLeft) { 

    HoldTurntableUp = false;
    HoldTurntableDown = true;

    if(_V1 >= MaxV){                      //Si el Valor en CHSV del color 1 es mayor o igual que el maximo
        if(LED1 <= IniLedEnc_1){          //cuando se llegue al limite de LEDs volver al LED 0
          LED1 = 10;
        }
        if(LED1_ <= IniLedEnc_1){
          LED1_ = IniLedEnc_2;
        }
        if(_LED1 <= IniLedEnc_1){
          _LED1 = IniLedEnc_2;
        }        
        LED1_ --;                         //Cambiar los LED del color 1 al siguiente LED
        LED1 --;
        _LED1 --;
        _V1 = MinV;                       //Valor CHSV del color 1 por defecto al comenzar con el siguiente LED
        V1 = MaxV;
        V1_ = MaxV; 
      }
    
      if(_V2 >= MaxV){                    //Si el Valor en CHSV del color 2 es mayor o igual que el maximo           
        if(LED2 <= IniLedEnc_1){          //cuando se llegue al limite de LEDs volver al LED 0
          LED2 = IniLedEnc_2;
        }
        if(LED2_ <= IniLedEnc_1){
          LED2_ = IniLedEnc_2;
        }      
        if(_LED2 <= IniLedEnc_1){
          _LED2 = IniLedEnc_2;
        }            
        LED2_ --;                         //Cambiar los LED del color 2 al siguiente LED
        LED2 --;
        _LED2 --;
        _V2 = MinV;                       //Valor CHSV del color 2 por defecto al comenzar con el siguiente LED
        V2 = MaxV;
        V2_ = MaxV;
      }
      
      _V1 = _V1 + Vel;                    //Cambiar el Valor CHSV de los colores segun velosidad
      V1_ = V1_ - Vel;    
      _V2 = _V2 + Vel;
      V2_ = V2_ - Vel;
  }


  if (newRight < positionRight) {

    HoldTurntableLeft = true;
    HoldTurntableRight = false;
    
    if(V3_ >= MaxV){                      //Si el Valor en CHSV del color 1 es mayor o igual que el maximo    
        LED3_ ++;                         //Cambiar los LED del color 1 al siguiente LED
        LED3 ++;
        _LED3 ++;        
        _V3 = MaxV;                       //Valor CHSV del color 1 por defecto al comenzar con el siguiente LED
        V3 = MaxV;
        V3_ = MinV;    
        if(LED3 >= FinLedEnc_2 + 1){      //cuando se llegue al limite de LEDs volver al LED 0
          LED3 = IniLedEnc_2;
        }        
        if(LED3_ >= FinLedEnc_2 + 1){
          LED3_ = IniLedEnc_2;
        }        
        if(_LED3 >= FinLedEnc_2 + 1){
          _LED3 = IniLedEnc_2;
        } 
      }

      if(V4_ >= MaxV){                    //Si el Valor en CHSV del color 2 es mayor o igual que el maximo      
        LED4_ ++;                         //Cambiar los LED del color 2 al siguiente LED
        LED4 ++;
        _LED4 ++;        
        _V4 = MaxV;                       //Valor CHSV del color 2 por defecto al comenzar con el siguiente LED
        V4 = MaxV;
        V4_ = MinV;    
        if(LED4 >= FinLedEnc_2 + 1){      //cuando se llegue al limite de LEDs volver al LED 0
          LED4 = IniLedEnc_2;
        }        
        if(LED4_ >= FinLedEnc_2 + 1){
          LED4_ = IniLedEnc_2;
        }        
        if(_LED4 >= FinLedEnc_2 + 1){
          _LED4 = IniLedEnc_2;
        } 
      }

      _V3 = _V3 - Vel;                    //Cambiar el Valor CHSV de los colores segun velosidad
      V3_ = V3_ + Vel;  
      _V4 = _V4 - Vel;
      V4_ = V4_ + Vel;
  }
  
  if (newRight > positionRight) {

    HoldTurntableRight = true;
    HoldTurntableLeft = false;
    
    if(_V3 >= MaxV){                      //Si el Valor en CHSV del color 1 es mayor o igual que el maximo
        LED3_ --;                         //Cambiar los LED del color 1 al siguiente LED
        LED3 --;
        _LED3 --;    
        if(LED3 <= FinLedEnc_1){          //cuando se llegue al limite de LEDs volver al LED 0
          LED3 = FinLedEnc_2;
        }        
        if(LED3_ <= FinLedEnc_1){
          LED3_ = FinLedEnc_2;
        }        
        if(_LED3 <= FinLedEnc_1){
          _LED3 = FinLedEnc_2;
        }         
        _V3 = MinV;                       //Valor CHSV del color 1 por defecto al comenzar con el siguiente LED
        V3 = MaxV;
        V3_ = MaxV;
      }

      if(_V4 >= MaxV){                    //Si el Valor en CHSV del color 2 es mayor o igual que el maximo
        LED4_ --;                         //Cambiar los LED del color 2 al siguiente LED
        LED4 --;
        _LED4 --;      
        if(LED4 <= FinLedEnc_1){          //cuando se llegue al limite de LEDs volver al LED 0
          LED4 = FinLedEnc_2;
        }        
        if(LED4_ <= FinLedEnc_1){
          LED4_ = FinLedEnc_2;
        }        
        if(_LED4 <= FinLedEnc_1){
          _LED4 = FinLedEnc_2;
        } 
        _V4 = MinV;                       //Valor CHSV del color 2 por defecto al comenzar con el siguiente LED
        V4 = MaxV;
        V4_ = MaxV;        
      }

      _V3 = _V3 + Vel;                    //Cambiar el Valor CHSV de los colores segun velosidad
      V3_ = V3_ - Vel;  
      _V4 = _V4 + Vel;
      V4_ = V4_ - Vel;
  }
  
  
  if(newLeft != positionLeft || newRight != positionRight) {
    positionLeft = newLeft;
    positionRight = newRight;

    leds[LED1] = CHSV(H1,S1,V1);          //Enviar el color de los led
    leds[LED1_] = CHSV(H1,S1,V1_);
    leds[_LED1] = CHSV(H1,S1,_V1);
    leds[LED2] = CHSV(H2,S2,V2);
    leds[LED2_] = CHSV(H2,S2,V2_);
    leds[_LED2] = CHSV(H2,S2,_V2);
    leds[LED3] = CHSV(H1,S1,V3);          //Enviar el color de los led
    leds[LED3_] = CHSV(H1,S1,V3_);
    leds[_LED3] = CHSV(H1,S1,_V3);        
    leds[LED4] = CHSV(H2,S2,V4);
    leds[LED4_] = CHSV(H2,S2,V4_);
    leds[_LED4] = CHSV(H2,S2,_V4);
    FastLED.show();                       //Enviar datos a los LEDs
  }

  /////////////////////////////// Fin Iluminación Encoder + LEDs ///////////////////////////////
  
}
