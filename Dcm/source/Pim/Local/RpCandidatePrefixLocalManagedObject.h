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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Chirag Taunk                                                 *
 **************************************************************************/

#ifndef RPCANDIDATEPREFIXLOCALMANAGEDOBJECT_H
#define RPCANDIDATEPREFIXLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Pim/Local/PimTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimLocalObjectManager;
    class RpCandidatePrefixLocalManagedObject : virtual public 
		DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            RpCandidatePrefixLocalManagedObject (PimLocalObjectManager 
				*pPimLocalObjectManager);
            virtual    ~RpCandidatePrefixLocalManagedObject ();
            static string getClassName();
			void setAction(const PimPrefixActionType &prefixAction);
			PimPrefixActionType mapPrefixActionToEnumVal(const UI8 
				&prefixAction);
            PimPrefixActionType getAction()  const;
			void setPrefixIP(const IpV4Address &prefixIP);
            IpV4Address getPrefixIP()  const;
            void setPrefixLen(const UI16 &prefixLen);
            UI16 getPrefixLen()  const;

// Now the data members

        private:
        protected:
        public:
			PimPrefixActionType  m_prefixAction;
            IpV4Address  m_prefixIP;
            UI16  m_prefixLen;
    };
}
#endif           //RPCANDIDATEPREFIXLOCALMANAGEDOBJECT_H
