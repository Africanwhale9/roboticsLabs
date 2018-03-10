#pragma config(StandardModel, "EV3_REMBOT");

int starting_point_x	=1;			// change to correct value
int starting_point_y	=1;			// change to correct value

int end_point_x	=4;						// change to correct value
int end_point_y	=2;						// change to correct value

int grid_size_x	=5;						// change to correct value
int grid_size_y	=3;						// change to correct value

TLegoColors x_axis_y_intercepts_color	=colorBlue;		// change to correct value VERTICAL
TLegoColors y_axis_x_intercepts_color	=colorRed;		// change to correct value HORIZONTAL
TLegoColors edege_color	=colorBlack;									// change to correct value
TLegoColors board_color	=colorWhite;									// change to correct value

int	global_lines_gone	=0;
bool left_right;
bool up_down;
bool edge;
bool right;
bool left;

int motorSpeed = 15;



void stopMoving(){
	setMotorSync(leftMotor,rightMotor, 0, 0);
}


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
	startTask(monitor_deg_of_turn);
	turn_is_done=false;
	degree_global=degree;
	ratio_finder();

	sleep(100);
	while (true){
		setMotorSync(leftMotor, rightMotor, ratio, motorSpeed);
		if(abs(degree_turned_so_far)>= abs(degree)){
			setMotorSync(leftMotor, rightMotor, 0, 0);
			stopTask(monitor_deg_of_turn);
			turn_is_done=true;
			return;
		}
	}
}







// go past X lines
void go(int lines_need_to_go){
	float current_color;
	global_lines_gone=0;
	int tmpGone = global_lines_gone;
	while (global_lines_gone< lines_need_to_go+tmpGone){//this should be <=, currently it will stop one box before. It needs to detect the far line of the box, not the close line
		current_color = getColorName(colorSensor);
		setMotorSync(leftMotor,rightMotor,0,motorSpeed);
		if(current_color!=board_color){
			global_lines_gone++;
			if(global_lines_gone< lines_need_to_go){//if it hasn't gone enough lines it'll sleep so it gets off the line. Otherwise, it should stop on the line
				sleep(1000);
			}
		}
	}

	stopMoving();
	return;
}





// get line color
void get_color(){
	left_right	=false;
	up_down	=false;
	edge	=false;
	float current_color = getColorName(colorSensor);
	if(current_color!= x_axis_y_intercepts_color){
		left_right=true;
	}
	else{
		if(current_color!= y_axis_x_intercepts_color){
			up_down=true;
		}
		else{
			edge	=true;
		}
	}
}


// to turn allong the x axis
void face_with_x(){
	get_color(); //COMMENTED OUT FOR TESTING, ASSUME THAT IT IS FACING DOWN X AXIS
	//left_right = true;//THIS IS ADDED FOR TESTING
	if(up_down){
		turn(90);
	}
	return;
}

//goes to where we think the edge is
void go_to_edge(){//no idea what this function is doing
	int need_to_go;
	left=false;
	right=false;
	if(starting_point_x< grid_size_x-starting_point_x){
		need_to_go=starting_point_x;
		left=true;
	}
	else{
		need_to_go= grid_size_x-starting_point_x;
		right=true;
	}
	go(need_to_go+1);//how does it know what direction to go here? Wouldn't it need to turn?
}

//checks if this as an edge
void check_if_edge(){
	get_color();
	if(!edge){
		turn(180);
		sleep(1000);
		go(global_lines_gone+1);
		global_lines_gone=0;
		go_to_edge();	// now at the edge
	}
}

task display(){
	while(true){
		displayBigTextLine(3, "lines gone: %d", global_lines_gone);
	}
}


task main(){
	startTask(display, 7);
	go(1);
	face_with_x();//makes it face down the x axis
	go_to_edge();
	check_if_edge();
	//if(right){
	//	turn(180);
	//	int goes=grid_size_x-end_point_x;
	//	go(goes);
	//	if(end_point_y>starting_point_y){//shouldn't these be y's?
	//		turn(90);
	//		go(end_point_y-starting_point_y);
	//	}
	//	else{
	//		turn(-90);
	//		go(starting_point_y-end_point_y);
	//	}
	//}
	//else{
	//	turn(180);
	//	int goes=end_point_x;
	//	go(goes);
	//	if(end_point_y>starting_point_y){//here also
	//		turn(-90);
	//		go(end_point_y-starting_point_y);
	//	}
	//	else{
	//		turn(90);
	//		go(starting_point_y-end_point_y);
	//	}
	//}
	while(true){}

}


//task main(){
	//startTask(display, 7);
	//go(3);

	//turn(90);

	//turn(-90);

	//turn(180);

	//turn(-180);

//}
