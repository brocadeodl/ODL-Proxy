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
 *   Author : ranantha/rhkumar                                             *
 **************************************************************************/

#ifndef AGLOCALMANAGEDOBJECT_H
#define AGLOCALMANAGEDOBJECT_H

#define DEFAULT_REL_COUNTER_VALUE 25
#define DEFAULT_FNMTO_VALUE 120


#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class AgLocalObjectManager;
    class AgLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            AgLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager);
            AgLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager,
                                        const vector<string> &policy, const bool &fportTrunkEnable);
            virtual ~AgLocalManagedObject ();
            static string getClassName();
            void setPolicy(vector<string> &policy);
            vector<string> getPolicy() const;
            void setFportTrunkEnable(bool &fportTrunkEnable);
            bool getFportTrunkEnable() const;
            UI32 getFnmtov() const;
            void setFnmtov(UI32 &fnmtovalue);
            UI32 getRelCounterValue() const;
            void setRelCounterValue(UI32 &relcountervalue);

        // Now the data members

        private:
        protected:
        public:
            vector<string> m_policy;
            bool   m_fportTrunkEnable;
            UI32   m_fnmtovalue;
            UI32   m_relcountervalue;
    };
}
#endif                                            //AGLOCALMANAGEDOBJECT_H
