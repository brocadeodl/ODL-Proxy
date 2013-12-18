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
 *   Author : Venkat                                                       *
 **************************************************************************/

#ifndef VRRPLOCALSHOWMESSAGE_H
#define VRRPLOCALSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class VRRPLocalShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            VRRPLocalShowMessage ();
            VRRPLocalShowMessage(const UI32 &vrid, const string &ifName, const UI32 &cmdCode, const UI32 &showType, const InterfaceType &ifType);
            virtual ~VRRPLocalShowMessage ();

            void setVrid (const UI32 &vrid);
            UI32 getVrid () const;

            void setIfName(const string &ifName);
            string getIfName() const;

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;

            void setShowType(const UI32 &showType);
            UI32 getShowType() const;

            void setIfType (const InterfaceType &ifType);
            InterfaceType getIfType () const;

            void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);

			void setLastIndex(const UI32 &lastIndex);
			UI32 getLastIndex() const;

// Now the data members

        private:
        protected:
        public:
            UI32    m_vrid;
            string  m_ifName;
            UI32    m_cmdCode;
            UI32    m_showType;
            InterfaceType m_ifType;
			UI32    m_lastIndex;

    };
}
#endif                                            //VRRPLOCALSHOWMESSAGE_H
