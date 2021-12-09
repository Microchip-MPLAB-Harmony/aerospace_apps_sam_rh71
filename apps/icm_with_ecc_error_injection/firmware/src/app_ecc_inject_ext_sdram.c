/*******************************************************************************
 MPLAB Harmony Application ECC Inject Ext SDRAM Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecc_inject_ext_sdram.c

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

// *****************************************************************************
// *****************************************************************************
// Section: Extern
// *****************************************************************************
// *****************************************************************************
extern volatile app_ecc_error_count_t g_areaEccErrCountTable[APP_MEMORY_REGION_NUM];

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
    void APP_ECC_INJECT_EXT_SDRAM_FixCallback(uintptr_t context)

   Summary:
    Interrupt callback for fixable error.

   Description:
    This function handle write back for single error on HEMC memory.

   Precondition:
    None.

   Parameters:
    context - pointer to application context object.

   Returns:
    None.

   Note:
    HECC controller issues:
     - When data is read by the ICM, the fault address is incremented by 4.
       -> Workaround: Correct both fail address and previous address
     - When UNAL bit is activated in HSDRAMC, two interrupts are issued.
       -> Workaround: Treat both interrupt. Software counter is incremented twice.
*/
void APP_ECC_INJECT_EXT_SDRAM_FixCallback(uintptr_t context)
{
    /* Read the fault address and data corrected on the fly at fault address before clearing the interrupt*/
    uint32_t* fault_pointer = HEMC_HeccGetFailAddress();
    uint32_t fault_data = *fault_pointer;

    /* HECC controller issue with ICM : write back also the previous address */
    uint32_t fault_data_decr = 0;
    if ( (uint32_t)fault_pointer > 0x64000000UL )
    {
        fault_data_decr = *(fault_pointer - 1);
    }

    __DSB();
    __ISB();
    bool isDataCacheWasEnabled = false;
    bool isInstCacheWasEnabled = false;
    HEMC_HECC_STATUS status = HEMC_HeccGetStatus();

    if ( (status & HEMC_HECC_STATUS_MEM_FIX) == HEMC_HECC_STATUS_MEM_FIX )
    {
        if ( DATA_CACHE_IS_ENABLED() )
        {
            isDataCacheWasEnabled = true;
            DCACHE_DISABLE();
        }
        if ( INSTRUCTION_CACHE_IS_ENABLED() )
        {
            isInstCacheWasEnabled = true;
            ICACHE_DISABLE();
        }

        (*fault_pointer) = fault_data;
        __DSB();
        __ISB();

        if ( (uint32_t)fault_pointer > 0x64000000UL )
        {
            *(fault_pointer - 1) = fault_data_decr;
            __DSB();
            __ISB();
        }

        if ( isDataCacheWasEnabled )
            DCACHE_ENABLE();
        if ( isInstCacheWasEnabled )
            ICACHE_ENABLE();

        g_areaEccErrCountTable[APP_MEMORY_REGION_EXTERNAL_SDRAM].current_fix++;
    }
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_EXT_SDRAM_NoFixCallback(uintptr_t context)

   Summary:
    Interrupt callback for not fixable error.

   Description:
    This function handle write 0xDEADDEAD at fault address for double error
    on HEMC memory.

   Precondition:
    None.

   Parameters:
    context - pointer to application context object.

   Returns:
    None.

   Note:
    HECC controller issues:
     - When data is read by the ICM, the fault address is incremented by 4.
       -> Workaround: Correct both fail address and previous address
     - When UNAL bit is activated in HSDRAMC, two interrupts are issued.
       -> Workaround: Treat both interrupt. Software counter is incremented twice.
*/
void APP_ECC_INJECT_EXT_SDRAM_NoFixCallback(uintptr_t context)
{
    /* Read the fault address before clearing the interrupt*/
    uint32_t* fault_pointer = HEMC_HeccGetFailAddress();
    bool isDataCacheWasEnabled = false;
    bool isInstCacheWasEnabled = false;
    HEMC_HECC_STATUS value = HEMC_HeccGetStatus();

    if ( (value & HEMC_HECC_STATUS_MEM_NOFIX) == HEMC_HECC_STATUS_MEM_NOFIX )
    {
        if ( DATA_CACHE_IS_ENABLED() )
        {
            isDataCacheWasEnabled = true;
            DCACHE_DISABLE();
        }
        if ( INSTRUCTION_CACHE_IS_ENABLED() )
        {
            isInstCacheWasEnabled = true;
            ICACHE_DISABLE();
        }

        (*fault_pointer) = 0xDEADDEAD;
        __DSB();
        __ISB();

        /* HECC controller issue with ICM : write back also the previous address */
        if ( (uint32_t)fault_pointer > 0x64000000UL )
        {
            *(fault_pointer - 1) = 0xDEADDEAD;
            __DSB();
            __ISB();
        }

        if ( isDataCacheWasEnabled )
            DCACHE_ENABLE();
        if ( isInstCacheWasEnabled )
            ICACHE_ENABLE();

        g_areaEccErrCountTable[APP_MEMORY_REGION_EXTERNAL_SDRAM].current_nofix++;
    }
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_EXT_SDRAM_init(void)

   Summary:
    Initialize External SDRAM ECC error injection.

   Description:
    This function perform initialization for SDRAM ECC error injection test. It
    set the interrupt handlers callbacks.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void APP_ECC_INJECT_EXT_SDRAM_init(void)
{
    HEMC_FixCallbackRegister(APP_ECC_INJECT_EXT_SDRAM_FixCallback, (uintptr_t)NULL);
    HEMC_NoFixCallbackRegister(APP_ECC_INJECT_EXT_SDRAM_NoFixCallback, (uintptr_t)NULL);
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_EXT_SDRAM_initialize_error(
            app_ecc_error_inject_t* pEccErrorInject, uint32_t* pBuffer,
            uint32_t buffer_index)

   Summary:
    Initialize ECC error injection variables for HEMC memory.

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
void APP_ECC_INJECT_EXT_SDRAM_initialize_error(
        app_ecc_error_inject_t* pEccErrorInject, uint32_t* pBuffer,
        uint32_t buffer_index)
{
    pEccErrorInject->buffer_index = buffer_index;

    // Get ECC value of the tested data
    HEMC_TestModeReadEnable(HEMC_HEMC_CH_HSDRAMC);
    __DSB();
    __ISB();

    if ( DATA_CACHE_IS_ENABLED() )
        DCACHE_INVALIDATE_BY_ADDR(&(pBuffer[pEccErrorInject->buffer_index]), (int32_t)(sizeof(uint32_t)));

    pEccErrorInject->data = pBuffer[pEccErrorInject->buffer_index];
    __DSB();
    __ISB();

    pEccErrorInject->ecc_tcb1 = (uint8_t)(HEMC_TestModeGetCbValue(HEMC_HEMC_CH_HSDRAMC) & 0xFF);
    __DSB();
    __ISB();

    HEMC_TestModeReadDisable(HEMC_HEMC_CH_HSDRAMC);
}

// *****************************************************************************
/* Function:
    void APP_ECC_INJECT_EXT_SDRAM_generate_error(app_ecc_error_inject_t* pEccErrorInject,
            uint32_t* pBuffer, app_error_type_t error_type)

   Summary:
    Generate single or double ECC error using injection variables in HEMC memory.

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
void APP_ECC_INJECT_EXT_SDRAM_generate_error(const app_ecc_error_inject_t* pEccErrorInject,
        uint32_t* pBuffer, app_error_type_t error_type)
{
    uint8_t tcb1 = pEccErrorInject->ecc_tcb1;
    bool isDataCacheWasEnabled = false;

    if ( error_type == APP_ERROR_TYPE_FIXABLE )
    {
        tcb1 ^= 0x04;
    }
    else if ( error_type == APP_ERROR_TYPE_UNFIXABLE )
    {
        tcb1 ^= 0x05;
    }

    if ( DATA_CACHE_IS_ENABLED() )
    {
        isDataCacheWasEnabled = true;
        DCACHE_DISABLE();
    }
    __DSB();
    __ISB();

    HEMC_TestModeWriteEnable(HEMC_HEMC_CH_HSDRAMC);
    __DSB();
    __ISB();

    HEMC_TestModeSetCbValue(HEMC_HEMC_CH_HSDRAMC, tcb1);
    __DSB();
    __ISB();

    pBuffer[pEccErrorInject->buffer_index] = pEccErrorInject->data;
    __DSB();
    __ISB();

    HEMC_TestModeWriteDisable(HEMC_HEMC_CH_HSDRAMC);

    __DSB();
    __ISB();
    if ( isDataCacheWasEnabled )
    {
        DCACHE_ENABLE();
    }
}
