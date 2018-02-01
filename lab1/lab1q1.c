#pragma config(StandardModel, "EV3_REMBOT")
task main()
{
	int i;
	i=1;
	while (i<=10){

		displayTextLine(i,"%d",i*i);
		i++;
	}


	sleep(10000);


}
