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

#ifndef VALINTERFACECONFIGUREVLANMESSAGE_H
#define VALINTERFACECONFIGUREVLANMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class ValInterfaceConfigureVlanMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                          ValInterfaceConfigureVlanMessage ();
                          ValInterfaceConfigureVlanMessage (const ObjectId &interfaceChildObjectId, const UI32 &id);
                          ValInterfaceConfigureVlanMessage (const ObjectId &interfaceChildObjectId, vector<UI32> &list);
                          ValInterfaceConfigureVlanMessage (const ObjectId &interfaceChildObjectId, vector<UI32> &list, string fromString);

        virtual          ~ValInterfaceConfigureVlanMessage ();

                ObjectId  getInterfaceChildObjectId        () const;
				UI32      getId                            () const;
                void      setId                            (const UI32 &vlanId);			
                void      setInterfaceChildObjectId        (const ObjectId &interfaceObjectId);
                vector<UI32> getVlanIdVector () const;
                void      setVlanIdVector (vector<UI32> &list);
                vector<UI32> getVlanIdVector2 () const;
                void      setVlanIdVector2 (vector<UI32> &list);
    // Now the data members

    private :
        ObjectId m_interfaceChildObjectId;
		UI32     m_id;
        vector<UI32> m_vlanIdList;
        vector<UI32> m_vlanIdList2;

    protected :
    public :
};

}

#endif // VALINTERFACECONFIGUREVLANMESSAGE_H
