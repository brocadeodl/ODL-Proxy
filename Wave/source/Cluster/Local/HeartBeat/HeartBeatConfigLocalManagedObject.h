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
 *   Author : jvenkata                                                     *
 **************************************************************************/

#ifndef HEARTBEATCONFIGLOCALMANAGEDOBJECT_H
#define HEARTBEATCONFIGLOCALMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveLocalManagedObject.h"

namespace WaveNs
{

    class HeartBeatObjectManager;
    class HeartBeatConfigLocalManagedObject : public WaveLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            HeartBeatConfigLocalManagedObject (HeartBeatObjectManager *pHeartBeatObjectManager);
            HeartBeatConfigLocalManagedObject (HeartBeatObjectManager *pHeartBeatObjectManager,const UI32 &defaultHeartBeatInterval,const UI32 &defaultHeartBeatFailureThreshold);
            virtual    ~HeartBeatConfigLocalManagedObject ();
            static string getClassName();
            void setDefaultHeartBeatInterval(const UI32 &defaultHeartBeatInterval);
            UI32 getDefaultHeartBeatInterval()  const;
            void setDefaultHeartBeatFailureThreshold(const UI32 &defaultHeartBeatFailureThreshold);
            UI32 getDefaultHeartBeatFailureThreshold()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_defaultHeartBeatInterval;
            UI32  m_defaultHeartBeatFailureThreshold;
    };
}
#endif                                            //HEARTBEATCONFIGLOCALMANAGEDOBJECT_H