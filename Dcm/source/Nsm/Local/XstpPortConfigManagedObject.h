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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nsong                                                     *
 **************************************************************************/

#ifndef XSTPPORTCONFIGMANAGEDOBJECT_H
#define XSTPPORTCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Mstp/Global/MstpGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
    class NsmUpdatePhyStpConfigMessage;
    class NsmGlobalUpdatePoStpConfigMessage;
    class NsmUpdatePoStpConfigSPMessage;
    class MstpPortConfigMessage;

    class XstpPortConfigManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:

            XstpPortConfigManagedObject (WaveObjectManager *pNsmLocalObjectManager);
            virtual    ~XstpPortConfigManagedObject ();

            static string getClassName();
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
            void setCost(const UI32 &cost);
            UI32 getCost()  const;
            void setRestrictedRole(const bool &restrictedRole);
            bool getRestrictedRole()  const;
            void setRestrictedTcn(const bool &restrictedTcn);
            bool getRestrictedTcn()  const;
            void setAutoEdge(const bool &autoEdge);
            bool getAutoEdge()  const;
            void setPortFast(const bool &portFast);
            bool getPortFast()  const;
            void setEdgePort(const bool &edgePort);
            bool getEdgePort()  const;
            void setBpduGuardEnable(const bool &bpduGuardEnable);
            bool getBpduGuardEnable()  const;
            void setBpduGuard(const bool &bpduGuard);
            bool getBpduGuard()  const;
            void setBpduFilter(const bool &bpduFilter);
            bool getBpduFilter()  const;
            void setGuard(const bool &guard);
            bool getGuard()  const;
            void setLinkShared(const MstpPortLinkTypeEnum &linkShared);
            MstpPortLinkTypeEnum getLinkShared()  const;
            void setStpEnable(const bool &stpEnable);
            bool getStpEnable()  const;
            void setPort(const string &port);
            string getPort()  const;
            void setBpduMac(const MstpPortPvstBpDuMac &guard);
            MstpPortPvstBpDuMac getBpduMac()  const;
            void setTaggedBpduEnable (bool taggedBpduEnable);
            bool getTaggedBpduEnable () const;
            void setPortUid (UI32 value);
            UI32 getPortUid () const;
            void setBpduDropDirection (UI32 const value);
            UI32	 getBpduDropDirection () const;

	    Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);
	    void setDefaultAttribute (const string &ifName, bool resetStpEnable = true);
        void setDefaultAttribute (bool resetStpEnable, bool recordFieldNames);
	    ResourceId setAttributeByMessage(NsmGlobalUpdatePoStpConfigMessage  *pMsg);
	    ResourceId setAttributeByMessage(NsmUpdatePhyStpConfigMessage  *pMsg);
	    ResourceId getAttributeToMessage(MstpPortConfigMessage *&pMsg);

	    static void initDefaults();
        virtual string customConfigurationDisplay ( WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext);

// Now the data members

        private:
	        // Default shutdown state - true in VCS mode and false in standalone..
	        static bool DEFAULT_SHUT_STATE;

        protected:
        public:
            string  m_port;
            UI32  m_priority;
            UI32  m_helloTime;
            UI32  m_cost;
            bool  m_restrictedRole;
            bool  m_restrictedTcn;
            bool  m_autoEdge;
            bool  m_portFast;
            bool  m_edgePort;
            bool  m_bpduGuardEnable;
            bool  m_bpduGuard;
            bool  m_bpduFilter;
            bool  m_guard;
            MstpPortLinkTypeEnum  m_linkShared;
			MstpPortPvstBpDuMac		m_mac;
            bool  m_taggedBpduEnable;
            bool  m_stpDisable;
            UI32  m_portUid;
            UI32  m_bpduDropDirection;
    };
}
#endif                                            //XSTPPORTCONFIGMANAGEDOBJECT_H
