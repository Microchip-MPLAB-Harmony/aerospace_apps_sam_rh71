---
title: Harmony 3 Aerospace Application for SAM RH71 Family
nav_order: 1
has_children: true
has_toc: false
---

![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# MPLAB® Harmony 3 Aerospace Application for SAM RH71 Family

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices.  Refer to the following links for more information.

- [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
- [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
- [Microchip MPLAB® X IDE](https://www.microchip.com/mplab/mplab-x-ide)
- [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
- [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)

This repository contains the MPLAB® Harmony 3 Aerospace Application examples for SAM RH71 Family.

- [Release Notes](./release_notes.md)
- [MPLAB® Harmony License](mplab_harmony_license.md)
- [MPLAB® Harmony 3 Aerospace Application for SAM RH71 Family Help](https://microchip-mplab-harmony.github.io/aerospace_apps_sam_rh71)

# Contents Summary

| Folder     | Description                                               |
| ---        | ---                                                       |
| apps       | Example applications for Aerospace SAM RH71 Family       |
| docs       | Aerospace Application in html format for offline viewing. Use [github pages](https://microchip-mplab-harmony.github.io/aerospace_apps_sam_rh71/) of this repository for viewing it online. |

## Code Examples

The following applications are provided to demonstrate the typical or interesting usage models of one or more peripheral libraries.

| MIL1553 Examples | Description |
| --- | :---: |
| [IP1553 Bus Controller operation blocking](apps/ip1553/ip1553_bc_operation_blocking/readme.md) | This example shows how to start the IP1553 module in Bus Controller mode and issue data transfer commands in a blocking manner. |
| [IP1553 Bus Controller operation interrupt](apps/ip1553/ip1553_bc_operation_interrupt/readme.md) | This example shows how to start the IP1553 module in Bus Controller mode and issue data transfer commands in a non-blocking manner. |
| [IP1553 Remote Terminal operation blocking](apps/ip1553/ip1553_rt_operation_blocking/readme.md) | This example shows how to start the IP1553 module in Remote Terminal mode with RT01 address and receive data transfer commands in a blocking manner. |
| [IP1553 Remote Terminal operation interrupt](apps/ip1553/ip1553_rt_operation_interrupt/readme.md) | This example shows how to start the IP1553 module in Remote Terminal mode with RT01 address and receive data transfer commands in a non-blocking manner. |

| ICM with ECC errors injections Example | Description |
| --- | :---: |
| [ICM with ECC errors injections](apps/icm_with_ecc_error_injection/readme.md) | This example shows how configure the ICM and the different ECC controllers to handler and correct if possible ECC errors. |

| SpaceWire Examples | Description |
| --- | :---: |
| [SpaceWire Loopback](apps/spw/spw_loopback/readme.md) | This example shows how configure the SpaceWire peripheral to send and receive multiple packets. |
| [SpaceWire RMAP Loopback](apps/spw/spw_rmap_loopback/readme.md) | This example shows how configure the SpaceWire peripheral to configure the RMAP module, send an RMAP command with the packet transceiver and receive the RMAP reply with the packet receiver. |
| [SpaceWire Escape Character and Time Code Handler Loopback](apps/spw/spw_escChar_tch_loopback/readme.md) | This example shows how to configure the SpaceWire peripheral to handle escape character with Link interface and with TCH module. |

____

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71/blob/master/mplab_harmony_license.md)

[![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71.svg)](https://github.com/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71/releases/latest)

[![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71.svg)](https://github.com/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71/releases/latest)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71.svg)](https://github.com/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/aerospace_apps_sam_rh71.svg)]()

____

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/aerospace.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/aerospace.svg?style=social)]()
