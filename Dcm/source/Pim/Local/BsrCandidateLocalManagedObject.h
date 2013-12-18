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

#ifndef BSRCANDIDATELOCALMANAGEDOBJECT_H
#define BSRCANDIDATELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Pim/Local/PimTypes.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimLocalObjectManager;
    class BsrCandidateLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            BsrCandidateLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager);
            virtual    ~BsrCandidateLocalManagedObject ();
            static string getClassName();
            void setIntfType(const PimIntfType &intfType);
            PimIntfType mapIntfTypeToEnumVal(const UI32 &intfType);
            PimIntfType getIntfType()  const;
            void setIntfName(const string &intfName);
            string getIntfName()  const;
            void setMask(const UI16 &mask);
            UI16 getMask()  const;
            void setPriority(const UI32 &priority, bool userConfig = false);
            UI32 getPriority()  const;

// Now the data members

        private:
        protected:
        public:
            PimIntfType  m_intfType;
            string  m_intfName;
            UI16  m_mask;
            UI32UC  m_priority;
    };
}
#endif                                            //BSRCANDIDATELOCALMANAGEDOBJECT_H
