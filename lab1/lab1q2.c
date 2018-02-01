#pragma config(StandardModel, "EV3_REMBOT")
task main()
{
	int first;
	int second;
	int answer;
	int i;

	first = 0;
	second = 1;
	i=0;

	while (i<20){

		answer = first + second;

		if(i<2){
			displayCenteredBigTextLine(8, "%d", i);
		}
		else{
			displayCenteredBigTextLine(8, "%d", answer);
			first = second;
			second = answer;
		}



		sleep(1000);
		i++;
	}


}
