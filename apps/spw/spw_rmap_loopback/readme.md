﻿---
parent: Examples applications
title: SpaceWire RMAP Loopback Example
nav_order: 7

family: SAMRH71
function: SpaceWire
market:
  - aerospace
---

# SpaceWire RMAP Loopback Example

This example shows how configure the SpaceWire peripheral to configure the RMAP module, send an RMAP command with the packet transceiver and receive the RMAP reply with the packet receiver.

## Building The Application
The parent folder for all the MPLAB X IDE projects for this application is given below:

**Application Path**: aerospace\apps\spw\spw_rmap_loopback\firmware

To build the application, refer to the table below and open the appropriate project file in MPLABX IDE.

| Project Name  | Description   |
| ------------- |:-------------:|
| sam_rh71_ek.X | SAM RH71 Evaluation Kit board  |


## MPLAB Harmony Configurations

Refer to the MHC project graph for the components used and the respective configuration options.

## Hardware Setup

1. Project sam_rh71_ek.X
    * Hardware Used
        * SAM RH71 Evaluation Kit
        * SpaceWire cable
    * Hardware Setup
        * Connect the debugger probe to J33
        * Connect the USB port on the board to the computer using a mini USB cable.
        * Connect the two SpaceWire link on the board to each other using a SpaceWire cable.

## Running The Application

1. Open the Terminal application (Ex.:Tera term) on the computer.
2. Connect to the Virtual COM port and configure the serial settings as follows:
    * Baud : 115200
    * Data : 8 Bits
    * Parity : None
    * Stop : 1 Bit
    * Flow Control : None
3. Build and Program the application using the MPLAB X IDE.
4. See the following message in the console.

```console
-----------------------------------------------------------

  SPW RMAP loopback example

-----------------------------------------------------------

Wait for both SWP link switch to run state
    SPW Link in Run state
Initialize packet receiver to receive 1 packet(s) of 8 bytes
Send SWP packet with RMAP command write with reply
Wait RMAP reply
    Receive RMAP WRITE reply : ID = 0xabcd, with status : 0x0
  RMAP write command reply with status OK
Initialize packet receiver to receive 1 packet(s) of 80 bytes
Send SWP packet with RMAP command read
Wait RMAP reply
    Receive RMAP READ reply : ID = 0xabcd, with status : 0x0
      Receive 64 bytes of data :
        00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 0A, 0B, 0C, 0D, 0E, 0F,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 1A, 1B, 1C, 1D, 1E, 1F,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 2A, 2B, 2C, 2D, 2E, 2F,
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 3A, 3B, 3C, 3D, 3E, 3F,
  RMAP read command reply with status OK
```
