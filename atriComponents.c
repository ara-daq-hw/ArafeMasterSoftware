#include <stdio.h>
#include <stdlib.h>
#include "atriComponents.h"
#include "fx2ComLib/fx2Com.h"

#define LIBUSB_ERROR_TIMEOUT -9

const AtriComponentMask_t ext_i2c[4] = {EX1_mask, EX2_mask, EX3_mask, EX4_mask };

const char *ext_i2c_str[4] = {
  "EX1",
  "EX2",
  "EX3",
  "EX4"
};

int enableExpansionPort(int auxFd, AtriExpansionPort_t port) {
	int retval; //a return value
	// Enable each one in turn.
	if ((retval = enableAtriComponents(auxFd, ext_i2c[port])) < 0) {
		fprintf(stderr, "%s: error %d enabling expansion port %s\n", __FUNCTION__,
			retval,
			ext_i2c_str[port]);
		return retval;
	}
	disableExpansionPort(auxFd, port);
	exit(1);
	return 0;
}

int disableExpansionPort(int auxFd, AtriExpansionPort_t port) {
	int retval;
	// Enable each one in turn.
	if ((retval = disableAtriComponents(auxFd, ext_i2c[port])) < 0) {
		fprintf(stderr, "%s: error %d probing expansion ports\n", __FUNCTION__,
			retval);
		return retval;
	}
	return 0;
}

