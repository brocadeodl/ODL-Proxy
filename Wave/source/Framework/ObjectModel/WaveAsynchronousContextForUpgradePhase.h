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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef WAVEASYNCHRONOUSCONTEXTFORUPGRADEPHASE_H
#define WAVEASYNCHRONOUSCONTEXTFORUPGRADEPHASE_H

#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectRelationalMapping/OMSpecificSchemaChangeInfoForUpgrade.h"

namespace WaveNs
{

class FirmwareVersion
{
    private :
    protected :
    public :
                        FirmwareVersion ();
                        FirmwareVersion (const string &majorNumber, const string &minorNumber, const string &patchString, const string &description);
                       ~FirmwareVersion ();

        bool            isSameVersion   (const string &versionString);
                       
    private :
        string          m_majorNumber;
        string          m_minorNumber;
        string          m_patchString;
        string          m_description;
    protected :
    public :
};

class WaveAsynchronousContextForUpgradePhase : public WaveAsynchronousContextForBootPhases 
{
    private :
    protected :
    public :
                                WaveAsynchronousContextForUpgradePhase  (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL);
        virtual                ~WaveAsynchronousContextForUpgradePhase  ();
        
/*
        vector<string>          getNewManagedObjects                    () const;
        void                    setNewManagedObjects                    (const vector<string> &newManagedObjects);

        vector<string>          getChangedManagedObjects                () const;
        void                    setChangedManagedObjects                (const vector<string> &changedManagedObjects);
*/
        OMSpecificSchemaChangeInfoForUpgrade* getSchemaChangeInfo       () const;  
        void                                  setSchemaChangeInfo       (OMSpecificSchemaChangeInfoForUpgrade* pSchemaChangeInfo);
 
        bool                    checkFromVersionString                  (const string &versionString);
        bool                    checkToVersionString                    (const string &versionString);

    // Now the data members

    private :
        /*
        vector<string>  m_changedManagedObjects;
        vector<string>  m_newManagedObjects;
        */
        
        OMSpecificSchemaChangeInfoForUpgrade* m_schemaChangeInfo;
        FirmwareVersion m_fromVersion;
        FirmwareVersion m_toVersion;

    protected :
    public :
};



}

#endif // WAVEASYNCHRONOUSCONTEXTFORUPGRADEPHASE_H
