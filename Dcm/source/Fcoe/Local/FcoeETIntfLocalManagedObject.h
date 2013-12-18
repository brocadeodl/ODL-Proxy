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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef FCOEETINTFLOCALMANAGEDOBJECT_H
#define FCOEETINTFLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalObjectManager;
    class FcoeETIntfLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FcoeETIntfLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager);
            virtual    ~FcoeETIntfLocalManagedObject ();
            static string getClassName();
            void setName(const string &name);
            string getName() const;
            void setIfindex(const UI64 &ifindex);
            UI64 getIfindex() const;
            void setStatus(const bool &status);
            bool getStatus() const;
            void setFcUportNum(const SI32 &fc_uport_num);
            SI32 getFcUportNum() const;
            void setRbId(const UI32 &rb_id);
            UI32 getRbId() const;
            void setBindType(const UI32 &bind_type);
            UI32 getBindType() const;
            void setTrunkStatus(const bool &trunk_status);
            bool getTrunkStatus() const;
            void setAsBreakoutPort(const bool &breakout);
            bool isBreakoutPort() const;

        // Now the data members

        private:
        protected:
        public:
            string  m_name;
            UI64    m_ifindex;
            SI32    m_fc_uport_num;
            bool    m_status;
            bool    m_trunk_status;
            UI32    m_rb_id;
            UI32    m_bind_type;
            bool    m_breakout;
    };
}
#endif                                            //FCOEINTFLOCALMANAGEDOBJECT_H
