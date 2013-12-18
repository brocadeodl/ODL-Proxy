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
 *   Author : MMREDDY                                                      *
 **************************************************************************/

#ifndef FCOEENODEMANAGEDOBJECT_H
#define FCOEENODEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalObjectManager;
    class FcoeEnodeLocalManagedObject :virtual public DcmLocalManagedObject 
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FcoeEnodeLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager);
            FcoeEnodeLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager, string fcoe_enodes_fab_map, uint32_t fcoe_enodes, bool upgrade);
            static string getClassName();
            virtual    ~FcoeEnodeLocalManagedObject ();
            void setFcoeEnodes(const UI32 &fcoe_enodes);
            UI32 getFcoeEnodes() const;
            void setFcoeEnodesUpgradeFlag(const bool &upgrade);
            bool getFcoeEnodesUpgradeFlag() const;
            void setFcoeEnodesFabricMap(const string &enodes_fab_map);
            string getFcoeEnodesFabricMap() const;
            void setupKeys();

// Now the data members

        private:
        protected:
        public:
            string m_fcoe_enodes_fab_map;
            UI32  m_fcoe_enodes;
            bool  m_upgrade;
    };
}
#endif                                            //FCOEFABRICMAPMANAGEDOBJECT_H

