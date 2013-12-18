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

#ifndef LINECARDALERTLOCALMANAGEDOBJECT_H
#define LINECARDALERTLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/AttributeBitMap.h"
using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorLocalObjectManager;
    class LineCardAlertLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            LineCardAlertLocalManagedObject (SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager);
            LineCardAlertLocalManagedObject (SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager,const BitMap &state,const BitMap &action);
            virtual    ~LineCardAlertLocalManagedObject ();
            static string getClassName();
            void setState(const BitMap &state);
            BitMap getState()  const;
            void setAction(const BitMap &action);
            BitMap getAction()  const;

// Now the data members

        private:
        protected:
        public:
            BitMap  m_state;
            BitMap  m_action;
    };
}
#endif                                            //LINECARDALERTLOCALMANAGEDOBJECT_H
