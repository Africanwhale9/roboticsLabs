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

int	global_lines_gone	=0;
bool left_right;
bool up_down;
bool edge;
bool right;
bool left;








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





// get line color
void get_color(){
	left_right	=false;
	up_down	=false;
	edge	=false;
	float current_color = getColorReflected(colorSensor);
	if(current_color<= x_axis_y_intercepts_color+3 && current_color>= x_axis_y_intercepts_color-3){
		left_right=true;
	}
	else{
		if(current_color<= y_axis_x_intercepts_color+3 && current_color>= y_axis_x_intercepts_color-3){
			up_down=true;
		}
		else{
			edege_color	=true;
		}
	}
}


// to turn allong the x axis
void face_with_x(){
	get_color();
	if(up_down){
		turn(90);
	}
	return;
}

//goes to where we think the edge is
void go_to_edge(){
	int need_to_go;
	if(starting_point_x< grid_size_x-starting_point_x){
		need_to_go=starting_point_x;
		left=true;
	}
	else{
		need_to_go= grid_size_x-starting_point_x;
		right=true;
	}
	go(need_to_go);
}

//checks if this as an edge
void check_if_edge(){
	get_color();
	if(!edge){
		turn(180);
		go(global_lines_gone-1);
		global_lines_gone=0;
		go_to_edge();	// now at the edge
	}
}


task main(){
	go(1);
	face_with_x();
	go_to_edge();
	check_if_edge();
	if(right){
		turn(180);
		int goes=grid_size_x-end_point_x;
		go(goes);
		if(end_point_x>starting_point_x){
			turn(90);
			go(end_point_y-starting_point_y);
		}
		else{
			turn(-90);
			go(starting_point_y-end_point_y);
		}
	}
	else{
		turn(180);
		int goes=end_point_x;
		go(goes);
		if(end_point_x>starting_point_x){
			turn(-90);
			go(end_point_y-starting_point_y);
		}
		else{
			turn(90);
			go(starting_point_y-end_point_y);
		}
	}
	while(true){}

}
