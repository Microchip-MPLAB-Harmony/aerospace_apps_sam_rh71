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

/*----------------------------------------------------------------------------
 *        Types
 *----------------------------------------------------------------------------*/
/* Application circular buffer descriptor object for receive buffer ID list

   Summary:
    Application circular buffer descriptor object for receive buffer.

   Description:
    This data structure defines an circular buffer descriptor for receive buffers.

   Remarks:
    None.
*/
typedef struct
{
    uint16_t tail;
    uint16_t head;
    uint8_t full;
} app_spw_rx_buffer_list_desc;

// *****************************************************************************
// *****************************************************************************
// Section: Defines
// *****************************************************************************
// *****************************************************************************

/* Data size of sent and received packets in words */
#define APP_DATA_SIZE_WORDS             (16)

/* Number of Rx buffers in receiver list */
#define APP_RX_RECV_LIST                (20)

/* Number of Rx packets in receive buffer */
#define APP_RX_PACKET_NUM               (100)

/* Size of Rx packet in receive buffers in bytes*/
#define APP_RX_PACKET_SIZE_BYTES        (64*APP_RX_PACKET_NUM)

/* Sent Number of send list */
#define APP_TX_NUM_SEND                 (15625)

/* Number of packets per send */
#define APP_TX_PACKET_PER_SEND          (25)

/* Number of Tx packets in transmitter list */
#define APP_TX_NUM_PACKET               (APP_TX_PACKET_PER_SEND)

// *****************************************************************************
// *****************************************************************************
// Section: Variables
// *****************************************************************************
// *****************************************************************************

/* Tx buffer of data */
uint32_t __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_tx_data[APP_TX_NUM_PACKET][APP_DATA_SIZE_WORDS] = {0};

/* Tx send list */
SPW_PKTTX_SEND_LIST_ENTRY __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_tx_packet_send_list[APP_TX_NUM_PACKET] = {0};

/* Store information if the TX sequence is ended */
bool app_tx_is_end = false;

/* Counter time value at end of TX sequence */
uint32_t app_tx_time_elapsed = 0;

/* Number of TX send sequences complete */
uint32_t app_tx_num_sent = 0;

/* Rx buffer of data */
uint8_t __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_rx_buffer_data[APP_RX_RECV_LIST][APP_RX_PACKET_SIZE_BYTES] = {0};

/* Rx packet information list */
SPW_PKTRX_INFO __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_rx_packet_info[APP_RX_RECV_LIST][APP_RX_PACKET_NUM] = {0};

/* Rx packet status */
SPW_PKTRX_PREV_STATUS app_rx_packet_status[APP_RX_RECV_LIST] = {0};

/* Store the current state of receive buffer list */
volatile app_spw_rx_buffer_list_desc app_rx_buff_list = {0};

/* Global variable to store if there is at least one packet in current receive buffer */
bool app_rx_packet_in_current = false;

/* Store the current average of process time */
uint32_t app_rx_average_process_time = 0;

/* Store the number of values in the average of process time */
uint32_t app_rx_average_process_time_values = 0;

/* Number of received packets */
uint32_t app_rx_num_packets = 0;

/* Expected index for next received packet */
uint32_t app_rx_next_index_expected = 0;

/* Store information if all expected packets have been received */
bool app_rx_is_all_data_received = false;

/* Number of receive index sequence error */
uint32_t app_rx_seq_error = 0;

// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************
/* void APP_SPW_PrintInterruptErrors(uint32_t errors)

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
void APP_SPW_PrintInterruptErrors(uint32_t errors)
{
    if ((errors & SPW_LINK_INT_MASK_DISERR) == SPW_LINK_INT_MASK_DISERR)
        printf("  Link interface disconnection error detected.\r\n");
    if ((errors & SPW_LINK_INT_MASK_PARERR) == SPW_LINK_INT_MASK_PARERR)
        printf("  Link interface parity error detected.\r\n");
    if ((errors & SPW_LINK_INT_MASK_ESCERR) == SPW_LINK_INT_MASK_ESCERR)
        printf("  Link interface ESC error detected.\r\n");
    if ((errors & SPW_LINK_INT_MASK_CRERR) == SPW_LINK_INT_MASK_CRERR)
        printf("  Link interface credit error detected.\r\n");
    if ((errors & SPW_LINK_INT_MASK_LINKABORT) == SPW_LINK_INT_MASK_LINKABORT)
        printf("  Link state has made a transition from Run to Error Reset\r\n");
    if ((errors & SPW_LINK_INT_MASK_EEPTRANS) == SPW_LINK_INT_MASK_EEPTRANS)
        printf("  EEP transmitted.\r\n");
    if ((errors & SPW_LINK_INT_MASK_EEPREC) == SPW_LINK_INT_MASK_EEPREC)
        printf("  EEP received.\r\n");
    if ((errors & SPW_LINK_INT_MASK_DISCARD) == SPW_LINK_INT_MASK_DISCARD)
        printf("  Transmit packet discarded.\r\n");
}

/* void APP_SPW_StartSendList(void)

   Summary:
    Start to transmit the application send list.

   Description:
    This function will set the application prepared send list to be transmitted
    immediately.

   Remarks:
    None.
*/
static void APP_SPW_StartSendList(void)
{
    // Prepare next send list without router bytes.
    SPW_PKTTX_SetNextSendList(
        NULL,
        &(app_tx_packet_send_list[0]),
        APP_TX_NUM_PACKET,
        false,
        SPW_PKTTX_NXTSEND_START_NOW,
        0);
}

// *****************************************************************************
/* void APP_SPW_SetNextReceiveBuffer(uint16_t bufferId, SPW_PKTRX_NXTBUF_START startMode)

   Summary:
    Set next receive buffer ID with staring condition.

   Description:
    This function will set the next receive buffer ID for packet reception using
    the given start condition (NOW or LATER).

   Parameters:
    bufferId - the ID of the buffer to set as next receive buffer.
    startMode - the start mode for the receive buffer (NOW or LATER).

   Remarks:
    None.
*/
static void APP_SPW_SetNextReceiveBuffer(uint16_t bufferId, SPW_PKTRX_NXTBUF_START startMode)
{
    SPW_PKTRX_SetNextBuffer(
        &(app_rx_buffer_data[bufferId][0]),
        APP_RX_PACKET_SIZE_BYTES,
        &(app_rx_packet_info[bufferId][0]),
        APP_RX_PACKET_NUM,
        false,
        startMode,
        0);
}

/* void APP_SPW_InitRx(void)

   Summary:
    Initialize SPW packet reception circular buffer and start the receptions
    with the first buffers.

   Description:
    This function initialize SPW packet reception using the circular buffer
    descriptor to set the two first buffer as current and next buffers.

   Remarks:
    None.
*/
static void APP_SPW_InitRx(void)
{
    uint16_t next_buffer = 0;

    app_rx_buff_list.tail = 0;
    app_rx_buff_list.head = 0;
    app_rx_buff_list.full = 0;

    /* Disable discard mode to stall incoming packets */
    SPW_PKTRX_SetDiscard(false);

    /* Set list element 0 in Next buffer with start condition now */
    next_buffer = app_rx_buff_list.head;
    APP_SPW_SetNextReceiveBuffer(next_buffer, SPW_PKTRX_NXTBUF_START_NOW);

    /* Wait list become active */
    while ((SPW_PKTRX_StatusGet() & SPW_PKTRX_STATUS_ACT) == 0);

    /* Set list element 1 in Next buffer with start condition later (on current buffer deactivation) */
    next_buffer = (app_rx_buff_list.head + 1);
    APP_SPW_SetNextReceiveBuffer(next_buffer, SPW_PKTRX_NXTBUF_START_LATER);

    /* Start timer for RX process time calculation */
    TC0_CH2_TimerStart();

    /* Enable packet RX interrupts */
    SPW_PKTRX_InterruptEnable(SPW_PKTRX_INT_MASK_DEACT | SPW_PKTRX_INT_MASK_EOP);
}

/* void APP_SPW_InitTx(void)

   Summary:
    Initialize SPW packet emission to start transmitting data.

   Description:
    This function initialize SPW packet emission by writing the initial data in
    TX buffer, preparing the send list and initiate the first send of data.

   Remarks:
    None.
*/
static void APP_SPW_InitTx(void)
{
    app_tx_num_sent = 0;

    /* Build buffer of data to be transmitted */
    for (uint32_t pck = 0; pck < APP_TX_PACKET_PER_SEND; pck++)
    {
        for (uint32_t i = 0; i < APP_DATA_SIZE_WORDS; i++)
        {
            if (i == 0)
                app_tx_data[pck][i] = (((i * 4) + 3) << 24) + (((i * 4) + 2) << 16) + (((i * 4) + 1) << 8) + pck;
            else
                app_tx_data[pck][i] = (((i * 4) + 3) << 24) + (((i * 4) + 2) << 16) + (((i * 4) + 1) << 8) + (i * 4);
        }

        // Prepare send list
        app_tx_packet_send_list[pck].RSize = 2;
        app_tx_packet_send_list[pck].RB1 = SPW_ROUTER_LINK1_PORT;
        app_tx_packet_send_list[pck].RB2 = SPW_ROUTER_PKTRX_PORT;
        app_tx_packet_send_list[pck].EscMask = 0xF;
        app_tx_packet_send_list[pck].EscChar = 0xFA;
        app_tx_packet_send_list[pck].HSize = 0;
        app_tx_packet_send_list[pck].HAddr = 0;
        app_tx_packet_send_list[pck].DSize = APP_DATA_SIZE_WORDS * 4;
        app_tx_packet_send_list[pck].DAddr = (unsigned int) &(app_tx_data[pck][0]);
    }

    /* Start timer for TX time calculation */
    TC0_CH1_TimerStart();
    app_tx_is_end = false;

    /* Enable packet RX interrupts */
    SPW_PKTTX_InterruptEnable(SPW_PKTTX_INT_MASK_DEACT);

    /* Initiate first send */
    APP_SPW_StartSendList();
}

/* void APP_SPW_CheckData(uint32_t buffer_id)

   Summary:
    Check the content of the received data buffer.

   Description:
    This function check the status and the content of the received buffer.
    It check the number of packets and the correct index order of received packets.

   Parameters:
    bufferId - the ID of the receive buffer to check.

   Remarks:
    None.
*/
static int8_t APP_SPW_CheckData(uint32_t buffer_id)
{
    int8_t res = 0;
    uint16_t packet, num_packet = 0;
    SPW_PKTRX_PREV_STATUS status = app_rx_packet_status[buffer_id];

    // If status lock : previous status is updated
    if (status & SPW_PKTRX_PREV_STATUS_LOCKED)
    {
        // Get number of received messages
        num_packet = SPW_PKTRX_PREV_STATUS_GET_COUNT(status);
        if (num_packet > 0)
        {
            for (packet = 0; packet < num_packet; packet++)
            {
                // Check received packet is not split
                if (app_rx_packet_info[buffer_id][packet].Split)
                {
                    printf("INFO : Packet %u split\r\n", (unsigned int) packet);
                    res = -1;
                }

                uint8_t* packet_buffer = (uint8_t*)(app_rx_packet_info[buffer_id][packet].DAddr);
                app_rx_num_packets++;

                if (packet_buffer[0] != app_rx_next_index_expected)
                {
                    if (packet_buffer[0] > app_rx_next_index_expected)
                        app_rx_seq_error += (packet_buffer[0] - app_rx_next_index_expected);
                    else
                        app_rx_seq_error += (app_rx_next_index_expected - packet_buffer[0]);
                    app_rx_next_index_expected = (packet_buffer[0] + 1);
                }
                else
                {
                    app_rx_next_index_expected++;
                }

                if (app_rx_next_index_expected > 0xFF)
                {
                    app_rx_next_index_expected = 0;
                }

                if ((app_rx_num_packets + app_rx_seq_error) >= (APP_TX_NUM_SEND * APP_TX_PACKET_PER_SEND))
                {
                    app_rx_is_all_data_received = true;
                }
            }
        }
    }
    else
    {
        printf("ERROR: No lock in status : 0x%x\r\n", (unsigned int)status);
    }

    return res;
}

/* void APP_SPW_Callback(SPW_INT_MASK irqStatus, uintptr_t context)

  Summary:
    Function called by SPW PLIB.

  Description:
    This function will be called by SPW PLIB when interrupt has occurred.

  Remarks:
    None.
*/
void APP_SPW_Callback(SPW_INT_MASK irqStatus, uintptr_t context)
{
    if (irqStatus & SPW_INT_MASK_PKTTX1)
    {
        SPW_PKTTX_INT_MASK status = SPW_PKTTX_IrqStatusGetMaskedAndClear();

        if (status & SPW_PKTTX_INT_MASK_DEACT)
        {
            // List deactivate, update pack index and reload list
            if ((++app_tx_num_sent) < APP_TX_NUM_SEND)
            {
                for (uint32_t pck = 0; pck < APP_TX_PACKET_PER_SEND; pck++)
                {
                    uint8_t* tx_buff = (uint8_t*) (&app_tx_data[pck][0]);
                    tx_buff[0] = (APP_TX_PACKET_PER_SEND * app_tx_num_sent) + pck;
                }

                // Unlock previous buffer status
                SPW_PKTTX_UnlockStatus();

                APP_SPW_StartSendList();
            }
            else
            {
                /* End of send */
                app_tx_is_end = true;
                /* Stop send timer for TX time calculation */
                app_tx_time_elapsed = TC0_CH1_TimerCounterGet();
                TC0_CH1_TimerStop();
            }
        }
    }

    if (irqStatus & SPW_INT_MASK_PKTRX1)
    {
        SPW_PKTRX_INT_MASK status = SPW_PKTRX_IrqStatusGetMaskedAndClear();

        if (status & SPW_PKTRX_INT_MASK_EOP)
        {
            if (!app_rx_packet_in_current)
            {
                app_rx_packet_in_current = true;

                /* Start timer for Rx flush if number of received packet is less
                 than number of packets in active list */
                TC0_CH0_TimerStart();
            }
        }

        if (status & SPW_PKTRX_INT_MASK_DEACT)
        {
            if (app_rx_packet_in_current)
            {
                /* Stop Rx flush Timer */
                TC0_CH0_TimerStop();

                app_rx_packet_in_current = false;
            }
            uint16_t next_buffer = ((app_rx_buff_list.head + 2) % APP_RX_RECV_LIST);
            if (next_buffer == app_rx_buff_list.tail)
            {
                if (app_rx_buff_list.full == 0)
                {
                    app_rx_buff_list.full = 1;
                }
            }
            else
            {
                uint16_t prev_rx_buff = app_rx_buff_list.head;
                // Increment head
                app_rx_buff_list.head = ((app_rx_buff_list.head + 1) % APP_RX_RECV_LIST);
                // set next buffer
                APP_SPW_SetNextReceiveBuffer(next_buffer, SPW_PKTRX_NXTBUF_START_LATER);
                // Get Status to unlock previous buffer
                app_rx_packet_status[prev_rx_buff] = SPW_PKTRX_GetPreviousBufferStatus();
            }
        }
    }

    if (irqStatus & SPW_INT_MASK_LINK1)
    {
        SPW_LINK_INT_MASK status = SPW_LINK_IrqStatusGetMaskedAndClear(SPW_LINK_1);
        printf("ERROR(s) on SPW Link 1 :\r\n");
        APP_SPW_PrintInterruptErrors(status);
    }

    if (irqStatus & SPW_INT_MASK_LINK2)
    {
        SPW_LINK_INT_MASK status = SPW_LINK_IrqStatusGetMaskedAndClear(SPW_LINK_2);
        printf("ERROR(s) on SPW Link 2 :\r\n");
        APP_SPW_PrintInterruptErrors(status);
    }
}

/* void TC0_CH0_Callback(TC_TIMER_STATUS status, uintptr_t context)

  Summary:
    Function called by TC PLIB.

  Description:
    This function will be called by TC PLIB when interrupt has occurred for
    channel 0.

  Remarks:
    None.
*/
void TC0_CH0_Callback(TC_TIMER_STATUS status, uintptr_t context)
{
    /* Packets were receive but didn't reach the count of current buffer number
       of packets : Deactivate current buffer (Abort with split) */
    SPW_PKTRX_CurrentPacketSplit();
}

/* void TC0_CH1_Callback(TC_TIMER_STATUS status, uintptr_t context)

  Summary:
    Function called by TC PLIB.

  Description:
    This function will be called by TC PLIB when interrupt has occurred for
    channel 1.

  Remarks:
    None.
*/
void TC0_CH1_Callback(TC_TIMER_STATUS status, uintptr_t context)
{
    printf("ERROR : Timer should not trigger interrupt during test.\r\n");
}

/* void TC0_CH2_Callback(TC_TIMER_STATUS status, uintptr_t context)

  Summary:
    Function called by TC PLIB.

  Description:
    This function will be called by TC PLIB when interrupt has occurred for
    channel 2.

  Remarks:
    None.
*/
void TC0_CH2_Callback(TC_TIMER_STATUS status, uintptr_t context)
{
    printf("ERROR : Timer should not trigger interrupt during test.\r\n");
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    printf("\n\r-----------------------------------------------------------");
    printf("\n\r  SPW loopback example                                     ");
    printf("\n\r-----------------------------------------------------------\n\r");

    /* Add SPW interrupt callback */
    SPW_CallbackRegister(APP_SPW_Callback, (uintptr_t)NULL);

    /* Register callback function for timer counters interrupt */
    TC0_CH0_TimerCallbackRegister(TC0_CH0_Callback, (uintptr_t)NULL);
    TC0_CH1_TimerCallbackRegister(TC0_CH1_Callback, (uintptr_t)NULL);
    TC0_CH2_TimerCallbackRegister(TC0_CH2_Callback, (uintptr_t)NULL);

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
    SPW_LINK_STATE spwLink1Status = 0;
    SPW_LINK_STATE spwLink2Status = 0;
    do
    {
        spwLink1Status = SPW_LINK_GET_STATE(SPW_LINK_StatusGet(SPW_LINK_1));
        spwLink2Status = SPW_LINK_GET_STATE(SPW_LINK_StatusGet(SPW_LINK_2));
    }
    while ( (spwLink1Status != SPW_LINK_STATE_RUN) &&
            (spwLink2Status != SPW_LINK_STATE_RUN) );

    printf("SPW Link in Run state\r\n");

    /* Prepare RX packet reception */
    APP_SPW_InitRx();

    /* Prepare TX packet reception */
    APP_SPW_InitTx();

    while (true)
    {
        app_spw_rx_buffer_list_desc rx_buff_list = app_rx_buff_list;
        if ((rx_buff_list.tail != rx_buff_list.head) || (rx_buff_list.full))
        {
            // If number of list buffer is 2, first unlock previous buffer
            if ( APP_RX_RECV_LIST == 2)
            {
                app_rx_packet_status[rx_buff_list.head] = SPW_PKTRX_GetPreviousBufferStatus();
            }

            /* Get timer start value for RX process time calculation */
            uint32_t start_process = TC0_CH2_TimerCounterGet();

            /* Treat received buffer to check status and received data */
            APP_SPW_CheckData(rx_buff_list.tail);

            /* Compute timer time for RX process time calculation */
            uint32_t elapsed_time = TC0_CH2_TimerCounterGet() - start_process;

            app_rx_average_process_time = (app_rx_average_process_time / app_rx_average_process_time_values)
                    * (app_rx_average_process_time_values / (app_rx_average_process_time_values + 1))
                    + (elapsed_time / (app_rx_average_process_time_values + 1));

            // Clear buffer
            app_rx_packet_status[rx_buff_list.tail] = 0;
            memset(&(app_rx_buffer_data[rx_buff_list.tail][0]), 0, sizeof(app_rx_buffer_data[rx_buff_list.tail]));
            memset(&(app_rx_packet_info[rx_buff_list.tail][0]), 0, sizeof(SPW_PKTRX_INFO) * APP_RX_PACKET_NUM);

            app_rx_buff_list.tail = (app_rx_buff_list.tail + 1) % APP_RX_RECV_LIST;

            // Set next buffer if none was set on last interrupt
            if (rx_buff_list.full)
            {
                app_rx_buff_list.full = 0;
                // Increment head
                app_rx_buff_list.head = ((app_rx_buff_list.head + 1) % APP_RX_RECV_LIST);
                // Set nex buffer
                uint16_t next_buffer = ((app_rx_buff_list.head + 1) % APP_RX_RECV_LIST);

                if ( APP_RX_RECV_LIST > 2)
                {
                    APP_SPW_SetNextReceiveBuffer(next_buffer, SPW_PKTRX_NXTBUF_START_LATER);
                    // Get Status to unlock previous buffer
                    app_rx_packet_status[rx_buff_list.head] = SPW_PKTRX_GetPreviousBufferStatus();
                }
                else
                {
                    SPW_PKTRX_NXTBUF_START next_start_condition = SPW_PKTRX_NXTBUF_START_LATER;
                    SPW_PKTRX_STATUS rx_status = SPW_PKTRX_StatusGet();
                    if (rx_status & SPW_PKTRX_STATUS_LOCKED)
                    {
                        app_rx_buff_list.full = 1;
                        next_start_condition = SPW_PKTRX_NXTBUF_START_NOW;
                    }
                    APP_SPW_SetNextReceiveBuffer(next_buffer, next_start_condition);
                }
            }
        }

        if (app_rx_is_all_data_received)
        {
            float time_us = ((float) app_rx_average_process_time / (float)(TC0_CH2_TimerFrequencyGet() / (float)1000000.0));
            printf("Average RX process time = %lu us\r\n", (unsigned long) time_us);
            printf("Rx sequence errors = %u\r\n", (unsigned int)app_rx_seq_error);
            app_rx_is_all_data_received = false;
            app_rx_seq_error = 0;
            app_rx_next_index_expected = 0;
            app_rx_num_packets = 0;
            printf("End of transfer\r\n");
            TC0_CH0_TimerStop();
            TC0_CH1_TimerStop();
            TC0_CH2_TimerStop();
        }

        if (app_tx_is_end)
        {
            app_tx_is_end = false;
            float time_us = ((float) app_tx_time_elapsed / (float)(TC0_CH1_TimerFrequencyGet() / (float)1000000.0));
            uint32_t bytes = APP_TX_NUM_SEND * APP_TX_NUM_PACKET * APP_DATA_SIZE_WORDS * 4;
            printf("TX time=%lu us, for %u bytes\r\n", (unsigned long) time_us, (unsigned int)bytes);
            float rate = ((float) bytes * 8) / (float) time_us;
            printf("Effective rate=%lu Mb/s\r\n", (unsigned long) rate);
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

