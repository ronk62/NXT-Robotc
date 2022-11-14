/* PixyCam V1 with Lego f/w
 * Ron King
 * 6/16/2019
 */

#ifndef __PIXYC_H__
#define __PIXYC_H__

// THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define PIXYC_I2C_ADDR       0x02  /*!< PIXYC I2C device address */

#define PIXYC_GEN_QRY        0x50  /*!< PIXYC General Query (6 bytes) */

#define PIXYC_SIG_QRY1       0x51  /*!< PIXYC Signature Query 1 (5 bytes) */
#define PIXYC_SIG_QRY2       0x52  /*!< PIXYC Signature Query 2 (5 bytes) */
#define PIXYC_SIG_QRY3       0x53  /*!< PIXYC Signature Query 3 (5 bytes) */
#define PIXYC_SIG_QRY4       0x54  /*!< PIXYC Signature Query 4 (5 bytes) */
#define PIXYC_SIG_QRY5       0x55  /*!< PIXYC Signature Query 5 (5 bytes) */
#define PIXYC_SIG_QRY6       0x56  /*!< PIXYC Signature Query 6 (5 bytes) */
#define PIXYC_SIG_QRY7       0x57  /*!< PIXYC Signature Query 7 (5 bytes) */

#define PIXYC_CC_QRY         0x58  /*!< PIXYC Color Code Query (6 bytes) */

#define PIXYC_ANG_QRY        0x60  /*!< PIXYC Angle Query (1 byte) */ 


bool PIXYCreadGeneralQry(tSensors link);

bool PIXYCreadSignatureQry(tSensors link, ubyte qryAddress);

bool PIXYCreadColorCodeQry(tSensors link, ubyte ColorCodeByte1, ubyte ColorCodeByte2);

bool PIXYCreadAngleQry(tSensors link);

bool PIXYCsendCmd(tSensors link, ubyte command);


tByteArray PIXYC_I2CRequest;       /*!< Array to hold I2C command data */
tByteArray PIXYC_I2CReply;         /*!< Array to hold I2C reply data */

short sigVal=0
ubyte xPos=0,yPos=0,width=0,height=0,angle=0,qryAddress=0x51,numberOfBlocks=0,ColorCodeByte1=0,ColorCodeByte2=0;


/**
 * Initialize I2C sensor. 'bFastI2C' is a flag to indicate whether standard speed I2C
 * or fast speed transmission should be used with the sensor.
 */
void initializeI2CSensor(const tSensors nPortIndex, const bool bFastI2C)
{
	SensorType[nPortIndex] = bFastI2C ? sensorI2CCustomFast : sensorI2CCustom;
}


/**
 * Return 6 bytes that describe the largest detected block within all signatures,
 * including color coded  blocks.  If no object is detected, all bytes will be  0.
 */
bool PIXYCreadGeneralQry(tSensors link) {
  memset(PIXYC_I2CRequest, 0, sizeof(tByteArray));

  PIXYC_I2CRequest[0] = 2;                // Number of bytes in I2C command
  PIXYC_I2CRequest[1] = PIXYC_I2C_ADDR;   // PIXYC I2C device address
  PIXYC_I2CRequest[2] = PIXYC_GEN_QRY;    // Set write address to sensor mode register

  if (!writeI2C(link, PIXYC_I2CRequest, PIXYC_I2CReply, 6))
    return false;

  // 16­bit value that describes the signature of the largest block. A value of 1 thru 7  corresponds to signatures 1 thru 7.
  // A value greater than 7 is a color code  encoded in octal (base­8).
  sigVal = (PIXYC_I2CReply[1] << 8) + PIXYC_I2CReply[0];

  // X value of center of largest detected block, ranging between 0 and 255.  An x value of 255 is the far right­side of Pixy’s image.
  xPos = PIXYC_I2CReply[2];

  // Y value of center of largest detected block, ranging between 0 and 199.  A value of 199 is the far bottom­side of Pixy’s image.
  yPos = PIXYC_I2CReply[3];

  // Width of largest block, ranging between 1 and 255.  A width of 255 is the full width of Pixy’s image. 
  width = PIXYC_I2CReply[4];

  // Height of largest block, ranging between 1 and 200.  A height of 200 is the full height of Pixy’s image. 
  height = PIXYC_I2CReply[5];
  return true;
}



/**
 * Return 5 bytes that describe the largest detected block  within requested signature.
 * If no object is detected, all bytes will be  0.
 */
bool PIXYCreadSignatureQry(tSensors link, ubyte qryAddress) {
  memset(PIXYC_I2CRequest, 0, sizeof(tByteArray));

  PIXYC_I2CRequest[0] = 2;                // Number of bytes in I2C command
  PIXYC_I2CRequest[1] = PIXYC_I2C_ADDR;   // PIXYC I2C device address
  PIXYC_I2CRequest[2] = qryAddress;       // Set write address to sensor mode register

  if (!writeI2C(link, PIXYC_I2CRequest, PIXYC_I2CReply, 5))
    return false;

  // Number of blocks that match the specified signature.
  numberOfBlocks = PIXYC_I2CReply[0];

  // X value of center of largest detected block, ranging between 0 and 255.  An x value of 255 is the far right­side of Pixy's image.
  xPos = PIXYC_I2CReply[1];

  // Y value of center of largest detected block, ranging between 0 and 199.  A value of 199 is the far bottom­side of Pixy's image.
  yPos = PIXYC_I2CReply[2];

  // Width of largest block, ranging between 1 and 255.  A width of 255 is the full width of Pixy's image. 
  width = PIXYC_I2CReply[3];

  // Height of largest block, ranging between 1 and 200.  A height of 200 is the full height of Pixy's image. 
  height = PIXYC_I2CReply[4];
  return true;
}


/**
 * Return 6 bytes that describe the largest block that matches the specified color code
 * If no detected  object matches the specified color code, all  bytes will be 0.
 */
bool PIXYCreadColorCodeQry(tSensors link, ubyte ColorCodeByte1, ubyte ColorCodeByte2) {
  memset(PIXYC_I2CRequest, 0, sizeof(tByteArray));

  PIXYC_I2CRequest[0] = 4;                // Number of bytes in I2C command
  PIXYC_I2CRequest[1] = PIXYC_I2C_ADDR;   // PIXYC I2C device address
  PIXYC_I2CRequest[2] = PIXYC_CC_QRY;     // Set write address to sensor mode register
  PIXYC_I2CRequest[3] = ColorCodeByte1;   // The LSB of color code (octal).
	PIXYC_I2CRequest[4] = ColorCodeByte2;   // The MSB of color code (octal).

  if (!writeI2C(link, PIXYC_I2CRequest, PIXYC_I2CReply, 6))
    return false;

  // Number of blocks that match the specified signature.
  numberOfBlocks = PIXYC_I2CReply[0];

  // X value of center of largest detected block, ranging between 0 and 255.  An x value of 255 is the far right­side of Pixy's image.
  xPos = PIXYC_I2CReply[1];

  // Y value of center of largest detected block, ranging between 0 and 199.  A value of 199 is the far bottom­side of Pixy's image.
  yPos = PIXYC_I2CReply[2];

  // Width of largest block, ranging between 1 and 255.  A width of 255 is the full width of Pixy's image. 
  width = PIXYC_I2CReply[3];

  // Height of largest block, ranging between 1 and 200.  A height of 200 is the full height of Pixy's image. 
  height = PIXYC_I2CReply[4];

  // Angle of largest color code block, ranging between 0 and 255.  A value of 255  corresponds to an angle of 360 degrees.
  angle = PIXYC_I2CReply[5];
  return true;
}


// Return 1 byte which corresponds to the angle of largest color coded block returned by a previous General Query.
ubyte PIXYCreadAngleQry(tSensors link) {
  memset(PIXYC_I2CRequest, 0, sizeof(tByteArray));

  PIXYC_I2CRequest[0] = 2;                // Number of bytes in I2C command
  PIXYC_I2CRequest[1] = PIXYC_I2C_ADDR;   // PIXYC I2C device address
  PIXYC_I2CRequest[2] = PIXYC_ANG_QRY;    // Set write address to sensor mode register

  if (!writeI2C(link, PIXYC_I2CRequest, PIXYC_I2CReply, 1))
    return false;

  // Angle of largest color code block, ranging between 0 and 255.  A value of 255  corresponds to an angle of 360 degrees.
  angle = PIXYC_I2CReply[0];
  return true;
}


/**
 * Send a command to the sensor
 * @param link the sensor port number
 * @param command the command to be sent
 * @return true if no error occured, false if it did
 */
bool PIXYCsendCmd(tSensors link, ubyte command) {
  memset(PIXYC_I2CRequest, 0, sizeof(tByteArray));

  PIXYC_I2CRequest[0] = 2;               // Number of bytes in I2C command
  PIXYC_I2CRequest[1] = PIXYC_I2C_ADDR;   // I2C address of accel sensor
  PIXYC_I2CRequest[2] = command;         // Set write address to sensor mode register

  return writeI2C(link, PIXYC_I2CRequest);
}

#endif //__PIXYC_H__


// ****************************************************************************************

/**
 * The Above is the driver code.
 *
 * The Below is test code to validate the driver.
 */

 // ****************************************************************************************

