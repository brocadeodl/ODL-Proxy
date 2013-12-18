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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef FRAMEWORKTESTABILITYTYPES_H
#define FRAMEWORKTESTABILITYTYPES_H

namespace WaveNs
{

typedef enum
{
    FRAMEWORK_TESTABILITY_MESSAGE1  = 0,
    FRAMEWORK_TESTABILITY_MESSAGE2,
    FRAMEWORK_TESTABILITY_MESSAGE3,
    FRAMEWORK_TESTABILITY_MESSAGE4,
    FRAMEWORK_TESTABILITY_MESSAGE5,
    FRAMEWORK_TESTABILITY_MESSAGE6,
    FRAMEWORK_TESTABILITY_MESSAGE7,
    FRAMEWORK_TESTABILITY_MESSAGE8,
    FRAMEWORK_TESTABILITY_MESSAGE9,
    FRAMEWORK_TESTABILITY_MESSAGE10,
    FRAMEWORK_TESTABILITY_MESSAGE11,
    FRAMEWORK_TESTABILITY_EVENT1,
    FRAMEWORK_TESTABILITY_EVENT2,
    FRAMEWORK_TESTABILITY_EVENT3,
    FRAMEWORK_TESTABILITY_EVENT4,
    FRAMEWORK_TESTABILITY_EVENT5,
    FRAMEWORK_TESTABILITY_EVENT6,
    FRAMEWORK_TESTABILITY_EVENT7,
    FRAMEWORK_TESTABILITY_EVENT8,
    FRAMEWORK_TESTABILITY_EVENT9,
    FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE1,
    FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE2,
    FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE3,
    FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE4,
    FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE_FOR_MEMORY_LEAKS,
    FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_MESSAGE_FOR_OBJECT_LEAKS,
    FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_FILE_SERVICE_USAGE,
    FRAMEWORK_TESTABILITY_MANAGEMENT_INTERFACE_TRACK_LEAKS_WITH_BT,

    FRAMEWORK_TESTABILITY_4_GET_MANAGED_OBJECTS,
	FRAMEWORK_TESTABILITY_6_GET_MANAGED_OBJECTS
} FraemworkTestabilityOpCodes;

typedef enum
{
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
} Day;

typedef enum
{
    MESSAGE_LEAK,
    OBJECT_LEAK
} LeakTypes;

}

#endif // FRAMEWORKTESTABILITYTYPES_H
