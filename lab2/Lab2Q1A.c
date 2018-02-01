int desiredPower;
int currentPower;
int increment;


task display()
{
	while (true){
		displayBigTextLine(8,"%d/%d",currentPower, desiredPower);
	}
}

task motorControlTask()
{
	while (true){
		if (desiredPower!=currentPower){
			if (currentPower < desiredPower){
				if(currentPower + increment > desiredPower){
					currentPower = desiredPower;
				}
				else{
					currentPower += increment;
				}
			}
			else{
				if(currentPower - increment > desiredPower){
					currentPower = desiredPower;
				}
				else{
					currentPower -= increment;
				}
			}
		}
		sleep(100);
	}
}





task main()
{
	currentPower=0;
	desiredPower=100;
	increment=1;
	startTask(display, 10);
	startTask(motorControlTask, 10);
	while(true){}
}
