![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# MPLAB® Harmony 3 Aerospace Application for SAM RH71 Family

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices.  Refer to the following links for more information.

- [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
- [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
- [Microchip MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
- [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)

This repository contains the MPLAB® Harmony 3 Aerospace Application examples for SAM RH71 Family.

- [Release Notes](./release_notes.md)
- [MPLAB® Harmony License](Microchip_SLA001.md)

# Documentation

Click [here](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=AEROSPACE_APPS_SAM_RH71_INTRODUCTION&redirect=true) to view the online documentation of code examples hosted in this repository.

To view the documentation offline, follow these steps:

- Download the publication as a zip file from [here](https://onlinedocs.microchip.com/download/GUID-2DDC6AFC-2AB8-4F56-B4BD-6814989295CD?type=webhelp).
- Extract the zip file into a folder.
- Navigate to the folder and open **index.html** in a web browser of your choice.

# Contents Summary

| Folder     | Description                                               |
| ---        | ---                                                       |
| apps       | Example applications for Aerospace SAM RH71 Family        |

## Code Examples

The following applications are provided to demonstrate the typical or interesting usage models of one or more peripheral libraries.

| MIL1553 Examples | Description |
| --- | :---: |
| [IP1553 Bus Controller operation blocking](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=AEROSPACE_APPS_SAM_RH71_IP1553_BC_OPERATION_BLOCKING&redirect=true) | This example shows how to start the IP1553 module in Bus Controller mode and issue data transfer commands in a blocking manner. |
| [IP1553 Bus Controller operation interrupt](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=AEROSPACE_APPS_SAM_RH71_IP1553_BC_OPERATION_INTERRUPT&redirect=true) | This example shows how to start the IP1553 module in Bus Controller mode and issue data transfer commands in a non-blocking manner. |
| [IP1553 Remote Terminal operation blocking](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=AEROSPACE_APPS_SAM_RH71_IP1553_RT_OPERATION_BLOCKING&redirect=true) | This example shows how to start the IP1553 module in Remote Terminal mode with RT01 address and receive data transfer commands in a blocking manner. |
| [IP1553 Remote Terminal operation interrupt](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=AEROSPACE_APPS_SAM_RH71_IP1553_RT_OPERATION_INTERRUPT&redirect=true) | This example shows how to start the IP1553 module in Remote Terminal mode with RT01 address and receive data transfer commands in a non-blocking manner. |

| ICM with ECC errors injections Example | Description |
| --- | :---: |
| [ICM with ECC errors injections](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=AEROSPACE_APPS_SAM_RH71_ICM_ECC_ERROR_INJECTION&redirect=true) | This example shows how configure the ICM and the different ECC controllers to handler and correct if possible ECC errors. |

| SpaceWire Examples | Description |
| --- | :---: |
| [SpaceWire Loopback](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=AEROSPACE_APPS_SAM_RH71_SPW_LOOPBACK&redirect=true) | This example shows how configure the SpaceWire peripheral to send and receive multiple packets. |
| [SpaceWire RMAP Loopback](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=AEROSPACE_APPS_SAM_RH71_SPW_RMAP_LOOPBACK&redirect=true) | This example shows how configure the SpaceWire peripheral to configure the RMAP module, send an RMAP command with the packet transceiver and receive the RMAP reply with the packet receiver. |
| [SpaceWire Escape Character and Time Code Handler Loopback](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=AEROSPACE_APPS_SAM_RH71_SPW_ESCCHAR_TCH_LOOPBACK&redirect=true) | This example shows how to configure the SpaceWire peripheral to handle escape character with Link interface and with TCH module. |

____

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71/blob/master/Microchip_SLA001.md)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71.svg)](https://github.com/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71.svg)]()

____

[![Developer Help](https://img.shields.io/badge/Youtube-Developer%20Help-red.svg)](https://www.youtube.com/MicrochipDeveloperHelp)
[![Developer Help](https://img.shields.io/badge/XWiki-Developer%20Help-torquiose.svg)](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/harmony/)
[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/aerospace.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/aerospace.svg?style=social)]()
