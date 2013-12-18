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
 *   Copyright (C) 2013      Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Kuai Yu                                                      *
 ***************************************************************************/

#ifndef CLIBLOCKMESSAGE_H
#define CLIBLOCKMESSAGE_H 

#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

class CliBlockDetail;
class CliBlockMessage : public PrismMessage
{
    private :
    protected :
               void                     setupAttributesForSerialization     ();

    public :
                                        CliBlockMessage                     ();
                                        CliBlockMessage                     (bool operation, const CliBlockDetail& cliBlockDetail);
        virtual                        ~CliBlockMessage                     ();

         const bool                     getOperation                        () const;
         const ResourceId               getReason                           () const;
         const LocationId               getOriginator                       () const;
         const PrismServiceId           getPrismServiceId                   () const;

    // Now the data members

    private :
        bool                m_operation; //true is block, false is unblock
        ResourceId          m_reason;
        LocationId          m_originator;
        PrismServiceId      m_prismServiceId;

    protected :
    public :
};

}

#endif // CLIBLOCKMESSAGE_H
