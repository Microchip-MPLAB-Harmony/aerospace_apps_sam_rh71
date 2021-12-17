/*******************************************************************************
  MIL-1553 Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_ip1553.c

  Summary:
    IP1553 PLIB Implementation file

  Description:
    This file defines the interface to the IP1553 peripheral library.
    This library provides access to and control of the associated peripheral
    instance.

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>
#include "device.h"
#include "plib_ip1553.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

static IP1553_OBJ ip1553Obj;

// *****************************************************************************
// *****************************************************************************
// IP1553 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/* Function:
    void IP1553_Initialize(void)

   Summary:
    Initializes given instance of the IP1553 peripheral.
    Set the Mode (Bus Controller or Remote Terminal) and reset the instance.
    In RT mode, set the RT Address.
    Enable all 1553 interrupt sources.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void IP1553_Initialize(void)
{
    // Select BC mode and reset instance
    IP1553_REGS->IP1553_CR = IP1553_CR_PT(1) | IP1553_CR_RST(1);
}

// *****************************************************************************
/* Function:
    void IP1553_BuffersConfigSet(uint16_t* txBuffers, uint16_t* rxBuffers)

   Summary:
    Set the memory base address for emmission and reception buffers.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    txBuffers - Pointer to application allocated emission buffer base address.
                Application must allocate buffer from non-cached
                contiguous memory and buffer size must be
                16 bit * IP1553_BUFFERS_SIZE * IP1553_BUFFERS_NUM
    rxBuffers - Pointer to application allocated reception buffer base address.
                Application must allocate buffer from non-cached
                contiguous memory and buffer size must be
                16 bit * IP1553_BUFFERS_SIZE * IP1553_BUFFERS_NUM

   Returns:
    None.
*/
void IP1553_BuffersConfigSet(uint16_t* txBuffers, uint16_t* rxBuffers)
{
    IP1553_REGS->IP1553_ARW = IP1553_ARW_REG_ADDR_APB_W((uint32_t) txBuffers);
    IP1553_REGS->IP1553_ARR = IP1553_ARR_REG_ADDR_APB_R((uint32_t) rxBuffers);
}

// *****************************************************************************
/* Function:
    uint32_t IP1553_GetTxBuffersStatus(void)

   Summary:
    Returns the transmission buffers status.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    None.

   Returns:
    Bitfield value that indicates for each of the 32 buffers if they are
    ready to be sent (1) or are empty (0).
*/
uint32_t IP1553_GetTxBuffersStatus(void)
{
    return IP1553_REGS->IP1553_TXBSR;
}

// *****************************************************************************
/* Function:
    void IP1553_ResetTxBuffersStatus(uint32_t buffers)

   Summary:
    Reset the transmission buffers status.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    buffers  - Bitfield of buffer to be reset.
               When reset a buffer is set ready to be sent (1) in status.

   Returns:
    None.
*/
void IP1553_ResetTxBuffersStatus(uint32_t buffers)
{
    IP1553_REGS->IP1553_TXBSR = buffers;
}

// *****************************************************************************
/* Function:
    uint32_t IP1553_GetRxBuffersStatus(void)

   Summary:
    Returns the reception buffers status.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    None.

   Returns:
    Bitfield value that indicates for each of the 32 buffers if they are
    free to receive data or not : empty (1) or full(0).
*/
uint32_t IP1553_GetRxBuffersStatus(void)
{
    return IP1553_REGS->IP1553_RXBSR;
}

// *****************************************************************************
/* Function:
    void IP1553_ResetRxBuffersStatus(uint32_t buffers)

   Summary:
    Reset the reception buffers status.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    buffers  - Bitfield of buffer to be reset.
               When reset a buffer is set ready to receive data (1) in status.

   Returns:
    None.
*/
void IP1553_ResetRxBuffersStatus(uint32_t buffers)
{
    IP1553_REGS->IP1553_RXBSR = buffers;
}

// *****************************************************************************
/* Function:
    IP1553_INT_MASK IP1553_IrqStatusGet( void )

   Summary:
    Returns the IP1553 status.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    None.

   Returns:
    Current status of instance.
*/
IP1553_INT_MASK IP1553_IrqStatusGet( void )
{
    return (IP1553_INT_MASK)(IP1553_REGS->IP1553_ISR);
}

// *****************************************************************************
/* Function:
    void IP1553_BcStartDataTransfer(IP1553_DATA_TX_TYPE transferType, uint8_t txAddr, uint8_t txSubAddr, uint8_t rxAddr, uint8_t rxSubAddr, uint8_t dataWordCount, IP1553_BUS bus )

   Summary:
    Start BC command for data transfer.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.
    IP1553_BuffersConfigSet must have been called to set allocated buffers.
    IP1553_ResetRxBuffersStatus and IP1553_ResetTxBuffersStatus must 
    have been called for the concerned buffers (sub-address) used in the command.

   Parameters:
    transferType   - Type of data tranfer command to issue.
    txAddr         - The transmitter address : 0 if BC, RT address otherwise.
    txSubAddr      - The transmitter sub-address.
    rxAddr         - The receiver address : 0 if BC, RT address otherwise.
    rxSubAddr      - The receiver sub-address.
    dataWordCount  - Number of data word (16 bit) to read/write. 0 stand for 32 data word.
    bus            - Indicate if the transfer uses physical BUS A or B. 

   Returns:
    None.
*/
void IP1553_BcStartDataTransfer(IP1553_DATA_TX_TYPE transferType, uint8_t txAddr, uint8_t txSubAddr, uint8_t rxAddr, uint8_t rxSubAddr, uint8_t dataWordCount, IP1553_BUS bus )
{
    // Receiver terminal
    IP1553_REGS->IP1553_CMDR1 = \
            IP1553_CMDR1_RTADDRESS(rxAddr) | \
            IP1553_CMDR1_RTSUBADDRESS(rxSubAddr) | \
            IP1553_CMDR1_DATAWORDCOUNT(dataWordCount) ;

    // Transmitter terminal
    IP1553_REGS->IP1553_CMDR2 = \
            IP1553_CMDR2_RTADDRESS(txAddr) | \
            IP1553_CMDR2_T_R(1) | \
            IP1553_CMDR2_RTSUBADDRESS(txSubAddr) | \
            IP1553_CMDR2_DATAWORDCOUNT(dataWordCount) ;

    uint32_t cmdr3 = IP1553_CMDR3_BUS(bus);
    if (transferType == IP1553_DATA_TX_TYPE_BC_TO_RT)
    {
        cmdr3 |= IP1553_CMDR3_BCE(1);
    }
    else if (transferType == IP1553_DATA_TX_TYPE_RT_TO_BC)
    {
        cmdr3 |= IP1553_CMDR3_BCR(1);
    }
    IP1553_REGS->IP1553_CMDR3 = cmdr3;
}

// *****************************************************************************
/* Function:
    void IP1553_BcModeCommandTransfer(uint8_t rtAddr, IP1553_MODE_CMD modeCommand, uint16_t cmdParameter, IP1553_BUS bus)

   Summary:
    Start BC mode command transfer.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.
    IP1553_BuffersConfigSet must have been called to set allocated buffers.

   Parameters:
    rtAddr         - The remote terminal address or 0x1F for broadcast.
    modeCommand    - The mode command code.
    cmdParameter   - Optional command parameter for applicable commande code.
    bus            - Indicate if the transfer uses physical BUS A or B. 

   Returns:
    None.
*/
void IP1553_BcModeCommandTransfer(uint8_t rtAddr, IP1553_MODE_CMD modeCommand, uint16_t cmdParameter, IP1553_BUS bus)
{
    uint8_t cmdr1Tr = 1;
    uint32_t cmdr2 = 0;

    /* Check if mode command contains data parameter */
    if ( modeCommand == IP1553_MODE_CMD_SYNCHRONIZE_WITH_DATA )
    {
        cmdr1Tr = 0;
        cmdr2 = cmdParameter;
    }

    // Mode command
    IP1553_REGS->IP1553_CMDR1 = \
            IP1553_CMDR1_RTADDRESS(rtAddr) | \
            IP1553_CMDR1_RTSUBADDRESS(0) | \
            IP1553_CMDR1_T_R(cmdr1Tr) | \
            IP1553_CMDR1_DATAWORDCOUNT(modeCommand) ;

    // Parameter for applicable commands
    IP1553_REGS->IP1553_CMDR2 = cmdr2;

    IP1553_REGS->IP1553_CMDR3 = IP1553_CMDR3_BUS(bus) | IP1553_CMDR3_ER(1);
}
// *****************************************************************************
/* Function:
    uint16_t IP1553_GetFirstStatusWord( void )

   Summary:
    Returns the IP1553 transfer first status word.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    None.

   Returns:
    Value of transfer first status word.
*/
uint16_t IP1553_GetFirstStatusWord( void )
{
    return ( ( IP1553_REGS->IP1553_CTRL1 & IP1553_CTRL1_IP1553DATA1_Msk ) >> IP1553_CTRL1_IP1553DATA1_Pos );
}

// *****************************************************************************
/* Function:
    uint16_t IP1553_GetSecondStatusWord( void )

   Summary:
    Returns the IP1553 transfer second status word.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    None.

   Returns:
    Value of transfer second status word.
*/
uint16_t IP1553_GetSecondStatusWord( void )
{
    return ( ( IP1553_REGS->IP1553_CTRL1 & IP1553_CTRL1_IP1553DATA2_Msk) >> IP1553_CTRL1_IP1553DATA2_Pos );
}

// *****************************************************************************
/* Function:
    void IP1553_CallbackRegister(IP1553_CALLBACK callback,
                                                              uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given 1553's interrupt events occur.

  Description:
    This function sets the pointer to a client function to be called "back" when
    the given 1553's interrupt events occur. It also passes a context value
    (usually a pointer to a context structure) that is passed into the function
    when it is called. The specified callback function will be called from the
    peripheral interrupt context.

  Precondition:
    IP1553_Initialize must have been called for the associated
    1553 instance.

  Parameters:
    callback      - A pointer to a function with a calling signature defined by
                    the IP1553_CALLBACK data type. Setting this to NULL
                    disables the callback feature.

    contextHandle - A value (usually a pointer) passed (unused) into the
                    function identified by the callback parameter.

  Returns:
    None.

  Example:
    <code>
        // Refer to the description of the IP1553_CALLBACK data type for
        // example usage.
    </code>

  Remarks:
    None.
*/
void IP1553_CallbackRegister(IP1553_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    ip1553Obj.callback = callback;
    ip1553Obj.context = contextHandle;
}

// *****************************************************************************
/* Function:
    void IP1553_InterruptHandler(void)

   Summary:
    IP1553 Peripheral Interrupt Handler.

   Description:
    This function is IP1553 Peripheral Interrupt Handler and will
    called on every IP1553 interrupt.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    The function is called as peripheral instance's interrupt handler if the
    instance interrupt is enabled. If peripheral instance's interrupt is not
    enabled user need to call it from the main while loop of the application.
*/
void IP1553_InterruptHandler(void)
{

    if (ip1553Obj.callback != NULL)
    {
        ip1553Obj.callback(ip1553Obj.context);
    }
}

// *****************************************************************************
/* Function:
    void IP1553_InterruptEnable(IP1553_INT_MASK interruptMask)

   Summary:
    Enables IP1553 module Interrupt.

   Precondition:
    IP1553_Initialize must have been called for the associated IP1553 instance.

   Parameters:
    interruptMask - Interrupt to be enabled

   Returns:
    None
*/
void IP1553_InterruptEnable(IP1553_INT_MASK interruptMask)
{
    IP1553_REGS->IP1553_IER = interruptMask;
}

// *****************************************************************************
/* Function:
    void IP1553_InterruptDisable(IP1553_INT_MASK interruptMask)

   Summary:
    Disables IP1553 module Interrupt.

   Precondition:
    IP1553_Initialize must have been called for the associated IP1553 instance.

   Parameters:
    interruptMask - Interrupt to be disabled

   Returns:
    None
*/
void IP1553_InterruptDisable(IP1553_INT_MASK interruptMask)
{
    IP1553_REGS->IP1553_IDR = interruptMask;
}
