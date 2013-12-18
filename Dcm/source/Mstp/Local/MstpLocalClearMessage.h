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

/* **
 * MstpLocalClearMessage.h
 *
 * Dec 26, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef MSTPLOCALCLEARMESSAGE_H
#define MSTPLOCALCLEARMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpClearMessage;


    class MstpLocalClearMessage : public PrismMessage
    {
        public:
            MstpLocalClearMessage ();
            virtual ~MstpLocalClearMessage ();

            void copyFrom (MstpLocalClearMessage *other);
            void copyTo (MstpClearMessage *pClearMessage);

            void setCmdCode (UI32 cmdCode);
            UI32 getCmdCode () const;
            void setInterfaceInfo (InterfaceType ifType, const string &ifName);
            InterfaceType getIfType() const;
            string getIfName() const;

            void setDistribute (bool b);
            bool isDistribute() const;

        protected:
            virtual void setupAttributesForSerialization ();


        public:
            // When set to true, MstpLocal plugin would relay the
            // message to the cluster.. Otherwise the message will be
            // forwarded to mstpd.
            bool    m_distribute;

            UI32            m_cmdCode;
            InterfaceType   m_ifType;
            string          m_ifName;

    };

}
#endif
