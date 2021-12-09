/*******************************************************************************
 MPLAB Harmony Application ECC ERROR Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecc_error.c

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
#include <stdio.h>
#include "app_ecc_error.h"

// *****************************************************************************
// *****************************************************************************
// Section: Globals
// *****************************************************************************
// *****************************************************************************
/* Table to store the current error count on each regions */
volatile app_ecc_error_count_t g_areaEccErrCountTable[APP_MEMORY_REGION_NUM] = {0};

/* Region name strings for output print */
const char* g_memoryRegionString[] = { "ITCM", "DTCM", "int SRAM", "ext SDRAM" };

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void APP_ECC_ERROR_print_interrupt_count(void)

   Summary:
    Application print of last ECC Error count.

   Description:
    This function print the differences of new ECC Error count
    since last call.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.
*/
void APP_ECC_ERROR_print_interrupt_count(void)
{
    for (uint8_t area = 0; area < 4; area++)
    {
        uint32_t tmp = g_areaEccErrCountTable[area].current_fix;
        if ( tmp != g_areaEccErrCountTable[area].last_fix )
        {
            printf("  %u new FIX interrupt on region %s\n\r",
                    (unsigned int)(tmp - g_areaEccErrCountTable[area].last_fix),
                    g_memoryRegionString[area]);
            g_areaEccErrCountTable[area].last_fix = tmp;
        }

        tmp = g_areaEccErrCountTable[area].current_nofix;
        if ( tmp != g_areaEccErrCountTable[area].last_nofix )
        {
            printf("  %u new NOFIX interrupt on region %s\n\r",
                    (unsigned int)(tmp - g_areaEccErrCountTable[area].last_nofix),
                    g_memoryRegionString[area]);
            g_areaEccErrCountTable[area].last_nofix = tmp;
        }
    }
}
