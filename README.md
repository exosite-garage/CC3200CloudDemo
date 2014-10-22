Note: This code is now depreciated and won't be updated in future. The second
version is in process and is available in the v2 branch. All new development 
should be done against the version 2 code, but note that it is still prerelease
quality and is not feature complete. (But is already more reliable than the code
in this repo.)

================================================================================
About Cloud enabled CC3200 Launchpad
================================================================================

This project is an embedded software project meant to run on the Texas
Instruments cc3200 launchpad to demonstrate interacting with
the Exosite cloud using the onboard WiFi module.

The project functionality includes:

  * Write Temperature Sensor Value
  * Write Accelerometer Movment
  * Write Push Button Count
  * Write IP Address Once on Boot
  * Control LED On/Off from Web Dashboard

Tested with:
  * Code Composer Studio v6 (Version: 6) for Windows
  * CC3200 Compiler
  * CC3200 launchpad (Rev 3.2)
  * CC3200 SDK v0.5.2

================================================================================
Developer Getting Started (Build Project & Program)
================================================================================
These directions assume that you have already successfully configured Code
Composer Studio to build example applications from the CC3200 SDK, if not,
follow the instructions in the Quick Start Guide included in the SDK.

1) Clone this repository or download the zip and extract.

2) Download the TI RTOS and install it.
   See: http://processors.wiki.ti.com/index.php/TI-RTOS You want the
   'tirtos_simplelink_2_00_02_36 Windows' version.

3) Open Code Composer Studio v6.0

4) Import all the projects.
   * Select "Projects -> Import CCS Projects..."
   * Browse for and select the folder you created in step one.
   * Check all boxes in "Discovered Projects"
   * Click Finish

5) Import the TI RTOS project (`ti_rtos_config`)
   * Select "Projects -> Import CCS Projects..."
   * Browse for and select the installation folder.
     (`C:\ti\tirtos_simplelink_2_00_02_36` by default.)
   * Check the box for `ti_rtos_config`.
   * Click Finish

6) Build Each Project
   * "ti_rtos" (must be first)
   * "driverlib"
   * "oslib"
   * "simplelink"
   * "exosite_cloud_demo_for_cc3200" (must be last) 

7) You can now flash the device using uniiflash or run the debugger, but note
   that you must flash it at least once before running it in the debugger to get
   the supporting files.

8) Open the ucf file using the uniflash tool, select the proper COM port and
   press the "Program" button, it may then ask you to press the reset button.
   Note that the board must be in flash more, do this by placing a jumpper on
   the SOP2 header. 

9) Run the application (remove the SOP2 header), if you're watching the serial
   terminal you should see:

          Manufacturer ID: 0x5449
          Device ID: 0x67
          Configuration register value: 0x7480
          CHIP ID: 0xf8
          ProfileID:0
          Scan AP in STA mode
          0) SSID exosite_demo_wep
            ....
          Switch back to AP Mode

10) If it already had a valid AP's profile in MCU, you will see below message

          SSID:exosite16
          SSID Len:9
          Auto Connect..
          STA mode setup ready.
          Work in STA mode.
          Wait to auto connect..
          MAC - 78:a5:4:10:f7:95
          MAC string : 78a50410f795
          IP Addess : 10.137.1.85
          Exosite Cloud App Start.
            ...

11) If it did not have a valid AP's profile, connect to the
    "mysimplelink-XXXXXX" Access Point on smartphone, tablet or notebook.

12) Open the browser on the smartphone, tablet or notebook, and connect to
    "mysimplelink.net"

13) Click the Profiles icon to fill your Access Point's SSID, Security type
    and Security Key in Add Profile item, then click Add button

14) The device will connect to your Access Point, and to the Exosite cloud.

================================================================================
LED Status
================================================================================
** LED D7 - blinking 2 times - NO Authentication or TCP socket error
          - blinking 3 times - CIK not valid or Conflict

================================================================================
Known Issues
================================================================================
* Getting debugger errors, not sure if code/config/project problem or virtualbox
  problem. (Let me know if you see any issues.)
  - Workaround: Wait until it breaks in boot.asm and press continue, your code
    will then run normally.
* The temperature readings seem to be very eratic, jupping several degrees
  within a single second.
* The application will only run for about 20 minutes, this seems to be an issue
  related to using i2c.
  - Update: The first stability issue has been "fixed". It has something to do
    with running out of sockets. I'm not sure if this means file descriptors
    or actual open sockets where they just aren't getting closed quick enough.
    The "fix" is to just disconnect from the AP and start over any time we get
    an error we're not expecting.

================================================================================
Release Info
================================================================================
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Release 2014-08-08
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Initial Public Release
* Note: Based On Internal Commit 925f5948cf37bdf480b96dd34b0cc9f87d507eef
