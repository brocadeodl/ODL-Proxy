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
 *   Author : nbellari                                                     *
 **************************************************************************/

#ifndef DSCPMARKLISTMANAGEDOBJECT_H
#define DSCPMARKLISTMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class DscpMarkListManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            DscpMarkListManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager);
            DscpMarkListManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &markList,const SI32 &toValue);
            virtual    ~DscpMarkListManagedObject ();
            static string getClassName();
            void setMarkList(const string &markList);
            string getMarkList()  const;
            void setToValue(const SI32 &toValue);
            SI32 getToValue()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_markList;
            SI32  m_toValue;
    };
}
#endif                                            //DSCPMARKLISTMANAGEDOBJECT_H
