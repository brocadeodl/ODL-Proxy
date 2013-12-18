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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpCommunityWorker.h"
#include "SnmpServer/Global/SnmpContextMappingWorker.h"
#include "SnmpServer/Global/ConfigHostWorker.h"
#include "SnmpServer/Global/ConfigUserWorker.h"
#include "SnmpServer/Global/ConfigV3HostWorker.h"
#include "SnmpServer/Global/ConfigSystemGroupWorker.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "SnmpServer/Global/CommunityManagedObject.h"
#include "SnmpServer/Global/HostsManagedObject.h"
//#include "snmp/snmplib.h"

namespace DcmNs
{

    SnmpServerObjectManager::SnmpServerObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pSnmpCommunityWorker = new SnmpCommunityWorker (this);
        prismAssert (NULL != m_pSnmpCommunityWorker, __FILE__, __LINE__);
        m_pConfigHostWorker = new ConfigHostWorker (this);
        prismAssert (NULL != m_pConfigHostWorker, __FILE__, __LINE__);
        m_pConfigSystemGroupWorker = new ConfigSystemGroupWorker (this);
        prismAssert (NULL != m_pConfigSystemGroupWorker, __FILE__, __LINE__);
        m_pConfigV3HostWorker = new ConfigV3HostWorker (this);
        prismAssert (NULL != m_pConfigV3HostWorker, __FILE__, __LINE__);
        m_pConfigUserWorker = new ConfigUserWorker (this);
        prismAssert (NULL != m_pConfigUserWorker, __FILE__, __LINE__);
        m_pSnmpContextMappingWorker = new SnmpContextMappingWorker (this);
        prismAssert (NULL != m_pSnmpContextMappingWorker,__FILE__, __LINE__);
    }

    SnmpServerObjectManager::~SnmpServerObjectManager ()
    {
		delete (m_pSnmpCommunityWorker);
		delete (m_pConfigUserWorker);
		delete (m_pConfigHostWorker);
		delete (m_pConfigV3HostWorker);
		delete (m_pConfigSystemGroupWorker);
        delete (m_pSnmpContextMappingWorker);
    }

    SnmpServerObjectManager  *SnmpServerObjectManager::getInstance()
    {
        static SnmpServerObjectManager *pSnmpServerObjectManager = new SnmpServerObjectManager ();

        WaveNs::prismAssert (NULL != pSnmpServerObjectManager, __FILE__, __LINE__);

        return (pSnmpServerObjectManager);
    }

    string  SnmpServerObjectManager::getClassName()
    {
        return ("SnmpServer");
    }

    PrismServiceId  SnmpServerObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *SnmpServerObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }
/*
void SnmpServerObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status               = FRAMEWORK_ERROR;
    UI32        i                   = 0;
//	agtcfg_t    pComm;
    CommunityManagedObject      *pSetObject=NULL;
    vector<CommunityManagedObject *> CommunityGroup;
    string CommString[] = {"Secret C0de", "OrigEquipMfr", "private", "public", "common", "FibreChannel"};

    UI32 CommAccess[] = {1, 1, 1, 0, 0, 0};

    startTransaction ();

    for (i = 0; i < 6; i++) {
        pSetObject = new CommunityManagedObject (this); 
        pSetObject->setCommunity (string (CommString[i]));
        pSetObject->setAccess ((Access) (CommAccess[i]));
//		strcpy(pComm.agtcommunityentries[i].communitystr, CommString[i].c_str());
//		pComm.agtcommunityentries[i].access = CommAccess[i];
        CommunityGroup.push_back (pSetObject);
    }
//		int ret = agtcfgXMSet(&pComm);

 //   	if (!ret) 
//		trace (TRACE_LEVEL_ERROR, "Install - return OK");

        status = commitTransaction ();

        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, "SFILocalLocalObjectManager::install: installed 1 record by default for demo");

            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "SFILocalLocalObjectManager::install: Failed to install 1 record  by default for demo");
            prismAssert (false, __FILE__, __LINE__);
        }

    delete pSetObject;
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
     pWaveAsynchronousContextForBootPhases->callback ();
}
*/
    WaveManagedObject  *SnmpServerObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
