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

#include "device.h"
#include "plib_ip1553.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


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
    // Select RT mode and reset instance
    IP1553_REGS->IP1553_CR = IP1553_CR_PT(0) | IP1553_CR_TA(1) | IP1553_CR_RST(1);
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
    void IP1553_BCEnableCmdSet(bool enable)

   Summary:
    Enable BCEnableCmd bit to accepts or rejects the control when the terminal
    receives a valid Dynamic Bus Control mode command. This is the value indicated
    in the Dynamic Bus Control bit of the status word sent in response to the mode command.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    enable - If true, The terminal accepts the bus control. If false, the terminal reject the bus control.

   Returns:
    None
*/
void IP1553_BCEnableCmdSet(bool enable)
{
    uint32_t crReg = ( IP1553_REGS->IP1553_CR & ~IP1553_CR_BEC_Msk );
    if (enable == true)
        crReg |= IP1553_CR_BEC(1);
    IP1553_REGS->IP1553_CR = crReg;
    while (IP1553_REGS->IP1553_CR != crReg);
}

// *****************************************************************************
/* Function:
    void IP1553_SREQBitCmdSet(bool enable)

   Summary:
    Enable or Disable SREQBitCmd bit. Indicates the value of the Service Request
    bit to be returned in status word transfers.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    enable - If true, Service Request bit returned in status word transfers is 1. 0 if false.

   Returns:
    None
*/
void IP1553_SREQBitCmdSet(bool enable)
{
    uint32_t crReg = ( IP1553_REGS->IP1553_CR & ~IP1553_CR_SRC_Msk );
    if (enable == true)
        crReg |= IP1553_CR_SRC(1);
    IP1553_REGS->IP1553_CR = crReg;
    while (IP1553_REGS->IP1553_CR != crReg);
}

// *****************************************************************************
/* Function:
    void IP1553_BusyBitCmdSet(bool enable)

   Summary:
    Enable or Disable SREQBitCmd bit. Indicates the value of the busy bit to be
    returned in status word transfers. If enabled, Inhibits the transmission of
    the data words in response to a transmit command and its corresponding interrupt.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    enable - If true, busy bit returned in status word transfers is 1. 0 if false.

   Returns:
    None
*/
void IP1553_BusyBitCmdSet(bool enable)
{
    uint32_t crReg = ( IP1553_REGS->IP1553_CR & ~IP1553_CR_BC_Msk );
    if (enable == true)
        crReg |= IP1553_CR_BC(1);
    IP1553_REGS->IP1553_CR = crReg;
    while (IP1553_REGS->IP1553_CR != crReg);
}

// *****************************************************************************
/* Function:
    void IP1553_SSBitCmdSet(bool enable)

   Summary:
    Enable or Disable SSBitCmd bit. Indicates the value of the Subsystem Flag bit
    to be returned in status word transfers.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    enable - If true, Subsystem Flag bit returned in status word transfers is 1. 0 if false.

   Returns:
    None
*/
void IP1553_SSBitCmdSet(bool enable)
{
    uint32_t crReg = ( IP1553_REGS->IP1553_CR & ~IP1553_CR_SC_Msk );
    if (enable == true)
        crReg |= IP1553_CR_SC(1);
    IP1553_REGS->IP1553_CR = crReg;
    while (IP1553_REGS->IP1553_CR != crReg);
}

// *****************************************************************************
/* Function:
    void IP1553_TRBitCmdSet(bool enable)

   Summary:
    Enable or Disable TRBitCmd bit. Indicates the value of the T/F bit to be
    returned in status word transfers.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    enable - If true, T/F bit returned in status word transfers is 1. 0 if false.

   Returns:
    None

   Note:
    After reception of a valid Inhibit T/F Bit command the T/F bit is maintained
    at logic level 0.
*/
void IP1553_TRBitCmdSet(bool enable)
{
    uint32_t crReg = ( IP1553_REGS->IP1553_CR & ~IP1553_CR_TC_Msk );
    if (enable == true)
        crReg |= IP1553_CR_TC(1);
    IP1553_REGS->IP1553_CR = crReg;
    while (IP1553_REGS->IP1553_CR != crReg);
}

// *****************************************************************************
/* Function:
    void IP1553_BitWordSet(uint16_t bitWord)

   Summary:
    Set the built-in self test results in BIT register. This value is sent by
    the terminal in response to a "Transmit Built-In Test".

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    bitWord - Built-in self test results value.

   Returns:
    None
*/
void IP1553_BitWordSet(uint16_t bitWord)
{
    IP1553_REGS->IP1553_BITR = ( bitWord & 0xFFFF );
}

// *****************************************************************************
/* Function:
    void IP1553_VectorWordSet(uint16_t vectorWord)

   Summary:
    Set the Vector Word value to be sent by the terminal in response to
    a "Transmit Vector Word" command.

   Precondition:
    IP1553_Initialize must have been called for the IP1553 instance.

   Parameters:
    vectorWord - Vector Word value to be sent by the terminal.

   Returns:
    None
*/
void IP1553_VectorWordSet(uint16_t vectorWord)
{
    IP1553_REGS->IP1553_VWR = ( vectorWord & 0xFFFF );
}

