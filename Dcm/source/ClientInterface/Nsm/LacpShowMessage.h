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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#ifndef LACPSHOWMESSAGE_H
#define LACPSHOWMESSAGE_H

#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Nsm/PoPaginationMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LacpShowMessage : public PoPaginationMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LacpShowMessage ();
            LacpShowMessage (const string &ifName,const UI32 &poNum,const UI32 &cmdCode, const UI32 &mappedId);
            virtual ~LacpShowMessage ();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setPoNum(const UI32 &poNum);
            UI32 getPoNum() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;
            void setMappedId(const UI32 &mappedId);
            UI32 getMappedId() const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 
													*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, 
												UI32 &size, void *&pBuffer);
            void getOutputBufferRef (UI32 bufferNum, 
												UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_ifName;
            UI32 m_poNum;
            UI32 m_cmdCode;
            UI32 m_mappedId;
			UI32 m_ifType;
    };
}
#endif  //LACPSHOWMESSAGE_H