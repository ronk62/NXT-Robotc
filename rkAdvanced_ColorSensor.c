#pragma config(Sensor, S3,     colorPort,           sensorColorNxtFULL)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void displayRGBColorValues();
void displaySingleColorValue();
void displayPegCounts();

task buttonTask();

bool bDisplayPegCounts = false;

task main()
{
  //
  // Wait for Sensor to be connected.
  //
  displayCenteredBigTextLine(0, "Color");
  displayCenteredBigTextLine(2, "Sensor");
  displayCenteredBigTextLine(4, "Test");

  startTask(buttonTask);

  wait1Msec(200);  // Give sensor time to initialize

  nVolume = 2;
  while (true)
  {
	  //
    // Wait for the sensor to be initialized and connected. The sensor can take up to 250 milliseconds to
    // initialize.
    //
    // Re-initialization is required every time the sensor is changed to a different color sensor type
    //
    bool bFlashingState = true;

	  while (true)
	  {
	    if (validColorSensor(colorPort))
	      break;
	    if (bFlashingState)
	    {
	      displayCenteredTextLine(6, "Connect Sensor");
		    displayCenteredTextLine(7, "to Port S3");
		    playSound(soundBeepBeep);
		  }
	    else
	    {
	      displayCenteredTextLine(6, "");
		    displayCenteredTextLine(7, "");
		  }
	    bFlashingState = !bFlashingState;
	    wait1Msec(300);
		}

		//
    // Sensor is now connected.
    //
		playSound(soundFastUpwardTones);
    eraseDisplay();
    while (true)
		{
		  //
		  // Continuously loop. On every cycle of the loop display information on the current sensor state on the NXT LCD.
		  //
		  if (!validColorSensor(colorPort))
		    break;

		  if (bDisplayPegCounts)
		  {
		    displayPegCounts();
		  }
		  else
		  {
				displayTextLine(2, "");
		    switch (SensorType[colorPort])
		    {
		    case sensorColorNxtNONE:     displayCenteredBigTextLine(0, "Ambient");    displaySingleColorValue();  break;
		    case sensorColorNxtRED:      displayCenteredBigTextLine(0, "Red");        displaySingleColorValue();  break;
		    case sensorColorNxtGREEN:    displayCenteredBigTextLine(0, "Green");      displaySingleColorValue();  break;
		    case sensorColorNxtBLUE:     displayCenteredBigTextLine(0, "Blue");       displaySingleColorValue();  break;
		    case sensorColorNxtFULL:     displayCenteredBigTextLine(0, "RGB");        displayRGBColorValues();    break;
		    default:                  displayCenteredBigTextLine(0, "Bad Type");   break;
				}
			}
			wait1Msec(150);
		}

		// "validColorSensor" has failed. Either the sensor has been disconnected / transiently failed. Or the type of color
		// sensor has been changed by the "buttonTask".
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                              buttonTask
//
// A small task to switch between the various color sensor types using the left and right arrows.
//
// The "Enter" button is used to switch between displaying the sensor value and a display of diagnostic peg counts.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

task buttonTask()
{
  while (true)
  {
    wait1Msec(100);
    if (!validColorSensor(colorPort))
      continue;

    switch (nNxtButtonPressed)
    {
    case  kExitButton:
      stopAllTasks();
      break;


    case kEnterButton:
			//
			// Switch between sensor value display and diagnostic peg count mode
			//
			hogCPU();
			eraseDisplay();
			bDisplayPegCounts = !bDisplayPegCounts;
			releaseCPU();
			while (nNxtButtonPressed != kNoButton)
      {}
      break;

    case kRightButton:
			//
	    // "Increment" to the next valid color sensor type
	    //
	    switch (SensorType[colorPort])
	    {
	    case sensorColorNxtNONE:     SensorType[colorPort] = sensorColorNxtRED;         break;
	    case sensorColorNxtRED:      SensorType[colorPort] = sensorColorNxtGREEN;       break;
	    case sensorColorNxtGREEN:    SensorType[colorPort] = sensorColorNxtBLUE;        break;
	    case sensorColorNxtBLUE:     SensorType[colorPort] = sensorColorNxtFULL;        break;
	    default:
	    case sensorColorNxtFULL:     SensorType[colorPort] = sensorColorNxtNONE;        break;
			}
			while (nNxtButtonPressed != kNoButton)
      {}
      break;

    case kLeftButton:
			//
	    // "Decremet" to the previous valid color sensor type
	    //
			switch (SensorType[colorPort])
	    {
	    case sensorColorNxtNONE:     SensorType[colorPort] = sensorColorNxtFULL;       break;
	    case sensorColorNxtRED:      SensorType[colorPort] = sensorColorNxtNONE;       break;
	    case sensorColorNxtGREEN:    SensorType[colorPort] = sensorColorNxtRED;        break;
	    case sensorColorNxtBLUE:     SensorType[colorPort] = sensorColorNxtGREEN;      break;
	    default:
	    case sensorColorNxtFULL:     SensorType[colorPort] = sensorColorNxtBLUE;       break;
			}
			while (nNxtButtonPressed != kNoButton)
      {}
      break;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                              displayColorIndex
//
// When in RGB mode, the sensor tries to dtect one of six reflected colors. This function displays the detected color
// on the NXT LCD.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void displayColorIndex()
{
  string sColor;

  switch (SensorValue[colorPort])
  {
  case BLACKCOLOR:    sColor = "Black";     break;
  case BLUECOLOR:     sColor = "Blue";      break;
  case GREENCOLOR:    sColor = "Green";     break;
  case YELLOWCOLOR:   sColor = "Yellow";    break;
  case REDCOLOR:      sColor = "Red";       break;
  case WHITECOLOR:    sColor = "White";     break;
  default:            sColor = "???";       break;
  }
  displayCenteredTextLine(2, sColor);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                              displayRGBColorValues
//
// Displays all four possible color analog values on the NXT screen. The four values are:
//     Red          -- Analog value showing reflected light from Red   LED
//     Green        -- Analog value showing reflected light from Green LED
//     Blue         -- Analog value showing reflected light from Blue  LED
//     Ambient      -- No background LED turned on
//
// There are three numbers displayed for each color:
//     1. The raw analog value detected.
//     2. The "normalized" value detected. This is simply the raw value less the value of ambient light.
//     3. The "percentage" value detected. This is the "normalized" value scaled on a 0 to 100 (max value) scale.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void displayRGBColorValues()
{
	short nAtoDValues[4];
	short nRawValues[4];
	short nColorValues[4];
	string sTemp;

  getColorSensorData(colorPort, colorAtoD,   &nAtoDValues[0]);
  getColorSensorData(colorPort, colorRaw,    &nRawValues[0]);
  getColorSensorData(colorPort, colorValue,  &nColorValues[0]);

  displayColorIndex();

  displayTextLine(3, "   AtoD  Raw   %%");
  stringFormat(sTemp, "R  %4i%5i", nAtoDValues[0], nRawValues[0]);      displayTextLine(4, "%s%4i", sTemp, nColorValues[0]);
  stringFormat(sTemp, "G  %4i%5i", nAtoDValues[1], nRawValues[1]);      displayTextLine(5, "%s%4i", sTemp, nColorValues[1]);
  stringFormat(sTemp, "B  %4i%5i", nAtoDValues[2], nRawValues[2]);      displayTextLine(6, "%s%4i", sTemp, nColorValues[2]);
  stringFormat(sTemp, "Amb%4i%5i", nAtoDValues[3], nRawValues[3]);      displayTextLine(7, "%s%4i", sTemp, nColorValues[3]);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                              displaySingleColorValue
//
// Displays the analog value of a single color on the NXT LCD screen
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void displaySingleColorValue()
{
	word nAtoDValue;
	word nRawValue;
	word nColorValue;
	string sTemp;

	nAtoDValue    = 0;
	nRawValue     = SensorRaw[colorPort];
	nColorValue   = SensorValue[colorPort];

	displayTextLine(3, "   AtoD  Raw   %%");
	switch (SensorType[colorPort])
  {
  case sensorColorNxtRED:      stringFormat(sTemp, "R  %4i%5i", nAtoDValue, nRawValue);      displayTextLine(4, "%s%4i", sTemp, nColorValue);  break;
  case sensorColorNxtGREEN:    stringFormat(sTemp, "G  %4i%5i", nAtoDValue, nRawValue);      displayTextLine(5, "%s%4i", sTemp, nColorValue);  break;
  case sensorColorNxtBLUE:     stringFormat(sTemp, "B  %4i%5i", nAtoDValue, nRawValue);      displayTextLine(6, "%s%4i", sTemp, nColorValue);  break;
  case sensorColorNxtNONE:     stringFormat(sTemp, "Amb%4i%5i", nAtoDValue, nRawValue);      displayTextLine(7, "%s%4i", sTemp, nColorValue);  break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                              displayPegCounts
//
// ROBOTC firmware keeps "peg counts" on various error conditions within the Color Sensor device driver. This is a
// function to display these error counts on the NXT's LCD screen.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void displayPegCounts()
{
  /*
	typedef struct
	{
		long nDisconnects;						// Incremented once when sensor detection (see nWaitForConnect) fails after sensor has
																	// been connected.
		long nSoftwareResets;         // Another pin on sensor indicates internal error condition. Incremented once per erro.
		long nCRCMismatch;            // 50 bytes are uploaded from sensor on startup. A CRC-code is used to detect errors.
																	// Peg count incremented once for each time failure is detected.
		long nWaitForConnect;					// Counts the number of times the sensor is not detected. There's an I/O pin on the
																	// sensor cable indicating whether sensor is working OK. This peg count is incremented
																	// every millisecond when not connected when it is supposed to be connected. Usually
																	// triggered by removing the sensor in mid-operation.
		long nReadCount;              // Not an error! A count of the number of times data is read from the sensor.
	} TColorPegCounts;
	*/

	TColorPegCounts nPegCounts;

  getColorSensorPegCounts(colorPort, &nPegCounts);

  displayCenteredBigTextLine(0, "PegCounts");

	displayTextLine(2, "%5d Reads",        nPegCounts.nReadCount);
	displayTextLine(3, "");
	displayTextLine(4, "%5d Disconnect",   nPegCounts.nDisconnects);
	displayTextLine(5, "%5d Soft Reset",   nPegCounts.nSoftwareResets);
	displayTextLine(6, "%5d CRC Errs:",    nPegCounts.nCRCMismatch);
	displayTextLine(7, "%5d Wait 4 H/W",   nPegCounts.nWaitForConnect);
}
