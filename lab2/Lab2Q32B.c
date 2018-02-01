#pragma config(StandardModel, "EV3_REMBOT")
int Diameter;
int left;
int bottom;
int top;
int right;
int vertv;
int horv;

int desiredPowerL;
int desiredPowerR;
int currentPowerR;
int currentPowerL;
int increment;
int speed;

int Ldist;
int Rdist;
int circumference;
long LencoderGone;
long RencoderGone;
int Ltempval;
int Rtempval;

void switchvertdir(){
	vertv = vertv*-1;
	if (top>=128){
		desiredPowerL=speed;
		desiredPowerR=speed;  //these should be switch, bottom should be -speed, top should just be speed
	}
	if(bottom<=0){
		desiredPowerL=speed*-1;
		desiredPowerR=speed*-1;
	}
}

void switchhordir(){
	horv = horv*-1;
	if(left<=0){
			desiredPowerL=speed*-1;
			desiredPowerR=speed;
	}
	if(right>=178){
			desiredPowerL=speed;
			desiredPowerR=speed*-1;
	}
}

void sound(){
	playSound(soundBlip);
	sleep(1000);
}

void update_points(){
	left = left + horv;
	bottom = bottom+vertv;

	right = left + Diameter;
	top = bottom+Diameter;
}

void doellipse(){

	drawEllipse(left,top,right,bottom);
	if(left<=0 || right >=178){
		switchhordir();
		sound();
	}
	if(bottom<=0 || top >=128){
		switchvertdir();
		sound();
	}

	sleep(100);
	eraseEllipse(left,top,right,bottom);
	update_points();
}

void choose_vel(){
	horv = rand() % (3 + 1 - -3) + -3;
	vertv = rand() % (3 + 1 - -3) + -3;
}
task doel(){
	while (true){
		doellipse();
	}
}


void motorspeed(int desiredPower, int *currentPower){
		while (*currentPower != desiredPower){
			if (desiredPower!=*currentPower){
				if (*currentPower < desiredPower){
					if(*currentPower + increment > desiredPower){
						*currentPower = desiredPower;
					}
					else{
						*currentPower += increment;
					}
				}
				else{
					if(*currentPower - increment > desiredPower){
						*currentPower = desiredPower;
					}
					else{
						*currentPower -= increment;
					}
				}
			}
			sleep(500);
		}

	}

void go(){
	setMotorSpeed(leftMotor, currentPowerL);
	setMotorSpeed(rightMotor, currentPowerR);
}


task motorControlTaskL()
{
	while (true){
		motorspeed(desiredPowerL,&currentPowerL);
	}
}

task motorControlTaskR()
{
	while (true){
		motorspeed(desiredPowerR,&currentPowerR);
	}
}

task gotask()
{
	while (true){
	go();
	}
}

task display()
{
	while (true){
		displayTextLine(15,"LDist: %d mm",Ldist);
		displayTextLine(12,"RDist: %d mm",Rdist);
	}
}


task wheelDistance()
{
	circumference = 175.93;

	while(true){
		LencoderGone = getMotorEncoder(leftMotor);
		RencoderGone = getMotorEncoder(rightMotor);
		
		Ltempval=LencoderGone;
		Rtempval=RencoderGone;
				
		
		Ldist+= abs((Ltempval/360)*circumference);
		Rdist+= abs((Rtempval/360)*circumference);
	}
}


task main()
{
	Diameter=30;
	left = 50;
	bottom = 50;
	right = Diameter+left;
	top = Diameter+bottom;
	choose_vel();
	startTask (doel,10);

	speed= (sqrt(horv*horv)+(vertv*vertv))*10;
	currentPowerL=0;
	desiredPowerL=speed;
	currentPowerR=0;
	desiredPowerR=speed;
	increment=20;
	startTask(motorControlTaskL, 10);
	startTask(motorControlTaskR, 10);
	startTask(gotask,10);
	startTask(wheelDistance, 10);
	startTask(display, 10);

	while (true){
	}


}