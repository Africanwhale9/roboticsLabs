#pragma config(StandardModel, "EV3_REMBOT");

int starting_point_x	=4;			// change to correct value
int starting_point_y	=0;			// change to correct value

int end_point_x	=0;						// change to correct value
int end_point_y	=1;						// change to correct value

int grid_size_x	=4;						// change to correct value
int grid_size_y	=5;						// change to correct value

float x_axis_y_intercepts_color	=25;		// change to correct value
float y_axis_x_intercepts_color	=35;		// change to correct value
float edege_color	=60;									// change to correct value
float board_color	=100;									// change to correct value

int x_axis_y_intercept_to_cross	=starting_point_x-end_point_x;
int y_axis_x_intercept_to_cross	=starting_point_y-end_point_y;

int min_intersect_to_edge_x;
int min_intersect_to_edge_y;

bool x_axis =false;
bool y_axis =false;

bool left	=false;
bool right =false;
bool up =false;
bool down =false;










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










// first steps, to figure out orientation

void min_intersect_to_edge(){
	if(grid_size_x-starting_point_x < starting_point_x){
		min_intersect_to_edge_x=grid_size_x-starting_point_x;
	}
	else{
		min_intersect_to_edge_x=starting_point_x;
	}
	if(grid_size_y-starting_point_y < starting_point_y){
		min_intersect_to_edge_y=grid_size_y-starting_point_y;
	}
	else{
		min_intersect_to_edge_y=starting_point_y;
	}
	return;
}

void find_axis(){
	float current_color = getColorReflected(colorSensor);
	while(current_color <= board_color+3 && current_color >= board_color-3){
		current_color = getColorReflected(colorSensor);
		setMotorSync(leftMotor,rightMotor,0,15);
	}
	if(current_color<= x_axis_y_intercepts_color+3 && current_color>= x_axis_y_intercepts_color-3){
		x_axis=true;
	}
	else{
		if(current_color<= y_axis_x_intercepts_color+3 && current_color>= y_axis_x_intercepts_color-3){
			y_axis=true;
		}
		else{
			turn(180);
			find_axis();
		}
	}
}

bool check_if_edge(){
	float current_color = getColorReflected(colorSensor);
	while(current_color <= board_color+3 && current_color >= board_color-3){
		current_color = getColorReflected(colorSensor);
		setMotorSync(leftMotor,rightMotor,0,15);
	}
	if(current_color<= board_color+3 && current_color>= board_color-3){
		return true;
	}
	else{
		return false;
	}
}

void left_or_right(){
	int gone_so_far=0;
	float current_color = getColorReflected(colorSensor);
	while (gone_so_far!= min_intersect_to_edge_x){
		current_color = getColorReflected(colorSensor);
		setMotorSync(leftMotor,rightMotor,0,15);
		if(current_color<= x_axis_y_intercepts_color+3 && current_color>= x_axis_y_intercepts_color-3){
			gone_so_far++;
			sleep(500);
		}
	}
	bool is_edge= check_if_edge();
	if(is_edge){

	}
}

void up_or_down(){

}

void find_specific_dir(){
	if(min_intersect_to_edge_x<min_intersect_to_edge_y){
		if(x_axis){
			left_or_right();
		}
		else{
			turn(90);
			left_or_right();
		}
	}
	else{
		if(y_axis){
			up_or_down();
		}
		else{
			turn(90);
			up_or_down();
		}
	}
}




task main(){
	min_intersect_to_edge();
	find_axis();
	find_specific_dir();





}
