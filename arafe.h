#ifndef ICECAL_H
#define ICECAL_H

#include "atriComponents.h"

//! Function to write to an ARAFE register
	/*!
	\param auxFD
	\param reg The register in the ARAFE memory you want to write to
	\param value The value you want to write to that register
	\return Integer for succes or failure of the function. Zero means success, negative number means failure.
	*/
int arafeWriteRegister(int auxFd, unsigned char reg, unsigned char value);


//! Function to read to an ARAFE register
	/*!
	\param auxFD
	\param reg The register in the ARAFE memory you want to write to
	\param value The byte in that register that you want back after the read completes
	\return Integer for succes or failure of the function. Zero means success, negative number means failure.
	*/
int arafeReadRegister(int auxFd, unsigned char reg, unsigned char * value);

//! Function to initialize an ARAFE Master
	/*!
	\param auxFD
	\return Integer for succes or failure of the function. Zero means success, negative number means failure.
	*/
int initializeArafe(int auxFd);

#endif
