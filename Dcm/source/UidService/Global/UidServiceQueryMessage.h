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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sholla                                                     *
 **************************************************************************/

#ifndef UIDSERVICEQUERYMESSAGE_H
#define UIDSERVICEQUERYMESSAGE_H

#include <vector>
#include <string>
#include "UidServiceMessage.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs
{

class UidServiceQueryMessage : public UidServiceMessage
{
    private:
    protected:
        virtual void setupAttributesForSerialization();

    public:
        UidServiceQueryMessage (const UI32 opcode);
        virtual ~UidServiceQueryMessage ();

        UI32    getCount () const;
        UI32    getIdAt (const UI32 index) const;
        bool    getActiveAt (const UI32 index) const;
        string  getEntityNameAt (const UI32 index) const;

        void addEntityName (const string& entityName);

        void addIdInfo (const string& entityName, const UI32 id, const bool active);

        // Indicate that all id info are set, and ready
        // for transmission..
        void pack();


    private:
        vector<UI32>    idList;
        vector<bool>    activeList;
        vector<string>  entityNameList;

};

}

#endif                                            //UIDSERVICEMESSAGE_H
