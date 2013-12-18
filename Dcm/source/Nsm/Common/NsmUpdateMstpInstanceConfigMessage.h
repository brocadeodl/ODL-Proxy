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
 * NsmUpdateMstpInstanceConfigMessage.h
 *
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef NSMUPDATEMSTPINSTANCECONFIGMESSAGE_H_
#define NSMUPDATEMSTPINSTANCECONFIGMESSAGE_H_

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmUpdateMstpInstanceConfigMessage : public PrismMessage
    {
    public:

        static NsmUpdateMstpInstanceConfigMessage* forNsmGlobal();
        static NsmUpdateMstpInstanceConfigMessage* forNsmLocal();

        NsmUpdateMstpInstanceConfigMessage (PrismServiceId targetSvc, UI32 opCode);

        UI32 getInstanceId () const;
        void setInstanceId (UI32 id);
        vector<ObjectId> getInstanceVlans () const;
        void setInstanceVlans (const vector<ObjectId> &vlans);

        void setInstanceInfo (UI32 id, const vector<ObjectId> &vlans);

    protected:
        virtual void setupAttributesForSerialization ();

    private:
        UI32 m_instanceId;
        vector<ObjectId> m_instanceVlans;

    };

}
#endif
