/*******************************************************************************
 MPLAB Harmony Application ECC Inject Flexram Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecc_inject_flexram.c

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
#include "app_ecc_inject_flexram.h"

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
    void APP_ECC_INJECT_FLEXRAM_FixCallback(uintptr_t context)

   Summary:
    Interrupt callback for fixable error.

   Description:
    This function handle write back for single error on FlexRAM memory.

   Precondition:
    None.

   Parameters:
    context - pointer to application context object.

   Returns:
    None.
*/
static void APP_ECC_INJECT_FLEXRAM_FixCallback(uintptr_t context)
{
    uint32_t* fault_pointer = FLEXRAMECC_GetFailAddress();
    FLEXRAMECC_STATUS status_reg = FLEXRAMECC_StatusGet();

    if ( ( status_reg & FLEXRAMECC_STATUS_MEM_FIX ) != 0U )
    {
        // Fault address not used
        (void)(fault_pointer);
        g_areaEccErrCountTable[APP_MEMORY_REGION_INTERNAL_SRAM].current_fix++;
    }
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_FLEXRAM_NoFixCallback(uintptr_t context)

   Summary:
    Interrupt callback for not fixable error.

   Description:
    This function handle write 0xDEADDEAD at fault address for double error
    on FlexRAM memory.

   Precondition:
    None.

   Parameters:
    context - pointer to application context object.

   Returns:
    None.
*/
static void APP_ECC_INJECT_FLEXRAM_NoFixCallback(uintptr_t context)
{
    uint32_t* fault_pointer = FLEXRAMECC_GetFailAddress();
    __DSB();
    __ISB();
    bool isDataCacheWasEnabled = false;
    bool isInstCacheWasEnabled = false;
    FLEXRAMECC_STATUS status_reg = FLEXRAMECC_StatusGet();

    if ( ( status_reg & FLEXRAMECC_STATUS_MEM_NOFIX ) != 0U )
    {
        if ( DATA_CACHE_IS_ENABLED() != 0U )
        {
            isDataCacheWasEnabled = true;
            DCACHE_DISABLE();
        }
        if ( INSTRUCTION_CACHE_IS_ENABLED() != 0U )
        {
            isInstCacheWasEnabled = true;
            ICACHE_DISABLE();
        }

        //write backs shall be implemented to avoid dead lock when MPU of flexRAM is in normal mode.
        (*fault_pointer) = 0xDEADDEADUL;
        __ISB();
        __DSB();

        if ( isDataCacheWasEnabled )
        {
            DCACHE_ENABLE();
        }
        if ( isInstCacheWasEnabled )
        {
            ICACHE_ENABLE();
        }

        g_areaEccErrCountTable[APP_MEMORY_REGION_INTERNAL_SRAM].current_nofix++;
    }
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_FLEXRAM_init(void)

   Summary:
    Initialize Internal FlexRAM ECC error injection.

   Description:
    This function perform initialization for FlexRAM ECC error injection test.
    It set the interrupt handlers callbacks.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void APP_ECC_INJECT_FLEXRAM_init(void)
{
    FLEXRAMECC_FixCallbackRegister(APP_ECC_INJECT_FLEXRAM_FixCallback, (uintptr_t)NULL);
    FLEXRAMECC_NoFixCallbackRegister(APP_ECC_INJECT_FLEXRAM_NoFixCallback, (uintptr_t)NULL);
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_FLEXRAM_initialize_error(
            app_ecc_error_inject_t* pEccErrorInject, uint32_t* pBuffer,
            uint32_t buffer_index)

   Summary:
    Initialize ECC error injection variables for FlexRAM memory.

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
void APP_ECC_INJECT_FLEXRAM_initialize_error(
    app_ecc_error_inject_t* pEccErrorInject, uint32_t* pBuffer,
    uint32_t buffer_index)
{
    pEccErrorInject->buffer_index = buffer_index;

    // Get ECC value of the tested data
    FLEXRAMECC_TestModeSetCbValue(0);
    FLEXRAMECC_TestModeReadEnable();
    __DSB();
    __ISB();

    if ( DATA_CACHE_IS_ENABLED() != 0U )
    {
        DCACHE_INVALIDATE_BY_ADDR(&(pBuffer[pEccErrorInject->buffer_index]), (int32_t)(sizeof(uint32_t)));
    }

    pEccErrorInject->data = pBuffer[pEccErrorInject->buffer_index];
    __DSB();
    __ISB();

    pEccErrorInject->ecc_tcb1 = FLEXRAMECC_TestModeGetCbValue();
    __DSB();
    __ISB();

    FLEXRAMECC_TestModeReadDisable();
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_FLEXRAM_generate_error(const app_ecc_error_inject_t* pEccErrorInject,
            uint32_t* pBuffer, app_error_type_t error_type)

   Summary:
    Generate single or double ECC error using injection variables in FlexRAM memory.

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
void APP_ECC_INJECT_FLEXRAM_generate_error(const app_ecc_error_inject_t* pEccErrorInject,
    uint32_t* pBuffer, app_error_type_t error_type)
{
    uint8_t tcb1 = pEccErrorInject->ecc_tcb1;
    bool isDataCacheWasEnabled = false;

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

    if ( DATA_CACHE_IS_ENABLED() != 0U )
    {
        isDataCacheWasEnabled = true;
        DCACHE_DISABLE();
    }

    __disable_irq();
    __DSB();
    __ISB();

    FLEXRAMECC_TestModeWriteEnable();
    __DSB();
    __ISB();

    FLEXRAMECC_TestModeSetCbValue(tcb1);
    __DSB();
    __ISB();

    pBuffer[pEccErrorInject->buffer_index] = pEccErrorInject->data;
    __DSB();
    __ISB();

    FLEXRAMECC_TestModeWriteDisable();
    __DSB();
    __ISB();
    __enable_irq();

    if ( isDataCacheWasEnabled )
    {
        DCACHE_ENABLE();
    }
}
