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
 *   Author : Vidyasagara Reddy Guntaka / Mandar Datar                     *
 ***************************************************************************/

#ifndef YANGDATAELEMENT_H
#define YANGDATAELEMENT_H

#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "Modeling/YANG/ObjectModel/YangDisplayConfigurationContext.h"

namespace WaveNs
{

class YangDataElement : public YangElement
{
    private :
    protected :
    public :
                        YangDataElement                    (const string &yangName, const string &name);
                        YangDataElement                    (const string &yangName);
        virtual        ~YangDataElement                    ();

        virtual string  getCliTargetNodeNameForData        () const;

        virtual void    computeIsConfigurationForSelf      ();
        virtual void    computeFirstDataElementInHierarchy ();

        virtual UI32    getDataIndex                       () const;
                void    setDataIndex                       (const UI32 &dataIndex);

        virtual void    processAttribute                   (const string &attributeName, const string &attributeValue);
        virtual void    processChildElement                (YangElement *pYangElement);
        virtual void    unprocessChildElement              (YangElement *pYangElement);

        virtual bool            isYangElementForData                            () const;

                ResourceId      getValueString                                  (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, string &valueString, ResourceId &valueType);
                ResourceId      getValueStringForAttribute                      (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, Attribute *pAttribute, string &valueString, ResourceId &valueType, const bool &checkForAttributeDefaultValue);
                void            getKeyStringForAssociatedManagedObject          (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, const ObjectId &objectId, string &valueString);
                ResourceId      getAttributeValueStringForDisplayConfiguration  (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, Attribute *pAttribute, string &displayString, WaveManagedObject *pWaveManagedObject, ResourceId &valueType , Attribute *&pFinalAttribute, const bool &checkForAttributeDefaultValue);
                void            displayConfigurtionForSingleElement             (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, const bool &displayOnMatch, const string &stringToMatch, const bool &checkForYangDefault);
                void            getConfigurtionForSingleElement                 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, bool &nothingToPrint, bool &printNoConfig, string &configString, bool &incompleteCommand, string &configValue, const bool &checkForYangDefault);

        virtual bool            isValueSameAsDefault                            (const string &valueString, ResourceId &valueType);

        virtual bool            evaluateWhenConditions                          (YangDisplayConfigurationContext *pYangDisplayConfigurationContext);

        virtual void            applyFormattingToConfigString                   (string &configString) const;

                ResourceId      formatValueString                               (string &valueString, const ResourceId &valueType) const;

        virtual bool            considerForHFileForCLanguage                    () const;

    // Now the data members

    private :
        UI32                        m_dataIndex;

    protected :
    public :
};

}

#endif // YANGDATAELEMENT_H
