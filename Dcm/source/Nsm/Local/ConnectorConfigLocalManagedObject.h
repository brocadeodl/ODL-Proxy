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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Venkat                                                       *
 **************************************************************************/

#ifndef CONNECTORCONFIGLOCALMANAGEDOBJECT_H
#define CONNECTORCONFIGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;
namespace DcmNs
{

    class NsmLocalObjectManager;
    class ConnectorConfigLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            ConnectorConfigLocalManagedObject (NsmLocalObjectManager *pVcsFabricLocalObjectManager);
            ConnectorConfigLocalManagedObject (NsmLocalObjectManager *pVcsFabricLocalObjectManager, const string &name, const bool &breakout);
            virtual ~ConnectorConfigLocalManagedObject();
            static string getClassName();
            void setName(const string &name);
            string getName() const;
            void setBreakout(const bool &breakout);
            bool getBreakout() const;

// Now the data members

        private:
            string m_name;
			bool m_breakout;
        protected:
        public:
    };
}
#endif                                            //CONNECTORCONFIGLOCALMANAGEDOBJECT_H
