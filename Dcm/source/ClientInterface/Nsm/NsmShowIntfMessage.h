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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef NSMSHOWINTFMESSAGE_H
#define NSMSHOWINTFMESSAGE_H

#include "ClientInterface/Nsm/PoPaginationMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmShowIntfMessage : public PoPaginationMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmShowIntfMessage ();
            NsmShowIntfMessage (const string &ifName,const UI32 &cmdCode,const UI32 &mappedId);
            virtual    ~NsmShowIntfMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
			void setMappedId (const UI32 &mappedId);
			UI32 getMappedId ()  const;
			void setReqNoRecords(const UI32 &reqNoRecords);
			UI32 getReqNoRecords()  const;
			void setReqStartIndex(const string &reqStartIndex);
			string getReqStartIndex()  const;
			void setReqEndIndex(const string &reqEndIndex);
			string getReqEndIndex()  const;
			void setReqIfType(const InterfaceType &reqIfType);
			InterfaceType getReqIfType()  const;
			void setReqRequestType(const UI32 &reqRequestType);
			UI32 getReqRequestType()  const;

            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32  m_cmdCode;
			UI32  m_mappedId;
			UI32  m_reqNoRecords;
			string m_reqStartIndex;
			string m_reqEndIndex;
			InterfaceType m_reqIfType;
			UI32 m_reqRequestType;
    };
}
#endif                                            //NSMSHOWINTFMESSAGE_H
