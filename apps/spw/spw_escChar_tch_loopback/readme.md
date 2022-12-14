# SpaceWire Escape Character and Time Code Handler Loopback Example

This example shows how to configure the SpaceWire peripheral to handle escape character. It shows several ways to do so:

* With Link interfaces:
  * Send escape character.
  * Generate matching interrupt event on incoming escape character.
  * Generate distributed interrupt or distributed interrupt acknowledge event on corresponding incoming escape character.
* With Time Code Handlers:
  * Distribute Time Code periodically using internal events.
  * Generate interrupts on matching incoming time code.

**Building The Application**

The parent folder for all the MPLABX IDE projects for this application is given below:

* *Application Path* : apps\spw\spw_escChar_tch_loopback\firmware

To build the application, refer to the table below and open the appropriate project file in MPLABX IDE.

| Project Name  | Description   |
| ------------- |:-------------:|
| sam_rh71_ek.X | SAM RH71 Evaluation Kit board  |

**MPLAB Harmony Configurations**

Refer to the MHC project graph for the components used and the respective configuration options.

**Hardware Setup**

1. Project sam_rh71_ek.X
    * Hardware Used
        * SAM RH71 Evaluation Kit
        * SpaceWire cable
    * Hardware Setup
        * Connect the debugger probe to J33
        * Connect the USB port on the board to the computer using a mini USB cable.
        * Connect the two SpaceWire link on the board to each other using a SpaceWire cable.

**Running The Application**

1. Open the Terminal application (Ex.:Tera term) on the computer.
2. Connect to the Virtual COM port and configure the serial settings as follows:
    * Baud : 115200
    * Data : 8 Bits
    * Parity : None
    * Stop : 1 Bit
    * Flow Control : None
3. Build and Program the application using the MPLABX IDE.
4. See the following message in the console.

    ```console
    -----------------------------------------------------------

      SPW EscChar and Time Code Handler loopback example       

    -----------------------------------------------------------

    Wait for both SWP link switch to run state
        SPW Link in Run state
    Set link 2 escape character match event 1 for time code 0x22
    Transmit escape character (time code 0x22) on link 1
      - Link 2 Esc Event 1 : 0x22
    Configure TCH to send Time Code on Link 1
    Configure Time Code event to count time code '0x10' occurrences
    Prepare send of distributed interrupt '4' with send list that start on matching time code 0x04
    Manually send time code '0x04' now to trigger TX send
      - TC=0x04
      - Reload next PKTTX
      - Link2 DI=0x00000010
    Configure TCH to send Time code using RTCOUT0 event and restart every second
      - TC=0x05
      - TC=0x01
      - TC=0x02
      - TC=0x03
      - TC=0x04
      - Reload next PKTTX
      - Link2 DI=0x00000010
      - TC=0x05
      - TC=0x06
      - TC=0x07
      - TC=0x08
      - TC=0x09
      - TC=0x0A
      - TC=0x0B
      - TC=0x0C
      - TC=0x0D
      - TC=0x0E
      - TC=0x0F
      - TC=0x10
      - TC event
      - TC=0x11
      - TC=0x12
      - TC=0x13
      - TC=0x14
      - TC=0x15
      - TC=0x16
      - TC=0x17
      - TC=0x18
      - TC=0x19
      - TC=0x1A
      - TC=0x1B
      - TC=0x1C
      - TC=0x1D
      - TC=0x1E
      - TC=0x1F
      - TC=0x20
      - TC=0x01
      - TC=0x02
      - TC=0x03
      - TC=0x04
      - Reload next PKTTX
      - Link2 DI=0x00000010
      - TC=0x05
      - TC=0x06
      - TC=0x07
      - TC=0x08
      - TC=0x09
      - TC=0x0A
      - TC=0x0B
      - TC=0x0C
      - TC=0x0D
      - TC=0x0E
      - TC=0x0F
      - TC=0x10
      - TC event
    Stop TCH event and restart
    ```
