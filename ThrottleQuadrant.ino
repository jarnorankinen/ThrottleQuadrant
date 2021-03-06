/************************************************************************************
 * Throttle Quadrant using Arduino Micro & slide potentiometers
 * version 1.11
 * 
 * https://github.com/jarnorankinen/ThrottleQuadrant
 * 
 * Jarno Rankinen
 * jarno.rankinen+github@gmail.com
 * 
 * It is recommended to connect one slider at a time and calibrate, then wire the next one and so on.
 * 
 * Check the input pins in setup()!!
 * 
 * Shows up as three joysticks, all named "Arduino Micro", use simulators own interface to bind the controls to correct axis.
 * It may be necessary to invert some axis.
 * 
 * Calibration: (for throttle_l-axis in this example, you only need to do this once or after hardware modifications)
 * - Set calib = true
 * - Upload to Arduino
 * - Open the serial monitor
 * - Move the slider corresponding to throttle_l-axis to its min & max positions
 * - Note the min & max values of throttle_l
 * - Input the values to throttle_lmin & throttle_lmax
 * - Upload to Arduino
 * - Repeat for each axis
 * - Afterwards set calib == false & Upload to Arduino
 * - Remember to calibrate controller in the simulator
 * 
 * Changelog:
 * 
 * v1.0     Initial release
 * 
 * v1.1     Switched to Joystick.h (single joystick library) for hassle-free Linux support. Changed behavior to update joystick axis output value
 *          only if raw value changes more than +/-3, to filter input and allow simultaneous keyboard control.
 *      
 * v1.11    Added rule to ignore spike values outside min-max range
 * 
 * v1.2     Improved signal filtering. DELTA_MIN defines treshold for ignoring small changes, DELTA_MAX defines treshold for ignoring sudden large changes
 *          from e.g. faulty potentiometers. Not perfect yet.
 * 
 **********************************************************************************/





#include "Joystick.h"

//constants to filter input value glitching
const uint8_t DELTA_MIN = 2;
const uint8_t DELTA_MAX = 10;

bool update = false;

uint16_t throttle_l = 0;       // raw data from the slider
uint16_t throttle_lmin = 14, throttle_lmax = 236; //min & max values for calibration
int throttle_l_axis = -128;     // the value to give to the Joystick axis
int tl_old = -128;

uint16_t throttle_r = 0;
uint16_t throttle_rmin = 14, throttle_rmax = 234;
int throttle_r_axis = -128;
int tr_old = -128;

uint16_t prop_l = 0;
uint16_t prop_lmin = 14, prop_lmax = 226;
int prop_l_axis = 127;
int pl_old = 127;

uint16_t prop_r = 0;
uint16_t prop_rmin = 14, prop_rmax = 232;
int prop_r_axis = 359;
int pr_old = 359;

uint16_t mix_l = 0;
uint16_t mix_lmin = 15, mix_lmax = 231;
int mix_l_axis = 0;
int ml_old = 0;

uint16_t mix_r = 0;
uint16_t mix_rmin = 15, mix_rmax = 235;
int mix_r_axis = 0;
int mr_old = 0;

double rotation_multip = 1.591;

/************* SET THIS AS true TO TURN CALIBRATION ON********************/
bool calib = false; //Serial monitor on/off

unsigned int delta(uint16_t a, uint16_t b) {
  int x = a - b;
  return abs(x);
}

void setup() {

  //Check that these are correct for your build
  pinMode(A0, INPUT_PULLUP);  //left throttle, X-axis
  pinMode(A1, INPUT_PULLUP);  //right throttle, Y-axis
  pinMode(A2, INPUT_PULLUP);  //left prop, Z-axis
  pinMode(A3, INPUT_PULLUP);  //right prop, X-rotation axis
  pinMode(A4, INPUT_PULLUP);  //left mixture, Y-rotation axis
  pinMode(A5, INPUT_PULLUP);  //right mixture, rudder axis

  Serial.begin(9600);
  Joystick.begin(false);

  tl_old = throttle_l = analogRead(A0);
  tr_old = throttle_r = analogRead(A1);
  pl_old = prop_l = analogRead(A2);
  pr_old = prop_r = analogRead(A3);
  ml_old = mix_l = analogRead(A4);
  mr_old = mix_r = analogRead(A5);
  
}

void loop() {
  
    throttle_l = analogRead(A0);
    if (calib == false && (throttle_l > throttle_lmax)) throttle_l = throttle_lmax;
    if (calib == false && (throttle_l < throttle_lmin)) throttle_l = throttle_lmin;
    
  
    throttle_r = analogRead(A1);
    if (calib == false && (throttle_r > throttle_rmax)) throttle_r = throttle_rmax;
    if (calib == false && (throttle_r < throttle_rmin)) throttle_r = throttle_rmin;
    
  
    prop_l = analogRead(A2);
    if (calib == false && (prop_l > prop_lmax)) prop_l = prop_lmax;
    if (calib == false && (prop_l < prop_lmin)) prop_l = prop_lmin;
    

    prop_r = analogRead(A3);
    if (calib == false && (prop_r > prop_rmax)) prop_r = prop_rmax;
    if (calib == false && (prop_r < prop_rmin)) prop_r = prop_rmin;

    
    mix_l = analogRead(A4);
    if (calib == false && (mix_l > mix_lmax)) mix_l = mix_lmax;
    if (calib == false && (mix_l < mix_lmin)) mix_l = mix_lmin;
    
  
    mix_r = analogRead(A5);
    if (calib == false && (mix_r > mix_rmax)) mix_r = mix_rmax;
    if (calib == false && (mix_r < mix_rmin)) mix_r = mix_rmin;
    
  
  if(calib==true) {               //Serial monitor for calibration
    Serial.println();
    Serial.print("throttle_l=");Serial.print(throttle_l);
    Serial.print(" -> ");
    Serial.print("throttle_l_axis=");Serial.println(throttle_l_axis);
    Serial.print("throttle_r=");Serial.print(throttle_r);
    Serial.print(" -> ");
    Serial.print("throttle_r_axis=");Serial.println(throttle_r_axis);
    Serial.print("prop_l=");Serial.print(prop_l);
    Serial.print(" -> ");
    Serial.print("prop_l_axis=");Serial.println(prop_l_axis);
    Serial.print("prop_r=");Serial.print(prop_r);
    Serial.print(" -> ");
    Serial.print("prop_r_axis=");Serial.println(prop_r_axis);
    Serial.print("mix_l=");Serial.print(mix_l);
    Serial.print(" -> ");
    Serial.print("mix_l_axis=");Serial.println(mix_l_axis);
    Serial.print("mix_r=");Serial.print(mix_r);
    Serial.print(" -> ");
    Serial.print("mix_r_axis=");Serial.println(mix_r_axis);
  }

    if (DELTA_MIN<delta(throttle_l, tl_old) && delta(throttle_l, tl_old)<DELTA_MAX) {
        throttle_l_axis = 256*throttle_l/(throttle_lmax-throttle_lmin+5) - 128 - throttle_lmin;  //convert raw values -> -128 - 128 (roughly, joystick calibration is still recommended via e.g. Windows joystick interface)
        Joystick.setXAxis(throttle_l_axis);
        tl_old = throttle_l;
        update = true;
    }

    else if (DELTA_MIN<delta(throttle_r, tr_old) && delta(throttle_r, tr_old)<DELTA_MAX) {
        throttle_r_axis = 256*throttle_r/(throttle_rmax-throttle_rmin+5) - 128 - throttle_rmin;
        Joystick.setYAxis(throttle_r_axis);
        tr_old = throttle_r;
        update = true;
    }
    
    else if (DELTA_MIN<delta(prop_l, pl_old) && delta(prop_l, pl_old)<DELTA_MAX) {
        prop_l_axis = 256*prop_l/(prop_lmax-prop_lmin+5) - 128 - prop_lmin;
        Joystick.setZAxis(prop_l_axis);
        pl_old = prop_l;
        update = true;
    }

    else if (DELTA_MIN<delta(prop_r, pr_old) && delta(prop_r, pr_old)<DELTA_MAX) {
        prop_r_axis = prop_r * rotation_multip - (prop_rmin + 5);
        Joystick.setXAxisRotation(prop_r_axis);
        pr_old = prop_r;
        update = true;
    }
    
    else if (DELTA_MIN<delta(mix_l, ml_old) && delta(mix_l, ml_old)<DELTA_MAX) {
        mix_l_axis = mix_l * rotation_multip - (mix_lmin + 5);
        Joystick.setYAxisRotation(mix_l_axis);
        ml_old = mix_l;
        update = true;
    }

     else if (DELTA_MIN<delta(mix_r, mr_old) && delta(mix_r, mr_old)<DELTA_MAX) {
        mix_r_axis = 254*(mix_r - mix_rmin)/(mix_rmax - mix_rmin);
        Joystick.setRudder(mix_r_axis);
        mr_old = mix_r;
        update = true;
     }

     else update=false;

     if (update==true) {
        Joystick.sendState();
        update=false;
     }

  Serial.println(delta(mr_old, mix_r));
}
