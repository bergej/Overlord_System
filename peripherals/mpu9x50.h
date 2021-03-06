//*****************************************************************************
//
// mpu9X50.h - Prototypes for the MPU9X50 accelerometer, gyroscope, and
//             magnetometer driver.
//
// Copyright (c) 2013-2016 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.3.156 of the Tiva Firmware Development Package.
//
//*****************************************************************************

#ifndef __SENSORLIB_MPU9X50_H__
#define __SENSORLIB_MPU9X50_H__

//*****************************************************************************
//
// Necessary includes so user does not need to find proper order
//
//*****************************************************************************
#include <stdint.h>
#include "sensorlib/hw_ak8975.h"
#include "sensorlib/hw_ak8963.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/ak8975.h"
#include "sensorlib/ak8963.h"

#include "hw_mpu9x50.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// The structure that defines the internal state of the MPU9X50 driver.
//
//*****************************************************************************
typedef struct
{
    //
    // The pointer to the I2C master interface instance used to communicate
    // with the MPU9X50.
    //
    tI2CMInstance *psI2CInst;

#ifdef MPU9X50_IS_9150
    //
    // The AK8975 inst that used to access the on-chip AK8975 magnetometer
    //
    tAK8975 sAK8975Inst;
#else
    //
    // The AK8963 inst that used to access the on-chip AK8963 magnetometer
    //
    tAK8963 sAK8963Inst;
#endif
    //
    // The I2C address of the MPU9X50.
    //
    uint8_t ui8Addr;

    //
    // The state of the state machine used while accessing the MPU9X50.
    //
    uint8_t ui8State;

    //
    // The current accelerometer afs_sel setting
    //
    uint8_t ui8AccelAfsSel;

    //
    // The new accelerometer afs_sel setting, which is used when a register
    // write succeeds.
    //
    uint8_t ui8NewAccelAfsSel;

    //
    // The current gyroscope fs_sel setting
    //
    uint8_t ui8GyroFsSel;

    //
    // The new gyroscope fs_sel setting, which is used when a register write
    // succeeds.
    //
    uint8_t ui8NewGyroFsSel;

    //
    // The data buffer used for sending/receiving data to/from the MPU9X50.
    //
    uint8_t pui8Data[24];

    //
    // The function that is called when the current request has completed
    // processing.
    //
    tSensorCallback *pfnCallback;

    //
    // The callback data provided to the callback function.
    //
    void *pvCallbackData;

    //
    // A union of structures that are used for read, write and
    // read-modify-write operations.  Since only one operation can be active at
    // a time, it is safe to re-use the memory in this manner.
    //
    union
    {
        //
        // A buffer used to store the write portion of a register read.
        //
        uint8_t pui8Buffer[6];

        //
        // The write state used to write register values.
        //
        tI2CMWrite8 sWriteState;

        //
        // The read-modify-write state used to modify register values.
        //
        tI2CMReadModifyWrite8 sReadModifyWriteState;
    }
    uCommand;
}
tMPU9X50;

//*****************************************************************************
//
// Function prototypes.
//
//*****************************************************************************
extern uint_fast8_t MPU9X50Init(tMPU9X50 *psInst, tI2CMInstance *psI2CInst,
                                uint_fast8_t ui8I2CAddr,
                                tSensorCallback *pfnCallback,
                                void *pvCallbackData);
#ifdef MPU9X50_IS_9150
extern tAK8975 *MPU9X50InstAK8963Get(tMPU9X50 *psInst);
#else
extern tAK8963 *MPU9X50InstAK8963Get(tMPU9X50 *psInst);
#endif
extern uint_fast8_t MPU9X50Read(tMPU9X50 *psInst, uint_fast8_t ui8Reg,
                                uint8_t *pui8Data, uint_fast16_t ui16Count,
                                tSensorCallback *pfnCallback,
                                void *pvCallbackData);
extern uint_fast8_t MPU9X50Write(tMPU9X50 *psInst, uint_fast8_t ui8Reg,
                                 const uint8_t *pui8Data,
                                 uint_fast16_t ui16Count,
                                 tSensorCallback *pfnCallback,
                                 void *pvCallbackData);
extern uint_fast8_t MPU9X50ReadModifyWrite(tMPU9X50 *psInst,
                                           uint_fast8_t ui8Reg,
                                           uint_fast8_t ui8Mask,
                                           uint_fast8_t ui8Value,
                                           tSensorCallback *pfnCallback,
                                           void *pvCallbackData);
extern uint_fast8_t MPU9X50DataRead(tMPU9X50 *psInst,
                                    tSensorCallback *pfnCallback,
                                    void *pvCallbackData);
extern void MPU9X50DataAccelGetRaw(tMPU9X50 *psInst,
                                   uint_fast16_t *pui16AccelX,
                                   uint_fast16_t *pui16AccelY,
                                   uint_fast16_t *pui16AccelZ);
extern void MPU9X50DataAccelGetFloat(tMPU9X50 *psInst, float *pfAccelX,
                                     float *pfAccelY, float *pfAccelZ);
extern void MPU9X50DataGyroGetRaw(tMPU9X50 *psInst, uint_fast16_t *pui16GyroX,
                                  uint_fast16_t *pui16GyroY,
                                  uint_fast16_t *pui16GyroZ);
extern void MPU9X50DataGyroGetFloat(tMPU9X50 *psInst, float *pfGyroX,
                                    float *pfGyroY, float *pfGyroZ);
extern void MPU9X50DataMagnetoGetRaw(tMPU9X50 *psInst,
                                     uint_fast16_t *pui16MagnetoX,
                                     uint_fast16_t *pui16MagnetoY,
                                     uint_fast16_t *pui16MagnetoZ);
extern void MPU9X50DataMagnetoGetFloat(tMPU9X50 *psInst, float *pfMagnetoX,
                                       float *pfMagnetoY, float *pfMagnetoZ);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __SENSORLIB_MPU9X50_H__
