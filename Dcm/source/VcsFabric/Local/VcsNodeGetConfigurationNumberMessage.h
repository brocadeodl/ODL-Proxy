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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef VCSNODEGETCONFIGURATIONNUMBERMESSAGE_H
#define VCSNODEGETCONFIGURATIONNUMBERMESSAGE_H

#include "ManagementInterface/ManagementInterfaceServiceIndependentMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeGetConfigurationNumberMessage : public ManagementInterfaceServiceIndependentMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        //PhysicalInterfaceCreateMessage ();
                        VcsNodeGetConfigurationNumberMessage ();
        virtual        ~VcsNodeGetConfigurationNumberMessage ();

                void    getConfigurationNumber           (UI16 configurationNumber[16]);
                void    setconfigurationNumber           (const vector<UI64> &configurationNumber);

    // Now the data members

    private :
        vector<UI64>    m_configurationNumber;

    protected :
    public :
};

}

#endif // VCSNODEGETCONFIGURATIONNUMBERMESSAGE_H
