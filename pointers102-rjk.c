task main()
{
	int **ppInt;
	int *pInt;
	int Int=0;
	pInt = &Int;
	ppInt = &pInt;

	ubyte Array1[] =
	{
		1,
		2,
		3,
	};

	long pArray1=&Array1;

	while(true)
	{
		eraseDisplay();
		for(int i=0;i<102;i++)
		{
			//Int=i;
			memset(&Array1, i,1);
			//displayString(nLineNumber, pChar, ...);
			displayString(0, "Array1[0] = %d", Array1[0]);
			displayString(1, "*Array1[0] = %d", Array1[0]);
			displayString(2, "Array1[1] = %d", Array1[1]);
			displayString(3, "*Array1[1] = %d", Array1[1]);
			displayString(4, "Array1[2] = %d", Array1[2]);
			displayString(5, "*Array1[2] = %d", Array1[2]);
			displayString(6, "&Array1 = %x", &Array1);
			displayString(7, "pArray1 = %x", pArray1);
			//something more
			wait1Msec(1000);
		}
	}
}
