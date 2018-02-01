int i;

task math()
{
	i=0;
	while(true){
		if (getButtonPress(buttonRight)){
			i++;
		}
		else if (getButtonPress(buttonLeft)){
			i--;
		}
		sleep(250);
	}

}




task display()
{


	displayCenteredTextLine(1, "Current Value:");

	while(true){

		if (getButtonPress(buttonUp)||getButtonPress(buttonDown)){
			stopAllTasks();
		}

		else{
			displayCenteredBigTextLine(4, "%d", i);
		}







		sleep(250);
	}



}




task main(){
	startTask(math, 10);
	startTask(display, 10);
	while(true){}
}
