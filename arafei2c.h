#ifndef ICECALI2C_H
#define ICECALI2C_H

//! Function to intialize the ARAFE master
	/*!
	\param auxFD
	\return Always returns 0. Information in printf's while function executes
	*/
int initializeArafeI2C(int auxFd);

#ifndef ARAFEI2C_C
extern unsigned char i2c_addr; //the I2C address of the ARAFE
#endif

#endif
