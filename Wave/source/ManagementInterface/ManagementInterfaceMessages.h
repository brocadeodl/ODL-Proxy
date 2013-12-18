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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef MANAGEMENTINTERFACEMESSAGES_H
#define MANAGEMENTINTERFACEMESSAGES_H

#include "Framework/Attributes/AttributesMap.h"
#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

class RequestToManagementInterfaceMessage : public PrismMessage
{
    private :
    protected :
    public :
        RequestToManagementInterfaceMessage (PrismServiceId servciceId, UI32 operationId, AttributesMap *pAttributesMap);

     // Now the data members

     private :
        AttributesMap *m_pInputAttributesMap;
     protected :
     public :
};

}

#endif // MANAGEMENTINTERFACEMESSAGES_H
