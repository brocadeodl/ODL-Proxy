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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef QOSCLIENTCREATEDSCPMARKLISTMESSAGE_H
#define QOSCLIENTCREATEDSCPMARKLISTMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosClientCreateDscpMarkListMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosClientCreateDscpMarkListMessage ();
            QosClientCreateDscpMarkListMessage (const string &markString, const string &dscpMapName, const SI32 &toValue, const UI8 &qosMapType);
            virtual    ~QosClientCreateDscpMarkListMessage ();
			void setMarkString(const string &dscpMarkString);
			string getMarkString() const;
			void setDscpMapName(const string &dscpMapName);
			string getDscpMapName() const;
			void setDscpMarkToValue(const SI32 &toValue);
			SI32 getDscpMarkToValue() const;
			void setQosMapType(const UI8 &qosMapType);
			UI8 getQosMapType() const;
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
			string m_markString;
			string m_dscpMapName;
			SI32	m_toValue;
			UI8		m_qosMapType;
    };
}
#endif                                          //QOSCLIENTCREATEDSCPMARKLISTMESSAGE_H
