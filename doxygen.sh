#!/bin/bash
# Indent: tab, Tabsize: 4, Encoding: UTF-8, Language: Bash, Breaks: linux
################################################################################
#
# This file is part of PERSwitch.
# Copyright © 2010 C. Bischko.
# Use under GNU General Public License.
#
################################################################################

clear
clear
doxygen doxygen.cfg
cd docu/latex
make
cp refman.pdf ../
cd ..
rm -r latex/
evince refman.pdf&
