#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "atriComponents.h"

int enableExpansionPort(int auxFd, AtriExpansionPort_t port) {
  return 0;
}
int disableExpansionPort(int auxFd, AtriExpansionPort_t port) {
  return 0;
}

int openConnectionToFx2ControlSocket() {
  return 1;
}

int arafeWriteRegister(int auxFd, unsigned char reg, unsigned char value) {
  printf("register %d write 0x%2.2x\n", reg, value);
  return 0;
}

int arafeReadRegister(int auxFd, unsigned char reg, unsigned char *value) {
  printf("register %d read\n");
  *value = 0xFF;
  return 0;
}
