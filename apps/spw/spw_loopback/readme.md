# SpaceWire Loopback Example

This example shows how to configure the SpaceWire peripheral to send and receive multiple packets. It uses multiple receive buffer to exercise in an optimize way the 3 states (NEXT, CURRENT, PREVIOUS) for packet reception.

**Building The Application**

The parent folder for all the MPLABX IDE projects for this application is given below:

* *Application Path* : apps\spw\spw_loopback\firmware

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
    SPW loopback example
    -----------------------------------------------------------
    Wait for both SWP link switch to run state
    SPW Link in Run state
    TX time=2284450 us, for 25000000 bytes
    Effective rate=87 Mb/s
    Average RX process time = 148 us
    Rx sequence errors = 0
    End of transfer
    ```
