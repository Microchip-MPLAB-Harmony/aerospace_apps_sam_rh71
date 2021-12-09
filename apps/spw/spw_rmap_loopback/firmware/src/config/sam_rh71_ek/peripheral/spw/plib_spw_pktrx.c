/*******************************************************************************
  SPW Peripheral Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw_pktrx.c

  Summary:
    SPW PLIB Implementation file for packet RX module.

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
#include "plib_spw_pktrx.h"

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
    void SPW_PKTRX_SetDiscard(bool discard)

   Summary:
    Set the SPW packet RX inactive incoming mode to discard or stall option.

   Precondition:
    None.

   Parameters:
    discard - True for discard mode, false for stall mode.

   Returns:
    None.

    Note:
     The discard mode is not working with SAMRH71F20C device.
     It will always stall by defaut, see ErrataSheet.
*/
void SPW_PKTRX_SetDiscard(bool discard)
{
    if (discard == true)
        SPW_REGS->SPW_PKTRX1_CFG = SPW_PKTRX1_CFG_DISCARD(1);
    else
        SPW_REGS->SPW_PKTRX1_CFG = 0;
}

// *****************************************************************************
/* Function:
    void SPW_PKTRX_SetNextBuffer(
        uint8_t* dataAddress,
        uint32_t dataLengthBytes,
        SPW_PKTRX_INFO* packetInfoAddress,
        uint16_t pckCount,
        bool split,
        SPW_PKTRX_NXTBUF_START startMode,
        uint8_t startValue)

   Summary:
    Set the SPW next receive buffer informations and start conditions.

   Precondition:
    None.

   Parameters:
    dataAddress - pointer to data buffer address.
    dataLengthBytes - length in byte of the data buffer address.
    packetInfoAddress - pointer to the packet info type address were packet information will be stored.
    pckCount - maximum number of entries in packet info buffer.
    split - set to True if any ongoing packet should be split when the start condition is met.
    startMode - receive buffer start mode condition.
    startValue - Matching value for event start condition.

   Returns:
    None.
*/
void SPW_PKTRX_SetNextBuffer(
    uint8_t* dataAddress,
    uint32_t dataLengthBytes,
    SPW_PKTRX_INFO* packetInfoAddress,
    uint16_t pckCount,
    bool split,
    SPW_PKTRX_NXTBUF_START startMode,
    uint8_t startValue)
{
    SPW_REGS->SPW_PKTRX1_NXTBUFDATAADDR = (uint32_t)(dataAddress);
    SPW_REGS->SPW_PKTRX1_NXTBUFDATALEN = dataLengthBytes;
    SPW_REGS->SPW_PKTRX1_NXTBUFPKTADDR = (uint32_t)(packetInfoAddress);
    SPW_REGS->SPW_PKTRX1_NXTBUFCFG = (SPW_PKTRX1_NXTBUFCFG_MAXCNT(pckCount)  | \
                                      SPW_PKTRX1_NXTBUFCFG_VALUE(startValue) | \
                                      SPW_PKTRX1_NXTBUFCFG_START(startMode)  | \
                                      SPW_PKTRX1_NXTBUFCFG_SPLIT(split) );
}

// *****************************************************************************
/* Function:
    SPW_PKTRX_STATUS SPW_PKTRX_StatusGet(void)

   Summary:
    Get the SPW packet RX module status.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Current status of SPW packet RX module.
*/
SPW_PKTRX_STATUS SPW_PKTRX_StatusGet(void)
{
    return (SPW_PKTRX_STATUS)(SPW_REGS->SPW_PKTRX1_STATUS);
}

// *****************************************************************************
/* Function:
    SPW_PKTRX_INT_MASK SPW_PKTRX_IrqStatusGetMaskedAndClear(void);

   Summary:
    Get the SPW packet RX module interrupt status for masked interrupts and clear them.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Current interrupt status of SPW packet RX module.
*/
SPW_PKTRX_INT_MASK SPW_PKTRX_IrqStatusGetMaskedAndClear(void)
{
    uint32_t pendingMaskedIrq = SPW_REGS->SPW_PKTRX1_PI_RM;
    SPW_REGS->SPW_PKTRX1_PI_C = pendingMaskedIrq;
    return (SPW_PKTRX_INT_MASK)(pendingMaskedIrq);
}

// *****************************************************************************
/* Function:
    void SPW_PKTRX_InterruptEnable(SPW_PKTRX_INT_MASK interruptMask)

   Summary:
    Enables SPW RX module Interrupt.

   Precondition:
    SPW_Initialize must have been called for the associated SPW instance.

   Parameters:
    interruptMask - Interrupt to be enabled

   Returns:
    None
*/
void SPW_PKTRX_InterruptEnable(SPW_PKTRX_INT_MASK interruptMask)
{
    SPW_REGS->SPW_PKTRX1_IM_S = (uint32_t)interruptMask;
}

// *****************************************************************************
/* Function:
    void SPW_PKTRX_InterruptDisable(SPW_PKTRX_INT_MASK interruptMask)

   Summary:
    Disables SPW RX module Interrupt.

   Precondition:
    SPW_Initialize must have been called for the associated SPW instance.

   Parameters:
    interruptMask - Interrupt to be disabled

   Returns:
    None
*/
void SPW_PKTRX_InterruptDisable(SPW_PKTRX_INT_MASK interruptMask)
{
    SPW_REGS->SPW_PKTRX1_IM_C = (uint32_t)interruptMask;
}


// *****************************************************************************
/* Function:
    void SPW_PKTRX_CurrentPacketAbort(void);

   Summary:
    Immediately abort and discard any ongoing SPW packet in RX module.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void SPW_PKTRX_CurrentPacketAbort(void)
{
    SPW_REGS->SPW_PKTRX1_CURBUFCFG = SPW_PKTRX1_CURBUFCFG_ABORT_Msk;
}

// *****************************************************************************
/* Function:
    void SPW_PKTRX_CurrentPacketSplit(void);

   Summary:
    Immediately split any ongoing SPW packet in RX module.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void SPW_PKTRX_CurrentPacketSplit(void)
{
    SPW_REGS->SPW_PKTRX1_CURBUFCFG = SPW_PKTRX1_CURBUFCFG_SPLIT_Msk;
}

// *****************************************************************************
/* Function:
    SPW_PKTRX_PREV_STATUS SPW_PKTRX_GetPreviousBufferStatus(void)

   Summary:
    Get the previous buffer status of SPW packet RX module.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Current value of previous buffer status of SPW packet RX module.
*/
SPW_PKTRX_PREV_STATUS SPW_PKTRX_GetPreviousBufferStatus(void)
{
    return (SPW_PKTRX_PREV_STATUS)(SPW_REGS->SPW_PKTRX1_PREVBUFSTS);
}

