#include "main.h"

bool frontClawOpen = true;
bool backClawOpen = true;

//left side driver motors
pros::Motor left_mtr(1);

//right side drive motors
pros::Motor right_mtr(4, true);

//arm motors
pros::Motor rightArm_mtr(5);
pros::Motor leftArm_mtr(6, true);

//claws
pros:: Motor frontClaw(7);
pros:: Motor backClaw(8, true);

//startup code
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Code Running");

	rightArm_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	leftArm_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	frontClaw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	backClaw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

}

void disabled() {}

void competition_initialize() {}

/*
	AUTONOMOUS METHODS
	- moveFor(int)
	- armsMoveFor(int, bool)
	- rotateTo(int, bool)
*/

void moveFor (int ms, bool direction)
{
	int reverser = 1;
	if (!direction)
	{
		reverser = -1;
	}

	left_mtr.move_velocity(200);
	right_mtr.move_velocity(200);

	pros::delay(ms);
	left_mtr.move_velocity(0);
	right_mtr.move_velocity(0);
}

void armsMoveFor (int ms, bool direction)
{
	int reverser = 1;
	if (!direction)
	{
		reverser = -1;
	}

	rightArm_mtr.move_velocity(reverser*200);
	leftArm_mtr.move_velocity(reverser*200);

	pros::delay(ms);
	leftArm_mtr.move_velocity(0);
	rightArm_mtr.move_velocity(0);

}

void rotateTo (double degrees, bool direction)
{
	int reverser = 1;
	if (!direction){
		reverser = -1;
	}

	double distanceToRotate = degrees * (54.98/360);

	left_mtr.move_velocity(200*reverser);
	right_mtr.move_velocity(-200*reverser);

	pros::delay(distanceToRotate*23.866348);
	left_mtr.move_velocity(0);
	right_mtr.move_velocity(0);
}

/*
	OPERATOR METHODS
	-	dropFront()
	- dropBack()
	- openFront()
	- openBack()
*/

//drops front claw
void dropFront()
{
	if (frontClawOpen)
	{
		frontClaw.move_velocity(-200);
		pros::delay(110);
		frontClaw.move_velocity(0);
		frontClawOpen = false;
	}
}

//drops back claw
void dropBack()
{
	if (backClawOpen)
	{
		backClaw.move_velocity(-200);
		pros::delay(110);
		backClaw.move_velocity(0);
		backClawOpen = false;
	}
}

//open front claw
void openFront()
{
	if (!frontClawOpen)
	{
		frontClaw.move_velocity(200);
		pros::delay(110);
		frontClaw.move_velocity(0);
		frontClawOpen = true;
	}
}

//drops back claw
void openBack()
{
	if (!backClawOpen)
	{
		backClaw.move_velocity(200);
		pros::delay(110);
		backClaw.move_velocity(0);
		backClawOpen = true;
	}
}


//autonomous
void autonomous() {
	moveFor(1300, true);
	dropFront();

	moveFor(250, false);
	rotateTo(315, true);

	moveFor(787, false);
	dropBack();

}

//operator controls
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	while (true) {

		//move the arms up and down
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
		{
			rightArm_mtr.move_velocity(200);
			leftArm_mtr.move_velocity(200);
		}
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
		{
			rightArm_mtr.move_velocity(-200);
			leftArm_mtr.move_velocity(-200);
		}
		else
		{
			rightArm_mtr.move_velocity(0);
			leftArm_mtr.move_velocity(0);
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
		{
			dropFront();
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
		{
			dropBack();
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X))
		{
			openFront();
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
		{
			openBack();
		}

		//arcade control
		left_mtr.move(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X)/2);
		right_mtr.move (master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X)/2);

		pros::delay(20);
	}
}
