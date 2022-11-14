task main()
{
	int **ppInt;
	int *pInt;
	int Int=0;
	pInt = &Int;
	ppInt = &pInt;

	while(true)
	{
		eraseDisplay();
    for(int i=0;i<102;i++)
		{
			//Int=i;
			memset(pInt, i,1);
			//something less
			//displayString(nLineNumber, pChar, ...);
			displayString(0, "Int = %d", Int);
			displayString(1, "*pInt = %d", *pInt);
			displayString(2, "&Int = %x", &Int);
			displayString(3, "pInt = %x", pInt);
			displayString(4, "&pInt = %x", &pInt);
			//something more
			wait1Msec(1000);
		}
	}
}
