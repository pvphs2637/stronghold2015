/*
 *
 * This is the final robot code.
 * Ultrasonic code commented out b/c ultrasonic device is useless-crap
 * All changes will be documented here with the following format.
 * -Reed Schrier 2016-02-19 13:33s
 *
 *
 */
/*
 */
#include "WPILib.h"
#include "drive.h"
#include "OI.h"
#include "hardware.h"

/*THIS CODE INCLUDES:
-Autonomous code
-the infamous STRONGHOLD CODE
-NavX automation
-Ultrasonic automation
 */

/*
JOYSTICK ASSIGNMENTS for THIS CODE(not checked by Mr. Rudder)
JOYSTICK DRIVE
X and Y axis drive robot
button 1: while pressed: turns the robot towards 0 degrees on NavX's GetAngle
button 2: while pressed: turns the robot towards 180 degrees on NavX's GetAngle
button 7: resets NavX's GetAngle

JOYSTICK ARM
button 1: sucks balls in
button 2: shoots balls out
button 3: lowers bicep
button 4: lowers forearm
button 5: raises bicep
button 6: raises forearm
Y-Axis controls auxiliary arm
*/

class Robot:
public IterativeRobot
{
private:
	Drive* driver;
	OI* oi;                 //defines variables
	Hardware* hardware;
	double circumference;
	double distance1;
	double distance2;
	double degrees;

public:
	Robot()
	{
		hardware= new Hardware();
		driver= new Drive(hardware->Fleft, hardware->Fright, hardware->Rleft, hardware->Rright);
		oi= new OI();              //initiates robot components
		degrees = 0;
		distance1 = 0;
		distance2 = 0;
		circumference = 8;
	}
	void DisabledPeriodic()
	{
		//while disabled, the robot's talon motor controllers will be set at neutral mode
		SmartDashboard::PutString("In DisabledPeriodic", "yes");
		hardware->Fleft->ConfigNeutralMode(CANTalon::kNeutralMode_Jumper);
		hardware->Rleft->ConfigNeutralMode(CANTalon::kNeutralMode_Jumper);
		hardware->Fright->ConfigNeutralMode(CANTalon::kNeutralMode_Jumper);
		hardware->Rright->ConfigNeutralMode(CANTalon::kNeutralMode_Jumper);

	}
	void RobotInit()
	{
		//autonomous variables

		SmartDashboard::PutNumber("Wheel Circumference (in inches)", circumference);
		SmartDashboard::PutNumber("Autonomous Forward Distance (in inches)", distance1);    //before a match, smartdashboard will ask for these variables
		SmartDashboard::PutNumber("Autonomous Forward Distance (in inches)", distance2);
		SmartDashboard::PutNumber("Autonomous Turn Degrees", degrees);      //these above variables will be used in autonomous

		//motor speed variables

		SmartDashboard::PutNumber("bicep speed", .450);
		SmartDashboard::PutNumber("forearm speed", .450);

		//arm automation variables
		/*
		SmartDashboard::PutNumber("bicep desired position for sally port",90);
		SmartDashboard::PutNumber("forearm desired position for sally port",90);
		SmartDashboard::PutNumber("Bicep Starting Position",(double)hardware->bicepEncoder->GetRaw);
		SmartDashboard::PutNumber("Bicep Starting Position",(double)hardware->forearmEncoder->GetRaw);
		hardware->bicepEncoder->SetDistancePerPulse(1);
		hardware->forearmEncoder->SetDistancePerPulse(1);
		*/

		hardware->wheelEncoder->Reset();                       //resets the encoder
		//hardware->ultra->SetAutomaticMode(true);      //prepares ultrasonic sensor for match
		hardware->navx->ZeroYaw();					//resets NavX MXP compass reading */


	}
	void AutonomousInit()
	{
		distance1 = SmartDashboard::GetNumber("Autonomous initial Forward Distance", distance1);     //initializes distance1 variable
		distance2 = SmartDashboard::GetNumber("Autonomous Forward Distance after turn", distance2);  //initializes distance2 variable
		degrees = SmartDashboard::GetNumber("Autonomous RIGHT turn Degrees(between 1 and 300)", degrees);     //initializes degrees variable
		circumference = SmartDashboard::GetNumber("Wheel Circumference", circumference);  //initializes circumference variable
		hardware->wheelEncoder->SetDistancePerPulse(circumference/360);     //prepares encoder for autonomous
		while(hardware->wheelEncoder->GetDistance() <= distance1)
		{
			driver->AutoDrive(-.5, .5);    //while the encoder reads less than distance1, drive forward at 1/2 speed
		}
		while(hardware->navx->GetAngle() <= degrees || hardware->navx->GetAngle() >300)
		{
			driver->AutoDrive(-.3, -.3);  //while the NavX reads less than the specified degrees it will turn right
		}
		hardware->wheelEncoder->Reset();
		while(hardware->wheelEncoder->GetDistance() <= distance2);
		{
			driver->AutoDrive(-.5, .5);			//while the encoder reads less than distance2, drive forward at half speed
		}
		hardware->intake->Set(-1);
		Wait(2);                        //outtake for 2 seconds
		hardware->intake->Set(0);
	}
	void TeleopPeriodic()
	{

		//vision with USB camera

		//Talon mode 15 fix

		if (oi->getJoystickDrive()->GetRawButton(11))
		{
			hardware->Fleft->EnableControl();
			hardware->Rleft->EnableControl();
			hardware->Fright->EnableControl();
			hardware->Rright->EnableControl();
		}

		//Drive

		driver->drive(oi->getJoystickDrive());          //JoystickDrive controls drive motors

		//Arm Bicep

		if(oi->getJoystickArm()->GetRawButton(3))
		{
			hardware->bicep->Set(SmartDashboard::GetNumber("bicep speed", .25));    //button 3 raises bicep
		}
		else if(oi->getJoystickArm()->GetRawButton(4))
		{
			hardware->bicep->Set(-SmartDashboard::GetNumber("bicep speed", .25));   //button 4 lowers bicep
		}
		else
		{
			hardware->bicep->Set(0);
		}

		//Arm Forearm

		if(oi->getJoystickArm()->GetRawButton(6))
		{
			hardware->forearm->Set(SmartDashboard::GetNumber("forearm speed", .25));  //button 6 raises forearm
		}
		else if(oi->getJoystickArm()->GetRawButton(5))
		{
			hardware->forearm->Set(-SmartDashboard::GetNumber("forearm speed", .25));  //button 5 lowers forearm
		}
		else
		{
			hardware->forearm->Set(0);
		}

		//Arm Auxilary

		hardware->auxArm->Set(oi->getJoystickArm()->GetY()/-2);

		//Intake

		if (oi->getJoystickDrive()->GetRawButton(1))    //button 1 on arm joystick sucks balls in
		{
			hardware->intake->Set(1.0);
		}
		else if (oi->getJoystickDrive()->GetRawButton(2))      //button 2 on arm joystick shoots balls out
		{
			hardware->intake->Set(-1.0);
		}
		else if (oi->getJoystickDrive()->GetPOV() == 0)       //the POV shoots balls out
		{
			hardware->intake->Set(-1.0);
		}
		else
		{
			hardware->intake->Set(0);
		}

		//Display if ultrasonic reading is greater than or less than 12 inches

		/*if (hardware->ultra->GetRangeInches()<12)
		{
			SmartDashboard::PutBoolean("ultrasonic", true);
		}
		else
		{
			SmartDashboard::PutBoolean("ultrasonic", false);
		}
*/
		//NavX MXP

		SmartDashboard::PutNumber("Orientation",(double)hardware->navx->GetAngle());
		if (oi->getJoystickDrive()->GetRawButton(7)==1)
		{
			hardware->navx->ZeroYaw();						//button 7 resets orientation degree value from navx
		}
		double orientation;
		orientation = hardware->navx->GetAngle();
		if(oi->getJoystickDrive()->GetRawButton(3)==1)
		{
			if(hardware->navx->GetAngle() != 0)           //button 1 on JoystickDrive orients the robot to 0 degrees
			{
				if (orientation < 0 && orientation > 180 )
				{
					driver->AutoDrive(-.7,.7);
				}
				else if (orientation > 180 && orientation < 360)
				{
					driver->AutoDrive(.7,-.7);
				}
			}
		}
		else if(oi->getJoystickDrive()->GetRawButton(5)==1)
		{
			if(hardware->navx->GetAngle() != 180)           //button 2 on JoystickDrive orients the robot to 180 degrees
			{
				if (orientation < 0 && orientation > 180 )
				{
					driver->AutoDrive(.7,-.7);
				}
				else if (orientation > 180 && orientation < 360)
				{
					driver->AutoDrive(-.7,.7);
				}
			}
	    }
	}
};

START_ROBOT_CLASS(Robot);
