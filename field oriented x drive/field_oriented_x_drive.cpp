// VEX V5 C++ Project
#include "vex.h"
#include <algorithm>
#include <cmath>
using namespace vex;

//#region config_globals
vex::motor      back_right_motor(vex::PORT1, vex::gearSetting::ratio18_1, true);
vex::motor      back_left_motor(vex::PORT10, vex::gearSetting::ratio18_1, false);
vex::motor      front_right_motor(vex::PORT11, vex::gearSetting::ratio18_1, true);
vex::motor      front_left_motor(vex::PORT20, vex::gearSetting::ratio18_1, false);
vex::controller con(vex::controllerType::primary);
vex::inertial Inertial2(vex::PORT4);
//#endregion config_globals


int main(void) {
    Inertial2.setHeading(0.0, degrees);
    Inertial2.setRotation(0.0, degrees);
    Inertial2.startCalibration();
    while (Inertial2.isCalibrating()) { 
        task::sleep(10); 
    }
    Inertial2.setHeading(0.0, degrees);
    Inertial2.setRotation(0.0, degrees);
    double maxSpeed = 100;
    while(true) {
      if ( con.ButtonY.pressing() ) {
        maxSpeed = 50;
      }
      if ( con.ButtonRight.pressing() ) {
        maxSpeed = 100;
      }

      if ( con.ButtonDown.pressing() ) {
          
        //Get the raw sums of the X and Y joystick axes
        double front_left  = (double)(con.Axis3.position(pct) + con.Axis4.position(pct));
        double back_left   = (double)(con.Axis3.position(pct) - con.Axis4.position(pct));
        double front_right = (double)(con.Axis3.position(pct) - con.Axis4.position(pct));
        double back_right  = (double)(con.Axis3.position(pct) + con.Axis4.position(pct));
        
        //Find the largest possible sum of X and Y
        double max_raw_sum = (double)(abs(con.Axis3.position(pct)) + abs(con.Axis4.position(pct)));
        
        //Find the largest joystick value
        double max_XYstick_value = (double)(std::max(abs(con.Axis3.position(pct)),abs(con.Axis4.position(pct))));
        
        //The largest sum will be scaled down to the largest joystick value, and the others will be
        //scaled by the same amount to preserve directionality
        if (max_raw_sum != 0) {
            front_left  = front_left / max_raw_sum * max_XYstick_value;
            back_left   = back_left / max_raw_sum * max_XYstick_value;
            front_right = front_right / max_raw_sum * max_XYstick_value;
            back_right  = back_right / max_raw_sum * max_XYstick_value;
        }
        
        //Now to consider rotation
        //Naively add the rotational axis
        front_left  = front_left  + con.Axis1.position(pct);
        back_left   = back_left   + con.Axis1.position(pct);
        front_right = front_right - con.Axis1.position(pct);
        back_right  = back_right  - con.Axis1.position(pct);
        
        //What is the largest sum, or is 100 larger?
        max_raw_sum = std::max(std::abs(front_left),std::max(std::abs(back_left),std::max(std::abs(front_right),std::max(std::abs(back_right),maxSpeed))));
        
        //Scale everything down by the factor that makes the largest only 100, if it was over
        front_left  = front_left  / max_raw_sum * maxSpeed;
        back_left   = back_left   / max_raw_sum * maxSpeed;
        front_right = front_right / max_raw_sum * maxSpeed;
        back_right  = back_right  / max_raw_sum * maxSpeed;
        
        //Write the manipulated values out to the motors
         front_left_motor.spin(fwd,front_left, velocityUnits::pct);
          back_left_motor.spin(fwd,back_left,  velocityUnits::pct);
        front_right_motor.spin(fwd,front_right,velocityUnits::pct);
         back_right_motor.spin(fwd,back_right, velocityUnits::pct);
      } else {
          double headingRadians = Inertial2.heading() * 3.14159/180;
          double yPos = con.Axis3.position(pct);
          double xPos = con.Axis4.position(pct);
          double sineHeading = sin(headingRadians);
          double cosHeading = cos(headingRadians);
          double rotatedYPos = xPos * sineHeading + yPos * cosHeading;
          double rotatedXPos = xPos * cosHeading - yPos * sineHeading;
          
          //Get the raw sums of the X and Y joystick axes
          double front_left  = (double)(rotatedYPos + rotatedXPos);
          double back_left   = (double)(rotatedYPos - rotatedXPos);
          double front_right = (double)(rotatedYPos - rotatedXPos);
          double back_right  = (double)(rotatedYPos + rotatedXPos);
    
          
          //Find the largest possible sum of X and Y
          double max_raw_sum = (double)(abs(con.Axis3.position(pct)) + abs(con.Axis4.position(pct)));
          
          //Find the largest joystick value
          double max_XYstick_value = (double)(std::max(abs(con.Axis3.position(pct)),abs(con.Axis4.position(pct))));
          
          //The largest sum will be scaled down to the largest joystick value, and the others will be
          //scaled by the same amount to preserve directionality
          if (max_raw_sum != 0) {
              front_left  = front_left / max_raw_sum * max_XYstick_value;
              back_left   = back_left / max_raw_sum * max_XYstick_value;
              front_right = front_right / max_raw_sum * max_XYstick_value;
              back_right  = back_right / max_raw_sum * max_XYstick_value;
          }
          
          //Now to consider rotation
          //Naively add the rotational axis
          front_left  = front_left  + con.Axis1.position(pct);
          back_left   = back_left   + con.Axis1.position(pct);
          front_right = front_right - con.Axis1.position(pct);
          back_right  = back_right  - con.Axis1.position(pct);
          
          //What is the largest sum, or is 100 larger?
          max_raw_sum = std::max(std::abs(front_left),std::max(std::abs(back_left),std::max(std::abs(front_right),std::max(std::abs(back_right),maxSpeed))));
          
          //Scale everything down by the factor that makes the largest only 100, if it was over
          front_left  = front_left  / max_raw_sum * maxSpeed;
          back_left   = back_left   / max_raw_sum * maxSpeed;
          front_right = front_right / max_raw_sum * maxSpeed;
          back_right  = back_right  / max_raw_sum * maxSpeed;
          
          //Write the manipulated values out to the motors
          front_left_motor.spin(fwd,front_left, velocityUnits::pct);
          back_left_motor.spin(fwd,back_left,  velocityUnits::pct);
          front_right_motor.spin(fwd,front_right,velocityUnits::pct);
          back_right_motor.spin(fwd,back_right, velocityUnits::pct);

      }
    }
}
