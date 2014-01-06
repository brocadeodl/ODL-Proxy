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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef VALINTERFACELOCALOBJECTMANAGERCONFIGURECHILDRENINTERFACEMESSAGE_H
#define VALINTERFACELOCALOBJECTMANAGERCONFIGURECHILDRENINTERFACEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                          ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage ();
                          ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage (const ObjectId &interfaceChildObjectId, const bool &isAllowed);

        virtual          ~ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage ();

                ObjectId  getInterfaceChildObjectId                                    () const;

				bool      getIsAllowed                                           () const;
			
    // Now the data members

    private :
        ObjectId m_interfaceChildObjectId;
		bool     m_isAllowed;

    protected :
    public :
};

}

#endif // VALINTERFACELOCALOBJECTMANAGERCONFIGURECHILDRENINTERFACEMESSAGE_H