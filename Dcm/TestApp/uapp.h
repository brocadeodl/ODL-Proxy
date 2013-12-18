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

/* **
 * uapp.h
 *
 * June 15, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef _UAPP_H_
#define _UAPP_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Types/WaveCValue.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "WaveCStatus.h"

int printUnifiedClientUpdate (const void *inp, void ** const out);
int printUnifiedClientCreate (const void *inp, void ** const out);
int printUnifiedClientDelete (const void *inp, void ** const out);

int printUnifiedClientAttributes (UnifiedClientMessage_t *msg);

/**
 * Start the client process and wait forever.... Handles unified plugin
 * create/update/delete messages.
 */
void start_client(const char *name, int mgmt_port_offset);

#endif

