/*******************************************************************************
  SPW Peripheral Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw_tch.c

  Summary:
    SPW PLIB Implementation file

  Description:
    This file defines the interface to the SPW peripheral library.
    This library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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
#include "plib_spw_tch.h"

// *****************************************************************************
// *****************************************************************************
// SPW PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void SPW_TCH_LinkListenerSet(SPW_TCH_SEL_LINK_MASK links)

   Summary:
    Set SPW TCH listener links.

   Precondition:
    None.

   Parameters:
    links - Bitfield value that indicates for each link if they are listener (1) or not

   Returns:
    None
*/
void SPW_TCH_LinkListenerSet(SPW_TCH_SEL_LINK_MASK links)
{
    uint32_t cfgListenReg = 0;
    if ( (links & SPW_TCH_SEL_LINK_MASK_L1) == SPW_TCH_SEL_LINK_MASK_L1)
        cfgListenReg |= SPW_TCH_CFGLISTEN_L1(1);
    if ( (links & SPW_TCH_SEL_LINK_MASK_L2) == SPW_TCH_SEL_LINK_MASK_L2)
        cfgListenReg |= SPW_TCH_CFGLISTEN_L2(1);
    SPW_REGS->SPW_TCH_CFGLISTEN = cfgListenReg;
}

// *****************************************************************************
/* Function:
    void SPW_TCH_LinkSenderSet(SPW_TCH_SEL_LINK_MASK links)

   Summary:
    Set SPW TCH sender links.

   Precondition:
    None.

   Parameters:
    links - Bitfield value that indicates for each link if they are sender (1) or not

   Returns:
    None
*/
void SPW_TCH_LinkSenderSet(SPW_TCH_SEL_LINK_MASK links)
{
    uint32_t cfgSendReg = 0;
    if ( (links & SPW_TCH_SEL_LINK_MASK_L1) == SPW_TCH_SEL_LINK_MASK_L1)
        cfgSendReg |= SPW_TCH_CFGSEND_S1(1);
    if ( (links & SPW_TCH_SEL_LINK_MASK_L2) == SPW_TCH_SEL_LINK_MASK_L2)
        cfgSendReg |= SPW_TCH_CFGSEND_S2(1);
    SPW_REGS->SPW_TCH_CFGSEND = cfgSendReg;
}

// *****************************************************************************
/* Function:
    void SPW_TCH_ConfigureEvent(SPW_SYNC_EVENT_MASK eventMask)

   Summary:
    Set the SPW TCH event source that drives the time codes.

   Precondition:
    None.

   Parameters:
    eventMask - selected event source that drives the time codes.

   Returns:
    None
*/
void SPW_TCH_ConfigureEvent(SPW_SYNC_EVENT_MASK eventMask)
{
    SPW_REGS->SPW_TCH_CFG = SPW_TCH_CFG_EVENT(eventMask);
}

// *****************************************************************************
/* Function:
    void SPW_TCH_ConfigureRestart(uint8_t timeCodeValue, bool oneshot, SPW_TCH_CFG_RESTART_IN inputEvent, SPW_SYNC_EVENT_MASK eventMask)

   Summary:
    Set SPW TCH restart configuration to set up once or regularly the time code value.

   Precondition:
    None.

   Parameters:
    timeCodeValue - Time code value set at restart event.
    oneshot - clears restart configuration register after restart event
    inputEvent - Select restart input source.
    eventMask - If input source is syncronization event, selected the event source.

   Returns:
    None
*/
void SPW_TCH_ConfigureRestart(uint8_t timeCodeValue, bool oneshot, SPW_TCH_CFG_RESTART_IN inputEvent, SPW_SYNC_EVENT_MASK eventMask)
{
    uint32_t cgfRestartReg = SPW_TCH_CFGRESTART_VALUE(timeCodeValue);
    if (oneshot == true)
        cgfRestartReg |= SPW_TCH_CFGRESTART_ONESHOT(1);
    if (inputEvent == SPW_TCH_CFG_RESTART_IN_PPS)
    {
        cgfRestartReg |= SPW_TCH_CFGRESTART_PPS(1);
    }
    else if (inputEvent == SPW_TCH_CFG_RESTART_IN_EVENT)
    {
        cgfRestartReg |= SPW_TCH_CFGRESTART_EVENT(eventMask);
    }
    SPW_REGS->SPW_TCH_CFGRESTART = cgfRestartReg;
}

// *****************************************************************************
/* Function:
    void SPW_TCH_ConfigureTcEvent(uint8_t timeCodeMask, uint8_t timeCodeValue)

   Summary:
    Set SPW TCH Time Code event configuration.

   Precondition:
    None.

   Parameters:
    timeCodeMask - Bits of new Time Code to check.
    timeCodeValue - Value used to check the new Time Code.

   Returns:
    None
*/
void SPW_TCH_ConfigureTcEvent(uint8_t timeCodeMask, uint8_t timeCodeValue)
{
    SPW_REGS->SPW_TCH_CFGTCEVENT = SPW_TCH_CFGTCEVENT_MASK(timeCodeMask) | SPW_TCH_CFGTCEVENT_VALUE(timeCodeValue);
}

// *****************************************************************************
/* Function:
    void SPW_TCH_ConfigureWatchdog(uint16_t earlyNumTick, uint16_t lateNumTick)

   Summary:
    Set SPW TCH watchdog configuration for early and late watchdog interrupts.

   Precondition:
    None.

   Parameters:
    earlyNumTick - Trigger watchdog if new Time Code before this number of tick (TimeTick clock).
    lateNumTick - Trigger watchdog if No new Time Code is received before this number of tick (TimeTick clock).

   Returns:
    None
*/
void SPW_TCH_ConfigureWatchdog(uint16_t earlyNumTick, uint16_t lateNumTick)
{
    SPW_REGS->SPW_TCH_CFGWD = SPW_TCH_CFGWD_EARLY(earlyNumTick) | SPW_TCH_CFGWD_LATE(lateNumTick);
}

// *****************************************************************************
/* Function:
    void SPW_TCH_LastTimeCodeSet(uint8_t timeCode, bool now)

   Summary:
    Set the SPW TCH Time Code Value. If send now condition is set, send the written
    time code. Otherwise send the written time code incremented by one at next event.

   Precondition:
    None.

   Parameters:
    timeCode - Next Time Code N-1 to be distributed on next event.
    now - Distribute the written time code now.

   Returns:
    None
*/
void SPW_TCH_LastTimeCodeSet(uint8_t timeCode, bool now)
{
    uint32_t lastTimeCodeReg = SPW_TCH_LASTTIMECODE_VALUE(timeCode);
    if (now == true)
        lastTimeCodeReg |= SPW_TCH_LASTTIMECODE_SEND(1);
    SPW_REGS->SPW_TCH_LASTTIMECODE = lastTimeCodeReg;
}

// *****************************************************************************
/* Function:
    uint8_t SPW_TCH_LastTimeCodeGet(void)

   Summary:
    Get last SPW TCH Time Code distributed

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Last Time Code distributed.
*/
uint8_t SPW_TCH_LastTimeCodeGet(void)
{
    return (uint8_t)( (SPW_REGS->SPW_TCH_LASTTIMECODE & SPW_TCH_LASTTIMECODE_VALUE_Msk) >> SPW_TCH_LASTTIMECODE_VALUE_Pos );
}

// *****************************************************************************
/* Function:
    SPW_TCH_INT_MASK SPW_TCH_IrqStatusGetMaskedAndClear(void)

   Summary:
    Get the SPW TCH interrupt status for masked interrupts and clear them.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Current SPW TCH interrupt status.
*/
SPW_TCH_INT_MASK SPW_TCH_IrqStatusGetMaskedAndClear(void)
{
    uint32_t pendingMaskedIrq = SPW_REGS->SPW_TCH_PI_RM;
    SPW_REGS->SPW_TCH_PI_C = pendingMaskedIrq;
    return (SPW_TCH_INT_MASK)(pendingMaskedIrq);
}

// *****************************************************************************
/* Function:
    void SPW_TCH_InterruptEnable(SPW_TCH_INT_MASK interruptMask)

   Summary:
    Enable SPW TCH interrupts.

   Precondition:
    None.

   Parameters:
    interruptMask - Interrupts to be enabled.

   Returns:
    None
*/
void SPW_TCH_InterruptEnable(SPW_TCH_INT_MASK interruptMask)
{
    SPW_REGS->SPW_TCH_IM_S = (uint32_t)interruptMask;
}

// *****************************************************************************
/* Function:
    void SPW_TCH_InterruptDisable(SPW_TCH_INT_MASK interruptMask)

   Summary:
    Disable SPW TCH interrupts.

   Precondition:
    None.

   Parameters:
    interruptMask - Interrupts to be disabled.

   Returns:
    None
*/
void SPW_TCH_InterruptDisable(SPW_TCH_INT_MASK interruptMask)
{
    SPW_REGS->SPW_TCH_IM_C = (uint32_t)interruptMask;
}
