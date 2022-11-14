task main()
{
	int Array1[] =
	{
		1,
		2,
		3,
	};

	int *pArray1=&Array1;

	while(true)
	{
		eraseDisplay();
		for(int i=0;i<102;i++)
		{
			memset(&Array1, i,1);
			pArray1=&Array1;
			for(int j=0;j<3;j++)
			{
				//displayString(nLineNumber, pChar, ...);
				displayString(0, "Array1[0] = %d", Array1[0]);
				displayString(1, "%x", &Array1[0]);
				displayString(2, "Array1[1] = %d", Array1[1]);
				displayString(3, "%x", &Array1[1]);
				displayString(4, "Array1[2] = %d", Array1[2]);
				displayString(5, "%x", &Array1[2]);
				//int val = Array1[j];
				displayString(6, "*pArray1 = %d", *pArray1);
				displayString(7, "pArray1 = %x", pArray1);
				//something more
				pArray1 ++;
			}
			wait1Msec(1000);
		}
	}
}
