/*
 * Simple Arafe-through-I2C command interface.
 * 
 * Available commands:
 * power - change the power settings for the four ARAFE slaves; you must change all four at once
 * init - intializes the connection to the ARAFE master through the I2C port
 * att - change the attenuator settings for any of the four ARAFE slaves
 * 
 * Modeleded after the IceCal stuff by PSA
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#ifndef USB_I2C_DIOLAN
#include "fx2ComLib/fx2Com.h"
#else
#include "i2cComLib/i2cCom.h"
#endif

#include "arafe.h"
#include "arafed.h"
#include "arafei2c.h"

//the new main for the ARAFE master
int main(int argc, char **argv){

	int auxFd;
	int retval;
	
	argc--; argv++;
	if(!argc) {exit(1); }
	
	
	while(argc){ //okay, loop over the arguments
		if (strstr(*argv, "init")) { //if the command is to initialize the port
			auxFd = openConnectionToFx2ControlSocket(); //open a connection to the FX2 control socket
			if(auxFx ==0){ //check to see if the FX2 connection actually got made
				fprintf(stderr, "Error opening connection to FX2_CONTROL_SOCKET\n");
				exit(1);
			}
			//here, I'm going to declare that we want the ARAFE Master to be on EX3, the fourth I2C port when I try to connect to it
			if ((retval = enableAtriComponents(auxFd, ext_i2c[3]))<0) { //try and enable this port, and throw an error if that fails
				fprintf(stderr, "this is an error message\n");
				exit(1);
			}
		}
		if (strstr(*argv, "power")) { //if the command is to change the power settings
			//code needs to be added here to do power control
		}
		if (strstr(*argv, "att")) { //if the command is to change the attenuator settings
			//code needs to be added here to do attenuator control
		}
	
	} //finish looping over the arguments

	close(auxFd);
	return 0;
}
