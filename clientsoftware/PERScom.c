/* encoding: UTF-8, break: linux, indet-mode: 4 spaces, lang: C90/eng */
/**
 * \file PERScom.c
 * Command line client for PERswitch. Used to communicate with the periphery
 * switch over usb. Can be used to turn certain pins on a given port on/off or
 * query their status.
 *
 * Most of this is based on <a href="www.obdev.at">Objective Development</a>'s
 * VUSB software and examples.
 * \date 31. Oct. 2010
 * \author <a href="mailto:con-f-use@gmx.net">con-f-use</a>
 * \copyright
 * This file is part of PERswitch. Copyright © 2012 con-f-use. Use permitted
 * under GNU General Public License.
 */

/* ------------------------------------------------------------------------- */
/* ----------------------- Definitions and Includes ------------------------ */
/* ------------------------------------------------------------------------- */

#include "cmdline.h"

/* ------------------------------------------------------------------------- */
/* ------------------------------ Main Section ----------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * Main function of the command line client.
 */
int main(int argc, char **argv) {
    // DECLARATIONS AND INITIALISATIONS
    usb_dev_handle *handle = NULL;
    int* status = (int*)malloc(sizeof(status));
    *status = 0; //get rid of uninitialized warnung
    char* command;
    unsigned int portNbr, pinNbr;

    usb_init();

    // ARGUMENT HANDLING
    if(argc != 4){
        usage(argv[0]);
        exit(1);
    }
    portNbr = atoi(argv[1]);
    pinNbr = atoi(argv[2]);
    command = argv[3];

    // CREATE HANDLE FOR USB DEVICE
    if(createHandle(&handle)) exit(1);

    // DO ACTUAL REQUESTS
    if( doRequest(handle,portNbr,pinNbr,command,status) ) {
        usage(argv[0]);
        exit(1);
    }

    // TIDY UP
    usb_close(handle);
    return 0;
}
