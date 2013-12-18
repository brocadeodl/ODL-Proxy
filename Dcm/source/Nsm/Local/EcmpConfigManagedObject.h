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
 *   Author : Amulya Makam                                                   *
 **************************************************************************/

#ifndef ECMPCONFIGMANAGEDOBJECT_H
#define ECMPCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Framework/Attributes/AttributeUI32UC.h"

using namespace WaveNs;

namespace DcmNs
{
    class EcmpConfigManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            EcmpConfigManagedObject (WaveObjectManager *pWaveObjectManager);
            EcmpConfigManagedObject (WaveObjectManager *pWaveObjectManager,const UI32 &loadBalanceMode,
                const UI32 &rbridgeId,const UI32 &hashSwap);
            virtual    ~EcmpConfigManagedObject ();
            static string getClassName();
            void setLoadBalanceMode(const UI32 &loadBalanceMode);
            UI32 getLoadBalanceMode() const;
            void setHashSwap(const UI32 &hashSwap, const bool &flag);
            UI32UC getHashSwap() const;
            void setRbridgeId(const UI32 &rbridgeId);
            UI32 getRbridgeId() const;

            Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);

// Now the data members

        private:
        protected:
        public:
            UI32   m_loadBalanceMode;
            UI32   m_rbridgeId;
            UI32UC   m_hashSwap;
    };
}
#endif

