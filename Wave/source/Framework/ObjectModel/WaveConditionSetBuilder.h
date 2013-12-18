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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef WAVECONDITIONSETBUILDER_H
#define WAVECONDITIONSETBUILDER_H

#include "Framework/Attributes/AttributeTypes.h"

#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class Attribute;

class WaveConditionSetBuilder
{
    private :
        class WaveConditionSet
        {
            private :
            protected :
            public :
                                                    WaveConditionSet                (const string &waveConditionSetName);            
                virtual                            ~WaveConditionSet                ();            

                const   string                     &getConditionSetName             () const; 
                        void                        addAttributeToConditionSet      (Attribute *pAttribute, AttributeConcatenationOperator concatenationOperation, AttributeConditionOperator conditionOperation);
                        string                      getSql                          ();

            // Now the data members
            private :
                        string                                              m_conditionSetName;
                        vector<Attribute *>                                 m_attributes;
                        map<Attribute *, AttributeConcatenationOperator>    m_attributeConcatenationOperatorsMap;
                        map<Attribute *, AttributeConditionOperator>        m_attributeConditionOperatorsMap;

            protected :
            public :
        };

                bool                                        isConditionSetFound                     (const string &conditionSetName);
                bool                                        isConditionSetCombinationSyntaxCorrect  (const string &conditionSetCombination);

    protected :
    public :
                                                            WaveConditionSetBuilder                 ();
        virtual                                            ~WaveConditionSetBuilder                 ();

                string                                      getSql                                  () const;

                void                                        createConditionSet                      (const string &conditionSetName);
                void                                        addAttributeToConditionSet              (const string &conditionSetName, Attribute *pAttribute, AttributeConcatenationOperator concatenationOperation, AttributeConditionOperator operation);
                void                                        combineConditionSets                    (const string &conditionSetCombination);


    // Now the data members
    private :
                map<string, WaveConditionSet *>             m_conditionSets;                   
                string                                      m_conditionSetCombinationSql;

    protected :
    public :
};

}

#endif // WAVECONDITIONSETBUILDER_H
