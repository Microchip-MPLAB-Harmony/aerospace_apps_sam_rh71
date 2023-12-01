/*******************************************************************************
  SPW Peripheral Library
  Instance Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw.h

  Summary:
    SPW PLIB Header file

  Description:
    This file defines the interface to the SPW peripheral 
    library. This library provides access to and control of the associated 
    peripheral instance.
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

#ifndef PLIB_SPW_H
#define PLIB_SPW_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include "device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: SPW defines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: SPW Types
// *****************************************************************************
// *****************************************************************************
/* SPW IRQ status
   Summary:
    Identifies the SPW IRQ lines that have pending interrupt

   Description:
    This data type identifies the SPW IRQ lines that have pending interrupt
*/
typedef uint32_t SPW_INT_MASK;

#define SPW_INT_MASK_NONE                                         (0UL)
#define SPW_INT_MASK_PKTRX1                                       ( 1UL << 0U)
#define SPW_INT_MASK_PKTTX1                                       ( 1UL << 1U)
#define SPW_INT_MASK_TCH                                          ( 1UL << 2U)
#define SPW_INT_MASK_LINK2                                        ( 1UL << 3U)
#define SPW_INT_MASK_DIA2                                         ( 1UL << 4U)
#define SPW_INT_MASK_DI2                                          ( 1UL << 5U)
#define SPW_INT_MASK_LINK1                                        ( 1UL << 6U)
#define SPW_INT_MASK_DIA1                                         ( 1UL << 7U)
#define SPW_INT_MASK_DI1                                          ( 1UL << 8U)
#define SPW_INT_MASK_INVALID                                      (0xFFFFFFFFUL)

// *****************************************************************************
/* SPW Synchronization events mask
   Summary:
    Identifies the SPW synchronization events mask

   Description:
    This data type identifies the SPW synchronization events mask
*/
typedef uint32_t SPW_SYNC_EVENT_MASK;

#define SPW_SYNC_EVENT_MASK_RTCOUT0                               (0x01UL)
#define SPW_SYNC_EVENT_MASK_RTCOUT1                               (0x02UL)
#define SPW_SYNC_EVENT_MASK_INVALID                               (0xFFFFFFFFUL)


// *****************************************************************************
/* SPW Callback

   Summary:
    SPW Callback Function Pointer.

   Description:
    This data type defines the SPW Callback Function Pointer.

   Remarks:
    None.
*/
typedef void (*SPW_CALLBACK) (SPW_INT_MASK irqStatus, uintptr_t contextHandle);

// *****************************************************************************

/* SPW PLib Instance Object

   Summary:
    SPW PLib Object structure.

   Description:
    This data structure defines the SPW PLib Instance Object.

   Remarks:
    None.
*/
typedef struct
{
    /* Transfer Event Callback for interrupt*/
    SPW_CALLBACK callback;

    /* Transfer Event Callback Context for interrupt*/
    uintptr_t context;
} SPW_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*
 * The following functions make up the methods (set of possible operations) of
 * this interface.
 */

void SPW_Initialize(void);

void SPW_CallbackRegister(SPW_CALLBACK callback, uintptr_t contextHandle);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_SPW_H */
