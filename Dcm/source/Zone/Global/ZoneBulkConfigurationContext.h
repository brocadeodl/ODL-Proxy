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

#ifndef ZONEBULKCONFIGURATIONCONTEXT_H
#define ZONEBULKCONFIGURATIONCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/ObjectId.h"

#include <string>
#include <map>

using namespace WaveNs;
using namespace std;

namespace DcmNs
{

class ZoneBulkConfigurationMessage;

class ZoneBulkConfigurationContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                 ZoneBulkConfigurationContext                 (ZoneBulkConfigurationMessage *pZoneBulkConfigurationMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                ~ZoneBulkConfigurationContext                 ();

        vector<string>          &getCfgs                                      ();
        vector<vector<string> > &getCfgMembers                                ();
        vector<string>          &getZones                                     ();
        vector<vector<string> > &getZoneMembers                               ();

        void                     loadZoneDatabaseConciseInformarionIntoMemory ();
        void                     loadZoneDatabaseConciseInformarionInternal   (const string &managedObjectClassName, const bool &addToNamesMap);
        void                     getExistanceAndValidatyDetailsForCfg         (const string &cfgName,  bool &isValid, bool &doesCfgExist,  ObjectId &cfgObjectId);
        void                     getExistanceAndValidatyDetailsForZone        (const string &zoneName, bool &isValid, bool &doesZoneExist, ObjectId &zoneObjectId);

        void                     addKeyStringInformation                      (const string &managedObjectClassName, const string &keyString, const ObjectId &objectId, const string &name, const bool &addToNamesMap = true);
        bool                     isAKnownKeyString                            (const string &keyString) const;
        bool                     isAKnownName                                 (const string &managedObjectClassName, const string &name);

    // Now the data members

    private :
        vector<string>                      &m_cfgs;
        vector<vector<string> >             &m_cfgMembers;
        vector<string>                      &m_zones;
        vector<vector<string> >             &m_zoneMembers;

        map<string, ObjectId>                m_keyStringsMap;
        map<string, map<string, ObjectId> >  m_namesMap;

    protected :
    public :
};

}

#endif // ZONEBULKCONFIGURATIONCONTEXT_H
