/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 **************************************************************************/

#ifndef UNIFIEDCLIENTCOMMON_H
#define UNIFIEDCLIENTCOMMON_H

#include <stdio.h>
#include <unistd.h>

#include "Framework/Types/WaveCValue.h"

enum UnifiedClientOpcode
{
    UNIFIEDCLIENTUPDATE = 370,
    UNIFIEDCLIENTCREATE,
    UNIFIEDCLIENTDELETE,
    UNIFIEDCLIENTPOSTBOOT,
    UNIFIEDCLIENTNOTIFY,       
    UNIFIEDCLIENTGETDATA
};

typedef struct UnifiedClientMessage
{
    int           groupCode;
    const char   *managedObjectName;
    const char  **attributeNames;
    const char  **parentManagedObjectNames;
    WaveCValue   *attributeValues;
    int           attributeSize;
    int           parentManagedObjectNamesSize;
    int           numberOfRecordsRequested; 
    int           isConfigReplayInProgress;
}UnifiedClientMessage_t;

typedef struct UnifiedClientBackendNotifyMessage
{
    int  backendNotifyValue;
} UnifiedClientNotifyMessage_t;

typedef struct UnifiedClientMessageInput
{
    int           groupCode;
    int           commandType;
    int           numberOfRecordsRequested;
    int           inputBufferSize;
    const void   *inputBuffer;
    int           commandStringSize; //argc in case of operational data
    const char  **commandString; //argv in case of operational data
}UnifiedClientMessageInput_t;

typedef struct UnifiedClientMessageOutput
{   
    int         moreRecords;
    int         bufferTag;
    int         outputBufferSize;
    const void *outputBuffer;
}UnifiedClientMessageOutput_t;

typedef struct UnifiedClientMessageOutputForConfig
{
    int           warningCode;
    const char  **attributeNames;
    const char  **attributeValues;
    int           attributeSize;
}UnifiedClientMessageOutputForConfig_t;


#endif
