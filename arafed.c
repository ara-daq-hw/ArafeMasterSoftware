/*
 * Simple Arafe-through-I2C command interface.
 * 
 * Available commands:
 * power - change the power settings for the four ARAFE slaves; you must change all four at once
 * att - change the attenuator settings for any of the four ARAFE slaves
 * 
 * 
 * You can set the verbosity level setting the variable "int v" to either 0 (not verbose) or 1 (verbose)
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

int v =0; //variable to control verbosity of the output, because c doesn't natively support booleans...
//1 = verbose, 0 = not verbose (!0 = true,  0 = false)
//naughty thing to do with a global variable, but oh well...

//the new main for the ARAFE master
int main(int argc, char **argv){

	int auxFd;
	int retval;
   
	argc--; argv++; //decrease the number of parameters the program thinks it has
	if(!argc) {printf("nothing in argc, quitting now\n"); exit(1); }
       
	auxFd = openConnectionToFx2ControlSocket(); //open a connection to the 
	if(auxFd ==0) {
		fprintf(stderr,"Error opening connection to FX2_CONTROL_SOCKET\n");
		exit(1);
	}
	if(v) printf("using aux port %d\n",auxFd); //print confirmation if verbosity is active
   
	while(argc){ //okay, loop over the arguments
		/*
		if (strstr(*argv, "connect")) { //if the command is to initialize the port
			//declare that we want the ARAFE Master to be on EX0 (the first I2C port) when I try to connect to it
			retval = enableAtriComponents(auxFd, ext_i2c[0]); //enable this port
			if (retval<0) { //throw an error if that fails
				fprintf(stderr, "Something went wrong with assigning the ARAFE master to EX0\n");
				exit(1); //exit
			}
			else{ //print out the confirmation only if verbosity (v) is active
				 if(v) {printf("connection successful, retval is %d\n",retval);} //otherwise, the enable was successful, and we're good to go
			}
		}
		*/
		if (strstr(*argv, "power")) { //if the command is to change the power settings
			//first, enable the expansion port, and declare that we want the ARAFE Master to be on EX0 (the first I2C port)
			retval = enableExpansionPort(auxFd, 0); //enable the expansion port
			if (retval<0) { //throw an error if that fails
				fprintf(stderr, "Something went wrong with assigning the ARAFE master to EX0\n");
				exit(1); //exit
			}
			if(v) printf("connection successful, retval is %d\n",retval); //print out a confirmation if verbosity is active
		
		   
		        int arg1 = atoi(argv[4]);
		        int arg2 = atoi(argv[3]);
		        int arg3 = atoi(argv[2]);
		        int arg4 = atoi(argv[1]);
		   
		        if(v) {
			        printf("arg 1 %d\n", arg1);
		                printf("arg 2 %d\n", arg2);
		                printf("arg 3 %d\n", arg3);
		                printf("arg 4 %d\n", arg4);
			}
		   
			if((arg1 > 1) || (arg2 > 1) || (arg3 > 1) || (arg4> 1)){
				fprintf(stderr, "You can only turn a slave on or off with 0 or 1, nothing more\n"); //throw an error if the user tries to set them to something greater than 1
				exit(1);
			}
			//now to set up the transmission
			//they need to set all slaves at once, so [x x x x] = [slave3 slave2 slave1 slave0], with space in between
			char str[8]; //declare a blank eight bits
			sprintf(str,"1000%d%d%d%d",arg1,arg2,arg3,arg4); //form this up in binary 
			int int_ver = (int) strtol(str, NULL, 2); //convert it to a integer number
			unsigned char value = (unsigned char) int_ver; //convert it to an unsigned character (this is super naive and wrong, bug I want to have something here...)
			unsigned char reg = 0x00; //for power control, this is the register that's necessary
			retval = arafeWriteRegister(auxFd, reg, value);  //actually write to the register
			
			if( retval<0){ //if it fails
				printf("retval is %d, and setting the power failed\n", retval); //say something useful if it's wrong
				exit(1); //exit
			}
			if(v) printf("setting the power was successful, and retval is %d\n", retval); //print out a confirmation if verbosity is active
		}
		if (strstr(*argv, "att")) { //if the command is to change the attenuator settings
			//code needs to be added here to do attenuator control
		}

		argc--; argv++; //now, advance the argument
	} //finish looping over the arguments
   
	close(auxFd);
	return 0;
}
