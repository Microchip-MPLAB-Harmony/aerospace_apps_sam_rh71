/*******************************************************************************
 MPLAB Harmony Application ECC Inject TCM Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecc_inject_tcm.c

  Summary:
    ECC ERROR Implementation File

  Description:
    None

*******************************************************************************/

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "definitions.h"                // SYS function prototypes
#include "app_ecc_error.h"
#include "app_ecc_inject_tcm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Globals
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/* Function:
    void APP_ECC_INJECT_TCM_FixCallback(uintptr_t context)

   Summary:
    Interrupt callback for fixable error.

   Description:
    This function handle write back for single error on TCM memory.

   Precondition:
    None.

   Parameters:
    context - pointer to application context object.

   Returns:
    None.
*/
static void APP_ECC_INJECT_TCM_FixCallback(uintptr_t context)
{
    uint32_t fault_data_word = 0;
    uint32_t* fault_pointer_word = TCMECC_GetFailAddressDTCM();
    uint64_t* fault_pointer = TCMECC_GetFailAddressITCM();
    uint64_t fault_data = *fault_pointer;

    if ( ( (uint32_t)fault_pointer_word >= 0x20000000UL ) && ( (uint32_t)fault_pointer_word <= 0x2003FFFFUL ) )
    {
        fault_data_word = *fault_pointer_word;
    }

    TCMECC_STATUS status_reg = TCMECC_StatusGet();

    if ( ( status_reg & TCMECC_STATUS_MEM_FIX_I ) != 0U )
    {
        (*fault_pointer) = fault_data;
        __ISB();
        __DSB();
        g_areaEccErrCountTable[APP_MEMORY_REGION_ITCM].current_fix++;
    }

    if ( ( status_reg & TCMECC_STATUS_MEM_FIX_D ) != 0U )
    {
        if ( ( (uint32_t)fault_pointer_word >= 0x20000000UL) && ( (uint32_t)fault_pointer_word <= 0x2003FFFFUL) )
        {
            (*fault_pointer_word) = fault_data_word;
            __ISB();
            __DSB();
            g_areaEccErrCountTable[APP_MEMORY_REGION_DTCM].current_fix++;
        }
    }
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_TCM_NoFixCallback(uintptr_t context)

   Summary:
    Interrupt callback for not fixable error.

   Description:
    This function handle write 0xDEADDEAD at fault address for double error
    on TCM memory.

   Precondition:
    None.

   Parameters:
    context - pointer to application context object.

   Returns:
    None.
*/
static void APP_ECC_INJECT_TCM_NoFixCallback(uintptr_t context)
{
    uint32_t* fault_pointer_word = TCMECC_GetFailAddressDTCM();
    uint64_t* fault_pointer = TCMECC_GetFailAddressITCM();
    TCMECC_STATUS status_reg = TCMECC_StatusGet();

    if ( ( status_reg & TCMECC_STATUS_MEM_NOFIX_I ) != 0U )
    {
        (*fault_pointer) = 0xDEADDEADDEADDEADULL;
        __ISB();
        __DSB();
        g_areaEccErrCountTable[APP_MEMORY_REGION_ITCM].current_nofix++;
    }

    if ( (status_reg & TCMECC_STATUS_MEM_NOFIX_D) != 0U )
    {
        if ( ( (uint32_t)fault_pointer_word >= 0x20000000UL) && ( (uint32_t)fault_pointer_word <= 0x2003FFFFUL) )
        {
            (*fault_pointer_word) = 0xDEADDEADUL;
            __ISB();
            __DSB();
             g_areaEccErrCountTable[APP_MEMORY_REGION_DTCM].current_nofix++;
        }
    }
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_TCM_init(void)

   Summary:
    Initialize External TCM ECC error injection.

   Description:
    This function perform initialization for TCM ECC error injection test. It
    set the interrupt handlers callbacks.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void APP_ECC_INJECT_TCM_init(void)
{
    /* Set handler for fix and nofix callback */
    TCMECC_FixCallbackRegister(APP_ECC_INJECT_TCM_FixCallback, (uintptr_t)NULL);
    TCMECC_NoFixCallbackRegister(APP_ECC_INJECT_TCM_NoFixCallback, (uintptr_t)NULL);
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_TCM_initialize_error(
            app_ecc_error_inject_t* pEccErrorInject, uint32_t* pBuffer,
            uint32_t buffer_index)

   Summary:
    Initialize ECC error injection variables for TCM memory.

   Description:
    Get the ECC check bit value at the selected index in the buffer.
    Update the data structure with the buffer index and read check bit.

   Precondition:
    None.

   Parameters:
    pEccErrorInject - pointer to ECC injection parameters data structure to be updated.
    pBuffer - pointer to data buffer.
    buffer_index - index of the word in the data buffer where data and check bit will be read.

   Returns:
    None.
*/
void APP_ECC_INJECT_TCM_initialize_error(
        app_ecc_error_inject_t* pEccErrorInject, uint32_t* pBuffer,
        uint32_t buffer_index)
{
    pEccErrorInject->buffer_index = buffer_index;
    __DSB();
    __ISB();

    TCMECC_TestModeSetCbValue(0, 0);
    __DSB();
    __ISB();

    TCMECC_TestModeReadEnable();
    __DSB();
    __ISB();

    pEccErrorInject->data = pBuffer[pEccErrorInject->buffer_index];
    TCMECC_TestModeReadDisable();
    __DSB();
    __ISB();

    do
    {
        TCMECC_TestModeGetCbValue(&(pEccErrorInject->ecc_tcb1), &(pEccErrorInject->ecc_tcb2));
    }
    while ( (pEccErrorInject->ecc_tcb1 == 0U) && (pEccErrorInject->ecc_tcb2 == 0U) );
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_TCM_generate_error(const app_ecc_error_inject_t* pEccErrorInject,
            uint32_t* pBuffer, app_error_type_t error_type)

   Summary:
    Generate single or double ECC error using injection variables in TCM memory.

   Description:
    Compute ECC check bit with single or double error and write in the data
    buffer using parameters in the given error injection parameter structure.

   Precondition:
    None.

   Parameters:
    pEccErrorInject - pointer to ECC injection parameters data structure.
    pBuffer - pointer to data buffer.
    error_type - type or ECC error to inject (APP_ERROR_TYPE_FIXABLE or APP_ERROR_TYPE_UNFIXABLE).

   Returns:
    None.
*/
void APP_ECC_INJECT_TCM_generate_error(const app_ecc_error_inject_t* pEccErrorInject,
        uint32_t* pBuffer, app_error_type_t error_type)
{
    uint8_t tcb1 = pEccErrorInject->ecc_tcb1;
    uint8_t tcb2 = pEccErrorInject->ecc_tcb2;

    if ( error_type == APP_ERROR_TYPE_FIXABLE )
    {
        tcb1 ^= 0x04U;
    }
    else if ( error_type == APP_ERROR_TYPE_UNFIXABLE )
    {
        tcb1 ^= 0x05U;
    }
    else
    {
        /* Error Type not handled */
    }

    __disable_irq();
    __DSB();
    __ISB();

    TCMECC_TestModeWriteEnable();
    __DSB();
    __ISB();

    TCMECC_TestModeSetCbValue(tcb1, tcb2);
    __DSB();
    __ISB();

    pBuffer[pEccErrorInject->buffer_index] = pEccErrorInject->data;
    __DSB();
    __ISB();

    TCMECC_TestModeWriteDisable();
    __DSB();
    __ISB();
    __enable_irq();
}
