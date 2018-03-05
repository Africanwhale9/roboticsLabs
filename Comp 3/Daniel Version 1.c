#pragma config(StandardModel, "EV3_REMBOT");

int board_size_x	=6;										// change to correct value
int board_size_y	=4;										// change to correct value

int starting_point_x	=5;								// change to correct value
int starting_point_y	=3;								// change to correct value

int ending_point_x	=1;									// change to correct value
int ending_point_y	=2;									// change to correct value

float x_axis_y_intercepts_color	=25;		// change to correct value
float y_axis_x_intercepts_color	=35;		// change to correct value
float edege_color	=60;									// change to correct value
float board_color	=100;									// change to correct value

// how many intercepts there are
int x_axis_y_intercepts	=board_size_x-1;
int y_axis_x_intercepts	=board_size_y-1;

int number_of_left_right_need_to_go	=board_size_x-starting_point_x;
int number_of_up_down_need_to_go	=board_size_y-starting_point_y;


// calculations to know how many lines the robot needs to cross

int x_axis_y_intercepts_need_to_cross	=starting_point_x-ending_point_x;
int y_axis_x_intercepts_need_to_cross	=starting_point_y-ending_point_y;

int x_axis_y_intercepts_crossed	=0;
int y_axis_x_intercepts_crossed	=0;


// for color_sensor_updater

float current_color;


// bool to figure out direction

bool facing_up_or_down		=false;
bool facing_left_or_right	=false;
bool facing_edege					=false;

bool facing_left					=false;
bool facing_right					=false;
bool facing_up						=false;
bool facing_down					=false;











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
		facing_left_or_right=true;
	}
	else{
		if(current_color== y_axis_x_intercepts_color){
			facing_up_or_down=true;
		}
		else{
			if(current_color== edege_color){		// we know it is next to the border // we will call this again to find out which axis if it in next to an edege, or find if it is next to an edege
				facing_edege=true;
				turn(180);
				stopTask(color_sensor_updater);
				find_rough_direction();
			}
		}
	}
}

bool check_if_next_is_edege(){

}

void find_left_right(){
	int left_right_gone=0;
	startTask(color_sensor_updater,7);
	while (true){
		setMotorSync(leftMotor, rightMotor, 0, 15);
		if(current_color<=x_axis_y_intercepts_color+3 && current_color>=x_axis_y_intercepts_color-3){
			left_right_gone++;
			sleep(500);
		}
		if (left_right_gone== number_of_left_right_need_to_go){
			setMotorSync(leftMotor, rightMotor, 0, 0);
			sleep(500);
			stopTask(color_sensor_updater);
		}
	}
	if(
}

void find_up_down(){

}

void find_exact_direction(){
	if (facing_up_or_down){
		find_up_down();
		}else{
		find_left_right();
	}
}

task main(){
	find_rough_direction();
	find_exact_direction();


}
