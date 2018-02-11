#pragma config(StandardModel, "EV3_REMBOT");

int bumps;
bool touch;

task display(){
	while (true){
		if(touch){
			displayTextLine(10, "Pressed");
			}else{
			if(bumps && !touch){
				displayTextLine(10, "Bumped");
			}
			else{
				displayTextLine(10, "Released");
			}
		}
	}
}



task sensor(){
	while (true){
		bumps=getBumpedValue(touchSensor);
		if(bumps>2){
			resetBumpedValue(touchSensor);
			bumps = 0;
		}
		touch=getTouchValue(touchSensor);
	}
}



task main(){
	startTask(sensor,7);
	startTask(display,7);

	while(true){}



}
