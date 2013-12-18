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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef SNMPOBJECTID_H
#define SNMPOBJECTID_H

#include "Framework/Types/Types.h"
#include <string>

using namespace std;

namespace WaveNs
{

class SnmpObjectId
{
    private:
              void          parseSnmpObjectId   (string &objectIdentifier);
              void          copy                (const SnmpObjectId &objectIdentifier);
    protected:
    public:
                            SnmpObjectId        ();
                            SnmpObjectId        (const string &objectIdentifier);
                            SnmpObjectId        (const SnmpObjectId &objectIdentifier);
              virtual      ~SnmpObjectId        ();

              string        toString            () const;
              void          fromString          (const string &objectIdentifier);
              ResourceId    loadFromPlainString (const string &objectIdentifier);
              void          getOid              (UI32 *oid) const;
              SI32          getLength           () const;

              bool          operator ==             (const SnmpObjectId &snmpObjectId) const;
              bool          operator !=             (const SnmpObjectId &snmpObjectId) const;
              SnmpObjectId  &operator =             (const SnmpObjectId &snmpObjectId);


            // now the data members
    private:
              UI32        m_Oid[128];
              SI32        m_Length;

    protected:
    public:
};

}

#endif    //SNMPOBJECTID_H
