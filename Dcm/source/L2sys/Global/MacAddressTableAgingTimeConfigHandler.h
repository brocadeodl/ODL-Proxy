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
 *   Copyright (C) 2005 - 2012 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef MACADDRESSTABLEAGINGTIMECONFIGHANDLER_H
#define MACADDRESSTABLEAGINGTIMECONFIGHANDLER_H

#include "SystemManagement/WaveConfigurationSegmentWorker.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "SystemManagement/WaveConfigurationSegmentDeleteContext.h"

using namespace WaveNs;

namespace DcmNs
{

class MacAddressTableAgingTimeConfigHandler : public WaveConfigurationSegmentWorker
{
    struct AttributeModel {
        int agingTime;
    };

    private :
        AttributeModel                  getAttributeModel (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext);

        virtual void                    trace (TraceLevel traceLevel, const string &stringToTrace);
        void                            printVector (const string label, vector<UI32>& attributeUserTags);
        void                            printSVector (const string label, vector<string>& attributeUserTags);
        ResourceId                      validateAgingTime (int agingTime);

    protected :
    public :
                                            MacAddressTableAgingTimeConfigHandler (WaveObjectManager *pWaveObjectManager);
        virtual                             ~MacAddressTableAgingTimeConfigHandler ();

        virtual ManagementInterfaceMessage *createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext);
        virtual ManagementInterfaceMessage *deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext);

    // Data members
    private :
    protected:
    public :
};

}

#endif // 
