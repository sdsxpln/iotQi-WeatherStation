// Copyright (c) LooUQ Incorporated. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef _USERMODEL_H
#define _USERMODEL_H
#include "bme280.h"

#include <iotQiTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

    IOTQIMODEL_RESULT initUserModel();
    void deinitUserModel();
    
    IOTQIMODEL_RESULT UserModel_GetCommands(STRING_HANDLE commandsMeta);
    EXECUTE_COMMAND_RESULT UserModel_CommandMsgCallback(const char* cmdBuffer);

    TelemetryTemplate BmeSensor(STRING_HANDLE* sample_data);
    AlertTemplate WindAlert(STRING_HANDLE* alert_data);
 
#ifdef __cplusplus
}
#endif

#endif      // #define _USERMODEL_H

