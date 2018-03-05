#pragma config(StandardModel, "EV3_REMBOT");


// grid size, number of intercepts inside the grid, not inculding the exterior walls

int x_axis_y_intercepts	=5;							// change to correct value
int y_axis_x_intercepts	=3;							// change to correct value


// grid line color values

float x_axis_y_intercepts_color	=25;		// change to correct value
float y_axis_x_intercepts_color	=35;		// change to correct value
float edege_color	=60;									// change to correct value
float board_color	=100;									// change to correct value


// starting and end points

int starting_point_x	=5;								// change to correct value
int starting_point_y	=3;								// change to correct value

int ending_point_x	=1;									// change to correct value
int ending_point_y	=2;									// change to correct value


// calculations to know how many lines the robot needs to cross

int x_axis_y_intercepts_need_to_cross	=starting_point_x-ending_point_x;
int y_axis_x_intercepts_need_to_cross	=starting_point_y-ending_point_y;

int x_axis_y_intercepts_crossed	=0;
int y_axis_x_intercepts_crossed	=0;


// bool to figure out direction

bool facing_up_or_down		=false;
bool facing_left_or_right	=false;
bool facing_edege					=false;

bool facing_left					=false;
bool facing_right					=false;
bool facing_up						=false;
bool facing_down					=false;


// for color_sensor_updater

float current_color;

// To turn a certian degree
float degree_turned_so_far;
int degree_global;
int ratio;
bool turn_is_done;

task monitor_deg_of_turn(){
	float starting_deg_robot_facing=getGyroDegrees(gyroSensor);
	degree_turned_so_far=0;
	while (true){
		degree_turned_so_far= getGyroDegrees(gyroSensor)-starting_deg_robot_facing;
	}
}
void ratio_finder(){
	int error_turn;
	error_turn=degree_turned_so_far-degree_global;
	if(error_turn>=1){
		ratio=-100;
		}else{
		ratio=100;
	}
	return;
}
void turn(int degree){
	turn_is_done=false;
	degree_global=degree;
	ratio_finder();
	startTask(monitor_deg_of_turn);
	while (true){
		setMotorSync(leftMotor, rightMotor, ratio, 20);
		if(degree_turned_so_far>= degree){
			setMotorSync(leftMotor, rightMotor, 0, 0);
			stopTask(monitor_deg_of_turn);
			turn_is_done=true;
			return;
		}
	}
}


// update the color sensor, will be started whenver it is needed, then ended when it is no longer needed
task color_sensor_updater(){
	while(true){
		current_color = getColorReflected(colorSensor);
	}

}

void find_rough_direction(){
	startTask(color_sensor_updater,7);
	sleep(500);	//let color sensor start
	while (current_color <= board_color+3 && current_color >= board_color-3){ // gives a range for the color sensor to fluctuate
		setMotorSync(leftMotor,rightMotor,0,15);
	}
	if(current_color== x_axis_y_intercepts_color){
		if (facing_left_or_right){
			stopTask(color_sensor_updater);
			return;
			}else{
			facing_left_or_right=true;
		}
	}
	else{
		if(current_color== y_axis_x_intercepts_color){
			if (facing_up_or_down){
				stopTask(color_sensor_updater);
				return;
			}
			else{
				facing_up_or_down=true;
			}
		}
		else{
			if(current_color== board_color){		// we know it is next to the border
				facing_edege=true;
			}
		}
	}
	// we will call this again to find out which axis if it in next to an edege, or find if it is next to an edege
	turn(180);
	stopTask(color_sensor_updater);
	find_rough_direction();
}

void find_left_right(){

}

void find_up_down(){

}

//void find_exact_direction(){
//	if (){

//	}
//}

task main(){
	find_rough_direction();


}
