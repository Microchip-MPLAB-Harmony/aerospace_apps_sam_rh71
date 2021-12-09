/*******************************************************************************
  SPW Peripheral Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw_pkttx.c

  Summary:
    SPW PLIB Implementation file for packet TX module.

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
#include "plib_spw_pkttx.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// SPW PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
/* Function:
    SPW_PKTTX_STATUS SPW_PKTTX_StatusGet(void)

   Summary:
    Get the SPW packet TX module status.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Current status of SPW packet TX module.
*/
SPW_PKTTX_STATUS SPW_PKTTX_StatusGet(void)
{
    return (SPW_PKTTX_STATUS)(SPW_REGS->SPW_PKTTX1_STATUS);
}

// *****************************************************************************
/* Function:
    SPW_PKTTX_INT_MASK SPW_PKTTX_IrqStatusGetMaskedAndClear(void);

   Summary:
    Get the SPW packet TX module interrupt status for masked interrupts and clear them.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Current interrupt status of SPW packet TX module.
*/
SPW_PKTTX_INT_MASK SPW_PKTTX_IrqStatusGetMaskedAndClear(void)
{
    uint32_t pendingMaskedIrq = SPW_REGS->SPW_PKTTX1_PI_RM;
    SPW_REGS->SPW_PKTTX1_PI_C = pendingMaskedIrq;
    return (SPW_PKTTX_INT_MASK)(pendingMaskedIrq);
}

// *****************************************************************************
/* Function:
    void SPW_PKTTX_InterruptEnable(SPW_PKTTX_INT_MASK interruptMask)

   Summary:
    Enables SPW TX module Interrupt.

   Precondition:
    SPW_Initialize must have been called for the associated SPW instance.

   Parameters:
    interruptMask - Interrupt to be enabled

   Returns:
    None
*/
void SPW_PKTTX_InterruptEnable(SPW_PKTTX_INT_MASK interruptMask)
{
    SPW_REGS->SPW_PKTTX1_IM_S = (uint32_t)interruptMask;
}

// *****************************************************************************
/* Function:
    void SPW_PKTTX_InterruptDisable(SPW_PKTTX_INT_MASK interruptMask)

   Summary:
    Disables SPW TX module Interrupt.

   Precondition:
    SPW_Initialize must have been called for the associated SPW instance.

   Parameters:
    interruptMask - Interrupt to be disabled

   Returns:
    None
*/
void SPW_PKTTX_InterruptDisable(SPW_PKTTX_INT_MASK interruptMask)
{
    SPW_REGS->SPW_PKTTX1_IM_C = (uint32_t)interruptMask;
}
// *****************************************************************************
/* Function:
    void SPW_PKTTX_SetNextSendList(uint8_t* routerBytesTable,
                                   SPW_PKTTX_SEND_LIST_ENTRY* sendListAddress,
                                   uint16_t length,
                                   bool abort,
                                   SPW_PKTTX_NXTSEND_START startMode,
                                   uint8_t startValue)

   Summary:
    Set the SPW next send list informations and start conditions.

   Precondition:
    None.

   Parameters:
    routerBytesTable - table of 4 router bytes to prepend (if non-zero) to each
                       packets in the send list. All bytes are set to 0 if this pointer is NULL.
    sendListAddress - pointer to the send list type object that will be transmitted.
    length - number of entries in send list.
    abort - set to True if any ongoing send list should be abort when this send list wants to start.
    startMode - send list start mode condition.
    startValue - Matching value for event start condition.

   Returns:
    None.
*/
void SPW_PKTTX_SetNextSendList(uint8_t* routerBytesTable,
                               SPW_PKTTX_SEND_LIST_ENTRY* sendListAddress,
                               uint16_t length,
                               bool abort,
                               SPW_PKTTX_NXTSEND_START startMode,
                               uint8_t startValue)
{
    if ( routerBytesTable == NULL )
        SPW_REGS->SPW_PKTTX1_NXTSENDROUT = 0;
    else
        SPW_REGS->SPW_PKTTX1_NXTSENDROUT = (routerBytesTable[0] << 24 ) | \
                                           (routerBytesTable[1] << 16 ) | \
                                           (routerBytesTable[2] << 8 ) | \
                                           (routerBytesTable[3]);
    SPW_REGS->SPW_PKTTX1_NXTSENDADDR = (uint32_t)sendListAddress;
    SPW_REGS->SPW_PKTTX1_NXTSENDCFG = SPW_PKTTX1_NXTSENDCFG_ABORT(abort) | \
                                      SPW_PKTTX1_NXTSENDCFG_START(startMode)  | \
                                      SPW_PKTTX1_NXTSENDCFG_VALUE(startValue)  | \
                                      SPW_PKTTX1_NXTSENDCFG_LEN(length);
}

// *****************************************************************************
/* Function:
    void SPW_PKTTX_UnlockStatus(void)

   Summary:
    Unlock the SPW TX module previous buffer status.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void SPW_PKTTX_UnlockStatus(void)
{
    /* Perform dummy write to status to unlock previous buffer status */
    SPW_REGS->SPW_PKTTX1_STATUS = (0x01);
}
