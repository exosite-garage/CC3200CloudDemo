================================================================================
Exosite CC3200 Cloud Demo
================================================================================

This project is an embedded software project meant to run on the Texas
Instruments cc3200 launchpad to demonstrate interacting with the Exosite cloud
using the onboard WiFi connectivity.

The project functionality includes:

  * Write Temperature Sensor Value
  * Write Accelerometer Movment
  * Write Push Button Count (Planned)
  * Control LED On/Off from Web Dashboard (Planned)

Tested with:
  * Code Composer Studio 6 On Windows
  * CC3200 Compiler
  * CC3200 Launchpad (Rev 4.2)
  * CC3200 SDK v1.0.1

================================================================================
Developer Getting Started (Build Project & Program)
================================================================================
These directions assume that you have already successfully configured Code
Composer Studio to build example applications from the CC3200 SDK, if not,
follow the instructions in the Quick Start Guide included in the SDK.

1) Clone this repository or download the zip and extract.

2) Open Code Composer Studio v6.0 or greater.

3) Import the project files.
   * Select "Projects -> Import CCS Projects..."
   * Browse for and select the folder you created in step one.
   * Check the box for this project in "Discovered Projects"
   * Click Finish

4) Build The Project

5) You can now flash the device using uniflash or run the debugger, but note
   that you must do a full system flash using uniflash at least once before you
   will be able to run the application in the debugger to install necessary
   supporting files.

6) Open the ucf file using the uniflash tool, select the proper serial port and
   press the "Program" button, it may then ask you to press the reset button.
   Note that the board must be in flash more, do this by placing a jumper on
   the SOP2 header.

7) Open a serial terminal application such as CoolTerm or Putty and connect to
   the debugging serial port.

8) If not already installed, place a jumper between VCC and P58 to enable AP
   mode for configuring of network parameters.

9) Run the application.
   * Remove the SOP2 jumper.
   * Reset the board.

10) Connect to the "mysimplelink-XXXXXX" Access Point on smartphone, tablet or
    laptop and open the webpage at "mysimplelink.net".

11) Take note of the MAC address of your board from the status page, you'll need
    it in a minute.

12) Click the profiles link to fill your Access Point's SSID, Security type
    and Security Key in Add Profile item, then click Add button

13) Remove the P58 jumper and reset the board. It will now connect to your
    network and begin communicating with the Exosite OnePlatform.

14) Add a matching client to your portal on https://ti.exosite.com.
   * In the Manage menu, click "Devices".
   * Click "+ Add Device"
   * Choose to add a CC3200 type device
   * Give your device a name and enter the device's MAC address.
   * After clicking through the wizard you will be taken to the dashboard for
     your new CC3200 Launchpad.
   * (Note: If the board has been running for some time it may take up to 10
      minutes to activate. You can force an activation attempt by resetting the
      board again. )

================================================================================
Known Issues
================================================================================

* The "User Button" counting from v1 has not been implemented yet.
* Setting the LED on or off from v1 has not been implemented yet.
* Bloat: There is a lot of code and html/js files left over from the out of box
  demo that this is based on. This needs to be removed and replaced with a nice
  looking, mobile friendly, single page, the C code that makes the API will
  probably stay as-is.

================================================================================
Release Info
================================================================================
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Release 2014-10-11
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Initial Public Release
