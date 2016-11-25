/*
 * drive.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: Developer
 */
#include "drive.h"

void Drive::drive(Joystick*stick)
{
	driver->ArcadeDrive(stick);
}
void Drive::AutoDrive(float x, float y)
{
	driver->TankDrive(x, y);
}
