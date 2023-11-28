/*******************************************************************************
  MIL-STD-1553 Library
  Instance Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_ip1553.h

  Summary:
    IP1553 PLIB Header file

  Description:
    This file defines the interface to the IP1553 peripheral 
    library. This library provides access to and control of the associated 
    peripheral instance.
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018-2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_IP1553_H
#define PLIB_IP1553_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: IP1553 defines
// *****************************************************************************
// *****************************************************************************
/* Number of IP1553 buffers */
#define IP1553_BUFFERS_NUM                               (32UL)

/* Size in number of 16 bit word in one buffers */
#define IP1553_BUFFERS_SIZE                              (32UL)

/* Return 32 bit bitfield with bit value to 1 for the corresponding x buffer. */
#define IP1553_BUFFER_TO_BITFIELD_SA(x)                  (1UL << (x))

/* IP1553 RT address value for broadcast mode */
#define IP1553_RT_ADDRESS_BROADCAST_MODE                 (0x1FU)

/* Return ETRANS field value in IP1553 interrupt mask. */
#define IP1553_INT_MASK_GET_ETRANS(status)               (uint8_t)( ((status) & IP1553_INT_MASK_ETRANS_MASK) >> IP1553_ISR_ETRANS_Pos ) 

// *****************************************************************************
// *****************************************************************************
// Section: IP1553 Types
// *****************************************************************************
// *****************************************************************************
/* IP1553 Interrupt Status
   Summary:
    Identifies the IP1553 current interrupt status.

   Description:
    This data type identifies the IP1553 interrupt status.
*/
typedef uint32_t IP1553_INT_MASK;

#define IP1553_INT_MASK_EMT             ( IP1553_ISR_EMT_Msk )
#define IP1553_INT_MASK_MTE             ( IP1553_ISR_MTE_Msk )
#define IP1553_INT_MASK_ERX             ( IP1553_ISR_ERX_Msk )
#define IP1553_INT_MASK_ETX             ( IP1553_ISR_ETX_Msk )
#define IP1553_INT_MASK_ETRANS_MASK     ( IP1553_ISR_ETRANS_Msk )
#define IP1553_INT_MASK_TE              ( IP1553_ISR_TE_Msk )
#define IP1553_INT_MASK_TCE             ( IP1553_ISR_TCE_Msk )
#define IP1553_INT_MASK_TPE             ( IP1553_ISR_TPE_Msk )
#define IP1553_INT_MASK_TDE             ( IP1553_ISR_TDE_Msk )
#define IP1553_INT_MASK_TTE             ( IP1553_ISR_TTE_Msk )
#define IP1553_INT_MASK_TWE             ( IP1553_ISR_TWE_Msk )
#define IP1553_INT_MASK_BE              ( IP1553_ISR_BE_Msk )
#define IP1553_INT_MASK_ITR             ( IP1553_ISR_ITR_Msk )
#define IP1553_INT_MASK_TVR             ( IP1553_ISR_TVR_Msk )
#define IP1553_INT_MASK_DBR             ( IP1553_ISR_DBR_Msk )
#define IP1553_INT_MASK_STR             ( IP1553_ISR_STR_Msk )
#define IP1553_INT_MASK_TSR             ( IP1553_ISR_TSR_Msk )
#define IP1553_INT_MASK_OSR             ( IP1553_ISR_OSR_Msk )
#define IP1553_INT_MASK_SDR             ( IP1553_ISR_SDR_Msk )
#define IP1553_INT_MASK_SWD             ( IP1553_ISR_SWD_Msk )
#define IP1553_INT_MASK_RRT             ( IP1553_ISR_RRT_Msk )
#define IP1553_INT_MASK_ITF             ( IP1553_ISR_ITF_Msk )
#define IP1553_INT_MASK_OTF             ( IP1553_ISR_OTF_Msk )
#define IP1553_INT_MASK_IPB             ( IP1553_ISR_IPB_Msk )
#define IP1553_INT_MASK_ERROR_MASK      ( IP1553_INT_MASK_MTE | \
                                          IP1553_INT_MASK_TE  | \
                                          IP1553_INT_MASK_TCE | \
                                          IP1553_INT_MASK_TPE | \
                                          IP1553_INT_MASK_TDE | \
                                          IP1553_INT_MASK_TTE | \
                                          IP1553_INT_MASK_TWE | \
                                          IP1553_INT_MASK_BE  | \
                                          IP1553_INT_MASK_ITR )
#define IP1553_INT_MASK_INVALID         ( 0xFFFFFFFFUL )

// *****************************************************************************
/* IP1553 Callback

   Summary:
    IP1553 Callback Function Pointer.

   Description:
    This data type defines the IP1553 Callback Function Pointer.

   Remarks:
    None.
*/
typedef void (*IP1553_CALLBACK) (uintptr_t contextHandle);

// *****************************************************************************
/* IP1553 PLib Instance Object

   Summary:
    IP1553 PLib Object structure.

   Description:
    This data structure defines the IP1553 PLib Instance Object.

   Remarks:
    None.
*/
typedef struct
{
    /* Transfer Event Callback */
    IP1553_CALLBACK callback;

    /* Transfer Event Callback Context */
    uintptr_t context;
} IP1553_OBJ;

// *****************************************************************************
/* IP1553 Bus Controller data transfer type

   Summary:
    IP1553 data transfer types.

   Description:
    This data type identifies the data transfer type that BC initiate.

   Remarks:
    None.
*/
typedef enum
{
    IP1553_DATA_TX_TYPE_BC_TO_RT = 0U,
    IP1553_DATA_TX_TYPE_RT_TO_BC,
    IP1553_DATA_TX_TYPE_RT_TO_RT,
} IP1553_DATA_TX_TYPE;

// *****************************************************************************
/* IP1553 Output Bus Selection

   Summary:
    IP1553 output bus selection.

   Description:
    his data type identifies the IP1553 output bus selection.

   Remarks:
    None.
*/
typedef enum
{
    IP1553_BUS_A = 0U,
    IP1553_BUS_B
} IP1553_BUS;

/* MISRA C-2012 Rule 5.2 is deviated in the below code block. Deviation record ID - H3_MISRAC_2012_R_5_2_DR_1*/

// *****************************************************************************
/* IP1553 Bus Controller Mode Command code

   Summary:
    IP1553 Mode Command code.

   Description:
    This data type identifies the 1553 protocol Mode Command code values.

   Remarks:
    None.
*/
typedef enum
{
    IP1553_MODE_CMD_DYNAMIC_BUS_CONTROL = 0U,
    IP1553_MODE_CMD_SYNCHRONIZE_WITHOUT_DATA = 1U,
    IP1553_MODE_CMD_TRANSMIT_STATUS_WORD = 2U,
    IP1553_MODE_CMD_INITIATE_SELF_TEST = 3U,
    IP1553_MODE_CMD_TRANSMITTER_SHUTDOWN = 4U,
    IP1553_MODE_CMD_OVERRIDE_TRANSMITTER_SHUTDOWN = 5U,
    IP1553_MODE_CMD_INHIBIT_TERMINAL_FLAG_BIT = 6U,
    IP1553_MODE_CMD_OVERRIDE_INHIBIT_TERMINAL_FLAG_BIT = 7U,
    IP1553_MODE_CMD_RESET_REMOTE_TERMINAL = 8U,
    IP1553_MODE_CMD_TRANSMIT_VECTOR_WORD = 16U,
    IP1553_MODE_CMD_SYNCHRONIZE_WITH_DATA = 17U,
    IP1553_MODE_CMD_TRANSMIT_LAST_COMMAND = 18U,
    IP1553_MODE_CMD_TRANSMIT_BIT_WORD = 19U,
} IP1553_MODE_CMD;

/* MISRAC 2012 deviation block end */

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*
 * The following functions make up the methods (set of possible operations) of
 * this interface.
 */

void IP1553_Initialize(void);

void IP1553_BuffersConfigSet(uint16_t* txBuffers, uint16_t* rxBuffers);

uint32_t IP1553_GetTxBuffersStatus(void);

void IP1553_ResetTxBuffersStatus(uint32_t buffers);

uint32_t IP1553_GetRxBuffersStatus(void);

void IP1553_ResetRxBuffersStatus(uint32_t buffers);

IP1553_INT_MASK IP1553_IrqStatusGet( void );

void IP1553_BcStartDataTransfer(IP1553_DATA_TX_TYPE transferType, uint8_t txAddr, uint8_t txSubAddr, uint8_t rxAddr, uint8_t rxSubAddr, uint8_t dataWordCount, IP1553_BUS bus );

void IP1553_BcModeCommandTransfer(uint8_t rtAddr, IP1553_MODE_CMD modeCommand, uint16_t cmdParameter, IP1553_BUS bus);
uint16_t IP1553_GetFirstStatusWord( void );
uint16_t IP1553_GetSecondStatusWord( void );
void IP1553_CallbackRegister(IP1553_CALLBACK callback, uintptr_t contextHandle);

void IP1553_InterruptEnable(IP1553_INT_MASK interruptMask);

void IP1553_InterruptDisable(IP1553_INT_MASK interruptMask);
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_IP1553_H */
