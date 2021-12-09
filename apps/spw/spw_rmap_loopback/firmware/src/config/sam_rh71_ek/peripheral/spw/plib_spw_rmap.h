/*******************************************************************************
  SPW Peripheral Library
  Instance Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw_rmap.h

  Summary:
    SPW PLIB RMAP Header file

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

#ifndef PLIB_SPW_RMAP_H
#define PLIB_SPW_RMAP_H

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
/* Return SPW RMAP Error Code from RMAP status word. */
#define SPW_RMAP_STATUS_GET_ERRCODE(status) ( (SPW_RMAP_ERRCODE)( (status & SPW_RMAP_STATUS_ERRCODE_MASK) >> SPW_RMAP1_STS_RC_ERRCODE_Pos ) )

// *****************************************************************************
// *****************************************************************************
// Section: SPW Types
// *****************************************************************************
// *****************************************************************************
/* SPW RMAP status
   Summary:
    Identifies the SPW RMAP current status

   Description:
    This data type identifies the SPW RMAP status
*/
typedef enum
{
    SPW_RMAP_STATUS_ERRCODE_MASK = SPW_RMAP1_STS_RC_ERRCODE_Msk, 
    SPW_RMAP_STATUS_VALID = SPW_RMAP1_STS_RC_VALID_Msk,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_RMAP_STATUS_INVALID = 0xFFFFFFFF
} SPW_RMAP_STATUS;

// *****************************************************************************
/* SPW RMAP Error Code
   Summary:
    Identifies the SPW RMAP Error Code

   Description:
    This data type identifies the SPW RMAP Error Code
*/
typedef enum
{
    /* No error detected */
    SPW_RMAP_ERRCODE_NOERROR = SPW_RMAP1_STS_RC_ERRCODE_NOERROR_Val,
    /* Error while DMA accessing the internal bus, e.g. illegal address. */
    SPW_RMAP_ERRCODE_DMAERROR = SPW_RMAP1_STS_RC_ERRCODE_DMAERROR_Val,
    /* Unused RMAP command according to [RMAP] */
    SPW_RMAP_ERRCODE_RMAPERROR = SPW_RMAP1_STS_RC_ERRCODE_RMAPERROR_Val,
    /* Destination key error */
    SPW_RMAP_ERRCODE_DESTKEYERROR = SPW_RMAP1_STS_RC_ERRCODE_DESTKEYERROR_Val,
    /* Data CRC error */
    SPW_RMAP_ERRCODE_DATACRCERROR = SPW_RMAP1_STS_RC_ERRCODE_DATACRCERROR_Val,
    /* Early EOP in header or data, i.e. EOP has been received with less data than expected from the RMAP command header. */
    SPW_RMAP_ERRCODE_EOPERROR = SPW_RMAP1_STS_RC_ERRCODE_EOPERROR_Val,
    /* Cargo too large. Late EOP or EEP in data, i.e. EOP/EEP has been received with more data than expected from the RMAP command header. */
    SPW_RMAP_ERRCODE_CARGOERROR = SPW_RMAP1_STS_RC_ERRCODE_CARGOERROR_Val,
    /* Early EEP in data for RMAP commands. EEP has been received with less data or exactly as much as expected from the RMAP command header. */
    SPW_RMAP_ERRCODE_EEPERROR = SPW_RMAP1_STS_RC_ERRCODE_EEPERROR_Val,
    /* Authorisation error:Invalid or unsupported command. */
    SPW_RMAP_ERRCODE_CMDERROR = SPW_RMAP1_STS_RC_ERRCODE_CMDERROR_Val,
    /* Non-matching Target Logical Address. */
    SPW_RMAP_ERRCODE_TLAERROR = SPW_RMAP1_STS_RC_ERRCODE_TLAERROR_Val,
    /* Incorrect header CRC. */
    SPW_RMAP_ERRCODE_HEADERCRCERROR = SPW_RMAP1_STS_RC_ERRCODE_HEADERCRCERROR_Val,
    /* Protocol Identifier not supported. */
    SPW_RMAP_ERRCODE_PROTOCOLIDERROR = SPW_RMAP1_STS_RC_ERRCODE_PROTOCOLIDERROR_Val,
    /* Unsupported reply address length */
    SPW_RMAP_ERRCODE_REPLYADDERROR = SPW_RMAP1_STS_RC_ERRCODE_REPLYADDERROR_Val,
    /* Force the compiler to reserve 32-bit memory for enum */
    SPW_RMAP_ERRCODE_INVALID = 0xFFFFFFFF
} SPW_RMAP_ERRCODE;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*
 * The following functions make up the methods (set of possible operations) of
 * this interface.
 */

void SPW_RMAP_Initialize(void);

SPW_RMAP_STATUS SPW_RMAP_StatusGetAndClear(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_SPW_RMAP_H */
