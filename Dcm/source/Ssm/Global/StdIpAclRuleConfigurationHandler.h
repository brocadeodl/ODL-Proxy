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
 *   Author : Balachandar Mani                                             *
 ***************************************************************************/

#ifndef STDIPACLRULECONFIGURATIONHANDLER_H
#define STDIPACLRULECONFIGURATIONHANDLER_H

#include "SystemManagement/WaveConfigurationSegmentWorker.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "SystemManagement/WaveConfigurationSegmentDeleteContext.h"

using namespace WaveNs;

namespace DcmNs
{

class StdIpAclRuleConfigurationHandler : public WaveConfigurationSegmentWorker
{
    private :
    protected :
    public :
						StdIpAclRuleConfigurationHandler  (WaveObjectManager *pWaveObjectManager);
        virtual        ~StdIpAclRuleConfigurationHandler  ();

        virtual ManagementInterfaceMessage *createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext);

        virtual ManagementInterfaceMessage *deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext);

        virtual string 						getErrorString(ManagementInterfaceMessage* pMessage);

        void getSrcdstIpAddressandSrcdstIpMaskAddress
			(string &srcDstIpAddressHold, IpV4Address &srcDstIpAddressMask,
			 IpV4Address &srcDstIpAddress, bool &srcDstIpAddressSlashflag, int position);

    // Now the data members

    protected :

    public :

};

}

#endif // STDIPACLRULECONFIGURATIONHANDLER_H
