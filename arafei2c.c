#include <stdio.h>
#include <stdlib.h>
#define ARAFEI2C_C
#include "arafe.h"
#include "arafei2c.h"
#include "fx2ComLib/fx2Com.h"

const unsigned char i2c_addr = (0x1E) << 1; //the I2C address of the ARAFE Master, defined in the firmware (https://github.com/ara-daq-hw/arafe-master), bitshifted down to be a 7 bit number
const unsigned char i2c_bsl_addr = (0x40); //the I2C address of the BSL


//this function is a direct copy from the icecal version, and I don't understand what it's doing
/*
int icecalAssign(int auxFd, unsigned int cpuId, unsigned char port) {
	unsigned char data[5];
	int retval; //the return value
  
	//if (!auxFd || ((port != ICECAL_PORTA) && (port != ICECAL_PORTB))) return -1; //something left over

	//the basic strategy here is to load information into the 
	data[0] = cpuId & 0xFF;
	data[1] = (cpuId >> 8) & 0xFF;
	data[2] = (cpuId >> 16) & 0xFF;
	data[3] = (cpuId >> 24) & 0xFF;
	data[4] = (port == ICECAL_PORTA) ? 0x0A : 0x0C;
	retval = writeToI2C(auxFd, icbc_addr, 5, data);  
	if (retval < 0) {
		fprintf(stderr, "arafeAssign: error %d writing to broadcast address\n", retval);
		return retval;
	}
	printf("icecalAssign: assigned %8.8x to address %2.2x\n", cpuId, data[4]);
	return 0;
}

*/

//this is the actual assign code for the ARAFE
int arafeAssign(int auxFd, unsigned int cpuId, unsigned char port){
        unsigned char data[5]; //data to send
        int retval; //a return value
        //load the information
        data[0] = cpuId & 0XFF;
        data[1] = (cpuId >> 8) & 0xFF;                                                                                                                                                                                                       
        data[2] = (cpuId >> 16) & 0xFF;                                                                                                                                                                                                      
        data[3] = (cpuId >> 24) & 0xFF;                                                                                                                                                                                                      
        data[4] = 0x30; //this should assing the I2C port of the device      
        return 0;
}


//intialize the ARAFE Master
int initializeArafeI2C(int auxFd) {
 	int retval;
	printf("ARAFE Master assigned I2C address 0x30. FYI this is hard coded\n");
	//retval = arafeAssign(auxFd, icecal_A, ICECAL_PORTA);
	return 0;
}
