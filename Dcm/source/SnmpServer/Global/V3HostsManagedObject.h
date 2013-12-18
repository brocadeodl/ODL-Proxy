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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef V3HOSTSMANAGEDOBJECT_H
#define V3HOSTSMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "Framework/Attributes/AttributeHostUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerObjectManager;
    class V3HostsManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            V3HostsManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager);
            V3HostsManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager,const HostUC &ip,const NotifyType &nType,const ObjectId &username,const UI32 &udpPort, const string &engineId, const SevLevel &sevLevel);
            virtual    ~V3HostsManagedObject ();
            static string getClassName();
            void setHostUC(const HostUC &ip);
            HostUC getHostUC()  const;
            void setNotify(const NotifyType &notify);
            NotifyType getNotify()  const;
            void setUsername(const ObjectId &username);
            ObjectId getUsername()  const;
            void setUdpPort(const UI32 &udpPort);
            UI32 getUdpPort()  const;
            void setEngineid(const string &engineId);
            string getEngineid()  const;
            void setSevLevel(const SevLevel &sevLevel);
            SevLevel getSevLevel()  const;


// Now the data members

        private:
        protected:
        public:
            HostUC  m_ip;
            NotifyType  m_nType;
            ObjectId  m_username;
            UI32  m_udpPort;
            string m_engineId;
			SevLevel m_sevLevel;
    };
}
#endif                                            //V3HOSTSMANAGEDOBJECT_H
