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
	
	
	
	while(argc){ //okay, loop over the arguments
		if (strstr(*argv, "program")) { //if the command is to change the power settings

			retval = enableExpansionPort(auxFd,0);
			if (retval<0) { //throw an error if that fails
				fprintf(stderr, "Something went wrong with assigning the ARAFE master to EX0\n");
				exit(1); //exit
			}
			if(v) printf("connection successful, retval is %d\n",retval); //print out a confirmation if verbosity is active
		
			unsigned char value;
			unsigned char reg = 0x40; //this is the register we need to write to to use the BSL
			//open the file
			int file = open(argv[0], O_RDONLY); //open the file you passed me
			if(!file){
				printf("Something went wrong with opening the file!\n");
				exit(1);
			}
	
			//erase what's currently there
			retval = arafeWriteRegister(auxFd, reg, 0x55);
			if( retval<0){ //if it fails
				printf("retval is %d, and something went wrong with erasing the memory\n", retval); //say something useful if it's wrong
				exit(1); //exit
			}
			if(v) printf("erasing was successful, and retval is %d\n", retval); //print out a confirmation if verbosity is active
	
			delay(100); //hold off for 100 ms to let the erase proceed
	
			while (read(file, &value, 1) > 0){
				retval  = arafeWriteRegister(auxFd, reg, value); //write to the register
				if( retval<0){ //if it fails
					printf("retval is %d, and writing a byte during the BSL installation failed\n", retval); //say something useful if it's wrong
					exit(1); //exit
				}
				delay(10); //hold off for 10 ms between each byte; this is conservative, but whatever
			}
			
			disableExpansionPort(auxFd, 0); //disable the expansion port
			exit(0); //get out
		}
		
		if (strstr(*argv, "erase")) { //if the command is to just do the erase

			retval = enableExpansionPort(auxFd,0);
			if (retval<0) { //throw an error if that fails
				fprintf(stderr, "Something went wrong with assigning the ARAFE master to EX0\n");
				exit(1); //exit
			}
			if(v) printf("connection successful, retval is %d\n",retval); //print out a confirmation if verbosity is active
		
			unsigned char reg = 0x40; //this is the register we need to write to to use the BSL
	
			//erase what's currently there
			retval = arafeWriteRegister(auxFd, reg, 0x55);
			if( retval<0){ //if it fails
				printf("retval is %d, and something went wrong with erasing the memory\n", retval); //say something useful if it's wrong
				exit(1); //exit
			}
			if(v) printf("erasing was successful, and retval is %d\n", retval); //print out a confirmation if verbosity is active
	
			delay(100); //hold off for 100 ms to let the erase proceed
			exit(1); 
		}
		
		close(auxFd); //close the port
		exit(0); //close the program

}