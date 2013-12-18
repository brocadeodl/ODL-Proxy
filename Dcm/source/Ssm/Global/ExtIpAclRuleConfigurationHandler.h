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

#ifndef EXTIPACLRULECONFIGURATIONHANDLER_H
#define EXTIPACLRULECONFIGURATIONHANDLER_H

#include "SystemManagement/WaveConfigurationSegmentWorker.h"
#include "SystemManagement/WaveConfigurationSegmentCreateContext.h"
#include "SystemManagement/WaveConfigurationSegmentDeleteContext.h"

using namespace WaveNs;

namespace DcmNs
{

class ExtIpAclRuleConfigurationHandler : public WaveConfigurationSegmentWorker
{
    private :
    protected :
    public :
						ExtIpAclRuleConfigurationHandler  (WaveObjectManager *pWaveObjectManager);

        virtual        ~ExtIpAclRuleConfigurationHandler  ();

        virtual ManagementInterfaceMessage *createConfig (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext);

        virtual ManagementInterfaceMessage *deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext);

        virtual string 						getErrorString(ManagementInterfaceMessage* pMessage);

		UI32 StringtoWellknownNumberTcp (const char *ipprotocol);

		UI32 StringtoWellknownNumberUdp (const char *ipprotocol);

		UI32 NumbertoWellknownNumberTcp (UI32 ipprotocol);

		UI32 NumbertoWellknownNumberUdp (UI32 ipprotocol);

		void getSrcdstIpAddressandSrcdstIpMaskAddress
			(string &srcDstIpAddressHold, IpV4Address &srcDstIpAddressMask,
			 IpV4Address &srcDstIpAddress, bool &srcDstIpAddressSlashflag, int position);

		UI32 IsWellknownTos (const char *PrecDscpTosString, UI32 tos, int &PrecDscpTosFlag);

		UI32 IsWellknownPrecedence (const char *PrecDscpTosString, UI32 precedence, int &PrecDscpTosFlag);

		UI32 IsWellknownDscp (const char *PrecDscpTosString, UI32 dscp, int &PrecDscpTosFlag);

    // Now the data members

    protected :

    public :

};

}

#endif // EXTIPACLRULECONFIGURATIONHANDLER_H
