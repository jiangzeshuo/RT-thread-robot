// Q1 lite Simple Quadruped Robot 3 (Designed by Jason Workshop)
//
// Firmware version 3.0.1
// Last Update: 16 Jan 2020
//
// Parts List:
// 1. TinyPlan97 Servo Control Board
//    Buy on Jason Workshop Robot Store: http://store.jasonworkshop.com
// 2. Screws螺丝
//    M2 x 8mm Self Tap Screw x 20 pcs
//    M2.5 x 8mm Socket Button Screw x 8 pcs
// 3. 3D Print Parts: http://www.thingiverse.com/thing:4149143
//
// Q1 Robot
// Website: http://q1.jasonworkshop.com
// FB page: http://fb.com/Q1.JasonWorkshop
//
// Jason Workshop
// Website: http://jasonworkshop.com
// FB page: http://fb.com/jasonworkshop
//
// TinyPlan97 Servo Control Board
//  ----------------
// |      oooo      |
// | P10        P02 |
// | P13        P05 |
// | P11        P04 |
// | P12        P07 |
// | P17        P03 |
// | P16        P15 |
// |                |
// |                |
// |  USB   Switch  |
//  ----------------
//
// Q1 lite (Top View)
//  -----               -----
// |  5  |             |  1  |
// | P02 |             | P10 |
//  ----- -----   ----- -----
//       |  6  | |  2  |
//       | P04 | | P11 |
//        -----   -----
//       |  7  | |  3  |
//       | P07 | | P12 |
//  ----- -----   ----- -----
// |  8  |             |  4  |
// | P15 |             | P16 |
//  -----               -----
//
// ----------------------------------------------------------------------------------------------------
//
// This Firmware licensed under the Attribution-NonCommercial-ShareAlike 4.0 (CC-BY-NC-SA 4.0)
//
// Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
// You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
//
// ShareAlike: If you remix, transform, or build upon the material,
// you must distribute your contributions under the same license as the original.
//
// License Deed
// http://creativecommons.org/licenses/by-sa/4.0/
//
// ----------------------------------------------------------------------------------------------------

#include <EEPROM.h>
#include <Servo.h>
#include <LRemote.h>



// Custom value
// ----------------------------------------------------------------------------------------------------

String robotName = "Q1 lite 3"; // Robot name

const int enableCalibration = true; // Enable calibration button校准

// ----------------------------------------------------------------------------------------------------



// System value
// ----------------------------------------------------------------------------------------------------

const int numberOfServos = 8; // Number of servos 
const int numberOfACE = 9; // Number of action code elements控制
int servoCal[] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // Servo calibration data校准数据
int servoPos[] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // Servo current position 方向
int servoPrevPrg[] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // Servo previous prg 
int servoPrgPeriod = 20; // 50 ms 
Servo servo[numberOfServos]; // Servo object

int autoMode = 0;

// LinkIt remote interface object
int sliderX;
int sliderWidth;
LRemoteButton button01;
LRemoteButton button02;
LRemoteButton button03;
LRemoteButton button04;
LRemoteButton button05;
LRemoteButton button06;
LRemoteButton button07;
LRemoteButton button08;
LRemoteButton button09;
LRemoteButton button10;
LRemoteButton button11;
LRemoteButton button12;
LRemoteButton button13;
LRemoteButton button14;
LRemoteButton button15;
LRemoteButton buttonZero;
LRemoteButton buttonClear;
LRemoteButton buttonC01a;
LRemoteButton buttonC01b;
LRemoteButton buttonC02a;
LRemoteButton buttonC02b;
LRemoteButton buttonC03a;
LRemoteButton buttonC03b;
LRemoteButton buttonC04a;
LRemoteButton buttonC04b;
LRemoteButton buttonC05a;
LRemoteButton buttonC05b;
LRemoteButton buttonC06a;
LRemoteButton buttonC06b;
LRemoteButton buttonC07a;
LRemoteButton buttonC07b;
LRemoteButton buttonC08a;
LRemoteButton buttonC08b;
LRemoteLabel label01;
LRemoteSlider slider01;
LRemoteSlider slider02;
LRemoteSlider slider03;
LRemoteSlider slider04;
LRemoteSlider slider05;
LRemoteSlider slider06;
LRemoteSlider slider07;
LRemoteSlider slider08;
LRemoteSwitch switch01;

// ----------------------------------------------------------------------------------------------------



// Action code
// ----------------------------------------------------------------------------------------------------

// Servo zero position
int servoAct00 [] PROGMEM =
  // P10, P11, P12, P16, P02, P04, P07, P15
{  135,  45, 135,  45,  45, 135,  45, 135 };
//  1     2   3     4    5   6     7   8
// Zero
int servoPrg00step = 1;
int servoPrg00 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {  135,  45, 135,  45,  45, 135,  45, 135, 1000  }, // zero position
};

// Standby待机
int servoPrg01step = 2;
int servoPrg01 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   90,  90,  90,  90,  90,  90,  90,  90,  200  }, // prep standby
  {  -20,   0,   0,  20,  20,   0,   0, -20,  200  }, // standby
};
//
// Q1 lite (Top View)
//  -----               -----
// |  5  |             |  1  |
// | P02 |             | P10 |
//  ----- -----   ----- -----
//       |  6  | |  2  |
//       | P04 | | P11 |
//        -----   -----
//       |  7  | |  3  |
//       | P07 | | P12 |
//  ----- --4--   ----- -----
// |  8  |             |  4  |
// | P15 |             | P16 |
//  -----               -----
// Forward
int servoPrg02step = 11;
int servoPrg02 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   110,  90,  90, 110, 110,  90,  90,  70,  100 }, // standby
  {   20,   0,   0,   0,   0,   0, -45,  20,  100  }, // leg1,4 up; leg4 fw
  {  -20,   0,   0,   0,   0,   0,   0, -20,  100  }, // leg1,4 dn
  {    0,   0,   0, -20, -20,   0,   0,   0,  100  }, // leg2,3 up
  {    0, -45,  45,   0,   0,   0,  45,   0,  100  }, // leg1,4 bk; leg2 fw
  {    0,   0,   0,  20,  20,   0,   0,   0,  100  }, // leg2,3 dn
  {   20,  45,   0,   0,   0,   0,   0,  20,  100  }, // leg1,4 up; leg1 fw
  {    0,   0, -45,   0,   0,  45,   0,   0,  100  }, // leg2,3 bk
  {  -20,   0,   0,   0,   0,   0,   0, -20,  100  }, // leg1,4 dn
  {    0,   0,   0,   0, -20,   0,   0,   0,  100  }, // leg3 up
  {    0,   0,   0,   0,  20, -45,   0,   0,  100  }, // leg3 fw dn
};

// Backward
int servoPrg03step = 11;
int servoPrg03 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   70,  90,  90, 110, 110,  90,  90,  70,  100  }, // standby
  {   20, -45,   0,   0,   0,   0,   0,  20,  100  }, // leg4,1 up; leg1 fw
  {  -20,   0,   0,   0,   0,   0,   0, -20,  100  }, // leg4,1 dn
  {    0,   0,   0, -20, -20,   0,   0,   0,  100  }, // leg3,2 up
  {    0,  45,   0,   0,   0,  45, -45,   0,  100  }, // leg4,1 bk; leg3 fw
  {    0,   0,   0,  20,  20,   0,   0,   0,  100  }, // leg3,2 dn
  {   20,   0,   0,   0,   0,   0,  45,  20,  100  }, // leg4,1 up; leg4 fw
  {    0,   0,  45,   0,   0, -45,   0,   0,  100  }, // leg3,1 bk
  {  -20,   0,   0,   0,   0,   0,   0, -20,  100  }, // leg4,1 dn
  {    0,   0,   0, -20,   0,   0,   0,   0,  100  }, // leg2 up
  {    0,   0, -45,  20,   0,   0,   0,   0,  100  }, // leg2 fw dn
};

// Move Left
int servoPrg04step = 11;
int servoPrg04 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   70,  90,  90, 110, 110,  90,  90,  70,  100  }, // standby
  {    0,   0, -45, -20, -20,   0,   0,   0,  100  }, // leg3,2 up; leg2 fw
  {    0,   0,   0,  20,  20,   0,   0,   0,  100  }, // leg3,2 dn
  {   20,   0,   0,   0,   0,   0,   0,  20,  100  }, // leg1,4 up
  {    0,  45,  45,   0,   0, -45,   0,   0,  100  }, // leg3,2 bk; leg1 fw
  {  -20,   0,   0,   0,   0,   0,   0, -20,  100  }, // leg1,4 dn
  {    0,   0,   0, -20, -20,  45,   0,   0,  100  }, // leg3,2 up; leg3 fw
  {    0, -45,   0,   0,   0,   0,  45,   0,  100  }, // leg1,4 bk
  {    0,   0,   0,  20,  20,   0,   0,   0,  100  }, // leg3,2 dn
  {    0,   0,   0,   0,   0,   0,   0,  20,  100  }, // leg4 up
  {    0,   0,   0,   0,   0,   0, -45, -20,  100  }, // leg4 fw dn
};

// Move Right
int servoPrg05step = 11;
int servoPrg05 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   70,  90,  90, 110, 110,  90,  90,  70,  100  }, // standby
  {    0,   0,   0, -20, -20, -45,   0,   0,  100  }, // leg2,3 up; leg3 fw
  {    0,   0,   0,  20,  20,   0,   0,   0,  100  }, // leg2,3 dn
  {   20,   0,   0,   0,   0,   0,   0,  20,  100  }, // leg4,1 up
  {    0,   0, -45,   0,   0,  45,  45,   0,  100  }, // leg2,3 bk; leg4 fw
  {  -20,   0,   0,   0,   0,   0,   0, -20,  100  }, // leg4,1 dn
  {    0,   0,  45, -20, -20,   0,   0,   0,  100  }, // leg2,3 up; leg2 fw
  {    0,  45,   0,   0,   0,   0, -45,   0,  100  }, // leg4,1 bk
  {    0,   0,   0,  20,  20,   0,   0,   0,  100  }, // leg2,3 dn
  {   20,   0,   0,   0,   0,   0,   0,   0,  100  }, // leg1 up
  {  -20, -45,   0,   0,   0,   0,   0,   0,  100  }, // leg1 fw dn
};

// Turn left
int servoPrg06step = 8;
int servoPrg06 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   70,  90,  90, 110, 110,  90,  90,  70,  100  }, // standby
  {   20,   0,   0,   0,   0,   0,   0,  20,  100  }, // leg1,4 up
  {    0,  45,   0,   0,   0,   0,  45,   0,  100  }, // leg1,4 turn
  {  -20,   0,   0,   0,   0,   0,   0, -20,  100  }, // leg1,4 dn
  {    0,   0,   0, -20, -20,   0,   0,   0,  100  }, // leg2,3 up
  {    0,   0,  45,   0,   0,  45,   0,   0,  100  }, // leg2,3 turn
  {    0,   0,   0,  20,  20,   0,   0,   0,  100  }, // leg2,3 dn
  {    0, -45, -45,   0,   0, -45, -45,   0,  100  }, // leg1,2,3,4 turn
};

// Turn right
int servoPrg07step = 8;
int servoPrg07 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   70,  90,  90, 110, 110,  90,  90,  70,  100  }, // standby
  {    0,   0,   0, -20, -20,   0,   0,   0,  100  }, // leg2,3 up
  {    0,   0, -45,   0,   0, -45,   0,   0,  100  }, // leg2,3 turn
  {    0,   0,   0,  20,  20,   0,   0,   0,  100  }, // leg2,3 dn
  {   20,   0,   0,   0,   0,   0,   0,  20,  100  }, // leg1,4 up
  {    0, -45,   0,   0,   0,   0, -45,   0,  100  }, // leg1,4 turn
  {  -20,   0,   0,   0,   0,   0,   0, -20,  100  }, // leg1,4 dn
  {    0,  45,  45,   0,   0,  45,  45,   0,  100  }, // leg1,2,3,4 turn
};

// Lie
int servoPrg08step = 1;
int servoPrg08 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {  110,  90,  90,  70,  70,  90,  90, 110,  500  }, // leg1,4 up
};

// Say Hi
int servoPrg09step = 4;
int servoPrg09 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {  120,  90,  90, 110,  60,  90,  90,  70,  200  }, // leg1, 3 down
  {  -50,   0,   0,   0,  50,   0,   0,   0,  200  }, // standby
  {   50,   0,   0,   0, -50,   0,   0,   0,  200  }, // leg1, 3 down
  {  -50,   0,   0,   0,  50,   0,   0,   0,  200  }, // standby
};

// Fighting
int servoPrg10step = 11;
int servoPrg10 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {  120,  90,  90, 110,  60,  90,  90,  70,  200  }, // leg1, 2 down
  {    0, -20, -20,   0,   0, -20, -20,   0,  200  }, // body turn left
  {    0,  40,  40,   0,   0,  40,  40,   0,  200  }, // body turn right
  {    0, -40, -40,   0,   0, -40, -40,   0,  200  }, // body turn left
  {    0,  40,  40,   0,   0,  40,  40,   0,  200  }, // body turn right
  {  -50, -20, -20, -40,  50, -20, -20,  40,  200  }, // leg1, 2 up ; leg3, 4 down
  {    0, -20, -20,   0,   0, -20, -20,   0,  200  }, // body turn left
  {    0,  40,  40,   0,   0,  40,  40,   0,  200  }, // body turn right
  {    0, -40, -40,   0,   0, -40, -40,   0,  200  }, // body turn left
  {    0,  40,  40,   0,   0,  40,  40,   0,  200  }, // body turn right
  {    0, -20, -20,   0,   0, -20, -20,   0,  200  }, // leg1, 2 up ; leg3, 4 down
};

// Push up
int servoPrg11step = 11;
int servoPrg11 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   70,  90,  90, 110, 110,  90,  90,  70,  300  }, // start
  {   30,   0,   0, -30, -30,   0,   0,  30,  400  }, // down
  {  -30,   0,   0,  30,  30,   0,   0, -30,  500  }, // up
  {   30,   0,   0, -30, -30,   0,   0,  30,  600  }, // down
  {  -30,   0,   0,  30,  30,   0,   0, -30,  700  }, // up
  {   30,   0,   0, -30, -30,   0,   0,  30,  1300 }, // down
  {  -30,   0,   0,  30,  30,   0,   0, -30,  1800 }, // up
  {   65,   0,   0, -65, -65,   0,   0,  65,  200  }, // fast down
  {  -65,   0,   0,   0,  15,   0,   0,   0,  500  }, // leg1 up
  {    0,   0,   0,   0,  50,   0,   0,   0,  500  }, // leg2 up
  {    0,   0,   0,  65,   0,   0,   0, -65,  500  }, // leg3, leg4 up
};

// Sleep
int servoPrg12step = 2;
int servoPrg12 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   35,  90,  90, 145, 145,  90,  90,  35,  400  }, // leg1,4 dn
  {    0, -45,  45,   0,   0,  45, -45,   0,  400  }, // protect myself
};

// Dancing 1
int servoPrg13step = 10;
int servoPrg13 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   90,  90,  90,  90,  90,  90,  90,  90,  300  }, // leg1,2,3,4 up
  {  -40,   0,   0,   0,   0,   0,   0,   0,  300  }, // leg1 dn
  {   40,   0,   0,  40,   0,   0,   0,   0,  300  }, // leg1 up; leg2 dn
  {    0,   0,   0, -40,   0,   0,   0, -40,  300  }, // leg2 up; leg4 dn
  {    0,   0,   0,   0,  40,   0,   0,  40,  300  }, // leg4 up; leg3 dn
  {  -40,   0,   0,   0, -40,   0,   0,   0,  300  }, // leg3 up; leg1 dn
  {   40,   0,   0,  40,   0,   0,   0,   0,  300  }, // leg1 up; leg2 dn
  {    0,   0,   0, -40,   0,   0,   0, -40,  300  }, // leg2 up; leg4 dn
  {    0,   0,   0,   0,  40,   0,   0,  40,  300  }, // leg4 up; leg3 dn
  {    0,   0,   0,   0, -40,   0,   0,   0,  300  }, // leg3 up
};

// Dancing 2
int servoPrg14step = 9;
int servoPrg14 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   70,  45, 135, 110, 110, 135,  45,  70,  300  }, // leg1,2,3,4 two sides
  {   45,   0,   0, -45,   0,   0,   0,   0,  300  }, // leg1,2 up
  {  -45,   0,   0,  45, -45,   0,   0,  45,  300  }, // leg1,2 dn; leg3,4 up
  {   45,   0,   0, -45,  45,   0,   0, -45,  300  }, // leg3,4 dn; leg1,2 up
  {  -45,   0,   0,  45, -45,   0,   0,  45,  300  }, // leg1,2 dn; leg3,4 up
  {   45,   0,   0, -45,  45,   0,   0, -45,  300  }, // leg3,4 dn; leg1,2 up
  {  -45,   0,   0,  45, -45,   0,   0,  45,  300  }, // leg1,2 dn; leg3,4 up
  {   45,   0,   0, -45,  45,   0,   0, -45,  300  }, // leg3,4 dn; leg1,2 up
  {  -40,   0,   0,  40,   0,   0,   0,   0,  300  }, // leg1,2 dn
};

// Dancing 3
int servoPrg15step = 10;
int servoPrg15 [][numberOfACE] PROGMEM = {
  // P10, P11, P12, P16, P02, P04, P07, P15,  ms
  {   70,  45,  45, 110, 110, 135, 135,  70,  300  }, // leg1,2,3,4 bk
  {   40,   0,   0, -50, -40,   0,   0,   0,  300  }, // leg1,2,3 up
  {  -40,   0,   0,  50,  40,   0,   0,   0,  300  }, // leg1,2,3 dn
  {   40,   0,   0,   0, -40,   0,   0,  50,  300  }, // leg1,3,4 up
  {  -40,   0,   0,   0,  40,   0,   0, -50,  300  }, // leg1,3,4 dn
  {   40,   0,   0, -50, -40,   0,   0,   0,  300  }, // leg1,2,3 up
  {  -40,   0,   0,  50,  40,   0,   0,   0,  300  }, // leg1,2,3 dn
  {   40,   0,   0,   0, -40,   0,   0,  50,  300  }, // leg1,3,4 up
  {  -40,   0,   0,   0,  40,   0,   0, -50,  300  }, // leg1,3,4 dn
  {    0,  45,  45,   0,   0, -45, -45,   0,  300  }, // standby
};

// ----------------------------------------------------------------------------------------------------



// Setup
// ----------------------------------------------------------------------------------------------------

void setup() {
  // EEPROM Clear (For Debug Only)
  // eepromClear();

  // Serial.begin(9600); // Open serial communications

  getServoCal(); // Get servoCal from EEPROM

  // Setup UI canvas
  LRemote.setName(robotName);
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(12, 21);

  // Add a push button
  // ButtonLabel, PosX, PosY, SizeW, SizeH, Color, ObjectName
  addButton("Turn Left",  0,  0, 4, 2, RC_GREEN,  button01);
  addButton("Forward",    4,  0, 4, 2, RC_BLUE,   button02);
  addButton("Turn Right", 8,  0, 4, 2, RC_GREEN,  button03);
  addButton("Left",       0,  2, 4, 2, RC_BLUE,   button04);
  addButton("Backward",   4,  2, 4, 2, RC_BLUE,   button05);
  addButton("Right",      8,  2, 4, 2, RC_BLUE,   button06);
  addButton("Standby",    0,  4, 4, 2, RC_PINK,   button07);
  addButton("Say Hi!",    4,  4, 4, 2, RC_ORANGE, button08);
  addButton("Push Up",    8,  4, 4, 2, RC_ORANGE, button09);
  addButton("Lie",        0,  6, 4, 2, RC_ORANGE, button10);
  addButton("Fighting",   4,  6, 4, 2, RC_ORANGE, button11);
  addButton("Sleep",      8,  6, 4, 2, RC_ORANGE, button12);
  addButton("Dancing 1",  0,  8, 4, 2, RC_ORANGE, button13);
  addButton("Dancing 2",  4,  8, 4, 2, RC_ORANGE, button14);
  addButton("Dancing 3",  8,  8, 4, 2, RC_ORANGE, button15);
  addButton("Zero",       0, 20, 2, 1, RC_ORANGE, buttonZero);
  addSwitch("",           8, 18, 3, 3, RC_BLUE, switch01);
  addLabel("Auto",        6, 20, 2, 1, RC_GREY, label01);

  if (enableCalibration == true) {
    sliderX = 0;
    sliderWidth = 5;
  } else {
    sliderX = 13;
    sliderWidth = 6;
  }
  addButton("Clr Cal",  2 + sliderX, 20, 2, 1, RC_PINK, buttonClear);
  addButton("+",        5 + sliderX, 11, 1, 1, RC_PINK, buttonC05a);
  addButton("-",        5 + sliderX, 12, 1, 1, RC_PINK, buttonC05b);
  addButton("+",       11 + sliderX, 11, 1, 1, RC_PINK, buttonC01a);
  addButton("-",       11 + sliderX, 12, 1, 1, RC_PINK, buttonC01b);
  addButton("+",        5 + sliderX, 13, 1, 1, RC_PINK, buttonC06a);
  addButton("-",        5 + sliderX, 14, 1, 1, RC_PINK, buttonC06b);
  addButton("+",       11 + sliderX, 13, 1, 1, RC_PINK, buttonC02a);
  addButton("-",       11 + sliderX, 14, 1, 1, RC_PINK, buttonC02b);
  addButton("+",        5 + sliderX, 15, 1, 1, RC_PINK, buttonC07a);
  addButton("-",        5 + sliderX, 16, 1, 1, RC_PINK, buttonC07b);
  addButton("+",       11 + sliderX, 15, 1, 1, RC_PINK, buttonC03a);
  addButton("-",       11 + sliderX, 16, 1, 1, RC_PINK, buttonC03b);
  addButton("+",        5 + sliderX, 17, 1, 1, RC_PINK, buttonC08a);
  addButton("-",        5 + sliderX, 18, 1, 1, RC_PINK, buttonC08b);
  addButton("+",       11 + sliderX, 17, 1, 1, RC_PINK, buttonC04a);
  addButton("-",       11 + sliderX, 18, 1, 1, RC_PINK, buttonC04b);

  // Add a slider摇杆
  // SliderLabel, PosX, PosY, SizeW, SizeH, RangeMin, RangeMax, RangInit, Color, ObjectName
  addSlider("P02", 0, 11, sliderWidth, 2, 10, 170, servoAct00[4], RC_BLUE, slider05);
  addSlider("P10", 6, 11, sliderWidth, 2, 10, 170, servoAct00[0], RC_BLUE, slider01);
  addSlider("P04", 0, 13, sliderWidth, 2, 45, 135, servoAct00[5], RC_BLUE, slider06);
  addSlider("P11", 6, 13, sliderWidth, 2, 45, 135, servoAct00[1], RC_BLUE, slider02);
  addSlider("P07", 0, 15, sliderWidth, 2, 45, 135, servoAct00[6], RC_BLUE, slider07);
  addSlider("P12", 6, 15, sliderWidth, 2, 45, 135, servoAct00[2], RC_BLUE, slider03);
  addSlider("P15", 0, 17, sliderWidth, 2, 10, 170, servoAct00[7], RC_BLUE, slider08);
  addSlider("P16", 6, 17, sliderWidth, 2, 10, 170, servoAct00[3], RC_BLUE, slider04);

  LRemote.begin(); // Start broadcasting remote controller

  // Servo Pin Set
  servo[0].attach(10);
  servo[0].write(90 + servoCal[0]);
  servo[1].attach(11);
  servo[1].write(90 + servoCal[1]);
  servo[2].attach(12);
  servo[2].write(90 + servoCal[2]);
  servo[3].attach(16);
  servo[3].write(90 + servoCal[3]);
  servo[4].attach(2);
  servo[4].write(90 + servoCal[4]);
  servo[5].attach(4);
  servo[5].write(90 + servoCal[5]);
  servo[6].attach(7);
  servo[6].write(90 + servoCal[6]);
  servo[7].attach(15);
  servo[7].write(90 + servoCal[7]);

  runServoPrg(servoPrg00, servoPrg00step); // zero position
}

// ----------------------------------------------------------------------------------------------------



// Loop
// ----------------------------------------------------------------------------------------------------

void loop() {
  // Check connection
  if (!LRemote.connected()) {
    delay(1000);
  }

  // Process the incoming BLE write request
  LRemote.process();

  // When button pressed
  if (button01.getValue()) {
    runServoPrgV(servoPrg06, servoPrg06step); // turnLeft
  } else if (button02.getValue()) {
    runServoPrgV(servoPrg02, servoPrg02step); // forward
  } else if (button03.getValue()) {
    runServoPrgV(servoPrg07, servoPrg07step); // turnRight
  } else if (button04.getValue()) {
    runServoPrgV(servoPrg04, servoPrg04step); // moveLeft
  } else if (button05.getValue()) {
    runServoPrgV(servoPrg03, servoPrg03step); // backward
  } else if (button06.getValue()) {
    runServoPrgV(servoPrg05, servoPrg05step); // moveRight
  } else if (button07.getValue()) {
    runServoPrgV(servoPrg01, servoPrg01step); // standby
  } else if (button08.getValue()) {
    runServoPrgV(servoPrg09, servoPrg09step); // sayHi
  } else if (button09.getValue()) {
    runServoPrgV(servoPrg11, servoPrg11step); // pushUp
  } else if (button10.getValue()) {
    runServoPrgV(servoPrg08, servoPrg08step); // lie
  } else if (button11.getValue()) {
    runServoPrgV(servoPrg10, servoPrg10step); // fighting
  } else if (button12.getValue()) {
    runServoPrgV(servoPrg12, servoPrg12step); // sleep
  } else if (button13.getValue()) {
    runServoPrgV(servoPrg13, servoPrg13step); // dancing1
  } else if (button14.getValue()) {
    runServoPrgV(servoPrg14, servoPrg14step); // dancing2
  } else if (button15.getValue()) {
    runServoPrgV(servoPrg15, servoPrg15step); // dancing3
  } else if (buttonZero.getValue()) {
    runServoPrgV(servoPrg00, servoPrg00step); // zero position
  } else if (buttonClear.getValue()) {
    clearCal(); // Clear Servo calibration data
  } else if (buttonC01a.getValue()) {
    calibration(0, 1);
  } else if (buttonC01b.getValue()) {
    calibration(0, -1);
  } else if (buttonC02a.getValue()) {
    calibration(1, 1);
  } else if (buttonC02b.getValue()) {
    calibration(1, -1);
  } else if (buttonC03a.getValue()) {
    calibration(2, 1);
  } else if (buttonC03b.getValue()) {
    calibration(2, -1);
  } else if (buttonC04a.getValue()) {
    calibration(3, 1);
  } else if (buttonC04b.getValue()) {
    calibration(3, -1);
  } else if (buttonC05a.getValue()) {
    calibration(4, 1);
  } else if (buttonC05b.getValue()) {
    calibration(4, -1);
  } else if (buttonC06a.getValue()) {
    calibration(5, 1);
  } else if (buttonC06b.getValue()) {
    calibration(5, -1);
  } else if (buttonC07a.getValue()) {
    calibration(6, 1);
  } else if (buttonC07b.getValue()) {
    calibration(6, -1);
  } else if (buttonC08a.getValue()) {
    calibration(7, 1);
  } else if (buttonC08b.getValue()) {
    calibration(7, -1);
  }

  // When slider change
  if (slider01.isValueChanged()) {
    servo[0].write(slider01.getValue() + servoCal[0]);
  } else if (slider02.isValueChanged()) {
    servo[1].write(slider02.getValue() + servoCal[1]);
  } else if (slider03.isValueChanged()) {
    servo[2].write(slider03.getValue() + servoCal[2]);
  } else if (slider04.isValueChanged()) {
    servo[3].write(slider04.getValue() + servoCal[3]);
  } else if (slider05.isValueChanged()) {
    servo[4].write(slider05.getValue() + servoCal[4]);
  } else if (slider06.isValueChanged()) {
    servo[5].write(slider06.getValue() + servoCal[5]);
  } else if (slider07.isValueChanged()) {
    servo[6].write(slider07.getValue() + servoCal[6]);
  } else if (slider08.isValueChanged()) {
    servo[7].write(slider08.getValue() + servoCal[7]);
  }

  // When switch button change
  if (switch01.isValueChanged()) {
    autoMode = switch01.getValue();
  }//随机模式
  if (autoMode == 1) {
    switch (random(0, 6)) {
      case 0:
        runServoPrgV(servoPrg06, servoPrg06step); // turnLeft
        break;
      case 1:
        runServoPrgV(servoPrg02, servoPrg02step); // forward
        break;
      case 2:
        runServoPrgV(servoPrg07, servoPrg07step); // turnRight
         break;
      case 3:
       runServoPrgV(servoPrg04, servoPrg04step); // moveLeft
        break;
      case 4:
        runServoPrgV(servoPrg03, servoPrg03step); // backward
         break;
      case 5:
       runServoPrgV(servoPrg05, servoPrg05step); // moveRight
         break;
      case 6:
       runServoPrgV(servoPrg01, servoPrg01step); // standby
         break;
      case 7:
       runServoPrgV(servoPrg09, servoPrg09step); // sayHi
        break;
      case 8:
        runServoPrgV(servoPrg11, servoPrg11step); // pushUp
        break;
      case 9:
        runServoPrgV(servoPrg08, servoPrg08step); // lie
        break;
      case 10:
        runServoPrgV(servoPrg10, servoPrg10step); // fighting
        break;
      case 11:
        runServoPrgV(servoPrg12, servoPrg12step); // sleep
        break;
      case 12:
        runServoPrgV(servoPrg13, servoPrg13step); // dancing1
        break;
      case 13:
        runServoPrgV(servoPrg14, servoPrg14step); // dancing2
        break;
      case 14:
        runServoPrgV(servoPrg15, servoPrg15step); // dancing3
        break;
    }
    delay(500);
  }

  delay(50);
}

// ----------------------------------------------------------------------------------------------------



// Function
// ----------------------------------------------------------------------------------------------------

// EEPROM Clear (For debug only)
void eepromClear() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}

// Get servoCal from EEPROM
void getServoCal() {
  int eeAddress = 0;
  for (int i = 0; i < numberOfServos; i++) {
    EEPROM.get(eeAddress, servoCal[i]);
    eeAddress += sizeof(servoCal[i]);
  }
}

// Put servoCal to EEPROM
void putServoCal() {
  int eeAddress = 0;
  for (int i = 0; i < numberOfServos; i++) {
    EEPROM.put(eeAddress, servoCal[i]);
    eeAddress += sizeof(servoCal[i]);
  }
}

// Add a label
void addLabel(String text, int posX, int posY, int sizeW, int sizeH, RCColorType colorType, LRemoteLabel &label) {
  label.setText(text);
  label.setPos(posX, posY);
  label.setSize(sizeW, sizeH);
  label.setColor(colorType);
  LRemote.addControl(label);
}

// Add a push button
void addButton(String label, int posX, int posY, int sizeW, int sizeH, RCColorType colorType, LRemoteButton &button) {
  button.setText(label);
  button.setPos(posX, posY);
  button.setSize(sizeW, sizeH);
  button.setColor(colorType);
  LRemote.addControl(button);
}

// Add a slider
void addSlider(String label, int posX, int posY, int sizeW, int sizeH, int rangeMin, int rangeMax, int rangeInit, RCColorType colorType, LRemoteSlider &slider) {
  slider.setText(label);
  slider.setPos(posX, posY);
  slider.setSize(sizeW, sizeH);
  slider.setColor(colorType);
  slider.setValueRange(rangeMin, rangeMax, rangeInit);
  LRemote.addControl(slider);
}

// Add a switch button
void addSwitch(String label, int posX, int posY, int sizeW, int sizeH, RCColorType colorType, LRemoteSwitch &switchButton) {
  switchButton.setText(label);
  switchButton.setPos(posX, posY);
  switchButton.setSize(sizeW, sizeH);
  switchButton.setColor(colorType);
  LRemote.addControl(switchButton);
}

// Clear Servo calibration data
void clearCal() {
  for (int i = 0; i < numberOfServos; i++) {
    servoCal[i] = 0;
  }
  putServoCal(); // Put servoCal to EEPROM
  runServoPrg(servoPrg00, servoPrg00step); // zero position
}

// Calibration
void calibration(int i, int change) {
  servoCal[i] = servoCal[i] + change;
  servo[i].write(servoAct00[i] + servoCal[i]);
  putServoCal(); // Put servoCal to EEPROM
  delay(400);
}

void runServoPrg(int servoPrg[][numberOfACE], int step)
{
  for (int i = 0; i < step; i++) { // Loop for step

    int totalTime = servoPrg[i][numberOfACE - 1]; // Total time of this step

    // Get servo start position
    for (int s = 0; s < numberOfServos; s++) {
      servoPos[s] = servo[s].read() - servoCal[s];
    }

    for (int j = 0; j < totalTime / servoPrgPeriod; j++) { // Loop for time section
      for (int k = 0; k < numberOfServos; k++) { // Loop for servo
        servo[k].write((map(j, 0, totalTime / servoPrgPeriod, servoPos[k], servoPrg[i][k])) + servoCal[k]);
      }
      delay(servoPrgPeriod);
    }
  }
}

// runServoPrg vector mode向量
void runServoPrgV(int servoPrg[][numberOfACE], int step) {
  for (int i = 0; i < step; i++) { // Loop for step

    int totalTime = servoPrg[i][numberOfACE - 1]; // Total time of this step

    // Get servo start position
    for (int s = 0; s < numberOfServos; s++) {
      servoPos[s] = servo[s].read() - servoCal[s];
    }

    for (int p = 0; p < numberOfServos; p++) { // Loop for servo
      if (i == 0) {
        servoPrevPrg[p] = servoPrg[i][p];
      } else {
        servoPrevPrg[p] = servoPrevPrg[p] + servoPrg[i][p];
      }
    }

    for (int j = 0; j < totalTime / servoPrgPeriod; j++) { // Loop for time section
      for (int k = 0; k < numberOfServos; k++) { // Loop for servo
        servo[k].write((map(j, 0, totalTime / servoPrgPeriod, servoPos[k], servoPrevPrg[k]) + servoCal[k]));
      }
      delay(servoPrgPeriod);
    }
  }
}

// ----------------------------------------------------------------------------------------------------
