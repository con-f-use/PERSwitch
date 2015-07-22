/* encoding: UTF-8, break: linux, indet-mode: 4 spaces, lang: C90/eng */
/**
 * \file cmdline.h
 * PERswitch utility functions. Used by both the PERcom command line client
 * and the PERgui tray program.
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

#include <usb.h>        // this is libusb

/**
 * Prints how to use the command line client.
 */
void usage(char* name);

/**
 * Creates the device handle for PERswitch.
 *
 * \param handle Pointer to the future handle
 *
 * \return 0 if the handle was created, 1 otherwise
 */
int createHandle(usb_dev_handle** handle);

/**
 * Does the actual request. Switches a pin #pinNbr at port #portNbr on, off or
 * requests its status.
 *
 * \param handle Handle created by \see createHandle
 * \param portNbr Number of the port the request is about
 * \param pinNbr Number of the pin the request is about
 * \param command Action that is to be performed (either "on", "off" or "status")
 *
 * \return 0 if the request was successfull, 1 otherwise
 */
int doRequest(
    usb_dev_handle* handle,
    unsigned int portNbr,
    unsigned int pinNbr,
    char* command,
    int* status
);
