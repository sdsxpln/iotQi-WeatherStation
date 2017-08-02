// Copyright (c) LooUQ Incorporated. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifdef ARDUINO
#include "AzureIoTHub.h"
#else
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/platform.h"
#include "serializer.h"
#include "iothub_client_ll.h"
#include "iothubtransporthttp.h"
#endif
#include "sdk/schemaserializer.h"

#include <iotQi.h>
#include <globals.h>
#include "userModel.h"
#include "bme280.h"
/* ------------------------------------------------------------------------------------------------------------------------------------------
*  Add any required headers for project specific functionality.  These headers are likely to be required in the UserModel.c file as well.
------------------------------------------------------------------------------------------------------------------------------------------ */

//#include "oled-display.h"

/* ------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------ */

/* ==========================================================================================================================================
*  Model Type            Description
*  -------------------   ---------------------------------------------
*  double                double precision floating point number
*  int                   32 bit integer
*  float                 single precision floating point number
*  long                  long integer
*  int8_t                8 bit integer
*  int16_t               16 bit integer
*  int32_t               32 bit integer
*  int64_t               64 bit integer
*  bool                  boolean
*  ascii_char_ptr        ASCII string
*  EDM_DATE_TIME_OFFSET  date time offset
*  EDM_GUID              GUID
*  EDM_BINARY            binary
*  DECLARE_STRUCT        complex data type

https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-device-sdk-c-serializer
========================================================================================================================================== */


/* Model Definition ====================================================================================================================== */

BEGIN_NAMESPACE(UserModelNamespace);

DECLARE_STRUCT(_Bme,
  float, Temperature,
  float, Humidity,
  float, Pressure
);

DECLARE_MODEL(UserModel,
WITH_DATA(ascii_char_ptr, DeviceId),
WITH_DATA(int, WindSpeed),
WITH_DATA(_Bme, Bme),
WITH_ACTION(TurnFanOn),
WITH_ACTION(TurnFanOff),
WITH_ACTION(SetAirResistance, int, Position)
);

END_NAMESPACE(UserModelNamespace);


/* Model variables ====================================================================================================================== */
UserModel* userModel;

/* Add any persistance model variables here */

/* Model command methods ================================================================================================================= */


EXECUTE_COMMAND_RESULT TurnFanOn(UserModel* device)
{
    (void)device;
    (void)printf("Turning fan on.\r\n");
    return EXECUTE_COMMAND_SUCCESS;
}

EXECUTE_COMMAND_RESULT TurnFanOff(UserModel* device)
{
    (void)device;
    (void)printf("Turning fan off.\r\n");
    return EXECUTE_COMMAND_SUCCESS;
}

EXECUTE_COMMAND_RESULT SetAirResistance(UserModel* device, int Position)
{
    (void)device;
    (void)printf("Setting Air Resistance Position to %d.\r\n", Position);
    return EXECUTE_COMMAND_SUCCESS;
}

// ------------------------------------------------------------------------------------------------------------------------------------------

IOTQI_RESULT UserModel_DoWork()
{
    (void)printf("userModel_DoWork(): entering...\r\n");
 
    readBmeValues();

    
    int avgWindSpeed = 10;
    userModel->DeviceId = "IotqiDevice1";
    userModel->WindSpeed = avgWindSpeed + (rand() % 4 + 2);
    userModel->Bme.Temperature = temperature;
    userModel->Bme.Humidity = humidity;
    userModel->Bme.Pressure = barPressure;
    

    char eventValue[EVENTVALUE_SIZE];
    snprintf(eventValue, EVENTVALUE_SIZE, "Wind Speed=%d","Temp=%.2f *F, Humi=%.2f %, Pres=%.2f in   %s\r\n\r\n",userModel->WindSpeed, userModel->Bme);


    unsigned char* msgBuffer;
    size_t msgSize;
    if (SERIALIZE(&msgBuffer, &msgSize, userModel->DeviceId, userModel->WindSpeed, userModel->Bme) != CODEFIRST_OK)
    {
        (void)printf("***Failed to serialize model to message\r\n");
    }
    else
    {
        (void)printf("userModel_DoWork(): serialization result=%.*s size=%d \r\n", msgSize, msgBuffer, msgSize);
        iotQiSendTelemetry(msgBuffer, msgSize, "AnometerData", eventValue);
    }
}


/* iotQi Required ======================================================================================================
 * The following methods are required to enable your model code to interface with iotQi
 * ================================================================================================================== */

/* initialize and destroy model objects */

void initUserModel()
{
    /* initialize any parts of your model you need here */
    srand((unsigned int)time(NULL));
    int avgWindSpeed = 10;


    /* iotQi needs to have your "model" initialized too*/
	userModel = CREATE_MODEL_INSTANCE(UserModelNamespace, UserModel);
	if (userModel == NULL)
	{
		(void)printf("Failed on create user-defined model\r\n");
        return IOTQI_ERROR;
	}
    (void)printf("User-Defined Model Initialized\r\n");
    return IOTQI_OK;
}

void deinitUserModel()
{
    (void)printf("Deconstructing User-Defined Model\r\n");
	DESTROY_MODEL_INSTANCE(userModel);
}

/* -----------------------------------------------------------------------------------------------------------------------
 * The next two methods link your user model commands back to iotQi
 * -------------------------------------------------------------------------------------------------------------------- */

IOTQI_RESULT UserModel_GetCommands(STRING_HANDLE commandsMeta)
{
    #if defined(IOTQI_DEBUG)
    (void)printf("> Fetching user-defined commands\r\n");
    #endif

	if (SchemaSerializer_SerializeCommandMetadata(GET_MODEL_HANDLE(UserModelNamespace, UserModel), commandsMeta) != SCHEMA_SERIALIZER_OK)
	{
		return IOTQI_ERROR;
	}
	return IOTQI_OK;
}


EXECUTE_COMMAND_RESULT UserModel_CommandMsgCallback(char* cmdBuffer)
{
    #if defined(IOTQI_DEBUG)
    (void)printf("> Invoking user command: %s \r\n", cmdBuffer);
    #endif

    EXECUTE_COMMAND_RESULT result = EXECUTE_COMMAND(userModel, cmdBuffer);
}



