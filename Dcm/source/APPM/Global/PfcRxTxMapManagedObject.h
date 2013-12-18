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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef PFCRXTXMAPMANAGEDOBJECT_H
#define PFCRXTXMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "vcs.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalObjectManager;
    class PfcRxTxMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PfcRxTxMapManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            PfcRxTxMapManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const SI32 &pfcValue,const UI32 &txValue,const UI32 &rxValue);
            virtual    ~PfcRxTxMapManagedObject ();
            static string getClassName();
            void setPfcValue(const SI32 &pfcValue);
            SI32 getPfcValue()  const;
            void setTxValue(const UI32 &txValue);
            UI32 getTxValue()  const;
            void setRxValue(const UI32 &rxValue);
            UI32 getRxValue()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_pfcValue;
            UI32  m_txValue;
            UI32  m_rxValue;
    };
}
#endif                                            //PFCRXTXMAPMANAGEDOBJECT_H
