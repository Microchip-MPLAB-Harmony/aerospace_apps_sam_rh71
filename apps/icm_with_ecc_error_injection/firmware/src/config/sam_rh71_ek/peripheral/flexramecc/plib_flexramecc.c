/*******************************************************************************
  FLEXRAMECC Peripheral Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_flexramecc.c

  Summary:
    FLEXRAMECC PLIB Implementation file

  Description:
    This file defines the interface to the FLEXRAMECC peripheral library.
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
#include "plib_flexramecc.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

static FLEXRAMECC_OBJ flexrameccObj;

// *****************************************************************************
// *****************************************************************************
// FLEXRAMECC PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/* Function:
    void FLEXRAMECC_Initialize(void)

   Summary:
    Initializes given instance of the FLEXRAMECC peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void FLEXRAMECC_Initialize(void)
{
    FLEXRAMECC_REGS->FLEXRAMECC_CR = FLEXRAMECC_CR_ONE_Msk;

    FLEXRAMECC_REGS->FLEXRAMECC_IER = (FLEXRAMECC_IER_MEM_FIX_Msk | FLEXRAMECC_IER_MEM_NOFIX_Msk);
}
// *****************************************************************************
/* Function:
    FLEXRAMECC_STATUS FLEXRAMECC_StatusGet(void)

   Summary:
    Get the status of the FLEXRAMECC peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Current status of the FLEXRAMECC peripheral.
*/
FLEXRAMECC_STATUS FLEXRAMECC_StatusGet(void)
{
    return (FLEXRAMECC_STATUS)(FLEXRAMECC_REGS->FLEXRAMECC_SR);
}

// *****************************************************************************
/* Function:
    uint32_t* FLEXRAMECC_GetFailAddress(void)

   Summary:
    Get the last fail address were ECC error occurs in FLEXRAM memory.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Pointer of fail address were fixable or unfixable error occurred in FLEXRAM memory.
*/
uint32_t* FLEXRAMECC_GetFailAddress(void)
{
    return (uint32_t*)(FLEXRAMECC_REGS->FLEXRAMECC_FAILAR);
}

// *****************************************************************************
/* Function:
    void FLEXRAMECC_ResetCounters(void)

   Summary:
    Reset Fix and NoFix error counters of the FLEXRAMECC peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void FLEXRAMECC_ResetCounters(void)
{
    FLEXRAMECC_REGS->FLEXRAMECC_CR |= (FLEXRAMECC_CR_RST_FIX_CPT_Msk | FLEXRAMECC_CR_RST_NOFIX_CPT_Msk);
}

// *****************************************************************************
/* Function:
    void FLEXRAMECC_FixCallbackRegister(FLEXRAMECC_CALLBACK callback,
                                                              uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given FLEXRAMECC's ECC Fixable Error interrupt events occur.

  Description:
    This function sets the pointer to a client function to be called "back" when
    the given FLEXRAMECC's interrupt events occur. It also passes a context value
    (usually a pointer to a context structure) that is passed into the function
    when it is called. The specified callback function will be called from the
    peripheral interrupt context.

  Precondition:
    FLEXRAMECC_Initialize must have been called for the associated
    FLEXRAMECC instance.

  Parameters:
    callback      - A pointer to a function with a calling signature defined by
                    the FLEXRAMECC_CALLBACK data type. Setting this to NULL
                    disables the callback feature.

    contextHandle - A value (usually a pointer) passed (unused) into the
                    function identified by the callback parameter.

  Returns:
    None.

  Remarks:
    None.
*/
void FLEXRAMECC_FixCallbackRegister(FLEXRAMECC_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    flexrameccObj.fix_callback = callback;
    flexrameccObj.fix_context = contextHandle;
}

// *****************************************************************************
/* Function:
    void FLEXRAMECC_NoFixCallbackRegister(FLEXRAMECC_CALLBACK callback,
                                                              uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given FLEXRAMECC's ECC Not Fixable Error interrupt events occur.

  Description:
    This function sets the pointer to a client function to be called "back" when
    the given FLEXRAMECC's interrupt events occur. It also passes a context value
    (usually a pointer to a context structure) that is passed into the function
    when it is called. The specified callback function will be called from the
    peripheral interrupt context.

  Precondition:
    FLEXRAMECC_Initialize must have been called for the associated
    FLEXRAMECC instance.

  Parameters:
    callback      - A pointer to a function with a calling signature defined by
                    the FLEXRAMECC_CALLBACK data type. Setting this to NULL
                    disables the callback feature.

    contextHandle - A value (usually a pointer) passed (unused) into the
                    function identified by the callback parameter.

  Returns:
    None.

  Remarks:
    None.
*/
void FLEXRAMECC_NoFixCallbackRegister(FLEXRAMECC_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    flexrameccObj.nofix_callback = callback;
    flexrameccObj.nofix_context = contextHandle;
}


// *****************************************************************************
/* Function:
    void FLEXRAMECC_INTFIX_InterruptHandler(void)

   Summary:
    FLEXRAMECC Peripheral Interrupt Handler.

   Description:
    This function is FLEXRAMECC Peripheral Interrupt Handler and will
    called on every FLEXRAMECC ECC Fixable error interrupt.

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
void __attribute__((used)) FLEXRAMECC_INTFIX_InterruptHandler(void)
{

    if (flexrameccObj.fix_callback != NULL)
    {
        flexrameccObj.fix_callback(flexrameccObj.fix_context);
    }
}

// *****************************************************************************
/* Function:
    void FLEXRAMECC_INTNOFIX_InterruptHandler(void)

   Summary:
    FLEXRAMECC Peripheral Interrupt Handler.

   Description:
    This function is FLEXRAMECC Peripheral Interrupt Handler and will
    called on every FLEXRAMECC ECC Not Fixable error interrupt.

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
void __attribute__((used)) FLEXRAMECC_INTNOFIX_InterruptHandler(void)
{

    if (flexrameccObj.nofix_callback != NULL)
    {
        flexrameccObj.nofix_callback(flexrameccObj.nofix_context);
    }
}
