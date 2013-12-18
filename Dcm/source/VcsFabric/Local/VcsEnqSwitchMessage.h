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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh			                                   *
 ***************************************************************************/

#ifndef VCSENQSWITCHMESSAGE
#define VCSENQSWITCHMESSAGE

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Local/VcsFabricLocalSerializableObject.h"
#include "Firmware/Global/NodeAutoUpgradeConfiguration.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsEnqSwitchMessage : public ManagementInterfaceMessage
{
    private :
            void                                setupAttributesForSerialization ();

    protected :
    public :
                                                VcsEnqSwitchMessage           ();
    virtual                                    ~VcsEnqSwitchMessage           ();
                                                VcsEnqSwitchMessage           (const string &className, const UI32 &messageOpcode);

            VcsFabricLocalSwitchInfoObject      getRemoteVcsSwitchInfoObject  () const;
            void                                setRemoteVcsSwitchInfoObject  (const VcsFabricLocalSwitchInfoObject &vcsFabricLocalSwitchInfoObject);
            UI32                                getRemoteNodeDomainId         () const;
            void                                setRemoteNodeDomainId         (const UI32 &domainId);
			//void								setNodeAutoUpgradeConfiguration (const NodeAutoUpgradeConfiguration &UpgradeConfigData);
			//NodeAutoUpgradeConfiguration		getNodeAutoUpgradeConfiguration() const;
            UI32                                getAutoUpgradeStatus() const;
            void                                setAutoUpgradeStatus(const UI32 &autoupgradeStatus);
			
			//get and set function for firmware serializable object
    // Now the data members


    private :
		    VcsFabricLocalSwitchInfoObject      m_vcsFabricLocalSwitchInfoObject;
            UI32                                m_domainId;
    		//NodeAutoUpgradeConfiguration		m_NodeAutoUpgradeConfiguration;
			UI32                                m_autoupgradeStatus;
    protected :
    public :
};

}

#endif // VCSENQSWITCHMESSAGE


