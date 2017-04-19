/*
 * Simple Arafe-through-I2C command interface.
 * 
 * Available commands (alphabetical list):
 *
 * defaultpwr - change the power default settings for the four ARAFE slaves
 *              you must change all four at once
 *              syntax is: ./arafed power x x x x where x = 0 for off, 1 for on
 *              example: "./arafed power 1 1 1 1" changes the default power on behavior so that all slaves have power at startup 
 *
 * power -      change the power settings for the four ARAFE slaves
 *              you must change all four at once
 *              syntax is: ./arafed power x x x x where x = 0 for off, 1 for on
 *              example: "./arafed power 1 1 1 1" turns all slaves on
 *
 * slave -      send a command to one of the four ARAFE slaves
 *              you must specify a slave, a command, and an argument
 *              slave must be an integer number: 0 -> 3
 *              command must be a hex command from the ARAFE command table, ie 0x00 -> 0XFF (https://github.com/ara-daq-hw/arafe_slave_v2/blob/master/Documentation/ARAFE_slave_protocol.pdf)
 *              argument must be a valid hex argument from the ARAFE argument list, ie 0x00 -> 0xFF (https://github.com/ara-daq-hw/arafe_slave_v2/blob/master/Documentation/ARAFE_slave_protocol.pdf)
 * 
 * 
 * need to write a "read monitoring" function
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
		   
			if((arg1 > 1) || (arg2 > 1) || (arg3 > 1) || (arg4> 1) || (arg1 < 0) || (arg2 < 0) || (arg3 < 0) || (arg4 < 0)){ //throw an error if the user tries to set them to something greater than 1
				fprintf(stderr, "You can only turn a slave on or off with 0 or 1, nothing more\n"); 
				exit(1);
			}
			//now to set up the transmission
			//they need to set all slaves at once, so [x x x x] = [slave3 slave2 slave1 slave0], with space in between
			char str[8]; //declare a blank eight bits
			sprintf(str,"1000%d%d%d%d",arg1,arg2,arg3,arg4); //form this up in binary 
			int int_ver = (int) strtol(str, NULL, 2); //convert it to a integer number
			unsigned char value = (unsigned char) int_ver; //convert it to an unsigned character
			unsigned char reg = 0x00; //for power control, this is the register that's necessary
			retval = arafeWriteRegister(auxFd, reg, value);  //actually write to the register
			
			if( retval<0){ //if it fails
				printf("retval is %d, and setting the power failed\n", retval); //say something useful if it's wrong
				exit(1); //exit
			}
			if(v) printf("setting the power was successful, and retval is %d\n", retval); //print out a confirmation if verbosity is active
			exit(0); //successfully exit
		}
		
		if (strstr(*argv, "defaultpwr")) { //if the command is to change the default power settings
			//this command protocol will be almost identical to the power protocol, except we want to set a different register (0x01 for default instead of 0x00 for control)
			
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
		   
			if((arg1 > 1) || (arg2 > 1) || (arg3 > 1) || (arg4> 1) || (arg1 < 0) || (arg2 < 0) || (arg3 < 0) || (arg4 < 0)){ //throw an error if the user tries to set them to something greater than 1
				fprintf(stderr, "You can only turn a slave on or off with 0 or 1, nothing more\n"); 
				exit(1);
			}
			//now to set up the transmission
			//they need to set all slaves at once, so [x x x x] = [slave3 slave2 slave1 slave0], with space in between
			char str[8]; //declare a blank eight bits
			sprintf(str,"1000%d%d%d%d",arg1,arg2,arg3,arg4); //form this up in binary 
			int int_ver = (int) strtol(str, NULL, 2); //convert it to a integer number
			unsigned char value = (unsigned char) int_ver; //convert it to an unsigned character
			unsigned char reg = 0x01; //for power control default, this is the register that's necessary
			retval = arafeWriteRegister(auxFd, reg, value);  //actually write to the register
			
			if( retval<0){ //if it fails
				printf("retval is %d, and setting the power failed\n", retval); //say something useful if it's wrong
				exit(1); //exit
			}
			if(v) printf("setting the power was successful, and retval is %d\n", retval); //print out a confirmation if verbosity is active
			exit(0); //successfully exit
		}
		
		if (strstr(*argv, "slave")) { //if the command to send a general command to a slave
			//usage will be: slave slave# command arg
			//code needs to be added here to do attenuator control
			
			//first, enable the expansion port, and declare that we want the ARAFE Master to be on EX0 (the first I2C port)
			retval = enableExpansionPort(auxFd, 0); //enable the expansion port
			if (retval<0) { //throw an error if that fails
				fprintf(stderr, "Something went wrong with assigning the ARAFE master to EX0\n");
				exit(1); //exit
			}
			if(v) printf("connection successful, retval is %d\n",retval); //print out a confirmation if verbosity is active
			
			//check they want to do something to a slave that exists
			//we do this check so that the transmission registers aren't pre-loaded with something before we know if the transmission will be valid
			int slave = atoi(argv[1]);
			 //check to see if they're doing something dumb about which slave to alter
			if( slave < 0 || slave !=0 || slave !=1 || slave !=2 || slave !=3){
				printf(stderr, "You can only turn on one of the four slaves (indexed 0 -> 3)\n") //tell them
				exit(1); //get out
			}
			
			//it would be difficult to write argument checking for the argument and command, but should probably try at some point in the future
			
			//need to set the command register, register 5
			char str_command[155]; //declare an empty bank for the bytes, make it unnecessarily large
			str_command = argv[2]; //the string should be set equal to the second thing passed to the function, which is the command for the slave
			int int_ver_command = (int) strtol(str_command, NULL, 16); //convert this to an int
			unsigned char command = (unsigned char) int_ver_command; //convert this to an unsigned character
			unsigned char command_reg = 0x05; //the command register for the ARAFE master is register 5
			retval = arafeWriteRegister(auxFd, command_reg, command); //actually write to the register
			if( retval<0){ //if it fails
				printf("retval is %d, and setting the command register failed\n", retval); //say something useful if it's wrong
				exit(1); //exit
			}
			if(v) printf("setting the command register was successful, and retval is %d\n", retval); //print out a confirmation if verbosity is active
			
			//need to set the argument register, register 6
			char str_argument[155]; //declare an empty bank for the bytes, make it unnecessarily large
			str_argument = argv[3]; //the string should be set equal to the third thing passed to the function, which is the argument for the slave
			int int_ver_argument = (int) strtol(str_argument, NULL, 16); //convert this to an int
			unsigned char argument = (unsigned char) int_ver_argument; //convert this to an unsigned character
			unsigned char argument_reg = 0x06; //the argument register for the ARAFE master is register 6
			retval = arafeWriteRegister(auxFd, argument_reg, argument); //actually write to the register	
			if( retval<0){ //if it fails
				printf("retval is %d, and setting the argument register failed\n", retval); //say something useful if it's wrong
				exit(1); //exit
			}
			if(v) printf("setting the argument register was successful, and retval is %d\n", retval); //print out a confirmation if verbosity is active

			//need to set the slavectrl register, register 4
			//we set the slavectrl register last because setting the high bit of this register will actually trigger the dispatch to the slave
			char str_ctrl[8]; //declare a blank eight bits
			int settings = {0,0,0,0}; //declare an array for the four slaves they can pick from, and intialize with using "none", so all zeros
			settings[slave] = 1; //for whatever slave they want turned on, turn that one on
			sprintf(str,"1000%d%d%d%d",settings[0],settings[1],settings[2],settings[3]); //form this up in binary 
			int int_ver = (int) strtol(str, NULL, 2); //convert it to a integer number
			unsigned char value = (unsigned char) int_ver; //convert it to an unsigned character
			unsigned char reg = 0x04; //for power control, this is the register that's necessary
			retval = arafeWriteRegister(auxFd, command_reg, command); //actually write to the register
			if( retval<0){ //if it fails
				printf("retval is %d, and setting the control register failed\n", retval); //say something useful if it's wrong
				exit(1); //exit
			}
			if(v) printf("setting the control register was successful, and retval is %d\n", retval); //print out a confirmation if verbosity is active
			
			exit(0);
			
		}
		
		if (strstr(*argv, "help")) { //they need help!
			printf("Available commands are :\n");
			printf(" 'power x x x x' Sets the current operations for the slaves. Use is 'power 1 1 1 1' would turn all slaves on, etc");
		}		

		argc--; argv++; //now, advance the argument
	} //finish looping over the arguments
   
	close(auxFd);
	return 0;
}
