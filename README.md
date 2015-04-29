Broadcom BCM43143 Firmware MAC Tweaker
======================================

This utility will automatically change the hard-coded
MAC address in the BCM43143 WiFi adapter firmware file
if it is currently the factory default.  A new random
MAC address is selected and automatically inserted into
the file.  You can re-program it as many times as you like
by specifying the -f flag to force a new change, and you can
provide your own MAC address to use with the -m flag.  MAC
addresses must be provided in the format xx:xx:xx:xx:xx:xx
where 'xx' is a two-digit hexadecimal value between 00 and ff.

