// Copyright (c) LooUQ Incorporated. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef _USERMODEL_H
#define _USERMODEL_H
#include "bme280.h"

#ifdef __cplusplus
extern "C" {
#endif

    void initUserModel();
    void deinitUserModel();

    
    IOTQI_RESULT UserModel_GetCommands(STRING_HANDLE commandsMeta);
    EXECUTE_COMMAND_RESULT UserModel_CommandMsgCallback(char* cmdBuffer);

    IOTQI_RESULT UserModel_DoWork();
    
#ifdef __cplusplus
}
#endif

#endif      // #define _USERMODEL_H

