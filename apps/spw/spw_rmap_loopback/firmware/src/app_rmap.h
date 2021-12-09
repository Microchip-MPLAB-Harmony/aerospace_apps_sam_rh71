/*******************************************************************************
  MPLAB Harmony Application RMAP Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_rmap.h

  Summary:
    RMAP Prototypes and definitions file.

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

#ifndef APP_RMAP_H
#define	APP_RMAP_H

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
/* RMAP Protocol ID */
#define APP_RMAP_PROTOCOL_ID                                      (0x01)

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
/* RMAP packet type

   Summary:
    RMAP packet type identifier.

   Description:
    This data type defines the different RMAP packet type.

   Remarks:
    None.
*/
typedef enum {
    APP_RMAP_PCK_TYPE_REPLY = 0,
    APP_RMAP_PCK_TYPE_COMMAND = 1,
} APP_RMAP_PCK_TYPE;

// *****************************************************************************
/* RMAP command code

   Summary:
    RMAP command code identifier.

   Description:
    This data type defines the different RMAP command code.

   Remarks:
    None.
*/
typedef enum {
    /* Read incrementing addresses */
    APP_RMAP_COMMAND_CODE_READ_INC = 0x03,
    /* Write, incrementing addresses, don’t verify before writing, no reply */
    APP_RMAP_COMMAND_CODE_WRITE_INC = 0x09,
    /* Write, incrementing addresses, don’t verify before writing, send reply */
    APP_RMAP_COMMAND_CODE_WRITE_INC_REPLY = 0x0B,
} APP_RMAP_COMMAND_CODE;

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

uint8_t APP_RMAP_GenerateHeader( const uint8_t* headerBufferAddress,
                               uint8_t targetLogicalAddr,
                               APP_RMAP_PCK_TYPE packetType,
                               APP_RMAP_COMMAND_CODE commandCode,
                               uint8_t key,
                               const uint8_t *replyAddrTablePtr,
                               uint8_t replyAddrTableSize,
                               uint8_t initiatorLogicalAddr,
                               uint16_t transactionId,
                               uint8_t extendedMemAddr,
                               uint32_t MemAddr,
                               uint32_t dataLength );

uint8_t APP_RMAP_GenerateInstructionByte(APP_RMAP_PCK_TYPE packetType, APP_RMAP_COMMAND_CODE commandCode, uint8_t replyAddrLength);

APP_RMAP_PCK_TYPE APP_RMAP_PackeTypeGet(uint8_t instuctionByte);

APP_RMAP_COMMAND_CODE APP_RMAP_CommandCodeGet(uint8_t instuctionByte);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif	/* APP_RMAP_H */

