#pragma config(StandardModel, "EV3_REMBOT");
float speed=0;
float slewrate;
float totalleft=0;
float totalright=0;
float distance;
float reqdist;
bool stops=true;






void decrease(){
	if (speed-slewrate>=9){
		speed-=slewrate;
		}else{
		speed=9;
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
		if(distance-totalleft<=((((.136*speed)+2.4)*speed)*2)*(3/slewrate) || distance-totalright<=((((.136*speed)+2.4)*speed)*2)*(slewrate/3)){
			while (true){
				if(distance-totalleft<=((((.136*speed)+2.4)*speed)*2)*(3/slewrate) || distance-totalright<=((((.136*speed)+2.4)*speed)*2)*(slewrate/3)){
					decrease();
				}
			}
			}else{
			increase();
		}
	}
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
			stops=false;
		}
	}
}

void distancetravled(){
	float leftgonedeg;
	float rightgonedeg;
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
void reset(){
	speed=0;
	slewrate=3;
	totalleft=0;
	totalright=0;
	stops=true;
}

task test(){
	go(reqdist);
}
task slewTask(){
	slew();
}
task trav(){
	distancetravled();
}

void foward(float dist){
	reqdist=dist;
	startTask (trav,7);
	startTask (test,7);
	startTask (slewTask,7);
	while (true){
		if (!stops){
			stopTask(test);
			stopTask(trav);
			stopTask(slewTask);
			reset();
			return;
		}
	}
}




















float diameter=58;
float l=60;
float A;
float theta;



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

















float currentAngle;
float desiredAngle;
float desiredRadian;
float k;
float error;
float tolerance;
float angleRemaining;//this is a percentage .25 = 25% of angle left
float angleGone;//percentage of angle gone
int currentPower;
int desiredPower;
float turnRate;
bool decel=false;
float decelpower;
bool stopturn=false;


void reset2(){
	currentAngle=0;
	desiredAngle=0;
	desiredRadian=0;
	angleRemaining=0;
	angleGone=0;
	currentPower=0;
	decel=false;
	decelpower=0;
	stopturn=false;
	resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);

}


void decelerate(){
	int ratio;

	ratio = -100;

	if(desiredAngle<0){
		ratio = 100;
	}

	//float decelpower;
	while (true){
		decel=true;

		if(error<= tolerance){
			error=0;//this should stop the robot once error is within the tolerance range
			setMotorSync (leftMotor, rightMotor, ratio, 0);
			stopturn=true;
			return;
		}



		if( (k* error) < currentPower){
			decelpower=k*error;
		}
		else{
			decelpower=currentPower;
		}
		if( decelpower<7){
			decelpower=7;
		}
		setMotorSync (leftMotor, rightMotor, ratio, decelpower);
		sleep(50);
		//We probably don't want it to sleep at all, but it may not make a difference
	}

}



void accelerate(){
	// accelerate until full power or there is only a percentage of the angle left
	int ratio;

	ratio = -100;

	if (desiredAngle<0){
		ratio = 100;
	}

	//&&  (angleGone<angleRemaining)
	while((currentPower<desiredPower) ){
		currentPower+=turnRate;
		setMotorSync (leftMotor, rightMotor, ratio, currentPower);
		sleep(100);
	}

}



//void maintain(){
//	while(angleGone<angleRemaining){
//		sleep(20);//may need a setMotorSync here of it at full power at correct ratio
//	}
//}

task goes(){

	accelerate();

	// maintain(); this may be unnecessary due to the k*error calculation, if it is above 100 i assume it just puts the motor power at 100

	decelerate();

}

task updateangle(){
	while (true){

		currentAngle= getMotorEncoder(rightMotor)/2;
		angleGone = currentAngle/desiredAngle;
		error = abs(desiredAngle-currentAngle);

		// if(error<= tolerance){
		// 	error=0;
		// 	stopTask (updateangle);//maybe this should be stop all tasks
		// }
	}
}


void turn(float deg){
	if(deg>0){
		desiredAngle= deg+5;
	}else{
		desiredAngle= deg-7;
	}

	theta+= deg;
	startTask(goes, 7);
	startTask(updateangle, 7);
	while (true){
		if (stopturn){
			stopTask(goes);
			stopTask(updateangle);
			sleep(500);
			reset2();
			return;
		}
	}
}



task main(){
	diameter = 58;
	l = 60;
	k=.5;
	tolerance=1;
	angleRemaining = 1 - .25;
	turnRate = 3;
	currentPower=0;
	desiredPower=5;
	currentAngle=0;
	slewrate=3;
	startTask(display,7);



	turn(60);
	foward(500);
	turn(-90);
	foward(1000);
	turn(150);
	foward(300);


	stopAllTasks();










	while(true){}
}
