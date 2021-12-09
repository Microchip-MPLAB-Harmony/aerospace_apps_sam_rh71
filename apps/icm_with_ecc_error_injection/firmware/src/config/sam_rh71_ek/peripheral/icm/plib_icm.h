/*******************************************************************************
  ICM Peripheral Library
  Instance Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_icm.h

  Summary:
    ICM PLIB Header file

  Description:
    This file defines the interface to the ICM peripheral 
    library. This library provides access to and control of the associated 
    peripheral instance.
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

#ifndef PLIB_ICM_H
#define PLIB_ICM_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: ICM defines
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: ICM Types
// *****************************************************************************
// *****************************************************************************
/* ICM Region Mask

   Summary:
    ICM Region Mask.

   Description:
    This data type defines the ICM Region.

   Remarks:
    None.
*/
typedef enum
{
    ICM_REGION0_MASK = 0x1,
    ICM_REGION1_MASK = 0x2,
    ICM_REGION2_MASK = 0x4,
    ICM_REGION3_MASK = 0x8,
    /* Force the compiler to reserve 32-bit memory for enum */
    ICM_REGION_MASK_INVALID = 0xFFFFFFFF
} ICM_REGION_MASK;

// *****************************************************************************

/* ICM Interrupt Mask

   Summary:
    ICM Interrupt Mask.

   Description:
    This data type defines the ICM Interrupt sources.

   Remarks:
    None.
*/
typedef enum
{
    ICM_INT_MSK_HASH_R0_MASK = ICM_IER_RHC(ICM_REGION0_MASK), 
    ICM_INT_MSK_HASH_R1_MASK = ICM_IER_RHC(ICM_REGION1_MASK), 
    ICM_INT_MSK_HASH_R2_MASK = ICM_IER_RHC(ICM_REGION2_MASK), 
    ICM_INT_MSK_HASH_R3_MASK = ICM_IER_RHC(ICM_REGION3_MASK), 
    ICM_INT_MSK_DIGEST_MISMATCH_R0_MASK = ICM_IER_RDM(ICM_REGION0_MASK), 
    ICM_INT_MSK_DIGEST_MISMATCH_R1_MASK = ICM_IER_RDM(ICM_REGION1_MASK), 
    ICM_INT_MSK_DIGEST_MISMATCH_R2_MASK = ICM_IER_RDM(ICM_REGION2_MASK), 
    ICM_INT_MSK_DIGEST_MISMATCH_R3_MASK = ICM_IER_RDM(ICM_REGION3_MASK), 
    ICM_INT_MSK_BUS_ERROR_R0_MASK = ICM_IER_RBE(ICM_REGION0_MASK), 
    ICM_INT_MSK_BUS_ERROR_R1_MASK = ICM_IER_RBE(ICM_REGION1_MASK), 
    ICM_INT_MSK_BUS_ERROR_R2_MASK = ICM_IER_RBE(ICM_REGION2_MASK), 
    ICM_INT_MSK_BUS_ERROR_R3_MASK = ICM_IER_RBE(ICM_REGION3_MASK), 
    ICM_INT_MSK_WRAP_R0_MASK = ICM_IER_RWC(ICM_REGION0_MASK),
    ICM_INT_MSK_WRAP_R1_MASK = ICM_IER_RWC(ICM_REGION1_MASK),
    ICM_INT_MSK_WRAP_R2_MASK = ICM_IER_RWC(ICM_REGION2_MASK),
    ICM_INT_MSK_WRAP_R3_MASK = ICM_IER_RWC(ICM_REGION3_MASK),
    ICM_INT_MSK_END_BIT_R0_MASK = ICM_IER_REC(ICM_REGION0_MASK),
    ICM_INT_MSK_END_BIT_R1_MASK = ICM_IER_REC(ICM_REGION1_MASK),
    ICM_INT_MSK_END_BIT_R2_MASK = ICM_IER_REC(ICM_REGION2_MASK),
    ICM_INT_MSK_END_BIT_R3_MASK = ICM_IER_REC(ICM_REGION3_MASK),
    ICM_INT_MSK_STATUS_UPDATE_R0_MASK = ICM_IER_RSU(ICM_REGION0_MASK),
    ICM_INT_MSK_STATUS_UPDATE_R1_MASK = ICM_IER_RSU(ICM_REGION1_MASK),
    ICM_INT_MSK_STATUS_UPDATE_R2_MASK = ICM_IER_RSU(ICM_REGION2_MASK),
    ICM_INT_MSK_STATUS_UPDATE_R3_MASK = ICM_IER_RSU(ICM_REGION3_MASK),
    ICM_INT_MSK_UNDEFINED_REG_ACCESS_MASK = ICM_IER_URAD_Msk,
    /* Force the compiler to reserve 32-bit memory for enum */
    ICM_INT_MSK_INVALID = 0xFFFFFFFF
} ICM_INT_MSK;

// *****************************************************************************
/* ICM status
   Summary:
    Identifies the ICM current status

   Description:
    This data type identifies the ICM status
*/
typedef enum
{
    ICM_STATUS_ENABLE = ICM_SR_ENABLE_Msk,
    ICM_STATUS_RAW_MON_DIS_R0 = ICM_SR_RAWRMDIS(ICM_REGION0_MASK), 
    ICM_STATUS_RAW_MON_DIS_R1 = ICM_SR_RAWRMDIS(ICM_REGION1_MASK),
    ICM_STATUS_RAW_MON_DIS_R2 = ICM_SR_RAWRMDIS(ICM_REGION2_MASK),
    ICM_STATUS_RAW_MON_DIS_R3 = ICM_SR_RAWRMDIS(ICM_REGION3_MASK),
    ICM_STATUS_MON_DIS_R0 = ICM_SR_RMDIS(ICM_REGION0_MASK),
    ICM_STATUS_MON_DIS_R1 = ICM_SR_RMDIS(ICM_REGION1_MASK),
    ICM_STATUS_MON_DIS_R2 = ICM_SR_RMDIS(ICM_REGION2_MASK),
    ICM_STATUS_MON_DIS_R3 = ICM_SR_RMDIS(ICM_REGION3_MASK), 

    /* Force the compiler to reserve 32-bit memory for enum */
    ICM_STATUS_INVALID = 0xFFFFFFFF
} ICM_STATUS;

// *****************************************************************************

/* ICM Callback

   Summary:
    ICM Callback Function Pointer.

   Description:
    This data type defines the ICM Callback Function Pointer.

   Remarks:
    None.
*/
typedef void (*ICM_CALLBACK) (uintptr_t contextHandle);

// *****************************************************************************

/* ICM PLib Instance Object

   Summary:
    ICM PLib Object structure.

   Description:
    This data structure defines the ICM PLib Instance Object.

   Remarks:
    None.
*/
typedef struct
{
    /* Transfer Event Callback */
    ICM_CALLBACK callback;

    /* Transfer Event Callback Context */
    uintptr_t context;
} ICM_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*
 * The following functions make up the methods (set of possible operations) of
 * this interface.
 */

void ICM_Initialize(void);

void ICM_SetEndOfMonitoringDisable(bool disable);

void ICM_WriteBackDisable(bool disable);

void ICM_SoftwareReset(void);

void ICM_SetDescStartAddress(icm_descriptor_registers_t* addr);

void ICM_SetHashStartAddress(uint32_t addr);

void ICM_Enable(void);

void ICM_Disable(void);

void ICM_ReComputeHash(ICM_REGION_MASK regions);

void ICM_MonitorEnable(ICM_REGION_MASK regions);

void ICM_MonitorDisable(ICM_REGION_MASK regions);

ICM_INT_MSK ICM_InterruptGet(void);

ICM_INT_MSK ICM_InterruptMasked(void);

void ICM_InterruptEnable(ICM_INT_MSK interruptMask);

void ICM_InterruptDisable(ICM_INT_MSK interruptMask);

ICM_STATUS ICM_StatusGet(void);


void ICM_CallbackRegister(ICM_CALLBACK callback, uintptr_t contextHandle);


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_ICM_H */
