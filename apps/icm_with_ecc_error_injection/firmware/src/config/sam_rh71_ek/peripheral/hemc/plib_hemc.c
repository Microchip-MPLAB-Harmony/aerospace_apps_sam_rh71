/*****************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    plib_hemc.c

  Summary:
    HEMC PLIB Source file

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
#include <string.h>
#include "device.h"
#include "plib_hemc.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

static volatile HEMC_OBJ hemcObj;

// *****************************************************************************
// *****************************************************************************
// Section: HEMC Implementation
// *****************************************************************************
// *****************************************************************************

static void SW_DelayUs(uint32_t delay)
{
    uint32_t i, count;

    /* delay * (CPU_FREQ/1000000) / 6 */
    count = delay *  (100000000U/1000000U)/6U;

    /* 6 CPU cycles per iteration */
    for (i = 0; i < count; i++)
    {
        __NOP();
    }
}


void HSDRAMC_Initialize( void )
{
    uint8_t i;
    uint16_t *pSdramBaseAddress = (uint16_t *)0x64000000;

    /* Step 1:
     * Configure SDRAM features and timing parameters */
    HSDRAMC_REGS->HSDRAMC_CR = HSDRAMC_CR_NC_COL9 | HSDRAMC_CR_NR_ROW13 | HSDRAMC_CR_NB_BANK4\
                                                | HSDRAMC_CR_CAS(3) ;

    HSDRAMC_REGS->HSDRAMC_SDR = HSDRAMC_SDR_TRAS(5) |  HSDRAMC_SDR_TRP(2) |  HSDRAMC_SDR_TRC_TRFC(6) \
                                                | HSDRAMC_SDR_TRCD(2) | HSDRAMC_SDR_TWR(2) | HSDRAMC_SDR_TXSR(7);
    HSDRAMC_REGS->HSDRAMC_CFR1= HSDRAMC_CFR1_TMRD(2) | HSDRAMC_CFR1_UNAL_Msk;

    /* Step 2:
     * A pause of at least 200 us must be observed before a signal toggle */
    SW_DelayUs(200);

    /* Step 3:
     * Issue a NOP command to the SDRAM device by writing to the Mode Register.
     * Read back the Mode Register and add a memory barrier assembly instruction just after the read.
     * Write to any SDRAM address to acknowledge this command. Now the clock which drives SDRAM device is enabled */

    HSDRAMC_REGS->HSDRAMC_MR = HSDRAMC_MR_MODE_NOP;
    HSDRAMC_REGS->HSDRAMC_MR;
    __DMB();
    *pSdramBaseAddress = 0x0;

    /* Step 4:
     * Issue an All banks precharge command by writing to the Mode Register.
     * Read back the Mode Register and add a memory barrier assembly instruction just after the read.
     * Write to any SDRAM address to acknowledge this command. */

    HSDRAMC_REGS->HSDRAMC_MR = HSDRAMC_MR_MODE_ALLBANKS_PRECHARGE;
    HSDRAMC_REGS->HSDRAMC_MR;
    __DMB();
    *pSdramBaseAddress = 0x0;

    /* Step 5:
     * Issue 8 Auto Refresh(CBR) cycles by writing to the Mode Register.
     * Read back the Mode Register and add a memory barrier assembly instruction just after the read.
     * Perform a write access to any SDRAM address 8 times. */
    HSDRAMC_REGS->HSDRAMC_MR = HSDRAMC_MR_MODE_AUTO_REFRESH;
    HSDRAMC_REGS->HSDRAMC_MR;
    __DMB();
    for (i = 0; i < 8U; i++)
    {
        *pSdramBaseAddress = i;
    }

    /* Step 6:
     * Issue Mode Register Set(MRS) to program the parameters of the SDRAM device, in particular CAS latency and burst length */
    HSDRAMC_REGS->HSDRAMC_MR = HSDRAMC_MR_MODE_LOAD_MODEREG;
    HSDRAMC_REGS->HSDRAMC_MR;
    __DMB();
    *((uint16_t *)(pSdramBaseAddress + 0x0)) = 0;

    /* Step 7:
     * Transition the SDRAM to NORMAL mode. Read back the Mode
     * Register and add a memory barrier assembly instruction just after the
     * read. Perform a write access to any SDRAM address. */
    HSDRAMC_REGS->HSDRAMC_MR = HSDRAMC_MR_MODE_NORMAL;
    HSDRAMC_REGS->HSDRAMC_MR;
    __DMB();
    *pSdramBaseAddress = 0x0;

    /* Step 8:
     * Configure the Refresh Timer Register. */
    HSDRAMC_REGS->HSDRAMC_TR = 195;


}








void HEMC_Initialize( void )
{
    /* Read NCS0 Pin configuration for HECC */
    uint8_t eccEnableDefault =(uint8_t)( (HEMC_REGS->HEMC_HECC_CR0 & HEMC_HECC_CR0_ENABLE_Msk) >> HEMC_HECC_CR0_ENABLE_Pos);
    uint8_t eccAlgoDefault = (uint8_t)( (HEMC_REGS->HEMC_HECC_CR0 & HEMC_HECC_CR0_ECC12_ENABLE_Msk) >> HEMC_HECC_CR0_ECC12_ENABLE_Pos);

    /* Enable NCS0 configuration modification through registers */
    HEMC_REGS->HEMC_CR_NCS0 |= HEMC_CR_NCS0_WRITE_ECC_CONF(1U);

    HEMC_REGS->HEMC_CR_NCS0 = HEMC_CR_NCS0_TYPE(0U) |
                              HEMC_CR_NCS0_ADDBASE(0x3ffffU) |
                              HEMC_CR_NCS0_BANKSIZE(0x1FU) |
                              HEMC_CR_NCS0_WRITE_ECC_CONF(1U) |
                              HEMC_CR_NCS0_ECC_ENABLE((uint32_t)eccEnableDefault) |
                              HEMC_CR_NCS0_ECC12_ENABLE((uint32_t)eccAlgoDefault);

    HEMC_REGS->HEMC_CR_NCS4 = HEMC_CR_NCS4_TYPE(1) |
                              HEMC_CR_NCS4_ADDBASE(0x1000) |
                              HEMC_CR_NCS4_BANKSIZE(0xC) |
                              HEMC_CR_NCS4_ECC_ENABLE(1);
    HSDRAMC_Initialize();

    /* For RAM memories on NCS4, perform memory initialization of ECC check bit */
    (void) memset((uint32_t*)(0x64000000), 0x00, 0x100000);
    if (DATA_CACHE_IS_ENABLED() != 0U)
    {
        DCACHE_CLEAN_INVALIDATE_BY_ADDR((uint32_t*)(0x64000000), 0x100000);
    }

    /* Wait all memory is zeroized and clear previous interrupts when memory ECC wasn't initialized */
    __DSB();
    __ISB();
    HEMC_HECC_STATUS dummyStatus = HEMC_HeccGetStatus();
    /* Ignore the warning */
    (void)dummyStatus;

    // Enable interrupts
    HEMC_REGS->HEMC_HECC_IER = (HEMC_HECC_IER_MEM_FIX_Msk | HEMC_HECC_IER_MEM_NOFIX_Msk);

} /* HEMC_Initialize */

// *****************************************************************************
/* Function:
    void HEMC_DisableECC(uint8_t chipSelect);

   Summary:
    Disable the ECC for the given chip select.

   Precondition:
    None.

   Parameters:
    chipSelect - The chip select for which ECC is disabled.

   Returns:
    True if ECC was disable for this chip select, False otherwise.
*/
bool HEMC_DisableECC(uint8_t chipSelect)
{
    bool ret = false;
    volatile uint32_t* pHemcCrNcsReg = NULL;
    uint32_t hemcCrEnableMask = 0;
    bool DisEccCheck = true;

    switch (chipSelect)
    {
        case 0:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS0);
            hemcCrEnableMask = HEMC_CR_NCS0_ECC_ENABLE_Msk;
            break;
        }
        case 1:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS1);
            hemcCrEnableMask = HEMC_CR_NCS1_ECC_ENABLE_Msk;
            break;
        }
        case 2:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS2);
            hemcCrEnableMask = HEMC_CR_NCS2_ECC_ENABLE_Msk;
            break;
        }
        case 3:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS3);
            hemcCrEnableMask = HEMC_CR_NCS3_ECC_ENABLE_Msk;
            break;
        }
        case 4:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS4);
            hemcCrEnableMask = HEMC_CR_NCS4_ECC_ENABLE_Msk;
            break;
        }
        case 5:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS5);
            hemcCrEnableMask = HEMC_CR_NCS5_ECC_ENABLE_Msk;
            break;
        }
        default:
            DisEccCheck = false;
            break;
    }

    if( DisEccCheck == false)
    {
        return DisEccCheck;
    }
    if ( (*pHemcCrNcsReg & hemcCrEnableMask) == hemcCrEnableMask)
    {
        *pHemcCrNcsReg &= ~(hemcCrEnableMask);
        while((*pHemcCrNcsReg & hemcCrEnableMask) == hemcCrEnableMask)
        {
            /* Nothing to do */
        }
        ret = true;
    }

    return ret;
}

// *****************************************************************************
/* Function:
    void HEMC_EnableECC(uint8_t chipSelect);

   Summary:
    Enable the ECC for the given chip select.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    True if ECC was enable for this chip select, False otherwise.
*/
bool HEMC_EnableECC(uint8_t chipSelect)
{
    bool ret = false, EnEccCheck = true;
    volatile uint32_t* pHemcCrNcsReg = NULL;
    uint32_t hemcCrEnableMask = 0;

    switch (chipSelect)
    {
        case 0:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS0);
            hemcCrEnableMask = HEMC_CR_NCS0_ECC_ENABLE_Msk;
            break;
        }
        case 1:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS1);
            hemcCrEnableMask = HEMC_CR_NCS1_ECC_ENABLE_Msk;
            break;
        }
        case 2:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS2);
            hemcCrEnableMask = HEMC_CR_NCS2_ECC_ENABLE_Msk;
            break;
        }
        case 3:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS3);
            hemcCrEnableMask = HEMC_CR_NCS3_ECC_ENABLE_Msk;
            break;
        }
        case 4:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS4);
            hemcCrEnableMask = HEMC_CR_NCS4_ECC_ENABLE_Msk;
            break;
        }
        case 5:
        {
            pHemcCrNcsReg = &(HEMC_REGS->HEMC_CR_NCS5);
            hemcCrEnableMask = HEMC_CR_NCS5_ECC_ENABLE_Msk;
            break;
        }
        default:
             EnEccCheck = false;
             break;
    }

    if( EnEccCheck == false)
    {
        return EnEccCheck;
    }

    *pHemcCrNcsReg |= hemcCrEnableMask;
    while((*pHemcCrNcsReg & hemcCrEnableMask) != hemcCrEnableMask)
    {
        /* Nothing to do */
    }
    ret = true;

    return ret;
}

// *****************************************************************************
/* Function:
    void HEMC_HeccGetStatus(void)

   Summary:
    Get the HECC status of the HEMC peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Current status of the HEMC peripheral.
*/
HEMC_HECC_STATUS HEMC_HeccGetStatus(void)
{
    return (HEMC_HECC_STATUS)(HEMC_REGS->HEMC_HECC_SR);
}

// *****************************************************************************
/* Function:
    uint32_t* HEMC_HeccGetFailAddress(void)

   Summary:
    Get the last fail address were ECC error occurs in a HEMC memory.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Pointer of fail address were fixable or unfixable error occured in a HEMC memory.
*/
uint32_t* HEMC_HeccGetFailAddress(void)
{
    return (uint32_t*)(HEMC_REGS->HEMC_HECC_FAILAR);
}

// *****************************************************************************
/* Function:
    void HEMC_HeccResetCounters(void)

   Summary:
    Reset Fix and NoFix error counters of the HEMC peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void HEMC_HeccResetCounters(void)
{
    HEMC_REGS->HEMC_HECC_CR0 |= (HEMC_HECC_CR0_RST_FIX_CPT_Msk | HEMC_HECC_CR0_RST_NOFIX_CPT_Msk);
    HEMC_REGS->HEMC_HECC_CR1 |= (HEMC_HECC_CR1_RST_FIX_CPT_Msk | HEMC_HECC_CR1_RST_NOFIX_CPT_Msk);
    HEMC_REGS->HEMC_HECC_CR2 |= (HEMC_HECC_CR2_RST_FIX_CPT_Msk | HEMC_HECC_CR2_RST_NOFIX_CPT_Msk);
}

// *****************************************************************************
/* Function:
    void HEMC_FixCallbackRegister(HEMC_CALLBACK callback,
                                                              uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given HEMC's ECC Fixable Error interrupt events occur.

  Description:
    This function sets the pointer to a client function to be called "back" when
    the given HEMC's interrupt events occur. It also passes a context value
    (usually a pointer to a context structure) that is passed into the function
    when it is called. The specified callback function will be called from the
    peripheral interrupt context.

  Precondition:
    HEMC_Initialize must have been called for the associated
    HEMC instance.

  Parameters:
    callback      - A pointer to a function with a calling signature defined by
                    the HEMC_CALLBACK data type. Setting this to NULL
                    disables the callback feature.

    contextHandle - A value (usually a pointer) passed (unused) into the
                    function identified by the callback parameter.

  Returns:
    None.

  Example:
    <code>

    </code>

  Remarks:
    None.
*/
void HEMC_FixCallbackRegister(HEMC_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    hemcObj.fix_callback = callback;
    hemcObj.fix_context = contextHandle;
}

// *****************************************************************************
/* Function:
    void HEMC_NoFixCallbackRegister(HEMC_CALLBACK callback,
                                                              uintptr_t context)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given HEMC's ECC Not Fixable Error interrupt events occur.

  Description:
    This function sets the pointer to a client function to be called "back" when
    the given HEMC's interrupt events occur. It also passes a context value
    (usually a pointer to a context structure) that is passed into the function
    when it is called. The specified callback function will be called from the
    peripheral interrupt context.

  Precondition:
    HEMC_Initialize must have been called for the associated
    HEMC instance.

  Parameters:
    callback      - A pointer to a function with a calling signature defined by
                    the HEMC_CALLBACK data type. Setting this to NULL
                    disables the callback feature.

    contextHandle - A value (usually a pointer) passed (unused) into the
                    function identified by the callback parameter.

  Returns:
    None.

  Example:
    <code>

    </code>

  Remarks:
    None.
*/
void HEMC_NoFixCallbackRegister(HEMC_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    hemcObj.nofix_callback = callback;
    hemcObj.nofix_context = contextHandle;
}


// *****************************************************************************
/* Function:
    void HEMC_FIX_InterruptHandler(void)

   Summary:
    HEMC Peripheral Interrupt Handler.

   Description:
    This function is HEMC Peripheral Interrupt Handler and will
    called on every HEMC ECC Fixable error interrupt.

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
void __attribute__((used)) HEMC_INTFIX_InterruptHandler(void)
{

    if (hemcObj.fix_callback != NULL)
    {
        uintptr_t fix_context = hemcObj.fix_context;

        hemcObj.fix_callback(fix_context);
    }
}

// *****************************************************************************
/* Function:
    void HEMC_NOFIX_InterruptHandler(void)

   Summary:
    HEMC Peripheral Interrupt Handler.

   Description:
    This function is HEMC Peripheral Interrupt Handler and will
    called on every HEMC ECC Not Fixable error interrupt.

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
void __attribute__((used)) HEMC_INTNOFIX_InterruptHandler(void)
{

    if (hemcObj.nofix_callback != NULL)
    {
        uintptr_t nofix_context = hemcObj.nofix_context;

        hemcObj.nofix_callback(nofix_context);
    }
}

/*******************************************************************************
 End of File
*/
