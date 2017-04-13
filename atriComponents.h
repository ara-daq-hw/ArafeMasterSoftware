#ifndef ATRICOMPONENTS_H
#define ATRICOMPONENTS_H


typedef enum atriExpansionPort {
  EX1 = 0,
  EX2 = 1,
  EX3 = 2,
  EX4 = 3
} AtriExpansionPort_t;

//functions to handle the ATRI ports

//! Function to open one of the ATRI expansion ports
	/*!
	\param auxFD
	\param port The desired port to be opened
	\return Integer for succes or failure of the function. Zero means success, negative number means failure.
	*/
int enableExpansionPort(int auxFd, AtriExpansionPort_t port);

//! Function to close one of the ATRI expansion ports
	/*!
	\param auxFD
	\param port The desired port to be closed
	\return Integer for succes or failure of the function. Zero means success, negative number means failure.
	*/
int disableExpansionPort(int auxFd, AtriExpansionPort_t port);

typedef enum atriComponentMask {
  EX1_mask = 0x01,
  EX2_mask = 0x02,
  EX3_mask = 0x04,
  EX4_mask = 0x08,
  gpsPowerMask = 0x20,
  fpgaOscillatorMask = 0x40
} AtriComponentMask_t;

extern const AtriComponentMask_t ext_i2c[4];
extern const char *ext_i2c_str[4];
#endif
