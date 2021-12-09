/*******************************************************************************
  SPW Peripheral Library
  Instance Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw_pkttx.h

  Summary:
    SPW PLIB PKTTX module Header file

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

#ifndef PLIB_SPW_PKTTX_H
#define PLIB_SPW_PKTTX_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include <stdbool.h>
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
/* Return PREV field value in SPW PKTTX status word. */
#define SPW_PKTTX_STATUS_PREV_GET(status) ( (SPW_PKTTX_PREV)( (status & SPW_PKTTX_STATUS_PREV_MASK) >> SPW_PKTTX1_STATUS_PREV_Pos ) )

// *****************************************************************************
// *****************************************************************************
// Section: SPW Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* SPW PKTTX status
   Summary:
    Identifies the SPW PKTTX current status

   Description:
    This data type identifies the SPW PKTTX status
*/
typedef enum
{
    SPW_PKTTX_STATUS_ARM = SPW_PKTTX1_STATUS_ARM_Msk, 
    SPW_PKTTX_STATUS_ACT = SPW_PKTTX1_STATUS_ACT_Msk,
    SPW_PKTTX_STATUS_PENDING = SPW_PKTTX1_STATUS_PENDING_Msk,
    SPW_PKTTX_STATUS_DEACT = SPW_PKTTX1_STATUS_DEACT_Msk,
    SPW_PKTTX_STATUS_PREV_MASK = SPW_PKTTX1_STATUS_PREV_Msk,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_PKTTX_STATUS_INVALID = 0xFFFFFFFF
} SPW_PKTTX_STATUS;

// *****************************************************************************
/* SPW PKTTX Previous status
   Summary:
    Identifies the SPW PKTTX Previous status.

   Description:
    This data type identifies the PKTTX Previous status.
*/
typedef enum
{
    SPW_PKTTX_PREV_NOINFO = 0,
    SPW_PKTTX_PREV_LASTSENDLISTOK,
    SPW_PKTTX_PREV_ABORTEDMEMERR,
    SPW_PKTTX_PREV_ABORTEDNEWSD,
    SPW_PKTTX_PREV_ABORTEDUSERCMD,
    SPW_PKTTX_PREV_ABORTEDTIMEOUT,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_PKTTX_PREV_INVALID = 0xFFFFFFFF
}SPW_PKTTX_PREV;

// *****************************************************************************
/* SPW PKTTX interrupt status
   Summary:
    Identifies the SPW PKTTX current interrupt status

   Description:
    This data type identifies the SPW PKTTX interrupt status
*/
typedef enum
{
    SPW_PKTTX_INT_MASK_DEACT = SPW_PKTTX1_PI_R_DEACT_Msk,
    SPW_PKTTX_INT_MASK_ACT = SPW_PKTTX1_PI_R_ACT_Msk,
    SPW_PKTTX_INT_MASK_EOP = SPW_PKTTX1_PI_R_EOP_Msk,
    SPW_PKTTX_INT_MASK_EEP = SPW_PKTTX1_PI_R_EEP_Msk,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_PKTTX_INT_MASK_INVALID = 0xFFFFFFFF
}SPW_PKTTX_INT_MASK;

// *****************************************************************************
/* SPW PKTTX next send list start mode
   Summary:
    Identifies the SPW PKTTX next send list start mode

   Description:
    This data type identifies the SPW PKTTX next send list start mode
*/
typedef enum
{
    SPW_PKTTX_NXTSEND_START_EVENT = SPW_PKTTX1_NXTSENDCFG_START_STARTEVENT_Val, 
    SPW_PKTTX_NXTSEND_START_NOW = SPW_PKTTX1_NXTSENDCFG_START_STARTNOW_Val,
    SPW_PKTTX_NXTSEND_START_TCH1 = SPW_PKTTX1_NXTSENDCFG_START_STARTTCH1_Val,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_PKTTX_NXTSEND_START_INVALID = 0xFFFFFFFF
}SPW_PKTTX_NXTSEND_START;

// *****************************************************************************
/* SPW PKTTX send list entry

   Summary:
    SPW send list entry.

   Description:
    This data structure defines a SPW send list entry.

   Remarks:
    None.
*/
typedef struct
{
    /* Word 0 */
    uint32_t Start      : 19;
    uint32_t NotUsed1   :  5;
    uint32_t RSize      :  4;
    uint32_t NotUsed0   :  1;
    uint32_t Entry      :  2;
    uint32_t Skip       :  1;

    /* Word 1 */
    uint32_t RB4        :  8;
    uint32_t RB3        :  8;
    uint32_t RB2        :  8;
    uint32_t RB1        :  8;

    /* Word 2 */
    uint32_t RB8        :  8;
    uint32_t RB7        :  8;
    uint32_t RB6        :  8;
    uint32_t RB5        :  8;

    /* Word 3 */
    uint32_t HSize      :  8;
    uint32_t HCrc       :  1;
    uint32_t NotUsed3   :  7;
    uint32_t EscChar    :  8;
    uint32_t EscMask    :  4;
    uint32_t NotUsed2   :  4;

    /* Word 4 */
    uint32_t HAddr      : 32;

    /* Word 5 */
    uint32_t DSize      : 24;
    uint32_t DCrc       :  1;
    uint32_t NotUsed4   :  7;

    /* Word 6 */
    uint32_t DAddr      : 32;

    /* Word 7 */    
    uint32_t TimeOut    : 19;
    uint32_t NotUsed5   : 13;
}SPW_PKTTX_SEND_LIST_ENTRY;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*
 * The following functions make up the methods (set of possible operations) of
 * this interface.
 */
SPW_PKTTX_STATUS SPW_PKTTX_StatusGet(void);

SPW_PKTTX_INT_MASK SPW_PKTTX_IrqStatusGetMaskedAndClear(void);

void SPW_PKTTX_InterruptEnable(SPW_PKTTX_INT_MASK interruptMask);

void SPW_PKTTX_InterruptDisable(SPW_PKTTX_INT_MASK interruptMask);

void SPW_PKTTX_SetNextSendList(uint8_t* routerBytesTable,
                               SPW_PKTTX_SEND_LIST_ENTRY* sendListAddress,
                               uint16_t length,
                               bool abort,
                               SPW_PKTTX_NXTSEND_START startMode,
                               uint8_t startValue);

void SPW_PKTTX_UnlockStatus(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_SPW_PKTTX_H */
