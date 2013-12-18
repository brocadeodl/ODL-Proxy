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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef PFCFLOWCONTROLLOCALMANAGEDOBJECT_H
#define PFCFLOWCONTROLLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
    class PfcFlowControlLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PfcFlowControlLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            PfcFlowControlLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            PfcFlowControlLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &tx,const UI32 &rx,const SI32 &cos);
            PfcFlowControlLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &tx,const UI32 &rx,const SI32 &cos);
            virtual    ~PfcFlowControlLocalManagedObject ();
            static string getClassName();
            void setTx(const UI32 &tx);
            UI32 getTx()  const;
            void setRx(const UI32 &rx);
            UI32 getRx()  const;
            void setCos(const SI32 &cos);
            SI32 getCos()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_tx;
            UI32  m_rx;
            SI32  m_cos;
    };
}
#endif                                            //PFCFLOWCONTROLLOCALMANAGEDOBJECT_H
