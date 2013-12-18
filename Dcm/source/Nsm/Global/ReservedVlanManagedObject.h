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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Ram Kumar Gandhi                                                  *
 **************************************************************************/

#ifndef RESERVEDVLANMANAGEDOBJECT_H
#define RESERVEDVLANMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class ReservedVlanManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ReservedVlanManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            ReservedVlanManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager, const UI32 &start, const UI32 &end);
            virtual    ~ReservedVlanManagedObject ();
            static string getClassName();
            void setStart (const UI32 &start);
            UI32 getStart ()  const;
  	    		void setEnd (const UI32 &end);
	      		UI32 getEnd() const;

        private:
        protected:
        public:
            UI32UC  m_start;
            UI32UC  m_end;
    };
}
#endif 
