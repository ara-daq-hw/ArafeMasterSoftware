#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef USB_I2C_DIOLAN
#include "i2cComLib/i2cCom.h"
#else
#include "fx2ComLib/fx2Com.h"
#endif

#include "arafe.h"
#include "arafei2c.h"

const unsigned char ic2_addr = 0X0A; //some dummy I2C address for the ARAFE master, which will be static

// WRITE TO A REGISTER
int arafeWriteRegister(int auxFd, unsigned char reg, unsigned char value) {
	unsigned char data[2];
	data[0] = reg; //what register do you want to write to
	data[1] = value; //what value do you want to write there (need 8 bits cast into hex)
	int len = 2; //the length of the thing we want to pass is 2
	retval = writeToI2C(auxFd, ic2_addr, len, data); //write these things to I2C
	return retval; //return this value
}

// READ FROM A REGISTER
int arafeReadRegister(int axuFx, unsigned char reg, unsigned char value){
	unsigned char data[1];
	data[0] = reg;
	int len = 1;
	retval = writeToI2C(auxFd, ic2_addr, len, data); //write to I2C //this function is defined in arafei2c
	retval = readFromI2C(auxFd, ic2_addr | 0x1, 1, data); //now read it
	//data[0] now has the return data
	cout<<"Register returned "<<data[0]<<endl; //so we print it out
}

//INITIALIZE AN I2C INTERFACE
int initializeArafe(int auxFd){
	return initializeArafeI2C(auxFd); //just return the value of this function call //this function is defined in arafei2c
}
