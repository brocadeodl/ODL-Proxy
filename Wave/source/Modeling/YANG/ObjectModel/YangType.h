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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef YANGTYPE_H
#define YANGTYPE_H

#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "Framework/Attributes/Attribute.h"

namespace WaveNs
{

class YangType : public YangElement
{
    private :
    protected :
    public :
                              YangType                                 ();
        virtual              ~YangType                                 ();

        static  string        getYangName                              ();


        static  YangElement  *createInstance                           ();

        static  void          initializeBuiltinTypes                   ();
                bool          isKnownBuiltinType                       (const string &typeName) const;

                void          populateEnumAttribute                    (string name, UI32 value);
                ResourceId    getValueFromEnumName                     (const string name, UI32 &value);
                ResourceId    getEnumNameFromValue                     (string &name, const UI32 &value);

                void          populateBitMapAttribute                  (string name, UI32 value);
                void          getBitPositionFromBitName                (string name, UI32 &value);
                void          getBitNameFromBitPosition                (string &name, UI32 value);
                void          getStringValueFrom32BitBitMap            (string &bitMapString, UI32 bitPattern);
                void          getStringValueFrom64BitBitMap            (string &bitMapString, UI64 bitPattern);
                void          getDisplayStringForYangTypeInsideUnion   (Attribute *pAttribute, string &displayString );

        virtual void          processChildElement                      (YangElement *pYangElement);

    // Now the data members
    private :

        static  set<string>    m_knownBuiltinTypes;

                Attribute     *m_pAttribute;

                UI32           m_nextEnumValue;

    protected :
    public :
};

}

#endif // YANGTYPE_H
