int Diameter;
int left;
int bottom;
int top;
int right;
int vertv;
int horv;

void switchvertdir(){
	vertv = vertv*-1;
}

void switchhordir(){
	horv = horv*-1;
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




task main()
{

	Diameter=30;

	left = 0;

	bottom = 0;

	right = Diameter+left;

	top = Diameter+bottom;

	choose_vel();

	while (true){
		doellipse();
	}


}
