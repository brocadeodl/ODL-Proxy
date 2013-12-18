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

#ifndef FCOEINTFLOCALMANAGEDOBJECT_H
#define FCOEINTFLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalObjectManager;
    class FcoeIntfLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FcoeIntfLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager);
            FcoeIntfLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager,
                                        const string &name, 
                                        const UI64 &ifindex, 
                                        const UI32 &fc_uport_num, 
                                        const UI32 &mapped_id, 
                                        const UI32 &vn_num, 
                                        const UI32 &port_num,
                                        const bool &status,
                                        const string &mode,
                                        const string &binding,
                                        const UI32 &binding_type);
           
            virtual    ~FcoeIntfLocalManagedObject ();
            static string getClassName();
            void setName(const string &name);
            string getName() const;
            void setIfindex(const UI64 &ifindex);
            UI64 getIfindex() const;
            void setStatus(const bool &status);
            bool getStatus() const;
            void setMode(const string &mode);
            string getMode()  const;
            void setBinding(const string &binding);
            string getBinding()  const;
            void setFcUportNum(const UI32 &fc_uport_num);
            UI32 getFcUportNum() const;
            void setMappedId(const UI32 &mapped_id);
            UI32 getMappedId() const;
            void setVnNum(const UI32 &vn_num);
            UI32 getVnNum() const;
            void setPortNum(const UI32 &port_num);
            UI32 getPortNum() const;
            void setBindingType(const UI32 &binding_type);
            UI32 getBindingType() const;

        // Now the data members

        private:
        protected:
        public:
            string  m_name;
            // Obsolete
            string  m_fc_port_name;

            UI64    m_ifindex;
            UI32    m_fc_uport_num;
            UI32    m_mapped_id;
            UI32    m_vn_num;
            UI32    m_port_num;
            bool    m_status;
            string  m_mode;
            string  m_binding;
            UI32    m_binding_type;
    };
}
#endif                                            //FCOEINTFLOCALMANAGEDOBJECT_H
