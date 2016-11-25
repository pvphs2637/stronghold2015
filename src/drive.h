/*
 * drive.h
 *
 *  Created on: Feb 1, 2016
 *      Author: Developer
 */
#ifndef DRIVE_H
#define DRIVE_H
#include "WPILib.h"

class Drive
{
private:
	RobotDrive* driver;
public:
	Drive(CANTalon*Fleft,CANTalon*Fright,CANTalon*Rleft,CANTalon*Rright)
	{
		driver = new RobotDrive(Fleft,Fright,Rleft,Rright);
	}
	void drive(Joystick*);
	void AutoDrive(float x, float y);
};
#endif


