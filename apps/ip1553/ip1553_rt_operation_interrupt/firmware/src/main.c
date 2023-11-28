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

// *****************************************************************************
// *****************************************************************************
// Section: Application defines
// *****************************************************************************
// *****************************************************************************
/* Define the bitfield mask of used 1553 buffers by the application */
#define APP_IP1553_BUFFER_USED              (IP1553_BUFFER_TO_BITFIELD_SA (0) |\
                                             IP1553_BUFFER_TO_BITFIELD_SA (1) |\
                                             IP1553_BUFFER_TO_BITFIELD_SA (2) |\
                                             IP1553_BUFFER_TO_BITFIELD_SA (3) |\
                                             IP1553_BUFFER_TO_BITFIELD_SA (31) )

/* Define bitfield mask for mode command interrupt indications */
#define APP_IP1553_MC_INT_IND               (IP1553_INT_MASK_OTF |\
                                             IP1553_INT_MASK_ITF |\
                                             IP1553_INT_MASK_RRT |\
                                             IP1553_INT_MASK_SWD |\
                                             IP1553_INT_MASK_SDR |\
                                             IP1553_INT_MASK_OSR |\
                                             IP1553_INT_MASK_TSR |\
                                             IP1553_INT_MASK_STR |\
                                             IP1553_INT_MASK_DBR |\
                                             IP1553_INT_MASK_TVR )

/* Define the switch state press : Active LOW switch */
#define APP_SWITCH_PRESSED_STATE           (0U)
/* Define the switch button PB0 flag */
#define APP_ID_BUTTON0                     (0x01U)
/* Define the switch button PB1 flag */
#define APP_ID_BUTTON1                     (0x02U)
/* Define the switch button PB2 flag */
#define APP_ID_BUTTON2                     (0x04U)

// *****************************************************************************
// *****************************************************************************
// Section: Application types
// *****************************************************************************
// *****************************************************************************
/* Application IP1553 Button 0 states

   Summary:
    Button 0 states types.

   Description:
    This data type identifies the button 0 rotating states.

   Remarks:
    None.
*/
typedef uint8_t APP_IP1553_BP0_STATES;

#define APP_IP1553_BP0_STATES_NONE                     (0U)
#define APP_IP1553_BP0_STATES_BCE                      (1U)
#define APP_IP1553_BP0_STATES_SREQ                     (2U)
#define APP_IP1553_BP0_STATES_BUSY                     (3U)
#define APP_IP1553_BP0_STATES_SUBSYSTEM                (4U)
#define APP_IP1553_BP0_STATES_TR                       (5U)


// *****************************************************************************
/* Application IP1553 Button 0 states

   Summary:
    Button 0 states types.

   Description:
    This data type identifies the button 0 rotating states.

   Remarks:
    None.
*/
typedef uint8_t APP_IP1553_BP1_STATES;

#define APP_IP1553_BP1_VECT_ABCD_BIT_FEED              (0U)
#define APP_IP1553_BP1_VECT_1234_BIT_CAFE              (1U)


// *****************************************************************************
// *****************************************************************************
// Section: Globals
// *****************************************************************************
// *****************************************************************************
/* Allocation of receive buffer for IP1553 */
static uint16_t IP1553RxBuffersRAM[IP1553_BUFFERS_NUM][IP1553_BUFFERS_SIZE] __attribute__((aligned (32)))__attribute__((section (".ram_nocache")));

/* Allocation of transmit buffer for IP1553 */
static uint16_t IP1553TxBuffersRAM[IP1553_BUFFERS_NUM][IP1553_BUFFERS_SIZE] __attribute__((aligned (32)))__attribute__((section (".ram_nocache")));

/* Bit field of reception buffer that are marked full */
static volatile uint32_t activeRxBuffers = 0;

/* Bit field of reception buffer that are marked sent */
static volatile uint32_t activeTxBuffers = 0;

/* Bit field of status containing the mode command interrupts indications */
static volatile uint32_t activeModeCommandInd = 0;

/* Bit field of status containing the error interrupts that were triggered during transfer */
static volatile uint32_t transferErrors = 0;

/* Variable containing the bitfield of the pressed button status */
static volatile uint8_t buttonStatus = 0;

// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************
/* 
 Function:
  static void APP_IP1553_Print_Errors(uint32_t errors)

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
/*
 Function:
  void APP_IP1553_PrintModeCommandInd(uint32_t status)

 Summary:
  Print the mode command interrupt informations that are set in the status
  bit field.

 Description:
  Function called by the application to print the description of the mode
  command interrupt informations that occurred.

 Parameters:
  status - Bit field of the IP1553 status that contains the mode
  command interrupt informations.

 Remarks:
 None.
 */
static void APP_IP1553_PrintModeCommandInd(uint32_t status)
{
    if ( (status & IP1553_INT_MASK_OTF) == IP1553_INT_MASK_OTF )
    {
        printf("MC : Override Inhibit Terminal Flag.\r\n");
    }
    if ( (status & IP1553_INT_MASK_ITF) == IP1553_INT_MASK_ITF )
    {
        printf("MC : Inhibit Terminal Flag.\r\n");
    }
    if ( (status & IP1553_INT_MASK_RRT) == IP1553_INT_MASK_RRT )
    {
        printf("MC : Reset Remote Terminal.\r\n");
    }
    if ( (status & IP1553_INT_MASK_SWD) == IP1553_INT_MASK_SWD )
    {
        printf("MC : Synchronize Without Data Word.\r\n");
    }
    if ( (status & IP1553_INT_MASK_SDR) == IP1553_INT_MASK_SDR )
    {
        printf("MC : Synchronize With Data Word.\r\n");
    }
    if ( (status & IP1553_INT_MASK_OSR) == IP1553_INT_MASK_OSR )
    {
        printf("MC : Override Selected Transmitter Shutdown.\r\n");
    }
    if ( (status & IP1553_INT_MASK_TSR) == IP1553_INT_MASK_TSR )
    {
        printf("MC : Selected Transmitter Shutdown.\r\n");
    }
    if ( (status & IP1553_INT_MASK_STR) == IP1553_INT_MASK_STR )
    {
        printf("MC : Initiate Self-Test.\r\n");
    }
    if ( (status & IP1553_INT_MASK_DBR) == IP1553_INT_MASK_DBR )
    {
        printf("MC : Dynamic Bus Control.\r\n");
    }
    if ( (status & IP1553_INT_MASK_TVR) == IP1553_INT_MASK_TVR )
    {
        printf("MC : Transmit Vector Word.\r\n");
    }
}

// *****************************************************************************
/* 
 Function
  static void APP_IP1553_Callback(uintptr_t context)

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
    
    /* Get active Rx buffers if receive data interrupt occurs */
    if ( (readStatus & IP1553_INT_MASK_ERX) == IP1553_INT_MASK_ERX )
    {
        uint32_t buffersStatusRx = IP1553_GetRxBuffersStatus();
        /* Store Rx buffer status for processing */
        activeRxBuffers = ~buffersStatusRx & APP_IP1553_BUFFER_USED;
    }

    /* Get active Tx buffers if transmit data interrupt occurs */
    if ( (readStatus & IP1553_INT_MASK_ETX) == IP1553_INT_MASK_ETX )
    {
        uint32_t buffersStatusTx = IP1553_GetTxBuffersStatus();
        /* Store Tx buffer status for processing */
        activeTxBuffers = ~buffersStatusTx & APP_IP1553_BUFFER_USED;
    }

    /* Store RT mode command indications */
    if ( (readStatus & APP_IP1553_MC_INT_IND) != 0U )
    {
        activeModeCommandInd = readStatus & APP_IP1553_MC_INT_IND;
    }

    /* Store transfer error interrupt status if errors occurs */
    if ( (readStatus & IP1553_INT_MASK_ERROR_MASK) != 0U )
    {
        transferErrors = readStatus & IP1553_INT_MASK_ERROR_MASK;
    }

    NVIC_ClearPendingIRQ(IP1553_IRQn);
}

// *****************************************************************************
/* Function:
    static void APP_ControlSwitch(PIO_PIN pin, uintptr_t context)

   Summary:
    Interrupt callback for PIO interrupt.

   Description:
    This function update the global Bitfield to indicate press event occurs
    on button.

   Parameters:
    pin - pio structure of the pin that triggers the interrupt.
    context - pointer to application context object.

   Returns:
    None.
*/
static void APP_ControlSwitch(PIO_PIN pin, uintptr_t context)
{
    if ( pin == SWITCH0_PIN )
    {
        if ( SWITCH0_Get() == APP_SWITCH_PRESSED_STATE )
        {
            buttonStatus |= APP_ID_BUTTON0;
    	}
    }
    else if ( pin == SWITCH1_PIN )
    {
        if ( SWITCH1_Get() == APP_SWITCH_PRESSED_STATE )
        {
            buttonStatus |= APP_ID_BUTTON1;
        }
    }
    else if ( pin == SWITCH2_PIN )
    {
        if ( SWITCH2_Get() == APP_SWITCH_PRESSED_STATE )
        {
            buttonStatus |= APP_ID_BUTTON2;
        }
    }
    else
    {
        /* Pin not handled */
    }
}

// *****************************************************************************
/* Function:
    void APP_IP1553_HandleButtonEvents(uint32_t buttons)

   Summary:
    Interrupt callback for PIO interrupt.

   Description:
    This function update the global Bitfield to indicate press event occurs
    on button.

   Parameters:
    buttons - Bitfield of pressed buttons.

   Returns:
    None.
*/
static void APP_IP1553_HandleButtonEvents(uint32_t buttons)
{
    static APP_IP1553_BP0_STATES statePb0 = APP_IP1553_BP0_STATES_NONE;
    static APP_IP1553_BP1_STATES statePb1 = APP_IP1553_BP1_VECT_ABCD_BIT_FEED;

    if ( ( buttons & APP_ID_BUTTON0 ) != 0U )
    {
        if (statePb0 == APP_IP1553_BP0_STATES_NONE)
        {
            printf("-> Set config : BCE=0, SREQ=0, Busy=0, SubSystem=0, TR=0\n\r");
            IP1553_BCEnableCmdSet(false);
            IP1553_SREQBitCmdSet(false);
            IP1553_BusyBitCmdSet(false);
            IP1553_SSBitCmdSet(false);
            IP1553_TRBitCmdSet(false);
        }
        else if (statePb0 == APP_IP1553_BP0_STATES_BCE)
        {
            printf("-> Set config : BCE=1, SREQ=0, Busy=0, SubSystem=0, TR=0\n\r");
            IP1553_BCEnableCmdSet(true);
            IP1553_SREQBitCmdSet(false);
            IP1553_BusyBitCmdSet(false);
            IP1553_SSBitCmdSet(false);
            IP1553_TRBitCmdSet(false);
        }
        else if (statePb0 == APP_IP1553_BP0_STATES_SREQ)
        {
            printf("-> Set config : BCE=0, SREQ=1, Busy=0, SubSystem=0, TR=0\n\r");
            IP1553_BCEnableCmdSet(false);
            IP1553_SREQBitCmdSet(true);
            IP1553_BusyBitCmdSet(false);
            IP1553_SSBitCmdSet(false);
            IP1553_TRBitCmdSet(false);
        }
        else if (statePb0 == APP_IP1553_BP0_STATES_BUSY)
        {
            printf("-> Set config : BCE=0, SREQ=0, Busy=1, SubSystem=0, TR=0\n\r");
            IP1553_BCEnableCmdSet(false);
            IP1553_SREQBitCmdSet(false);
            IP1553_BusyBitCmdSet(true);
            IP1553_SSBitCmdSet(false);
            IP1553_TRBitCmdSet(false);
        }
        else if (statePb0 == APP_IP1553_BP0_STATES_SUBSYSTEM)
        {
            printf("-> Set config : BCE=0, SREQ=0, Busy=0, SubSystem=1, TR=0\n\r");
            IP1553_BCEnableCmdSet(false);
            IP1553_SREQBitCmdSet(false);
            IP1553_BusyBitCmdSet(false);
            IP1553_SSBitCmdSet(true);
            IP1553_TRBitCmdSet(false);
        }
        else if (statePb0 == APP_IP1553_BP0_STATES_TR)
        {
            printf("-> Set config : BCE=0, SREQ=0, Busy=0, SubSystem=0, TR=1\n\r");
            IP1553_BCEnableCmdSet(false);
            IP1553_SREQBitCmdSet(false);
            IP1553_BusyBitCmdSet(false);
            IP1553_SSBitCmdSet(false);
            IP1553_TRBitCmdSet(true);
        }
        else
        {
            /* State not handled */
        }

        statePb0++;
        if ( statePb0 > APP_IP1553_BP0_STATES_TR)
        {
            statePb0 = APP_IP1553_BP0_STATES_NONE;
        }
    }

    if ( ( buttons & APP_ID_BUTTON1 ) != 0U )
    {
        if (statePb1 == APP_IP1553_BP1_VECT_ABCD_BIT_FEED)
        {
            printf("-> Set vector word = 0xABCD and BIT word = 0xFEED\n\r");
            IP1553_VectorWordSet(0xABCD);
            IP1553_BitWordSet(0xFEED);
        }
        else if (statePb1 == APP_IP1553_BP1_VECT_1234_BIT_CAFE)
        {
            printf("-> Set vector word = 0x1234 and BIT word = 0xCAFE\n\r");
            IP1553_VectorWordSet(0x1234);
            IP1553_BitWordSet(0xCAFE);
        }
        else
        {
            /* State not handled */
        }

        statePb1++;
        if ( statePb1 > APP_IP1553_BP1_VECT_1234_BIT_CAFE)
        {
            statePb1 = APP_IP1553_BP1_VECT_ABCD_BIT_FEED;
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void)
{
    /* Initialize all modules */
    SYS_Initialize(NULL);

    printf("\n\r-----------------------------------------------------------");
    printf("\n\r  IP1553 - RT mode interrupt operation example             ");
    printf("\n\r-----------------------------------------------------------\n\r");
    
    PIO_PinInterruptCallbackRegister(SWITCH0_PIN, APP_ControlSwitch, (uintptr_t)NULL);
    PIO_PinInterruptCallbackRegister(SWITCH1_PIN, APP_ControlSwitch, (uintptr_t)NULL);
    PIO_PinInterruptCallbackRegister(SWITCH2_PIN, APP_ControlSwitch, (uintptr_t)NULL);
    PIO_PinInterruptEnable(SWITCH0_PIN);
    PIO_PinInterruptEnable(SWITCH1_PIN);
    PIO_PinInterruptEnable(SWITCH2_PIN);

    /* Set buffers Configuration */
    IP1553_BuffersConfigSet(&IP1553TxBuffersRAM[0][0], &IP1553RxBuffersRAM[0][0]);

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

    /* Set 1553 application callback on interrupt event */
    IP1553_CallbackRegister(APP_IP1553_Callback, (uintptr_t) NULL);

    /* Reset Tx and Rx status for buffers 0, 1, 2, 3 and 31 */
    IP1553_ResetTxBuffersStatus(APP_IP1553_BUFFER_USED);
    IP1553_ResetRxBuffersStatus(APP_IP1553_BUFFER_USED);

    /* Prepare content in buffers to send*/
    for (uint8_t i = 0U; i < IP1553_BUFFERS_NUM; i++)
    {
        for (uint8_t j = 0U; j < IP1553_BUFFERS_SIZE; j++)
        {
            IP1553TxBuffersRAM[i][j] = (uint16_t)( ((uint16_t)i << 12U) + (j + 1U) );
        }
    }
    
    /* Simulate first push button to set default state */
    APP_IP1553_HandleButtonEvents(APP_ID_BUTTON0 | APP_ID_BUTTON1);

    printf("\n\rMIL1553 RT mode, wait for BC commands\r\n");

    while ( true )
    {
        /* Check if there was error during transfer */
        if ( transferErrors != 0U )
        {
            APP_IP1553_Print_Errors(transferErrors);
            transferErrors = 0U;
        }

        /* Print and reset Rx buffers*/
        if ( activeRxBuffers != 0U )
        {
            uint32_t lastActiveBuffers = activeRxBuffers;
            activeRxBuffers = 0U;
            uint32_t buffer = 0UL;
            while ( lastActiveBuffers != 0U )
            {
                while ( ( lastActiveBuffers & 0x1U ) == 0U )
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

        /* Reset Tx buffers*/
        if ( activeTxBuffers != 0U )
        {
            uint32_t lastActiveBuffers = activeTxBuffers;
            activeTxBuffers = 0U;
            uint32_t buffer = 0UL;
            while ( lastActiveBuffers != 0U )
            {
                while ( ( lastActiveBuffers & 0x1U ) == 0U )
                {
                    buffer++;
                    lastActiveBuffers >>= 1U;
                }

                /* Reset Tx buffer status */
                IP1553_ResetTxBuffersStatus(IP1553_BUFFER_TO_BITFIELD_SA(buffer));

                printf("TX buffer reset: %u\r\n", (unsigned int) buffer);

                /* Go to next bit in lastActiveBuffer bit field */
                buffer++;
                lastActiveBuffers >>= 1U;
            }
        }
        
        /* Display mode command interrupt indications */
        if ( activeModeCommandInd != 0U )
        {
            APP_IP1553_PrintModeCommandInd(activeModeCommandInd);
            
            /* If synchronize with data word is received, print received data */
            if ( (activeModeCommandInd & IP1553_INT_MASK_SDR) == IP1553_INT_MASK_SDR )
            {
                printf("  Received word : 0x%04X\r\n", IP1553_GetFirstStatusWord());
            }

            activeModeCommandInd = 0U;
        }

        /* Handle buttons events */
        if ( buttonStatus != 0U )
        {
            uint8_t button = buttonStatus;
            APP_IP1553_HandleButtonEvents(button);
            buttonStatus &= ~(button & 0xFFU);
        }
    }

    /* Execution should not come here during normal operation */

    return (EXIT_FAILURE);
}

/*******************************************************************************
 End of File
 */

