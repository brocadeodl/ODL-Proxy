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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#ifndef WAVECONFIGURATIONATTRIBUTES_H
#define WAVECONFIGURATIONATTRIBUTES_H

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/WaveCValue.h"
#include "Framework/Attributes/Attribute.h"

using namespace WaveNs;

namespace WaveNs
{

class WaveConfigurationAttributes : virtual public SerializableObject
{
    private :
    protected :
                virtual void setupAttributesForSerialization                        ();
                virtual void setupAttributesForSerializationInAttributeOrderFormat  ();
    public :
                        WaveConfigurationAttributes (string configName, string configValue, ResourceId configType);
                        WaveConfigurationAttributes ();
        virtual        ~WaveConfigurationAttributes ();

        string          getConfigName               ();
        string          getConfigValue              ();
        ResourceId      getConfigType               ();
        void            loadWaveConfiguration       (const string &globalConfig);
        void            getAttributeValue           (const ResourceId attributeType, const string attributeValue, WaveCValue *pCValue);

    static void         getConfigurationVectors     (vector<WaveConfigurationAttributes> &configAttributes, vector<string> &configNames, vector<string> &configValues, vector<ResourceId> &configTypes);   
    // Now the data members
    private :   
    protected :
                string      m_configName;
                string      m_configValue;
                ResourceId  m_configAttributeType;
    public :

};

}

#endif // WAVECONFIGURATIONATTRIBUTES_H
