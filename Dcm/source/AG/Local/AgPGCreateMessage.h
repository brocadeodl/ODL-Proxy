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
 *   Author : Ramachandran Anantharamn/Rohit Kumar H                       *
 **************************************************************************/

#ifndef AGPGCREATEMESSAGE_H
#define AGPGCREATEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    class AgPGCreateMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            AgPGCreateMessage ();
            AgPGCreateMessage (const UI32 &pgId, const vector<string> &nPorts, const vector<string> &fPorts, const string &pgName, const vector<string> &pgModes);

            AgPGCreateMessage (const UI32 &pgId);

            virtual    ~AgPGCreateMessage ();


            void setpgId(const UI32 &pgId);
            UI32 getpgId() const;

            void setnPorts(const vector<string> &nPorts);
            vector<string> getnPorts()  const;

            void setfPorts(const vector<string> &fPorts);
            vector<string> getfPorts()  const;

            void setpgName(const string &pgName);
            string getpgName()  const;

            void setpgModes(const vector<string> &pgModes);
            vector<string> getpgModes()  const;

// Now the data members

        private:
        protected:
        public:
            UI32 m_pgId;
            vector<string> m_nPorts;
            vector<string> m_fPorts;
            string m_pgName;
            vector<string> m_pgModes;

    };
}
#endif                                            //AGPGCREATEMESSAGE_H
