/*******************************************************************************
  TCMECC Peripheral Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_tcmecc.c

  Summary:
    TCMECC PLIB Implementation file

  Description:
    This file defines the interface to the TCMECC peripheral library.
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
#include "plib_tcmecc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

static TCMECC_OBJ tcmeccObj;

// *****************************************************************************
// *****************************************************************************
// TCMECC PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/* Function:
    void TCMECC_Initialize(void)

   Summary:
    Initializes given instance of the TCMECC peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void TCMECC_Initialize(void)
{
    TCMECC_REGS->TCMECC_CR = TCMECC_CR_ENABLE(1);

    TCMECC_REGS->TCMECC_IER = TCMECC_IER_MEM_FIX_I_Msk | TCMECC_IER_MEM_NOFIX_I_Msk | \
                              TCMECC_IER_MEM_FIX_D_Msk | TCMECC_IER_MEM_NOFIX_D_Msk;
}

// *****************************************************************************
/* Function:
    TCMECC_STATUS TCMECC_StatusGet(void)

   Summary:
    Get the status of the TCMECC peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Current status of the TCMECC peripheral.
*/
TCMECC_STATUS TCMECC_StatusGet(void)
{
    return (TCMECC_STATUS)(TCMECC_REGS->TCMECC_SR);
}

// *****************************************************************************
/* Function:
    uint64_t* TCMECC_GetFailAddressITCM(void)

   Summary:
    Get the last fail address were ECC error occurs in Instruction TCM.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Pointer of fail address were fixable or unfixable error occurred in ITCM.
*/
uint64_t* TCMECC_GetFailAddressITCM(void)
{
    return (uint64_t*)((TCMECC_REGS->TCMECC_FAILAR)  & (ITCM_ADDR + ITCM_SIZE - 1));
}

// *****************************************************************************
/* Function:
    uint32_t* TCMECC_GetFailAddressDTCM(void)

   Summary:
    Get the last fail address were ECC error occurs in Data TCM.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Pointer of fail address were fixable or unfixable error occurred in DTCM.
*/
uint32_t* TCMECC_GetFailAddressDTCM(void)
{
    return (uint32_t*)((TCMECC_REGS->TCMECC_FAILARD) & (DTCM_ADDR + DTCM_SIZE - 1));
}

// *****************************************************************************
/* Function:
    void TCMECC_ResetCounters(void)

   Summary:
    Reset Fix and NoFix error counters of the TCMECC peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void TCMECC_ResetCounters(void)
{
    TCMECC_REGS->TCMECC_CR |= (TCMECC_CR_RST_FIX_CPT_Msk | TCMECC_CR_RST_NOFIX_CPT_Msk);
}

// *****************************************************************************
/* Function:
    void TCMECC_FixCallbackRegister(TCMECC_CALLBACK callback, uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given TCMECC's Fixable interrupt events occur.

  Description:
    This function sets the pointer to a client function to be called "back" when
    the given TCMECC's interrupt events occur. It also passes a context value
    (usually a pointer to a context structure) that is passed into the function
    when it is called. The specified callback function will be called from the
    peripheral interrupt context.

  Precondition:
    TCMECC_Initialize must have been called for the associated
    TCMECC instance.

  Parameters:
    callback      - A pointer to a function with a calling signature defined by
                    the TCMECC_CALLBACK data type. Setting this to NULL
                    disables the callback feature.

    contextHandle - A value (usually a pointer) passed (unused) into the
                    function identified by the callback parameter.

  Returns:
    None.

  Example:
    <code>
        // Refer to the description of the TCMECC_CALLBACK data type for
        // example usage.
    </code>

  Remarks:
    None.
*/
void TCMECC_FixCallbackRegister(TCMECC_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    tcmeccObj.fix_callback = callback;
    tcmeccObj.fix_context = contextHandle;
}

// *****************************************************************************
/* Function:
    void TCMECC_NoFixCallbackRegister(TCMECC_CALLBACK callback,
                                                              uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given TCMECC's UnFixable interrupt events occur.

  Description:
    This function sets the pointer to a client function to be called "back" when
    the given TCMECC's interrupt events occur. It also passes a context value
    (usually a pointer to a context structure) that is passed into the function
    when it is called. The specified callback function will be called from the
    peripheral interrupt context.

  Precondition:
    TCMECC_Initialize must have been called for the associated
    TCMECC instance.

  Parameters:
    callback      - A pointer to a function with a calling signature defined by
                    the TCMECC_CALLBACK data type. Setting this to NULL
                    disables the callback feature.

    contextHandle - A value (usually a pointer) passed (unused) into the
                    function identified by the callback parameter.

  Returns:
    None.

  Example:
    <code>
        // Refer to the description of the TCMECC_CALLBACK data type for
        // example usage.
    </code>

  Remarks:
    None.
*/
void TCMECC_NoFixCallbackRegister(TCMECC_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    tcmeccObj.nofix_callback = callback;
    tcmeccObj.nofix_context = contextHandle;
}

// *****************************************************************************
/* Function:
    void TCMECC_INTFIX_InterruptHandler(void)

   Summary:
    TCMECC Peripheral Interrupt Handler.

   Description:
    This function is TCMECC Peripheral Interrupt Handler and will
    called on every TCMECC Fixable interrupt.

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
void TCMECC_INTFIX_InterruptHandler(void)
{

    if (tcmeccObj.fix_callback != NULL)
    {
        tcmeccObj.fix_callback(tcmeccObj.fix_context);
    }
}

// *****************************************************************************
/* Function:
    void TCMECC_INTNOFIX_InterruptHandler(void)

   Summary:
    TCMECC Peripheral Interrupt Handler.

   Description:
    This function is TCMECC Peripheral Interrupt Handler and will
    called on every TCMECC UnFixable interrupt.

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
void TCMECC_INTNOFIX_InterruptHandler(void)
{
    if (tcmeccObj.nofix_callback != NULL)
    {
        tcmeccObj.nofix_callback(tcmeccObj.nofix_context);
    }
}
