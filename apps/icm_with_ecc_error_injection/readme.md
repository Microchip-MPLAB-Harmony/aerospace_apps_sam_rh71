# ICM with ECC error injection Application Example

This example shows how to configure the ICM and the different ECC controllers to handle and to correct the ECC errors. This example use the ECC test mode to inject ECC errors in the memories monitored by the ICM.

**Building The Application**

The parent folder for all the MPLABX IDE projects for this application is given below:

* *Application Path* : apps\icm_with_ecc_error_injection\firmware

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
    * Hardware Setup
        * Connect the debugger probe to J33
        * Connect the USB port on the board to the computer using a mini USB cable.

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

      ICM with ECC error injection example

    -----------------------------------------------------------

    Enable ICM region(0-3)...

    ICM monitoring started for region (0-3)...

    Press [Push Button 0] to change ecc error injection memory target(ITCM, DTCM, FlexRAM, external SDRAM)

    Press [Push Button 1] to inject fixable ecc error

    Press [Push Button 2] to inject unfixable ecc error

    Current ECC error injection is selected for memory : ITCM

    ```

5. By pressing the button you can change the memory area where ECC error are injected. When an error is injected, it display the interrupts that occurs and the ICM region mismatch if any.

    For example an error injection in ITCM:

    ```console
    -> Generate fixable error on region ITCM at index : 0

      1 new FIX interrupt on region ITCM
    ```
