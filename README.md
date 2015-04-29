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

The WiFi module will subsequently be identified as a new device
(wlan1 instead of wlan0 for instance) so you will need to
edit /etc/udev/rules.d/70-persistent-net.rules to change it back to
wlan0 and remove the old wlan0 entry, or disable the persistent
rules system completely by replacing the file with a symlink to
/dev/null.
