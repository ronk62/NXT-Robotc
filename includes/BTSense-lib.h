/**

Port of BTSense-lib.nxc to BTSense-lib.h RobotC

Ron King, Sep 2017

*/



/************************************************************************/
/*                                                                      */
/* Program Name: BTSense-lib.nxc */
/* =============================                                        */
/*                                                                      */
/* Copyright (c) 2013 by mindsensors.com                                */
/* Email: info (<at>) mindsensors (<dot>) com                           */
/*                                                                      */
/* This program is free software. You can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation; version 3 of the License.              */
/* Read the license at: http://www.gnu.org/licenses/gpl.txt             */
/*                                                                      */
/************************************************************************/
/*
* When        Who             Comments
* 07/09/2013  Deepak Patil    Initial authoring.
*/

#ifndef BTS_LIB
#define BTS_LIB


struct bts_gps
{
	int latt_d;
	int latt_m;
	int latt_s;

	int long_d;
	int long_m;
	int long_s;
};

struct bts_3axes
{
	int   ax;
	int   ay;
	int   az;
	int error;
};

#define LEN 64
#define LENFULL 126
#define LENTOLOOK 100

long tick_before, tick_after;

static bool reader_task_started=false;
string bt_buff_01;
string bt_buff_02;
string bt_buff_03;
string bt_buff_temp;
static short tr_id;


void BTS_Wait (int connection){  //wait until BT is idle
	//until(BluetoothStatus(connection)==NO_ERR);
	while (!BluetoothStatus(connection)==NO_ERR)
	{
		// wait
	}


}

int BTstring_keep(string in, short lenToRead, byte pp, string &s)
{
	short len, i, j;

	//len = StrLen(in);
	len = LENFULL;

	for(i=LENTOLOOK; i > 0; i--) {
		if ( in[i] == 'D' && in[i+1] == ':' ) {
			if ( in[i+2] == pp ) {
				//s = SubStr(in, i, len-i);
				// WIPronk  stringFromChars(s, in[i to );
				return i;
				} else {
				i -= 10;  // that's about minimum length of the data packet
			}
		}
	}
	return false;
}

void read_from_bt(byte pp)
{
	string in, in2, in_full;
	int retVal;
	string s;
	byte ptr;

	GetBTInputBuffer(0, LEN, in);
	GetBTInputBuffer(63, LEN, in2);
	ptr = BTInputBufferInPtr();
	SetBTInputBufferInPtr(0);

	if ( ptr == 0 ) {
		// join the two buffers.
		in_full = strcat(in, in2);
		} else {
		return;
	}
	//AppendLineToFile("debuglog.csv", format_hex(ptr) + " 00>"+ str_to_hex(in_full) +"<");

	retVal = BTstring_keep(in_full, LEN, pp, s);
	if ( retVal != false ) {
		switch (pp) {
		case '1':
			bt_buff_01 = s;
			break;
		case '2':
			bt_buff_02 = s;
			break;
		case '3':
			bt_buff_03 = s;
			break;
		}
	}
	return;
}

int BTS_MLBToInteger(byte msb, byte lsb) {
	int i;
	i = lsb + (msb<<8);
	return i;
}

unsigned long BTS_ByteArrayToLong(string in)
{
	int i;
	unsigned long factor = 1;
	unsigned long result;
	result = 0;
	for (i=3; i>=0; i--) {
		result += in[i]*factor;
		factor = factor * 256;
	}
	return result;
}

void BTS_Disconnect(byte connection, string phoneName)
{
	CommBTConnectionType args;
	args.Name = phoneName;
	args.ConnectionSlot = connection;
	args.Action = false;  // disconnect
	if(!BluetoothStatus(connection)==NO_ERR)
		SysCommBTConnection(args);
}

void BTS_SendCommand(byte conn, string command)
{
	byte b_array[];

	ArrayInit(b_array, 0, 59);
	StrToByteArray(command, b_array);
	BTS_Wait(conn);
	SendRemoteString(conn, 0, b_array);
	Wait(5);
	BTS_Wait(conn);
}

void BTS_Identify(byte connection)
{
	string s;
	s = "@:xIN";
	s[2] = tr_id++;
	BTS_SendCommand(connection, s);
}

bool BTS_Connect(byte connection, string phoneName)
{
	bool connected;
	CommBTConnectionType args;
	args.Name = phoneName;
	args.ConnectionSlot = connection;
	args.Action = TRUE;
	connected = false;
	int i;
	i = 0;
	if(BluetoothStatus(connection) != NO_ERR)
	{
		SysCommBTConnection(args); // try to connect.
		//
		//  Wait for 2000 milli-seconds for the connection to establish.
		//
		while ( i < 2000 && connected == false ) {
			TextOut(0, LCD_LINE7, "Connecting to...", false);
			TextOut(0, LCD_LINE8, phoneName, false);
			Wait(10);
			if (BluetoothStatus(connection) == NO_ERR) {
				//                TextOut(27, LCD_LINE3, "Connected",true);
				connected = true;
			}
			i++;
		}
		if ( i > 1999 ) {
			TextOut(0, LCD_LINE3, "Connect Failed",true);
			NumOut(45, LCD_LINE5, args.Result);
		}
		Wait(1000);
		} else {
		// already connected.
		connected = true;
	}
	TextOut(0, LCD_LINE7, "                ", false);
	TextOut(0, LCD_LINE8, "                ", false);
	BTS_Wait(connection);
	BTS_Identify(connection);
	return (connected);
}

void BTS_SendEndCommand(byte conn, short t, string command)
{
	string s;
	s = "-:x";
	s += command;
	s[2] = t;
	BTS_SendCommand(conn,s);
}

void BTS_SendBeginCommand(byte conn, short t, string command)
{
	string s, in;
	bool conn_check = 1;
	int i, status, count = 0;
	s = "+:x";
	s += command;
	s[2] = t;
	for (i=0; i< 10; i++) {
		BTS_SendCommand(conn, s);
		BTS_Wait(conn);
		Wait(5);
	}
}

void BTS_End_AllDevices(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "*");
}

static long last_tick;
// about 100 ms is reasonable time.
void BTS_KeepAlive(byte connection, long time)
{
	long tick = CurrentTick();

	if ( (tick - last_tick ) >= time) {
		string s;
		s = "@:xK";
		s[2] = tr_id++;
		BTS_SendCommand(connection, s);
		last_tick = tick;
	}
}

short hexval(byte a)
{
	switch (a) {
	case '0':
		return 0;
		break;
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	case '3':
		return 3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	case '7':
		return 7;
		break;
	case '8':
		return 8;
		break;
	case '9':
		return 9;
		break;
	case 'A':
	case 'a':
		return 10;
		break;
	case 'B':
	case 'b':
		return 11;
		break;
	case 'C':
	case 'c':
		return 12;
		break;
	case 'D':
	case 'd':
		return 13;
		break;
	case 'E':
	case 'e':
		return 14;
		break;
	case 'F':
	case 'f':
		return 15;
		break;
	}
}

void unhex(string s, int len, string &out)
{
	short val, j, i;
	string msg;
	for (i=0,j=0; i < len; i++,j+=2) {
		out[i] = (hexval(s[j])*16)+hexval(s[j+1]);
	}
}

// hex encoded - un-hex it.
bool BTstring_sub (string in, byte sensorType, short lenToRead, string &s)
{
	short len, i, j;
	short ltor=lenToRead*2;
	string s_hex;
	string out = "        ";

	len = StrLen(in);
	if ( len < 9 ) return false;  // data is too short.

	for (i=0; i < 5; i++) {
		if ( in[3+i] != 0xFF ) {
			if (((in[3+i]+2+ltor) < len) && in[in[3+i]+1] == sensorType) {
				s_hex = SubStr(in, in[3+i]+2, ltor);
				unhex(s_hex, lenToRead, out);
				s = out;
				return true;
			}
			} else {
			return false;
		}
	}
	return false;

}

// no padding
bool BTstring_sub_no_padding (string in, byte sensorType, short lenToRead, string &s)
{
	short len, i, j;

	len = StrLen(in);
	if ( len < 9 ) return false;  // data is too short.

	for (i=0; i < 5; i++) {
		if ( in[3+i] != 0xFF ) {
			if (((in[3+i]+2+lenToRead) < len) && in[in[3+i]+1] == sensorType) {
				s = SubStr(in, in[3+i]+2, lenToRead);
				if ( in[in[3+i]] == ';' ) { // data was xor'ed
					for (j=0; j < lenToRead; j++) {
						s[j] = s[j] ^ 0xff;
					}
				}
				return true;
			}
			} else {
			return false;
		}
	}
	return false;

}

// padded data unpad it
bool BTstring_sub_padding (string in, byte sensorType, short lenToRead, string &s)
{
	short len, i, j;
	string buff;
	string msg;

	len = StrLen(in);
	if ( len < 9 ) return false;  // data is too short.

	for (i=0; i < 5; i++) {
		if ( in[3+i] != 0xFF ) {
			if (((in[3+i]+2+lenToRead) < len) && in[in[3+i]+1] == sensorType) {

				buff = SubStr(in, in[3+i]+2, lenToRead+2);
				int k,m;
				for (k=0, m=0; m<lenToRead; k++,m++) {
					if ( in[3+i]+2+k >= len ) {
						// data is truncated.
						return false;
					}
					buff[m] = in[in[3+i]+2+k];
					// if there was data which looked like LEGO telegram type,
					// app has padded a 0x55 after that, remove that padding.
					if ( in[in[3+i]+2+k] == 0x80 ||
						in[in[3+i]+2+k] == 0x81 ||
					in[in[3+i]+2+k] == 0x00 ||
					in[in[3+i]+2+k] == 0x01 ||
					in[in[3+i]+2+k] == 0x02
					) {
						k++;
					}
				}
				s = buff;
				return true;
			}
			} else {
			return false;
		}
	}
	return false;
}


void BTS_End_TimeDevice(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "T");
}

void BTS_Begin_TimeDevice(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "T2");
}

bool BTS_Read_TimeDevice(byte conn,
int &hour,
int &minutes,
int &seconds,
int &milliseconds)
{
	string in, in2;
	long timeStamp;
	short mailbox = 0;
	short len;
	string s;
	bool b;
	int status;

	read_from_bt('2');

	b = BTstring_sub(bt_buff_02, 'T', 4, s);
	if ( b == true ) {
		timeStamp = BTS_ByteArrayToLong(s);
		hour = timeStamp/(60*60*1000);
		timeStamp = timeStamp%(60*60*1000);
		minutes = timeStamp/60000;
		timeStamp = timeStamp%60000;
		seconds = timeStamp/1000;
		milliseconds = timeStamp%1000;
		return true;
		} else {
		return false;
	}
}

void BTS_End_DateDevice(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "D");
}

void BTS_Begin_DateDevice(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "D3");
}

bool BTS_Read_DateDevice(byte conn,
int &year,
int &month,
int &date)
{
	string in, in2;
	long value;
	long y;
	short mailbox = 0;
	short len;
	string s;
	bool b;

	read_from_bt('3');

	b = BTstring_sub(bt_buff_03, 'D', 4, s);
	if ( b == true ) {
		value = BTS_ByteArrayToLong(s);
		y = ((value&0xFFFF0000) / (256*256));
		year = y;
		month = ((value&0x0000FF00) / 256);
		date = (value&0x000000FF);
		return true;
		} else {
		return false;
	}
}

void BTS_End_Accelerometer(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "A");
}

void BTS_Begin_Accelerometer(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "A1");
}

bool BTS_Read_Accelerometer(byte conn, bts_3axes &currAccl)
{
	string in;
	string last_in;
	long value;
	long y;
	short mailbox = 0;
	short len;
	string s;
	bool b;
	int status;

	read_from_bt('1');

	b = BTstring_sub(bt_buff_01, 'A', 6, s);
	if ( b == true ) {
		currAccl.ax = BTS_MLBToInteger(s[0], s[1]);
		currAccl.ay = BTS_MLBToInteger(s[2], s[3]);
		currAccl.az = BTS_MLBToInteger(s[4], s[5]);
		currAccl.error = false;
		return true;
		} else {
		currAccl.error = true;
		return false;
	}
}


void BTS_End_Gyro(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "G");
}

void BTS_Begin_Gyro(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "G1");
}

bool BTS_Read_Gyro(byte conn, bts_3axes &currGyro)
{
	string in;
	string last_in;
	long value;
	long y;
	short mailbox = 0;
	short len;
	string s;
	bool b;
	int status;

	read_from_bt('1');

	b = BTstring_sub(bt_buff_01, 'G', 6, s);
	if ( b == true ) {
		currGyro.ax = BTS_MLBToInteger(s[0], s[1]);
		currGyro.ay = BTS_MLBToInteger(s[2], s[3]);
		currGyro.az = BTS_MLBToInteger(s[4], s[5]);
		currGyro.error = false;
		return true;
		} else {
		currGyro.error = true;
		return false;
	}
}

void BTS_End_Compass(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "C");
}

void BTS_Begin_Compass(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "C2");
}

bool BTS_Read_Compass(byte conn, bts_3axes &currCmps)
{
	string in;
	string last_in;
	long value;
	long y;
	short mailbox = 0;
	short len;
	string s;
	bool b;
	int status;

	read_from_bt('2');

	b = BTstring_sub(bt_buff_02, 'C', 6, s);
	if ( b == true ) {
		currCmps.ax = BTS_MLBToInteger(s[0], s[1]);
		currCmps.ay = BTS_MLBToInteger(s[2], s[3]);
		currCmps.az = BTS_MLBToInteger(s[4], s[5]);
		currCmps.error = false;
		return true;
		} else {
		currCmps.error = true;
		return false;
	}
}

void BTS_End_Version(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "V");
}

void BTS_Begin_Version(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "V3");
}

bool BTS_Read_Version(byte conn, string &version)
{
	string in;
	long value;
	long y;
	short mailbox = 0;
	short len;
	string s;
	bool b;

	read_from_bt('3');

	b = BTstring_sub(bt_buff_03, 'V', 4, s);
	if ( b == true ) {
		version = s;
		return true;
		} else {
		return false;
	}
}

void BTS_End_SoundLevel(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "S");
}

void BTS_Begin_SoundLevel(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "S2");
}

int BTS_Read_SoundLevel(byte conn)
{
	string s;
	bool b;

	read_from_bt('2');

	b = BTstring_sub(bt_buff_02, 'S', 2, s);
	if ( b == true ) {
		return BTS_MLBToInteger(s[0], s[1]);
		} else {
		return 255;
	}
}


void BTS_End_Proximity(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "P");
}

void BTS_Begin_Proximity(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "P2");
}

int BTS_Read_Proximity(byte conn)
{
	string in;
	string last_in;
	string s;
	bool b;

	read_from_bt('2');

	b = BTstring_sub(bt_buff_02, 'P', 2, s);
	if ( b == true ) {
		return BTS_MLBToInteger(s[0], s[1]);
		} else {
		return 255;
	}
}

void BTS_End_Light(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "L");
}

void BTS_Begin_Light(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "L2");
}

int BTS_Read_Light(byte conn)
{
	string in;
	string last_in;
	string s;
	bool b;

	read_from_bt('2');

	b = BTstring_sub(bt_buff_02, 'L', 2, s);
	if ( b == true ) {
		return BTS_MLBToInteger(s[0], s[1]);
		} else {
		return 255;
	}
}


void BTS_End_GPS(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "O");
}

void BTS_Begin_GPS(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "O3");
}

int BTS_Read_GPS(byte conn, bts_gps &gps)
{
	string in;
	string last_in;
	string s;
	bool b;

	// FIXME: outdoor testing required (at multiple locations).
	read_from_bt('3');

	b = BTstring_sub(bt_buff_03, 'O', 12, s);
	if ( b == true ) {
		gps.latt_d = BTS_MLBToInteger(s[0], s[1]);
		gps.latt_m = BTS_MLBToInteger(s[2], s[3]);
		gps.latt_s = BTS_MLBToInteger(s[4], s[5]);

		gps.long_d = BTS_MLBToInteger(s[6], s[7]);
		gps.long_m = BTS_MLBToInteger(s[8], s[9]);
		gps.long_s = BTS_MLBToInteger(s[10], s[11]);
		return true;
		} else {
		return false;
	}
}


void BTS_End_Location(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "W");
}

void BTS_Begin_Location(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "W3");
}

int BTS_Read_Location(byte conn, int &lattitude, int &longitude)
{
	string in;
	string last_in;
	string s;
	bool b;

	read_from_bt('3');
	// FIXME: outdoor testing required (at multiple locations).

	b = BTstring_sub(bt_buff_03, 'W', 4, s);
	if ( b == true ) {
		lattitude = BTS_MLBToInteger(s[0], s[1]);
		longitude = BTS_MLBToInteger(s[2], s[3]);

		} else {
		return 255;
	}
}

void BTS_End_MagneticField(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "M");
}

void BTS_Begin_MagneticField(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "M2");
}

int BTS_Read_MagneticField(byte conn, bts_3axes &currMag)
{
	string in;
	string last_in;
	string s;
	bool b;

	read_from_bt('2');

	b = BTstring_sub(bt_buff_02, 'M', 6, s);
	if ( b == true ) {
		currMag.ax = BTS_MLBToInteger(s[0], s[1]);
		currMag.ay = BTS_MLBToInteger(s[2], s[3]);
		currMag.az = BTS_MLBToInteger(s[4], s[5]);
		currMag.error = false;
		return true;
		} else {
		currMag.error = true;
		return false;
	}
}

void BTS_End_LinearAcceleration(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "N");
}

void BTS_Begin_LinearAcceleration(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "N1");
}

bool BTS_Read_LinearAcceleration(byte conn, bts_3axes &currAccl)
{
	string in;
	string last_in;
	long value;
	long y;
	short mailbox = 0;
	short len;
	string s;
	bool b;
	int status;

	read_from_bt('1');

	b = BTstring_sub(bt_buff_01, 'N', 6, s);
	if ( b == true ) {
		currAccl.ax = BTS_MLBToInteger(s[0], s[1]);
		currAccl.ay = BTS_MLBToInteger(s[2], s[3]);
		currAccl.az = BTS_MLBToInteger(s[4], s[5]);
		currAccl.error = false;
		return true;
		} else {
		currAccl.error = true;
		return false;
	}
}


void BTS_End_Pressure(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "E");
}

void BTS_Begin_Pressure(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "E3");
}

int BTS_Read_Pressure(byte conn)
{
	string in;
	string last_in;
	string s;
	bool b;

	read_from_bt('3');

	b = BTstring_sub(bt_buff_03, 'P', 2, s);
	if ( b == true ) {
		return BTS_MLBToInteger(s[0], s[1]);
		} else {
		return 255;
	}
}


void BTS_End_Humidity(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "H");
}

void BTS_Begin_Humidity(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "H3");
}

int BTS_Read_Humidity(byte conn)
{
	string in;
	string last_in;
	string s;
	bool b;

	read_from_bt('3');

	b = BTstring_sub(bt_buff_03, 'H', 2, s);
	if ( b == true ) {
		return BTS_MLBToInteger(s[0], s[1]);
		} else {
		return 255;
	}
}

void BTS_End_Gravity(byte connection)
{
	BTS_SendEndCommand(connection, tr_id++, "R");
}

void BTS_Begin_Gravity(byte connection)
{
	BTS_SendBeginCommand(connection, tr_id++, "R3");
}

bool BTS_Read_Gravity(byte conn, bts_3axes &currGravity)
{
	string in;
	string last_in;
	long value;
	long y;
	short mailbox = 0;
	short len;
	string s;
	bool b;
	int status;

	read_from_bt('3');

	b = BTstring_sub(bt_buff_03, 'R', 6, s);
	if ( b == true ) {
		currGravity.ax = BTS_MLBToInteger(s[0], s[1]);
		currGravity.ay = BTS_MLBToInteger(s[2], s[3]);
		currGravity.az = BTS_MLBToInteger(s[4], s[5]);
		currGravity.error = false;
		return true;
		} else {
		currGravity.error = true;
		return false;
	}
}


void BTS_SetMessageInterval (int connection, int message_interval)
{
	string msg;

	msg = "@:xBxx";
	msg[2] = tr_id++;
	msg[4] = (message_interval&0xFF);
	BTS_SendCommand(connection, msg);
	BTS_SendCommand(connection, msg);
	BTS_SendCommand(connection, msg);
}

void BTS_SetFloodCount (int connection, int flood_count)
{
	string msg;

	msg = "@:xFxx";
	msg[2] = tr_id++;
	msg[4] = ((flood_count&0xFF00) >> 8);
	msg[5] = (flood_count&0xFF);
	BTS_SendCommand(connection, msg);
	BTS_SendCommand(connection, msg);
	BTS_SendCommand(connection, msg);
}



#endif
