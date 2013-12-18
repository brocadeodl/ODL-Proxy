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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : rhkumar                                                      *
 **************************************************************************/

#ifndef AGPORTGROUPLOCALMANAGEDOBJECT_H
#define AGPORTGROUPLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class AgLocalObjectManager;
    class AgPortGroupLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();

        public:
            AgPortGroupLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager);
            AgPortGroupLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager,
                                        const UI32 &agPgId,  const string &agPgName, const vector<string> &agPgModes);
            AgPortGroupLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager,
                                        const UI32 &agPgId, const bool &agPgEnabled,
                                        const vector<string> &agPgFlags, const string &agPgName,
                                        const vector<string> &agPgNPortTopo, const vector<string> &agPgFPortTopo);

            virtual ~AgPortGroupLocalManagedObject ();
            static string getClassName();

            UI32   getPgId() const;
            void   setPgEnabled(bool &agPgEnabled);
            bool   getPgEnabled() const;
            void   setPgFlags(vector<string> &agPgFlags);
            vector<string> getPgFlags() const;
            void   setPgName(string &agPgName);
            string getPgName() const;
            void   setPgNPortTopo(vector<string> &agPgNPortTopo);
            vector<string> getPgNPortTopo() const;
            void   setPgFPortTopo(vector<string> &agPgFPortTopo);
            vector<string> getPgFPortTopo() const;

        // Now the data members

        private:
        protected:
        public:
            UI32    m_agPgId;
            bool    m_agPgEnabled;
            vector<string>  m_agPgFlags;
            string  m_agPgName;
            vector<string> m_agPgNPortTopo;
            vector<string> m_agPgFPortTopo;
    };
}
#endif                                            //AGPORTGROUPLOCALMANAGEDOBJECT_H
