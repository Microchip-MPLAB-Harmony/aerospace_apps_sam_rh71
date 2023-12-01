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
#include <string.h>                     // Defines memset
#include "definitions.h"                // SYS function prototypes
#include "app_rmap.h"

// *****************************************************************************
// *****************************************************************************
// Section: Defines
// *****************************************************************************
// *****************************************************************************

/* Data size of sent and received packets in words */
#define APP_DATA_SIZE_WORDS                 (16U)

/* Number of Rx packet in receive buffer */
#define APP_RX_PACKET_NUM                   (1U)

/* Size of Rx packet in receive buffer in bytes :
   Set to the maximum receive size for RMAP reply : 4 word status + data */
#define APP_RX_PACKET_SIZE_BYTES            ((16U+(APP_DATA_SIZE_WORDS*4U))*APP_RX_PACKET_NUM)

/* Number of Tx packet in transmitter list */
#define APP_TX_NUM_PACKET                   (1U)

/* RMAP target logical address for RMAP command examples */
#define APP_RMAP_TARGET_LOGICAL_ADDRESS     (0x42U)

/* RMAP destination key for RMAP command examples */
#define APP_RMAP_DESTINATION_KEY            (0x89U)

/* RMAP initiator reply logical address for RMAP command examples */
#define APP_RMAP_REPLY_LOGICAL_ADDRESS      (0x46U)

/* RMAP header max size */
#define APP_RMAP_HEADER_MAX_SIZE            (28U)

/* RMAP transaction ID for RMAP command examples */
#define APP_RMAP_TRANSACTION_ID             (0xABCDU)

/* RMAP write reply size in bytes */
#define APP_RMAP_WRITE_REPLY_SIZE_BYTES     (8U)

/* RMAP read reply size in bytes (without data bytes) */
#define APP_RMAP_READ_REPLY_SIZE_BYTES      (13U)

// *****************************************************************************
// *****************************************************************************
// Section: Variables
// *****************************************************************************
// *****************************************************************************

/* Tx buffer of data */
static uint32_t __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_tx_data[APP_DATA_SIZE_WORDS] = {0};

/* Tx send list */
static SPW_PKTTX_SEND_LIST_ENTRY __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_tx_packet_send_list[APP_TX_NUM_PACKET] = {0};

/* Store information if the TX sequence is ended */
static volatile bool app_tx_is_end = false;

/* Rx buffer of data */
static uint8_t __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_rx_buffer_data[APP_RX_PACKET_SIZE_BYTES] = {0};

/* Rx packet information list */
static SPW_PKTRX_INFO __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_rx_packet_info[APP_RX_PACKET_NUM] = {0};

/* Store information if the RX sequence is ended */
static volatile bool app_rx_is_end = false;

/* Rmap header buffer */
static uint8_t __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_buffer_rmap_header[APP_RMAP_HEADER_MAX_SIZE] = {0};

/* Rmap target buffer to be read/write using the RMAP commands */
static uint32_t __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_rmap_modified_buffer[APP_DATA_SIZE_WORDS] = {0};

// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************
/* static void APP_SPW_PrintInterruptErrors(uint32_t errors)

  Summary:
    Function called by the application to print the description of the link
    interrupt errors .

  Description:
    Print the description of the errors that are set in the given bit field.

  Parameters:
    errors - Bit field of the SPW interrupt status that contains errors.

  Remarks:
    None.
*/
static void APP_SPW_PrintInterruptErrors(uint32_t errors)
{
    if ((errors & SPW_LINK_INT_MASK_DISERR) == SPW_LINK_INT_MASK_DISERR)
    {
        printf("  Link interface disconnection error detected.\r\n");
    }
    if ((errors & SPW_LINK_INT_MASK_PARERR) == SPW_LINK_INT_MASK_PARERR)
    {
        printf("  Link interface parity error detected.\r\n");
    }
    if ((errors & SPW_LINK_INT_MASK_ESCERR) == SPW_LINK_INT_MASK_ESCERR)
    {
        printf("  Link interface ESC error detected.\r\n");
    }
    if ((errors & SPW_LINK_INT_MASK_CRERR) == SPW_LINK_INT_MASK_CRERR)
    {
        printf("  Link interface credit error detected.\r\n");
    }
    if ((errors & SPW_LINK_INT_MASK_LINKABORT) == SPW_LINK_INT_MASK_LINKABORT)
    {
        printf("  Link state has made a transition from Run to Error Reset\r\n");
    }
    if ((errors & SPW_LINK_INT_MASK_EEPTRANS) == SPW_LINK_INT_MASK_EEPTRANS)
    {
        printf("  EEP transmitted.\r\n");
    }
    if ((errors & SPW_LINK_INT_MASK_EEPREC) == SPW_LINK_INT_MASK_EEPREC)
    {
        printf("  EEP received.\r\n");
    }
    if ((errors & SPW_LINK_INT_MASK_DISCARD) == SPW_LINK_INT_MASK_DISCARD)
    {
        printf("  Transmit packet discarded.\r\n");
    }
}

// *****************************************************************************
/* static void APP_SPW_InitRx(void)

   Summary:
    Initialize SPW packet reception circular buffer and start the receptions
    with the first buffers.

   Description:
    This function initialize SPW packet reception using the circular buffer
    descriptor to set the two first buffer as current and next buffers.

   Parameters:
    expectedDataSize - Number of expected bytes to receive.

   Remarks:
    None.
*/
static void APP_SPW_InitRx(uint8_t expectedDataSize)
{
    uint32_t rxDataSize = expectedDataSize;
    /* round up data size to word size */
    if ( (rxDataSize % 4U) != 0U )
    {
        rxDataSize += (4U - (rxDataSize % 4U));
    }

    printf("Initialize packet receiver to receive %d packet(s) of %u bytes\r\n", APP_RX_PACKET_NUM, (unsigned int)rxDataSize);

    if (rxDataSize > sizeof(app_rx_buffer_data))
    {
        printf("[ERROR] Expected data size over data buffer size : %u\r\n", sizeof(app_rx_buffer_data));
    }
    else
    {
        /* Clear current receive buffer */
        memset(&(app_rx_buffer_data[0]), 0, sizeof(app_rx_buffer_data));
        memset(&(app_rx_packet_info[0]), 0, sizeof(app_rx_packet_info));

        app_rx_is_end = false;

        // Set  in Next buffer with start condition now
        SPW_PKTRX_SetNextBuffer(
            &(app_rx_buffer_data[0]),
            rxDataSize,
            &(app_rx_packet_info[0]),
            APP_RX_PACKET_NUM,
            false,
            SPW_PKTRX_NXTBUF_START_NOW,
            0U);

    /* Enable packet RX interrupts */
    SPW_PKTRX_InterruptEnable(SPW_PKTRX_INT_MASK_DEACT);
    }
}

// *****************************************************************************
/* static void APP_SendCommandRMAP(SPW_LINK link, APP_RMAP_COMMAND_CODE commandCode, uint32_t memAddress)

   Summary:
    Send SPW RMAP command packet to read or write at given memory address.

   Description:
    Generate RMAP header to read or write data at the given memory address.
    Send SpaceWire RMAP packet using packet transmitter.
    The router bytes are used to send the packet thought
    a specified link and then to route to the RMAP router port.

   Parameters:
    link - Link ID to route the transmitted message.
    commandCode - RMAP command code.
    memAddress - Target memory address.

   Remarks:
    None.
*/
static void APP_SendCommandRMAP(SPW_LINK link, APP_RMAP_COMMAND_CODE commandCode, uint32_t memAddress)
{
    if (commandCode != APP_RMAP_COMMAND_CODE_READ_INC)
    {
        /* Build buffer of data to be transmitted */
        for (uint32_t i = 0U; i < APP_DATA_SIZE_WORDS; i++)
        {
            app_tx_data[i]= (((i*4U)+3U)<<24U) + \
                            (((i*4U)+2U)<<16U) + \
                            (((i*4U)+1U)<<8U)  + \
                            (i*4U) ;
        }
    }

    /* Build RMAP header */
    uint8_t rmap_header_size = APP_RMAP_GenerateHeader(&(app_buffer_rmap_header[0]),
                               APP_RMAP_TARGET_LOGICAL_ADDRESS,
                               APP_RMAP_PCK_TYPE_COMMAND,
                               commandCode,
                               APP_RMAP_DESTINATION_KEY,
                               NULL,
                               0U,
                               APP_RMAP_REPLY_LOGICAL_ADDRESS,
                               APP_RMAP_TRANSACTION_ID,
                               0U,
                               memAddress,
                               APP_DATA_SIZE_WORDS*4U);

    /* Prepare send list with 1 packet with RMAP header */
    memset(&(app_tx_packet_send_list[0]), 0, sizeof(SPW_PKTTX_SEND_LIST_ENTRY)*APP_TX_NUM_PACKET);
    app_tx_packet_send_list[0].RSize = 2U;
    app_tx_packet_send_list[0].RB1 = (uint32_t)link;
    app_tx_packet_send_list[0].RB2 = SPW_ROUTER_RMAP_PORT;
    app_tx_packet_send_list[0].EscMask = 0xFU;
    app_tx_packet_send_list[0].EscChar = 0xFAU;
    app_tx_packet_send_list[0].HCrc = 1U;
    app_tx_packet_send_list[0].HSize = rmap_header_size;
    app_tx_packet_send_list[0].HAddr = (unsigned int )  &(app_buffer_rmap_header[0]);

    /* If RMAP Command is write, add data buffer address and size */
    if (commandCode != APP_RMAP_COMMAND_CODE_READ_INC)
    {
        app_tx_packet_send_list[0].DCrc = 1U;
        app_tx_packet_send_list[0].DSize = APP_DATA_SIZE_WORDS * 4U;
        app_tx_packet_send_list[0].DAddr = (unsigned int) &(app_tx_data[0]);
    }

    /* Enable packet TX interrupts */
    SPW_PKTTX_InterruptEnable(SPW_PKTTX_INT_MASK_DEACT);

    app_tx_is_end = false;

    /* Prepare next send list without router bytes. */
    SPW_PKTTX_SetNextSendList(
        NULL,
        &(app_tx_packet_send_list[0]),
        APP_TX_NUM_PACKET,
        true,
        SPW_PKTTX_NXTSEND_START_NOW,
        0U);
}

// *****************************************************************************
/* static int8_t APP_SPW_WaitAndCheckRmapReply(uint32_t expectedSize)

   Summary:
    Wait for packet TX and RX deactivation and check RMAP reply status.

   Description:
    Wait for packet TX and RX deactivation and check both status.
    Check RMAP expected reply size and status.

   Parameters:
    expectedSize - Size of the expected RMAP reply.

   Remarks:
    Return 0 if RMAP reply was received correctly, -1 otherwise.
*/
static int8_t APP_SPW_WaitAndCheckRmapReply(uint32_t expectedSize)
{
    int8_t res = 0;
    bool tmp_app_rx_is_end = app_rx_is_end;

    /* Wait end of packet form transmit and receive modules */      
    while ( (app_tx_is_end == false) ||  (tmp_app_rx_is_end == false) )
    {
        tmp_app_rx_is_end = app_rx_is_end;
    }

    /* Check if there was error in TX send list */
    SPW_PKTTX_STATUS tx_status = SPW_PKTTX_StatusGet();
    SPW_PKTTX_PREV previous = SPW_PKTTX_STATUS_PREV_GET(tx_status);

    if (previous != SPW_PKTTX_PREV_LASTSENDLISTOK)
    {
        printf("    ERROR : Previous send list status KO : %d\r\n", (unsigned int)previous);
        res = -1;
    }

    /* Unlock previous buffer status */
    SPW_PKTTX_UnlockStatus();

    SPW_PKTRX_PREV_STATUS status = SPW_PKTRX_GetPreviousBufferStatus();
    /* Check we receive 1 packet with Buffer Info Full and Locked status. */

    SPW_PKTRX_PREV_STATUS expected_status = SPW_PKTRX_PREV_STATUS_LOCKED | SPW_PKTRX_PREV_STATUS_FULLI | SPW_PKTRX_PREV_STATUS_FULLD | 1U;
    if (status != expected_status) {
        printf("    ERROR : RX status differ from expected : 0x%x != 0x%x\r\n", (unsigned int)status, (unsigned int)expected_status);
        res = -1;
    }

    /* Check received packet is not split */
    if ( (app_rx_packet_info[0].Split) != 0U) {
        printf("    ERROR : Packet split : Data have been discarded\r\n");
        res = -1;
    }

    /* Compare receive packet expected size */
    if (app_rx_packet_info[0].DSize != expectedSize) {
        printf("    ERROR : Wrong packet size received (%d != %u) \r\n", app_rx_packet_info[0].DSize, (unsigned int)expectedSize);
        res = -1;
    }

    /* Get byte pointer on Rx buffer */
    uint8_t* rx_buffer_reply = (uint8_t*)(app_rx_packet_info[0].DAddr);
    if ( (rx_buffer_reply[1] == 0x01U) && (APP_RMAP_PackeTypeGet(rx_buffer_reply[2]) == APP_RMAP_PCK_TYPE_REPLY) )
    {
        // Write reply
        if ( APP_RMAP_CommandCodeGet(rx_buffer_reply[2]) == APP_RMAP_COMMAND_CODE_WRITE_INC_REPLY )
        {
            printf("    Receive RMAP WRITE reply : ");
        }
        else if ( APP_RMAP_CommandCodeGet(rx_buffer_reply[2]) == APP_RMAP_COMMAND_CODE_READ_INC )
        {
            printf("    Receive RMAP READ reply : ");
        }
        else
        {
            printf("    ERROR Receive unexpected RMAP reply\r\n");
            res = -1;
        }

        if (res == 0)
        {
            uint16_t transaction_id = ((uint16_t)(rx_buffer_reply[5]) << 8) + rx_buffer_reply[6];

            printf("ID = 0x%x, with status : 0x%x\r\n", transaction_id, rx_buffer_reply[3]);

            if ( (rx_buffer_reply[3] != 0U) || (transaction_id != APP_RMAP_TRANSACTION_ID) )
            {
                res = -1;
            }

            if ( APP_RMAP_CommandCodeGet(rx_buffer_reply[2]) == APP_RMAP_COMMAND_CODE_READ_INC )
            {
                uint32_t data_length =  (uint32_t)(((uint32_t)(rx_buffer_reply[8]) << 16U) + \
                                                   ((uint32_t)(rx_buffer_reply[9]) << 8U)  + \
                                                   rx_buffer_reply[10]);
                printf("      Receive %u bytes of data :", (unsigned int)data_length);
                for (uint32_t i=0U; i<data_length; i++)
                {
                    if ((i%16U) == 0U)
                    {
                        printf("\r\n        ");
                    }
                    printf("%02X, ", rx_buffer_reply[12U+i]);
                }
                printf("\r\n");
            }
        }
    }
    else
    {
       printf("    ERROR : Received unexpected packet, expected RMAP reply\r\n");
       res = -1;
    }

    return res;
}

// *****************************************************************************
/* static void APP_SPW_Callback(SPW_INT_MASK irqStatus, uintptr_t context)

  Summary:
    Function called by SPW PLIB.

  Description:
    This function will be called by SPW PLIB when interrupt has occurred.

  Remarks:
    None.
*/
static void APP_SPW_Callback(SPW_INT_MASK irqStatus, uintptr_t context)
{
    if ( (irqStatus & SPW_INT_MASK_PKTTX1) != 0U )
    {
        SPW_PKTTX_INT_MASK status = SPW_PKTTX_IrqStatusGetMaskedAndClear();

        if ( (status & SPW_PKTTX_INT_MASK_DEACT) != 0U )
        {
            app_tx_is_end = true;
        }
    }

    if ( (irqStatus & SPW_INT_MASK_PKTRX1) != 0U )
    {
        SPW_PKTRX_INT_MASK status = SPW_PKTRX_IrqStatusGetMaskedAndClear();

        if ( (status & SPW_PKTRX_INT_MASK_DEACT) != 0U )
        {
            app_rx_is_end = true;
        }
    }

    if ( (irqStatus & SPW_INT_MASK_LINK1) != 0U )
    {
        SPW_LINK_INT_MASK status = SPW_LINK_IrqStatusGetMaskedAndClear(SPW_LINK_1);
        printf("ERROR(s) on SPW Link 1 :\r\n");
        APP_SPW_PrintInterruptErrors(status);
    }

    if ( (irqStatus & SPW_INT_MASK_LINK2) != 0U )
    {
        SPW_LINK_INT_MASK status = SPW_LINK_IrqStatusGetMaskedAndClear(SPW_LINK_2);
        printf("ERROR(s) on SPW Link 2 :\r\n");
        APP_SPW_PrintInterruptErrors(status);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    int8_t result = 0;
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    printf("\n\r-----------------------------------------------------------");
    printf("\n\r  SPW RMAP loopback example                                ");
    printf("\n\r-----------------------------------------------------------\n\r");

    /* Add SPW interrupt callback */
    SPW_CallbackRegister(APP_SPW_Callback, (uintptr_t)NULL);

    /* Enable SPW Link interrupts */
    SPW_LINK_InterruptEnable(SPW_LINK_1 , SPW_LINK_INT_MASK_DISERR |
                                          SPW_LINK_INT_MASK_PARERR |
                                          SPW_LINK_INT_MASK_ESCERR |
                                          SPW_LINK_INT_MASK_CRERR |
                                          SPW_LINK_INT_MASK_LINKABORT |
                                          SPW_LINK_INT_MASK_EEPTRANS |
                                          SPW_LINK_INT_MASK_EEPREC |
                                          SPW_LINK_INT_MASK_DISCARD);
    SPW_LINK_InterruptEnable(SPW_LINK_2 , SPW_LINK_INT_MASK_DISERR |
                                          SPW_LINK_INT_MASK_PARERR |
                                          SPW_LINK_INT_MASK_ESCERR |
                                          SPW_LINK_INT_MASK_CRERR |
                                          SPW_LINK_INT_MASK_LINKABORT |
                                          SPW_LINK_INT_MASK_EEPTRANS |
                                          SPW_LINK_INT_MASK_EEPREC |
                                          SPW_LINK_INT_MASK_DISCARD);

    printf("Wait for both SWP link switch to run state\r\n");

    /* Wait link goes to Run state */
    SPW_LINK_STATE spwLink1Status = SPW_LINK_STATE_ERROR_RESET;
    SPW_LINK_STATE spwLink2Status = SPW_LINK_STATE_ERROR_RESET;
    do
    {
        spwLink1Status = SPW_LINK_GET_STATE(SPW_LINK_StatusGet(SPW_LINK_1));
        spwLink2Status = SPW_LINK_GET_STATE(SPW_LINK_StatusGet(SPW_LINK_2));
    }
    while ( (spwLink1Status != SPW_LINK_STATE_RUN) &&
            (spwLink2Status != SPW_LINK_STATE_RUN) );

    printf("    SPW Link in Run state\r\n");

    APP_SPW_InitRx(APP_RMAP_WRITE_REPLY_SIZE_BYTES);

    printf("Send SWP packet with RMAP command write with reply\r\n");
    APP_SendCommandRMAP(SPW_LINK_1, APP_RMAP_COMMAND_CODE_WRITE_INC_REPLY, (uint32_t)&(app_rmap_modified_buffer[0]));

    printf("Wait RMAP reply\r\n");
    result = APP_SPW_WaitAndCheckRmapReply(APP_RMAP_WRITE_REPLY_SIZE_BYTES);

    if (result != 0)
    {
        printf("  ERROR : RMAP write Command FAIL\r\n");
    }
    else
    {
        printf("  RMAP write command reply with status OK\r\n");
    }

    APP_SPW_InitRx( APP_RMAP_READ_REPLY_SIZE_BYTES + (APP_DATA_SIZE_WORDS * 4U) );

    printf("Send SWP packet with RMAP command read\r\n");
    APP_SendCommandRMAP(SPW_LINK_1, APP_RMAP_COMMAND_CODE_READ_INC, (uint32_t)&(app_rmap_modified_buffer[0]));

    printf("Wait RMAP reply\r\n");
    result = APP_SPW_WaitAndCheckRmapReply(APP_RMAP_READ_REPLY_SIZE_BYTES + (APP_DATA_SIZE_WORDS * 4U) );

    if (result != 0)
    {
        printf("  ERROR : RMAP read Command FAIL\r\n");
    }
    else
    {
        printf("  RMAP read command reply with status OK\r\n");
    }

    while ( true )
    {
        /* End of example */
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/
