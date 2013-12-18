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

#ifndef RPCANDIDATELOCALMANAGEDOBJECT_H
#define RPCANDIDATELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeStringUC.h"
#include "Pim/Local/PimTypes.h"


using namespace WaveNs;

namespace DcmNs
{

    class PimLocalObjectManager;
    class RpCandidateLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            RpCandidateLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager);
            virtual    ~RpCandidateLocalManagedObject ();
            static string getClassName();
            void setIntfType(const PimIntfType &intfType);
            PimIntfType mapIntfTypeToEnumVal(const UI32 &intfType);
            PimIntfType getIntfType()  const;
            void setIntfName(const string &intfName);
            string getIntfName()  const;
            void setPrefixList(const string &prefixList, bool userConfig = false);
            string getPrefixList()  const;

// Now the data members

        private:
        protected:
        public:
            PimIntfType  m_intfType;
            string  m_intfName;
            StringUC  m_prefixList;
    };
}
#endif                                            //RPCANDIDATELOCALMANAGEDOBJECT_H
