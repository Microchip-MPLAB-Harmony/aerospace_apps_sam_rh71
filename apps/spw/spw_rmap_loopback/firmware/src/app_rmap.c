/*******************************************************************************
 MPLAB Harmony Application RMAP Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_rmap.c

  Summary:
    RMAP Implementation File

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
#include "app_rmap.h"

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
                                   uint32_t dataLength )

   Summary:
    Generate the RMAP header in the given buffer using the RMAP parameters.

   Description:
    This function generate the RMAP header for the defined RMAP parameters.
    It writes the generated header in the given input buffer.

   Precondition:
    None.

   Parameters:
    headerBufferAddr - pointer to memory buffer to fill with RMAP header data.
    targetLogicalAddr - Target logical address.
    packetType - RMAP packet type APP_RMAP_PCK_TYPE_REPLY or APP_RMAP_PCK_TYPE_COMMAND.
    commandCode - RMAP Command code.
    key - RMAP target destination Key.
    replyAddrTablePtr - RMAP reply address table of byte pointer.
    replyAddrTableSize - RMAP reply address table size.
    initiatorLogicalAddr - Initiator logical address.
    transactionId - Transaction ID
    extendedMemAddr - optional msb bits for 40-bits memory address.
    MemAddr - 32 lsb bits of memory address.
    dataLength - data length in bytes.

   Returns:
    Size of the generated header in bytes.
*/
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
                               uint32_t dataLength )
{
    uint8_t* header_ptr = (uint8_t*)headerBufferAddress;
    uint8_t header_size = 15;
    uint8_t reply_address_length = 0;

    if (replyAddrTableSize != 0)
    {
        reply_address_length = (replyAddrTableSize / 4) + 1;
    }
    header_size += reply_address_length * 4;

    // Target logical address
    *(header_ptr++) = targetLogicalAddr;
    // Protocol ID
    *(header_ptr++) = APP_RMAP_PROTOCOL_ID;
    // Instruction
    uint8_t instruction = APP_RMAP_GenerateInstructionByte(packetType, commandCode, reply_address_length);
    *(header_ptr++) = instruction;
    // Key
    *(header_ptr++) = key ;
    // Reply address
    if (replyAddrTableSize != 0)
    {
        // Fill unused with zero
        for (uint8_t i=0; i< ( (reply_address_length * 4) - replyAddrTableSize); i++)
        {
            *(header_ptr++) = 0;
        }
        for (uint8_t i=0; i<replyAddrTableSize; i++)
        {
            *(header_ptr++) = replyAddrTablePtr[i];
        }
    }
    // Initiator logical address
    *(header_ptr++) = initiatorLogicalAddr ;
    // Transaction Identifier
    *(header_ptr++) = (transactionId >> 8) & 0xFF;
    *(header_ptr++) = transactionId & 0xFF;
    // Extended Address
    *(header_ptr++) = extendedMemAddr;
    // Memory address
    *(header_ptr++) = (MemAddr >> 24) & 0xFF;
    *(header_ptr++) = (MemAddr >> 16) & 0xFF;
    *(header_ptr++) = (MemAddr >> 8) & 0xFF;
    *(header_ptr++) = MemAddr & 0xFF;
    // Data lenght
    *(header_ptr++) = (dataLength >> 16) & 0xFF;
    *(header_ptr++) = (dataLength >> 8) & 0xFF;
    *(header_ptr++) = dataLength & 0xFF;

    return header_size;
}

// *****************************************************************************
/* Function:
    uint8_t APP_RMAP_GenerateInstructionByte(APP_RMAP_PCK_TYPE packetType, APP_RMAP_COMMAND_CODE commandCode, uint8_t replyAddrLength)

   Summary:
    Generate the RMAP instruction byte.

   Description:
    This function generate the RMAP instruction byte using the given RMAP
    parameters encoded in this byte.

   Precondition:
    None.

   Parameters:
    packetType - RMAP packet type APP_RMAP_PCK_TYPE_REPLY or APP_RMAP_PCK_TYPE_COMMAND.
    commandCode - RMAP Command code.
    replyAddrLength - RMAP reply address length.

   Returns:
    Generated RMAP instruction byte.
*/
uint8_t APP_RMAP_GenerateInstructionByte(APP_RMAP_PCK_TYPE packetType, APP_RMAP_COMMAND_CODE commandCode, uint8_t replyAddrLength)
{
    return (packetType << 6) | (commandCode << 2) | replyAddrLength;
}

// *****************************************************************************
/* Function:
    APP_RMAP_PCK_TYPE RMAP_PackeTypeGet(uint8_t instuctionByte)

   Summary:
    Get the RMAP packet type from instruction byte.

   Description:
    This function return the RMAP packet type from instruction byte.

   Precondition:
    None.

   Parameters:
    instuctionByte - RMAP instruction byte.

   Returns:
    RMAP packet type.
*/
APP_RMAP_PCK_TYPE APP_RMAP_PackeTypeGet(uint8_t instuctionByte)
{
    return (APP_RMAP_PCK_TYPE)(instuctionByte >> 6);
}

// *****************************************************************************
/* Function:
    APP_RMAP_COMMAND_CODE RMAP_CommandCodeGet(uint8_t instuctionByte)

   Summary:
    Get the RMAP command code from instruction byte.

   Description:
    This function return the RMAP command code from instruction byte.

   Precondition:
    None.

   Parameters:
    instuctionByte - RMAP instruction byte.

   Returns:
    RMAP command code.
*/
APP_RMAP_COMMAND_CODE APP_RMAP_CommandCodeGet(uint8_t instuctionByte)
{
    return (APP_RMAP_COMMAND_CODE)((instuctionByte & 0x3C) >> 2);
}

