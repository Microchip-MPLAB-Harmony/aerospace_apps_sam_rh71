# IP1553 Bus Controller operation interrupt Example

This example shows how to start the IP1553 module in Bus Controller mode and issue data transfer commands in a non-blocking manner. The peripheral interrupt is used to manage the transfer. It performs BC to RT01 data transfer, RT01 to BC data transfer and BC to RTs broadcast data transfer. Mode command can also be sent to RT01 address or in broadcast mode.

**Building The Application**

The parent folder for all the MPLABX IDE projects for this application is given below:

* *Application Path* : apps\ip1553\ip1553_bc_operation_interrupt\firmware

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
        * Configure RT01 to reply to data transfer command form BC to RT01 buffer 1 of 1 word.
        * Configure RT01 to reply to data transfer command form RT01 buffer 3 to BC buffer 1 of 3 words.
        * Configure RT01 to reply to broadcast data transfer command form BC to RTs buffer 3 of 32 words.
        * Configure RT01 to receive any type of mode command

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

    IP1553 - BC mode interrupt operation example

    -----------------------------------------------------------
    Press '1' to send one data from TX buffer 0 to RT1, on Bus A
    Press '2' to receive one data in buffer 3 from RT1, on bus A
    Press '3' to broadcast for RTs, from buffer 1, on bus A
    Press '4' to initiate mode command transfer to RT1 on bus A
    Press '5' to initiate broadcast mode command transfer on bus A
    ```

5. Type one of the menu characters in the terminal. It will echo back the chosen option and the result of the 1553 BC command (status word and received data if applicable).
