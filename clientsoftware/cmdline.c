/* encoding: UTF-8, break: linux, indet-mode: 4 spaces, lang: C90/eng */
/**
 * \file cmdline.c
 * Source file for cmdline.h
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmdline.h"
#include "opendevice.h" // common code from obdev

#include "../firmware/requests.h"   // custom request numbers
#include "../firmware/usbconfig.h"  // device's VID/PID and names

void usage(char* name) {
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s [portNbr] [pinNbr] on ...     turn on PIN\n", name);
    fprintf(stderr, "  %s [portNbr] [pinNbr] off ...... turn off PIN\n", name);
    fprintf(stderr, "  %s [portNbr] [pinNbr] toggle ... toggle PIN on/off\n", name);
    fprintf(
        stderr,
        "  %s [portNbr] [pinNbr] status ... ask current status of PIN\n",
        name
    );
}

int createHandle(usb_dev_handle** handle) {
    int vid, pid;
    const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID};
    const unsigned char rawPid[2] = {USB_CFG_DEVICE_ID};
    char vendor[] = {USB_CFG_VENDOR_NAME, 0};
    char product[] = {USB_CFG_DEVICE_NAME, 0};

    // SEARCH FOR DEVICE AND CREATE HANDLE
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    //if(usbOpenDevice(handle, vid, NULL, pid, NULL, NULL, stderr, stderr) != 0){
    //if(usbOpenDevice(handle, vid, vendor, pid, product, NULL, NULL, stderr) != 0){
    if(usbOpenDevice(handle, vid, "*", pid, "*", NULL, NULL, stderr) != 0){
        fprintf(
            stderr,
            "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n",
            product,
            vid,
            pid
        );
        return 1;
    }
    return 0;
}

int doRequest(
    usb_dev_handle* handle, unsigned int portNbr, unsigned int pinNbr,
    char* command, int* status
) {
    char buffer[4];
    int cnt, isOn, isToggle=0;
    unsigned int pinPort = (portNbr << 8) + pinNbr;

    // DO ACTUAL REQUESTS
    if( // status request
        strcasecmp(command,"status") == 0 ||
        (isToggle = strcasecmp(command, "toggle") == 0)
    ) {
        cnt = usb_control_msg(
            handle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
            CMD_GET,
            pinPort,
            0,
            buffer,
            sizeof(buffer),
            5000
        );
        if(cnt < 1){
            if(cnt < 0){
                fprintf(stderr, "USB error: %s\n", usb_strerror());
            }else{
                fprintf(stderr, "only %d bytes received.\n", cnt);
            }
            return 1;
        }else{
            *status = buffer[0] ? 1 : 0;
            printf(
                "PIN %d on PORT %d is %s\n",
                pinNbr, portNbr, buffer[0] ? "on" : "off"
            );
            if(isToggle==1) {
                doRequest( handle, portNbr, pinNbr, buffer[0] ? "off" : "on", status);
                printf("...toggled %s\n", buffer[0] ? "off" : "on");
            }
        }
    }
    else if(    // change pin status request
        (isOn = (strcasecmp(command, "on") == 0)) ||
        strcasecmp(command, "off") == 0
    ){
        cnt = usb_control_msg(
            handle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
            CMD_SET, pinPort, isOn, buffer, 0, 5000
        );
        if(cnt < 0){
            fprintf(stderr, "USB error: %s\n", usb_strerror());
            return 1;
        }
    }
    // tell the user he did something wrong
    else {
        return 1;
    }
    return 0;
}
