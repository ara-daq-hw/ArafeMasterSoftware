/*
 * Very simple bootloader for ARAFE Master reprogramming
 * 
 * Usage
 *
 * type "./bsl file.name" where file.name is the name of the binary file you want to program
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
#include <sys/stat.h>
#include <fcntl.h>

#ifndef USB_I2C_DIOLAN
#include "fx2ComLib/fx2Com.h"
#else
#include "i2cComLib/i2cCom.h"
#endif

#include "arafe.h"
#include "arafebsl.h"

int v =0; //variable to control verbosity of the output, because c doesn't natively support booleans...
//1 = verbose, 0 = not verbose (!0 = true,  0 = false)
//naughty thing to do with a global variable, but oh well...

//the new main for the ARAFE master
int main(int argc, char **argv){

	int auxFd;
	int retval;
   
	argc--; argv++; //decrease the number of parameters the program thinks it has
	if(!argc) {printf("I need a file to load! Usage is ./bsl file.name \n"); exit(1); }
       
	auxFd = openConnectionToFx2ControlSocket(); //open a connection to the FX2 
	if(auxFd ==0) {
		fprintf(stderr,"Error opening connection to FX2_CONTROL_SOCKET\n");
		exit(1);
	}
	if(v) printf("using aux port %d\n",auxFd); //print confirmation if verbosity is active

	unsigned char value;
	unsigned char reg = 0x80; //this is the register we need to write to to use the BSL
	int file = open(argv[0], O_RDONLY); //open the file you passed me
	while (read(file, &value, 1) > 0){
		retval  = arafeWriteRegister(auxFd, reg, value); //write to the register
		if( retval<0){ //if it fails
			printf("retval is %d, and writing a byte during the BSL installation failed\n", retval); //say something useful if it's wrong
			exit(1); //exit
		}	
		exit(0); //get out successfully
	}
	close(auxFd); //close the port
	return 0; //return successfully
}