/*******************************************************************************
  MPLAB Harmony Application ECC Error Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_ecc_error.h

  Summary:
    ECC ERROR Prototypes and definitions file.

  Description:
    None.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END/*

#ifndef APP_ECC_ERROR_H
#define	APP_ECC_ERROR_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Macros Definitions
// *****************************************************************************
// *****************************************************************************
/* Lenght of data buffer hashed by ICM */
#define APP_BUFFER_LENGTH                                         (2512)

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
/* Application ECC Error Type

   Summary:
    Type of ECC error injected.

   Description:
    This data type defines the type of ECC error injected.

   Remarks:
    None.
*/
typedef enum
{
    APP_ERROR_TYPE_FIXABLE = 0,
    APP_ERROR_TYPE_UNFIXABLE = 1
} app_error_type_t;

// *****************************************************************************
/* Application memory regions

   Summary:
    Application memory region identifier.

   Description:
    This data type defines the different memory region
    were data is hash by ICM and ECC error is injected.

   Remarks:
    None.
*/
typedef enum
{
    APP_MEMORY_REGION_ITCM = 0,
    APP_MEMORY_REGION_DTCM, 
    APP_MEMORY_REGION_INTERNAL_SRAM,
    APP_MEMORY_REGION_EXTERNAL_RAM,
    APP_MEMORY_REGION_NUM
} app_memory_region_t;

// *****************************************************************************
/* Application ECC Error Inject parameters

   Summary:
    ECC error injection parameters structure.

   Description:
    This data structure defines the parameters used for ECC error injection.

   Remarks:
    None.
*/
typedef struct
{
    uint32_t buffer_index;
    uint32_t data;
    uint8_t ecc_tcb1;
    uint8_t ecc_tcb2;
} app_ecc_error_inject_t;

// *****************************************************************************
/* Application ECC Error Count parameters

   Summary:
    ECC error count parameters structure.

   Description:
    This data structure defines the parameters used for counting ECC error.

   Remarks:
    None.
*/
typedef struct
{
    uint32_t current_fix;
    uint32_t current_nofix;
    uint32_t last_fix;
    uint32_t last_nofix;
} app_ecc_error_count_t;

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void APP_ECC_ERROR_print_interrupt_count(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif	/* APP_ECC_ERROR_H */

