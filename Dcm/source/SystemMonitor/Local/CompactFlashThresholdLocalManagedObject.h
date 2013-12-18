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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef COMPACTFLASHTHRESHOLDLOCALMANAGEDOBJECT_H
#define COMPACTFLASHTHRESHOLDLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorLocalObjectManager;
    class CompactFlashThresholdLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            CompactFlashThresholdLocalManagedObject (SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager);
            CompactFlashThresholdLocalManagedObject (SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager,const UI32 &downthreshold,const UI32 &marginalthreshold);
            virtual    ~CompactFlashThresholdLocalManagedObject ();
            static string getClassName();
            void setDownthreshold(const UI32 &downthreshold);
            UI32 getDownthreshold()  const;
            void setMarginalthreshold(const UI32 &marginalthreshold);
            UI32 getMarginalthreshold()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_downthreshold;
            UI32  m_marginalthreshold;
    };
}
#endif                                            //COMPACTFLASHTHRESHOLDLOCALMANAGEDOBJECT_H
