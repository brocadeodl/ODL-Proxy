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
 *   Author : Venkat                                                       *
 **************************************************************************/

#ifndef QOSSHOWMESSAGE_H
#define QOSSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosShowMessage();
            QosShowMessage(const string &ifName,const string &poNum, const UI32 &cmdCode, const UI32 &mappedId, const UI32 &lineCard,
				const UI32 &portSet, const SI32 &chipIndex, const SI32 &portIndex, const UI32 &noOfRecords);
            virtual ~QosShowMessage();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setPoNum(const string &poNum);
            string getPoNum() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;
            void setMappedId(const UI32 &mappedId);
            UI32 getMappedId() const;
            void setLineCard(const UI32 &lineCard);
            UI32 getLineCard() const;
            void setPortSet(const UI32 &portSet);
            UI32 getPortSet() const;

			void setChipIndex (SI32 &chipIndex);
            SI32 getChipIndex () const;
            void setPortIndex (SI32 &portIndex);
            SI32 getPortIndex () const;
            void setNoOfRecords (UI32 &noOfRecords);
            UI32 getNoOfRecords () const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_ifName;
            string m_poNum;
            UI32 m_cmdCode;
		    UI32 m_mappedId;
			UI32 m_lineCard;
			UI32 m_portSet;
			SI32    m_chipIndex;
            SI32    m_portIndex;
            UI32    m_noOfRecords;
    };
}
#endif  //QOSSHOWMESSAGE_H
