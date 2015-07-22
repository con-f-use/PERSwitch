/* encoding: UTF-8, break: linux, indet-mode: 4 spaces, lang: avrC90/eng */
/**
 * \file requests.h
 * This header is shared between the firmware and the host software. It
 * defines the USB request numbers used to communicate between the host and the
 * device.
 * \date 31 Oct. 2010
 * \author <a href="mailto:con-f-use@gmx.net">con-f-use</a>
 * \copyright
 * This file is part of PERswitch. Copyright © 2012 con-f-use. Use permitted
 * under GNU General Public License.
 */

/**
 * \mainpage PERswitch
 *
 * \copyright This file is part of PERswitch. Copyright © 2012 con-f-use. Use
 * permitted under GNU General Public License (see LICENSE.txt).
\n
 *  PERswitch is a free electronics project: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
\n
 *  PERswitch is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
\n
 *  You should have received a copy of the GNU General Public License
 *  along with ltea. If not, see <http://www.gnu.org/licenses/>
 *
 * \date 31 Oct. 2010
 * \author <a href="mailto:jcgb@unethische.org">C. Bischko</a>
 *
 * \note This is not the final version of the project. Inspect everything
 * carefully before you use it. Especially make sure, that the boards are right
 * (fit together and are not mirror-inverted on one side)! The
 * documentation on the windows client and driver is missing. Consult the
 * obdev website to fill the gaps. I lost dirve to do that properly since I'm
 * not using windows anymore and the prototype worked.
 *
 * \image latex controller-frontside.jpg "Front of the Controller board" width=12cm
 *
 * \section sec_intro Introduction
 * I grew tired of crouching under my desk to power my printer, scanner, sound
 * and external HDDs. So with much inspiration form other projects I came up
 * with this one.
 *
 * The periphery switch (PERswitch for short) is a project consisting of two
 * boards: a relay card and a controller board. Its intended use is the
 * switching of 230 V AC / 50 Hz (European Standard) periphery from a PC via
 * USB. Since all periphery will be powered by the switch it might be a good
 * idea to include a cheap usb hub. This way the usb cables plug into the
 * switch as well and you kill two birds with one cable.
 *
 * PERswitch is fairly easy to build, even for beginners and a great way to get
 * into electronics, since you end up with a cheap product, that's usefull to
 * almost anybody. Building it will cost you about 10,- € and a day of your
 * time. It uses both surface mount and DIP components. So you will need the
 * propper tools to handle them as well as the means to etch double sided PCBs
 * at home.
 *
 * \note Most of this project is based on other projects. Most prominently the
 * examples that come with
 * <a href="http://www.obdev.at/products/vusb/index.html">VUSB</a> by
 * <a href="http://www.obdev.at">Objetive Developement</a>. Thaks a lot guys!
 *
 * \warning This projects works with 230 V AC at 50 Hz. This is potentially
 * dangerous. Handle with care and know, what you're doing.
 *
 * \section sec_manu Manufacturing the Boards
 *
 * I will not go into much detail here. Form the links below you can learn the
 * necessary skills to build the boards yourself:
 * - <a href="http://thomaspfeifer.net/direct_toner_pcb.htm">Make single sided
 *      PCBs</a>
 * - <a href="http://thomaspfeifer.net/platinen_aetzen_doppelseitig.htm">
 *      Make double sided PCBs</a> (In German - but you'll get the drift of
 *      things)
 * - <a href="http://store.curiousinventor.com/guides/How_to_Solder">Learn how
 *      to solder</a>
 *
 * For beginners I recommend to start with the relay board. It is single sided
 * and therefore easier to etch. Also it uses through hole components only,
 * which makes it a good soldering exercise for beginners.
 *
 * You will find the layouts in the \e 'board/' directory. The scematics specify
 * the components you'll need and where they go.
 * <a href="http://www.cadsoftusa.com/">Eagle</a> files are included in
 * \e 'board/eagle/' in case you might want to modify the design. Eagle is free
 * for private users.
 *
 * I included a cheap noname 5 port usb-hub into my own PERswitch. It is
 * fairly easy to identify the usb-lines on one of those (with the help of the
 * figure below). Just
 * <a href="http://store.curiousinventor.com/guides/how_to_solder/desoldering/">
 * desolder</a> one of the connectors. Then you can wire the hub directly to
 * to the controller board. You can use the wall plug that comes with your hub
 * to power all the boards. You might want to desolder all the hub's connectors
 * and solder cables directly to the boards in order to conserve space.
 *
 * \image latex usbconnector.png "Identify the usb lines" height=4cm
 *
 * \section sec_firm Compiling and flashing the firmware
 *
 * The firmware for this project requires avr-gcc and avr-libc (a C-library for
 * the AVR controller). Please read the instructions at
 * http://www.nongnu.org/avr-libc/user-manual/install_tools.html for how to
 * install the GNU toolchain (avr-gcc, assembler, linker etc.) and avr-libc.
 *
 * <a href="http://www.ladyada.net/learn/avr/index.html">Lady Ada's avr
 * tutorial</a> covers the basics of flashing firmware to a microcontroller
 * nicely. Note that the controller board comes with a 6-pin ISP-Header so you
 * can connect your programmer directly to the board.
 *
 * However, running "make program" in the \e firmware direcotry should be
 * sufficient. Maybe you will have to edit the Makefile especially if you are
 * using a programmer other than
 * <a href="http://www.ladyada.net/make/usbtinyisp/">USBtinyISP</a>
 * (btw.: you can build USBtinyISP at home).
 *
 * \section sec_cli Clientsoftware
 *
 * Source code and binaries are included for both Ubuntu 10.10 and Windows
 * (32 bit versions). The clientsoftware consists of a command line tool and a
 * graphical user interface (GUI). The GUI requires the
 * <a href="http://www.gtk.org/download.html">gtk2 toolkit</a>
 * both executables require
 * <a href="http://www.libusb.org/">libusb</a>.
 *
 * Here is a screen shot of the status icon created the GUI. Each entry
 * in the pull down menu corresponds to one of the relays. It will toggle the
 * respective device on and off. There is a configuration file to edit the
 * labels (or you might want to edit the source code),
 * \image latex menu.png "Screenshot of the client program" height=2.5cm
 *
 * I opted to use a vendor request model instead of the more common approach of
 * hid USB requests. That was because I wanted to fit the the code into the
 * small flash of the AtTiny26. Unfortunately that approach requires the
 * installation of a driver to work on Windows Operating system. Windows
 * generally handles custom USB solutions very badly. The driver is located in
 * the windows driver directory.
 *
 * \section sec_electrical Hooking it up
 *
 * \warning To use the board, you need to hook it up to the power line at some
 * point.
 * Please be extremely carefull. Be sure to read and understand the entirety of
 * this section before you work with 230V. Use your common sense. Wall
 * plug electricity can kill you! (and is extremely unpleasant even if it
 * doesn't).
 *
 * The first thing you will want to do is to test the boards. Using only the 5 V
 * supplied by the usb connector on your computer is fine. So no wall plug 230 V
 * at first! 5 Volts are enough to drive the relays and you will hear a
 * satisfying click sound from them, if you assembeld and hooked up everything
 * right.
 *
 * Now on the controller board the pins labeled 0 to 4 of "Port A" connect to the
 * 5 pin header labeld "1,2,3,4,5" on the relay board. The Pins labeld "+5V" and
 * "GND" on the contoller board have to be connected to their counterparts on
 * the relay board. If everything went fine you sould be able to switch the
 * channels using \e 'clientsoftware/PERScom' (or 'PERScom.exe' on Windows).
 *
 * Once this test is done, you can connect your periphery devices and the wall
 * plug cord to the screw terminals. Do not plug it into the wall just now and
 * use the proper eylets (kabel shoes) for your bare wires. V0 is the label for
 * wall plug ground (yellow/green wire), V+ is the AC conductor (brown
 * insulation) and V- the zero conducor (blue wire). The latter two are
 * basically interchangable for AC.
 *
 * Now you have everything hooked up. You neew to put it in a non conducting,
 * robust case, make sure that conducors on 230V AC potential never touch other
 * conducors (and will never do so in the future). <b>Make sure that no one -
 * keep children in mind - will be able to touch the 230V ciruitry by accident.
 * </b>
 *
 * \section Legal Stuff
 *
 * I don't guaratee that any of this will work for anyone. Neither do I feel
 * responsible for any damage caused by the use or missuse of this project or
 * the software provided with it. You are free to use and modify the project in
 * any way you deem appropriate, provided the licenses metioned in the
 * respective files don't say otherwise.
 *
 * Take care and have fun.
 */

#ifndef __REQUESTS_H_INCLUDED__
#define __REQUESTS_H_INCLUDED__

// DEFINITIONS
#define CMD_GET 1 ///< Command code for GET-Request
#define CMD_SET 2 ///< Command code for SET-Request

#endif /* __REQUESTS_H_INCLUDED__ */
