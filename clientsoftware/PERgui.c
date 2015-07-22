/* Tabsize: 4, Encoding: UTF-8, Language: C++/English, Linux-Style linebreaks */
/**
 * \file PERgui.c
 * Controls the PERswitch periphery switch via a tray menu. Creates a system
 * tray icon. When clicked the icon pops a menu. Each menu item corresponds to
 * one of the PERswitch's channels and switches them on and off.
 *
 * On popup the menu refreshes the status of each channel automatically
 * ("on" or "off").
 *
 * \date 31. Oct. 2010
 * \author <a href="mailto:jcgb@unethische.org">C. Bischko</a>
 * \copyright
 * This file is part of PERswitch. Copyright © 2012 C. Bischko. Use permitted
 * under GNU General Public License.
 */

/* ------------------------------------------------------------------------- */
/* ----------------------- Definitions and Includes ------------------------ */
/* ------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <usb.h>
#include "cmdline.h"

#define MENU_SIZE 5
#define PORT_NBR 2
char* itemNames[MENU_SIZE] = {
    "Unnamed 1","Unnamed 2","Unnamed 3","Unnamed 4","Unnamed 5"
};

/* ------------------------------------------------------------------------- */
/* -------------------------------- Globals -------------------------------- */
/* ------------------------------------------------------------------------- */

int* pinStatus;
char* tmpItemNames[MENU_SIZE];
GtkWidget** menuItems;

/* ------------------------------------------------------------------------- */
/* --------------------------- Utility Functions --------------------------- */
/* ------------------------------------------------------------------------- */

int readNames(void) {
    int i;
    char* ptr;
    FILE* fileHandle = fopen("PERgui.cfg","r");
    for(i=0; i<MENU_SIZE; ++i)
        tmpItemNames[i] = (char*)calloc(64,sizeof(tmpItemNames[i]));

    // read file
    if(fileHandle != NULL) {
        for(i=0;
            i<MENU_SIZE &&  // don't read more lines than menu items
            fgets(tmpItemNames[i],64,fileHandle)!=NULL; //read until end of file
            ++i
        ) {
            if(tmpItemNames[i][3] != 0) {
                itemNames[i] = tmpItemNames[i];
                while( //remove new line characters
                    (ptr=strchr(itemNames[i],'\n')) != NULL ||
                    (ptr=strchr(itemNames[i],'\r')) != NULL
                ) {
                    *ptr = '\0';
                }
            }
        }
    // file not found
    } else {
        printf("No configuration file\n");
        return 1;
    }
    fclose(fileHandle);
    return 0;
}

void popNotFoundDialog(void) {
    int i;
    GtkWidget* dialog;
    dialog = gtk_message_dialog_new (
        NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_CLOSE,
        "Sorry, no PERswtich was found.\nMake sure the driver is installed "\
        "properly the device is connected."
    );
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    for(i=0; i<MENU_SIZE; ++i) {
        gtk_menu_item_set_label(GTK_MENU_ITEM(menuItems[i]),"Not Found");
    }
}

/* ------------------------------------------------------------------------- */
/* ------------------------------- Callbacks ------------------------------- */
/* ------------------------------------------------------------------------- */

static void opemnenu(
    GtkStatusIcon *status_icon, guint button, guint activate_time,
    gpointer user_data
) {
    int i;
    int status=0;
    char* charBuffer = (char*)malloc(128*sizeof(charBuffer));
    usb_dev_handle *handle = NULL;
    GtkMenu* menu = user_data;

    if( createHandle(&handle) == 0 ) {
        for(i=0; i<MENU_SIZE; ++i) {
            // channel on
            if( doRequest(handle,PORT_NBR,i+1,"status",&status)==0 && status ) {
                sprintf(charBuffer,"%s - %s",itemNames[i],"on");
                pinStatus[i] = 1;
            } else { // channel off
                sprintf(charBuffer,"%s - %s",itemNames[i],"off");
                pinStatus[i] = 0;
            }
            gtk_menu_item_set_label(GTK_MENU_ITEM(menuItems[i]),charBuffer);
        }
    } else {
        popNotFoundDialog();
    }
    free(charBuffer);
    gtk_menu_popup(
        GTK_MENU(menu),
        NULL,NULL,
        gtk_status_icon_position_menu,
        status_icon,
        button,
        activate_time
    );
}

static void channelCallback(GtkStatusIcon *widget, gpointer data) {
    int i = GPOINTER_TO_UINT(data);
    usb_dev_handle *handle = NULL;
    widget = widget;    //get rid of not used warning

    if( createHandle(&handle) == 0 ) {
        if( doRequest(handle,PORT_NBR,i+1,pinStatus[i]?"off":"on",NULL) == 0) {
            pinStatus[i] = !pinStatus[i];
        }
    } else {
        popNotFoundDialog();
    }
}

/* ------------------------------------------------------------------------- */
/* ------------------------------ Main Section ----------------------------- */
/* ------------------------------------------------------------------------- */

int main(int argc, char** argv) {
    // INITIALIZE STUFF
    int i;
    GtkStatusIcon* statusIcon;
    GtkWidget* menu;
    menuItems = (GtkWidget**)malloc(MENU_SIZE*sizeof(GtkWidget));
    pinStatus = calloc(MENU_SIZE,sizeof(pinStatus));
    GtkWidget* quitItem;
    GtkWidget* sepItem;

    gtk_init(&argc, &argv);
    usb_init();
    readNames();

    // CREATE STATUS ICON AND MENU
    statusIcon = gtk_status_icon_new_from_stock("gtk-preferences");
    gtk_status_icon_set_tooltip_text(
        statusIcon,"Controls a custom build power switch"
    );
    menu = gtk_menu_new();
    g_signal_connect(
        G_OBJECT(statusIcon),"popup-menu",G_CALLBACK(opemnenu),menu
    );
    gtk_status_icon_set_visible(statusIcon,TRUE);

    // CREATE MENU ITEMS
    for(i=0;i<MENU_SIZE;++i) {
        menuItems[i] = gtk_menu_item_new_with_label( itemNames[i] );
        gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuItems[i]);
        g_signal_connect(
            G_OBJECT(menuItems[i]),
            "activate",
            G_CALLBACK(channelCallback),
            GUINT_TO_POINTER(i)
        );
        gtk_widget_show(menuItems[i]);
    }

    // CREATE QUIT ITEM (and its seperator)
    sepItem = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),sepItem);
    gtk_widget_show(sepItem);
    quitItem = gtk_image_menu_item_new_from_stock("Quit",NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),quitItem);
    g_signal_connect_swapped(
        quitItem,"activate",G_CALLBACK(gtk_main_quit),NULL
    );
    gtk_widget_show(quitItem);

    // ENTER MAIN LOOP
    gtk_main();

    // CLEANUP
    free(menuItems);
    free(pinStatus);
    for(i=0;i<MENU_SIZE;++i) free(tmpItemNames[i]);
    return 0;
}
