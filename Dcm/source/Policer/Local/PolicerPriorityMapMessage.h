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

#ifndef POLICERPRIORITYMAPMESSAGE_H
#define POLICERPRIORITYMAPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerPriorityMapMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PolicerPriorityMapMessage ();
            PolicerPriorityMapMessage (const string &priorityMapName,const UI8 &conformPriorityMap0,const UI8 &conformPriorityMap1,const UI8 &conformPriorityMap2,const UI8 &conformPriorityMap3,const UI8 &conformPriorityMap4,const UI8 &conformPriorityMap5,const UI8 &conformPriorityMap6,const UI8 &conformPriorityMap7,const UI8 &exceedPriorityMap0,const UI8 &exceedPriorityMap1,const UI8 &exceedPriorityMap2,const UI8 &exceedPriorityMap3,const UI8 &exceedPriorityMap4,const UI8 &exceedPriorityMap5,const UI8 &exceedPriorityMap6,const UI8 &exceedPriorityMap7, const UI16 &conformCos, const UI16 &exceedCos, const UI32 &opcode);
            virtual    ~PolicerPriorityMapMessage ();
            void setPriorityMapName(const string &priorityMapName);
            string getPriorityMapName()  const;
            void setConformPriorityMap0(const UI8 &conformPriorityMap0);
            UI8 getConformPriorityMap0()  const;
            void setConformPriorityMap1(const UI8 &conformPriorityMap1);
            UI8 getConformPriorityMap1()  const;
            void setConformPriorityMap2(const UI8 &conformPriorityMap2);
            UI8 getConformPriorityMap2()  const;
            void setConformPriorityMap3(const UI8 &conformPriorityMap3);
            UI8 getConformPriorityMap3()  const;
            void setConformPriorityMap4(const UI8 &conformPriorityMap4);
            UI8 getConformPriorityMap4()  const;
            void setConformPriorityMap5(const UI8 &conformPriorityMap5);
            UI8 getConformPriorityMap5()  const;
            void setConformPriorityMap6(const UI8 &conformPriorityMap6);
            UI8 getConformPriorityMap6()  const;
            void setConformPriorityMap7(const UI8 &conformPriorityMap7);
            UI8 getConformPriorityMap7()  const;
            void setExceedPriorityMap0(const UI8 &exceedPriorityMap0);
            UI8 getExceedPriorityMap0()  const;
            void setExceedPriorityMap1(const UI8 &exceedPriorityMap1);
            UI8 getExceedPriorityMap1()  const;
            void setExceedPriorityMap2(const UI8 &exceedPriorityMap2);
            UI8 getExceedPriorityMap2()  const;
            void setExceedPriorityMap3(const UI8 &exceedPriorityMap3);
            UI8 getExceedPriorityMap3()  const;
            void setExceedPriorityMap4(const UI8 &exceedPriorityMap4);
            UI8 getExceedPriorityMap4()  const;
            void setExceedPriorityMap5(const UI8 &exceedPriorityMap5);
            UI8 getExceedPriorityMap5()  const;
            void setExceedPriorityMap6(const UI8 &exceedPriorityMap6);
            UI8 getExceedPriorityMap6()  const;
            void setExceedPriorityMap7(const UI8 &exceedPriorityMap7);
            UI8 getExceedPriorityMap7()  const;
            void setConformCos(const UI16 &conformCos);
			UI16 getConformCos() const;
			void setExceedCos(const UI16 &exceedCos);
			UI16 getExceedCos() const;
			void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_priorityMapName;
            UI8  m_conformPriorityMap0;
            UI8  m_conformPriorityMap1;
            UI8  m_conformPriorityMap2;
            UI8  m_conformPriorityMap3;
            UI8  m_conformPriorityMap4;
            UI8  m_conformPriorityMap5;
            UI8  m_conformPriorityMap6;
            UI8  m_conformPriorityMap7;
            UI8  m_exceedPriorityMap0;
            UI8  m_exceedPriorityMap1;
            UI8  m_exceedPriorityMap2;
            UI8  m_exceedPriorityMap3;
            UI8  m_exceedPriorityMap4;
            UI8  m_exceedPriorityMap5;
            UI8  m_exceedPriorityMap6;
            UI8  m_exceedPriorityMap7;
			UI16 m_set_conform_cos;
			UI16 m_set_exceed_cos;
            UI32  m_opcode;
    };
}
#endif                                            //POLICERPRIORITYMAPMESSAGE_H
