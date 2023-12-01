/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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

// *****************************************************************************
// *****************************************************************************
// Section: Defines
// *****************************************************************************
// *****************************************************************************

/* Number of Tx packet in transmitter list */
#define APP_TX_NUM_PACKET                   (1U)

#define APP_SPW_LINK_ERR_INT_MASK           ( SPW_LINK_INT_MASK_DISERR    | \
                                              SPW_LINK_INT_MASK_PARERR    | \
                                              SPW_LINK_INT_MASK_ESCERR    | \
                                              SPW_LINK_INT_MASK_CRERR     | \
                                              SPW_LINK_INT_MASK_LINKABORT | \
                                              SPW_LINK_INT_MASK_EEPTRANS  | \
                                              SPW_LINK_INT_MASK_EEPREC    | \
                                              SPW_LINK_INT_MASK_DISCARD)

// *****************************************************************************
// *****************************************************************************
// Section: Variables
// *****************************************************************************
// *****************************************************************************

/* Tx send list */
static SPW_PKTTX_SEND_LIST_ENTRY __attribute__((aligned (32)))__attribute__((section (".ram_nocache"))) app_tx_packet_send_list[APP_TX_NUM_PACKET] = {0};

/* Store number of TCH time code event */
static volatile uint8_t app_num_tc_event = 0;


// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
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
static void APP_PckTxSendEscapeCharOnTimeCode(SPW_LINK link, uint8_t escChar, uint8_t timeCode)
{
    /* Prepare send list with 1 packet with RMAP header */
    memset(&(app_tx_packet_send_list[0]), 0, sizeof(SPW_PKTTX_SEND_LIST_ENTRY)*APP_TX_NUM_PACKET);
    app_tx_packet_send_list[0].Entry = 1U;
    if ( link == SPW_LINK_1 )
    {
        app_tx_packet_send_list[0].EscMask = 0x8U;
    }
    else if ( link == SPW_LINK_2 )
    {
        app_tx_packet_send_list[0].EscMask = 0x4U;
    }
    else
    {
        /* Link ID not valid */
    }
    app_tx_packet_send_list[0].EscChar = escChar;

    /* Enable packet TX interrupts */
    SPW_PKTTX_InterruptEnable(SPW_PKTTX_INT_MASK_DEACT);

    /* Prepare next send list without router bytes. */
    SPW_PKTTX_SetNextSendList(
        NULL,
        &(app_tx_packet_send_list[0]),
        APP_TX_NUM_PACKET,
        true,
        SPW_PKTTX_NXTSEND_START_TCH1,
        timeCode);
}

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
    uint32_t distStatus = 0UL;
    
    if ( (irqStatus & SPW_INT_MASK_PKTTX1) != 0U )
    {
        SPW_PKTTX_INT_MASK status = SPW_PKTTX_IrqStatusGetMaskedAndClear();

        if ( (status & SPW_PKTTX_INT_MASK_DEACT) != 0U )
        {
            SPW_PKTTX_UnlockStatus();
            
            printf("  - Reload next PKTTX\r\n");
            APP_PckTxSendEscapeCharOnTimeCode(SPW_LINK_1, 0x84, 0x04);
        }
    }

    if ( (irqStatus & SPW_INT_MASK_LINK1) != 0U )
    {
        SPW_LINK_INT_MASK status = SPW_LINK_IrqStatusGetMaskedAndClear(SPW_LINK_1);

        if ( (status & SPW_LINK_INT_MASK_ESCEVENT1) != 0U )
        {
            printf("  - Link 1 Esc Event 1 : 0x%02X\r\n", (unsigned int)SPW_LINK_LastRecvEscapeCharEvent1Get(SPW_LINK_1));
        }

        if ( (status & SPW_LINK_INT_MASK_ESCEVENT2) != 0U )
        {
            printf("  - Link 1 Esc Event 2 : 0x%02X\r\n", (unsigned int)SPW_LINK_LastRecvEscapeCharEvent2Get(SPW_LINK_1));
        }
        
        if ( ( status & APP_SPW_LINK_ERR_INT_MASK) != 0U )
        {
            printf("ERROR(s) on SPW Link 1 :\r\n");
            APP_SPW_PrintInterruptErrors(status);
        }
    }

    if ( (irqStatus & SPW_INT_MASK_LINK2) != 0U )
    {
        SPW_LINK_INT_MASK status = SPW_LINK_IrqStatusGetMaskedAndClear(SPW_LINK_2);
        
        if ( (status & SPW_LINK_INT_MASK_ESCEVENT1) != 0U )
        {
            printf("  - Link 2 Esc Event 1 : 0x%02X\r\n", (unsigned int)SPW_LINK_LastRecvEscapeCharEvent1Get(SPW_LINK_2));
        }
        if ( (status & SPW_LINK_INT_MASK_ESCEVENT2) != 0U )
        {
            printf("  - Link 2 Esc Event 2 : 0x%02X\r\n", (unsigned int)SPW_LINK_LastRecvEscapeCharEvent2Get(SPW_LINK_2));
        }
        
        if ( ( status & APP_SPW_LINK_ERR_INT_MASK) != 0U )
        {
            printf("  - ERROR(s) on SPW Link 2 :\r\n");
            APP_SPW_PrintInterruptErrors(status);
        }
    }
    
    if ( (irqStatus & SPW_INT_MASK_DI1) != 0U )
    {
        distStatus = SPW_LINK_DistIrqStatusGetMaskedAndClear(SPW_LINK_1);
        if (distStatus != 0U)
        {
            printf("  - Link1 DI=0x%08X\r\n", (unsigned int)distStatus);
        }
    }
    
    if ( (irqStatus & SPW_INT_MASK_DIA1) != 0U )
    {
        distStatus = SPW_LINK_DistAckIrqStatusGetMaskedAndClear(SPW_LINK_1);
        if (distStatus != 0U)
        {
            printf("  - Link1 DIA=0x%08X\r\n", (unsigned int)distStatus);
        }
    }
    
    if ( (irqStatus & SPW_INT_MASK_DI2) != 0U )
    {
        distStatus = SPW_LINK_DistIrqStatusGetMaskedAndClear(SPW_LINK_2);
        if (distStatus != 0U)
        {
            printf("  - Link2 DI=0x%08X\r\n", (unsigned int)distStatus);
        }
    }
    
    if ( (irqStatus & SPW_INT_MASK_DIA2) != 0U )
    {
        distStatus = SPW_LINK_DistAckIrqStatusGetMaskedAndClear(SPW_LINK_2);
        if (distStatus != 0U)
        {
            printf("  - Link2 DIA=0x%08X\r\n", (unsigned int)distStatus);  
        }
    }
    
    if ( (irqStatus & SPW_INT_MASK_TCH) != 0U )
    {
        SPW_TCH_INT_MASK tchStatus = SPW_TCH_IrqStatusGetMaskedAndClear();
        if ( (tchStatus & SPW_TCH_INT_MASK_TIMECODE) != 0U )
        {
            printf("  - TC=0x%02X\r\n", (unsigned int)SPW_TCH_LastTimeCodeGet());
        }
        if ( (tchStatus & SPW_TCH_INT_MASK_TCEVENT) != 0U )
        {
            printf("  - TC event\r\n");
            app_num_tc_event++;
        }
    }

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
    printf("\n\r  SPW EscChar and Time Code Handler loopback example       ");
    printf("\n\r-----------------------------------------------------------\n\r");

    /* Add SPW interrupt callback */
    SPW_CallbackRegister(APP_SPW_Callback, (uintptr_t)NULL);

    /* Enable SPW Link interrupts */
    SPW_LINK_InterruptEnable(SPW_LINK_1 , SPW_LINK_INT_MASK_ESCEVENT2 |
                                          SPW_LINK_INT_MASK_ESCEVENT1 |
                                          SPW_LINK_INT_MASK_DISERR |
                                          SPW_LINK_INT_MASK_PARERR |
                                          SPW_LINK_INT_MASK_ESCERR |
                                          SPW_LINK_INT_MASK_CRERR |
                                          SPW_LINK_INT_MASK_LINKABORT |
                                          SPW_LINK_INT_MASK_EEPTRANS |
                                          SPW_LINK_INT_MASK_EEPREC |
                                          SPW_LINK_INT_MASK_DISCARD);
    SPW_LINK_InterruptEnable(SPW_LINK_2 , SPW_LINK_INT_MASK_ESCEVENT2 |
                                          SPW_LINK_INT_MASK_ESCEVENT1 |
                                          SPW_LINK_INT_MASK_DISERR |
                                          SPW_LINK_INT_MASK_PARERR |
                                          SPW_LINK_INT_MASK_ESCERR |
                                          SPW_LINK_INT_MASK_CRERR |
                                          SPW_LINK_INT_MASK_LINKABORT |
                                          SPW_LINK_INT_MASK_EEPTRANS |
                                          SPW_LINK_INT_MASK_EEPREC |
                                          SPW_LINK_INT_MASK_DISCARD);

    /* Enable all SPW Link distributed interrupts */
    SPW_LINK_DistInterruptEnable(SPW_LINK_1, SPW_LINK_DIST_INT_MASK_ALL);
    SPW_LINK_DistInterruptEnable(SPW_LINK_2, SPW_LINK_DIST_INT_MASK_ALL);
    SPW_LINK_DistAckInterruptEnable(SPW_LINK_1, SPW_LINK_DIST_ACK_MASK_ALL);
    SPW_LINK_DistAckInterruptEnable(SPW_LINK_2, SPW_LINK_DIST_ACK_MASK_ALL);
    
    /* Enable SPW TCH interrupts */
    SPW_TCH_InterruptEnable(SPW_TCH_INT_MASK_TCEVENT | SPW_TCH_INT_MASK_TIMECODE);
    
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
    
    printf("Set link 2 escape character match event 1 for time code 0x22\r\n");
    SPW_LINK_EscapeCharEvent1Set(SPW_LINK_2, true, 0xFF, 0x22);

    printf("Transmit escape character (time code 0x22) on link 1\r\n");
    SPW_LINK_TransmitEscapeChar(SPW_LINK_1, 0x22);

    printf("Configure TCH to send Time Code on Link 1\r\n");
    SPW_TCH_LinkSenderSet(SPW_TCH_SEL_LINK_MASK_L1);

    printf("Configure Time Code event to count time code '0x10' occurrences\r\n");
    SPW_TCH_ConfigureTcEvent(0xFF, 0x10);
    app_num_tc_event = 0U;
    
    printf("Prepare send of distributed interrupt '4' with send list that start on matching time code 0x04\r\n");
    APP_PckTxSendEscapeCharOnTimeCode(SPW_LINK_1, 0x84, 0x04);

    printf("Manually send time code '0x04' now to trigger TX send\r\n");
    SPW_TCH_LastTimeCodeSet(0x04, true);
    
    printf("Configure TCH to send Time code using RTCOUT0 event and restart every second\r\n");
    SPW_TCH_ConfigureEvent(SPW_SYNC_EVENT_MASK_RTCOUT0);
    SPW_TCH_ConfigureRestart(0, false, SPW_TCH_CFG_RESTART_IN_PPS, 0);

    while ( app_num_tc_event < 2U )
    {
        /* Wait events */
    }

    printf("Stop TCH event and restart\r\n");
    SPW_TCH_ConfigureEvent(0);
    SPW_TCH_ConfigureRestart(0, false, SPW_TCH_CFG_RESTART_IN_PPS, 0);
    
    while ( true );

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

