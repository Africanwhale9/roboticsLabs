#pragma config(StandardModel, "EV3_REMBOT");

int starting_point_x	=7;			// change to correct value
int starting_point_y	=2;			// change to correct value

int end_point_x	=2;						// change to correct value
int end_point_y	=4;						// change to correct value

int grid_size_x	=36;						// change to correct value
int grid_size_y	=72;						// change to correct value

float x_axis_y_intercepts_color	=25;		// change to correct value
float y_axis_x_intercepts_color	=35;		// change to correct value
float edege_color	=60;									// change to correct value
float board_color	=100;									// change to correct value

int x_axis_y_intercept_needed	=starting_point_x-end_point_x;
int y_axis_x_intercept_needed	=starting_point_y-end_point_y;

int	global_lines_gone	=0;
bool left_right	=false;
bool up_down	=false;








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
		setMotorSync(leftMotor, rightMotor, ratio, 10);
		if(degree_turned_so_far>= degree){
			setMotorSync(leftMotor, rightMotor, 0, 0);
			stopTask(monitor_deg_of_turn);
			turn_is_done=true;
			return;
		}
	}
}







// go past X lines
void go(int lines_need_to_go){
	float current_color = getColorReflected(colorSensor);
	while (global_lines_gone!= lines_need_to_go){
		current_color = getColorReflected(colorSensor);
		setMotorSync(leftMotor,rightMotor,0,10);
		if((!(current_color<= board_color+3)) && (!(current_color>= board_color-3))){
			global_lines_gone++;
			sleep(500);
		}
	}
	return;
}



task main()
{



}
