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
 * MstpPortIdConfigMessage.h
 *
 * Jul 11, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef MSTPPORTIDCONFIGMESSAGE_H_
#define MSTPPORTIDCONFIGMESSAGE_H_

#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"


namespace DcmNs {

class MstpPortIdConfigMessage : public DcmManagementInterfaceMessage
{

public:
    MstpPortIdConfigMessage ();
    virtual    ~MstpPortIdConfigMessage ();

    bool hasMore () const;
    void setHasMore (const bool b);

    void setPortIdInfo(vector<mstp_msg_port_id_info> &portIdList);

    virtual const void *getCStructureForInputs ();
    virtual void  loadOutputsFromCStructure (const void *out);

protected:
    virtual void setupAttributesForSerialization ();

private:
    bool    m_hasMore;  // Indicate if this is the last batch

};

}

#endif /* MSTPPORTIDCONFIGMESSAGE_H_ */
