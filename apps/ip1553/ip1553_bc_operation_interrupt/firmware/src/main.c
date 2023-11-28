/*******************************************************************************
 Main Source File

 Company:
 Microchip Technology Inc.

 File Name:
 main.c

 Summary:
 This file contains the "main" function for a project.

 Description:
 This file contains the "main" function for a project.  The
 "main" function calls the "SYS_Initialize" function to initialize the state
 machines of all modules in the system
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
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <string.h>                     // Defines strings for memset
#include <errno.h>

// *****************************************************************************
// *****************************************************************************
// Section: Application defines
// *****************************************************************************
// *****************************************************************************
/* Define the bitfield mask of used 1553 buffers by the application */
#define APP_IP1553_BUFFER_USED              (IP1553_BUFFER_TO_BITFIELD_SA (0U) |\
                                             IP1553_BUFFER_TO_BITFIELD_SA (1U) |\
                                             IP1553_BUFFER_TO_BITFIELD_SA (2U) |\
                                             IP1553_BUFFER_TO_BITFIELD_SA (3U) |\
                                             IP1553_BUFFER_TO_BITFIELD_SA (31U) )

/* Define the BC address (0) */
#define APP_IP1553_BC_ADDR                  (0U)

/* Define the BC buffer number (or sub-address) used for receiving data from RT */
#define APP_IP1553_BC_BUFFER_RECV_SUB_ADDR  (3U)

/* Define the RT device address that will be used to send and received data */
#define APP_IP1553_RT_ADDR                  (1U)

/* Define the RT buffer number (or sub-address) used for receiving data from BC */
#define APP_IP1553_RT_BUFFER_RECV_SUB_ADDR  (1U)

/* Define the RT buffer number (or sub-address) used to send data to BC */
#define APP_IP1553_RT_BUFFER_SEND_SUB_ADDR  (3U)

/* Define the transfert size in number words (16 bit words)*/
#define APP_IP1553_TRANSFER_WORD_SIZE       (1U)

// *****************************************************************************
// *****************************************************************************
// Section: Globals
// *****************************************************************************
// *****************************************************************************
/* Allocation of receive buffer for IP1553 */
static uint16_t IP1553RxBuffersRAM[IP1553_BUFFERS_NUM][IP1553_BUFFERS_SIZE] __attribute__((aligned (32)))__attribute__((section (".ram_nocache")));

/* Allocation of transmit buffer for IP1553 */
static uint16_t IP1553TxBuffersRAM[IP1553_BUFFERS_NUM][IP1553_BUFFERS_SIZE] __attribute__((aligned (32)))__attribute__((section (".ram_nocache")));

/* Variable containing the next BC buffer number (or sub-address) used for transmitting data to RT */
static uint8_t gBcSubAddrData = 0U;

/* Variable containing the next BC buffer number (or sub-address) used for transmitting data to RT */
volatile static uint32_t activeRxBuffers = 0;

/* Boolean to check if the end of transfer interrupt occurred */
volatile static bool isTransferEnded = false;

/* Boolean to check if the status word interrupt occurred */
volatile static uint8_t hasStatusWord = 0;

/* Bit field of status containing the error interrupts that were triggered during transfer */
volatile static uint32_t transferErrors = 0;

// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************
static void print_menu(void)
{
    printf(" ------------------------------ \r\n");
    printf(" Press '1' to send one data from TX buffer %u to RT1, on Bus A \r\n", gBcSubAddrData);
    printf(" Press '2' to receive one data in buffer 3 from RT1, on bus A \r\n");
    printf(" Press '3' to broadcast for RTs, from buffer 1, on bus A \r\n");
    printf(" Press '4' to initiate mode command transfer to RT1 on bus A \r\n");
    printf(" Press '5' to initiate broadcast mode command transfer on bus A \r\n");
}

// *****************************************************************************
/* void APP_IP1553_Print_Errors(uint32_t errors)

 Summary:
 Function called by the application to print the description of the errors that
 occurred during transfer.

 Description:
 Print the description of the errors that are set in the given bit field.

 Parameters:
 errors - Bit field of the IP1553 status that contains errors.

 Remarks:
 None.
 */
static void APP_IP1553_Print_Errors(uint32_t errors)
{
    if ( (errors & IP1553_INT_MASK_MTE) == IP1553_INT_MASK_MTE )
    {
        printf("  Error :  R/W memory transfer error has occurred.\r\n");
    }
    if ( (errors & IP1553_INT_MASK_TE) == IP1553_INT_MASK_TE )
    {
        printf("  Error : Error has occurred during processing of the reception, transmission, or transfer.\r\n");
    }
    if ( (errors & IP1553_INT_MASK_TCE) == IP1553_INT_MASK_TCE )
    {
        printf("  Error : Manchester code error has been detected on a word that has been received.\r\n");
    }
    if ( (errors & IP1553_INT_MASK_TPE) == IP1553_INT_MASK_TPE )
    {
        printf("  Error : Parity error has been detected on a received word.\r\n");
    }
    if ( (errors & IP1553_INT_MASK_TDE) == IP1553_INT_MASK_TDE )
    {
        printf("  Error : Data word has been received when a command word was expected and vice-versa.\r\n");
    }
    if ( (errors & IP1553_INT_MASK_TTE) == IP1553_INT_MASK_TTE )
    {
        printf("  Error : Response time of the addressed terminal is greater than expected or that the response is missing.\r\n");
    }
    if ( (errors & IP1553_INT_MASK_TWE) == IP1553_INT_MASK_TWE )
    {
        printf("  Error : The number of words received does not correspond to the number of words expected.\r\n");
    }
    if ( (errors & IP1553_INT_MASK_BE) == IP1553_INT_MASK_BE )
    {
        printf("  Error : A data word transmission has been stopped because data have not been provided in time on the Buffer interface.\r\n");
    }
    if ( (errors & IP1553_INT_MASK_ITR) == IP1553_INT_MASK_ITR )
    {
        printf("  Error : The transfer which has been commanded via the command IF is not legal and will not be performed.\r\n");
    }
}

// *****************************************************************************
/* void APP_IP1553_print_mc_list(bool broadcastOnly)

 Summary:
   Print 1553 mode command list and their code value.

 Description:
   Print 1553 mode command list and their code value.

 Parameters:
   broadcastOnly - Display only command values that are applicable to broadcast.

 Remarks:
   None.
 */
static void APP_IP1553_print_mc_list(bool broadcastOnly)
{
    if (broadcastOnly == false)
    {
        printf(" '0x%02X' : DYNAMIC_BUS_CONTROL\r\n",IP1553_MODE_CMD_DYNAMIC_BUS_CONTROL);
    }
    printf(" '0x%02X' : SYNCHRONIZE_WITHOUT_DATA\r\n",IP1553_MODE_CMD_SYNCHRONIZE_WITHOUT_DATA);
    if (broadcastOnly == false)
    {
        printf(" '0x%02X' : TRANSMIT_STATUS_WORD\r\n",IP1553_MODE_CMD_TRANSMIT_STATUS_WORD);
    }
    printf(" '0x%02X' : INITIATE_SELF_TEST\r\n",IP1553_MODE_CMD_INITIATE_SELF_TEST);
    printf(" '0x%02X' : TRANSMITTER_SHUTDOWN\r\n",IP1553_MODE_CMD_TRANSMITTER_SHUTDOWN);
    printf(" '0x%02X' : OVERRIDE_TRANSMITTER_SHUTDOWN\r\n",IP1553_MODE_CMD_OVERRIDE_TRANSMITTER_SHUTDOWN);
    printf(" '0x%02X' : INHIBIT_TERMINAL_FLAG_BIT\r\n",IP1553_MODE_CMD_INHIBIT_TERMINAL_FLAG_BIT);
    printf(" '0x%02X' : OVERRIDE_INHIBIT_TERMINAL_FLAG_BIT\r\n",IP1553_MODE_CMD_OVERRIDE_INHIBIT_TERMINAL_FLAG_BIT);
    printf(" '0x%02X' : RESET_REMOTE_TERMINAL\r\n",IP1553_MODE_CMD_RESET_REMOTE_TERMINAL);
    if (broadcastOnly == false)
    {
        printf(" '0x%02X' : TRANSMIT_VECTOR_WORD\r\n",IP1553_MODE_CMD_TRANSMIT_VECTOR_WORD);
    }
    printf(" '0x%02X' : SYNCHRONIZE_WITH_DATA\r\n",IP1553_MODE_CMD_SYNCHRONIZE_WITH_DATA);
    if (broadcastOnly == false)
    {
        printf(" '0x%02X' : TRANSMIT_LAST_COMMAND\r\n",IP1553_MODE_CMD_TRANSMIT_LAST_COMMAND);
    }
    if (broadcastOnly == false)
    {
        printf(" '0x%02X' : TRANSMIT_BIT_WORD\r\n",IP1553_MODE_CMD_TRANSMIT_BIT_WORD);
    }
}

// *****************************************************************************
/* void APP_IP1553_InitiateModeCommand(uint8_t rtAddress)

 Summary:
   Prompt user and send mode command to given RT address or broadcast address.

 Description:
   Prompt for mode command selection and custom data and send it to given
   RT address or broadcast address.

 Parameters:
   rtAddress - RT address or 0x1F for broadcast.

 Remarks:
   None.
 */
static void APP_IP1553_InitiateModeCommand(uint8_t rtAddress)
{
    uint16_t transferStatusWord = 0U;
    char cmdChar[3] = {0};
    char cmdDataChar[5] = {0};
    int scanfResult = 0;
    uint16_t cmdInput = 0U;
    uint16_t cmdData = 0U;
    IP1553_MODE_CMD modeCmd = 0U;

    transferErrors = 0;
    hasStatusWord = 0;

    if (rtAddress == IP1553_RT_ADDRESS_BROADCAST_MODE)
    {
        APP_IP1553_print_mc_list(true);
    }
    else
    {
        APP_IP1553_print_mc_list(false);
    }

    printf("\r\nEnter mode command (2 character) >> 0x_ _\r\n");
    
    /* Reset errno */
    errno = 0;
    
    scanfResult = scanf(" %2c", (char *)&cmdChar[0]);

    /* Expect 2 characters */
    if ( ( errno == 0 ) && ( scanfResult == 1 ) )
    {
        cmdInput = (uint16_t)strtol(&cmdChar[0], NULL, 16);
        if ( errno != 0 )
        {
            // Invalid command
            cmdInput = 0xFFU;    
        }
    }
    else
    {
        // Invalid command
        cmdInput = 0xFFU;
    }
    
    if (cmdInput != 0xFFU)
    {
        modeCmd = (IP1553_MODE_CMD)cmdInput;
        if ( ( (modeCmd >= IP1553_MODE_CMD_DYNAMIC_BUS_CONTROL) && (modeCmd <= IP1553_MODE_CMD_RESET_REMOTE_TERMINAL) ) ||
             ( (modeCmd >= IP1553_MODE_CMD_TRANSMIT_VECTOR_WORD) && (modeCmd <= IP1553_MODE_CMD_TRANSMIT_BIT_WORD) ) )
        {
            if ( modeCmd == IP1553_MODE_CMD_SYNCHRONIZE_WITH_DATA )
            {
                printf("\r\nEnter command data (4 character) >> 0x_ _ _ _\r\n");
    	
                /* Reset errno */
                errno = 0;

                scanfResult = scanf(" %4c", (char *)&cmdDataChar[0]);

                /* Expect 4 characters */
                if ( ( errno == 0 ) && ( scanfResult == 1 ) )
                {
                    cmdData = (uint16_t)strtol(&cmdDataChar[0], NULL, 16);
                    if ( errno != 0 )
                    {
                        // Invalid command
                        cmdData = 0xFFU;    
                    }
                }
                else
                {
                    cmdData = 0U;
                }
            }

            printf("Send Mode command 0x%02X", (unsigned int)modeCmd);
            if (cmdData != 0U)
            {
                printf(" with parameter data 0x%04X", (unsigned int)cmdData);
            }
            printf(" on bus A\r\n");

            IP1553_BcModeCommandTransfer(
                    rtAddress,
                        modeCmd,
                    cmdData,
                    IP1553_BUS_A);

            if (rtAddress != IP1553_RT_ADDRESS_BROADCAST_MODE)
            {
                while ( (hasStatusWord == 0U) && (transferErrors == 0U) )
                {
                     /* Wait end of transfer */
                }

                /* Check if there was error during transfer */
                if (transferErrors != 0U)
                {
                    APP_IP1553_Print_Errors(transferErrors);
                }
                else
                {
                    /* Print Transfer Status Word */
                    transferStatusWord = IP1553_GetFirstStatusWord();
                    printf("Transfer Status Word = 0x%04X\n", transferStatusWord);
                    if ( transferStatusWord != 0x0800UL )
                    {
                        printf("ERROR in status word\n");
                    }

                    if (hasStatusWord == 2U)
                    {
                        transferStatusWord = IP1553_GetSecondStatusWord();
                        printf("Transfer Data Word = 0x%04X\n", transferStatusWord);
                    }
                }
            }
        }
    }
}

// *****************************************************************************
/* void APP_IP1553_Callback(uintptr_t context)

 Summary:
 Function called by IP1553 PLIB.

 Description:
 This function will be called by IP1553 PLIB when interrupt has occurred.

 Remarks:
 None.
 */
static void APP_IP1553_Callback(uintptr_t context)
{
    uint32_t readStatus = IP1553_IrqStatusGet();

    if ( (readStatus & IP1553_INT_MASK_ERX) == IP1553_INT_MASK_ERX )
    {
        uint32_t buffersStatusRx = IP1553_GetRxBuffersStatus();
        /* Store Rx buffer status for processing */
        activeRxBuffers = ~buffersStatusRx & APP_IP1553_BUFFER_USED;
        isTransferEnded = true;
    }

    if ( (readStatus & IP1553_INT_MASK_ETX) == IP1553_INT_MASK_ETX )
    {
        uint32_t buffersStatusTx = IP1553_GetTxBuffersStatus();
        /* Reset Tx buffer status */
        IP1553_ResetTxBuffersStatus(~buffersStatusTx & APP_IP1553_BUFFER_USED);
        isTransferEnded = true;
    }

    if ( (readStatus & IP1553_INT_MASK_ETRANS_MASK) != 0U )
    {
        hasStatusWord = IP1553_INT_MASK_GET_ETRANS(readStatus) ;
    }

    if ( (readStatus & IP1553_INT_MASK_ERROR_MASK) != 0U )
    {
        transferErrors = readStatus & IP1553_INT_MASK_ERROR_MASK;
    }

    NVIC_ClearPendingIRQ(IP1553_IRQn);
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void)
{
    char user_input = '0';
    int scanfResult = 0;
    uint16_t transferStatusWord = 0U;

    /* Initialize all modules */
    SYS_Initialize(NULL);

    printf("\n\r-----------------------------------------------------------");
    printf("\n\r  IP1553 - BC mode interrupt operation example             ");
    printf("\n\r-----------------------------------------------------------");

    /* Set buffers Configuration */
    IP1553_BuffersConfigSet(&IP1553TxBuffersRAM[0][0], &IP1553RxBuffersRAM[0][0]);

    /* Set 1553 application callback on interrupt event */
    IP1553_CallbackRegister(APP_IP1553_Callback, (uintptr_t) NULL);

    /* Enable all IP1553 interrupts */
    IP1553_InterruptEnable(
        IP1553_INT_MASK_EMT |
        IP1553_INT_MASK_MTE |
        IP1553_INT_MASK_ERX |
        IP1553_INT_MASK_ETX |
        IP1553_INT_MASK_ETRANS_MASK |
        IP1553_INT_MASK_TE |
        IP1553_INT_MASK_TCE |
        IP1553_INT_MASK_TPE |
        IP1553_INT_MASK_TDE |
        IP1553_INT_MASK_TTE |
        IP1553_INT_MASK_TWE |
        IP1553_INT_MASK_BE |
        IP1553_INT_MASK_ITR |
        IP1553_INT_MASK_TVR |
        IP1553_INT_MASK_DBR |
        IP1553_INT_MASK_STR |
        IP1553_INT_MASK_TSR |
        IP1553_INT_MASK_OSR |
        IP1553_INT_MASK_SDR |
        IP1553_INT_MASK_SWD |
        IP1553_INT_MASK_RRT |
        IP1553_INT_MASK_ITF |
        IP1553_INT_MASK_OTF |
        IP1553_INT_MASK_IPB);

    /* Reset Tx and Rx status for buffers 0, 1, 2, 3 and 31 */
    IP1553_ResetTxBuffersStatus(APP_IP1553_BUFFER_USED);
    IP1553_ResetRxBuffersStatus(APP_IP1553_BUFFER_USED);

    /* Prepare content in buffers to send*/
    for (uint8_t i = 0U; i < IP1553_BUFFERS_NUM; i++)
    {
        for (uint8_t j = 0U; j < IP1553_BUFFERS_SIZE; j++)
        {
            IP1553TxBuffersRAM[i][j] = (uint16_t)(((uint16_t)i << 12U) + (uint16_t)j + 1U);
        }
    }

    print_menu();

    while ( true )
    {
        /* Reset errno */
        errno = 0;

        scanfResult = scanf("%c", (char *)&user_input);

        /* Get User input, expect 1 character */            
        if ( ( errno == 0 ) && ( scanfResult == 1 ) )
        {
            switch (user_input)
            {
                case '1':
                    printf("  > Send one data from TX buffer %u to RT1, on Bus A: \r\n", gBcSubAddrData);
                    isTransferEnded = false;
                    hasStatusWord = 0U;
                    transferErrors = 0U;
                    IP1553_BcStartDataTransfer(
                        IP1553_DATA_TX_TYPE_BC_TO_RT,
                        APP_IP1553_BC_ADDR,
                        gBcSubAddrData,
                        APP_IP1553_RT_ADDR,
                        APP_IP1553_RT_BUFFER_RECV_SUB_ADDR,
                        APP_IP1553_TRANSFER_WORD_SIZE,
                        IP1553_BUS_A);


                    while ( (transferErrors == 0U) && ( (isTransferEnded == false) || (hasStatusWord == 0U) ) )
                    {
                        /* Wait end of transfer */
                    }

                    /* Check if there was error during transfer */
                    if (transferErrors != 0U)
                    {
                        APP_IP1553_Print_Errors(transferErrors);
                    }
                    else
                    {
                        /* Read status word */
                        transferStatusWord = IP1553_GetFirstStatusWord();
                        printf("Transfer Status Word = 0x%04X\n", (unsigned int)transferStatusWord);
                        if ( transferStatusWord != 0x0800UL )
                        {
                            printf("ERROR in status word\n");
                        }

                        /* Change active buffer */
                        gBcSubAddrData++;
                        if (gBcSubAddrData > 3U)
                        {
                            gBcSubAddrData = 0U;
                        }
                    }
                break;

                case '2':
                    printf("  > Receive one data in buffer 3 from RT1, on bus A: \r\n");
                    isTransferEnded = false;
                    hasStatusWord = 0U;
                    transferErrors = 0U;
                    IP1553_BcStartDataTransfer(
                        IP1553_DATA_TX_TYPE_RT_TO_BC,
                        APP_IP1553_RT_ADDR,
                        APP_IP1553_RT_BUFFER_SEND_SUB_ADDR,
                        APP_IP1553_BC_ADDR,
                        APP_IP1553_BC_BUFFER_RECV_SUB_ADDR,
                        APP_IP1553_TRANSFER_WORD_SIZE,
                        IP1553_BUS_A);

                    while ( (transferErrors == 0U) && ( (isTransferEnded == false) || (hasStatusWord == 0U) ) )
                    {
                        /* Wait end of transfer */
                    }

                    /* Check if there was error during transfer */
                    if (transferErrors != 0U)
                    {
                        APP_IP1553_Print_Errors(transferErrors);
                    }
                    else
                    {
                        /* Read status word */
                        transferStatusWord = IP1553_GetFirstStatusWord();
                        printf(" Transfer Status Word = 0x%04X\n", (unsigned int) transferStatusWord);

                        /* Print and reset Rx buffers*/
                        uint32_t lastActiveBuffers = activeRxBuffers;
                        uint32_t buffer = 0U;
                        while ( lastActiveBuffers != 0U )
                        {
                            while ( (lastActiveBuffers & 0x1UL) == 0U )
                            {
                                buffer++;
                                lastActiveBuffers >>= 1U;
                            }

                            printf("RX buffer : %u", (unsigned int) buffer);
                            for (uint8_t index = 0U; index < 32U; index++)
                            {
                                if ( (index % 8U) == 0U )
                                {
                                    printf("\r\n    ");
                                }
                                else
                                {
                                    printf(" ,");
                                }
                                printf("0x%04X", IP1553RxBuffersRAM[buffer][index]);
                            }
                            printf("\r\n");

                            /*Reset Rx buffer content */
                            memset(&IP1553RxBuffersRAM[buffer][0], 0, IP1553_BUFFERS_SIZE);

                            /* Reset Rx buffer status */
                            IP1553_ResetRxBuffersStatus(IP1553_BUFFER_TO_BITFIELD_SA(buffer));

                            /* Go to next bit in lastActiveBuffer bit field */
                            buffer++;
                            lastActiveBuffers >>= 1U;
                        }
                    }
                break;

                case '3':
                    printf("  > Broadcast for RTs, from buffer 1, on bus A: \r\n");
                    isTransferEnded = false;
                    transferErrors = 0U;
                    IP1553_BcStartDataTransfer(
                        IP1553_DATA_TX_TYPE_BC_TO_RT,
                        0U,
                        0U,
                        IP1553_RT_ADDRESS_BROADCAST_MODE,
                        APP_IP1553_RT_BUFFER_SEND_SUB_ADDR,
                        0U,
                        IP1553_BUS_A);

                    while ( ( transferErrors == 0U ) && !isTransferEnded )
                    {
                        /* Wait end of transfer */
                    }

                    /* Check if there was error during transfer */
                    if ( transferErrors != 0U )
                    {
                        APP_IP1553_Print_Errors(transferErrors);
                    }
                break;

                case '4':
                    printf("  > Initiate mode command transfer to RT1 on bus A : \r\n");
                    APP_IP1553_InitiateModeCommand(APP_IP1553_RT_ADDR);
                break;

                case '5':
                    printf("  > Initiate broadcast mode command transfer on bus A : \r\n");
                    APP_IP1553_InitiateModeCommand(IP1553_RT_ADDRESS_BROADCAST_MODE);
                break;

                default:
                    printf("  > Invalid Input \r\n");
                break;
            }
        }
        else
        {
            printf("  > Invalid Input \r\n");
        }

        print_menu();

    }

    /* Execution should not come here during normal operation */

    return (EXIT_FAILURE);
}

/*******************************************************************************
 End of File
 */

