/*******************************************************************************
  SPW Peripheral Library
  Instance Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw_pktrx.h

  Summary:
    SPW PLIB PKTRX module Header file

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

#ifndef PLIB_SPW_PKTRX_H
#define PLIB_SPW_PKTRX_H

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
/* Return count field value in SPW PKTRX previous status word. This is the number of packets received in previous buffer. */
#define SPW_PKTRX_PREV_STATUS_GET_COUNT(status) ( (status & SPW_PKTRX_PREV_STATUS_CNT_MASK) >> SPW_PKTRX1_PREVBUFSTS_CNT_Pos )

// *****************************************************************************
// *****************************************************************************
// Section: SPW Types
// *****************************************************************************
// *****************************************************************************
/* SPW PKTRX status
   Summary:
    Identifies the SPW PKTRX current status

   Description:
    This data type identifies the SPW PKTRX status
*/
typedef enum
{
    SPW_PKTRX_STATUS_COUNT_MASK = SPW_PKTRX1_STATUS_COUNT_Msk, 
    SPW_PKTRX_STATUS_PACKET = SPW_PKTRX1_STATUS_PACKET_Msk,
    SPW_PKTRX_STATUS_LOCKED = SPW_PKTRX1_STATUS_LOCKED_Msk,
    SPW_PKTRX_STATUS_ARM = SPW_PKTRX1_STATUS_ARM_Msk,
    SPW_PKTRX_STATUS_ACT = SPW_PKTRX1_STATUS_ACT_Msk,
    SPW_PKTRX_STATUS_PENDING = SPW_PKTRX1_STATUS_PENDING_Msk,
    SPW_PKTRX_STATUS_DEACT = SPW_PKTRX1_STATUS_DEACT_Msk,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_PKTRX_STATUS_INVALID = 0xFFFFFFFF
} SPW_PKTRX_STATUS;

// *****************************************************************************
/* SPW PKTRX previous buffer status
   Summary:
    Identifies the SPW PKTRX previous buffer status

   Description:
    This data type identifies the SPW PKTRX previous buffer
*/
typedef enum
{
    SPW_PKTRX_PREV_STATUS_CNT_MASK = SPW_PKTRX1_PREVBUFSTS_CNT_Msk,
    SPW_PKTRX_PREV_STATUS_EEP = SPW_PKTRX1_PREVBUFSTS_EEP_Msk,
    SPW_PKTRX_PREV_STATUS_FULLI = SPW_PKTRX1_PREVBUFSTS_FULLI_Msk,
    SPW_PKTRX_PREV_STATUS_FULLD = SPW_PKTRX1_PREVBUFSTS_FULLD_Msk,
    SPW_PKTRX_PREV_STATUS_DMAERR = SPW_PKTRX1_PREVBUFSTS_DMAERR_Msk,
    SPW_PKTRX_PREV_STATUS_LOCKED = SPW_PKTRX1_PREVBUFSTS_LOCKED_Msk,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_PKTRX_PREV_STATUS_INVALID = 0xFFFFFFFF
} SPW_PKTRX_PREV_STATUS;

// *****************************************************************************
/* SPW PKTRX interrupt status
   Summary:
    Identifies the SPW PKTRX current interrupt status

   Description:
    This data type identifies the SPW PKTRX interrupt status
*/
typedef enum
{
    SPW_PKTRX_INT_MASK_DEACT = SPW_PKTRX1_PI_R_DEACT_Msk,
    SPW_PKTRX_INT_MASK_EOP = SPW_PKTRX1_PI_R_EOP_Msk,
    SPW_PKTRX_INT_MASK_EEP = SPW_PKTRX1_PI_R_EEP_Msk,
    SPW_PKTRX_INT_MASK_DISCARD = SPW_PKTRX1_PI_R_DISCARD_Msk,
    SPW_PKTRX_INT_MASK_ACT = SPW_PKTRX1_PI_R_ACT_Msk,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_PKTRX_INT_MASK_INVALID = 0xFFFFFFFF
} SPW_PKTRX_INT_MASK;

// *****************************************************************************
/* SPW PKTRX next receive buffer start condition
   Summary:
    Identifies the SPW PKTRX next receive buffer start condition

   Description:
    This data type identifies the SPW PKTRX next receive buffer start condition
*/
typedef enum
{
    SPW_PKTRX_NXTBUF_START_EVENT = SPW_PKTRX1_NXTBUFCFG_START_STARTEVENT_Val, 
    SPW_PKTRX_NXTBUF_START_NOW = SPW_PKTRX1_NXTBUFCFG_START_STARTNOW_Val,
    SPW_PKTRX_NXTBUF_START_TCH1 = SPW_PKTRX1_NXTBUFCFG_START_STARTTCH1_Val,
    SPW_PKTRX_NXTBUF_START_LATER = SPW_PKTRX1_NXTBUFCFG_START_STARTLATER_Val,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_PKTRX_NXTBUF_START_INVALID = 0xFFFFFFFF
} SPW_PKTRX_NXTBUF_START;

// *****************************************************************************
/* SPW PKTRX packet information data

   Summary:
    SPW received packet information data.

   Description:
    This data structure defines the received SPW packet information data.

   Remarks:
    None.
*/
typedef struct
{
    /* Word 0 */
    uint32_t Eop       :  1;
    uint32_t Eep       :  1;
    uint32_t Cont      :  1;
    uint32_t Split     :  1;
    uint32_t NotUsed1  : 12;
    uint32_t Crc       :  8;
    uint32_t NotUsed0  :  8;

    /* Word 1 */
    uint32_t DAddr     : 32;

    /* Word 2 */
    uint32_t DSize     : 24;
    uint32_t NotUsed2  :  8;

    /* Word 3 */
    uint32_t Etime     : 19;
    uint32_t NotUsed3  : 13;
} SPW_PKTRX_INFO;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*
 * The following functions make up the methods (set of possible operations) of
 * this interface.
 */

SPW_PKTRX_STATUS SPW_PKTRX_StatusGet(void);

void SPW_PKTRX_SetDiscard(bool discard);

void SPW_PKTRX_SetNextBuffer(
    uint8_t* dataAddress,
    uint32_t dataLengthBytes,
    SPW_PKTRX_INFO* packetInfoAddress,
    uint16_t pckCount,
    bool split,
    SPW_PKTRX_NXTBUF_START startMode,
    uint8_t startValue);

SPW_PKTRX_INT_MASK SPW_PKTRX_IrqStatusGetMaskedAndClear(void);

void SPW_PKTRX_InterruptEnable(SPW_PKTRX_INT_MASK interruptMask);

void SPW_PKTRX_InterruptDisable(SPW_PKTRX_INT_MASK interruptMask);

void SPW_PKTRX_CurrentPacketAbort(void);

void SPW_PKTRX_CurrentPacketSplit(void);

SPW_PKTRX_PREV_STATUS SPW_PKTRX_GetPreviousBufferStatus(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_SPW_PKTRX_H */
