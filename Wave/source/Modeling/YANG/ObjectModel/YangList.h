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

#ifndef YANGLIST_H
#define YANGLIST_H

#include "Modeling/YANG/ObjectModel/YangDataElement.h"

#include <vector>
#include <set>

using namespace std;

namespace WaveNs
{

class YangList : public YangDataElement
{
    private :
                ResourceId      getKeyCombinationValueString                (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, string &keyCombinationValueString);
                void            displayConfigurationForOneInstanceOfList    (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData, WaveManagedObject *pWaveManagedObject);

    protected :
    public :
                                YangList                       ();
        virtual                ~YangList                       ();

        static  string          getYangName                    ();

        static  YangElement    *createInstance                 ();

        virtual void            processChildElement            (YangElement *pYangElement);

                vector<string>  getKeyCombination              () const;
                void            setKeyCombination              (const vector<string> &keyCombination);

                bool            isLeafNamePartOfKeyCombination (const string &leafName) const;

        virtual void            displayConfiguration           (YangDisplayConfigurationContext *pYangDisplayConfigurationContext);

        virtual void            displayConfiguration2          (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData);

        virtual UI32            absorbeKey                     (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData) const;
        virtual void            restoreAbsorbedKeys            (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, const UI32 &numberOfKeysAbsorbed) const;

        virtual void            prepareTypeInformationForSelf  ();

                ResourceId      getStringRepresentationForKeyValueAtIndex   (const UI32 &index, const string &userInputKeyValue, string &stringRepresentation) const;
                void            addKeyDefaultValue                          (const string &keyEntry, const string &defaultValueString);
                void            getDefaultValueStringForKeyIfPresent        (const string &keyEntry, bool &isDefaultValuePresent, string &defaultValueString) const;
        virtual void            propagateCompactSyntaxOneLevelForSelf       ();
        virtual void            collectInformationAboutChildLeafWithCustomDisplayForSelf    ();

        virtual WaveYangMemberElementFilterInformation *collectMemberElementFilterInformation (YangDisplayConfigurationContext *pYangDisplayConfigurationContext) const;

    // Now the data members

    private :
        vector<string> m_keyCombination;
        set<string>    m_keyCombinationSet;

        vector<string>      m_keyCombinationTypes;
        map<string,string>  m_keyNameToDefaultValueMap;

        bool                m_hasChildLeafWithCustomDisplay;

    protected :
    public :
};

}

#endif // YANGLIST_H
