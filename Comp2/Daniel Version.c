#pragma config(StandardModel, "EV3_REMBOT");

//to go a set dist. If range finder can not find anything this will be used to go 255mm.
float total_dist_left=0;
float total_dist_right=0;

task monitor_dist_traveled(){
	float left_wheel_dist_encoder;
	float right_wheel_dist_encoder;

	int reset_encoder_counter=0;

	resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);

	while (true){
		left_wheel_dist_encoder=getMotorEncoder(leftMotor);
		right_wheel_dist_encoder=getMotorEncoder(rightMotor);
		if(reset_encoder_counter>=200){
			reset_encoder_counter=0;
			total_dist_left+=(left_wheel_dist_encoder*0.51);
			total_dist_right+=(right_wheel_dist_encoder*0.51);
			resetMotorEncoder(leftMotor);
			resetMotorEncoder(rightMotor);
		}
		reset_encoder_counter+=1;
	}
}

void go(float distance){
	startTask(monitor_dist_traveled,7);
	distance=distance/10; //mm to cm
	setMotorSpeed(leftMotor, 20);
	setMotorSpeed(rightMotor, 20);

	while (true){
		if(total_dist_left== distance || total_dist_right==distance){
			stopTask(monitor_dist_traveled);
			return;
		}
	}
}





// To turn a certian degree

float degree_turned_so_far;
int degree_global;
int ratio;

task monitor_deg_of_turn(){
	float starting_deg_robot_facing=getGyroDegrees(gyroSensor);
	degree_turned_so_far=0;
	while (true){
		degree_turned_so_far= getGyroDegrees(gyroSensor)-starting_deg_robot_facing;
	}
}
task error_task(){
	int error;
	while (true){
		error=degree_turned_so_far-degree_global;
		if(error>=1){
			ratio=100;
			}else{
			ratio=-100;
		}

	}
}
void turn(int degree){
	degree_global=degree;
	startTask(error_task,7);
	startTask(monitor_deg_of_turn);
	while (true){
		setMotorSync(leftMotor, rightMotor, ratio, 20);
		if(degree_turned_so_far== degree){
			stopTask(monitor_deg_of_turn);
			return;
		}
	}
}



// Sonar monitor, 10 readings a second.

float current_dist_from_object;

float dist_1=0;	float dist_2=0;	float dist_3=0;	float dist_4=0;	float dist_5=0;	float dist_6=0;	float dist_7=0;	float dist_8=0;	float dist_9=0;	float dist_10=0;

task average(){
	while (true){
		current_dist_from_object=((dist_1+dist_2+dist_3+dist_4+dist_5+dist_6+dist_7+dist_8+dist_9+dist_10)/10);
	}
}

task monitor_current_dist(){

	startTask(average,7);

	while (true){
		dist_1=getUSDistance(sonarSensor);
		sleep(100);
		dist_2=getUSDistance(sonarSensor);
		sleep(100);
		dist_3=getUSDistance(sonarSensor);
		sleep(100);
		dist_4=getUSDistance(sonarSensor);
		sleep(100);
		dist_5=getUSDistance(sonarSensor);
		sleep(100);
		dist_6=getUSDistance(sonarSensor);
		sleep(100);
		dist_7=getUSDistance(sonarSensor);
		sleep(100);
		dist_8=getUSDistance(sonarSensor);
		sleep(100);
		dist_9=getUSDistance(sonarSensor);
		sleep(100);
		dist_10=getUSDistance(sonarSensor);
		sleep(100);
	}
}


int dist_from_first_sonar;
int dist_from_wheels;
int dist_from_second_sonar;
int total_dist=0;

task main(){
	startTask(monitor_current_dist,7);


	// square up





	// Turn and go to the next wall
	turn(180);
	while(current_dist_from_object>=150){
		go(140);
		dist_from_wheels+=140;
	}
	dist_from_second_sonar=current_dist_from_object;
	total_dist=dist_from_first_sonar+dist_from_wheels+dist_from_second_sonar;

	while(total_dist==0){}
	sleep(5000);
}
