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
 *   Author : dchakrab                                                     *
 **************************************************************************/
#ifndef RMONHISTORYBOOKKEEPLOCALMANAGEDOBJECT_H
#define RMONHISTORYBOOKKEEPLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class RmonHistoryBookKeepLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            RmonHistoryBookKeepLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            RmonHistoryBookKeepLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const SI32 &historyIndex,const string &ifName);
            virtual    ~RmonHistoryBookKeepLocalManagedObject ();
            static string getClassName();
            void setHistoryIndex(const SI32 &historyIndex);
            SI32 getHistoryIndex()  const;
	    	void setIfName(const string& ifName);
	    	string getIfName() const;
// Now the data members

        private:
        protected:
        public:
            SI32  m_historyIndex;
			string m_ifName;		
    };
}
#endif                                            //RMONHISTORYLOCALMANAGEDOBJECT_H

