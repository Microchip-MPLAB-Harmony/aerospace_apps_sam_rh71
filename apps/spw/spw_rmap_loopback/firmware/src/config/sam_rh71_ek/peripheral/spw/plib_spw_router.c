/*******************************************************************************
  SPW Peripheral Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw_router.c

  Summary:
    SPW PLIB ROUTER Implementation file

  Description:
    This file defines the interface to the SPW peripheral library.
    This library provides access to and control of the associated peripheral
    instance.

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_spw_router.h"

// *****************************************************************************
// *****************************************************************************
// SPW PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/* Function:
    void SPW_ROUTER_Initialize(void)

   Summary:
    Initialize ROUTER modules of the SPW peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void SPW_ROUTER_Initialize(void)
{
    /* Set router configuration */
    SPW_ROUTER_TimeoutDisable(false);
    SPW_ROUTER_LogicalAddressRoutingEnable(true);
    SPW_ROUTER_FallbackEnable(false);

    /* Initialize all entried in logical address routing table to zero */
    for (uint8_t entry = 0; entry < 224; entry++)
    {
        SPW_REGS->SPW_ROUTER_TABLE[entry] = 0;
    }

    /* Configure logical addresses 70 in routing table */
    SPW_ROUTER_RoutingTableEntrySet(70, false, 9);
}

// *****************************************************************************
/* Function:
    void SPW_ROUTER_TimeoutDisable(bool disable)

   Summary:
    Disable SPW ROUTER timeout event.

   Precondition:
    None.

   Parameters:
    disable - if true, Router timeout event is disabled. Enabled if false.

   Returns:
    None
*/
void SPW_ROUTER_TimeoutDisable(bool disable)
{
    uint32_t cfgReg = ( SPW_REGS->SPW_ROUTER_CFG & ~SPW_ROUTER_CFG_DISTIMEOUT_Msk );
    if (disable == true)
        cfgReg |= SPW_ROUTER_CFG_DISTIMEOUT(1);
    SPW_REGS->SPW_ROUTER_CFG = cfgReg;
}

// *****************************************************************************
/* Function:
    void SPW_ROUTER_LogicalAddressRoutingEnable(bool enable)

   Summary:
    Enable SPW ROUTER Logical Address Routing.

   Precondition:
    None.

   Parameters:
    enable - if true, Router Logical Address Routing is enable. Disabled if false.

   Returns:
    None
*/
void SPW_ROUTER_LogicalAddressRoutingEnable(bool enable)
{
    uint32_t cfgReg = ( SPW_REGS->SPW_ROUTER_CFG & ~SPW_ROUTER_CFG_LAENA_Msk );
    if (enable == true)
        cfgReg |= SPW_ROUTER_CFG_LAENA(1);
    SPW_REGS->SPW_ROUTER_CFG = cfgReg;
}

// *****************************************************************************
/* Function:
    void SPW_ROUTER_FallbackEnable(bool enable)

   Summary:
    Enable SPW ROUTER Fallback.

   Precondition:
    None.

   Parameters:
    enable - if true, Router Fallback is enable. Disabled if false.

   Returns:
    None
*/
void SPW_ROUTER_FallbackEnable(bool enable)
{
    uint32_t cfgReg = ( SPW_REGS->SPW_ROUTER_CFG & ~SPW_ROUTER_CFG_FALLBACK_Msk );
    if (enable == true)
        cfgReg |= SPW_ROUTER_CFG_FALLBACK(1);
    SPW_REGS->SPW_ROUTER_CFG = cfgReg;
}

// *****************************************************************************
/* Function:
    void SPW_ROUTER_RoutingTableEntrySet(uint8_t logicalAddress, bool delHeader, uint8_t physicalAddress)

   Summary:
    Set a SPW routing table entry for a logical address.

   Precondition:
    None.

   Parameters:
    logicalAddress - Logical address entry to configure between 32 to 255.
    delHeader - if true, discard router byte for this logical address.
    physicalAddress - The physical address were incoming packet will be routed.

   Returns:
    None
*/
void SPW_ROUTER_RoutingTableEntrySet(uint8_t logicalAddress, bool delHeader, SPW_ROUTER_PHYS_ADDR physicalAddress)
{
    if ( logicalAddress >= 32 )
    {
        uint32_t entry = SPW_ROUTER_TABLE_ADDR(physicalAddress);
        if (delHeader == true)
            entry |= SPW_ROUTER_TABLE_DELHEAD(1);
        SPW_REGS->SPW_ROUTER_TABLE[logicalAddress-32] = entry; 
    }
}

// *****************************************************************************
/* Function:
    SPW_ROUTER_STATUS SPW_ROUTER_StatusGet(void)

   Summary:
    Get SPW ROUTER Status.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    SPW ROUTER Status
*/
SPW_ROUTER_STATUS SPW_ROUTER_StatusGet(void)
{
    return (SPW_ROUTER_STATUS)(SPW_REGS->SPW_ROUTER_STS);
}

// *****************************************************************************
/* Function:
    SPW_ROUTER_TIMEOUT_STATUS SPW_ROUTER_TimeoutStatusGet(void)

   Summary:
    Get SPW ROUTER Timeout Status.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    SPW ROUTER Timeout Status
*/
SPW_ROUTER_TIMEOUT_STATUS SPW_ROUTER_TimeoutStatusGet(void)
{
    return (SPW_ROUTER_TIMEOUT_STATUS)(SPW_REGS->SPW_ROUTER_TIMEOUT);
}
