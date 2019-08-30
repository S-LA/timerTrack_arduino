
#include <Arduino.h>
#include <Nokia_LCD.h>

// #include<testing.h> //used when unconnected from system
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||LCD|||||||||||||||

const byte lcdPinCLK = 8;
const byte lcdPinDIN = 9;
const byte lcdPinDC = 10;
const byte lcdPinCE = 11;
const byte lcdPinRST = 12;
const byte lcdPinLED = 13;
// Nokia_LCD lcd(CLK, DIN, DC, CE, RST );
Nokia_LCD lcd(lcdPinCLK, lcdPinDIN, lcdPinDC, lcdPinCE, lcdPinRST);

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||ARDUINO PINS||||
const byte input_lghtGate_A = 3;
const byte input_lghtGate_B = 7;
const byte input_lghtGate_C = 2;

const byte laserPinA = 4;
const byte laserPinB = 6;
const byte laserPinC = 5;

//||||||||||||||||||||||||||||||||||||||||||||||||||   TIME VARIABLES   ||||||||
// time delays

const int wait_twentyFive = 25;
const int wait_fifty = 50;
const int wait_hundred = 100;
const int wait_twoFifty = 250;
const int wait_halfSec = 500;
const int wait_fullSec = 1000;

const float million_flt = 1000000.00;

//||||||||||||||||||||||||||||||||||||||||||||||||||   ANALOG VARIABLES   ||||||||
//recieved from the buttons in the analog Pins

bool state_printOnceA; // used when prtinging <lcdSend_menuFrame();>
bool state_printOnceB = true;
bool state_printOnceC = true;
bool state_printOnceD = true;
int cntrl_mainMenu = 0;
int state_rsltMenu = 0; // used when turing pages in the results display
int state_trackPos = 0;
// int menuModeState = 0;
int button_mainMenu = 0;
int button_rsltMenu = 0;
int cntrl_functionMode = 0;
//||||||||||||||||||||||||||||||||||||||||||||||||||   STATE VARIABLES   ||||||||

int prnt_rsltPage = 1; //incrments when buttons are press

int buttonThresholdHigh = 768; //point at which anlg buttons will trigger
int buttonThresholdLow = 256;  //point at which anlg buttons will trigger

unsigned long time_Ai_lng; // micros when laser A goes LOW
unsigned long time_Af_lng; // micros when laser A goes HIGH AGAIN
unsigned long time_Ad_lng; // change in time between <time_Xx_lng>

unsigned long time_Bi_lng; // micros when laser B goes LOW
unsigned long time_Bf_lng; // micros when laser B goes HIGH AGAIN
unsigned long time_Bd_lng; // change in time between <time_Xx_lng>

unsigned long time_Ci_lng; // micros when laser C goes LOW
unsigned long time_Cf_lng; // micros when laser C goes HIGH AGAIN
unsigned long time_Cd_lng; // change in time between <time_Xx_lng>

unsigned long time_Di_lng; // micros when laser C goes HIGH(momentum)
unsigned long time_Df_lng; // micros when laser C goes LOW(momentum)
unsigned long time_Dd_lng; // micros change between A and B(momentum)

unsigned long time_ABi_lng; // avg of time at laser A
unsigned long time_ABf_lng; // avg of time at laser B
unsigned long time_ABd_lng; // change in time between <time_Xx_lng Laser A and B>

unsigned long time_BCi_lng; // avg of time at laser B
unsigned long time_BCf_lng; // avg of time at laser C
unsigned long time_BCd_lng; // change in time between <time_Xx_lng Laser B and C>

unsigned long time_ACi_lng; // avg of time at laser A
unsigned long time_ACf_lng; // avg of time at laser C
unsigned long time_ACd_lng; // change in time between <time_Xx_lng Laser A and C>

unsigned long time_CDi_lng; // avg of time at laser A
unsigned long time_CDf_lng; // avg of time at laser C
unsigned long time_CDd_lng; // change in time between <time_Xx_lng

unsigned long period_Ai_lng; // used when calulating <period_Xx_flt
unsigned long period_Af_lng; // used when calulating <period_Xx_flt
unsigned long period_Bi_lng; // used when calulating <period_Xx_flt
unsigned long period_Bf_lng; // used when calulating <period_Xx_flt
unsigned long period_Ci_lng; // used when calulating <period_Xx_flt
unsigned long period_Cf_lng; // used when calulating <period_Xx_flt
unsigned long period_Di_lng; // used when calulating <period_Xx_flt
unsigned long period_Df_lng; // used when calulating <period_Xx_flt

float period_Ai_flt;
float period_Af_flt;
float period_Bi_flt;
float period_Bf_flt;
float period_Ci_flt;
float period_Cf_flt;
float period_Di_flt;
float period_Df_flt;

const float dist_Trolley = 0.165;

int countTest = 0;

float period_A_glb;
float period_B_glb;
float period_C_glb;
float period_D_glb;
float period_AB_glb;
float period_BC_glb;
float period_AC_glb;
float period_CD_glb;

float time_Ai_glb;
float time_Af_glb;
float time_Bi_glb;
float time_Bf_glb;
float time_Ci_glb;
float time_Cf_glb;
float time_Di_glb;
float time_Df_glb;

float velocity_A_glb;
float velocity_B_glb;
float velocity_C_glb;
float velocity_D_glb;
float velocity_AB_glb;
float velocity_BC_glb;
float velocity_AC_glb;

float acceleration_AB_glb;
float acceleration_BC_glb;
float acceleration_AC_glb;

float dist_AB_glb;
float dist_BC_glb;
float dist_AC_glb;

int checkNum = 0;

// Serial Printing Valriables
int readingNum_time = 0;
int readingNum_period = 0;
int readingNum_velocity = 0;
int readingNum_acceleration = 0;
int readingNum = 0;

// RESET THE ARDUINO
void (*resetFunc)(void) = 0; //declare reset function at address 0
//----------------------------------------------------------------------------------
// #include <lcdGraphics.h>
const unsigned char hintEduLogo[504] PROGMEM = {
    0x00,
    0xFE,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0x22,
    0xA2,
    0xA2,
    0xA2,
    0xA2,
    0x22,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0xE2,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0xFE,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFC,
    0xFC,
    0xFC,
    0xFC,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFC,
    0xFC,
    0xFC,
    0xFC,
    0xFC,
    0x00,
    0x00,
    0x8F,
    0x8F,
    0x8F,
    0x8F,
    0x00,
    0x00,
    0xF0,
    0xF0,
    0xF0,
    0xF0,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xF0,
    0xF0,
    0xF0,
    0xF0,
    0x00,
    0x00,
    0xF0,
    0xF0,
    0xF0,
    0xFE,
    0xFE,
    0xFE,
    0xFE,
    0xF0,
    0xF0,
    0xF0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xF0,
    0xF0,
    0xF0,
    0xF0,
    0xF0,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x7F,
    0xFF,
    0xFE,
    0xFC,
    0xF8,
    0xF0,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x01,
    0x03,
    0x07,
    0x0F,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0E,
    0x0E,
    0x0E,
    0x0E,
    0x0E,
    0xCE,
    0x4E,
    0x4E,
    0xCE,
    0x0E,
    0xCE,
    0x4E,
    0x46,
    0xF6,
    0x06,
    0xCE,
    0x0E,
    0x0E,
    0xCE,
    0x0E,
    0xCE,
    0x4E,
    0x4E,
    0x4E,
    0x0E,
    0xCE,
    0x4E,
    0x4E,
    0xCE,
    0x0E,
    0x46,
    0xE6,
    0x46,
    0x06,
    0xD6,
    0x06,
    0xCE,
    0x4E,
    0x4E,
    0xCE,
    0x0E,
    0xCE,
    0x4E,
    0x4E,
    0xCE,
    0x0E,
    0x0E,
    0x0E,
    0x0E,
    0x0E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x7F,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x47,
    0x45,
    0x45,
    0x45,
    0x40,
    0x47,
    0x44,
    0x44,
    0x47,
    0x40,
    0x47,
    0x44,
    0x44,
    0x47,
    0x40,
    0x47,
    0x44,
    0x44,
    0x44,
    0x40,
    0x47,
    0x42,
    0x42,
    0x47,
    0x40,
    0x40,
    0x47,
    0x40,
    0x40,
    0x47,
    0x40,
    0x47,
    0x44,
    0x44,
    0x47,
    0x40,
    0x47,
    0x40,
    0x40,
    0x47,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x40,
    0x7F,
    0x00,
};
const unsigned char laserWarning[504] PROGMEM = {
    0xFF,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x7D,
    0x41,
    0x7D,
    0x41,
    0x7D,
    0x01,
    0x7D,
    0x15,
    0x15,
    0x7D,
    0x01,
    0x7D,
    0x15,
    0x35,
    0x5D,
    0x01,
    0x7D,
    0x19,
    0x31,
    0x7D,
    0x01,
    0x7D,
    0x01,
    0x7D,
    0x19,
    0x31,
    0x7D,
    0x01,
    0x7D,
    0x45,
    0x55,
    0x75,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0xFF,
    0xFF,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x81,
    0x61,
    0x11,
    0x09,
    0x11,
    0x61,
    0x81,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x41,
    0xC1,
    0x41,
    0x01,
    0xC1,
    0x01,
    0xC1,
    0x01,
    0xC1,
    0x01,
    0xC1,
    0x41,
    0x41,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x80,
    0x60,
    0x18,
    0x06,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x06,
    0x18,
    0x60,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xF0,
    0x17,
    0x10,
    0xE0,
    0x07,
    0xF1,
    0x57,
    0x10,
    0x07,
    0xF0,
    0x05,
    0xF5,
    0x07,
    0xF0,
    0x00,
    0xF0,
    0x10,
    0x10,
    0x00,
    0xF0,
    0x50,
    0x10,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xC0,
    0x20,
    0x18,
    0x06,
    0x01,
    0x10,
    0x20,
    0x40,
    0x80,
    0x00,
    0x80,
    0xFC,
    0x80,
    0x00,
    0x80,
    0x40,
    0x20,
    0x10,
    0x01,
    0x06,
    0x18,
    0x20,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x7D,
    0x41,
    0x7D,
    0x00,
    0x5C,
    0x55,
    0x75,
    0x01,
    0x7C,
    0x54,
    0x45,
    0x00,
    0x5C,
    0x55,
    0x74,
    0x01,
    0x01,
    0x01,
    0x00,
    0x01,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xC0,
    0x30,
    0x0C,
    0x03,
    0x00,
    0x00,
    0x00,
    0x04,
    0x04,
    0x04,
    0x84,
    0x44,
    0x2E,
    0x1F,
    0x3F,
    0xFF,
    0x3F,
    0x1F,
    0x2E,
    0x44,
    0x84,
    0x04,
    0x04,
    0x04,
    0x04,
    0x04,
    0x04,
    0x07,
    0x0C,
    0x30,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1F,
    0x10,
    0x10,
    0x00,
    0x1F,
    0x05,
    0x1F,
    0x00,
    0x17,
    0x15,
    0x1D,
    0x00,
    0x1F,
    0x15,
    0x11,
    0x00,
    0x1F,
    0x0D,
    0x17,
    0x00,
    0x17,
    0x15,
    0x1D,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0xB0,
    0xAC,
    0xA3,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA1,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA7,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA1,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA3,
    0xAC,
    0xB0,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0xFF,
};
const unsigned char menuLogo[504] PROGMEM = {
    0x00,
    0xFE,
    0x02,
    0xFA,
    0x2A,
    0x3A,
    0x02,
    0x3E,
    0x12,
    0x32,
    0x02,
    0x3A,
    0x2A,
    0xBA,
    0xC2,
    0x62,
    0x32,
    0x9A,
    0x32,
    0x62,
    0xC2,
    0x92,
    0x3A,
    0x12,
    0x02,
    0x3A,
    0x2A,
    0x3A,
    0x02,
    0x3A,
    0x0A,
    0x3A,
    0x02,
    0xFE,
    0x00,
    0x00,
    0x00,
    0xFE,
    0x82,
    0x00,
    0x78,
    0x24,
    0x24,
    0x24,
    0x78,
    0x00,
    0x82,
    0xFE,
    0x00,
    0x00,
    0x04,
    0x7E,
    0x84,
    0x84,
    0x60,
    0x00,
    0x00,
    0x10,
    0x38,
    0x10,
    0x00,
    0x00,
    0x3E,
    0x40,
    0x80,
    0x40,
    0x3E,
    0x00,
    0x00,
    0x10,
    0x38,
    0x10,
    0x00,
    0xFC,
    0x22,
    0x22,
    0x22,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x80,
    0xC0,
    0x60,
    0x30,
    0x18,
    0x0C,
    0x06,
    0x23,
    0x21,
    0x24,
    0xA8,
    0x70,
    0xFF,
    0x70,
    0xA8,
    0x24,
    0x21,
    0x23,
    0x26,
    0x2C,
    0x38,
    0x30,
    0x60,
    0xC0,
    0x80,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0xFC,
    0x04,
    0x00,
    0xF8,
    0xA8,
    0xA8,
    0xA8,
    0x50,
    0x00,
    0x04,
    0xFC,
    0x00,
    0x00,
    0xC0,
    0x70,
    0x18,
    0x0C,
    0x18,
    0x70,
    0xC0,
    0x00,
    0xFC,
    0x24,
    0x24,
    0x24,
    0x3C,
    0x00,
    0x00,
    0x20,
    0x70,
    0x20,
    0x00,
    0x00,
    0xFC,
    0x24,
    0x24,
    0x24,
    0x04,
    0x00,
    0xF0,
    0x80,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x1C,
    0x16,
    0xD3,
    0x91,
    0x10,
    0x90,
    0xD0,
    0x10,
    0x90,
    0x50,
    0x54,
    0x52,
    0x51,
    0x90,
    0x10,
    0x17,
    0x50,
    0x50,
    0xD1,
    0x52,
    0x54,
    0x10,
    0x10,
    0xD0,
    0x90,
    0x10,
    0x10,
    0xD1,
    0x13,
    0x16,
    0x1C,
    0xFF,
    0x00,
    0x00,
    0x00,
    0xF9,
    0x09,
    0x20,
    0x60,
    0xE0,
    0xE0,
    0xE0,
    0x60,
    0x20,
    0x09,
    0xF9,
    0x00,
    0x00,
    0x81,
    0xE1,
    0x31,
    0x19,
    0x31,
    0xE1,
    0x81,
    0x00,
    0x19,
    0xA0,
    0x40,
    0xA0,
    0x18,
    0x00,
    0x00,
    0xA0,
    0xA0,
    0xA0,
    0x00,
    0x00,
    0xF1,
    0x29,
    0x49,
    0x89,
    0xF1,
    0x00,
    0x81,
    0x80,
    0x81,
    0x80,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0xF7,
    0x20,
    0xC3,
    0x20,
    0xF7,
    0x00,
    0x07,
    0xF2,
    0x52,
    0x52,
    0x52,
    0x17,
    0x00,
    0x00,
    0xF4,
    0x64,
    0x47,
    0x84,
    0xF4,
    0x00,
    0x00,
    0xF7,
    0x01,
    0x01,
    0x02,
    0xF7,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0xF3,
    0x12,
    0x00,
    0x80,
    0xC0,
    0xE1,
    0xC0,
    0x80,
    0x00,
    0x12,
    0xF3,
    0x00,
    0x00,
    0xE3,
    0x42,
    0x82,
    0x42,
    0xE2,
    0x02,
    0xE3,
    0xA0,
    0xA3,
    0x20,
    0x00,
    0xE0,
    0x43,
    0x80,
    0xE0,
    0x00,
    0xE0,
    0x00,
    0xE0,
    0x00,
    0x01,
    0x02,
    0x02,
    0x02,
    0x01,
    0x00,
    0x03,
    0x00,
    0x03,
    0x00,
    0x03,
    0x00,
    0x00,
    0x00,
    0xE0,
    0x27,
    0x24,
    0x24,
    0x25,
    0x24,
    0x24,
    0x24,
    0x25,
    0xA4,
    0xA4,
    0x25,
    0x25,
    0x25,
    0x25,
    0x25,
    0x24,
    0x24,
    0x25,
    0xE4,
    0x04,
    0xE4,
    0x25,
    0x24,
    0x24,
    0x24,
    0x25,
    0x25,
    0xA5,
    0xA4,
    0xA4,
    0xA4,
    0xA4,
    0x27,
    0x20,
    0x20,
    0x20,
    0x27,
    0x24,
    0x21,
    0xE1,
    0x01,
    0xE1,
    0x21,
    0x21,
    0x21,
    0x24,
    0x27,
    0x20,
    0xA0,
    0xA7,
    0xA0,
    0xA1,
    0xA0,
    0x27,
    0x20,
    0x27,
    0x24,
    0x24,
    0x24,
    0x20,
    0xE7,
    0x00,
    0xE0,
    0x27,
    0xA0,
    0xA7,
    0xA4,
    0xA7,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0xA0,
    0x20,
    0xE0,
    0x00,
    0xFF,
    0x80,
    0x80,
    0xA0,
    0xB0,
    0xB8,
    0xBC,
    0xBE,
    0xBF,
    0xBF,
    0xBF,
    0xBF,
    0xBE,
    0xBC,
    0xB8,
    0xB0,
    0xA0,
    0x80,
    0x80,
    0xFF,
    0x00,
    0xFF,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0xBF,
    0x84,
    0x84,
    0x84,
    0x84,
    0x84,
    0xBF,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0xFF,
    0x00,
    0xFF,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0xBF,
    0xA4,
    0xA4,
    0xA4,
    0xA4,
    0x9B,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0xFF,
    0x00,
    0xFF,
    0x80,
    0x80,
    0x81,
    0x83,
    0x87,
    0x87,
    0x8F,
    0x9B,
    0x9F,
    0xBF,
    0x9F,
    0x8F,
    0x87,
    0x83,
    0x83,
    0x81,
    0x80,
    0x80,
    0xFF,
    0x00,
};
const unsigned char basic_outlineFrame[504] PROGMEM = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x41,
    0x41,
    0x41,
    0x55,
    0x7F,
    0x55,
    0x7F,
    0x55,
    0x41,
    0x41,
    0x41,
    0xFF,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x5F,
    0x61,
    0x5F,
    0x41,
    0x7D,
    0x53,
    0x7D,
    0x41,
    0x7F,
    0x61,
    0x41,
    0x7F,
    0x61,
    0x7F,
    0x41,
    0x7F,
    0x6B,
    0x63,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0xFF,
    0x41,
    0x41,
    0x41,
    0x55,
    0x7F,
    0x55,
    0x7F,
    0x55,
    0x41,
    0x41,
    0x41,
    0xFF,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x5F,
    0x61,
    0x5F,
    0x41,
    0x7D,
    0x53,
    0x7D,
    0x41,
    0x7F,
    0x61,
    0x41,
    0x7F,
    0x61,
    0x7F,
    0x41,
    0x7F,
    0x6B,
    0x63,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x09,
    0x19,
    0x39,
    0x79,
    0x39,
    0x19,
    0x09,
    0x01,
    0x7D,
    0x55,
    0x55,
    0x01,
    0x6D,
    0x11,
    0x6D,
    0x01,
    0x05,
    0x7D,
    0x05,
    0x01,
    0xFF,
    0x01,
    0x01,
    0x11,
    0x39,
    0x7D,
    0xFF,
    0x01,
    0x7D,
    0x55,
    0x29,
    0x01,
    0x7D,
    0x45,
    0x45,
    0x01,
    0x7D,
    0x11,
    0x6D,
    0x01,
    0x01,
    0xFF,
    0x01,
    0x01,
    0x7D,
    0x09,
    0x11,
    0x7D,
    0x01,
    0x6D,
    0x11,
    0x6D,
    0x01,
    0x05,
    0x7D,
    0x05,
    0x01,
    0xFF,
    0x7D,
    0x39,
    0x11,
    0x01,
    0x01,
    0xFF,
    0x01,
    0xF9,
    0x29,
    0x39,
    0x01,
    0xB9,
    0xA9,
    0xF9,
    0x01,
    0x21,
    0x01,
    0x7D,
    0x01,
    0x21,
    0x31,
    0x39,
    0x3D,
    0x39,
    0x31,
    0x21,
};
const unsigned char single_outlineFrame[504] PROGMEM = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x41,
    0x41,
    0x41,
    0x55,
    0x7F,
    0x55,
    0x7F,
    0x55,
    0x41,
    0x41,
    0x41,
    0xFF,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x5F,
    0x61,
    0x5F,
    0x41,
    0x7D,
    0x53,
    0x7D,
    0x41,
    0x7F,
    0x61,
    0x41,
    0x7F,
    0x61,
    0x7F,
    0x41,
    0x7F,
    0x6B,
    0x63,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0x41,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x09,
    0x19,
    0x39,
    0x79,
    0x39,
    0x19,
    0x09,
    0x01,
    0x7D,
    0x55,
    0x55,
    0x01,
    0x6D,
    0x11,
    0x6D,
    0x01,
    0x05,
    0x7D,
    0x05,
    0x01,
    0xFF,
    0x01,
    0x01,
    0x11,
    0x39,
    0x7D,
    0xFF,
    0x01,
    0x7D,
    0x55,
    0x29,
    0x01,
    0x7D,
    0x45,
    0x45,
    0x01,
    0x7D,
    0x11,
    0x6D,
    0x01,
    0x01,
    0xFF,
    0x01,
    0x01,
    0x7D,
    0x09,
    0x11,
    0x7D,
    0x01,
    0x6D,
    0x11,
    0x6D,
    0x01,
    0x05,
    0x7D,
    0x05,
    0x01,
    0xFF,
    0x7D,
    0x39,
    0x11,
    0x01,
    0x01,
    0xFF,
    0x01,
    0xF9,
    0x29,
    0x39,
    0x01,
    0xB9,
    0xA9,
    0xF9,
    0x01,
    0x21,
    0x01,
    0x7D,
    0x01,
    0x21,
    0x31,
    0x39,
    0x3D,
    0x39,
    0x31,
    0x21,
};
//########################################################################################
//                                TOOLS
// #include<tools.h> NOT IN USE
void codeChecker(int checking)
{
    bool useThis = false;

    if (useThis == true)
    {
        checkNum++;
        Serial.print("!!!! !!!!    CHECK CODE  [  ");
        Serial.print(checking);
        Serial.println("  ]     !!!! !!!!");
    }
}
//########################################################################################
// turns lasers on and off <first three bools> used for lasers last bool to print or not.
void laser_switch(bool lsrA_bool, bool lsrB_bool, bool lsrC_bool, bool serialPrint_bool)
{
    // Serial.print("##  \t"); // starts line with relavant symbol
    if (serialPrint_bool == true)
    {
        if (lsrA_bool == true)
        {
            digitalWrite(laserPinA, HIGH);
            if (serialPrint_bool == true)
            {
                Serial.println(F("**  | LASER A | ON  | **"));
            }
        }
        else
        {
            digitalWrite(laserPinA, LOW);
            if (serialPrint_bool == true)
            {
                Serial.println(F("**  | LASER A | OFF | **"));
            }
        }
        if (lsrB_bool == true)
        {
            digitalWrite(laserPinB, HIGH);
            if (serialPrint_bool == true)
            {
                Serial.println(F("**  | LASER B | ON  | **"));
            }
        }
        else
        {
            digitalWrite(laserPinB, LOW);
            if (serialPrint_bool == true)
            {
                Serial.println(F("**  | LASER B | OFF | **"));
            }
        }
        if (lsrC_bool == true)
        {
            digitalWrite(laserPinC, HIGH);
            if (serialPrint_bool == true)
            {
                Serial.println(F("**  | LASER C | ON  | **"));
            }
        }
        else
        {
            digitalWrite(laserPinC, LOW);
            if (serialPrint_bool == true)
            {
                Serial.println(F("**  | LASER C | OFF | **"));
            }
        }
    }
}
//----------------------------------------------------------------------------------------
void lcdLight_switch(bool stateLCD_LED)
{
    if (stateLCD_LED == false)
    {
        digitalWrite(lcdPinLED, LOW);
    }
    if (stateLCD_LED == true)
    {
        digitalWrite(lcdPinLED, HIGH);
    }
}
//----------------------------------------------------------------------------------------
//########################################################################################
//    Init Variables
//########################################################################################
void serialPrint_time_variables()
{
    Serial.println(">> ---- INIT -- CURRENT VALUES OF TIME VARIABLES ----<");
    Serial.print("-> time_Ai_lng :"); // micros when laser A goes LOW
    Serial.print(time_Ai_lng);        // micros when laser A goes LOW
    Serial.print(" | time_Af_lng :"); // micros when laser A goes HIGH AGAIN
    Serial.print(time_Af_lng);        // micros when laser A goes HIGH AGAIN
    Serial.print(" | time_Ad_lng :"); // change in time between <time_Xx_lng>
    Serial.println(time_Ad_lng);      // change in time between <time_Xx_lng>
    Serial.print("-> time_Bi_lng :"); // micros when laser B goes LOW
    Serial.print(time_Bi_lng);        // micros when laser B goes LOW
    Serial.print(" | time_Bf_lng :"); // micros when laser B goes HIGH AGAIN
    Serial.print(time_Bf_lng);        // micros when laser B goes HIGH AGAIN
    Serial.print(" | time_Bd_lng :"); // change in time between <time_Xx_lng>
    Serial.println(time_Bd_lng);      // change in time between <time_Xx_lng>
    Serial.print("-> time_Ci_lng :"); // micros when laser C goes LOW
    Serial.print(time_Ci_lng);        // micros when laser C goes LOW
    Serial.print(" | time_Cf_lng :"); // micros when laser C goes HIGH AGAIN
    Serial.print(time_Cf_lng);        // micros when laser C goes HIGH AGAIN
    Serial.print(" | time_Cd_lng :"); // change in time between <time_Xx_lng>
    Serial.println(time_Cd_lng);      // change in time between <time_Xx_lng>
    Serial.print("-> time_Di_lng :"); // micros when laser C goes HIGH(momentum)
    Serial.print(time_Di_lng);        // micros when laser C goes HIGH(momentum)
    Serial.print(" | time_Df_lng :"); // micros when laser C goes LOW(momentum)
    Serial.print(time_Df_lng);        // micros when laser C goes LOW(momentum)
    Serial.print(" | time_Dd_lng :"); // micros change between A and B(momentum)
    Serial.println(time_Dd_lng);      // micros change between A and B(momentum)

    Serial.print("-> time_ABi_lng:"); // avg of time at laser A
    Serial.print(time_ABi_lng);       // avg of time at laser A
    Serial.print(" | time_ABf_lng:"); // avg of time at laser B
    Serial.print(time_ABf_lng);       // avg of time at laser B
    Serial.print(" | time_ABd_lng:"); // change in time between <time_Xx_lng Laser A and B>
    Serial.println(time_ABd_lng);     // change in time between <time_Xx_lng Laser A and B>
    Serial.print("-> time_BCi_lng:"); // avg of time at laser B
    Serial.print(time_BCi_lng);       // avg of time at laser B
    Serial.print(" | time_BCf_lng:"); // avg of time at laser C
    Serial.print(time_BCf_lng);       // avg of time at laser C
    Serial.print(" | time_BCd_lng:"); // change in time between <time_Xx_lng Laser B and C>
    Serial.println(time_BCd_lng);     // change in time between <time_Xx_lng Laser B and C>
    Serial.print("-> time_ACi_lng:"); // avg of time at laser A
    Serial.print(time_ACi_lng);       // avg of time at laser A
    Serial.print(" | time_ACf_lng:"); // avg of time at laser C
    Serial.print(time_ACf_lng);       // avg of time at laser C
    Serial.print(" | time_ACd_lng:"); // change in time between <time_Xx_lng Laser A and C>
    Serial.println(time_ACd_lng);     // change in time between <time_Xx_lng Laser A and C>
    Serial.print("-> time_CDi_lng:"); // avg of time at laser A
    Serial.print(time_CDi_lng);       // avg of time at laser A
    Serial.print(" | time_CDf_lng:"); // avg of time at laser C
    Serial.print(time_CDf_lng);       // avg of time at laser C
    Serial.print(" | time_CDd_lng:"); // change in time between <time_Xx_lng
    Serial.println(time_CDd_lng);     // change in time between <time_Xx_lng
}
void serialPrint_control_variables()
{
    Serial.println(">> ---- INIT -- CURRENT VALUES OF CONTROL VARIABLES ----<");
    Serial.print("-> state_printOnceA: "); // used when prtinging <lcdSend_menuFrame();>
    Serial.print(state_printOnceA);        // used when prtinging <lcdSend_menuFrame();>
    Serial.print(" | state_printOnceB: ");
    Serial.print(state_printOnceB);
    Serial.print(" | state_printOnceC: ");
    Serial.print(state_printOnceC);
    Serial.print(" | state_printOnceD: ");
    Serial.println(state_printOnceD);
    Serial.print("-> state_rsltMenu:"); // used when turing pages in the results display
    Serial.print(state_rsltMenu);       // used when turing pages in the results display
    Serial.print(" | state_trackPos:");
    Serial.println(state_trackPos);
    Serial.print("-> button_mainMenu:");
    Serial.print(button_mainMenu);
    Serial.print(" | button_rsltMenu:");
    Serial.println(button_rsltMenu);
    Serial.print("-> cntrl_mainMenu:");
    Serial.print(cntrl_mainMenu);
    Serial.print(" | cntrl_functionMode:");
    Serial.println(cntrl_functionMode);
    Serial.print("-> prnt_rsltPage:");
    Serial.print(prnt_rsltPage);
    Serial.println(">> ---- INIT -- CURRENT VALUES OF CONTROL VARIABLES ----<");
}
void init_time_variables(bool serialPrint_bool)
{
    if (serialPrint_bool == true)
    {
        serialPrint_time_variables();
    }

    // Serial.println("INIT -- all time values set back to Zero");

    time_Ai_lng = 0;  // micros when laser A goes LOW
    time_Af_lng = 0;  // micros when laser A goes HIGH AGAIN
    time_Ad_lng = 0;  // change in time between <time_Xx_lng>
    time_Bi_lng = 0;  // micros when laser B goes LOW
    time_Bf_lng = 0;  // micros when laser B goes HIGH AGAIN
    time_Bd_lng = 0;  // change in time between <time_Xx_lng>
    time_Ci_lng = 0;  // micros when laser C goes LOW
    time_Cf_lng = 0;  // micros when laser C goes HIGH AGAIN
    time_Cd_lng = 0;  // change in time between <time_Xx_lng>
    time_Di_lng = 0;  // micros when laser C goes HIGH(momentum)
    time_Df_lng = 0;  // micros when laser C goes LOW(momentum)
    time_Dd_lng = 0;  // micros change between A and B(momentum)
    time_ABi_lng = 0; // avg of time at laser A
    time_ABf_lng = 0; // avg of time at laser B
    time_ABd_lng = 0; // change in time between <time_Xx_lng Laser A and B>
    time_BCi_lng = 0; // avg of time at laser B
    time_BCf_lng = 0; // avg of time at laser C
    time_BCd_lng = 0; // change in time between <time_Xx_lng Laser B and C>
    time_ACi_lng = 0; // avg of time at laser A
    time_ACf_lng = 0; // avg of time at laser C
    time_ACd_lng = 0; // change in time between <time_Xx_lng Laser A and C>
    time_CDi_lng = 0; // avg of time at laser A
    time_CDf_lng = 0; // avg of time at laser C
    time_CDd_lng = 0; // change in time between <time_Xx_lng
    if (serialPrint_bool == true)
    {
        serialPrint_time_variables();
    }
}

void init_control_variables(bool serialPrint_bool)
{
    if (serialPrint_bool == true)
    {
        serialPrint_control_variables();
    }
    state_printOnceA = true; // used when prtinging <lcdSend_menuFrame();>
    state_printOnceB = true;
    state_printOnceC = true;
    state_printOnceD = true;
    cntrl_mainMenu = 0;
    state_rsltMenu = 0; // used when turing pages in the results display
    state_trackPos = 0;
    button_mainMenu = 0;
    button_rsltMenu = 0;
    cntrl_functionMode = 0;
    // prnt_rsltPage = 1; //incrments when buttons are press
    if (serialPrint_bool == true)
    {
        serialPrint_control_variables();
    }
}
//########################################################################################
//    serial PARTS
//########################################################################################
void serialPart_singleTabLine()
{
    Serial.print(F("\t\t|")); // (F("\t| "))
}
//----------------------------------------------------------------------------------------
void serialPart_doubleTabLine()
{
    Serial.print(F("\t\t|")); // (F("\t\t| "))
}
//----------------------------------------------------------------------------------------
void serialPart_hashLineLong()
{
    Serial.println(F("#######################################################"));
}
//----------------------------------------------------------------------------------------
void serialPart_plusLineLong()
{
    Serial.println(F("++++++++++++++++++++++++++++++++++++++++++++++++++++"));
}
void serialPart_tab()
{
    Serial.print(F("\t "));
}
void serialPart_endLine()
{
    Serial.print(F("\n"));
}
//----------------------------------------------------------------------------------------
void serialPart_timeLineHead(char laserNum)
{
    Serial.print(++readingNum);
    serialPart_tab();
    Serial.print(++readingNum_time);
    serialPart_tab();
    Serial.print(F("sec_"));
    Serial.print(laserNum);
    serialPart_tab();
}
void serialPart_periodLineHead(char laserNum)
{
    Serial.print(++readingNum);
    serialPart_tab();
    Serial.print(++readingNum_period);
    serialPart_tab();
    Serial.print(F("per_"));
    Serial.print(laserNum);
    serialPart_tab();
}
void serialPart_veloLineHead(char laserNum)
{
    Serial.print(++readingNum);
    serialPart_tab();
    Serial.print(++readingNum_velocity);
    serialPart_tab();
    Serial.print(F("vel_"));
    Serial.print(laserNum);
    serialPart_tab();
}
void serialPart_accLineHead(char laserNum)
{
    Serial.print(++readingNum);
    serialPart_tab();
    Serial.print(++readingNum_velocity);
    serialPart_tab();
    Serial.print(F("acc_"));
    Serial.print(laserNum);
    serialPart_tab();
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------

//########################################################################################
//    SEND SERIAL
//########################################################################################
void serialStartUp(bool serialPrint_bool)
{
    if (serialPrint_bool == true)
    {
        Serial.println(F(""));
        serialPart_hashLineLong(); // "########################"
        Serial.println(F("    LASER TIMER, VELOCITY, ACCELERATION AND ENERGY"));
        serialPart_hashLineLong(); // "########################"
        // Serial.println(F(""));
    }
    for (int x = 0; x < 3; x++)
    {
        laser_switch(true, true, true, false);
        delay(wait_hundred);
        laser_switch(false, false, false, false);
        delay(wait_fifty);
    }
}
//----------------------------------------------------------------------------------------
void modeA_serialPrint(bool serialPrint_bool)
{
    if (serialPrint_bool == true)
    {
        serialPart_plusLineLong(); // "++++++++++++"
        Serial.print(F("\t\tTEST NUMBER: "));
        Serial.println(countTest);
        serialPart_plusLineLong(); // "++++++++++++"
        Serial.println(F("\n## ---- ---- ----   MODE A READY   ---- ---- ---- ##"));
    }
}
//----------------------------------------------------------------------------------------
void modeB_serialPrint(bool serialPrint_bool)
{
    if (serialPrint_bool == true)
    {
        serialPart_plusLineLong(); // "++++++++++++"
        Serial.print(F("\t\tTEST NUMBER: "));
        Serial.println(countTest);
        serialPart_plusLineLong(); // "++++++++++++"
        Serial.println(F("\n## ---- ---- ----   MODE B READY   ---- ---- ---- ##"));
    }
}
//--------------------------------------------- SERIAL - MODE A -------------------------------------------
void serialPrint_modeA_time()
{
    // Serial.println(F("#A# \t----\t TIME - MODE A  \t----\t #A#"));
    // Serial.println(F("<<| Laser | start(us) | stop(us) | dt(us) | dt(sec) |>>"));
    Serial.println("|\t MODE_A\t RESULTS\t |");
    Serial.println("TOT\t NUM \t NME \t VAL");
    serialPart_timeLineHead('a');
    Serial.print(time_Ai_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    //time B seiral
    serialPart_timeLineHead('A');
    Serial.print(time_Af_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_timeLineHead('b');
    Serial.print(time_Bi_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_timeLineHead('B');
    Serial.print(time_Bf_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_timeLineHead('c');
    Serial.print(time_Ci_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_timeLineHead('C');
    Serial.print(time_Cf_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))
    serialPart_endLine(); //(F("\t|>>\n"))
}
void serialPrint_modeA_period()
{
    // Serial.println(F(">> \t----\t PERIOD - MODE A  \t----\t <<"));
    // Serial.println(F("<<| Laser | start(us) | stop(us) | dt(us) | dt(sec) |>>"));

    serialPart_periodLineHead('a');
    // Serial.print(F("<<|TIMER A\t|"));
    // Serial.print(time_Ai_lng);
    Serial.print(period_A_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    //time B seiral
    serialPart_periodLineHead('b');
    // Serial.print(F("<<|TIMER B\t|"));
    // Serial.print(time_Bi_lng);
    Serial.print(period_B_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_periodLineHead('c');
    Serial.print(period_C_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_periodLineHead('A');
    Serial.print(period_AB_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_periodLineHead('B');
    Serial.print(period_BC_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_periodLineHead('C');
    Serial.print(period_AC_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))
    serialPart_endLine(); //(F("\t|>>\n"))
}
void serialPrint_modeA_velocity()
{
    // Serial.println(F("#A# \t----\t VELOCITY - MODE A  \t----\t #A#"));
    serialPart_veloLineHead('a'); //(F("%%|VELO x \t|"));

    Serial.print(velocity_A_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))

    serialPart_veloLineHead('A'); //(F("%%|VELO x \t|"));
    Serial.print(velocity_AB_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))

    serialPart_veloLineHead('b'); //(F("%%|VELO x \t|"));
    Serial.print(velocity_B_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))

    serialPart_veloLineHead('B'); //(F("%%|VELO x \t|"));
    Serial.print(velocity_BC_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))

    serialPart_veloLineHead('c'); //(F("%%|VELO x \t|"));
    Serial.print(velocity_C_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))

    serialPart_veloLineHead('C'); //(F("%%|VELO x \t|"));
    Serial.print(velocity_AC_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))
    serialPart_endLine(); //(F("\t\t|%%\n"))
}
void serialPrint_modeA_acceleration()
{
    // Serial.println(F("#A# \t----\t ACCELERATION - MODE  \t----\t #A#"));

    serialPart_accLineHead('A'); //(F("##|ACC AB\t| "))
    Serial.print(acceleration_AB_glb, 5);
    serialPart_endLine(); //(F("\t|##\n"))

    serialPart_accLineHead('B'); //(F("##|ACC AB\t| "))
    Serial.print(acceleration_BC_glb, 5);
    serialPart_endLine(); //(F("\t|##\n"))

    serialPart_accLineHead('C'); //(F("##|ACC AB\t| "))
    Serial.print(acceleration_AC_glb, 5);
    serialPart_endLine(); //(F("\t|##\n"))
    serialPart_endLine(); //(F("\t|##\n"))
}
//--------------------------------------------- SERIAL - MODE B -------------------------------------------
void serialPrint_modeB_timeA()
{
    // Serial.println(F("#B# \t----\t TIME A - MODE B  \t----\t #B#"));
    // Serial.println(F("<<| Laser | start(us) | stop(us) | dt(us) | dt(sec) |>>"));
    Serial.println("|\t MODE_B\t RESULTS\t |");
    Serial.println("TOT\t NUM \t NME \t VAL");
    serialPart_timeLineHead('a');
    Serial.print(time_Ai_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    //time B seiral
    serialPart_timeLineHead('A');
    Serial.print(time_Af_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_timeLineHead('b');
    Serial.print(time_Bi_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_timeLineHead('B');
    Serial.print(time_Bf_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))
    serialPart_endLine(); //(F("\t|>>\n"))
}
void serialPrint_modeB_timeB()
{
    // Serial.println(F("#B# \t----\t TIME B - MODE B  \t----\t #B#"));
    // Serial.println(F("<<| Laser | start(us) | stop(us) | dt(us) | dt(sec) |>>"));

    serialPart_timeLineHead('c');
    Serial.print(time_Ci_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    //time B seiral
    serialPart_timeLineHead('C');
    Serial.print(time_Cf_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_timeLineHead('d');
    Serial.print(time_Di_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_timeLineHead('D');
    Serial.print(time_Df_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))
    serialPart_endLine(); //(F("\t|>>\n"))
}
void serialPrint_modeB_periodA()
{
    // Serial.println(F("#B# \t----\t PERIOD A - MODE B  \t----\t #B#"));
    // Serial.println(F("<<| Laser | start(us) | stop(us) | dt(us) | dt(sec) |>>"));

    serialPart_periodLineHead('a');
    Serial.print(period_A_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    //time B seiral
    serialPart_periodLineHead('b');
    Serial.print(period_B_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_periodLineHead('c');
    Serial.print(period_C_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_periodLineHead('d');
    Serial.print(period_D_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))
    serialPart_endLine(); //(F("\t|>>\n"))
}
void serialPrint_modeB_periodB()
{
    // Serial.println(F("<< PERIOD B >>"));

    serialPart_periodLineHead('A');
    Serial.print(period_AB_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_periodLineHead('B');
    Serial.print(period_BC_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))

    serialPart_periodLineHead('C');
    Serial.print(period_CD_glb, 4);
    serialPart_endLine(); //(F("\t|>>\n"))
    serialPart_endLine(); //(F("\t|>>\n"))
}
void serialPrint_modeB_velocity()
{
    // Serial.println(F("%% \t----\t VELOCITY FORWARD  \t----\t ##"));
    serialPart_veloLineHead('A'); //(F("%%|VELO x \t|"));

    Serial.print(velocity_A_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))

    serialPart_veloLineHead('B'); //(F("%%|VELO x \t|"));
    Serial.print(velocity_B_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))

    serialPart_veloLineHead('C'); //(F("%%|VELO x \t|"));
    Serial.print(velocity_C_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))

    serialPart_veloLineHead('D'); //(F("%%|VELO x \t|"));
    Serial.print(velocity_D_glb, 3);
    serialPart_endLine(); //(F("\t\t|%%\n"))
    serialPart_endLine(); //(F("\t\t|%%\n"))
}
void serialPrint_modeB_acceleration()
{
    // Serial.println(F("## \t----\t ACCELERATION FORWARD  \t----\t ##"));
    // Serial.println("STILL UNDER DEVELOPMENT");
    // Serial.println("        UNDER");
    // Serial.println("                DEVELOPMENT");
}
//########################################################################################
//    LCD PARTS
//########################################################################################
void lcdPart_dashline(int yLine)
{
    lcd.setCursor(1, yLine);
    lcd.print("----------------");
}
//----------------------------------------------------------------------------------------
void lcdPart_clearScreen()
{
    lcd.clear();
}
//----------------------------------------------------------------------------------------
void lcdPart__cursorToZero()
{
    lcd.setCursor(0, 0); //  lcd.setCursor(0,0);
}
//----------------------------------------------------------------------------------------
void lcd_sendTrasitionScreen(char headTxt[], char subTxt[], char mssgTxt[])
{
    int yPos = 0;
    lcdPart_clearScreen();
    //. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    lcdPart_dashline(yPos++); // draws a line across the screen
    //. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    int nameLength = strlen(headTxt);    // gets legth of string
    int namePix = nameLength * 5;        // calculates the number of x pixels on screen
    int posXLength = (84 - namePix) / 2; // starting position (delta / 2)
    lcd.setCursor(posXLength, yPos++);
    lcd.print((char *)headTxt);

    lcdPart_dashline(yPos++); // draws a line across the screen

    int orderLength = strlen(subTxt);    // gets legth of string
    int orderPix = orderLength * 5;      // calculates the number of x pixels on screen
    int posXorder = (84 - orderPix) / 2; // starting position (delta / 2)
    lcd.setCursor(posXorder, yPos++);
    lcd.print((char *)subTxt);

    lcdPart_dashline(yPos++); // draws a line across the screen

    int messgageLength = strlen(mssgTxt);    // calculates the number of x pixels on screen
    int messagePix = messgageLength * 5;     // calculates the number of x pixels on screen
    int posXmessage = (84 - messagePix) / 2; // starting position (delta / 2)
    lcd.setCursor(posXmessage, yPos++);
    lcd.print((char *)mssgTxt);
}

//########################################################################################
//    SEND LCD
//########################################################################################
// full screen write BYTES
void lcdSend_startUpRoutine() // routine that gets sent when the device powers up
{
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    lcdPart_clearScreen();
    delay(wait_halfSec);
    lcdLight_switch(false);
    delay(wait_halfSec);
    lcdPart_clearScreen();
    delay(wait_halfSec);
    lcdLight_switch(true);

    lcd_sendTrasitionScreen((char *)"PhotonTimer", (char *)"beta: v.3.1", (char *)"29/06/2019");
    delay(wait_halfSec);
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    lcdPart_clearScreen();
    lcd.draw(hintEduLogo, sizeof(hintEduLogo) / sizeof(hintEduLogo[0]), true);
    delay(wait_halfSec);
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    lcdPart_clearScreen();
    lcd.draw(laserWarning, sizeof(laserWarning) / sizeof(laserWarning[0]), true);
    delay(wait_halfSec);
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    lcdPart_clearScreen();
}
void lcdSend_menuFrame()
{
    lcdPart_clearScreen();
    // lcdPart__cursorToZero(); //  lcd.setCursor(0,0);
    lcd.draw(menuLogo, sizeof(menuLogo) / sizeof(menuLogo[0]), true);
}
//--------------------------------------------- LCD - MODE A -------------------------------------------
void lcdPrint_modeA_time()
{
    lcdPart_clearScreen();
    //........................................................................................
    // lcdPart__cursorToZero(); //  lcd.setCursor(0,0);
    lcd.draw(basic_outlineFrame, sizeof(basic_outlineFrame) / sizeof(basic_outlineFrame[0]), true);
    lcd.setCursor(2, 0);
    lcd.print("Time(s)");
    lcd.setCursor(60, 0);
    lcd.print("1/4");
    //........................................................................................
    lcd.setCursor(2, 2);
    lcd.print("ai");
    lcd.setCursor(14, 2);
    char inputTimeA_char[4];
    dtostrf(time_Ai_glb, 4, 3, inputTimeA_char);
    lcd.print(inputTimeA_char);
    //........................................................................................
    lcd.setCursor(44, 2);
    lcd.print("af");
    lcd.setCursor(56, 2);
    char inputTimeB_char[4];
    dtostrf(time_Af_glb, 4, 3, inputTimeB_char);
    lcd.print(inputTimeB_char);

    lcd.setCursor(2, 3);
    lcd.print("bi");
    lcd.setCursor(14, 3);
    char inputTimeC_char[4];
    dtostrf(time_Bi_glb, 4, 3, inputTimeC_char);
    lcd.print(inputTimeC_char);

    lcd.setCursor(44, 3);
    lcd.print("bf");
    lcd.setCursor(56, 3);
    char inputTimeD_char[4];
    dtostrf(time_Bf_glb, 4, 3, inputTimeD_char);
    lcd.print(inputTimeD_char);

    lcd.setCursor(2, 4);
    lcd.print("ci");
    lcd.setCursor(14, 4);
    char inputTimeE_char[4];
    dtostrf(time_Ci_glb, 4, 3, inputTimeE_char);
    lcd.print(inputTimeE_char);

    lcd.setCursor(44, 4);
    lcd.print("cf");
    lcd.setCursor(56, 4);
    char inputTimeF_char[4];
    dtostrf(time_Cf_glb, 4, 3, inputTimeF_char);
    lcd.print(inputTimeF_char);
}
void lcdPrint_modeA_period()
{
    lcdPart_clearScreen();
    // lcdPart__cursorToZero(); //  lcd.setCursor(0,0);
    lcd.draw(basic_outlineFrame, sizeof(basic_outlineFrame) / sizeof(basic_outlineFrame[0]), true);
    lcd.setCursor(2, 0);
    lcd.print("Period(s)");
    lcd.setCursor(60, 0);
    lcd.print("2/4");

    lcd.setCursor(2, 2);
    lcd.print("a");
    lcd.setCursor(14, 2);
    char inputPeriod_A_char[4];
    dtostrf(period_A_glb, 4, 3, inputPeriod_A_char);
    lcd.print(inputPeriod_A_char);
    // second number

    lcd.setCursor(2, 3);
    lcd.print("b");
    lcd.setCursor(14, 3);
    char inputPeriod_B_char[4];
    dtostrf(period_B_glb, 4, 3, inputPeriod_B_char);
    lcd.print(inputPeriod_B_char);

    lcd.setCursor(2, 4);
    lcd.print("c");
    lcd.setCursor(14, 4);
    char inputPeriod_C_char[4];
    dtostrf(period_C_glb, 4, 3, inputPeriod_C_char);
    lcd.print(inputPeriod_C_char);

    lcd.setCursor(44, 2);
    lcd.print("ab");
    lcd.setCursor(56, 2);
    char inputPeriod_AB_char[4];
    dtostrf(period_AB_glb, 4, 3, inputPeriod_AB_char);
    lcd.print(inputPeriod_AB_char);

    lcd.setCursor(44, 3);
    lcd.print("bc");
    lcd.setCursor(56, 3);
    char inputPeriod_BC_char[4];
    dtostrf(period_BC_glb, 4, 3, inputPeriod_BC_char);
    lcd.print(inputPeriod_BC_char);

    lcd.setCursor(44, 4);
    lcd.print("ac");
    lcd.setCursor(56, 4);
    char inputPeriod_AC_char[4];
    dtostrf(period_AC_glb, 4, 3, inputPeriod_AC_char);
    lcd.print(inputPeriod_AC_char);
}
void lcdPrint_modeA_velocity()
{
    lcdPart_clearScreen();
    lcd.draw(basic_outlineFrame, sizeof(basic_outlineFrame) / sizeof(basic_outlineFrame[0]), true);

    lcd.setCursor(2, 0);
    lcd.print("VLCTY (m/s)");
    lcd.setCursor(60, 0);
    lcd.print("3/4");

    lcd.setCursor(2, 2);
    lcd.print("a");
    lcd.setCursor(14, 2);
    char inputVeloA_char[4];
    dtostrf(velocity_A_glb, 4, 3, inputVeloA_char);
    lcd.print(inputVeloA_char);

    lcd.setCursor(2, 3);
    lcd.print("ab");
    lcd.setCursor(14, 3);
    char inputVeloAB_char[4];
    dtostrf(velocity_AB_glb, 4, 3, inputVeloAB_char);
    lcd.print(inputVeloAB_char);

    lcd.setCursor(2, 4);
    lcd.print("b");
    lcd.setCursor(14, 4);
    char inputVeloB_char[4];
    dtostrf(velocity_B_glb, 4, 3, inputVeloB_char);
    lcd.print(inputVeloB_char);

    lcd.setCursor(44, 2);
    lcd.print("bc");
    lcd.setCursor(56, 2);
    char inputVeloBC_char[4];
    dtostrf(velocity_BC_glb, 4, 3, inputVeloBC_char);
    lcd.print(inputVeloBC_char);

    lcd.setCursor(44, 3);
    lcd.print("c");
    lcd.setCursor(56, 3);
    char inputVeloC_char[4];
    dtostrf(velocity_C_glb, 4, 3, inputVeloC_char);
    lcd.print(inputVeloC_char);

    lcd.setCursor(44, 4);
    lcd.print("ac");
    lcd.setCursor(56, 4);
    char inputVeloAC_char[4];
    dtostrf(velocity_AC_glb, 4, 3, inputVeloAC_char);
    lcd.print(inputVeloAC_char);
}
void lcdPrint_modeA_acceleration()
{
    lcdPart_clearScreen();
    lcd.setCursor(0, 0);
    lcd.draw(single_outlineFrame, sizeof(single_outlineFrame) / sizeof(single_outlineFrame[0]), true);
    lcd.setCursor(2, 0);
    lcd.print("ACC (m/s^2)");
    lcd.setCursor(60, 0);
    lcd.print("4/4");

    lcd.setCursor(2, 2);
    lcd.print("ab");
    lcd.setCursor(20, 2);
    char inputAccA_char[5];
    dtostrf(acceleration_AB_glb, 4, 3, inputAccA_char);
    lcd.print(inputAccA_char);

    lcd.setCursor(2, 3);
    lcd.print("bc");
    lcd.setCursor(20, 3);
    char inputAccB_char[5];
    dtostrf(acceleration_BC_glb, 4, 3, inputAccB_char);
    lcd.print(inputAccB_char);

    lcd.setCursor(2, 4);
    lcd.print("ac");
    lcd.setCursor(20, 4);
    char inputAccC_char[5];
    dtostrf(acceleration_AC_glb, 4, 3, inputAccC_char);
    lcd.print(inputAccC_char);
}
//--------------------------------------------- LCD - MODE B -------------------------------------------
void lcdPrint_modeB_timeA()
{
    lcdPart_clearScreen();
    //........................................................................................
    // lcdPart__cursorToZero(); //  lcd.setCursor(0,0);
    lcd.draw(basic_outlineFrame, sizeof(basic_outlineFrame) / sizeof(basic_outlineFrame[0]), true);
    lcd.setCursor(2, 0);
    lcd.print("Time A(s)");
    lcd.setCursor(60, 0);
    lcd.print("1/6");
    //........................................................................................
    lcd.setCursor(2, 2);
    lcd.print("ai");
    lcd.setCursor(14, 2);
    char inputTimeA_char[4];
    dtostrf(time_Ai_glb, 4, 3, inputTimeA_char);
    lcd.print(inputTimeA_char);
    //........................................................................................
    lcd.setCursor(44, 2);
    lcd.print("af");
    lcd.setCursor(56, 2);
    char inputTimeB_char[4];
    dtostrf(time_Af_glb, 4, 3, inputTimeB_char);
    lcd.print(inputTimeB_char);
    //........................................................................................
    lcd.setCursor(2, 3);
    lcd.print("bi");
    lcd.setCursor(14, 3);
    char inputTimeC_char[4];
    dtostrf(time_Bi_glb, 4, 3, inputTimeC_char);
    lcd.print(inputTimeC_char);
    //........................................................................................
    lcd.setCursor(44, 3);
    lcd.print("bf");
    lcd.setCursor(56, 3);
    char inputTimeD_char[4];
    dtostrf(time_Bf_glb, 4, 3, inputTimeD_char);
    lcd.print(inputTimeD_char);
}
void lcdPrint_modeB_timeB()
{
    lcdPart_clearScreen();
    //........................................................................................
    // lcdPart__cursorToZero(); //  lcd.setCursor(0,0);
    lcd.draw(basic_outlineFrame, sizeof(basic_outlineFrame) / sizeof(basic_outlineFrame[0]), true);
    lcd.setCursor(2, 0);
    lcd.print("Time B(s)");
    lcd.setCursor(60, 0);
    lcd.print("2/6");
    //........................................................................................
    lcd.setCursor(2, 2);
    lcd.print("ci");
    lcd.setCursor(14, 2);
    char inputTimeA_char[4];
    dtostrf(time_Ci_glb, 4, 3, inputTimeA_char);
    lcd.print(inputTimeA_char);
    //........................................................................................
    lcd.setCursor(44, 2);
    lcd.print("cf");
    lcd.setCursor(56, 2);
    char inputTimeB_char[4];
    dtostrf(time_Cf_glb, 4, 3, inputTimeB_char);
    lcd.print(inputTimeB_char);

    lcd.setCursor(2, 3);
    lcd.print("di");
    lcd.setCursor(14, 3);
    char inputTimeC_char[4];
    dtostrf(time_Di_glb, 4, 3, inputTimeC_char);
    lcd.print(inputTimeC_char);

    lcd.setCursor(44, 3);
    lcd.print("df");
    lcd.setCursor(56, 3);
    char inputTimeD_char[4];
    dtostrf(time_Df_glb, 4, 3, inputTimeD_char);
    lcd.print(inputTimeD_char);
}
void lcdPrint_modeB_periodA()
{
    lcdPart_clearScreen();
    // lcdPart__cursorToZero(); //  lcd.setCursor(0,0);
    lcd.draw(basic_outlineFrame, sizeof(basic_outlineFrame) / sizeof(basic_outlineFrame[0]), true);
    lcd.setCursor(2, 0);
    lcd.print("Period A(s)");
    lcd.setCursor(60, 0);
    lcd.print("3/6");

    lcd.setCursor(2, 2);
    lcd.print("A");
    lcd.setCursor(14, 2);
    char inputPeriodA_char[4];
    dtostrf(period_A_glb, 4, 3, inputPeriodA_char);
    lcd.print(inputPeriodA_char);
    // second number

    lcd.setCursor(2, 3);
    lcd.print("B");
    lcd.setCursor(14, 3);
    char inputPeriodB_char[4];
    dtostrf(period_B_glb, 4, 3, inputPeriodB_char);
    lcd.print(inputPeriodB_char);

    // lcd.setCursor(2, 4);
    lcd.setCursor(44, 2);
    lcd.print("C");
    lcd.setCursor(56, 2);
    // lcd.setCursor(14, 4);
    char inputPeriodC_char[4];
    dtostrf(period_C_glb, 4, 3, inputPeriodC_char);
    lcd.print(inputPeriodC_char);

    lcd.setCursor(44, 3);
    lcd.print("D");
    lcd.setCursor(56, 3);
    char inputPeriodD_char[4];
    dtostrf(period_D_glb, 4, 3, inputPeriodD_char);
    lcd.print(inputPeriodD_char);
}
void lcdPrint_modeB_periodB()
{
    lcd.clear();
    lcd.draw(single_outlineFrame, sizeof(single_outlineFrame) / sizeof(single_outlineFrame[0]), true);
    lcd.setCursor(2, 0);
    lcd.print("Period B(s)");
    lcd.setCursor(60, 0);
    lcd.print("4/6");

    lcd.setCursor(2, 2);
    lcd.print("ab");
    lcd.setCursor(20, 2);
    char inputPeriodAB_char[5];
    dtostrf(period_AB_glb, 4, 3, inputPeriodAB_char);
    lcd.print(inputPeriodAB_char);

    lcd.setCursor(2, 3);
    lcd.print("bc");
    lcd.setCursor(20, 3);
    char inputPeriodBC_char[5];
    dtostrf(period_BC_glb, 4, 3, inputPeriodBC_char);
    lcd.print(inputPeriodBC_char);

    lcd.setCursor(2, 4);
    lcd.print("cd");
    lcd.setCursor(20, 4);
    char inputPeriodCD_char[5];
    dtostrf(period_CD_glb, 4, 3, inputPeriodCD_char);
    lcd.print(inputPeriodCD_char);
}
void lcdPrint_modeB_velocity()
{
    lcdPart_clearScreen();
    lcd.draw(basic_outlineFrame, sizeof(basic_outlineFrame) / sizeof(basic_outlineFrame[0]), true);

    lcd.setCursor(2, 0);
    lcd.print("VLCTY (m/s)");
    lcd.setCursor(60, 0);
    lcd.print("5/6");

    lcd.setCursor(2, 2);
    lcd.print("a");
    lcd.setCursor(14, 2);
    char inputVeloA_char[4];
    dtostrf(velocity_A_glb, 4, 3, inputVeloA_char);
    lcd.print(inputVeloA_char);

    lcd.setCursor(2, 3);
    lcd.print("b");
    lcd.setCursor(14, 3);
    char inputVeloB_char[4];
    dtostrf(velocity_B_glb, 4, 3, inputVeloB_char);
    lcd.print(inputVeloB_char);

    lcd.setCursor(44, 2);
    lcd.print("c");
    lcd.setCursor(56, 2);
    char inputVeloC_char[4];
    dtostrf(velocity_C_glb, 4, 3, inputVeloC_char);
    lcd.print(inputVeloC_char);

    lcd.setCursor(44, 3);
    lcd.print("d");
    lcd.setCursor(56, 3);
    char inputVeloD_char[4];
    dtostrf(velocity_D_glb, 4, 3, inputVeloD_char);
    lcd.print(inputVeloD_char);
}
void lcdPrint_modeB_acceleration()
{
    lcdPart_clearScreen();
    lcd.setCursor(0, 0);
    lcd.draw(single_outlineFrame, sizeof(single_outlineFrame) / sizeof(single_outlineFrame[0]), true);
    lcd.setCursor(0, 0);
    lcd.print("ACC (m/s^2)");
      lcd.setCursor(60, 0);
    lcd.print("6/6");

    lcd.setCursor(20, 2);
    lcd.print("STILL");
    lcd.setCursor(20, 3);
    lcd.print("UNDER");
    lcd.setCursor(20, 4);
    lcd.print("DEVELOPMENT");
    // lcd.setCursor(60, 0);
    // lcd.print("4/4");
}

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//           READY       READY         READY           READY         READY         READY
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void modeA_timerPrep(int flash_period_start, int flash_period_norm) // send waiting screen to the LCD.
{
    laser_switch(false, false, false, false);
    countTest++;
    // lcdSend_buttonAFrame();
    delay(flash_period_start);
    lcd_sendTrasitionScreen((char *)"FORWARDS", (char *)"A->B->C", (char *)"loading...");
    for (int x = 0; x < 3; x++)
    {
        laser_switch(true, false, false, false);
        delay(flash_period_start);
        laser_switch(false, true, false, false);
        delay(flash_period_norm);
        laser_switch(false, false, true, false);
        delay(flash_period_norm);
    }

    laser_switch(true, true, true, true);
    lcd_sendTrasitionScreen((char *)"FORWARDS", (char *)"A->B->C", (char *)"<< READY >>");
    modeA_serialPrint(true);
}
void modeB_timerReady(int flash_period_start, int flash_period_norm) // send waiting screen to the LCD.
{
    laser_switch(false, false, false, false);
    countTest++;
    // lcdSend_buttonAFrame();
    delay(flash_period_start);
    lcd_sendTrasitionScreen((char *)"MOMENTUM", (char *)"A->C->C->A", (char *)"loading...");
    for (int x = 0; x < 3; x++) // just a little thing to flash LASERS in a routine.
    {
        laser_switch(true, false, true, false);
        delay(flash_period_start);
        laser_switch(false, true, false, false);
        delay(flash_period_norm);
        laser_switch(true, false, true, false);
        delay(flash_period_start);
        laser_switch(false, true, false, false);
        delay(flash_period_norm);
        laser_switch(true, false, true, false);
        delay(flash_period_start);
        laser_switch(false, true, false, false);
        delay(flash_period_norm);
    }

    laser_switch(true, false, true, true); // sets the lasers for how they will be used in trial
    lcd.clear(false);
    lcd_sendTrasitionScreen((char *)"MOMENTUM", (char *)"A->C->C->A", (char *)"<< READY >>");
    modeB_serialPrint(true);
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//            BTTN        BTTN          BTTN            BTTN          BTTN          BTTN
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
int getMenuMode(bool button_slctRange_bool) // if false buttons on arduino
{
    int menuModeState = 0;
    if (button_slctRange_bool == false) // checks the buttons on the arduino
    {
        int inputA_anlg = analogRead(A0);
        int inputB_anlg = analogRead(A1);
        int inputC_anlg = analogRead(A2);
        if (inputA_anlg > buttonThresholdHigh)
        {
            menuModeState = 7;
        }
        else if (inputB_anlg > buttonThresholdHigh)
        {
            menuModeState = 6;
        }
        else if (inputC_anlg > buttonThresholdHigh)
        {
            menuModeState = 5;
        }
    }
    if (button_slctRange_bool == true)
    {
        int inputD_anlg = analogRead(A4);
        int inputE_anlg = analogRead(A5);
        // right button on LCD (if 1 then red button high && if 2 the little button high)
        if (inputD_anlg > buttonThresholdHigh)
        {
            menuModeState = 1;
        }
        else if (inputD_anlg < buttonThresholdHigh && inputD_anlg > buttonThresholdLow)
        {
            menuModeState = 3;
        }
        else if (inputE_anlg > buttonThresholdHigh)
        {
            menuModeState = 2;
        }
        else if (inputE_anlg < buttonThresholdHigh && inputE_anlg > buttonThresholdLow)
        {
            menuModeState = 4;
        }
    }
    return menuModeState;
}

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//            CALC        CALC          CALC            CALC          CALC          CALC
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void calc_modeA_time(bool lcdSend_bool, bool serialPrint_bool)
{
    //time when trolley gets to LASER A
    time_Ai_glb = 0.0000;
    //........................................................................................
    // time when trolley leaves LASER A
    period_Af_lng = time_Af_lng - time_Ai_lng;
    time_Af_glb = (float)period_Af_lng / million_flt;
    //........................................................................................
    // time when trolley gets to LASER B
    period_Bi_lng = time_Bi_lng - time_Ai_lng;
    time_Bi_glb = (float)period_Bi_lng / million_flt;
    //........................................................................................
    // time when trolley leaves LASER B
    period_Bf_lng = time_Bf_lng - time_Ai_lng;
    time_Bf_glb = (float)period_Bf_lng / million_flt;
    //........................................................................................
    // time when trolley gets to LASER C
    period_Ci_lng = time_Ci_lng - time_Ai_lng;
    time_Ci_glb = (float)period_Ci_lng / million_flt;
    //........................................................................................
    // time when trolley passesLASER C
    period_Cf_lng = time_Cf_lng - time_Ai_lng;
    time_Cf_glb = (float)period_Cf_lng / million_flt;

    if (lcdSend_bool == true)
    {
        // lcdSend_forwardsFrameTime(); // send menu frame to screen
        lcdPrint_modeA_time();
    }
    //........................................................................................

    if (serialPrint_bool == true)
    {
        serialPrint_modeA_time();
    }
    //........................................................................................
}
void calc_modeA_period(bool lcdSend_bool, bool serialPrint_bool)
{
    // timer of dist_Trolley passing LASER A
    time_Ad_lng = time_Af_lng - time_Ai_lng;
    period_A_glb = (float)time_Ad_lng / million_flt;
    // timer of trolley passing LASER B
    time_Bd_lng = time_Bf_lng - time_Bi_lng;
    period_B_glb = (float)time_Bd_lng / million_flt;
    // timer of trolley passing LASER C
    time_Cd_lng = time_Cf_lng - time_Ci_lng;
    period_C_glb = (float)time_Cd_lng / million_flt;
    // timer of trolley passing LASER A and B
    time_ABi_lng = (time_Af_lng + time_Ai_lng) / 2; // when trolley is half way through A
    time_ABf_lng = (time_Bf_lng + time_Bi_lng) / 2; // when trolley is half way through B
    time_ABd_lng = (time_ABf_lng - time_ABi_lng);   // difference between A and B
    period_AB_glb = (float)time_ABd_lng / million_flt;
    // timer of trolley passing LASER B and C
    time_BCi_lng = (time_Bf_lng + time_Bi_lng) / 2; // when trolley is half way through A
    time_BCf_lng = (time_Cf_lng + time_Ci_lng) / 2; // when trolley is half way through B
    time_BCd_lng = (time_BCf_lng - time_BCi_lng);   // difference between A and B
    period_BC_glb = (float)time_BCd_lng / million_flt;
    // timer of trolley passing LASER A and C
    time_ACi_lng = (time_Af_lng + time_Ai_lng) / 2; // when trolley is half way through A
    time_ACf_lng = (time_Cf_lng + time_Ci_lng) / 2; // when trolley is half way through B
    time_ACd_lng = (time_ACf_lng - time_ACi_lng);   // difference between A and B
    period_AC_glb = (float)time_ACd_lng / million_flt;
    //........................................................................................
    if (lcdSend_bool == true)
    {
        // lcdSend_forwardsFramePeriod(); // send menu frame to screen
        lcdPrint_modeA_period();
    }
    //........................................................................................

    if (serialPrint_bool == true)
    {
        serialPrint_modeA_period();
    }
    //........................................................................................
}
void calc_modeA_velocity(bool lcdSend_bool, bool serialPrint_bool)
{

    // velocity of cart from laser A
    // time_Ad_lng = time_Af_lng - time_Ai_lng;
    // period_A_glb = (float)time_Ad_lng / million_flt;
    float velocity_A = dist_Trolley / period_A_glb;
    velocity_A_glb = velocity_A;

    // Velocity from Cart crossing laser B
    // time_Bd_lng = time_Bf_lng - time_Bi_lng;
    // period_B_glb = (float)time_Bd_lng / million_flt;
    float velocity_B = dist_Trolley / period_B_glb;
    velocity_B_glb = velocity_B;

    // Velocity from Cart crossing laser C
    // time_Cd_lng = time_Cf_lng - time_Ci_lng;
    // period_C_glb = (float)time_Cd_lng / million_flt;
    float velocity_C = dist_Trolley / period_C_glb;
    velocity_C_glb = velocity_C;

    // Velocity between laser A and Laser B
    float distanceAB = ((velocity_A_glb + velocity_B_glb) / 2) * period_AB_glb;
    dist_AB_glb = distanceAB;
    float velocity_AB = distanceAB / period_AB_glb;
    velocity_AB_glb = velocity_AB;

    // Velocity between laser B and Laser C
    float distanceBC = ((velocity_B_glb + velocity_C_glb) / 2) * period_BC_glb;
    dist_BC_glb = distanceBC;
    float velocity_BC = distanceBC / period_BC_glb;
    velocity_BC_glb = velocity_BC;

    // Velocity between laser A and Laser C
    float distanceAC = ((velocity_A_glb + velocity_C_glb) / 2) * period_AC_glb;
    dist_AC_glb = distanceAC;
    float velocity_AC = distanceAC / period_AC_glb;
    velocity_AC_glb = velocity_AC;

    // lcd print out for velocity
    if (lcdSend_bool == true) // page one print
    {
        lcdPrint_modeA_velocity();
    }
    //........................................................................................

    if (serialPrint_bool == true)
    {
        serialPrint_modeA_velocity();
    }
}
void calc_modeA_acceleration(bool lcdSend_bool, bool serialPrint_bool)
{
    // acceleration of Simple A and B
    float time_AB_flt = float(time_ABd_lng) / million_flt;
    float time_BC_flt = float(time_BCd_lng) / million_flt;
    float time_AC_flt = float(time_ACd_lng) / million_flt;

    acceleration_AB_glb = (velocity_B_glb - velocity_A_glb) / time_AB_flt;
    acceleration_BC_glb = (velocity_C_glb - velocity_B_glb) / time_BC_flt;
    acceleration_AC_glb = (velocity_C_glb - velocity_A_glb) / time_AC_flt;
    // LCD print out for acceleration
    if (lcdSend_bool == true) // page one print
    {
        lcdPrint_modeA_acceleration();
    }
    // Serial Print out for acceleration
    if (serialPrint_bool == true)
    {
        serialPrint_modeA_acceleration();
    }
}
//--------------------------------------------- CALC - MODE B -------------------------------------------
void calc_modeB_timeA(bool lcdSend_bool, bool serialPrint_bool)
{
    //period when trolley gets to LASER A
    time_Ai_glb = 0.0000;
    //........................................................................................
    // time when trolley leaves LASER A
    period_Af_lng = time_Af_lng - time_Ai_lng;
    time_Af_glb = (float)period_Af_lng / million_flt;
    //........................................................................................
    // time when trolley gets to LASER B
    period_Bi_lng = time_Bi_lng - time_Ai_lng;
    time_Bi_glb = (float)period_Bi_lng / million_flt;
    //........................................................................................
    // time when trolley leaves LASER B
    period_Bf_lng = time_Bf_lng - time_Ai_lng;
    time_Bf_glb = (float)period_Bf_lng / million_flt;
    //........................................................................................
    if (lcdSend_bool == true)
    {
        // lcdSend_forwardsFrameTime(); // send menu frame to screen
        lcdPrint_modeB_timeA();
    }
    //........................................................................................

    if (serialPrint_bool == true)
    {
        serialPrint_modeB_timeA();
    }
}
void calc_modeB_timeB(bool lcdSend_bool, bool serialPrint_bool)
{
    //period when trolley gets to LASER A
    time_Ai_glb = 0.0000;
    //........................................................................................
    // time when trolley gets to LASER C
    period_Ci_lng = time_Ci_lng - time_Ai_lng;
    time_Ci_glb = (float)period_Ci_lng / million_flt;
    //........................................................................................
    // time when trolley passesLASER C
    period_Cf_lng = time_Cf_lng - time_Ai_lng;
    time_Cf_glb = (float)period_Cf_lng / million_flt;
    //........................................................................................
    // time when trolley gets to LASER A
    period_Di_lng = time_Di_lng - time_Ai_lng;
    time_Di_glb = (float)period_Di_lng / million_flt;
    //........................................................................................
    // time when trolley passesLASER A
    period_Df_lng = time_Df_lng - time_Ai_lng;
    time_Df_glb = (float)period_Df_lng / million_flt;

    if (lcdSend_bool == true)
    {
        // lcdSend_forwardsFrameTime(); // send menu frame to screen
        lcdPrint_modeB_timeB();
    }
    //........................................................................................

    if (serialPrint_bool == true)
    {
        serialPrint_modeB_timeB();
    }
    //........................................................................................
}
void calc_modeB_periodA(bool lcdSend_bool, bool serialPrint_bool)
{

    // timer of dist_Trolley passing LASER A
    time_Ad_lng = time_Af_lng - time_Ai_lng;
    period_A_glb = (float)time_Ad_lng / million_flt;
    // timer of trolley passing LASER B
    time_Bd_lng = time_Bf_lng - time_Bi_lng;
    period_B_glb = (float)time_Bd_lng / million_flt;

    // timer of trolley passing LASER C
    time_Cd_lng = time_Cf_lng - time_Ci_lng;
    period_C_glb = (float)time_Cd_lng / million_flt;

    // timer of trolley passing LASER D
    time_Dd_lng = time_Df_lng - time_Di_lng;
    period_D_glb = (float)time_Dd_lng / million_flt;

    //........................................................................................
    if (lcdSend_bool == true)
    {
        // lcdSend_forwardsFramePeriod(); // send menu frame to screen
        lcdPrint_modeB_periodA();
    }
    //........................................................................................

    if (serialPrint_bool == true)
    {
        serialPrint_modeB_periodA();
    }
    //........................................................................................
}
void calc_modeB_periodB(bool lcdSend_bool, bool serialPrint_bool)
{

    time_ABi_lng = (time_Af_lng + time_Ai_lng) / 2; // when trolley is half way through A
    time_ABf_lng = (time_Bf_lng + time_Bi_lng) / 2; // when trolley is half way through B
    time_ABd_lng = (time_ABf_lng - time_ABi_lng);   // difference between A and B
    period_AB_glb = (float)time_ABd_lng / million_flt;
    // timer of trolley passing LASER B and C
    time_BCi_lng = (time_Bf_lng + time_Bi_lng) / 2; // when trolley is half way through A
    time_BCf_lng = (time_Cf_lng + time_Ci_lng) / 2; // when trolley is half way through B
    time_BCd_lng = (time_BCf_lng - time_BCi_lng);   // difference between A and B
    period_BC_glb = (float)time_BCd_lng / million_flt;
    // // codeChecker();
    // timer of trolley passing LASER A and C
    time_CDi_lng = (time_Cf_lng + time_Ci_lng) / 2; // when trolley is half way through A
    time_CDf_lng = (time_Df_lng + time_Di_lng) / 2; // when trolley is half way through B
    time_CDd_lng = (time_CDf_lng - time_CDi_lng);   // difference between A and B
    period_CD_glb = (float)time_CDd_lng / million_flt;
    // // codeChecker();

    //........................................................................................
    if (lcdSend_bool == true)
    {
        lcdPrint_modeB_periodB();
    }
    //........................................................................................

    if (serialPrint_bool == true)
    {
        serialPrint_modeB_periodB();
    }
    //........................................................................................
}
void calc_modeB_velocity(bool lcdSend_bool, bool serialPrint_bool)
{
    // velocity of cart from laser A
    time_Ad_lng = time_Af_lng - time_Ai_lng;
    period_A_glb = (float)time_Ad_lng / million_flt;
    float velocity_A = dist_Trolley / period_A_glb;
    velocity_A_glb = velocity_A;

    // Velocity from Cart crossing laser C
    time_Bd_lng = time_Bf_lng - time_Bi_lng;
    period_B_glb = (float)time_Bd_lng / million_flt;
    float velocity_B = dist_Trolley / period_B_glb;
    velocity_B_glb = velocity_B;

    // Velocity from Cart crossing laser C again
    time_Cd_lng = time_Cf_lng - time_Ci_lng;
    period_C_glb = (float)time_Cd_lng / million_flt;
    float velocity_C = dist_Trolley / period_C_glb;
    velocity_C_glb = velocity_C;

    // Velocity from Cart crossing laser A again
    time_Dd_lng = time_Df_lng - time_Di_lng;
    period_D_glb = (float)time_Dd_lng / million_flt;
    float velocity_D = dist_Trolley / period_D_glb;
    velocity_D_glb = velocity_D;

    // lcd print out for velocity
    if (lcdSend_bool == true) // page one print
    {
        lcdPrint_modeB_velocity();
    }
    //........................................................................................

    if (serialPrint_bool == true)
    {
        serialPrint_modeB_velocity();
    }
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void calc_modeB_acceleration(bool lcdSend_bool, bool serialPrint_bool)
{
    // acceleration of Simple A and B
    // float time_AB_flt = float(time_ABd_lng) / million_flt;
    // float time_BC_flt = float(time_BCd_lng) / million_flt;
    // float time_AC_flt = float(time_ACd_lng) / million_flt;

    // acceleration_AB_glb = (velocity_B_glb - velocity_A_glb) / time_AB_flt;
    // acceleration_BC_glb = (velocity_C_glb - velocity_B_glb) / time_BC_flt;
    // acceleration_AC_glb = (velocity_C_glb - velocity_A_glb) / time_AC_flt;
    // LCD print out for acceleration
    if (lcdSend_bool == true) // page one print
    {
        lcdPrint_modeB_acceleration();
    }
    // Serial Print out for acceleration
    if (serialPrint_bool == true)
    {
        serialPrint_modeB_acceleration();
    }
}
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//            SETUP        SETUP          SETUP            SETUP          SETUP          SETUP
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void setup()
{

    pinMode(input_lghtGate_A, INPUT);
    pinMode(input_lghtGate_B, INPUT);
    pinMode(input_lghtGate_C, INPUT);

    pinMode(laserPinA, OUTPUT);
    pinMode(laserPinB, OUTPUT);
    pinMode(laserPinC, OUTPUT);

    pinMode(lcdPinLED, OUTPUT);

    // Initialize Serial Comms
    Serial.begin(9600);

    serialStartUp(true); // bool - switch serial on and off

    // Initialize LCD screen
    lcd.begin();
    lcd.setContrast(48);

    lcdLight_switch(true);

    lcdSend_startUpRoutine(); //send start up routine to the lcd
}
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//            LOOP        LOOP          LOOP            LOOP          LOOP          LOOP
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void loop()
{

    Serial.print("|--- <<<< <<<< New Trial Started >>>> >>>> ---|\n");
    init_time_variables(false);
    // init_control_variables(false);
    //################################################################################## INIT VARIABLES
    cntrl_mainMenu = 0;      // variable used to control moving though the main menu. [resets to zero each time]
    state_printOnceA = true; // when <true> next instance will allow it to print  [lcdSend_menuFrame();]
    cntrl_functionMode = 0;
    state_printOnceB = true;
    state_trackPos = 0;
    state_printOnceC = true;
    state_printOnceD = true;
    state_rsltMenu = 0; // used when turing pages in the menu.
    button_mainMenu = 0;
    button_rsltMenu = 0;

    //################################################################################## MENU MODE 0
    while (cntrl_mainMenu == 0) // when opening the main menu
    {
        if (state_printOnceA == true) // prints the menu once every loop
        {
            lcdSend_menuFrame();      // send menu frame to screen
            state_printOnceA = false; // prevents page being printed prpeatedly.
        }

        button_mainMenu = getMenuMode(true); // starts POLLING the buttons below the screen
        // delay(wait_twoFifty);
        if (button_mainMenu == 1) // exit this while loop <main menu>
        {
            cntrl_mainMenu = 1; // button A has been pressed this is going to send you to the first control group
            break;
        }
        if (button_mainMenu == 2) // exit this while loop <main menu> go to outer two lasers.
        {
            cntrl_mainMenu = 2; // button C has been pressed this is going to send you to the second control group
            break;
        }
    }
    //################################################################################## MENU MODE 1
    //RUNS while mode is "1" or testing forward
    while (cntrl_mainMenu == 1) // loop for checking forward motion
    {
        //********************************************************************* function mode *****
        while (cntrl_functionMode == 0) // polling lasers to check forward motion
        {
            if (state_printOnceB == true) // prints the menu once every loop
            {
                modeA_timerPrep(wait_twoFifty, wait_hundred);
                state_trackPos = 0;       // gets ready to poll  lasers
                state_printOnceB = false; // deactovated loop
            }
            //********************************************************************* LASER A initial *****
            // Serial.print("++ lsr A | LOW |  ++\n");
            while (state_trackPos == 0) // Fisrt reading at LSR A
            {
                if (digitalRead(input_lghtGate_A) == 0)
                {
                    time_Ai_lng = micros(); // set variable to the current microSecond on the clock.
                    Serial.print("++ lsr A | LOW |  ++\n");
                    state_trackPos = 1; // set sub-sub routine controller
                    break;
                }
            }
            //********************************************************************* LASER A final *****
            while (state_trackPos == 1) // Second reading at LSR A
            {
                if (digitalRead(input_lghtGate_A) == 1) //when sensor A goes high
                {
                    time_Af_lng = micros(); // set variable to the current microSecond on the clock.
                    Serial.print("++ lsr A | HGH |  ++\n");
                    state_trackPos = 2;
                    break;
                }
            }
            //********************************************************************* LASER B initial *****
            while (state_trackPos == 2) // THREE
            {
                if (digitalRead(input_lghtGate_B) == 0)
                {
                    time_Bi_lng = micros(); // set variable to the current microSecond on the clock.
                    Serial.print("++ lsr B | LOW |  ++\n");
                    state_trackPos = 3;
                    break;
                }
            }
            //********************************************************************* FOUTH TIME *****
            while (state_trackPos == 3) // FOUR
            {
                if (digitalRead(input_lghtGate_B) == 1) //when sensor A goes high
                {
                    time_Bf_lng = micros(); // set variable to the current microSecond on the clock.
                    Serial.print("++ lsr B | HGH |  ++\n");
                    state_trackPos = 4;
                    break;
                }
            }
            //****************************************************************************************
            while (state_trackPos == 4) // FIVE
            {
                if (digitalRead(input_lghtGate_C) == 0)
                {
                    time_Ci_lng = micros(); // set variable to the current microSecond on the clock.
                    Serial.print("++ lsr C | LOW |  ++\n");
                    state_trackPos = 5;
                    break;
                }
            }
            //****************************************************************************************
            while (state_trackPos == 5) // SIX
            {
                if (digitalRead(input_lghtGate_C) == 1) //when sensor A goes high
                {
                    time_Cf_lng = micros(); // set variable to the current microSecond on the clock.
                    Serial.print("++ lsr C | HGH |  ++\n");
                    // Serial.print("++ Lasers All Read ++\n");
                    state_trackPos = 6;     // exit the loop
                    cntrl_functionMode = 1; // exit this loop
                }
            }
        }
        //****************************************************************************************
        while (cntrl_functionMode == 1) // going into forward results menu
        {
            // start of conviluted printing menu
            if (state_printOnceC == true) // prints once to keep track.
            {
                lcd_sendTrasitionScreen((char *)"TEST", (char *)"FINISHED", (char *)"got the data");
                delay(wait_fullSec);
                // serialPrint_control_variables();
                calc_modeA_time(true, true);          //(<send LCD> <print SRL>)
                calc_modeA_period(false, true);       //(<send LCD> <print SRL>)
                calc_modeA_velocity(false, true);     //(<send LCD> <print SRL>)
                calc_modeA_acceleration(false, true); //(<send LCD> <print SRL>)
                                                      // state_trackPos = 7;
                                                      // Serial.println("done printing");
                Serial.println("----------- done printing info ------------");
                laser_switch(false, false, false, true);
                state_printOnceC = false;
            }

            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            //  RESULT MENU
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            while (state_rsltMenu == 0) // waits for a button to be pressed after sending data to screen
            {
                button_rsltMenu = getMenuMode(true); // tests if a button
                if (button_rsltMenu != 0)
                {
                    state_rsltMenu = 1; // preps the variab
                }
            }
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            while (state_rsltMenu == 1)
            {
                switch (button_rsltMenu)
                {
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 1:
                    --prnt_rsltPage;        // negative increments to go to previous page
                    if (prnt_rsltPage == 0) // sends resutls back.
                    {
                        prnt_rsltPage = 4;
                    }
                    state_rsltMenu = 2;
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -   -
                case 2:
                    ++prnt_rsltPage;        //  positive increments to go to next page
                    if (prnt_rsltPage == 5) // if prnt_rsltPage is at max set back to min
                    {
                        prnt_rsltPage = 1;
                    }
                    state_rsltMenu = 2;
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 3:
                    state_rsltMenu = 3; // goes to last to exit.
                    button_rsltMenu = 4;
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                default:
                    // state_rsltMenu = 0; // if incorrect button pressed back to polling buttons.
                    // // print_rsltPage = 0;
                    break;
                }
            }

            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            while (state_rsltMenu == 2)
            {
                switch (prnt_rsltPage)
                {
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 1:
                    calc_modeA_time(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[FORWARD TIME SENT TO LCD]");
                    delay(wait_halfSec);
                    state_rsltMenu = 0;
                    break;

                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 2:
                    calc_modeA_period(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[FORWARD PERIOD SENT TO LCD]");
                    delay(wait_halfSec);
                    state_rsltMenu = 0;

                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -    -
                case 3:
                    calc_modeA_velocity(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[FORWARD VELOCITY SENT TO LCD]");
                    delay(wait_halfSec);
                    state_rsltMenu = 0;
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 4:
                    calc_modeA_acceleration(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[FORWARD ACCELERATION SENT TO LCD]");
                    state_rsltMenu = 0;
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                default:
                    state_rsltMenu = 0; // if incorrect button pressed back to polling buttons.
                    break;
                }
            }
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            if (state_rsltMenu == 3) // initise all variables back to start again
            {
                cntrl_mainMenu = 0;
                cntrl_functionMode = 0;
                state_rsltMenu = 4;
            }
        }
    }
    //################################################################################## MENU MODE 2
    //RUNS while mode is "2" or testing forward
    while (cntrl_mainMenu == 2) // main routine control
    {
        while (cntrl_functionMode == 0) //starts polling lasers and shows waiting screen on LCD for outside two
        {
            //*********************************************************************
            if (state_printOnceB == true) // prints the menu once every time this loop in enetered
            {
                // modeA_timerPrep(wait_hundred, wait_hundred);
                modeB_timerReady(wait_twoFifty, wait_hundred);
                state_trackPos = 0;       // gets ready to poll  lasers it will start with LASER A.
                state_printOnceB = false; // deactovated loop
            }
            //*********************************************************************
            while (state_trackPos == 0) // FIRST reading at LSR A
            {
                // codeChecker(1);
                if (digitalRead(input_lghtGate_A) == 0) //polling LASER A until it goes LOW
                {
                    // codeChecker(2);
                    time_Ai_lng = micros(); // initial timwe laser A
                    Serial.print("++ lsr A | LOW |  ++\n");
                    state_trackPos = 1; // set variable to prep for next polling state
                    break;
                }
            }
            //*********************************************************************
            while (state_trackPos == 1) // SECOND reading at LSR A
            {
                // codeChecker(3);
                if (digitalRead(input_lghtGate_A) == 1) //polling LASER A until it goes HIGH
                {
                    // codeChecker(4);
                    time_Af_lng = micros(); // save current microSeconds to variable
                    Serial.print("++ lsr A | HGH |  ++\n");
                    state_trackPos = 2; // set variable to prep for next polling state
                    break;
                }
            }
            //*********************************************************************
            while (state_trackPos == 2) // THIRD reading at LSR C
            {
                // codeChecker(5);
                if (digitalRead(input_lghtGate_C) == 0) //polling LASER B until it goes LOW
                {
                    // codeChecker(6);
                    time_Bi_lng = micros(); // save current microSeconds to variable
                    Serial.print("++ lsr C | LOW |  ++\n");
                    state_trackPos = 3; // set variable to prep for next polling state
                    break;
                }
            }
            //*********************************************************************
            while (state_trackPos == 3) // FOURTH reading at LSR C
            {
                // codeChecker(7);
                if (digitalRead(input_lghtGate_C) == 1) //polling LASER B until it goes HIGH
                {
                    // codeChecker(8);
                    time_Bf_lng = micros(); // save current microSeconds to variable
                    // codeChecker(9);
                    Serial.print("++ lsr C | HGH |  ++\n");
                    state_trackPos = 4; // set variable to prep for next polling state
                    // codeChecker(10);
                    break;
                }
            }
            // codeChecker(11);
            //*********************************************************************
            while (state_trackPos == 4) // FIFTH reading at LSR C
            {
                // codeChecker(12);
                if (digitalRead(input_lghtGate_C) == 0) //polling LASER C until it goes LOW
                {
                    // codeChecker(13);
                    time_Ci_lng = micros(); // save current microSeconds to variable
                    Serial.print("++ lsr C | LOW |  ++\n");
                    state_trackPos = 5; // set variable to prep for next polling state
                    break;
                }
            }
            //*********************************************************************
            while (state_trackPos == 5) // SIXTH reading at LSR C
            {
                // codeChecker(10);
                if (digitalRead(input_lghtGate_C) == 1) //polling LASER C until it goes HIGH
                {
                    // codeChecker(11);
                    time_Cf_lng = micros(); // save current microSeconds to variable
                    Serial.print("++ lsr C | HGH |  ++\n");
                    state_trackPos = 6; // set variable to prep for next polling state
                    break;
                }
            }
            //*********************************************************************
            while (state_trackPos == 6) // SEVENTH reading at LSR A
            {
                // codeChecker(12);
                if (digitalRead(input_lghtGate_A) == 0) //polling LASER A until it goes LOW
                {
                    // codeChecker(13);
                    time_Di_lng = micros(); // save current microSeconds to variable
                    Serial.print("++ lsr A | LOW |  ++\n");
                    state_trackPos = 7; // set variable to prep for next polling state
                    break;
                }
            }
            //********************************************************************* LAST TIME IN TRIAL *****
            while (state_trackPos == 7) // EIGHT reading at LSR A
            {
                // codeChecker(14);
                if (digitalRead(input_lghtGate_A) == 1) //polling LASER A until it goes HIGH
                {
                    // codeChecker(15);
                    time_Df_lng = micros(); // save current microSeconds to variable
                    Serial.print("++ lsr A | HGH |  ++\n");
                    state_trackPos = 8; // set variable to prep for next polling, will be reset at the <loop INIT>
                    //--------------------------------------------------------------------------------------
                    cntrl_functionMode = 1; // EXIT this loop <> PREP are for  < while (cntrl_functionMode == 1) >
                    //--------------------------------------------------------------------------------------
                    break;
                }
            }
            //*********************************************************************
        }
        //##################################################################################
        while (cntrl_functionMode == 1) // info collected and entered into menu
        {
            // Serial.println("!!!!!!!  CHECK      !!!!!!!!!    [  START  ]");
            // start of conviluted printing menu
            if (state_printOnceC == true) // prints once to keep track.
            {
                lcd_sendTrasitionScreen((char *)"TEST", (char *)"FINISHED", (char *)"got the data");
                // // codeChecker(1);
                delay(wait_fullSec);
                calc_modeB_timeA(true, true);         //(<send LCD> <print SRL>)
                calc_modeB_timeB(false, true);        //(<send LCD> <print SRL>)
                calc_modeB_periodA(false, true);      //(<send LCD> <print SRL>)
                calc_modeB_periodB(false, true);      //(<send LCD> <print SRL>)
                calc_modeB_velocity(false, true);     //(<send LCD> <print SRL>)
                calc_modeB_acceleration(false, true); //(<send LCD> <print SRL>)

                Serial.println("----------- done printing info ------------");
                laser_switch(false, false, false, true);
                state_printOnceC = false;
                state_rsltMenu = 0;
            }

            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            //  RESULT MENU
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            while (state_rsltMenu == 0) // waits for a button to be pressed after sending data to screen
            {
                // Serial.println("checking buttons");
                button_rsltMenu = getMenuMode(true); // tests if a button
                if (button_rsltMenu != 0)
                {
                    state_rsltMenu = 1; // preps the variab
                }
            }
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            while (state_rsltMenu == 1) // takes the button pressed and and ++ or -- to change page
            {
                switch (button_rsltMenu)
                {
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 1:
                    --prnt_rsltPage;        // negative increments to go to previous page
                    if (prnt_rsltPage == 0) // sends resutls back.
                    {
                        prnt_rsltPage = 6;
                    }
                    state_rsltMenu = 2;
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -   -
                case 2:
                    ++prnt_rsltPage;        //  positive increments to go to next page
                    if (prnt_rsltPage == 7) // if prnt_rsltPage is at max set back to min
                    {
                        prnt_rsltPage = 1;
                    }
                    state_rsltMenu = 2;
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 3:
                    state_rsltMenu = 3; // goes to last to exit.
                    button_rsltMenu = 6;
                    lcd.setCursor(10, 2);
                    lcd.clear();
                    lcd.print("RESETTING");
                    delay(wait_fullSec);
                    resetFunc();
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                default:
                    // state_rsltMenu = 0; // if incorrect button pressed back to polling buttons.
                    // // print_rsltPage = 0;
                    break;
                }
            }

            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            while (state_rsltMenu == 2) // prints the page selected and used button_resultMenu to select
            {
                switch (prnt_rsltPage)
                {
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 1:
                    calc_modeB_timeA(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[MOMENTUM TIME A SENT TO LCD]");
                    delay(wait_halfSec);
                    state_rsltMenu = 0; // send the menu back to waiting for a button to be pressed
                    break;

                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 2:
                    calc_modeB_timeB(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[MOMENTUM TIME B SENT TO LCD]");
                    delay(wait_halfSec);
                    state_rsltMenu = 0; // send the menu back to waiting for a button to be pressed

                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -    -
                case 3:
                    calc_modeB_periodA(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[MOMENTUM PERIOD A SENT TO LCD]");
                    delay(wait_halfSec);
                    state_rsltMenu = 0; // send the menu back to waiting for a button to be pressed
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 4:
                    calc_modeB_periodB(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[MOMENTUM PERIOD B SENT TO LCD]");
                    delay(wait_halfSec);
                    state_rsltMenu = 0; // send the menu back to waiting for a button to be pressed
                    break;
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 5:
                    calc_modeB_velocity(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[VELOCITY SENT TO LCD]");
                    delay(wait_halfSec);
                    state_rsltMenu = 0; // send the menu back to waiting for a button to be pressed
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                case 6:
                    calc_modeB_acceleration(true, false); //(<send LCD> <print SRL>)
                    Serial.println("[acceleration SENT TO LCD]");
                    delay(wait_halfSec);
                    state_rsltMenu = 0; // send the menu back to waiting for a button to be pressed
                    // break;
                    break;
                // -    -    -    -    -    -    -    -   -    -    -    -    -    -    -    -    -
                default:
                    state_rsltMenu = 0; // if incorrect button pressed back to polling buttons.
                    break;
                }
            }
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            if (state_rsltMenu == 3) // initise all variables back to start again
            {
                cntrl_mainMenu = 0;
                cntrl_functionMode = 0;
                state_rsltMenu = 4;
            }
        }
    }
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    delay(wait_fullSec);
    lcd.setCursor(10, 2);
    lcd.clear();
    lcd.print("RESETTING");
    delay(wait_fullSec);
    resetFunc();
}
