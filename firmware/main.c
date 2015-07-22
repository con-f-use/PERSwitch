/* encoding: UTF-8, break: linux, indet-mode: 4 spaces, lang: avrC90/eng */
/**
 * \file main.c
 * MCU Firmware for PERswitch.
 *
 * This contains the main magic on the divice's side.
 * There are two usb requests implemented: CMD_GET and CMD_SET. Respectively they
 * get or set the status on a given pin.
 * \note There is a lot crammed into the tiny 2k of the ATtiny26 controller.
 * Even minor changes in this code might cause the compiled version not to fit
 * into the ATtiny's program memory.
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

#include <avr/io.h>             // standard input/output
#include <avr/interrupt.h>      // for sei()
#include <util/delay.h>         // for _delay_ms()
#include <avr/pgmspace.h>

#include "osccal.h"
#include "./usbdrv/usbdrv.h"    // usbdriver by http://www.obdev.at
#include "requests.h"           // The custom request numbers we use

#define HELP_CONCAT_MACROS(x,y) x##y ///< Helper necessary to concat two macros
#define CONCAT_MACROS(x,y) HELP_CONCAT_MACROS(x,y)///< Fuses two Macros together

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */
/**
 * Pin-Setting-Function. Sets the status of a specified pin (HIGH or LOW).
 *
 * \param port Port the pin belongs to
 * \param pinNbr Number of the pin
 * \param pinStat Status of the pin (HIGH or LOW)
 */
void writePin(volatile uint8_t *port, uint8_t pinNbr, uint8_t pinStat) {
    if(pinStat) {
        *port |= pinNbr;
    } else {
        *port &= ~pinNbr;
    }
}

/**
 * USB-Setup-Handler. Handles setup-calls that are received from the
 * USB-Interface i.e. whole communication between the device and the host.
 * \param data Eight bytes of data: (byte, name, descriton)
 *  - 0 = bmRequestType,    Bitmask containing information about the context.
 *  - 1 = bRequest,         User specified number to identify request.
 *  - 2-5 = wValue wIndex,  In vendor requests this can be arbitrary data else
 *      it's defined in the specifications.
 *  - 6-7 = wLength,        Length of the data to follow.
 * \return The number of returned bytes (in replyBuffer[]).
 */
uint8_t usbFunctionSetup(uint8_t data[8]) {
    // GET REQUEST DATA
    usbRequest_t *rq = (void *)data;
    static uint8_t dataBuffer[2]; // buffer must stay valid after return
    uint8_t portNbr = rq->wValue.bytes[1];
    uint8_t pinNbr = 1<<rq->wValue.bytes[0];
    uint8_t pinStat = rq->wIndex.bytes[0];
    usbMsgPtr = dataBuffer;

    // IDENTIFY AND EXECUTE COMMAND
    // Get status command
    if(rq->bRequest == CMD_GET){
        if(portNbr == 1) { // PORT B Pins
            dataBuffer[0] = (PORTB & pinNbr) != 0;
        } else { // PORT A Pins
            dataBuffer[0] = (PORTA & pinNbr) != 0;
        }
        return 1;
    }
    // Set status command
    else if(rq->bRequest == CMD_SET && pinNbr<(1<<8) ){//eight pins on the ports
        if(
            portNbr == 1 && // PORT B
            pinNbr!=USB_CFG_DPLUS_BIT &&    // Exept D+ and
            pinNbr!=USB_CFG_DMINUS_BIT      // Except D- pin
        ) {
            writePin(&PORTB,pinNbr,pinStat);
        } else { // PORT A
            writePin(&PORTA,pinNbr,pinStat);
        }
        dataBuffer[0] = portNbr;
        dataBuffer[1] = pinNbr;
        return 2;
    }

    // RETURN
    return 0; // default for not implemented requests
}

/* ------------------------------------------------------------------------- */
/* -------------------------- BORING PREPATATIONS -------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * Main-function. Initializes the hardware and starts the main loop of the
 * application. Nothing terribly exciting here.
 * \return Error code - nothing in this case.
 */
int main(void) {
    // INITIALIZE STUFF

    loadOldCallibartion();
    usbInit();

    // ENFORCE REENUMBERATION
    // fake USB disconnect for > 255 ms (uint8_t i has 255 values)
    usbDeviceDisconnect(); _delay_ms(255); usbDeviceConnect();

    // PREPARE PINS
    DDRA = 1; DDRB = 1; // set all pins on port A and B outputs
    /* The following makes D- and D+ pins of the given port into inputs.
     * After preprocessing this macro voodo would read something like:
     * "DDRB = ~( (1<<2) | (1<<3) );" */
    CONCAT_MACROS(DDR,USB_CFG_IOPORTNAME) =
        ~( (1<<USB_CFG_DPLUS_BIT) | (1<<USB_CFG_DMINUS_BIT) )
    ;
    sei();  // Interrupts

    // INFINITE MAINLOOP (reset watchdog and poll usb)
    for(;;) {
        usbPoll();
    }

    return 0;
}
