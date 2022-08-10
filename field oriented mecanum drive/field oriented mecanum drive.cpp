// VEX V5 C++ Project
#include "vex.h"
#include <algorithm>
#include <cmath>
using namespace vex;


vex::motor back_right_motor( vex::PORT1, vex::gearSetting::ratio18_1, false );
vex::motor back_left_motor( vex::PORT3, vex::gearSetting::ratio18_1, true );
vex::motor front_right_motor( vex::PORT2, vex::gearSetting::ratio18_1, false );
vex::motor front_left_motor( vex::PORT4, vex::gearSetting::ratio18_1, true );
vex::controller con(vex::controllerType::primary);
vex::inertial Inertial2(vex::PORT5);

void usercontrol(void) {
  // User control code here, inside the loop
  Inertial2.setHeading(0.0, degrees);
    Inertial2.setRotation(0.0, degrees);
    Inertial2.startCalibration();
    while (Inertial2.isCalibrating()) { 
        task::sleep(10); 
    }
    Inertial2.setHeading(0.0, degrees);
    Inertial2.setRotation(0.0, degrees);
  while (1) {
    while(true)
    { 


        double headingRadians = Inertial2.heading() * 3.14159/180;
        double yPos = con.Axis3.position(pct);
        double xPos = con.Axis4.position(pct);
        double sineHeading = sin(headingRadians);
        double cosHeading = cos(headingRadians);
        double rotatedYPos = xPos * sineHeading + yPos * cosHeading;
        double rotatedXPos = xPos * cosHeading - yPos * sineHeading;
        double rotation = con.Axis1.position(pct);

        back_left_motor.spin( directionType::fwd, rotatedYPos - rotatedXPos + rotation, velocityUnits::pct );
        front_left_motor.spin( directionType::fwd, rotatedYPos + rotatedXPos + rotation, velocityUnits::pct );
        back_right_motor.spin( directionType::rev, - rotatedYPos - rotatedXPos + rotation, velocityUnits::pct );
        front_right_motor.spin( directionType::rev, - rotatedYPos + rotatedXPos + rotation, velocityUnits::pct );
    }
  }
}
void preAuton(void)
{
  wait(1, seconds);
}

void auton(void)
{
 
}

int main() {
  competition comp;

  comp.autonomous(auton);
  comp.drivercontrol(usercontrol);

  preAuton();
  return 0;
}
