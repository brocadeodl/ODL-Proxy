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

#ifndef POLICERGLOBALCLASSMAPMESSAGE_H
#define POLICERGLOBALCLASSMAPMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerGlobalClassMapMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PolicerGlobalClassMapMessage ();
            PolicerGlobalClassMapMessage (const string &classMapName,const bool &any,const UI32 &opcode, const UI32 &filterCriteria, const string &accessListName);
            virtual    ~PolicerGlobalClassMapMessage ();
            void setClassMapName(const string &classMapName);
            string getClassMapName()  const;
            void setAny(const bool &any);
            bool getAny()  const;
            void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;

            string getAccessListName() const;
            void setAccessListName(const string &accessListName);

            UI32 getFilterCriteria() const;
            void setFilterCriteria(const UI32 &filterCriteria);

// Now the data members

        private:
        protected:
        public:
            string  m_classMapName;
            bool  m_any;
            UI32  m_opcode;
            UI32 m_filterCriteria;
            string m_accessListName;
    };
}
#endif                                            //POLICERGLOBALCLASSMAPMESSAGE_H
