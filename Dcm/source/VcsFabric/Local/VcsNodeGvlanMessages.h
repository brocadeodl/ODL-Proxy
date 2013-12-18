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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Mithun BS                                                    *
 ***************************************************************************/

#ifndef VCSNODEGVLANMESSAGES_H
#define VCSNODEGVLANMESSAGES_H

#include "ManagementInterface/ManagementInterfaceServiceIndependentMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeIsGvlanCapableMessage : public ManagementInterfaceServiceIndependentMessage
{
    private :
                void        setupAttributesForSerialization ();

    protected :
    public :
                            VcsNodeIsGvlanCapableMessage();
		virtual	            ~VcsNodeIsGvlanCapableMessage();


                bool        isGvlanCapable          () const;
                void        setGvlanCapable         (bool gvlanCapable); 

    // Now the data members

    private :
                bool        m_isGvlanCapable;

    protected :
    public :
};

class VcsNodeIsGvlanEnabledMessage : public ManagementInterfaceServiceIndependentMessage
{
    private :
                void        setupAttributesForSerialization ();

    protected :
    public :
                            VcsNodeIsGvlanEnabledMessage();
		virtual	            ~VcsNodeIsGvlanEnabledMessage();


                bool        isGvlanEnabled          () const;
                void        setGvlanEnabled         (bool gvlanEnabled); 

    // Now the data members

    private :
                bool        m_isGvlanEnabled;

    protected :
    public :
};

}

#endif //VCSNODEGVLANMESSAGES_H
