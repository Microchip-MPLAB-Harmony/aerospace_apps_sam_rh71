# IP1553 Remote Terminal operation interrupt Example

This example shows how to start the IP1553 module in Remote Terminal mode with RT01 address and receive data transfer commands in a non-blocking manner. The peripheral interrupt is used to manage the transfer. It waits for any incoming commands and display the buffer sent or the received data. It will print indications when mode command is received by the RT. The push buttons can be used to modify the RT configuration on answers to mode commands.

**Building The Application**

The parent folder for all the MPLABX IDE projects for this application is given below:

* *Application Path* : apps\ip1553\ip1553_rt_operation_interrupt\firmware

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
        * MIL1553 Exerciser and Cables
    * Hardware Setup
        * Connect the debugger probe to J33.
        * Connect the USB port on the board to the computer using a mini USB cable.
        * Connect the MIL1553 exerciser on 1553_BUSA or 1553_BUSB connectors.
    * Exerciser configuration
        * Configure in Bus commander to send data and mode commands to RT01.

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

    IP1553 - RT mode interrupt operation example

    -----------------------------------------------------------

    -> Set config : BCE=0, SREQ=0, Busy=0, SubSystem=0, TR=0

    -> Set vector word = 0xABCD and BIT word = 0xFEED

    MIL1553 RT mode, wait for BC command
    ```

5. With MIL1553 exerciser, send data transfer command to or from RT01.
    * It will echo the buffer sent or the received data.
6. With MIL1553 exerciser, send mode command to RT01.
    * It will print information of received mode command on RT.
7. With the push buttons modify the RT configuration and send new mode command.
    * With push button PB0, each press will activate 1 bit in configuration register:

        ```console
        -> Set config : BCE=1, SREQ=0, Busy=0, SubSystem=0, TR=0
        -> Set config : BCE=0, SREQ=1, Busy=0, SubSystem=0, TR=0
        -> Set config : BCE=0, SREQ=0, Busy=1, SubSystem=0, TR=0
        -> Set config : BCE=0, SREQ=0, Busy=0, SubSystem=1, TR=0
        -> Set config : BCE=0, SREQ=0, Busy=0, SubSystem=0, TR=1
        -> Set config : BCE=0, SREQ=0, Busy=0, SubSystem=0, TR=0
        ```

    * With push button PB1, each press will switch the values of bit word and vector word:

        ```console
        -> Set vector word = 0x1234 and BIT word = 0xCAFE
        -> Set vector word = 0xABCD and BIT word = 0xFEED
        ```
