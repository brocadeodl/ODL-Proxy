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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef APPMVLANCLASSIFIERMANAGEDOBJECT_H
#define APPMVLANCLASSIFIERMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalObjectManager;
    class APPMVlanClassifierManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
			virtual void setupKeys();
        public:
            APPMVlanClassifierManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            APPMVlanClassifierManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const UI32 &grpId,const UI32 & dummy,const UI32 &vlanId);
            virtual    ~APPMVlanClassifierManagedObject ();
            static string getClassName();
            void setGrpId(const UI32 &grpId);
            UI32 getGrpId()  const;
            void setVlanclassifierOperation(const UI32 &vlanclassifieroperation);
            UI32 getVlanclassifierOperation()  const;
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_grpId;
            UI32  m_vlanclassifieroperation;
            UI32  m_vlanId;
    };
}
#endif                                            //APPMVLANCLASSIFIERMANAGEDOBJECT_H
