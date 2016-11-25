/*
 * hardware.h
 *
 *  Created on: Feb 1, 2016
 *      Author: Developer
 */
#ifndef HARDWARE_H
#define HARDWARE_H
#include "WPILib.h"
#include "AHRS.h"


class Hardware
{
public:
	Hardware();
	CANTalon*Fleft;
	CANTalon*Fright;
	CANTalon*Rleft;
	CANTalon*Rright;
	Spark*intake;
	Spark*bicep;
	Spark*forearm;
	Spark*auxArm;
//	Ultrasonic* ultra;
	AHRS* navx;
	Encoder *wheelEncoder;
	//Encoder *bicepEncoder;
	//Encoder *forearmEncoder;

 };
#endif


