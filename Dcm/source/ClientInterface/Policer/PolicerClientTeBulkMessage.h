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

#ifndef POLICERCLIENTTEBULKMESSAGE_H
#define POLICERCLIENTTEBULKMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerClientTeBulkMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PolicerClientTeBulkMessage ();
            PolicerClientTeBulkMessage (const string &ifName, const string &inputMapName,const string &outputMapName);
            virtual    ~PolicerClientTeBulkMessage ();
            void setInputMapName(const string &inputMapName);
            string getInputMapName()  const;
			void setIfName(const string &ifName);
            string getIfName() const;
            void setOutputMapName(const string &outputMapName);
			string getOutputMapName()  const;
			virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
			string m_ifName;
            string m_inputMapName;
			string m_outputMapName;
    };
}
#endif                                            //POLICERCLIENTTeMESSAGE_H
