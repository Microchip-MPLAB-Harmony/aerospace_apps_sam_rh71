/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
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

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#include <string.h>
#include "app_ecc_error.h"
#include "app_ecc_inject_tcm.h"
#include "app_ecc_inject_flexram.h"
#include "app_ecc_inject_ext_sdram.h"

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/
/* Define the switch state press : Active LOW switch */
#define APP_SWITCH_PRESSED_STATE           (0U)
/* Define the switch button PB0 flag */
#define APP_ID_BUTTON0                     (0x01U)
/* Define the switch button PB1 flag */
#define APP_ID_BUTTON1                     (0x02U)
/* Define the switch button PB2 flag */
#define APP_ID_BUTTON2                     (0x04U)

/* Define the ICM region 0 ID */
#define APP_ICM_ID_REGION0                 (0x01U)
/* Define the ICM region 1 ID */
#define APP_ICM_ID_REGION1                 (0x02U)
/* Define the ICM region 2 ID */
#define APP_ICM_ID_REGION2                 (0x04U)
/* Define the ICM region 3 ID */
#define APP_ICM_ID_REGION3                 (0x08U)
/* Define the ICM number of region used */
#define APP_ICM_NUM_REGION                 (4U)
/* Define the ICM hash size in words */
#define APP_ICM_HASH_SIZE_WORD             (8U)

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/
/* Variable containing the bitfield of the pressed button status */
static volatile uint8_t buttonStatus = 0;

/* Variable containing the selected memory region were ECC error is injected */
static app_memory_region_t g_selectedMemoryRegion;

/* Buffer of data placed in ITCM memory were ECC error will be injected */
static uint32_t __attribute__((section (".itcm_data"))) buffer_itcm[APP_BUFFER_LENGTH] = {0};

/* Buffer of data placed in DTCM memory were ECC error will be injected */
static uint32_t __attribute__((tcm)) buffer_dtcm[APP_BUFFER_LENGTH] = {0};

/* Buffer of data placed in internal FlexRAM memory were ECC error will be injected */
static uint32_t __attribute__((section (".ram_data"))) buffer_internal_sram[APP_BUFFER_LENGTH] = {0};

/* Buffer of data placed in external RAM memory were ECC error will be injected */
static uint32_t __attribute__((section (".ext_sdram"))) buffer_external_ram[APP_BUFFER_LENGTH];

/* Hash Buffer were ICM will store the computed hash for each region */
static uint32_t __attribute__((aligned (128))) bufferHash[APP_ICM_NUM_REGION][APP_ICM_HASH_SIZE_WORD] = {0};

/* Variable containing the bitfield of the ICM region were hash completed event occurs */
static volatile uint8_t regionHashCompleted;

/* Variable containing the bitfield of the ICM region were digest mismatch event occurs */
static volatile uint8_t regionDigestMismatch;

/* Table containing the current position in buffer were next error will be injected for each region */
static uint32_t g_eccErrorBufferPosition[APP_MEMORY_REGION_NUM] = {0};

/* Table of ECC injection parameters for each region */
static app_ecc_error_inject_t g_eccErrorInjectTable[APP_MEMORY_REGION_NUM] = {0};

// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************

/* Function:
    void APP_ControlSwitch(PIO_PIN pin, uintptr_t context)

   Summary:
    Interrupt callback for PIO interrupt.

   Description:
    This function update the global Bitfield to indicate press event occurs
    on button.

   Parameters:
    pin - pio structure of the pin that triggers the interrupt.
    context - pointer to application context object.

   Returns:
    None.
*/
static void APP_ControlSwitch(PIO_PIN pin, uintptr_t context)
{
    if ( pin == SWITCH0_PIN )
    {
        if ( SWITCH0_Get() == APP_SWITCH_PRESSED_STATE )
        {
            buttonStatus |= APP_ID_BUTTON0;
        }
    }
    else if ( pin == SWITCH1_PIN )
    {
        if ( SWITCH1_Get() == APP_SWITCH_PRESSED_STATE )
        {
            buttonStatus |= APP_ID_BUTTON1;
        }
    }
    else if ( pin == SWITCH2_PIN )
    {
        if ( SWITCH2_Get() == APP_SWITCH_PRESSED_STATE )
        {
            buttonStatus |= APP_ID_BUTTON2;
        }
    }
    else
    {
        /* Pin not handled */
    }
}

// *****************************************************************************
/* void APP_ICM_Callback(uintptr_t context)

  Summary:
    Function called by ICM PLIB.

  Description:
    This function will be called by ICM PLIB when interrupt has occurred.

  Remarks:
    None.
*/
static void APP_ICM_Callback(uintptr_t context)
{
    ICM_INT_MSK status = (ICM_InterruptGet() & ICM_InterruptMasked());

    /* Check Digest Mismatch interrupts */
    if ( ( status & ICM_INT_MSK_DIGEST_MISMATCH_R0_MASK ) != 0U )
    {
        regionDigestMismatch |= APP_ICM_ID_REGION0;
        ICM_InterruptDisable(ICM_INT_MSK_DIGEST_MISMATCH_R0_MASK);
    }

    if ( ( status & ICM_INT_MSK_DIGEST_MISMATCH_R1_MASK ) != 0U )
    {
        regionDigestMismatch |= APP_ICM_ID_REGION1;
        ICM_InterruptDisable(ICM_INT_MSK_DIGEST_MISMATCH_R1_MASK);
    }

    if ( ( status & ICM_INT_MSK_DIGEST_MISMATCH_R2_MASK ) != 0U )
    {
        regionDigestMismatch |= APP_ICM_ID_REGION2;
        ICM_InterruptDisable(ICM_INT_MSK_DIGEST_MISMATCH_R2_MASK);
    }

    if ( ( status & ICM_INT_MSK_DIGEST_MISMATCH_R3_MASK ) != 0U )
    {
        regionDigestMismatch |= APP_ICM_ID_REGION3;
        ICM_InterruptDisable(ICM_INT_MSK_DIGEST_MISMATCH_R3_MASK);
    }

    /* Check Hash Complete interrupts */
    if ( ( status & ICM_INT_MSK_HASH_R0_MASK ) != 0U )
    {
        regionHashCompleted |= APP_ICM_ID_REGION0;
        ICM_InterruptDisable(ICM_INT_MSK_HASH_R0_MASK);
    }

    if ( ( status & ICM_INT_MSK_HASH_R1_MASK ) != 0U )
    {
        regionHashCompleted |= APP_ICM_ID_REGION1;
        ICM_InterruptDisable(ICM_INT_MSK_HASH_R1_MASK);
    }

    if ( ( status & ICM_INT_MSK_HASH_R2_MASK ) != 0U )
    {
        regionHashCompleted |= APP_ICM_ID_REGION2;
        ICM_InterruptDisable(ICM_INT_MSK_HASH_R2_MASK);
    }

    if ( ( status & ICM_INT_MSK_HASH_R3_MASK ) != 0U )
    {
        regionHashCompleted |= APP_ICM_ID_REGION3;
        ICM_InterruptDisable(ICM_INT_MSK_HASH_R3_MASK);
    }
}

// *****************************************************************************
/* Function:
    void APP_generate_ecc_error(PIO_PIN pin, uintptr_t context)

   Summary:
    Generate ECC error for the current selected memory region.

   Description:
    This function call the function to initialize and generate a new ECC
    error depending on the current memory region selected.

   Parameters:
    error_type - type or ECC error to inject (APP_ERROR_TYPE_FIXABLE or APP_ERROR_TYPE_UNFIXABLE).

   Returns:
    None.
*/
static void APP_generate_ecc_error(app_error_type_t error_type)
{
    ICM_Disable();

    while ( (ICM_StatusGet() & ICM_STATUS_ENABLE) == ICM_STATUS_ENABLE )
    {
        /* Wait current memory region is monitored */
    }

    if ( error_type == APP_ERROR_TYPE_FIXABLE )
    {
        printf("-> Generate fixable error on region %s at index : %u \n\r",
                g_memoryRegionString[g_selectedMemoryRegion],
                (unsigned int)g_eccErrorBufferPosition[g_selectedMemoryRegion]);
    }
    else if ( error_type == APP_ERROR_TYPE_UNFIXABLE )
    {
        printf("-> Generate not fixable error on region %s at index : %u \n\r",
                g_memoryRegionString[g_selectedMemoryRegion],
                (unsigned int)g_eccErrorBufferPosition[g_selectedMemoryRegion]);
    }
    else
    {
        /* Wrong error type value */
    }

    switch ( g_selectedMemoryRegion )
    {
        case APP_MEMORY_REGION_ITCM:
            APP_ECC_INJECT_TCM_initialize_error(
                &(g_eccErrorInjectTable[APP_MEMORY_REGION_ITCM]),
                &(buffer_itcm[0]),
                g_eccErrorBufferPosition[APP_MEMORY_REGION_ITCM]);
            APP_ECC_INJECT_TCM_generate_error(
                &(g_eccErrorInjectTable[APP_MEMORY_REGION_ITCM]),
                &(buffer_itcm[0]),
                error_type);
            break;
        case APP_MEMORY_REGION_DTCM:
            APP_ECC_INJECT_TCM_initialize_error(
                &(g_eccErrorInjectTable[APP_MEMORY_REGION_DTCM]),
                &(buffer_dtcm[0]),
                g_eccErrorBufferPosition[APP_MEMORY_REGION_DTCM]);
            APP_ECC_INJECT_TCM_generate_error(
                &(g_eccErrorInjectTable[APP_MEMORY_REGION_DTCM]),
                &(buffer_dtcm[0]),
                error_type);
            break;
        case APP_MEMORY_REGION_INTERNAL_SRAM:
            APP_ECC_INJECT_FLEXRAM_initialize_error(
                &(g_eccErrorInjectTable[APP_MEMORY_REGION_INTERNAL_SRAM]),
                &(buffer_internal_sram[0]),
                g_eccErrorBufferPosition[APP_MEMORY_REGION_INTERNAL_SRAM]);
            APP_ECC_INJECT_FLEXRAM_generate_error(
                &(g_eccErrorInjectTable[APP_MEMORY_REGION_INTERNAL_SRAM]),
                &(buffer_internal_sram[0]),
                error_type);
            break;
        case APP_MEMORY_REGION_EXTERNAL_RAM:
            APP_ECC_INJECT_EXT_SDRAM_initialize_error(
                &(g_eccErrorInjectTable[APP_MEMORY_REGION_EXTERNAL_RAM]),
                &(buffer_external_ram[0]),
                g_eccErrorBufferPosition[APP_MEMORY_REGION_EXTERNAL_RAM]);
            APP_ECC_INJECT_EXT_SDRAM_generate_error(
                &(g_eccErrorInjectTable[APP_MEMORY_REGION_EXTERNAL_RAM]),
                &(buffer_external_ram[0]),
                error_type);
            break;
        default:
            /* Wrong value : not a memory region type */
            break;
    }

    ICM_Enable();

    /* Increment injection position in buffer */
    g_eccErrorBufferPosition[g_selectedMemoryRegion]++;
    if ( g_eccErrorBufferPosition[g_selectedMemoryRegion] >= 5UL )
    {
      g_eccErrorBufferPosition[g_selectedMemoryRegion] = 0UL;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
 {
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    printf("\n\r-----------------------------------------------------------");
    printf("\n\r  ICM with ECC error injection example                     ");
    printf("\n\r-----------------------------------------------------------\n\r");

    PIO_PinInterruptCallbackRegister(SWITCH0_PIN, APP_ControlSwitch, (uintptr_t)NULL);
    PIO_PinInterruptCallbackRegister(SWITCH1_PIN, APP_ControlSwitch, (uintptr_t)NULL);
    PIO_PinInterruptCallbackRegister(SWITCH2_PIN, APP_ControlSwitch, (uintptr_t)NULL);
    PIO_PinInterruptEnable(SWITCH0_PIN);
    PIO_PinInterruptEnable(SWITCH1_PIN);
    PIO_PinInterruptEnable(SWITCH2_PIN);

    APP_ECC_INJECT_TCM_init();
    APP_ECC_INJECT_FLEXRAM_init();
    APP_ECC_INJECT_EXT_SDRAM_init();

    /* Set initial data in memory buffers */
    memset(buffer_itcm, (int)('a'), sizeof(buffer_itcm));
    memset(buffer_dtcm, (int)('a'), sizeof(buffer_dtcm));
    memset(buffer_internal_sram, (int)('a'), sizeof(buffer_internal_sram));
    if ( DATA_CACHE_IS_ENABLED() != 0U )
    {
        DCACHE_CLEAN_INVALIDATE_BY_ADDR((uint32_t *)buffer_internal_sram, (int32_t)(sizeof(buffer_internal_sram)));
    }
    memset(buffer_external_ram, (int)('a'), sizeof(buffer_external_ram));
    if ( DATA_CACHE_IS_ENABLED() != 0U )
    {
        DCACHE_CLEAN_INVALIDATE_BY_ADDR((uint32_t *)buffer_external_ram, (int32_t)(sizeof(buffer_external_ram)));
    }

    /* Set ICM memory address for generated hash */
    ICM_SetHashStartAddress((uint32_t)&bufferHash);

    regionHashCompleted = 0;
    regionDigestMismatch = 0;

    /* Register ICM Callback */
    ICM_CallbackRegister(APP_ICM_Callback, (uintptr_t)NULL);

    ICM_InterruptEnable(ICM_INT_MSK_HASH_R0_MASK | ICM_INT_MSK_HASH_R1_MASK |
                        ICM_INT_MSK_HASH_R2_MASK | ICM_INT_MSK_HASH_R3_MASK);

    /* Enable ICM to perform first hash of regions */
    printf("Enable ICM region(0-3)...\n\r");
    ICM_Enable();


    while ( regionHashCompleted != (APP_ICM_ID_REGION0 | APP_ICM_ID_REGION1 | APP_ICM_ID_REGION2 | APP_ICM_ID_REGION3) )
    {
        /* Wait first hash is completed */
    }

    /* Disable ICM */
    ICM_Disable();

    /* Disable ICM Write back feature */
    ICM_WriteBackDisable(true);

    regionHashCompleted = 0;
    regionDigestMismatch = 0;

    /* Enable ICM monitor mode */
    ICM_MonitorEnable(ICM_REGION0_MASK | ICM_REGION1_MASK | ICM_REGION2_MASK | ICM_REGION3_MASK);
    ICM_InterruptEnable(ICM_INT_MSK_DIGEST_MISMATCH_R0_MASK |
                        ICM_INT_MSK_DIGEST_MISMATCH_R1_MASK |
                        ICM_INT_MSK_DIGEST_MISMATCH_R2_MASK |
                        ICM_INT_MSK_DIGEST_MISMATCH_R3_MASK);
    ICM_Enable();

    printf("ICM monitoring started for region (0-3)...\n\r");
    printf("Press [Push Button 0] to change ecc error injection memory target(ITCM, DTCM, FlexRAM, external RAM memory)\n\r");
    printf("Press [Push Button 1] to inject fixable ecc error\n\r");
    printf("Press [Push Button 2] to inject unfixable ecc error\n\r");
    printf("Current ECC error injection is selected for memory : %s\n\r",
            g_memoryRegionString[g_selectedMemoryRegion]);

    while (1)
    {
        /* Print ECC error counts */
        APP_ECC_ERROR_print_interrupt_count();

        /* Handle ICM mismatch event : set back initial data in buffers */
        if ( regionDigestMismatch != 0U )
        {
            uint32_t mismatch = regionDigestMismatch;
            ICM_Disable();

            while ( (ICM_StatusGet() & ICM_STATUS_ENABLE) == ICM_STATUS_ENABLE )
            {
                /* Wait end of current memory region monitored */
            }

            /* Preform dummy read of status to clear pending interrupts */
            ICM_INT_MSK dummy_status = ICM_InterruptGet();
            (void)(dummy_status);

            if ( ( mismatch & APP_ICM_ID_REGION0 ) != 0U )
            {
                memset(buffer_itcm, (int)('a'), sizeof(buffer_itcm));
                regionDigestMismatch &= ~(APP_ICM_ID_REGION0);
                ICM_InterruptEnable(ICM_INT_MSK_DIGEST_MISMATCH_R0_MASK);
            }
            if ( ( mismatch & APP_ICM_ID_REGION1 ) != 0U )
            {
                memset(buffer_dtcm, (int)('a'), sizeof(buffer_dtcm));
                regionDigestMismatch &= ~(APP_ICM_ID_REGION1);
                ICM_InterruptEnable(ICM_INT_MSK_DIGEST_MISMATCH_R1_MASK);
            }
            if ( ( mismatch & APP_ICM_ID_REGION2 ) != 0U )
            {
                memset(buffer_internal_sram, (int)('a'), sizeof(buffer_internal_sram));
                if ( DATA_CACHE_IS_ENABLED() )
                {
                    DCACHE_CLEAN_BY_ADDR((uint32_t *)buffer_internal_sram, (int32_t)(sizeof(buffer_internal_sram)));
                }
                regionDigestMismatch &= ~(APP_ICM_ID_REGION2);
                ICM_InterruptEnable(ICM_INT_MSK_DIGEST_MISMATCH_R2_MASK);
            }
            if ( ( mismatch & APP_ICM_ID_REGION3 ) != 0U )
            {
                memset(buffer_external_ram, (int)('a'), sizeof(buffer_external_ram));
                if ( DATA_CACHE_IS_ENABLED() )
                    DCACHE_CLEAN_BY_ADDR((uint32_t *)buffer_external_ram, (int32_t)(sizeof(buffer_external_ram)));
                regionDigestMismatch &= ~(APP_ICM_ID_REGION3);
                ICM_InterruptEnable(ICM_INT_MSK_DIGEST_MISMATCH_R3_MASK);
            }

            ICM_Enable();

            printf("  ICM Digest mismatch occurs @ region(s) 0x%x \n\r",
                    (unsigned int) mismatch);
        }

        /* Handle buttons events */
        if ( buttonStatus != 0U )
        {
            uint8_t button = buttonStatus;

            if ( button & APP_ID_BUTTON0 )
            {
                /* Change current memory region */
                g_selectedMemoryRegion++;
                if ( g_selectedMemoryRegion > APP_MEMORY_REGION_EXTERNAL_RAM )
                {
                    g_selectedMemoryRegion = APP_MEMORY_REGION_ITCM;
                }
                printf("ECC error injection in memory : %s\n\r",
                        g_memoryRegionString[g_selectedMemoryRegion]);
            }
            if ( button & APP_ID_BUTTON1 )
            {
                /* Generate one error in ECC */
                APP_generate_ecc_error(APP_ERROR_TYPE_FIXABLE);
            }
            if ( button & APP_ID_BUTTON2 )
            {
                /* Generate nofixable error */
                APP_generate_ecc_error(APP_ERROR_TYPE_UNFIXABLE);
            }

            buttonStatus &= ~(button & 0xFFU);
        }
    }

    /* Execution should not come here during normal operation */
    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

