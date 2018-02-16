#pragma config(StandardModel, "EV3_REMBOT");
bool wait=true;
short ambient;
short reflected;
string color;

long redValue;
long greenValue;
long blueValue;



task updateVariables(){
	while(true){
		sleep(100);

		//ambient
		ambient = getColorAmbient(colorSensor);

		//reflection
		reflected = getColorReflected(colorSensor);

		//Name color
		switch(getColorName(colorSensor)){
		case colorBlack:
			color = "Black";
			break;

		case colorBlue:
			color = "Blue";
			break;

		case colorGreen:
			color = "Green";
			break;

		case colorYellow:
			color = "Yellow";
			break;

		case colorRed:
			color = "Red";
			break;

		case colorWhite:
			color = "White";
			break;

		case colorBrown:
			color = "Brown";
			break;

		default:
			color = "None";

		}


		// RGB values
		getColorRGB(colorSensor, redValue, greenValue, blueValue);
	}
}


task display(){
	while (true){
		if (wait){
			displayBigTextLine(2, "Wait");
			eraseDisplay();
			}else{
			displayBigTextLine(2, "No Wait");
			displayBigTextLine(4, "Ambient: %d ", ambient);
			displayBigTextLine(6, "Reflected: %d ", reflected);
			displayBigTextLine(8, "Color: %s", color);
			displayBigTextLine(12,"RGB: %d, %d, %d", redValue, greenValue, blueValue);
		}
	}
}

task checkbump(){
	while (true){
		if (getBumpedValue(touchSensor)==1){
			wait=false;
			}else{
			wait=true;
			resetBumpedValue(touchSensor);
		}
	}
}

task main(){
	startTask(checkbump,7);
	startTask(display,7);
	startTask(updateVariables, 7);

	while(true){}


}
