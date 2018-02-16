#pragma config(StandardModel, "EV3_REMBOT");

#define DATALOG_SERIES_0 0
#define DATALOG_SERIES_1 1

long degree;
long heading;
long rate;

const int bufferSize = 6;
int gyroBuffer[bufferSize];


void insertGyroBuffer(int var){
	int i;
	for(i=0;i<bufferSize-1;i++){
		gyroBuffer[i]=gyroBuffer[i+1];
	}
	gyroBuffer[bufferSize-1]=var;
}

int average(){
	int x = 0;
	int i;
	for(i=0;i<bufferSize-1;i++){
		x+=gyroBuffer[i];
	}
	return x/bufferSize;
}




	task updateVariables(){
		while(true){
			int avg;
			int rateTMP = getGyroRate(gyroSensor)
			insertGyroBuffer(rateTMP);
			datalogAddValue(DATALOG_SERIES_1, rateTMP);

			degree= getGyroDegrees(gyroSensor);
			heading= getGyroHeading(gyroSensor);
			avg = average();
			datalogAddValue(DATALOG_SERIES_0, avg);
			rate= avg;

		}
	}


	task display(){
		while (true){
			displayBigTextLine(2, "Degree: %d", degree);
			displayBigTextLine(4, "Heading: %d ", heading);
			displayBigTextLine(6, "Rate: %d ", rate);
		}
	}

	task main(){
		resetGyro(gyroSensor);
		startTask(display,7);
		startTask(updateVariables, 7);

		while(true){}


	}
