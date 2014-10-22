# CC3200 Cloud Demo

This repository contains code that demonstrates using the Exosite cloud platform
with TI's CC3200 WiFi launchpad.

# Using It

The first step is to flash all the needed files to the board. There is a `.ucf`
file in the "flash" folder, this is a config file for TI's uniflash tool. It
will flash the included mcu inage and all the supporting files to the sflash
on the launch pad.

If you haven't programmed an access point profile (your WiFi SSID and key) yet,
place a jumpper between the VCC and P58 pins of the launchpad header and reset
the board. It should now reboot into AP mode. Connect to the
'mysimplelink-XXXXXX' AP with a phone or laptop and go to
http://mysimplelink.net and configure your network settings profile just like
in the out of box demo.

Once configured, remove the jumpper and reset the board. It will now connect to
your network and attempt to activate itself on the Exosite platform. That means
that you can now add it to your account on https://ti.exosite.com and it should
then be able to activate itself and begin writing data to the platform.

# Known Issues

* There may be an I2C issue, but as far as I can tell it's a hardware issue, or
  at least at a lower level than I can easily access.
* The "User Button" counting has not been implemented yet.
* Setting the LED on or off has not been implemented yet.
* Bloat; There is a lot of code and html/js files left over from the out of box
  demo that this is based on. This needs to be removed and replaced with a nice
  looking, mobile friendly, single page, the C code that makes the API will
  probably stay as-is.
* Make it go into AP mode if no profiles are saved or can't connect.
