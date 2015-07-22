#!/usr/bin/env perl
# Tabsize: 4, Encoding: UTF-8, Language: Perl/English, Linux-Style linebreaks
#
# GUI frontent for obdev's usbtool specificaly designed to send commands to a
#+custom build usb power switch (PERswitch).
# by con-f-use@gmx.net

# INCLUDES AND GLOBALS
use Glib qw/TRUE FALSE/;
use Gtk2 '-init';
use utf8;

#Edit the following line to the correct path to the commandline tool
$usbtool = "/home/confus/.cnfsbin/./PERScom";

#Name of the 5 relay switching channels (feel free to edit)
@relay = ("unused 1","unused 2","unused 3","unused 4","unused 5");

################################################################################
################################### CALLBACKS ##################################
################################################################################

# CALLBACK TO OPEN THE MENU
sub popupMenu {
    ($widget, $button, $time) = @_;
    # update pin status (all pin stati are queried)
    $i = 0;
    foreach( @menuItem ) {
        $i++;
        $itmLabel = $_->get_child;
        $itmTxt = $itmLabel->get_label;
        $itmTxt =~ s/\ (off|on)$//;
        $usbReturn = qx($usbtool 0 $i status);
        if($usbReturn =~ /.*off.*/) {
            $itmLabel->set_label("$itmTxt off");
            @pinState[$i] = 0;
        } else {
            $itmLabel->set_label("$itmTxt on");
            @pinState[$i] = 1;
        }
    }
    # open menu
    ($x, $y, $push_in) = Gtk2::StatusIcon::position_menu($menu, $widget);
    $menu->show_all();
    $menu->popup( undef, undef, sub{return ($x,$y,0)}, undef, $button, $time);
}

# CALLBACK FOR THE ACTUAL SWITCHING
sub togglePin {
    $pin = @_[1];
    $call = @pinState[$pin] == 1 ? "off" : "on";
    system("$usbtool 0 $pin $call");
}

################################################################################
################################# INITIALISATION ###############################
################################################################################

#CREATE STATUS ICON
$status_icon = Gtk2::StatusIcon->new_from_stock('gtk-preferences');
$status_icon->set_tooltip("Controls a custom build power switch");
$status_icon->signal_connect('popup-menu', \&popupMenu);
$status_icon->signal_connect('activate', \&popupMenu);
$status_icon->set_visible(1);

# CREATE MENU
$menu = Gtk2::Menu->new();

# one item for each channel of the switch
$i = 0;
foreach (@relay) {
    $i++;
    $menuItem = Gtk2::ImageMenuItem->new($_);
    $menuItem->signal_connect('activate', \&togglePin, $i);
    $menu->append($menuItem);
    push (@menuItem, $menuItem);    #holds all menu items related to the switch
    push (@pinState, 0);            #holds their status (on|off)
}

# quit button
$menu->append(Gtk2::SeparatorMenuItem->new);
$menuItem = Gtk2::ImageMenuItem->new_from_stock('gtk-quit');
$menuItem->signal_connect('activate', sub{Gtk2->main_quit();} );
$menu->append($menuItem);

Gtk2->main();
