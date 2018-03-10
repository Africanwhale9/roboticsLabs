#pragma config(StandardModel, "EV3_REMBOT");
task main()
{
	while(true){
		string color;
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

		displayBigTextLine(1, "%s", color);
	}



}
