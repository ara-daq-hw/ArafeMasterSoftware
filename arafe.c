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



// WRITE TO A REGISTER
int arafeWriteRegister(int auxFd, unsigned char reg, unsigned char value) {
	unsigned char data[2];
	data[0] = reg; //what register do you want to write to
	data[1] = value; //what value do you want to write there (need 8 bits cast into hex)
	printf("register is %2.2x\n",data[0]);
        printf("value is %2.2x\n",data[1]);
        int len = 2; //the length of the thing we want to pass is 2
	int retval = writeToI2C(auxFd, i2c_addr, len, data); //write these things to I2C //the ic2_addr is defined in arafei2c.c, and is the I2C address of the ARAFE Master
	return retval; //return this value
}

// READ FROM A REGISTER
int arafeReadRegister(int auxFd, unsigned char reg, unsigned char value){
	unsigned char data[1];
	data[0] = reg;
	int len = 1;
	int retval = writeToI2C(auxFd, i2c_addr, len, data); //write to I2C
	retval = readFromI2C(auxFd, i2c_addr | 0x1, len, data); //now read it
	//data[0] now has the return data
        printf("Register returned %d \n", data[0]);//so we print it out
        return retval; //return this value
}

//INITIALIZE AN I2C INTERFACE
//int initializeArafe(int auxFd){
//	return initializeArafeI2C(auxFd); //just return the value of this function call //this function is defined in arafei2c
//}
