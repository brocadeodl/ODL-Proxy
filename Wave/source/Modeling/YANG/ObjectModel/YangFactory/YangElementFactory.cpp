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

#include "Modeling/YANG/ObjectModel/YangFactory/YangElementFactory.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "Modeling/YANG/ObjectModel/YangAugment.h"
#include "Modeling/YANG/ObjectModel/YangBit.h"
#include "Modeling/YANG/ObjectModel/YangCase.h"
#include "Modeling/YANG/ObjectModel/YangChoice.h"
#include "Modeling/YANG/ObjectModel/YangConfig.h"
#include "Modeling/YANG/ObjectModel/YangContact.h"
#include "Modeling/YANG/ObjectModel/YangContainer.h"
#include "Modeling/YANG/ObjectModel/YangDefault.h"
#include "Modeling/YANG/ObjectModel/YangDescription.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "Modeling/YANG/ObjectModel/YangEnum.h"
#include "Modeling/YANG/ObjectModel/YangErrorMessage.h"
#include "Modeling/YANG/ObjectModel/YangFeature.h"
#include "Modeling/YANG/ObjectModel/YangFractionDigits.h"
#include "Modeling/YANG/ObjectModel/YangGrouping.h"
#include "Modeling/YANG/ObjectModel/YangIfFeature.h"
#include "Modeling/YANG/ObjectModel/YangImport.h"
#include "Modeling/YANG/ObjectModel/YangInput.h"
#include "Modeling/YANG/ObjectModel/YangKey.h"
#include "Modeling/YANG/ObjectModel/YangLeaf.h"
#include "Modeling/YANG/ObjectModel/YangLeafList.h"
#include "Modeling/YANG/ObjectModel/YangLength.h"
#include "Modeling/YANG/ObjectModel/YangList.h"
#include "Modeling/YANG/ObjectModel/YangMandatory.h"
#include "Modeling/YANG/ObjectModel/YangMaxElements.h"
#include "Modeling/YANG/ObjectModel/YangModule.h"
#include "Modeling/YANG/ObjectModel/YangNameSpace.h"
#include "Modeling/YANG/ObjectModel/YangOrderedBy.h"
#include "Modeling/YANG/ObjectModel/YangOrganization.h"
#include "Modeling/YANG/ObjectModel/YangOutput.h"
#include "Modeling/YANG/ObjectModel/YangPath.h"
#include "Modeling/YANG/ObjectModel/YangPattern.h"
#include "Modeling/YANG/ObjectModel/YangPosition.h"
#include "Modeling/YANG/ObjectModel/YangPrefix.h"
#include "Modeling/YANG/ObjectModel/YangPresence.h"
#include "Modeling/YANG/ObjectModel/YangRange.h"
#include "Modeling/YANG/ObjectModel/YangReference.h"
#include "Modeling/YANG/ObjectModel/YangRefine.h"
#include "Modeling/YANG/ObjectModel/YangRevision.h"
#include "Modeling/YANG/ObjectModel/YangRpc.h"
#include "Modeling/YANG/ObjectModel/YangStatus.h"
#include "Modeling/YANG/ObjectModel/YangText.h"
#include "Modeling/YANG/ObjectModel/YangTypedef.h"
#include "Modeling/YANG/ObjectModel/YangType.h"
#include "Modeling/YANG/ObjectModel/YangUnits.h"
#include "Modeling/YANG/ObjectModel/YangUses.h"
#include "Modeling/YANG/ObjectModel/YangValue.h"
#include "Modeling/YANG/ObjectModel/YangWhen.h"
#include "Modeling/YANG/ObjectModel/YangUnique.h"

#include "Modeling/YANG/ObjectModel/YangUserInterface.h"

#include "Modeling/YANG/ObjectModel/YangValue.h"

namespace WaveNs
{

map<string, YangElementInstantiator> YangElementFactory::m_yangElementInstantiatorMap;

bool YangElementFactory::isAKnownYangName (const string &yangName)
{
    map<string, YangElementInstantiator>::const_iterator element    = m_yangElementInstantiatorMap.find (yangName);
    map<string, YangElementInstantiator>::const_iterator endElement = m_yangElementInstantiatorMap.end  ();

    if (element != endElement)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void YangElementFactory::addYangElementInstantiator (const string &yangName, YangElementInstantiator yangElementInstantiator)
{
    if (false == (isAKnownYangName (yangName)))
    {
        m_yangElementInstantiatorMap[yangName] = yangElementInstantiator;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "YangElementFactory::addYangElementInstantiator : YANG Element Instantiator for \"" + yangName + "\" has already been added.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

YangElement *YangElementFactory::createYangElement (const string &yangName)
{
    YangElement *pYangElement = NULL;

    if (true == (isAKnownYangName (yangName)))
    {
        YangElementInstantiator yangElementInstantiator = m_yangElementInstantiatorMap[yangName];

        if (NULL != yangElementInstantiator)
        {
            pYangElement = (*yangElementInstantiator) ();
        }
    }

    return (pYangElement);
}

void YangElementFactory::initialize ()
{
    static bool alreadyinitialized = false;

    if (false == alreadyinitialized)
    {
        addYangElementInstantiator (YangAugment::getYangName (),        &YangAugment::createInstance);
        addYangElementInstantiator (YangBit::getYangName (),            &YangBit::createInstance);
        addYangElementInstantiator (YangCase::getYangName (),           &YangCase::createInstance);
        addYangElementInstantiator (YangChoice::getYangName (),         &YangChoice::createInstance);
        addYangElementInstantiator (YangConfig::getYangName (),         &YangConfig::createInstance);
        addYangElementInstantiator (YangContact::getYangName (),        &YangContact::createInstance);
        addYangElementInstantiator (YangContainer::getYangName (),      &YangContainer::createInstance);
        addYangElementInstantiator (YangDefault::getYangName (),        &YangDefault::createInstance);
        addYangElementInstantiator (YangDescription::getYangName (),    &YangDescription::createInstance);
        addYangElementInstantiator (YangEnum::getYangName (),           &YangEnum::createInstance);
        addYangElementInstantiator (YangErrorMessage::getYangName (),   &YangErrorMessage::createInstance);
        addYangElementInstantiator (YangFeature::getYangName (),        &YangFeature::createInstance);
        addYangElementInstantiator (YangFractionDigits::getYangName (), &YangFractionDigits::createInstance);
        addYangElementInstantiator (YangGrouping::getYangName (),       &YangGrouping::createInstance);
        addYangElementInstantiator (YangIfFeature::getYangName (),      &YangIfFeature::createInstance);
        addYangElementInstantiator (YangImport::getYangName (),         &YangImport::createInstance);
        addYangElementInstantiator (YangInput::getYangName (),          &YangInput::createInstance);
        addYangElementInstantiator (YangKey::getYangName (),            &YangKey::createInstance);
        addYangElementInstantiator (YangLeaf::getYangName (),           &YangLeaf::createInstance);
        addYangElementInstantiator (YangLeafList::getYangName (),       &YangLeafList::createInstance);
        addYangElementInstantiator (YangLength::getYangName (),         &YangLength::createInstance);
        addYangElementInstantiator (YangList::getYangName (),           &YangList::createInstance);
        addYangElementInstantiator (YangMandatory::getYangName (),      &YangMandatory::createInstance);
        addYangElementInstantiator (YangMaxElements::getYangName (),    &YangMaxElements::createInstance);
        addYangElementInstantiator (YangModule::getYangName (),         &YangModule::createInstance);
        addYangElementInstantiator (YangNameSpace::getYangName (),      &YangNameSpace::createInstance);
        addYangElementInstantiator (YangOrderedBy::getYangName (),      &YangOrderedBy::createInstance);
        addYangElementInstantiator (YangOrganization::getYangName (),   &YangOrganization::createInstance);
        addYangElementInstantiator (YangOutput::getYangName (),         &YangOutput::createInstance);
        addYangElementInstantiator (YangPath::getYangName (),           &YangPath::createInstance);
        addYangElementInstantiator (YangPattern::getYangName (),        &YangPattern::createInstance);
        addYangElementInstantiator (YangPosition::getYangName (),       &YangPosition::createInstance);
        addYangElementInstantiator (YangPrefix::getYangName (),         &YangPrefix::createInstance);
        addYangElementInstantiator (YangPresence::getYangName (),       &YangPresence::createInstance);
        addYangElementInstantiator (YangRange::getYangName (),          &YangRange::createInstance);
        addYangElementInstantiator (YangReference::getYangName (),      &YangReference::createInstance);
        addYangElementInstantiator (YangRefine::getYangName (),         &YangRefine::createInstance);
        addYangElementInstantiator (YangRevision::getYangName (),       &YangRevision::createInstance);
        addYangElementInstantiator (YangRpc::getYangName (),            &YangRpc::createInstance);
        addYangElementInstantiator (YangStatus::getYangName (),         &YangStatus::createInstance);
        addYangElementInstantiator (YangText::getYangName (),           &YangText::createInstance);
        addYangElementInstantiator (YangType::getYangName (),           &YangType::createInstance);
        addYangElementInstantiator (YangTypedef::getYangName (),        &YangTypedef::createInstance);
        addYangElementInstantiator (YangUnits::getYangName (),          &YangUnits::createInstance);
        addYangElementInstantiator (YangUses::getYangName (),           &YangUses::createInstance);
        addYangElementInstantiator (YangValue::getYangName (),          &YangValue::createInstance);
        addYangElementInstantiator (YangWhen::getYangName (),           &YangWhen::createInstance);
        addYangElementInstantiator (YangUnique::getYangName (),         &YangUnique::createInstance);

        // Reserve it for last for readability

        addYangElementInstantiator (YangUserInterface::getYangName (),  &YangUserInterface::createInstance);

        alreadyinitialized = true;
    }
}

}
