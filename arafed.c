/*
 * Simple Arafe-through-I2C command interface.
 * 
 * Available commands:
 * power - change the power settings for the four ARAFE slaves; you must change all four at once
 * connect - intializes the connection to the ARAFE master through the I2C port
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
   
	argc--; argv++; //decrease the number of parameters the program thinks it has
        if(!argc) {printf("nothing in argc, quitting now\n"); exit(1); }
       
        auxFd = openConnectionToFx2ControlSocket();
        if(auxFd ==0) {
	        fprintf(stderr,"Error opening connection to FX2_CONTROL_SOCKET\n");
	        exit(1);
        }
        printf("using aux port %d\n",auxFd);
   
	while(argc){ //okay, loop over the arguments
		if (strstr(*argv, "connect")) { //if the command is to initialize the port
			//here, I'm going to declare that we want the ARAFE Master to be on EX0, the first I2C port when I try to connect to it
		        retval = enableAtriComponents(auxFd, ext_i2c[0]);
		        if (retval<0) { //try and enable this port, and throw an error if that fails
				fprintf(stderr, "Something went wrong with assigning the ARAFE master to EX3\n");
				exit(1);
			}
		        else printf("connection successful, retval is %d\n",retval);
		}
		if (strstr(*argv, "power")) { //if the command is to change the power settings
		        unsigned char reg = 0x00;
		        unsigned char value = 0X84;
		        //enableAtriComponents(auxFd, ext_i2c[0]);
		        enableExpansionPort(auxFd, 0);
		        //enableAtriComponents(auxFd, ext_i2c[0]);
		        retval = arafeWriteRegister(auxFd, reg, value);
		        //retval = arafeReadRegister(auxFd, 0x00);
		        if( retval<0){ //write to register 0 (POWERCTL)  with 8F (High bit set and all slaves on)
		                printf("retval is %d, and setting the power failed\n", retval);
			}
		        else printf("setting the power was successful, and retval is %d\n", retval);
		}
		if (strstr(*argv, "att")) { //if the command is to change the attenuator settings
			//code needs to be added here to do attenuator control
		}

	        argc--;
	        argv++;
	} //finish looping over the arguments

   
	close(auxFd);
	return 0;
}
