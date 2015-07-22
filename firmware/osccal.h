/* Tabsize: 4, Encoding: UTF-8, Language: C++/English, Linux-Style linebreaks */
/**
 * \file osccal.h
 * Responsoble for the calibration of the interal RC oscillator.
 *
 * This module contains a function which calibrates the AVR's internal RC
 * oscillator so that the CPU runs at F_CPU (F_CPU is a macro which must be
 * defined when the module is compiled, best passed in the compiler command
 * line).
 *
 * The time reference is the USB frame clock of 1 kHz available
 * immediately after a USB RESET condition. Timing is done by counting CPU
 * cycles, so all interrupts must be disabled while the calibration runs. For
 * low level timing measurements, usbMeasureFrameLength() is called. This
 * function must be enabled in usbconfig.h by defining
 * USB_CFG_HAVE_MEASURE_FRAME_LENGTH to 1. It is recommended to call
 * calibrateOscillator() from the reset hook in usbconfig.h:
 * \code
 * #ifndef __ASSEMBLER__
 * #include <avr/interrupt.h>  // for sei()
 * extern void calibrateOscillator(void);
 * #endif
 * #define USB_RESET_HOOK(resetStarts)  if(!resetStarts){cli(); calibrateOscillator(); sei();}
 * \endcode
 * This routine is an alternative to the continuous synchronization described
 * in osctune.h.
 *
 * \section Algorithm
 *
 * calibrateOscillator() first does a binary search in the OSCCAL register for
 * the best matching oscillator frequency. Then it does a next neighbor search
 * to find the value with the lowest clock rate deviation. It is guaranteed to
 * find the best match among neighboring values, but for version 5 oscillators
 * (which have a discontinuous relationship between OSCCAL and frequency) a
 * better match might be available in another OSCCAL region.
 *
 * \section Limitations
 * This calibration algorithm may try OSCCAL values of up to 192 even if the
 * optimum value is far below 192. It may therefore exceed the allowed clock
 * frequency of the CPU in low voltage designs!
 * Precision depends on the OSCCAL vs. frequency dependency of the oscillator.
 * Typical precision for an ATMega168 (derived from the OSCCAL vs. F_RC diagram
 * in the data sheet) should be in the range of 0.4%. Only the 12.8 MHz and
 * 16.5 MHz versions of V-USB (with built-in receiver PLL) can tolerate this
 * deviation! All other frequency modules require at least 0.2% precision.
 *
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary
 * \date 2008-04-10
 * \author Christian Starkjohann
 * \version This Revision: $Id: osccal.h 762 2009-08-12 17:10:30Z cs
 */

#ifndef __OSCCAL_H_INCLUDED__
#define __OSCCAL_H_INCLUDED__

#ifndef uchar
#define uchar   unsigned char
#endif

/**
 * Calibrate the RC oscillator.
 *
 * Our timing reference is the Start Of Frame
 * signal (a single SE0 bit) repeating every millisecond immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 *
 * \note This calibration algorithm may try OSCCAL values of up to 192 even if
 * the optimum value is far below 192. It may therefore exceed the allowed clock
 * frequency of the CPU in low voltage designs!
 * You may replace this search algorithm with any other algorithm you like if
 * you have additional constraints such as a maximum CPU clock.
 * For version 5.x RC oscillators (those with a split range of 2x128 steps, e.g.
 * ATTiny25, ATTiny45, ATTiny85), it may be useful to search for the optimum in
 * both regions.
 *
 * \note This function calibrates the RC oscillator so that the CPU runs at F_CPU.
 * It MUST be called immediately after the end of a USB RESET condition!
 * Disable all interrupts during the call!
 * It is recommended that you store the resulting value in EEPROM so that a
 * good guess value is available after the next reset.
 */
void calibrateOscillator(void);

/**
 * Loads old oscillator calibration value.
 *
 * \return Sqrt(i)
 */
void loadOldCallibartion(void);


#endif /* __OSCCAL_H_INCLUDED__ */
