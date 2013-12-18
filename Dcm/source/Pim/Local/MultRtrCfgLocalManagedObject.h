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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef MULTRTRCFGLOCALMANAGEDOBJECT_H
#define MULTRTRCFGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimLocalObjectManager;
    class MultRtrCfgLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            MultRtrCfgLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager);
            MultRtrCfgLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager,const bool &presenceFlag,const bool &enable);
            virtual    ~MultRtrCfgLocalManagedObject ();
            static string getClassName();
            void setPresenceFlag(const bool &presenceFlag);
            bool getPresenceFlag()  const;
            void setEnable(const bool &enable);
            bool getEnable()  const;

// Now the data members

        private:
        protected:
        public:
            bool  m_presenceFlag;
            bool  m_enable;
    };
}
#endif                                            //MULTRTRCFGLOCALMANAGEDOBJECT_H
