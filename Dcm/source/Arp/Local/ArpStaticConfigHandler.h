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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Sajad Bashir Qadri                                           *
 ***************************************************************************/

#ifndef ARPSTATICCONFIGHANDLER_H
#define ARPSTATICCONFIGHANDLER_H

#include "Arp/Local/ArpStaticConfigLocalMessage.h"
#include "SystemManagement/WaveSystemManagementLogger.h"
#include "SystemManagement/WaveConfigurationSegmentWorker.h"
#include "SystemManagement/WaveConfigurationSegmentContext.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "SystemManagement/WaveConfigurationSegmentDeleteContext.h"

using namespace WaveNs;

namespace DcmNs
{

class ArpStaticConfigHandler : public WaveConfigurationSegmentWorker
{
    private :
        map<string, string>             getAttributeMap     (WaveConfigurationSegmentContext *pWaveConfigurationSegmentContext);
        int                             setInterface        (ArpStaticConfigLocalMessage *pMessage, map<string, string> attributeMap);

        virtual void                    trace               (TraceLevel traceLevel, const string &stringToTrace);

    protected :
    public :
                        ArpStaticConfigHandler  (WaveObjectManager *pWaveObjectManager);
        virtual        ~ArpStaticConfigHandler  ();

        virtual ManagementInterfaceMessage *createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext);
        virtual ManagementInterfaceMessage *deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext);

    // Data Members
    private :
                WaveSystemManagementLogger      logger;
    protected:
    public :
};

}

#endif //
