/*
 * hardware.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: Developer
 */
#include "hardware.h"

Hardware::Hardware()
{
	Fleft = new CANTalon(1);
	Fright = new CANTalon(2);
	Rleft = new CANTalon(3);
	Rright = new CANTalon(4);
	intake = new Spark(0);
	bicep = new Spark(1);
	forearm = new Spark(2);
	auxArm = new Spark(3);
	//ultra = new Ultrasonic(0,1);
	navx = new AHRS(SPI::kMXP);
	wheelEncoder = new Encoder(2,3,false,Encoder::EncodingType::k4X);
	//bicepEncoder = new Encoder(4,5,false,Encoder::EncodingType::k4X);
	//forearmEncoder = new Encoder (6,7,false,Encoder::EncodingType::k4X);

}


