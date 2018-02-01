
task main()
{
	int i;
	i =0;
	displayCenteredTextLine(1, "Current Value:");

	while(true){
		if (getButtonPress(buttonRight)){
			i++;
			displayCenteredBigTextLine(4, "%d", i);
		}
		else if (getButtonPress(buttonLeft)){
			i--;
			displayCenteredBigTextLine(4, "%d", i);
		}

		else if (getButtonPress(buttonUp)||getButtonPress(buttonDown)){
			return;
		}







		sleep(350);
	}



}
