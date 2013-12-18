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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#ifndef POLICERPRIORITYMAPMANAGEDOBJECT_H
#define POLICERPRIORITYMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerGlobalObjectManager;
    class PolicerPriorityMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PolicerPriorityMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager);
            PolicerPriorityMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager,const string &priorityMapName,const UI32UC &conformPriorityMap0,const UI32UC &conformPriorityMap1,const UI32UC &conformPriorityMap2,const UI32UC &conformPriorityMap3,const UI32UC &conformPriorityMap4,const UI32UC &conformPriorityMap5,const UI32UC &conformPriorityMap6,const UI32UC &conformPriorityMap7,const UI32UC &exceedPriorityMap0,const UI32UC &exceedPriorityMap1,const UI32UC &exceedPriorityMap2,const UI32UC &exceedPriorityMap3,const UI32UC &exceedPriorityMap4,const UI32UC &exceedPriorityMap5,const UI32UC &exceedPriorityMap6,const UI32UC &exceedPriorityMap7);
            virtual    ~PolicerPriorityMapManagedObject ();
            static string getClassName();
            void setPriorityMapName(const string &priorityMapName);
            string getPriorityMapName()  const;
            void setConformPriorityMap0(const UI32 &conformPriorityMap0);
            UI32 getConformPriorityMap0()  const;
            void setConformPriorityMap1(const UI32 &conformPriorityMap1);
            UI32 getConformPriorityMap1()  const;
            void setConformPriorityMap2(const UI32 &conformPriorityMap2);
            UI32 getConformPriorityMap2()  const;
            void setConformPriorityMap3(const UI32 &conformPriorityMap3);
            UI32 getConformPriorityMap3()  const;
            void setConformPriorityMap4(const UI32 &conformPriorityMap4);
            UI32 getConformPriorityMap4()  const;
            void setConformPriorityMap5(const UI32 &conformPriorityMap5);
            UI32 getConformPriorityMap5()  const;
            void setConformPriorityMap6(const UI32 &conformPriorityMap6);
            UI32 getConformPriorityMap6()  const;
            void setConformPriorityMap7(const UI32 &conformPriorityMap7);
            UI32 getConformPriorityMap7()  const;
            void setExceedPriorityMap0(const UI32 &exceedPriorityMap0);
            UI32 getExceedPriorityMap0()  const;
            void setExceedPriorityMap1(const UI32 &exceedPriorityMap1);
            UI32 getExceedPriorityMap1()  const;
            void setExceedPriorityMap2(const UI32 &exceedPriorityMap2);
            UI32 getExceedPriorityMap2()  const;
            void setExceedPriorityMap3(const UI32 &exceedPriorityMap3);
            UI32 getExceedPriorityMap3()  const;
            void setExceedPriorityMap4(const UI32 &exceedPriorityMap4);
            UI32 getExceedPriorityMap4()  const;
            void setExceedPriorityMap5(const UI32 &exceedPriorityMap5);
            UI32 getExceedPriorityMap5()  const;
            void setExceedPriorityMap6(const UI32 &exceedPriorityMap6);
            UI32 getExceedPriorityMap6()  const;
            void setExceedPriorityMap7(const UI32 &exceedPriorityMap7);
            UI32 getExceedPriorityMap7()  const;
			void setUserConfiguredConform();
			void setUserConfiguredExceed();
// Now the data members

        private:
        protected:
        public:
            string  m_priorityMapName;
            UI32UC  m_conformPriorityMap0;
            UI32UC  m_conformPriorityMap1;
            UI32UC  m_conformPriorityMap2;
            UI32UC  m_conformPriorityMap3;
            UI32UC  m_conformPriorityMap4;
            UI32UC  m_conformPriorityMap5;
            UI32UC  m_conformPriorityMap6;
            UI32UC  m_conformPriorityMap7;
            UI32UC  m_exceedPriorityMap0;
            UI32UC  m_exceedPriorityMap1;
            UI32UC  m_exceedPriorityMap2;
            UI32UC  m_exceedPriorityMap3;
            UI32UC  m_exceedPriorityMap4;
            UI32UC  m_exceedPriorityMap5;
            UI32UC  m_exceedPriorityMap6;
            UI32UC  m_exceedPriorityMap7;
    };
}
#endif                                            //POLICERPRIORITYMAPMANAGEDOBJECT_H
