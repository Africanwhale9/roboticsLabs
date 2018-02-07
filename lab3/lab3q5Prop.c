#pragma config(StandardModel, "EV3_REMBOT");

float mmperdeg;
float distToDeg;
float diameter=58;
float l=60;
float A;
float theta;


float speed;
float slewrate;
float totalleft=0;
float totalright=0;
float distance;
float disttomax=((((.136*speed+2.4)*speed)*(slewrate/3))*2);



void decrease(){
	if (speed-slewrate>=15){
		speed-=slewrate;
		}else{
		speed=10;
	}
	sleep(250);
}

void increase(){
	if (speed+slewrate<=100){
		speed+=slewrate;
		}else{
		speed=100;
	}
	sleep(250);
}

void slew(){
	while(true){
		disttomax=((((.136*speed)+2.4)*speed)*2)*(slewrate/3);
		if(totalleft>= distance/2){
			if(distance-totalleft<=disttomax || distance-totalright<=disttomax){
				decrease();
			}
		}else{
			increase();
		}
	}
}

void reset(){//use this to reset all of the variables that tasks in forward use
	speed=0;
	slewrate=3;
	totalleft=0;
	totalright=0;
}

void go(float dist){
	distance=dist;
	while (true){
		if(totalleft<distance || totalright<distance){
			setMotorSpeed(leftMotor, speed);
			setMotorSpeed(rightMotor, speed);
		}
		if (totalleft>=distance || totalright>=distance){
			speed=0;
			stopTask(trav);
			stopTask(slewTask);
			reset();
		}
	}
}

void distancetravled(){
	float leftgonedeg;
	float rightgonedeg;
	resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);
	int reset=0;
	while (true){
		leftgonedeg=getMotorEncoder(leftMotor);
		rightgonedeg=getMotorEncoder(rightMotor);
		if(reset>=200){
			reset=0;
			totalleft+=(leftgonedeg*0.51);
			totalright+=(rightgonedeg*0.51);
			resetMotorEncoder(leftMotor);
			resetMotorEncoder(rightMotor);
		}
		reset+=1;
	}
}


task slewTask(){
	slew();
}
task trav(){
	distancetravled();
}


void foward(float desiredDist){
	startTask (trav,10);
	startTask (slewTask,10);
	go(desiredDist);

}


void turn(float desiredAngle){
	theta+=desiredAngle;
	// insert working code from question 3 here

}



float getAngularVelRad(tMotor motor){
	float first;
	float second;
	first = getMotorEncoder(motor);
	sleep(10);
	second = getMotorEncoder(motor);
	float sign =  first-second;

	if (sign<0){
		return -((getMotorRPM(motor)*(2*PI))/60);
	}


	return ((getMotorRPM(motor)*(2*PI))/60);
}


task display(){
	float rightAngularVel;
	float leftAngularVel;
	while (true){
		leftAngularVel = getAngularVelRad(leftMotor);
		rightAngularVel =getAngularVelRad(rightMotor);

		A = (diameter/(2*l))*(rightAngularVel-leftAngularVel);

		displayTextLine(8, "Dot Theta: %f", A);
		displayTextLine(12, "Theta: %f", theta);

	}
}


task main(){
	startTask(display,10);

	turn(60);
	foward(500);
	turn(-90);
	foward(1000);
	turn(150);
	foward(300);





	while(true){}
}