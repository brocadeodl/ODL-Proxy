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
 * NsmVlanProvisionLocalMessage.h
 *
 * Jun 26, 2013
 * Copyright (C) 2013 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef NSMVLANPROVISIONLOCALMESSAGE_H_
#define NSMVLANPROVISIONLOCALMESSAGE_H_

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/UI32Range.h"

using namespace WaveNs;

namespace DcmNs {

class NsmVlanProvisionLocalMessage : public ManagementInterfaceMessage
{
public:
    NsmVlanProvisionLocalMessage ();
    virtual ~NsmVlanProvisionLocalMessage ();

    void setUnprovisionVlans (const UI32Range &givds);
    UI32Range getUnprovisionVlans () const;
    bool hasUnprovisionVlans() const;

    void setProvisionVlans (const UI32Range &gvids);
    UI32Range getProvisionVlans () const;
    bool hasProvisionVlans() const;

    void setNeedsExplicitUnprovision (bool flag);
    bool getNeedsExplicitUnprovision () const;

    bool isTriggerVlanUnprovision() const;
    void setTriggerVlanUnprovision(bool triggerVlanUnprovision);

protected:
    virtual void setupAttributesForSerialization ();

private:
    UI32Range   m_unprovisionVlans;

    UI32Range   m_provisionVlans;
    bool        m_needsExplicitUnprovision;
    bool        m_triggerVlanUnprovision;

};

}
#endif /* NSMVLANPROVISIONLOCALMESSAGE_H_ */
