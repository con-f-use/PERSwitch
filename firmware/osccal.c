/* Tabsize: 4, Encoding: UTF-8, Language: C++/English, Linux-Style linebreaks */
/**
 * \file osccal.c
 * Sourcefile to osccal.h. \see osccal.h
 *
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 * \date 2008-04-10
 * \author Christian Starkjohann
 * \version This Revision: $Id: osccal.h 762 2009-08-12 17:10:30Z cs
 */

#include <avr/io.h>             // required for basic avr use
#include <avr/eeprom.h>         // for storge of the old calibration value

#include "./usbdrv/usbdrv.h"

/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */

void calibrateOscillator(void) {
    uchar       step = 128;
    uchar       trialValue = 0, optimumValue;
    int         x, optimumDev,
                targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

    /* do a binary search: */
    do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    }while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev){
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}

/**
 * Reset hook. Necessary for the calibration via USB.
 * \see calibrateOscillator
 * \return Your Mom!
 */
void usbEventResetReady(void) {
    calibrateOscillator();
    eeprom_write_byte(0, OSCCAL);   /* store the calibrated value in EEPROM */
}

/**
 * Loads the old calibration value for the internal rc oscillator.
 */
void loadOldCallibartion(void) {
    uchar calibrationValue;
    calibrationValue = eeprom_read_byte(0); /* calibration value from last time */
    if(calibrationValue != 0xff){
        OSCCAL = calibrationValue;
    }
}
