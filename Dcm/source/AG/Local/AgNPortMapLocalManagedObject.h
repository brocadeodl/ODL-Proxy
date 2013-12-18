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

#ifndef AGNPORTMAPLOCALMANAGEDOBJECT_H
#define AGNPORTMAPLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/Attribute.h"

using namespace WaveNs;

namespace DcmNs
{

    class AgLocalObjectManager;
    class AgNPortMapLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            AgNPortMapLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager);
            AgNPortMapLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager,
                                        const string &agNPortNb);
            AgNPortMapLocalManagedObject (AgLocalObjectManager *pAgLocalObjectManager,
                                        const string &agNPortNb, const vector<string>  &agNFPortTopo,
                                        const bool &agFailover, const bool &agFailback);
            virtual ~AgNPortMapLocalManagedObject ();
            static string getClassName();

            void setNPortNb(const string &NPortNb);
            string getNPortNb() const;

            void setStaticNFPortTopo(const vector<string> &StaticNFPortTopo);
            vector<string> getStaticNFPortTopo() const;

            void setNFPortTopo(const vector<string> &NFPortTopo);
            vector<string> getNFPortTopo() const;

            void setFailover(const bool &agFailover);
            bool getFailover() const;

            void setFailback(const bool &agFailback);
            bool getFailback() const;

	        bool operator < (AgNPortMapLocalManagedObject *w)
	            {
	            return m_agNPortNb < w->m_agNPortNb;
	            }

        // Now the data members

        private:
        protected:
        public:
            string         m_agNPortNb;
            vector<string> m_agNFPortTopo;
            vector<string> m_agStaticNFPortTopo;
            bool           m_agFailover;
            bool           m_agFailback;

    };
}
#endif                                            //AGNPORTMAPLOCALMANAGEDOBJECT_H
