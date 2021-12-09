/*******************************************************************************
  SPW Peripheral Library
  Instance Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw_router.h

  Summary:
    SPW PLIB ROUTER Header file

  Description:
    This file defines the interface to the SPW peripheral 
    library. This library provides access to and control of the associated 
    peripheral instance.
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef PLIB_SPW_ROUTER_H
#define PLIB_SPW_ROUTER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include <stdbool.h>
#include "device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: SPW defines
// *****************************************************************************
// *****************************************************************************
/* Router Packet link 1 port */
#define SPW_ROUTER_LINK1_PORT                                  (1)

/* Router Packet link 2 port */
#define SPW_ROUTER_LINK2_PORT                                  (2)

/* Router Packet receiver port */
#define SPW_ROUTER_PKTRX_PORT                                  (9)

/* Router RMAP port */
#define SPW_ROUTER_RMAP_PORT                                   (17)

/* Return SPW ROUTER DEST field from Router status word. */
#define SPW_ROUTER_STATUS_GET_DEST(status) ( (status & SPW_ROUTER_STATUS_DEST_MASK) >> SPW_ROUTER_STS_DEST_Pos )

/* Return SPW ROUTER SOURCE from Router status word. */
#define SPW_ROUTER_STATUS_GET_SOURCE(status) ( (status & SPW_ROUTER_STATUS_SOURCE_MASK) >> SPW_ROUTER_STS_SOURCE_Pos )

/* Return SPW ROUTER BYTE from Router status word. */
#define SPW_ROUTER_STATUS_GET_BYTE(status) ( (status & SPW_ROUTER_STATUS_BYTE_MASK) >> SPW_ROUTER_STS_BYTE_Pos )

/* Return SPW ROUTER COUNT from Router status word. */
#define SPW_ROUTER_STATUS_GET_COUNT(status) ( (status & SPW_ROUTER_STATUS_COUNT_MASK) >> SPW_ROUTER_STS_COUNT_Pos )

/* Return SPW ROUTER TIMEOUT ADDR from Router status word. */
#define SPW_ROUTER_TIMEOUT_STATUS_GET_ADDR(status) ( (status & SPW_ROUTER_TIMEOUT_STATUS_ADDR_MASK) >> SPW_ROUTER_TIMEOUT_ADDR_Pos )

// *****************************************************************************
// *****************************************************************************
// Section: SPW Types
// *****************************************************************************
// *****************************************************************************
/* SPW ROUTER table physical address
   Summary:
    Identifies the SPW ROUTER table physical address.

   Description:
    This data type identifies the SPW ROUTER table physical address.
*/
typedef enum
{
    SPW_ROUTER_PHYS_ADDR_DISABLE = 0,
    SPW_ROUTER_PHYS_ADDR_LINK_1 = SPW_ROUTER_LINK1_PORT,
    SPW_ROUTER_PHYS_ADDR_LINK_2 = SPW_ROUTER_LINK2_PORT,
    SPW_ROUTER_PHYS_ADDR_PKTRX = SPW_ROUTER_PKTRX_PORT,
    SPW_ROUTER_PHYS_ADDR_RMAP = SPW_ROUTER_RMAP_PORT,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_ROUTER_PHYS_ADDR_INVALID = 0xFFFFFFFF
} SPW_ROUTER_PHYS_ADDR;

// *****************************************************************************
/* SPW ROUTER status
   Summary:
    Identifies the SPW ROUTER current status

   Description:
    This data type identifies the SPW ROUTER status
*/
typedef enum
{
    SPW_ROUTER_STATUS_DEST_MASK = SPW_ROUTER_STS_DEST_Msk, 
    SPW_ROUTER_STATUS_SOURCE_MASK = SPW_ROUTER_STS_SOURCE_Msk,
    SPW_ROUTER_STATUS_BYTE_MASK = SPW_ROUTER_STS_BYTE_Msk,
    SPW_ROUTER_STATUS_COUNT_MASK = SPW_ROUTER_STS_COUNT_Msk,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_ROUTER_STATUS_INVALID = 0xFFFFFFFF
} SPW_ROUTER_STATUS;

// *****************************************************************************
/* SPW ROUTER Timeout status
   Summary:
    Identifies the SPW ROUTER Timeout current status

   Description:
    This data type identifies the SPW ROUTER Timeout status
*/
typedef enum
{
    SPW_ROUTER_TIMEOUT_STATUS_ADDR_MASK = SPW_ROUTER_TIMEOUT_ADDR_Msk, 
    SPW_ROUTER_TIMEOUT_STATUS_LOCKED = SPW_ROUTER_TIMEOUT_LOCKED_Msk,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_ROUTER_TIMEOUT_STATUS_INVALID = 0xFFFFFFFF
} SPW_ROUTER_TIMEOUT_STATUS;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*
 * The following functions make up the methods (set of possible operations) of
 * this interface.
 */

void SPW_ROUTER_Initialize(void);

void SPW_ROUTER_TimeoutDisable(bool disable);

void SPW_ROUTER_LogicalAddressRoutingEnable(bool enable);

void SPW_ROUTER_FallbackEnable(bool enable);

void SPW_ROUTER_RoutingTableEntrySet(uint8_t logicalAddress, bool delHeader, SPW_ROUTER_PHYS_ADDR physicalAddress);

SPW_ROUTER_STATUS SPW_ROUTER_StatusGet(void);

SPW_ROUTER_TIMEOUT_STATUS SPW_ROUTER_TimeoutStatusGet(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_SPW_ROUTER_H */
