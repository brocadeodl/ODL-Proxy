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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Leifang Hu   			                                       *
 ***************************************************************************/

#ifndef WAVEPOSTBOOTEVENT_H
#define	WAVEPOSTBOOTEVENT_H

namespace WaveNs
{

typedef enum
{
    POSTBOOT_EVENT_SERVICE_READY,
	POSTBOOT_EVENT_LINECARD_READY,
	POSTBOOT_EVENT_SYSTEM_READY

} PostbootEventTypes;

typedef enum
{
    POSTBOOT_GLOBAL_CONFIG,
    POSTBOOT_LINECARD_CONFIG

} PostbootConfigTypes;

}

#endif // WAVEPOSTBOOTEVENT_H

