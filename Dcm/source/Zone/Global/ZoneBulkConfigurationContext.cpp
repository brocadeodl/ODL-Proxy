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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Zone/Global/ZoneBulkConfigurationContext.h"
#include "Zone/Global/ZoneBulkConfigurationMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/AssertUtils.h"
#include "Zone/Global/CfgManagedObject.h"
#include "Zone/Global/CfgMemberManagedObject.h"
#include "Zone/Global/ZoneManagedObject.h"
#include "Zone/Global/ZoneMemberManagedObject.h"

using WaveNs::prismAssert;

namespace DcmNs
{

ZoneBulkConfigurationContext::ZoneBulkConfigurationContext (ZoneBulkConfigurationMessage *pZoneBulkConfigurationMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pZoneBulkConfigurationMessage, pPrismElement, pSteps, numberOfSteps),
      m_cfgs                      (pZoneBulkConfigurationMessage->getCfgs        ()),
      m_cfgMembers                (pZoneBulkConfigurationMessage->getCfgMembers  ()),
      m_zones                     (pZoneBulkConfigurationMessage->getZones       ()),
      m_zoneMembers               (pZoneBulkConfigurationMessage->getZoneMembers ())
{
}

ZoneBulkConfigurationContext::~ZoneBulkConfigurationContext ()
{
}
vector<string> &ZoneBulkConfigurationContext::getCfgs ()
{
    return (m_cfgs);
}

vector<vector<string> > &ZoneBulkConfigurationContext::getCfgMembers ()
{
    return (m_cfgMembers);
}

vector<string> &ZoneBulkConfigurationContext::getZones ()
{
    return (m_zones);
}

vector<vector<string> > &ZoneBulkConfigurationContext::getZoneMembers ()
{
    return (m_zoneMembers);
}

void ZoneBulkConfigurationContext::loadZoneDatabaseConciseInformarionIntoMemory ()
{
    loadZoneDatabaseConciseInformarionInternal (CfgManagedObject::getClassName        (), true);
    loadZoneDatabaseConciseInformarionInternal (CfgMemberManagedObject::getClassName  (), false);
    loadZoneDatabaseConciseInformarionInternal (ZoneManagedObject::getClassName       (), true);
    loadZoneDatabaseConciseInformarionInternal (ZoneMemberManagedObject::getClassName (), false);
}

void ZoneBulkConfigurationContext::loadZoneDatabaseConciseInformarionInternal (const string &managedObjectClassName, const bool &addToNamesMap)
{
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (managedObjectClassName);

    synchronousQueryContext.addSelectField ("keyString");

    vector<WaveManagedObject *> *pResultsVector = WaveObjectManagerToolKit::querySynchronously (&synchronousQueryContext);

    prismAssert (NULL != pResultsVector, __FILE__, __LINE__);

    UI32 numberOfResults = pResultsVector->size ();
    UI32 i               = 0;

    for (i = 0; i < numberOfResults; i++)
    {
        WaveManagedObject *pWaveManagedObject = (*pResultsVector)[i];

        prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

        string         keyString        = pWaveManagedObject->getKeyString ();
        vector<string> keyStringTokens;
        string         name;

        StringUtils::tokenize (keyString, keyStringTokens, '*');

        if (2 <= (keyStringTokens.size ()))
        {
            name = keyStringTokens[1];
        }

        addKeyStringInformation (pWaveManagedObject->getObjectClassName(), keyString, pWaveManagedObject->getObjectId (), name, addToNamesMap);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultsVector);
}

void ZoneBulkConfigurationContext::addKeyStringInformation (const string &managedObjectClassName, const string &keyString, const ObjectId &objectId, const string &name, const bool &addToNamesMap)
{
    bool isKnown = isAKnownKeyString (keyString);

    if (true == isKnown)
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    m_keyStringsMap[keyString] = objectId;

    if (true == addToNamesMap)
    {
        m_namesMap[managedObjectClassName][name] = objectId;
    }
}

bool ZoneBulkConfigurationContext::isAKnownKeyString (const string &keyString) const
{
    map<string, ObjectId>::const_iterator element    = m_keyStringsMap.find (keyString);
    map<string, ObjectId>::const_iterator endElement = m_keyStringsMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void ZoneBulkConfigurationContext::getExistanceAndValidatyDetailsForCfg (const string &cfgName,  bool &isValid, bool &doesCfgExist,  ObjectId &cfgObjectId)
{
    bool isKnownCfg  = isAKnownName (CfgManagedObject::getClassName  (), cfgName);
    bool isKnownZone = isAKnownName (ZoneManagedObject::getClassName (), cfgName);



    if (true == isKnownCfg)
    {
        prismAssert (false == isKnownZone, __FILE__, __LINE__);

        isValid      = true;
        doesCfgExist = true;
        cfgObjectId  = m_namesMap[CfgManagedObject::getClassName ()][cfgName];
    }
    else
    {
        if (true == isKnownZone)
        {
            prismAssert (false == isKnownCfg, __FILE__, __LINE__);

            isValid      = false;
            doesCfgExist = false;
        }
        else
        {
            isValid      = true;
            doesCfgExist = false;
        }
    }
}

void ZoneBulkConfigurationContext::getExistanceAndValidatyDetailsForZone (const string &zoneName,  bool &isValid, bool &doesZoneExist,  ObjectId &zoneObjectId)
{
    bool isKnownCfg  = isAKnownName (CfgManagedObject::getClassName  (), zoneName);
    bool isKnownZone = isAKnownName (ZoneManagedObject::getClassName (), zoneName);



    if (true == isKnownZone)
    {
        prismAssert (false == isKnownCfg, __FILE__, __LINE__);

        isValid      = true;
        doesZoneExist = true;
        zoneObjectId  = m_namesMap[ZoneManagedObject::getClassName ()][zoneName];
    }
    else
    {
        if (true == isKnownCfg)
        {
            prismAssert (false == isKnownZone, __FILE__, __LINE__);

            isValid      = false;
            doesZoneExist = false;
        }
        else
        {
            isValid      = true;
            doesZoneExist = false;
        }
    }
}


bool ZoneBulkConfigurationContext::isAKnownName (const string &managedObjectClassName, const string &name)
{
    map<string, map <string, ObjectId> >::const_iterator element    = m_namesMap.find (managedObjectClassName);
    map<string, map <string, ObjectId> >::const_iterator endElement = m_namesMap.end  ();

    if (endElement != element)
    {
        map<string, ObjectId>::const_iterator element1    = (element->second).find (name);
        map<string, ObjectId>::const_iterator endElement1 = (element->second).end  ();

        if (endElement1 != element1)
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

}
