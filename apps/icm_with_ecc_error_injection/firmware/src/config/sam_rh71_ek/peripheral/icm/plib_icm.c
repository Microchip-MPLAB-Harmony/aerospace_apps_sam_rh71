/*******************************************************************************
  ICM Peripheral Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_icm.c

  Summary:
    ICM PLIB Implementation file

  Description:
    This file defines the interface to the ICM peripheral library.
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

#include <stddef.h>
#include "device.h"
#include "interrupts.h"
#include "plib_icm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

static ICM_OBJ icmObj;

/* MISRA C-2012 Rule 7.2 is deviated in the below code block. Deviation record ID - H3_MISRAC_2012_R_7_2_DR_1*/

/* ICM memory region descriptor object */
static const icm_descriptor_registers_t __attribute__((aligned (64))) icmListDescriptor[] =
{
    {
        .ICM_RADDR = ICM_RADDR_RADDR(0x10000),
        .ICM_RCFG = ICM_RCFG_ALGO(0) |
                    ICM_RCFG_PROCDLY(0) |
                    ICM_RCFG_SUIEN(0) |
                    ICM_RCFG_ECIEN(0) |
                    ICM_RCFG_WCIEN(0) |
                    ICM_RCFG_BEIEN(0) |
                    ICM_RCFG_DMIEN(0) |
                    ICM_RCFG_RHIEN(0) |
                    ICM_RCFG_EOM(0) |
                    ICM_RCFG_WRAP(0) |
                    ICM_RCFG_CDWBN(0),
        .ICM_RCTRL = ICM_RCTRL_TRSIZE(156),
        .ICM_RNEXT = 0,
    },
    {
        .ICM_RADDR = ICM_RADDR_RADDR(0x20000000),
        .ICM_RCFG = ICM_RCFG_ALGO(0) |
                    ICM_RCFG_PROCDLY(0) |
                    ICM_RCFG_SUIEN(0) |
                    ICM_RCFG_ECIEN(0) |
                    ICM_RCFG_WCIEN(0) |
                    ICM_RCFG_BEIEN(0) |
                    ICM_RCFG_DMIEN(0) |
                    ICM_RCFG_RHIEN(0) |
                    ICM_RCFG_EOM(0) |
                    ICM_RCFG_WRAP(0) |
                    ICM_RCFG_CDWBN(0),
        .ICM_RCTRL = ICM_RCTRL_TRSIZE(156),
        .ICM_RNEXT = 0,
    },
    {
        .ICM_RADDR = ICM_RADDR_RADDR(0x210b0000),
        .ICM_RCFG = ICM_RCFG_ALGO(0) |
                    ICM_RCFG_PROCDLY(0) |
                    ICM_RCFG_SUIEN(0) |
                    ICM_RCFG_ECIEN(0) |
                    ICM_RCFG_WCIEN(0) |
                    ICM_RCFG_BEIEN(0) |
                    ICM_RCFG_DMIEN(0) |
                    ICM_RCFG_RHIEN(0) |
                    ICM_RCFG_EOM(0) |
                    ICM_RCFG_WRAP(0) |
                    ICM_RCFG_CDWBN(0),
        .ICM_RCTRL = ICM_RCTRL_TRSIZE(156),
        .ICM_RNEXT = 0,
    },
    {
        .ICM_RADDR = ICM_RADDR_RADDR(0x64000000),
        .ICM_RCFG = ICM_RCFG_ALGO(0) |
                    ICM_RCFG_PROCDLY(0) |
                    ICM_RCFG_SUIEN(0) |
                    ICM_RCFG_ECIEN(0) |
                    ICM_RCFG_WCIEN(0) |
                    ICM_RCFG_BEIEN(0) |
                    ICM_RCFG_DMIEN(0) |
                    ICM_RCFG_RHIEN(0) |
                    ICM_RCFG_EOM(0) |
                    ICM_RCFG_WRAP(1) |
                    ICM_RCFG_CDWBN(0),
        .ICM_RCTRL = ICM_RCTRL_TRSIZE(156),
        .ICM_RNEXT = 0,
    },
};

/* MISRAC 2012 deviation block end */

// *****************************************************************************
// *****************************************************************************
// ICM PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/* Function:
    void ICM_Initialize(void)

   Summary:
    Initializes given instance of the ICM peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void ICM_Initialize(void)
{
    //Reset the ICM
    ICM_SoftwareReset();

    // Set ICM configuration
    ICM_REGS->ICM_CFG = ICM_CFG_BBC(0) |
                        ICM_CFG_SLBDIS(1);

    // Set Region list descriptor address.
    ICM_REGS->ICM_DSCR = (uint32_t)&icmListDescriptor;
}

// *****************************************************************************
/* Function:
    void ICM_SetEndOfMonitoringDisable(bool disable)

   Summary:
    Disables/enables the ICM End of Monitoring Configuration

   Description:
    None

   Precondition:
    None.

   Parameters:
    disable - if true, End of Monitoring is disabled. Enabled if false.

   Returns:
    None

   Remarks:
    None
*/
void ICM_SetEndOfMonitoringDisable(bool disable)
{
    uint32_t cfgReg = ( ICM_REGS->ICM_CFG & ~ICM_CFG_EOMDIS_Msk );
    if (disable == true)
    {
        cfgReg |= ICM_CFG_EOMDIS(1);
    }
    ICM_REGS->ICM_CFG = cfgReg;
}

// *****************************************************************************
/* Function:
    void ICM_WriteBackDisable(bool disable)

   Summary:
    Disables/enables the ICM Write Back Configuration

   Description:
    None

   Precondition:
    None.

   Parameters:
    disable - if true, Write Back is disabled. Enabled if false.

   Returns:
    None

   Remarks:
    None
*/
void ICM_WriteBackDisable(bool disable)
{
    uint32_t cfgReg = ( ICM_REGS->ICM_CFG & ~ICM_CFG_WBDIS_Msk );
    if (disable == true)
    {
        cfgReg |= ICM_CFG_WBDIS(1);
    }
    ICM_REGS->ICM_CFG = cfgReg;
}

// *****************************************************************************
/* Function:
    void ICM_SoftwareReset(void)

   Summary:
    Reset the ICM Peripheral.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void ICM_SoftwareReset(void)
{
    ICM_REGS->ICM_CTRL = ICM_CTRL_SWRST(1);
}

// *****************************************************************************
/* Function:
    void ICMICM_SetDescStartAddress(icm_descriptor_registers_t* addr)

   Summary:
    Set the ICM descriptor area start address register.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    addr - The address of the ICM list descriptor.

   Returns:
    None.
    
   Remarks: 
    The start address must be aligned with size of the data structure (64 bytes).
*/
void ICM_SetDescStartAddress(icm_descriptor_registers_t* addr)
{
    ICM_REGS->ICM_DSCR = (uint32_t)addr;
}

// *****************************************************************************
/* Function:
    void ICMICM_SetHashStartAddress(void)

   Summary:
    Set the ICM hash area start address register.

   Description:
    The ICM Hash Area should be a contiguous area of system memory that the controller
    and the processor can access. This address is a multiple of 128 bytes.
    For each region, 32 bytes are used to store the computed hash.

   Precondition:
    None.

   Parameters:
    addr - The address of the ICM Hash memory location.

   Returns:
    None.
    
   Remarks: 
    This field points at the Hash memory location. The address must be aligned to 128 bytes.
*/
void ICM_SetHashStartAddress(uint32_t addr)
{
    ICM_REGS->ICM_HASH = addr;
}

// *****************************************************************************
/* Function:
    void ICM_Enable(void)

   Summary:
    Enable the ICM Peripheral.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void ICM_Enable(void)
{
    ICM_REGS->ICM_CTRL = ICM_CTRL_ENABLE(1);
}

// *****************************************************************************
/* Function:
    void ICM_Disable(void)

   Summary:
    Disable the ICM Peripheral.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void ICM_Disable(void)
{
    ICM_REGS->ICM_CTRL = ICM_CTRL_DISABLE(1);
}

// *****************************************************************************
/* Function:
    void ICM_ReComputeHash(ICM_REGION_MASK regions)

   Summary:
    Re-compute the ICM hash area start for the given regions.

   Description:
    None.

   Precondition:
    Region monitoring should be disabled.

   Parameters:
    region - The regions for which the digest will be re-computed.

   Returns:
    None.
*/
void ICM_ReComputeHash(ICM_REGION_MASK regions)
{
    ICM_REGS->ICM_CTRL = ICM_CTRL_REHASH(regions);
}

// *****************************************************************************
/* Function:
    void ICM_MonitorEnable(ICM_REGION_MASK regions)

   Summary:
    Enable the ICM monitoring of the given regions.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    region - The regions for which the monitoring will be enabled.

   Returns:
    None.
*/
void ICM_MonitorEnable(ICM_REGION_MASK regions)
{
    ICM_REGS->ICM_CTRL = ICM_CTRL_RMEN(regions);
}

// *****************************************************************************
/* Function:
    void ICM_MonitorDisable(ICM_REGION_MASK regions)

   Summary:
    Disable the ICM monitoring of the given regions.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    region - The regions for which the monitoring will be disabled.

   Returns:
    None.
*/
void ICM_MonitorDisable(ICM_REGION_MASK regions)
{
    ICM_REGS->ICM_CTRL = ICM_CTRL_RMDIS(regions);
}

// *****************************************************************************
/* Function:
    ICM_INT_MSK ICM_InterruptGet(void)

   Summary:
    Returns the Interrupt status.

   Precondition:
    ICM_Initialize must have been called for the associated ICM instance.

   Parameters:
    None.

   Returns:
    Interrupts that are active.
*/
ICM_INT_MSK ICM_InterruptGet(void)
{
    return (ICM_INT_MSK)(ICM_REGS->ICM_ISR);
}

// *****************************************************************************
/* Function:
    ICM_INT_MSK ICM_InterruptMasked(void)

   Summary:
    Returns the current masked Interrupts.

   Precondition:
    ICM_Initialize must have been called for the associated ICM instance.

   Parameters:
    None.

   Returns:
    Interrupts that are currently masked.
*/
ICM_INT_MSK ICM_InterruptMasked(void)
{
    return (ICM_INT_MSK)(ICM_REGS->ICM_IMR);
}

// *****************************************************************************
/* Function:
    void ICM_InterruptEnable(ICM_INT_MSK interruptMask)

   Summary:
    Enables ICM Interrupt.

   Precondition:
    ICM_Initialize must have been called for the associated ICM instance.

   Parameters:
    interruptMask - Interrupt to be enabled

   Returns:
    None
*/
void ICM_InterruptEnable(ICM_INT_MSK interruptMask)
{
    ICM_REGS->ICM_IER = (uint32_t)(interruptMask);
}

// *****************************************************************************
/* Function:
    void ICM_InterruptDisable(ICM_INT_MSK interruptMask)

   Summary:
    Disables ICM Interrupt.

   Precondition:
    ICM_Initialize must have been called for the associated ICM instance.

   Parameters:
    interruptMask - Interrupt to be disabled

   Returns:
    None
*/
void ICM_InterruptDisable(ICM_INT_MSK interruptMask)
{
    ICM_REGS->ICM_IDR = (uint32_t)(interruptMask);
}

// *****************************************************************************
/* Function:
    ICM_STATUS ICM_StatusGet(void)

   Summary:
    Get the ICM Peripheral Status.

   Description:
    None.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    The current ICM status.
*/
ICM_STATUS ICM_StatusGet(void)
{
    return (ICM_STATUS)(ICM_REGS->ICM_SR);
}

// *****************************************************************************
/* Function:
    void ICM_CallbackRegister(ICM_CALLBACK callback,
                                                              uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given ICM's interrupt events occur.

  Description:
    This function sets the pointer to a client function to be called "back" when
    the given ICM's interrupt events occur. It also passes a context value
    (usually a pointer to a context structure) that is passed into the function
    when it is called. The specified callback function will be called from the
    peripheral interrupt context.

  Precondition:
    ICM_Initialize must have been called for the associated
    ICM instance.

  Parameters:
    callback      - A pointer to a function with a calling signature defined by
                    the ICM_CALLBACK data type. Setting this to NULL
                    disables the callback feature.

    contextHandle - A value (usually a pointer) passed (unused) into the
                    function identified by the callback parameter.

  Returns:
    None.

  Remarks:
    None.
*/
void ICM_CallbackRegister(ICM_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    icmObj.callback = callback;
    icmObj.context = contextHandle;
}

// *****************************************************************************
/* Function:
    void ICM_InterruptHandler(void)

   Summary:
    ICM Peripheral Interrupt Handler.

   Description:
    This function is ICM Peripheral Interrupt Handler and will
    called on every ICM interrupt.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    The function is called as peripheral instance's interrupt handler if the
    instance interrupt is enabled. If peripheral instance's interrupt is not
    enabled user need to call it from the main while loop of the application.
*/
void __attribute__((used)) ICM_InterruptHandler(void)
{

    if (icmObj.callback != NULL)
    {
        icmObj.callback(icmObj.context);
    }
}
