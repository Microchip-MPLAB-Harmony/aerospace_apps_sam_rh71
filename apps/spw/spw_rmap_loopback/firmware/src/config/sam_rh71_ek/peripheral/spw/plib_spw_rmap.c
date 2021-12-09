/*******************************************************************************
  SPW Peripheral Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_spw_rmap.c

  Summary:
    SPW PLIB RMAP Implementation file

  Description:
    This file defines the interface to the SPW peripheral library.
    This library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/
// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_spw_rmap.h"

// *****************************************************************************
// *****************************************************************************
// SPW PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

/* Function:
    void SPW_RMAP_Initialize(void)

   Summary:
    Initialize RMAP modules of the SPW peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void SPW_RMAP_Initialize(void)
{
    SPW_REGS->SPW_RMAP1_CFG = SPW_RMAP1_CFG_DESTKEY(137) |
                              SPW_RMAP1_CFG_TLA(66) |
                              SPW_RMAP1_CFG_RMAPENA(1);
}

// *****************************************************************************
/* Function:
    SPW_RMAP_STATUS SPW_RMAP_StatusGetAndClear(void)

   Summary:
    Get SPW RMAP Status and clear.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    SPW RMAP Status
*/
SPW_RMAP_STATUS SPW_RMAP_StatusGetAndClear(void)
{
    return (SPW_RMAP_STATUS)(SPW_REGS->SPW_RMAP1_STS_RC);
}
