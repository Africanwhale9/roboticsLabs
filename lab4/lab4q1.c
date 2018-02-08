int bumps;
bool touch;

task display(){
	while (true){
		if(touch){
			displayTextLine(10, "Pressed");
			bumps=0;
			}else{
			if(bumps){
				displayTextLine(10, "Numped");
			}
			else{
				displayTextLine(10, "Released");
				bumps=0;
			}
		}
	}
}



task sensor(){
	while (true){
		//bumps=getBumpedValue(ENTER PORT NUMBER);
		//touch=getTouchValue(ENTER PORT NUMBER);
	}
}



task main(){
	startTask(sensor,7);
	startTask(display,7);



}
