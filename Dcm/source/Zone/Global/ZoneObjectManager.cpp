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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgLocalManagedObject.h"
#include "PortCfg/Local/PortCfgPortCfgPortStateUpdateMessage.h"
#include "PortCfg/Local/PortCfgPortCfgTrunkUpdateMessage.h"
#include "PortCfg/Local/PortCfgPortCfgCreateMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "PortCfg/Local/PortCfgPortCfgCreateMessage.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/NsmDefaultConfigurationCmdsMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/DBAttributesManagedObject.h"
#include "Zone/Global/AliasMemberManagedObject.h"
#include "Zone/Global/AliasManagedObject.h"
#include "Zone/Global/ZoneMemberManagedObject.h"
#include "Zone/Global/ZoneManagedObject.h"
#include "Zone/Global/CfgMemberManagedObject.h"
#include "Zone/Global/CfgManagedObject.h"
#include "Zone/Global/ZoneDbCreateMessage.h"
#include "Zone/Global/ZoneDbAddMessage.h"
#include "Zone/Global/ZoneDbRemoveMessage.h"
#include "Zone/Global/ZoneDbDeleteMessage.h"
#include "Zone/Global/ZoneDbSaveMessage.h"
#include "Zone/Global/ZoneDbClearMessage.h"
#include "Zone/Global/ZoneDbEnableMessage.h"
#include "Zone/Global/ZoneDbDisableMessage.h"
#include "Zone/Global/ZoneDbTransAbortMessage.h"
#include "Zone/Global/ZoneOperationInfoMessage.h"
#include "Zone/Global/ZoneDefinedCfgConfigurationHandler.h"
#include "Zone/Global/ZoneDefinedCfgMemberConfigurationHandler.h"
#include "Zone/Global/ZoneDefinedZoneConfigurationHandler.h"
#include "Zone/Global/ZoneDefinedZoneMemberConfigurationHandler.h"
#include "Zone/Global/ZoneDefinedAliasConfigurationHandler.h"
#include "Zone/Global/ZoneDefinedAliasMemberConfigurationHandler.h"
#include "Zone/Global/ZoneTypes.h"

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "SystemManagement/WaveSystemManagementGateway.h"
#include "DcmCore/DcmToolKit.h"

#include "Framework/Utils/FrameworkToolKit.h"
#include "Shell/ShellDebug.h"
#include "SystemManagement/Configuration/CliConfigurationFile.h"
#include "Framework/Utils/TraceUtils.h"
#include "Zone/Global/ZoneBulkConfigurationMessage.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Zone/Global/ZoneBulkConfigurationWorker.h"

#include "vcs.h"

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"

extern "C"
{
#include "ts/public.h"

using WaveNs::trace;

#define CFG_NAME_MARKER	" *"
}

namespace DcmNs
{

    ZoneObjectManager::ZoneObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        DBAttributesManagedObject    DBAttributesManagedObject    (this);
        AliasMemberManagedObject    AliasMemberManagedObject    (this);
        AliasManagedObject    AliasManagedObject    (this);
        ZoneMemberManagedObject    ZoneMemberManagedObject    (this);
        ZoneManagedObject    ZoneManagedObject    (this);
        CfgMemberManagedObject    CfgMemberManagedObject    (this);
        CfgManagedObject    CfgManagedObject    (this);
        DBAttributesManagedObject.setupOrm ();
        addManagedClass (DBAttributesManagedObject::getClassName ());
        AliasMemberManagedObject.setupOrm ();
        addManagedClass (AliasMemberManagedObject::getClassName ());
        AliasManagedObject.setupOrm ();
        addManagedClass (AliasManagedObject::getClassName ());
        ZoneMemberManagedObject.setupOrm ();
        addManagedClass (ZoneMemberManagedObject::getClassName ());
        ZoneManagedObject.setupOrm ();
        addManagedClass (ZoneManagedObject::getClassName ());
        CfgMemberManagedObject.setupOrm ();
        addManagedClass (CfgMemberManagedObject::getClassName ());
        CfgManagedObject.setupOrm ();
        addManagedClass (CfgManagedObject::getClassName ());
        addOperationMap (ZONEDBCREATE, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneDbCreateMessageHandler));
        addOperationMap (ZONEDBADD, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneDbAddMessageHandler));
        addOperationMap (ZONEDBREMOVE, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneDbRemoveMessageHandler));
        addOperationMap (ZONEDBDELETE, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneDbDeleteMessageHandler));
        addOperationMap (ZONEDBSAVE, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneDbSaveMessageHandler));
        addOperationMap (ZONEDBCLEAR, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneDbClearMessageHandler));
        addOperationMap (ZONEDBENABLE, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneDbEnableMessageHandler));
        addOperationMap (ZONEDBDISABLE, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneDbDisableMessageHandler));
        addOperationMap (ZONEDBTRANSABORT, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneDbTransAbortMessageHandler));
	    addOperationMap (ZONEOPERATIONINFO, reinterpret_cast<PrismMessageHandler> (&ZoneObjectManager::ZoneOperationInfoMessageHandler));

		skipSCNReload = FALSE;
		firstTimeTransFailure = FALSE;

        PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());

        m_pZoneDefinedCfgConfigurationHandler = new ZoneDefinedCfgConfigurationHandler (this);
        prismAssert (NULL != m_pZoneDefinedCfgConfigurationHandler, __FILE__, __LINE__);        
        
        m_pZoneDefinedCfgMemberConfigurationHandler = new ZoneDefinedCfgMemberConfigurationHandler (this);
        prismAssert (NULL != m_pZoneDefinedCfgMemberConfigurationHandler, __FILE__, __LINE__);        
        
        m_pZoneDefinedZoneConfigurationHandler = new ZoneDefinedZoneConfigurationHandler (this);
        prismAssert (NULL != m_pZoneDefinedZoneConfigurationHandler, __FILE__, __LINE__);        
        
        m_pZoneDefinedZoneMemberConfigurationHandler = new ZoneDefinedZoneMemberConfigurationHandler (this);
        prismAssert (NULL != m_pZoneDefinedZoneMemberConfigurationHandler, __FILE__, __LINE__);

		m_pZoneDefinedAliasConfigurationHandler = new ZoneDefinedAliasConfigurationHandler (this);
		prismAssert (NULL != m_pZoneDefinedAliasConfigurationHandler, __FILE__, __LINE__);        

		m_pZoneDefinedAliasMemberConfigurationHandler = new ZoneDefinedAliasMemberConfigurationHandler (this);
		prismAssert (NULL != m_pZoneDefinedAliasMemberConfigurationHandler, __FILE__, __LINE__);

		m_pZoneBulkConfigurationWorker = new ZoneBulkConfigurationWorker (this);

		prismAssert (NULL != m_pZoneBulkConfigurationWorker, __FILE__, __LINE__);

		addDebugFunction ((ShellCmdFunction) (&ZoneObjectManager::loadZoningBulkConfiguration), "loadZoningBulkConfiguration");
    }

    ZoneObjectManager::~ZoneObjectManager ()
    {
    }

    ZoneObjectManager  *ZoneObjectManager::getInstance()
    {
        static ZoneObjectManager *pZoneObjectManager = new ZoneObjectManager ();

        WaveNs::prismAssert (NULL != pZoneObjectManager, __FILE__, __LINE__);

        return (pZoneObjectManager);
    }

    PrismServiceId  ZoneObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *ZoneObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case ZONEDBCREATE :
                pPrismMessage = new ZoneDbCreateMessage ();
                break;
            case ZONEDBADD :
                pPrismMessage = new ZoneDbAddMessage ();
                break;
            case ZONEDBREMOVE :
                pPrismMessage = new ZoneDbRemoveMessage ();
                break;
            case ZONEDBDELETE :
                pPrismMessage = new ZoneDbDeleteMessage ();
                break;
            case ZONEDBSAVE :
                pPrismMessage = new ZoneDbSaveMessage ();
                break;
            case ZONEDBCLEAR :
                pPrismMessage = new ZoneDbClearMessage ();
                break;
            case ZONEDBENABLE :
                pPrismMessage = new ZoneDbEnableMessage ();
                break;
            case ZONEDBDISABLE :
                pPrismMessage = new ZoneDbDisableMessage ();
                break;
            case ZONEDBTRANSABORT :
                pPrismMessage = new ZoneDbTransAbortMessage ();
                break;
	        case ZONEOPERATIONINFO :
	            pPrismMessage = new ZoneOperationInfoMessage ();
	            break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ZoneObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((DBAttributesManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new DBAttributesManagedObject(this);
        }
        else if ((AliasMemberManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new AliasMemberManagedObject(this);
        }
        else if ((AliasManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new AliasManagedObject(this);
        }
        else if ((ZoneMemberManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ZoneMemberManagedObject(this);
        }
        else if ((ZoneManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ZoneManagedObject(this);
        }
        else if ((CfgMemberManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new CfgMemberManagedObject(this);
        }
        else if ((CfgManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new CfgManagedObject(this);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "ZoneObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  ZoneObjectManager::ZoneDbAddMessageHandler( ZoneDbAddMessage *pZoneDbAddMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneDbAddMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  ZoneObjectManager::ZoneDbClearMessageHandler( ZoneDbClearMessage *pZoneDbClearMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::dbClear),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::freeAllocatedManagedObjects),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneDbClearMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

		chkFirstTimeTransFailure(pPrismSynchronousLinearSequencerContext);
    }

    void  ZoneObjectManager::ZoneDbCreateMessageHandler( ZoneDbCreateMessage *pZoneDbCreateMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::dbCreate),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

			/** Wyser Backend Create Support. Add ParentMOs to the Wyser
			 *  Cache lookup table. Then call freeAllocatedManagedObjects()
			 *  to do cleanup...ONLY do this for the parentMOs that are
			 *  newly created from dbCreate. */
	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::backendWyserZonePluginCreate),
	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::freeAllocatedManagedObjects),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneDbCreateMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

		chkFirstTimeTransFailure(pPrismSynchronousLinearSequencerContext);
    }

    void  ZoneObjectManager::ZoneDbDeleteMessageHandler( ZoneDbDeleteMessage *pZoneDbDeleteMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::dbDelete),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneDbDeleteMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

		chkFirstTimeTransFailure(pPrismSynchronousLinearSequencerContext);
    }

    void  ZoneObjectManager::ZoneDbDisableMessageHandler( ZoneDbDisableMessage *pZoneDbDisableMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::dbDisable),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::freeAllocatedManagedObjects),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneDbDisableMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  ZoneObjectManager::ZoneDbEnableMessageHandler( ZoneDbEnableMessage *pZoneDbEnableMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::dbEnable),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::freeAllocatedManagedObjects),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneDbEnableMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  ZoneObjectManager::ZoneDbRemoveMessageHandler( ZoneDbRemoveMessage *pZoneDbRemoveMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::dbRemove),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneDbRemoveMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();

		chkFirstTimeTransFailure(pPrismSynchronousLinearSequencerContext);

    }

    void  ZoneObjectManager::ZoneDbSaveMessageHandler( ZoneDbSaveMessage *pZoneDbSaveMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::dbSave),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::freeAllocatedManagedObjects),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneDbSaveMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  ZoneObjectManager::ZoneDbTransAbortMessageHandler( ZoneDbTransAbortMessage *pZoneDbTransAbortMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::dbAbort),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

			/* Wyser Backend Create Support. Create lookup table for zoning objects before doing cleanup */
	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::backendWyserZonePluginCreate),

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::freeAllocatedManagedObjects),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneDbTransAbortMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  ZoneObjectManager::ZoneOperationInfoMessageHandler( ZoneOperationInfoMessage *pZoneOperationInfoMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::getOperationInfo),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

	        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::freeAllocatedManagedObjects),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ZoneObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pZoneOperationInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    typedef struct nzNameHdr {
        u_char  name_len;
        u_char  reserved[3];
    } nzNameHdr_t;

    typedef struct nzZoneObjHdr {
        u_char          type;
        u_char          protocol;
        u_short         reserved;
        nzNameHdr_t     namehdr;
        char            name[1];        /* variable length upto 64 bytes */
    } nzZoneObjHdr_t;

    typedef struct nzMemHdr {
        u_char          type;
        u_char          reserved;
        u_char          flags;
        u_char          infolen;
    } nzMemHdr_t;

    typedef enum nzZoneObjType {
        N_ZONESET       = 0x01,
        N_ZONE          = 0x02,
        N_ZONEALIAS     = 0x03,
        /* vendor unique types */
        N_QLP           = 0xe0,
        N_IPO           = 0xe1
    } nzZoneObjType_e;

    typedef enum nzMemType {
        WWN_TYPE                = 0x01,
        DOMAIN_PORT_TYPE        = 0x02,
        FCID_TYPE               = 0x03,
        NAME_TYPE               = 0x04,
        /* vendor unique types */
        ALPA_TYPE               = 0xe0,
        INITIATOR_TYPE          = 0xe1
    } nzMemType_e;

	typedef enum {
//    NAME = 0,           /* simple "C" style name */
    CFG = 1,            /* configuration entry */
//    ZONE = 2,           /* zone entry */
//    ALIAS = 3,          /* alias entry */
//    QLP = 4,            /* quickloop entry */
//    CFG_END = 5,        /* end marker */
//    IPO = 6,            /* ipozone entry */
    ENABLE_CFG = 7     /* effective config entry */
//    ACTIVE_CFG = 8,     /* active config entry */
//    ACTIVE_CFG_END = 9, /* active config end marker */
//    INVALID_CFG = 0xff  /* invalid config type */
	} zn_ctype_t;

	static inline int zoneSupportCheck() {
        // should return 1 only if we are in
        // MC or FC with distributed service enabled.
        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            return WAVE_MESSAGE_SUCCESS;
            // if we are in FC but not in distributed service
            // is on
            //ZONE_ERROR_UNSUPPORTED_FIRMWARE_IN_FABRIC
        } else {
            return ZONE_ERROR_UNSUPPORTED_MODE;
        }
	}

	static int zone_parse_list(int type, char *name, char *list,
	    vector <string> *cfgName, vector <string> *zoneNameInCfg,
	    vector <string> *zoneName, vector <string> *zoneEntry,
		vector <string> *aliasName, vector <string> *aliasEntry) {
#define ISWZN_MAX_MEMBER_LEN 1000
	    char member[ISWZN_MAX_MEMBER_LEN];
	    char *first_mem = list;
	    char *second_mem;
	    char *last_byte;

	    if (list == NULL) {
	        return (0);
	    }

	    last_byte = first_mem + strlen(first_mem);

	    while (*first_mem != (char)NULL && first_mem < last_byte) {

	        second_mem = first_mem;
	        while (*second_mem != (char)NULL) {
	            /*
	             * find the next ";"
	             */
	            if (*second_mem == ';')
	                break;
	            else
	                second_mem++;
	        }
	        strncpy(member, first_mem, second_mem - first_mem);
	        member[second_mem - first_mem] = (char)NULL;

	        if (type == CFG) {
	            cfgName->push_back(name);
	            zoneNameInCfg->push_back(member);
	        } else if (type == ZONE) {
	            zoneName->push_back(name);
	            zoneEntry->push_back(member);
	        } else if (type == ALIAS) {
	            aliasName->push_back(name);
	            aliasEntry->push_back(member);
	        }

	        if (*second_mem == (char)NULL)
	            first_mem = second_mem;
	        else
	            first_mem = second_mem + 1;
		}

	    return (0);
    }

	static int nzCheckNameFillbytes(char *s, int *len) {
        int fullnlen = *len;
        int i = 0;
        char *p = s + fullnlen - 1;

        if (fullnlen > 64) {
	        tracePrintf (TRACE_LEVEL_INFO, false, false,
	            "fullnlen greater than 64\n");
            return (-1);
        }
        /* if full name length is not a multiple of 4 then return error */
        if ((fullnlen % 4)) {
	        tracePrintf (TRACE_LEVEL_INFO, false, false,
	            "fullnlen not multiple of 4\n");
            return (-1);
        }

        while (*p == '\0') {
            i++;
            p--;
        }

        *len = (fullnlen - i);
        return (i);
    }

	int ZoneObjectManager::zone_db_parse(char *cfg, int size,
	    vector <string> *dCfgName, vector <string> *dZoneNameInCfg,
	    vector <string> *dZoneName, vector <string> *dZoneEntry,
	    vector <string> *eCfgName, vector <string> *eZoneNameInCfg,
	    vector <string> *eZoneName, vector <string> *eZoneEntry,
	    vector <string> *dAliasName, vector <string> *dAliasEntry) {
	    int type;
	    char *p1, *p2;
	    char *end_ptr;
	    //int active_cfg_present = 0;
	    nzNameHdr_t *azname;
	    char *cazname;
	    int     name_len = 0;
	    //u_char fillbytes;
	    int ret_fillbytes;
	    u_int   *pnazobjs;
	    nzZoneObjHdr_t  *azo;
	    unsigned int i;
	    //int mem_num;

	    if (cfg == NULL) {
	        return (-1);
	    }
	    end_ptr = cfg + size;

	    type = *cfg;
	    while (cfg != NULL && cfg <= end_ptr && type != CFG_END) {
	        switch (type) {
	        /*
    	     * for all case, increment the string locations
    	     * but parse only if ZONE
    	     */
    	    case CFG:
    	    case ZONE:
    	    case ALIAS:
    	        p1 = cfg + 1;
    	        if (p1 == NULL || p1 > end_ptr) {
    	            goto done;
    	        }

	            p2 = p1 + strlen(p1) + 1;
	            if (p2 == NULL || p2 > end_ptr) {
	                goto done;
	            }

	            zone_parse_list(type, p1, p2,
	                dCfgName, dZoneNameInCfg, dZoneName, dZoneEntry,
								dAliasName, dAliasEntry);

	            cfg = p2 + strlen(p2) + 1;
	            if (cfg == NULL || cfg > end_ptr) {
	                goto done;
	            }

	            type = *cfg;
	            break;

	        case ENABLE_CFG:
	            p1 = cfg + 1;
	            if (p1 == NULL || p1 > end_ptr) {
	                goto done;
	            }

	            //active_cfg_present = 1;

	            //tracePrintf (TRACE_LEVEL_INFO, false, false,
	             //   "zone_db_parse: enable cfg %s\n", p1);

/*
	            for (i = 0; i < dCfgName->size(); i++) {
	                if (strcmp(p1, dCfgName->at(i).c_str()) == 0) {
	                    eCfgName->push_back(dCfgName->at(i));
	                    eZoneNameInCfg->push_back(dZoneNameInCfg->at(i));
	                }
	            }

	            for (i = 0; i < eZoneNameInCfg->size(); i++) {
	                for (unsigned int j = 0; j < dZoneName->size(); j++) {
	                    if (strcmp(eZoneNameInCfg->at(i).c_str(),
	                        dZoneName->at(j).c_str()) == 0) {
	                        eZoneName->push_back(dZoneName->at(j));
	                        eZoneEntry->push_back(dZoneEntry->at(j));
	                    }
	                }
	            }
*/

	            // need to go to the end of the effective cfg
	            // and account for NULL\n (5 chars)
	            cfg = p1 + strlen(p1) + 1 + 5;
	            if (cfg == NULL || cfg > end_ptr) {
	                goto done;
	            }

	            type = *cfg;

	            break;

    	    default:
	            p1 = cfg + 1;
	            if (p1 == NULL || p1 > end_ptr) {
	                goto done;
	            }

	            p2 = p1 + strlen(p1) + 1;
	            if (p2 == NULL || p2 > end_ptr) {
	                goto done;
    	        }

    	        cfg = p2 + strlen(p2) + 1;
    	        if (cfg == NULL || cfg > end_ptr) {
    	            goto done;
    	        }
    
    	        type = *cfg;
    	        break;
	        }
	    }

	done:

	    if (type == CFG_END) {
	        // it looks like I parsed everthing in defined config
	        // let's see if we could parse effective
	        if (cfg + 9 <= end_ptr &&
	            *((uint32_t *)(cfg + 1)) == NZ_ACTIVE_CFG_SIGN &&
	            *(cfg + 5) == ACTIVE_CFG) {
	            int effectiveSize = *((int *)(cfg+6));
	            //tracePrintf (TRACE_LEVEL_INFO, false, false,
	            //    "zone_db_parse: found effective cfg sz %d\n", effectiveSize);

	            if (effectiveSize > 0) {
	                azname = (nzNameHdr_t *)(cfg+10);
	                cazname = (char *)azname + sizeof (nzNameHdr_t);

	                name_len = azname->name_len;
	                eswap(cazname, name_len);
	                if ((ret_fillbytes =
	                    nzCheckNameFillbytes(cazname, &name_len)) < 0) {
	                    return (-1);
	                }
	                //fillbytes = (u_char)ret_fillbytes;

	                //tracePrintf (TRACE_LEVEL_INFO, false, false,
	                //    "zone_db_parse: effective cfg name %s\n", cazname);

	                pnazobjs = (u_int *)((char *)azname + sizeof (nzNameHdr_t) +
	                    azname->name_len);

	                azo = (nzZoneObjHdr_t *)((char *)pnazobjs + sizeof (int));

	                for (i = 0; i < *pnazobjs; i++) {
	                    u_int 	*pnzomems;
	                    nzMemHdr_t *zmemo;
	                    //int 	zotype = -1;
	                    unsigned int j = 0;

	                    name_len = azo->namehdr.name_len;
	                    eswap(azo->name, name_len);

	                    eCfgName->push_back(string(cazname));
                        eZoneNameInCfg->push_back(string(azo->name));

	                    if ((ret_fillbytes = nzCheckNameFillbytes(
	                        azo->name, &name_len)) < 0) {
	                        return (-1);
	                    }
	                    //fillbytes = (u_char)ret_fillbytes;
	                    pnzomems = (u_int *) (azo->name +
	                        azo->namehdr.name_len);

	                    if (*pnzomems == 0) {
	                        return (-1);
	                    }
	                    switch (azo->type) {
	                        case N_ZONE:
	                            //zotype = ZONE;
	                            break;
	                        default:
	                            return (-1);
	                   }

	                   zmemo = (nzMemHdr_t *)((char *)pnzomems +
	                       sizeof (int));

	                   //mem_num = 0;
	                   for (j = 0; j < *pnzomems; j++) {
	                       if (zmemo->type == WWN_TYPE && zmemo->infolen == 8) {
	                           wwn_t *wwn = (wwn_t *)((char *)zmemo +
	                               sizeof (nzMemHdr_t));
	                           eZoneName->push_back(string(azo->name));
	                           eZoneEntry->push_back(string(wwnfmt(wwn)));
	        	           }
	                      zmemo = (nzMemHdr_t *)((char *)zmemo +
	                          sizeof (nzMemHdr_t) + zmemo->infolen);
	                    }

	                    azo = (nzZoneObjHdr_t *)zmemo;
	                }
	            }
	        }
	    }

	return (0);
    }

	int ZoneObjectManager::removeDBAttributesManagedObject() {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext(
	        DBAttributesManagedObject::getClassName ());

	    // get all the entries
	    vector<WaveManagedObject *> *pResults =
	        querySynchronously (&synchronousQueryContext);

	    if (NULL != pResults) {
	        UI32 i;
	        for (i = 0; i < pResults->size(); i++) {
	            DBAttributesManagedObject *pDBAttributesManagedObject =
                    dynamic_cast<DBAttributesManagedObject *> ((*pResults)[i]);

	            trace (TRACE_LEVEL_INFO, string ("deleting db ") +
	                pDBAttributesManagedObject->getEffectiveCfgName() +
	                string (" from zone plugin db"));

	            delete pDBAttributesManagedObject;
            }

	        pResults->clear();
	        delete pResults;
        }

	    return (0);
    }

	int ZoneObjectManager::removeZoneMemberManagedObject() {
	    deleteAllManagedObjectInstances(
	        ZoneMemberManagedObject::getClassName());
	    trace (TRACE_LEVEL_INFO, string ("ZoneObj:deleting all zone Member MOs"));
	    return (0);
    }

	int ZoneObjectManager::removeZoneManagedObject() {
	    trace (TRACE_LEVEL_INFO, string ("ZoneObj:deleting all zone MOs"));
	    deleteAllManagedObjectInstances(
	        ZoneManagedObject::getClassName());

		trace (TRACE_LEVEL_INFO, string ("ZoneObj:deleting all Wyser zone MO ObjIds"));
		WaveSystemManagementGatewayWorker::deleteObjectIdsForBaseTargetNodeName("zoning defined-configuration zone");
	    return (0);
    }

	int ZoneObjectManager::removeCfgMemberManagedObject() {
	    deleteAllManagedObjectInstances(
	        CfgMemberManagedObject::getClassName());
	    trace (TRACE_LEVEL_INFO, string ("ZoneObj:deleting all cfg member MOs"));
	    return (0);
    }

	int ZoneObjectManager::removeCfgManagedObject() {
	    trace (TRACE_LEVEL_INFO, string ("ZoneObj:deleting all cfg MOs"));
	    deleteAllManagedObjectInstances(
	        CfgManagedObject::getClassName());

		trace (TRACE_LEVEL_INFO, string ("ZoneObj:deleting all Wyser cfg MO ObjIds"));
		WaveSystemManagementGatewayWorker::deleteObjectIdsForBaseTargetNodeName("zoning defined-configuration cfg");
	    return (0);
    }

	int ZoneObjectManager::removeAliasMemberManagedObject() {
	    deleteAllManagedObjectInstances(
	        AliasMemberManagedObject::getClassName());
	    trace (TRACE_LEVEL_INFO, string ("ZoneObj:deleting all alias Member MOs"));
	    return (0);
    }

	int ZoneObjectManager::removeAliasManagedObject() {
	    trace (TRACE_LEVEL_INFO, string ("ZoneObj:deleting all alias MOs"));
	    deleteAllManagedObjectInstances(
	        AliasManagedObject::getClassName());

	    trace (TRACE_LEVEL_INFO, string ("ZoneObj:deleting all Wyser alias MO ObjIds"));
		WaveSystemManagementGatewayWorker::deleteObjectIdsForBaseTargetNodeName("zoning defined-configuration alias");
	    return (0);
    }

	int ZoneObjectManager::dumpDcmdZoneDB()
	{
	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObj:starting dumpDcmdZoneDB\n");

	    // print cfg entries
	    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext(
	        CfgManagedObject::getClassName ());

	    // get all the cfg entries
	    vector<WaveManagedObject *> *pResults =
	        querySynchronously (&synchronousQueryContext);

	    if (NULL != pResults) {
	        UI32 i;
	        for (i = 0; i < pResults->size(); i++) {
	            CfgManagedObject *pCfgManagedObject =
                    dynamic_cast<CfgManagedObject *> ((*pResults)[i]);

	            trace (TRACE_LEVEL_INFO, string ("cfg entry - ") +
	                pCfgManagedObject->getCfgName() +
	                string (" - present in plugin db"));
            }

	        pResults->clear();
	        delete pResults;
        }
	    }

	    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext(
	        ZoneManagedObject::getClassName ());

	    // get all the zone entries
	    vector<WaveManagedObject *> *pResults =
	        querySynchronously (&synchronousQueryContext);

	    if (NULL != pResults) {
	        UI32 i;
	        for (i = 0; i < pResults->size(); i++) {
	            ZoneManagedObject *pZoneManagedObject =
                    dynamic_cast<ZoneManagedObject *> ((*pResults)[i]);

	            trace (TRACE_LEVEL_INFO, string ("zone - ") +
	                pZoneManagedObject->getZoneName() +
	                string (" - present in plugin db"));
            }

	        pResults->clear();
	        delete pResults;
        }
	    }

		{
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext(
	        AliasManagedObject::getClassName ());

	    // get all the alias entries
	    vector<WaveManagedObject *> *pResults =
	        querySynchronously (&synchronousQueryContext);

	    if (NULL != pResults) {
	        UI32 i;
	        for (i = 0; i < pResults->size(); i++) {
	            AliasManagedObject *pAliasManagedObject =
                    dynamic_cast<AliasManagedObject *> ((*pResults)[i]);

	            trace (TRACE_LEVEL_INFO, string ("alias - ") +
	                pAliasManagedObject->getAliasName() +
	                string (" - present in plugin db"));
            }

	        pResults->clear();
	        delete pResults;
        }
	    }

	    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext(
	        DBAttributesManagedObject::getClassName ());

	    // get all the pDBAttribute entries
	    vector<WaveManagedObject *> *pResults =
	        querySynchronously (&synchronousQueryContext);

	    if (NULL != pResults) {
	        UI32 i;
	        for (i = 0; i < pResults->size(); i++) {
	            DBAttributesManagedObject *pDBAttributesManagedObject =
                    dynamic_cast<DBAttributesManagedObject *> ((*pResults)[i]);

	            trace (TRACE_LEVEL_INFO, string ("db - ") +
	                pDBAttributesManagedObject->getEffectiveCfgName() +
	                string (" present in plugin db"));
            }

	        pResults->clear();
	        delete pResults;
        }
	    }

	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObj:done dumpDcmdZoneDB\n");

	    return (0);
	}

	int ZoneObjectManager::clearDcmdZoneDB()
	{
	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObj:starting clearDcmdZoneDB\n");

	    removeDBAttributesManagedObject();
	    removeZoneMemberManagedObject();
	    removeZoneManagedObject();
	    removeCfgMemberManagedObject();
	    removeCfgManagedObject();
		removeAliasMemberManagedObject();
	    removeAliasManagedObject();

	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObj:done clearDcmdZoneDB\n");

	    return (0);
	}

    int ZoneObjectManager::overwriteDcmdFromZoned (
	    int setCfgActionNone,
	    UI32 bootUpType,
		UI32 op_type)
    {
	    realSwitchInstall_optimized(setCfgActionNone, bootUpType, op_type);
	    //simulatorInstall();
	    return (WAVE_MESSAGE_SUCCESS);
	}

	void ZoneObjectManager::realSwitchInstall_optimized(
		int setCfgActionNone,
	    UI32 bootUpType, UI32 op_type)
    {
	    vector<string> dCfgName;
	    vector<string> dZoneNameInCfg;
	    vector<string> dZoneName;
	    vector<string> dZoneEntry;
	    vector<string> eCfgName;
	    vector<string> eZoneNameInCfg;
	    vector<string> eZoneName;
	    vector<string> eZoneEntry;
		vector<string> dAliasName;
		vector<string> dAliasEntry;

		unsigned int zone_db_max;
	    unsigned int zone_db_avail;
	    unsigned int zone_committed;
	    unsigned int zone_transaction;
		unsigned int adjustedMemberMOIndex = 0;

		int token;
		int abortable;
	    int rc = 0;
		int reload_defined_database = FALSE;
		int reload_effective_database = FALSE;
		int reload_effective_cfg_marker = FALSE;
		int isEffAndDefConsistent_flag = FALSE;
		size_t found;

		char *zone_buf = NULL;
		int max_zn_sz = MAX_ZONE_CFG_SZ;
	    int real_size;

		char	lastZoneChangedTempStr[ZN_TIMESTAMPSTR_SZ] = {0};
		string	lastZoneChangedTimestampStr;

		/* Execute */
	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObj:reload_optimized: start of reload\n");

		/* Let's reload the DBAttributes first since this always needs to be updated */

		// set the action entry to none all the time for display
	    // set to the entry triggers actions but the entry
	    // remains as none
		DBAttributesManagedObject *pDBAttributesManagedObject =
	                queryDBAttributesManagedObject(TRUE);

		/* If pDBAttributesManagedObject exists, allow updates to it via updateWaveManagedObject() */
		if (pDBAttributesManagedObject) {
			updateWaveManagedObject(pDBAttributesManagedObject);
		} else {
			/* No PDAttributes MO exists, we need to create it */
			pDBAttributesManagedObject = new DBAttributesManagedObject(this);
		}

		/* Add MO Pointer to Free List that gets cleaned up after Commit Step */
	    addDBAttributesManagedObjectToFree(pDBAttributesManagedObject);

		/* Init DBAttributesMO */
	    pDBAttributesManagedObject->setBootUpType(bootUpType);

	    if (setCfgActionNone) {
	        pDBAttributesManagedObject->setEffectiveCfgAction(
	            brocade_zone_cfg_none);
	    } else {
	        pDBAttributesManagedObject->setEffectiveCfgAction(
	            brocade_zone_cfg_save);
	    }
	    
	    cfgTransShow(&token, &abortable);
	    pDBAttributesManagedObject->setDbTransToken(token);

	    pDBAttributesManagedObject->setDefaultZoneAccess(
	        isDefzoneNoAccessSet() ?
	        brocade_zone_noaccess :
	        brocade_zone_allaccess);

	    cfgSize_telnet_no_prompt((int *)&zone_db_max, (int *)&zone_db_avail,
	        (int *)&zone_committed, (int *)&zone_transaction);

		pDBAttributesManagedObject->setDbMaxSize(zone_db_max);
		pDBAttributesManagedObject->setDbAvailableSize(zone_db_avail);
		pDBAttributesManagedObject->setDbCommittedSize(zone_committed);
		pDBAttributesManagedObject->setDbTransactionSize(zone_transaction);

		/* Update lastZoneChangedTimestamp */
		rc = znUpdateLastZoneChangedTimestamp(token);
		if (rc != 0) {
			tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObj: ERR! znUpdateLastZoneChangedTimestamp rc %d\n", rc);
		}

		/* Get/Set Time Zone info */
        rc = znGetLastZoneChangedTimestamp(lastZoneChangedTempStr);
        if (rc != 0) {
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: reload_optimized: ERR! znGetLastZoneChangedTimestamp rc %d\n", rc);
		}

        lastZoneChangedTimestampStr = lastZoneChangedTempStr;
        pDBAttributesManagedObject->setLastZoneChangedTimestamp(lastZoneChangedTimestampStr);

		string effectiveCfgName =
		pDBAttributesManagedObject->getEffectiveCfgName();

		// end DBAttributes

		/* Decode Op_Type */
		switch (op_type) {
		
		case defzone_mode_change_op:
			/** If we're doing a defzone_mode_change_op operation, check
			 *  if we need to append cfg marker and then exit */
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: defzone_mode_change_op operation, "
						 "make sure CfgMarker is set, and exit\n");
			if (!effectiveCfgName.empty() &&
    	            ((found = effectiveCfgName.find(string(CFG_NAME_MARKER)))
    	            == string::npos) &&
				 isDefzonePendingTrans()) {
					tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: something pending in the def zone change,"
						 " appending cfg marker\n");
    	            effectiveCfgName.append(CFG_NAME_MARKER);
    	            pDBAttributesManagedObject->setEffectiveCfgName(effectiveCfgName);
    	    }

			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: completed reload\n");
			return;

		case cfgsave_op:
			/** If we're doing a cfgsave_op operation, check for Eff vs Def
			 *  differences and exit */
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: cfgSave operation\n");

			reload_effective_cfg_marker = TRUE;
			updateLastZoneCommittedTimestamp(pDBAttributesManagedObject);
			break;

		case cfgclear_op:
			/** If we're doing a cfg_clear operation, then delete all
			 *  Defined Zone MO's and return */
			removeZoneMemberManagedObject();
			removeZoneManagedObject();
			removeCfgMemberManagedObject();
			removeCfgManagedObject();
			removeAliasMemberManagedObject();
			removeAliasManagedObject();

			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: cfgClear operation, deleted all "
						 "Defined Zone MO's, completed reload\n");
			return;

		case cfgdisable_op:
			/* Set Effective CfgName to be empty */
			updateLastZoneCommittedTimestamp(pDBAttributesManagedObject);
			pDBAttributesManagedObject->setEffectiveCfgName(string(""));

			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: cfgDisable operation, "
						 " set cfgName to empty, completed reload\n");
			return;

		case cfgenable_op:
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: cfgenable_op operation\n");
			/** If we're doing a cfgenable_op operation, set reload flag,
			 *  and fall through. */
			reload_effective_database = TRUE;
			updateLastZoneCommittedTimestamp(pDBAttributesManagedObject);
			break;

		case reloadAll_op:
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: reloadAll_op operation\n");
			/** If we're doing a reloadAll_op operation, set reload flags,
			 *  and fall through. */
			reload_defined_database = TRUE;
			reload_effective_database = TRUE;
			reload_effective_cfg_marker = TRUE;
			break;

		case cfgtransabort_op:
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: cfgtransabort_op operation\n");
			/** If we're doing a cfgtransabort_op operation, set reload
			 *  flags, and fall through. */
			reload_defined_database = TRUE;
			reload_effective_cfg_marker = TRUE;
			break;

		default:
			/* For unknown operations, log and exit */
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: Unknown operation type %d,"
						 " completed reload\n", op_type);
			return;

		}

		/* Malloc ZoneDB buffer if we are reloading Effective or Defined */
		if (reload_effective_database || reload_defined_database) {
			/* Malloc Effective ZoneDB buffer */
			if ((zone_buf = (char *)malloc(max_zn_sz)) == NULL) {
				tracePrintf (TRACE_LEVEL_INFO, false, false,
							 "ZoneObj:reload_optimized: critical: zone_buf malloc"
							 " failed, completed reload\n");
				return;
			}
		}

		/* RELOAD EFFECTIVE CfgName */
		if (reload_effective_database == TRUE) {
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: ***** Reloading EFFECTIVE Cfg\n");

			pDBAttributesManagedObject->setEffectiveCfgName(string(""));

			memset((void *)zone_buf, 0, MAX_ZONE_CFG_SZ);

			/* Get the Effective ZoneDB from FOS */
			real_size = cfgDump(zone_buf, max_zn_sz, ZN_FLAG_EFFECTIVE_CFG);
			zone_db_parse(zone_buf, real_size,
						  &dCfgName, &dZoneNameInCfg, &dZoneName, &dZoneEntry,
						  &eCfgName, &eZoneNameInCfg, &eZoneName, &eZoneEntry,
						  &dAliasName, &dAliasEntry);

			/* Set DCMD EffectiveCfg if FOS Effective exists */
			if (eCfgName.size() &&
				eCfgName.at(0).compare(string(defaultCfgName)) != 0) {

				pDBAttributesManagedObject->
				setEffectiveCfgName(eCfgName.at(0));

				tracePrintf (TRACE_LEVEL_INFO, false, false,
							 "\teffective cfg name %s\n", eCfgName.at(0).c_str());
			}
		}

		/* RELOAD DEFINED Database - CfgMos/ZoneMOs/AliasMOs */
		if (reload_defined_database == TRUE) {
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: ***** Reloading DEFINED DB\n");

			/* First delete MO's before reloading. */
			removeCfgMemberManagedObject();
			removeCfgManagedObject();
			removeZoneMemberManagedObject();
			removeZoneManagedObject();
			removeAliasMemberManagedObject();
			removeAliasManagedObject();

			/* Clear out Zone Buffer and Vector buffers */
			memset((void *)zone_buf, 0, MAX_ZONE_CFG_SZ);
			dCfgName.clear();
			dZoneNameInCfg.clear();
			dZoneName.clear();
			dZoneEntry.clear();
			eCfgName.clear();
			eZoneNameInCfg.clear();
			eZoneName.clear();
			eZoneEntry.clear();
			dAliasName.clear();
			dAliasEntry.clear();

			/** Depending on if there is an open transaction, call the
			 *  appropriate FOS API */
			if (token > 0) {
				/* Call this if there is an open transaction */
				real_size = cfgTransDump(zone_buf, max_zn_sz);
			} else {
				/* Call this if there is NO open transaction */
				real_size = cfgDump(zone_buf, max_zn_sz, 0);
			}

			/* Parse zone_buf into Vectors */
			zone_db_parse(zone_buf, real_size,
							  &dCfgName, &dZoneNameInCfg, &dZoneName, &dZoneEntry,
							  &eCfgName, &eZoneNameInCfg, &eZoneName, &eZoneEntry,
							  &dAliasName, &dAliasEntry);

			/* Processing defined Cfgs */
			string currentCfgName = "%currentCfgName";
			vector<WaveManagedObjectPointer<CfgMemberManagedObject> > cfgmember;
			CfgManagedObject *pCfgManagedObject = NULL;
			CfgMemberManagedObject *pCfgMemberManagedObject = NULL;
			int processed_cfg = 0;

			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "\tstart of defined cfg %d\n", dCfgName.size());

			adjustedMemberMOIndex = 0;

			for (unsigned int i = 0; i < dCfgName.size(); i++) {
				if (dCfgName.at(i).compare(string(defaultCfgName)) == 0)
					continue;

				if (currentCfgName != dCfgName.at(i)) {
					adjustedMemberMOIndex = 0;

					// if I have the new cfg & I have already
					// processed one already, put list of zones collected
					// as cfg members
					if (processed_cfg && pCfgManagedObject) {
						pCfgManagedObject->setCfgMembers(cfgmember);
					}
					currentCfgName = dCfgName.at(i);
					processed_cfg = 1;

					// allocate cfg name 
					pCfgManagedObject = new CfgManagedObject(this);

					/* Add MO Pointer to Free List that gets cleaned up after Commit Step */
					addCfgManagedObjectToFree(pCfgManagedObject);

					pCfgManagedObject->setCfgName(currentCfgName);

					// clear the cfg members
					cfgmember.clear();

					// get the zone name for the cfg & add to member
					// list
					pCfgMemberManagedObject =
					new CfgMemberManagedObject(this);

					pCfgMemberManagedObject->
					setCfgEntry(dZoneNameInCfg.at(i));
					pCfgMemberManagedObject->setListIndex(adjustedMemberMOIndex++);
					cfgmember.push_back(pCfgMemberManagedObject);

				} else {
					// I have started processing cfg already, just
					// put the zone found here to the member list
					pCfgMemberManagedObject =
					new CfgMemberManagedObject(this);

					pCfgMemberManagedObject->
					setCfgEntry(dZoneNameInCfg.at(i));
					pCfgMemberManagedObject->setListIndex(adjustedMemberMOIndex++);
					cfgmember.push_back(pCfgMemberManagedObject);

				}
			}

			// if I have started processing cfg already, just
			// add the member list to the current cfg
			if (processed_cfg) {
				pCfgManagedObject->setCfgMembers(cfgmember);
			}

			/* Processing defined Zones */
			string currentZoneName = "%currentZoneName";
			vector<WaveManagedObjectPointer<ZoneMemberManagedObject> > zonemember;
			ZoneManagedObject *pZoneManagedObject = NULL;
			ZoneMemberManagedObject *pZoneMemberManagedObject = NULL;
			int processed_zone = 0;

			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "\tstart of defined zone %d\n", dZoneName.size());

			for (unsigned int i = 0; i < dZoneName.size(); i++) {
				if (dZoneName.at(i).compare(string(defaultZoneName)) == 0)
					continue;

				if (currentZoneName != dZoneName.at(i)) {
					adjustedMemberMOIndex = 0;

					// if I have the new zone & I have already
					// processed one already, put list of entries collected
					// as zone members
					if (processed_zone && pZoneManagedObject) {
						pZoneManagedObject->setZoneMembers(zonemember);
					}
					currentZoneName = dZoneName.at(i);
					processed_zone = 1;

					// allocate zone name 
					pZoneManagedObject = new ZoneManagedObject(this);

					/* Add MO Pointer to Free List that gets cleaned up after Commit Step */
					addZoneManagedObjectToFree(pZoneManagedObject);

					pZoneManagedObject->setZoneName(currentZoneName);

					// clear the zone members
					zonemember.clear();

					// get the zone entry for the zone & add to member
					// list
					pZoneMemberManagedObject =
					new ZoneMemberManagedObject(this);

					pZoneMemberManagedObject->
					setZoneEntry(dZoneEntry.at(i));
					pZoneMemberManagedObject->setListIndex(adjustedMemberMOIndex++);
					zonemember.push_back(pZoneMemberManagedObject);

				} else {
					// I have started processing cfg already, just
					// put the zone found here to the member list
					pZoneMemberManagedObject =
					new ZoneMemberManagedObject(this);

					pZoneMemberManagedObject->
					setZoneEntry(dZoneEntry.at(i));
					pZoneMemberManagedObject->setListIndex(adjustedMemberMOIndex++);
					zonemember.push_back(pZoneMemberManagedObject);

				}
			}

			// if I have started processing cfg already, just
			// add the member list to the current cfg
			if (processed_zone) {
				pZoneManagedObject->setZoneMembers(zonemember);
			}

			/* Processing defined Aliases */
			string currentAliasName = "%currentAliasName";
			vector<WaveManagedObjectPointer<AliasMemberManagedObject> > aliasmember;
			AliasManagedObject *pAliasManagedObject = NULL;
			AliasMemberManagedObject *pAliasMemberManagedObject = NULL;
			int processed_alias = 0;

			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "\tstart of defined alias %d\n", dAliasName.size());

			for (unsigned int i = 0; i < dAliasName.size(); i++) {
				if (dAliasName.at(i).compare(string(defaultZoneName)) == 0)
					continue;

				if (currentAliasName != dAliasName.at(i)) {
					adjustedMemberMOIndex = 0;

					// if I have the new Alias & I have already
					// processed one already, put list of entries collected
					// as Alias members
					if (processed_alias && pAliasManagedObject) {
						pAliasManagedObject->setAliasMembers(aliasmember);
					}
					currentAliasName = dAliasName.at(i);
					processed_alias = 1;

					// allocate Alias name 
					pAliasManagedObject = new AliasManagedObject(this);

					/* Add MO Pointer to Free List that gets cleaned up after Commit Step */
					addAliasManagedObjectToFree(pAliasManagedObject);

					pAliasManagedObject->setAliasName(currentAliasName);

					// clear the Alias members
					aliasmember.clear();

					// get the Alias entry for the Alias & add to member
					// list
					pAliasMemberManagedObject =
					new AliasMemberManagedObject(this);

					pAliasMemberManagedObject->
					setAliasEntry(dAliasEntry.at(i));
					pAliasMemberManagedObject->setListIndex(adjustedMemberMOIndex++);
					aliasmember.push_back(pAliasMemberManagedObject);

				} else {
					// I have started processing Alias already, just
					// put the Alias found here to the member list
					pAliasMemberManagedObject =
					new AliasMemberManagedObject(this);

					pAliasMemberManagedObject->
					setAliasEntry(dAliasEntry.at(i));
					pAliasMemberManagedObject->setListIndex(adjustedMemberMOIndex++);
					aliasmember.push_back(pAliasMemberManagedObject);

				}
			}

			// if I have started processing cfg already, just
			// add the member list to the current cfg
			if (processed_alias) {
				pAliasManagedObject->setAliasMembers(aliasmember);
			}
		}

		/** Check to see if the enabled cfg has the same zones content
		 *  as defined. If not, we need to set the cfgname to
		 *  something different (append '*' wildcard character) in order
		 *  to allow re-enable to be possible.  DCMD will not allow
		 *  reenable to happen if the cfgname is the same. */
		if (reload_effective_cfg_marker) {
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:reload_optimized: ***** Reloading Effective CFG_NAME_MARKER\n");

			effectiveCfgName = pDBAttributesManagedObject->getEffectiveCfgName();

			if (!effectiveCfgName.empty()) {

				isEffAndDefConsistent_flag = isEffectiveAndDefinedConsistent();

				/* If cfgname doesn't have CFG_NAME_MARKER, but needs to, append MARKER */
				if (((found = effectiveCfgName.find(string(CFG_NAME_MARKER))) == string::npos) &&
					!isEffAndDefConsistent_flag) {
					/** A difference was detected, Append '*' cfg marker to
					 *  effective config name to allow re-enable to be possible */
					effectiveCfgName.append(CFG_NAME_MARKER);
					pDBAttributesManagedObject->setEffectiveCfgName(effectiveCfgName);
					tracePrintf (TRACE_LEVEL_INFO, false, false,
								 "ZoneObj:reload_optimized: Cfg Marker is not set but "
								 "should be...let's set it\n");
				} else if (!((found = effectiveCfgName.find(string(CFG_NAME_MARKER))) == string::npos) &&
					isEffAndDefConsistent_flag) {
					/** No difference detected, but marker is set, so erase '*' cfg
					 *  marker from effective config name. */
					effectiveCfgName.erase(found);
					pDBAttributesManagedObject->setEffectiveCfgName(effectiveCfgName);
					tracePrintf (TRACE_LEVEL_INFO, false, false,
								 "ZoneObj:reload_optimized: Cfg Marker is set but "
								 "shouldn't be...let's clear it\n");
				}
			}
		}

	    if (zone_buf) {
	        free(zone_buf);
	    }

	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObj:reload_optimized: completed reload\n");
	}	


	void ZoneObjectManager::simulatorInstall() {

	    removeDBAttributesManagedObject();
	    removeZoneMemberManagedObject();
	    removeZoneManagedObject();
	    removeCfgMemberManagedObject();
	    removeCfgManagedObject();

	    tracePrintf (TRACE_LEVEL_INFO, false, false, "simulator_install\n");

	    for (int j = 0; j < 3; j++) {
	        CfgManagedObject *pCfgManagedObject =
	            new CfgManagedObject(this);

			/* Add MO Pointer to Free List that gets cleaned up after Commit Step */
	        addCfgManagedObjectToFree(pCfgManagedObject);

	        string cfgname;

	        cfgname.append(string("cfg") + j);
	        pCfgManagedObject->setCfgName(cfgname);

	        vector<WaveManagedObjectPointer<CfgMemberManagedObject> > cfgmember;
 
	        for (int i = 0; i < 5; i++) {
	            CfgMemberManagedObject *pCfgMemberManagedObject =
	                new CfgMemberManagedObject(this);

	            string zonename;
	      
	            if (i != 0) { 
	                zonename.append(string("zone-") + j);
	                zonename.append(string("-") + i); 
	                pCfgMemberManagedObject->setCfgEntry(zonename);
	                cfgmember.push_back(pCfgMemberManagedObject);
	            } else {
	                zonename.append(string("zone-same"));
	                pCfgMemberManagedObject->setCfgEntry(zonename);
	                cfgmember.push_back(pCfgMemberManagedObject);
	            }
	        }

	        pCfgManagedObject->setCfgMembers(cfgmember);
	    }

	    for (int j = 0; j < 3; j++) {
	        ZoneManagedObject *pZoneManagedObject =
	            new ZoneManagedObject(this);

			/* Add MO Pointer to Free List that gets cleaned up after Commit Step */
	        addZoneManagedObjectToFree(pZoneManagedObject);

	        string zonename;

	        zonename.append(string("zone") + j);
	        pZoneManagedObject->setZoneName(zonename);

	        vector<WaveManagedObjectPointer<ZoneMemberManagedObject> > zonemember;
 
	        for (int i = 0; i < 5; i++) {
	            ZoneMemberManagedObject *pZoneMemberManagedObject =
	                new ZoneMemberManagedObject(this);

	            string zoneentry;
	      
	            zoneentry.append(string("11:22:33:44:55:66:77") + i);
	            pZoneMemberManagedObject->setZoneEntry(zoneentry);
	            zonemember.push_back(pZoneMemberManagedObject);
	        }

	        pZoneManagedObject->setZoneMembers(zonemember);
	    }

	    DBAttributesManagedObject *pDBAttributesManagedObject =
	        new DBAttributesManagedObject(this);

		/* Add MO Pointer to Free List that gets cleaned up after Commit Step */
	    addDBAttributesManagedObjectToFree(pDBAttributesManagedObject);

	    pDBAttributesManagedObject->setEffectiveCfgName(string("mycfg"));
	    pDBAttributesManagedObject->setEffectiveCfgAction(
	        brocade_zone_cfg_save);

	    int token;
	    int abortable;
	    cfgTransShow(&token, &abortable);
	    pDBAttributesManagedObject->setDbTransToken(token);

	    pDBAttributesManagedObject->setDefaultZoneAccess(
	        isDefzoneNoAccessSet() ?
	        brocade_zone_noaccess :
	        brocade_zone_allaccess);

	    int zone_db_max;
	    int zone_db_avail;
	    int zone_commited;
	    int zone_transaction;

	    cfgSize_telnet_no_prompt(&zone_db_max, &zone_db_avail,
	        &zone_commited, &zone_transaction);

	    pDBAttributesManagedObject->setDbMaxSize(zone_db_max);
	    pDBAttributesManagedObject->setDbAvailableSize(zone_db_avail);
	    pDBAttributesManagedObject->setDbCommittedSize(zone_commited);
	    pDBAttributesManagedObject->setDbTransactionSize(zone_transaction);
	}

	void ZoneObjectManager::install(
        WaveAsynchronousContextForBootPhases *
        pWaveAsynchronousContextForBootPhases) {
	    ResourceId status = WAVE_MESSAGE_SUCCESS; 
	    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

	    ResourceId zone_status = zoneSupportCheck();

	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObjectManager::install: reload or clear based on zoneSupportCheck [%s] %d [%s],"
			" isStartupConfigLinkedToDefaultConfig() %d\n",
	        (FrameworkToolKit::localizeToSourceCodeEnum (zone_status)).c_str(), WAVE_MESSAGE_SUCCESS,
	        (FrameworkToolKit::localizeToSourceCodeEnum (bootReason)).c_str(),
			DcmToolKit::isStartupConfigFileIsLinkedToDefaultConfig());

	    if (zone_status == WAVE_MESSAGE_SUCCESS) {
			/** If need to support FC->MC and MC->FC transition in the
			 *  future, should include additional check like so:
			 *  if
			 *  ((DcmToolKit::isStartupConfigFileIsLinkedToDefaultConfig()
			 *  == TRUE) && (bootReason !=
			 *  WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT))
			 *  
			 *  Until then, continue to use return code from
			 *  isStartupConfigFileIsLinkedToDefaultConfig() to
			 *  determine if need to clear zoning database on bootup in
			 *  order to support copy-default-toStartup operations. */
	        if (DcmToolKit::isStartupConfigFileIsLinkedToDefaultConfig() == TRUE) {
	            tracePrintf (TRACE_LEVEL_INFO, false, false,
	                "ZoneObjectManager::install: boot after copy default to startup\n");

	            DBAttributesManagedObject *pDBAttributesManagedObject =
	                queryDBAttributesManagedObject(TRUE);

	            startTransaction();
	            // we should not have the object here already
	            // let's just check to be sure. So, that we don't
	            // create multiple entries
	            if (pDBAttributesManagedObject) {
	                updateWaveManagedObject(pDBAttributesManagedObject);
	                tracePrintf (TRACE_LEVEL_INFO, false, false,
	                    "ZoneObjectManager::install: updating object\n");
	            } else {
	                // this would be the right path
	                pDBAttributesManagedObject =
	                    new DBAttributesManagedObject(this);
	                tracePrintf (TRACE_LEVEL_INFO, false, false,
	                    "ZoneObjectManager::install: creating object\n");
	            }
	            pDBAttributesManagedObject->setBootUpType(
	                db_boot_type_clear);

	            tracePrintf (TRACE_LEVEL_INFO, false, false,
	                "ZoneObjectManager::install: setBootUpType to %d\n",
	                pDBAttributesManagedObject->getBootUpType());

	            status = commitTransaction ();
	            if (status == FRAMEWORK_SUCCESS) {
                    status = WAVE_MESSAGE_SUCCESS;
                    trace (TRACE_LEVEL_DEVEL,"Success ");
                } else {
                    trace (TRACE_LEVEL_DEVEL,"Committ Failed ");
                }

	            delete (pDBAttributesManagedObject);

	        } else {
	            startTransaction();

	            overwriteDcmdFromZoned(0, db_boot_type_normal, reloadAll_op);

	            status = commitTransaction ();
	            if (status == FRAMEWORK_SUCCESS) {
                    status = WAVE_MESSAGE_SUCCESS;
                    trace (TRACE_LEVEL_DEVEL,"Success ");
                } else {
                    trace (TRACE_LEVEL_DEVEL,"Committ Failed ");
                }

				/** Wyser Backend Support. Create lookup table for
				 *  FOS-backend-sourced-refreshed zoning objects
				 *  before doing cleanup */
				(void)backendWyserZonePluginCreate();

	            (void)freeAllocatedManagedObjects();
	        }
	    }

	    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
	    pWaveAsynchronousContextForBootPhases->callback ();
	}

	void  ZoneObjectManager::externalStateSynchronization  (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
	{
	    ResourceId status = WAVE_MESSAGE_SUCCESS; 
	    ResourceId service_type = pWaveAsynchronousContextForExternalStateSynchronization->getServiceType(); 

		tracePrintf (TRACE_LEVEL_INFO, false, false,"ZoneObjectManager::externalStateSynchronization "
													"getFssStageNumber %d, svc_type %s\n",
					 pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber(),
					 (FrameworkToolKit::localizeToSourceCodeEnum (service_type)).c_str());

		if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber()) == 1001 && (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW)) {
	        // let's figure out if zoning check is supported or not
	        ResourceId zone_status = zoneSupportCheck();

	        // next, let's figure out if copy default to startup
	        // was initiated ad bootUpType is set to clear
            DBAttributesManagedObject *pDBAttributesManagedObject =
	            queryDBAttributesManagedObject(TRUE);
	        int need_to_clear = 0;

	        if (pDBAttributesManagedObject) {
	            tracePrintf (TRACE_LEVEL_INFO, false, false,
	                "ZoneObjectManager::externalStateSynchronization: found bootUpType of %d\n",
	                pDBAttributesManagedObject->getBootUpType());

	            if (pDBAttributesManagedObject->getBootUpType() ==
	                db_boot_type_clear) {
	                need_to_clear = 1;

	                // since I found this object, let's set it to
	                // normal boot as it should most of the time.
	                // so that we clean out copy default-config to
	                // startup-config bringup path

	                startTransaction();

	                updateWaveManagedObject(pDBAttributesManagedObject);
	                pDBAttributesManagedObject->setBootUpType(
	                    db_boot_type_normal);

					/** Defect 374502: Init the cfg-action and
					 *  default-zone-access fields. */
					pDBAttributesManagedObject->setEffectiveCfgAction(
						brocade_zone_cfg_save);

					pDBAttributesManagedObject->setDefaultZoneAccess(
						isDefzoneNoAccessSet() ?
						brocade_zone_noaccess :
						brocade_zone_allaccess);

	                status = commitTransaction ();
	                if (status == FRAMEWORK_SUCCESS) {
                        status = WAVE_MESSAGE_SUCCESS;
                        trace (TRACE_LEVEL_DEVEL,"Success ");
                    } else {
                        trace (TRACE_LEVEL_DEVEL,"Committ Failed ");
                    }
	            }

	            delete (pDBAttributesManagedObject);
	        }

	        tracePrintf (TRACE_LEVEL_INFO, false, false,
	            "ZoneObjectManager::externalStateSynchronization: startup reload or clear for zone flag %d & clear flag %d\n",
	            zone_status, need_to_clear);

	        // if clear bit is set or zoning is not supported
	        // let's clear out the database. otherwise,
	        // just load up the database from zoned
	        if (need_to_clear ||
	            zone_status != WAVE_MESSAGE_SUCCESS) {
	            int api_rc;
				znIpcStatus_t 	znStatus = {0};
	            api_rc = defZone_All_Access_telnet_no_prompt();
                tracePrintf (TRACE_LEVEL_INFO, false, false,
                    "def zone all access fss %d\n", api_rc);

                api_rc = cfgClear_telnet_no_prompt(&znStatus);
                tracePrintf (TRACE_LEVEL_INFO, false, false,
                    "cfg clear fss %d\n", api_rc);

                api_rc = cfgSave_telnet_no_prompt_no_rcs();
                tracePrintf (TRACE_LEVEL_INFO, false, false,
                    "cfg save fss %d\n", api_rc);

                api_rc = cfgDisable_telnet_no_prompt_no_rcs();
                tracePrintf (TRACE_LEVEL_INFO, false, false,
                    "cfg disable fss %d\n", api_rc);
	        } else {
	            startTransaction();

	            overwriteDcmdFromZoned(0, db_boot_type_normal, reloadAll_op);

	            status = commitTransaction ();
	            if (status == FRAMEWORK_SUCCESS) {
                    status = WAVE_MESSAGE_SUCCESS;
                    trace (TRACE_LEVEL_DEVEL,"Success ");
                } else {
                    trace (TRACE_LEVEL_DEVEL,"Committ Failed ");
                }

				/** Wyser Backend Support. Create lookup table for
				 *  FOS-backend-sourced-refreshed zoning objects
				 *  before doing cleanup */
				(void)backendWyserZonePluginCreate();

	            (void)freeAllocatedManagedObjects();
	        }
		}

		pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForExternalStateSynchronization->callback();
	}

	void ZoneObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases)
	{
		listenForEvent (VcsFabricLocalObjectManager::getPrismServiceId (),
						VCS_NODE_ZONE_CHANGED,
						reinterpret_cast<PrismEventHandler> (&ZoneObjectManager::zoneChangedEventHandler));

		listenForEvent (VcsFabricLocalObjectManager::getPrismServiceId (),
						VCS_NODE_ZONE_ABORTED,
						reinterpret_cast<PrismEventHandler> (&ZoneObjectManager::zoneAbortedEventHandler));

		pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForBootPhases->callback ();
	}

	// handle the SCN event when zone db has changed
	void ZoneObjectManager::zoneChangedEventHandler (const VcsNodeZoneChangedEvent *&pEvent)
	{
	    ResourceId status = WAVE_MESSAGE_SUCCESS; 
	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	                "ZoneObjectManager::zoneChangedEvent: zone changed here\n");

	    ResourceId zone_status = zoneSupportCheck();

		if (zone_status == WAVE_MESSAGE_SUCCESS) {

			if (skipSCNReload) {
				tracePrintf (TRACE_LEVEL_INFO, false, false,
							 "ZoneObjectManager::zoneChangedEvent: skipSCNReload flag is set! skip reload since Changed SCN was CLI-generated"
							 " and clear the flag\n",
							 zone_status);
				skipSCNReload = FALSE;
			} else {
				tracePrintf (TRACE_LEVEL_INFO, false, false,
							 "ZoneObjectManager::zoneChangedEvent: zone change reload or clear based on zoneSupportCheck %d\n",
							 zone_status);

				// zone change might be coming in between install
				// and externalstatesync. If bootuptype says I am
				// I need to make sure that I carry that info
				// across this reload.
				DBAttributesManagedObject *pDBAttributesManagedObject =
				queryDBAttributesManagedObject(TRUE);
				UI32 bootUpType = db_boot_type_normal;

				if (pDBAttributesManagedObject) {
					tracePrintf (TRACE_LEVEL_INFO, false, false,
								 "ZoneObjectManager::zoneChangedEvent: found bootUpType of %d during zone change\n",
								 pDBAttributesManagedObject->getBootUpType());

					if (pDBAttributesManagedObject->getBootUpType()
						== db_boot_type_clear) {
						bootUpType = db_boot_type_clear;
					}
					delete (pDBAttributesManagedObject);
				}

				startTransaction();

				overwriteDcmdFromZoned(0, bootUpType, reloadAll_op);

				status = commitTransaction ();
				if (status == FRAMEWORK_SUCCESS) {
					status = WAVE_MESSAGE_SUCCESS;
					trace (TRACE_LEVEL_DEVEL,"Success ");
				} else {
					trace (TRACE_LEVEL_DEVEL,"Committ Failed ");
				}

				/** Wyser Backend Support. Create lookup table for
				 *  FOS-backend-sourced-refreshed zoning objects
				 *  before doing cleanup */
				(void)backendWyserZonePluginCreate();

				(void)freeAllocatedManagedObjects();
			}
		}

		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
	}

	// handle the SCN event when zone transaction has aborted
	void ZoneObjectManager::zoneAbortedEventHandler (const VcsNodeZoneAbortedEvent *&pEvent)
	{
	    ResourceId status = WAVE_MESSAGE_SUCCESS; 
	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	                "ZoneObjectManager::zoneAbortedEvent: zone trans aborted here\n");

	    ResourceId zone_status = zoneSupportCheck();

		if (zone_status == WAVE_MESSAGE_SUCCESS) {

			if (skipSCNReload) {
				tracePrintf (TRACE_LEVEL_INFO, false, false,
							 "ZoneObjectManager::zoneAbortedEvent: skipZoneChangedSCNReload flag is set! skip reload since abort was CLI-generated"
							 " and clear the flag\n",
							 zone_status);
				skipSCNReload = FALSE;
			} else {
				startTransaction();
				tracePrintf (TRACE_LEVEL_INFO, false, false,
							 "ZoneObjectManager::zoneAbortedEvent: zone trans aborted reload or clear based on zoneSupportCheck %d\n",
							 zone_status);

				overwriteDcmdFromZoned(0, db_boot_type_normal, reloadAll_op);

				status = commitTransaction ();
				if (status == FRAMEWORK_SUCCESS) {
					status = WAVE_MESSAGE_SUCCESS;
					trace (TRACE_LEVEL_DEVEL,"Success ");
				} else {
					trace (TRACE_LEVEL_DEVEL,"Committ Failed ");
				}

				/** Wyser Backend Support. Create lookup table for
				 *  FOS-backend-sourced-refreshed zoning objects
				 *  before doing cleanup */
				(void)backendWyserZonePluginCreate();

				(void)freeAllocatedManagedObjects();
			}
		}

		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
	}

	/* START OF dbCreate_new */
	ResourceId ZoneObjectManager::dbCreate (PrismSynchronousLinearSequencerContext *pZoneDbCreateMessageContext)
    {
	    ZoneDbCreateMessage *pZoneDbCreateMessage = reinterpret_cast<ZoneDbCreateMessage *> (pZoneDbCreateMessageContext->getPPrismMessage ());
	    int api_rc = 0;
		int need_to_create_child_object = TRUE;
		int need_to_create_parent_object = FALSE;
		int trans_token = 0;
	    int abortable;
		int i = 0;
	    int znObj_type = 0;
		int numWWNChars = 0;
		bool endOfString = FALSE;
		UI32 memCount = 0;
		UI32 indexCount = 0;
		size_t found = 0;
		size_t start_loc = 0;
		size_t end_loc = 0;
		string entryName;
		string entryList;
		string znObjStr;
		string className;
		string childClassName;
		string compositionName;
		string targetNodeName;
		string createType;
		ObjectId parentMObjectId = pZoneDbCreateMessage->getParentMoObjectId ();
		ObjectId childMObjectId;
		CfgManagedObject *pCfgManagedObject = NULL;
		ZoneManagedObject *pZoneManagedObject = NULL;
		AliasManagedObject *pAliasManagedObject = NULL;

		CfgMemberManagedObject *pCfgMemberManagedObject = NULL;
		ZoneMemberManagedObject *pZoneMemberManagedObject = NULL;
		AliasMemberManagedObject *pAliasMemberManagedObject = NULL;

		vector<WaveManagedObjectPointer<CfgMemberManagedObject> > cfgmember;
		vector<WaveManagedObjectPointer<ZoneMemberManagedObject> > zonemember;
		vector<WaveManagedObjectPointer<AliasMemberManagedObject> > aliasmember;

        tracePrintf (TRACE_LEVEL_DEBUG, false, false, "dbCreate plugin: db_type %d, entry name (%s), entrylist (%s)\n",
	               pZoneDbCreateMessage->getDbEntryType(),
                   (char *)pZoneDbCreateMessage->getDbEntryName().c_str(),
    	           (char *)pZoneDbCreateMessage->getDbEntryList().c_str());

		/*
		 * First thing we want to do is check if there is an open transaction.  If so,
		 * then we can skip modifying the cfg-action and cfg-name fields. We want to
		 * do this to avoid having to query for the dbAttributes MO for every zoneDB
		 * edit for performance reasons (since query'ing MO's impacts performance)
		 */ 
		cfgTransShow(&trans_token, &abortable);

		/* Decode EntryType */
		switch (pZoneDbCreateMessage->getDbEntryType()) {
			case db_type_cfg:
				znObj_type = ZNCFG;
				znObjStr = "cfg";
				break;
	
			case db_type_zone:
				znObj_type = ZONE;
				znObjStr = "zone";
				break;
	
			case db_type_alias:
				znObj_type = ALIAS;
				znObjStr = "alias";
				break;
	
			default:
				tracePrintf (TRACE_LEVEL_ERROR, false, false, "ZoneObj: dbCreate: ERR1! illegal Type %d\n",
							 pZoneDbCreateMessage->getDbEntryType());
				api_rc = ZN_ERR_INVALID_KEY;
				pZoneDbCreateMessage->setRespCode(api_rc);
				return (zoneRespToStatus(api_rc));
		}

		// WYSER CHANGE --> If the Parent Object is provided from Wyser, don't query for it
		if (ObjectId::NullObjectId != parentMObjectId) {
			tracePrintf (TRACE_LEVEL_DEBUG, false, false, "ZoneObj: dbCreate: Wyser Lookup SUCCESS!\n");
			createType = "wyser";
		} else {
			// Parent does not exist, need to create the parent
			api_rc = 0;

			/* First check to make sure no duplicates exist for that parentMO */
			for (i = ZNCFG; i <= ALIAS; i++) {
				/* Don't want to check against same zone Object Type */
				if (i == znObj_type) {
					continue;
				}

				/* Build 'targetNodeName' to check against */
				switch (i) {
					case ZNCFG:
						targetNodeName = "zoning defined-configuration cfg " + pZoneDbCreateMessage->getDbEntryName();
						break;
	
					case ZONE:
						targetNodeName = "zoning defined-configuration zone " + pZoneDbCreateMessage->getDbEntryName();
						break;
	
					case ALIAS:
						targetNodeName = "zoning defined-configuration alias " + pZoneDbCreateMessage->getDbEntryName();
						break;
	
					default:
						tracePrintf (TRACE_LEVEL_ERROR, false, false, "ZoneObj: dbCreate: ERR2! illegal Type %d\n", i);
						break;
				}

				/** Check if another ParentMO type already exists with the
				 *  same name. If duplicate exists, error out! */
				if (true == WaveSystemManagementGatewayWorker::isTargetNodeNameInObjectIdMap (targetNodeName))
				{
					tracePrintf(TRACE_LEVEL_ERROR, false, false, "ZoneObj: dbCreate: %s - OVERLAP w/%s\n", znObjStr.c_str(), targetNodeName.c_str());
					api_rc = ZN_DUPLICATE_NAME;
				}
			}

			/* If no duplicates, then create Parent MO */
			if (api_rc == 0) {

				// entryList is empty, just create the parent entry but no backend api call
				switch (znObj_type) {
				case ZNCFG:
					/* First check if parentMO already exists as a sanity check */
					if ((pCfgManagedObject = queryCfgManagedObjectByName(pZoneDbCreateMessage->getDbEntryName(), FALSE))) {
						parentMObjectId = pCfgManagedObject->getObjectId();
						tracePrintf (TRACE_LEVEL_DEBUG, false, false,
									 "ZoneObj: dbCreate: cfg (%s) already EXISTS via QUERY!\n",
									 (char *)pZoneDbCreateMessage->getDbEntryName().c_str());
						pZoneDbCreateMessageContext->addManagedObjectForGarbageCollection(pCfgManagedObject);
						createType = "query";
					} else {
						/* If parentMO doesn't exist, create it */
						need_to_create_parent_object = TRUE;
					}
					break;

				case ZONE:
					/* First check if parentMO already exists as a sanity check */
					if ((pZoneManagedObject = queryZoneManagedObjectByName(pZoneDbCreateMessage->getDbEntryName(), FALSE))) {
						parentMObjectId = pZoneManagedObject->getObjectId();
						tracePrintf (TRACE_LEVEL_DEBUG, false, false,
									 "ZoneObj: dbCreate: zone (%s) already EXISTS via QUERY!\n",
									 (char *)pZoneDbCreateMessage->getDbEntryName().c_str());
						pZoneDbCreateMessageContext->addManagedObjectForGarbageCollection(pZoneManagedObject);
						createType = "query";
					} else {
						/* If parentMO doesn't exist, create it */
						need_to_create_parent_object = TRUE;
					}
					break;

				case ALIAS:
					/* First check if parentMO already exists as a sanity check */
					if ((pAliasManagedObject = queryAliasManagedObjectByName(pZoneDbCreateMessage->getDbEntryName(), FALSE))) {
						parentMObjectId = pAliasManagedObject->getObjectId();
						tracePrintf (TRACE_LEVEL_DEBUG, false, false,
									 "ZoneObj: dbCreate: alias (%s) already EXISTS via QUERY!\n",
									 (char *)pZoneDbCreateMessage->getDbEntryName().c_str());
						pZoneDbCreateMessageContext->addManagedObjectForGarbageCollection(pAliasManagedObject);
						createType = "query";
					} else {
						/* If parentMO doesn't exist, create it */
						need_to_create_parent_object = TRUE;
					}
					break;

				default:
					tracePrintf (TRACE_LEVEL_ERROR, false, false,"ZoneObj: dbCreate: ERR3! illegal Type %d\n", znObj_type);
					break;
				}
			}
		}

		/** Query for memCount, which is used to determine whether or
		 *  not to call groupCreate() vs groupAdd() */
		if (ObjectId::NullObjectId != parentMObjectId) {
			memCount = queryManagedObjectForMemberCount(parentMObjectId, znObj_type);
		} else {
			memCount = 0;
			createType = "new";
		}
		indexCount = memCount;

		/** If there aren't any child Member Objects, then call
		 *  groupCreate() backend API for first-time FOS creation. */
		if (memCount == 0) {
			/** Defect 378522: Make sure that the EntryList is populated
			 *  before trying to perform groupCreate(), otherwise the create
			 *  will fail and simultaneous 'copy file running-config'
			 *  scenarios involving multiple nodes could result in incorrect
			 *  zoneDB. */
			if (!pZoneDbCreateMessage->getDbEntryList().empty()) {
				if (need_to_create_parent_object == TRUE) {
					tracePrintf (TRACE_LEVEL_INFO, false, false,"ZoneObj: dbCreate: Dual parent/child create!\n");
				}

				api_rc = groupCreate_telnet(znObj_type,
											(char *)pZoneDbCreateMessage->getDbEntryName().c_str(),
											(char *)pZoneDbCreateMessage->getDbEntryList().c_str());
			} else {
				if (ObjectId::NullObjectId != parentMObjectId) {
					tracePrintf (TRACE_LEVEL_ERROR, false, false, "ZoneObj: dbCreate: create ERR! Parent Exists, memlist empty, skip create! %s (%s) memlist (%s)\n",
								 znObjStr.c_str(),
								 (char *)pZoneDbCreateMessage->getDbEntryName().c_str(),
								 (char *)pZoneDbCreateMessage->getDbEntryList().c_str());
				}
				need_to_create_child_object = FALSE;
			}
		} else {
			api_rc = groupAdd_telnet(znObj_type,
									 (char *)pZoneDbCreateMessage->getDbEntryName().c_str(),
									 (char *)pZoneDbCreateMessage->getDbEntryList().c_str());
		}

		/* Create ParentMO */
		if ((api_rc == 0) && (need_to_create_parent_object == TRUE)) {

			switch (znObj_type) {
				case ZNCFG:
					pCfgManagedObject = new CfgManagedObject(this);
					pCfgManagedObject->setCfgName(pZoneDbCreateMessage->getDbEntryName());
					/** We'll manually cleanup the parentMO in
					 *  freeAllocationManagedObjects() as opposed to using
					 *  garbageCollect().  We do it this way to keep track of
					 *  parentMO-only creation so we can add the parentObjectIds
					 *  to the Wyser lookup table in
					 *  backendWyserZonePluginCreate() */
					addCfgManagedObjectToFree(pCfgManagedObject);
					break;
	
				case ZONE:
					pZoneManagedObject = new ZoneManagedObject(this);
					pZoneManagedObject->setZoneName(pZoneDbCreateMessage->getDbEntryName());
					/** We'll manually cleanup the parentMO in
					 *  freeAllocationManagedObjects() as opposed to using
					 *  garbageCollect().  We do it this way to keep track of
					 *  parentMO-only creation so we can add the parentObjectIds
					 *  to the Wyser lookup table in
					 *  backendWyserZonePluginCreate() */
					addZoneManagedObjectToFree(pZoneManagedObject);
					break;
	
				case ALIAS:
					pAliasManagedObject = new AliasManagedObject(this);
					pAliasManagedObject->setAliasName(pZoneDbCreateMessage->getDbEntryName());
					/** We'll manually cleanup the parentMO in
					 *  freeAllocationManagedObjects() as opposed to using
					 *  garbageCollect().  We do it this way to keep track of
					 *  parentMO-only creation so we can add the parentObjectIds
					 *  to the Wyser lookup table in
					 *  backendWyserZonePluginCreate() */
					addAliasManagedObjectToFree(pAliasManagedObject);
					break;
	
				default:
					tracePrintf (TRACE_LEVEL_ERROR, false, false, "ZoneObj: dbCreate: ERR! illegal Type %d\n", i);
					break;
			}
		}

		/* Create ChildMO(s) */
		if ((api_rc == 0) && (need_to_create_child_object == TRUE)) {

			// Start of Multi-List Create
			entryList = pZoneDbCreateMessage->getDbEntryList();
			end_loc = entryList.size() + 1;

			/* Parse through entryList string and do individual childMO creates for each member */
			while (!endOfString) {

				/* Find first/next occurrence of ';' in entryList string */
				found = entryList.find(";", start_loc);

				/* If there is at least one ';', get entryName this way */
				if (found != string::npos) {
					entryName = entryList.substr(start_loc, found - start_loc);
				} else {
					/** If there is only one entry in the entryList list or we are
					 *  at the last entry, get entryName this way */
					entryName = entryList.substr(start_loc, end_loc - start_loc);
					endOfString = TRUE;
				}

				if (!entryName.empty()) {
					switch (znObj_type) {
					case ZNCFG:
						pCfgMemberManagedObject = new CfgMemberManagedObject(this);
						pCfgMemberManagedObject->setCfgEntry(entryName);
						pCfgMemberManagedObject->setListIndex(++indexCount);

						/* If Wyser cached parentMO exists, then do addToComposition */
						if (ObjectId::NullObjectId != parentMObjectId) {
							childMObjectId = pCfgMemberManagedObject->getObjectId();
							pCfgMemberManagedObject->setOwnerManagedObjectId(parentMObjectId);
							className = CfgManagedObject::getClassName();
							childClassName = CfgMemberManagedObject::getClassName();
							compositionName = "cfgMembers";
							/** Only add childMO to garbageCollect() if parent exists.
							 *  Otherwise if this is a case where the parent AND child
							 *  are created at the same time, the childMO ptr will get
							 *  cleaned up when we do a delete() on the parentMO in
							 *  freeAllocatedManagedObjects(). */
						pZoneDbCreateMessageContext->addManagedObjectForGarbageCollection(pCfgMemberManagedObject);
						} else {
							/* ..Otherwise do manual add using 'setXXXMembers()' */
							cfgmember.push_back(pCfgMemberManagedObject);
						}
						break;

					case ZONE:
						pZoneMemberManagedObject = new ZoneMemberManagedObject(this);

						/* Convert WWN chars to LowerCase */
						numWWNChars = 0;

						/** First check if WWN Format is Valid. If so, then zone member
						 *  entry must be a WWN, so convert chars to lowercase.
						 *  Otherwise DCMD queries might not work due to
						 *  case-sensitivity miscompares. */
						numWWNChars = isWWNFormatValid((char *)entryName.c_str());
						if (numWWNChars > 0) {
							entryName = wwnStrToLower(entryName);
						}

						pZoneMemberManagedObject->setZoneEntry(entryName);
						pZoneMemberManagedObject->setListIndex(++indexCount);

						/* If Wyser cached parentMO exists, then do addToComposition */
						if (ObjectId::NullObjectId != parentMObjectId) {
							childMObjectId = pZoneMemberManagedObject->getObjectId();
							pZoneMemberManagedObject->setOwnerManagedObjectId(parentMObjectId);
							className = ZoneManagedObject::getClassName();
							childClassName = ZoneMemberManagedObject::getClassName();
							compositionName = "zoneMembers";
							/** Only add childMO to garbageCollect() if parent exists.
							 *  Otherwise if this is a case where the parent AND child
							 *  are created at the same time, the childMO ptr will get
							 *  cleaned up when we do a delete() on the parentMO in
							 *  freeAllocatedManagedObjects(). */
						pZoneDbCreateMessageContext->addManagedObjectForGarbageCollection(pZoneMemberManagedObject);
						} else {
							/* ..Otherwise do manual add using 'setXXXMembers()' */
							zonemember.push_back(pZoneMemberManagedObject);
						}
						break;

					case ALIAS:
						pAliasMemberManagedObject = new AliasMemberManagedObject(this);
						pAliasMemberManagedObject->setAliasEntry(wwnStrToLower(entryName)); /* Convert WWN chars to lower case! */
						pAliasMemberManagedObject->setListIndex(++indexCount);

						/* If Wyser cached parentMO exists, then do addToComposition */
						if (ObjectId::NullObjectId != parentMObjectId) {
							childMObjectId = pAliasMemberManagedObject->getObjectId();
							pAliasMemberManagedObject->setOwnerManagedObjectId(parentMObjectId);
							className = AliasManagedObject::getClassName();
							childClassName = AliasMemberManagedObject::getClassName();
							compositionName = "aliasMembers";
							/** Only add childMO to garbageCollect() if parent exists.
							 *  Otherwise if this is a case where the parent AND child
							 *  are created at the same time, the childMO ptr will get
							 *  cleaned up when we do a delete() on the parentMO in
							 *  freeAllocatedManagedObjects(). */
						pZoneDbCreateMessageContext->addManagedObjectForGarbageCollection(pAliasMemberManagedObject);
						} else {
							/* ..Otherwise do manual add using 'setXXXMembers()' */
							aliasmember.push_back(pAliasMemberManagedObject);
						}
						break;

					default:
						tracePrintf (TRACE_LEVEL_ERROR, false, false,"ZoneObj: dbCreate: ERR4! illegal Type %d\n", znObj_type);
						break;

					}

					/* If Wyser cached parentMO exists, then do addToComposition */
					if (ObjectId::NullObjectId != parentMObjectId) {
						addToComposition(className, childClassName,
										 compositionName, parentMObjectId,
										 childMObjectId);
					}

					tracePrintf (TRACE_LEVEL_DEBUG, false, false, "ZoneObj: dbCreate: multi create: %s (%s) entry[%d] (%s)\n",
								 znObjStr.c_str(),
								 (char *)pZoneDbCreateMessage->getDbEntryName().c_str(),
								 memCount, entryName.c_str());
				}

				start_loc = found + 1;
			}

			/* If doing manual add, add the members here */
			if (ObjectId::NullObjectId == parentMObjectId) {
				switch (znObj_type) {
				case ZNCFG:
					pCfgManagedObject->setCfgMembers(cfgmember);
					break;

				case ZONE:
					pZoneManagedObject->setZoneMembers(zonemember);
					break;

				case ALIAS:
					pAliasManagedObject->setAliasMembers(aliasmember);
					break;

				default:
					tracePrintf (TRACE_LEVEL_ERROR, false, false, "ZoneObj: dbCreate: ERR5! illegal Type %d\n", znObj_type);
					break;
				}
			}
			// End of Multi-List Create
		}

		tracePrintf (TRACE_LEVEL_INFO, false, false, "ZoneObj: dbCreate: %s %s %s - rc %d: (%s) (%s)\n",
					 znObjStr.c_str(),
					 createType.c_str(),
					 (memCount == 0) ? "create" : "add",
					 api_rc,
					 (char *)pZoneDbCreateMessage->getDbEntryName().c_str(),
					 (char *)pZoneDbCreateMessage->getDbEntryList().c_str());

		pZoneDbCreateMessage->setRespCode(api_rc);

	    updateTransToken(pZoneDbCreateMessageContext, trans_token);

		/* Need to handle first time transaction failure */
		if ((trans_token == 0) && (api_rc != 0)) {
			firstTimeTransFailure = TRUE;
			tracePrintf (TRACE_LEVEL_ERROR, false, false,"ZoneObj: dbCreate: firstTimeTransFailure = TRUE\n");
		}

	    return (zoneRespToStatus(api_rc));
	}
	/* END OF dbCreate */

	ResourceId ZoneObjectManager::dbRemove (PrismSynchronousLinearSequencerContext *pZoneDbRemoveMessageContext)
    {
	    // removing one entry or deleting depending on
		// how many entries are left
	    ZoneDbRemoveMessage *pZoneDbRemoveMessage = reinterpret_cast<ZoneDbRemoveMessage *> (pZoneDbRemoveMessageContext->getPPrismMessage ());
	    int api_rc = 0;
		int trans_token = 0;
	    int abortable;
		int numWWNChars = 0;

		UI32 memCount = 0;
	    int znObj_type = 0;
		string znObjStr;
		string className;
		string childClassName; 
		string compositionName;
		string entryName;
		size_t found = 0;
		ObjectId childMObjectId = ObjectId::NullObjectId;
		ObjectId parentMObjectId = ObjectId::NullObjectId;
		CfgManagedObject *pCfgManagedObject = NULL;
		ZoneManagedObject *pZoneManagedObject = NULL;
		AliasManagedObject *pAliasManagedObject = NULL;

		CfgMemberManagedObject *pCfgMemberManagedObject = NULL;
		ZoneMemberManagedObject *pZoneMemberManagedObject = NULL;
		AliasMemberManagedObject *pAliasMemberManagedObject = NULL;

		WaveManagedObject *pWaveManagedObject = NULL;

		/* If there is at least one ';', exit immediately. We don't do multi-member remove operations */
		if ((found = pZoneDbRemoveMessage->getDbEntryList().find(string(";")))
				!= string::npos) {
			tracePrintf (TRACE_LEVEL_ERROR, false, false, "ZoneObj: dbRemove: ERR! multi-member (%s)\n",
							 pZoneDbRemoveMessage->getDbEntryList().c_str());
			api_rc = ZN_ERR_MULTI_MEMBER;
			pZoneDbRemoveMessage->setRespCode(api_rc);

			return (zoneRespToStatus(api_rc));
		}

		/*
		 * First thing we want to do is check if there is an open transaction.  If so,
		 * then we can skip modifying the cfg-action and cfg-name fields. We want to
		 * do this to avoid having to query for the dbAttributes MO for every zoneDB
		 * edit for performance reasons (since query'ing MO's impacts performance)
		 */ 
		cfgTransShow(&trans_token, &abortable);

		/* Decode EntryType */
		switch (pZoneDbRemoveMessage->getDbEntryType()) {
			case db_type_cfg:
				znObj_type = ZNCFG;
				znObjStr = "cfg";
				className = CfgManagedObject::getClassName();
				childClassName =	CfgMemberManagedObject::getClassName();
				compositionName = "cfgMembers";
				if ((pCfgManagedObject = queryCfgManagedObjectByName(pZoneDbRemoveMessage->getDbEntryName(), FALSE))) {
					parentMObjectId = pCfgManagedObject->getObjectId ();
					pWaveManagedObject = pCfgManagedObject;
				}
				break;
	
			case db_type_zone:
				znObj_type = ZONE;
				znObjStr = "zone";
				className = ZoneManagedObject::getClassName();
				childClassName =	ZoneMemberManagedObject::getClassName();
				compositionName = "zoneMembers";
				if ((pZoneManagedObject = queryZoneManagedObjectByName(pZoneDbRemoveMessage->getDbEntryName(), FALSE))) {
					parentMObjectId = pZoneManagedObject->getObjectId ();
					pWaveManagedObject = pZoneManagedObject;
				}
				break;
	
			case db_type_alias:
				znObj_type = ALIAS;
				znObjStr = "alias";
				className = AliasManagedObject::getClassName();
				childClassName =	AliasMemberManagedObject::getClassName();
				compositionName = "aliasMembers";
				if ((pAliasManagedObject = queryAliasManagedObjectByName(pZoneDbRemoveMessage->getDbEntryName(), FALSE))) {
					parentMObjectId = pAliasManagedObject->getObjectId ();
					pWaveManagedObject = pAliasManagedObject;
				}
				break;
	
			default:
				tracePrintf (TRACE_LEVEL_ERROR, false, false, "ZoneObj: dbRemove: ERR1! illegal Type %d\n",
							 pZoneDbRemoveMessage->getDbEntryType());
				break;
		}

		/* If Parent Object ID is not NULL, then remove child MOs */
		if (parentMObjectId != ObjectId::NullObjectId) {

			// I have the entry in the database already,
			// let's see if zone also exists. if it does, let's remove
			// that entry. Once everything is gone, remove the object
			if ((api_rc = groupRemove_telnet(znObj_type,
											 (char *)pZoneDbRemoveMessage->getDbEntryName().c_str(),
											 (char *)pZoneDbRemoveMessage->getDbEntryList().c_str())) == 0) {

				/* If more than 1 Member MO exists then just delete the Member MO */
				if ((memCount = queryManagedObjectForMemberCount(parentMObjectId, znObj_type)) > 1) {
					tracePrintf (TRACE_LEVEL_INFO, false, false,
								 "ZoneObj: dbRemove: %s left with %d entries\n",
								 znObjStr.c_str(),
								 memCount - 1);

					switch (znObj_type) {
					case ZNCFG:
						if ((pCfgMemberManagedObject = queryCfgMemberManagedObjectByName(parentMObjectId, pZoneDbRemoveMessage->getDbEntryList()))) {
							childMObjectId = pCfgMemberManagedObject->getObjectId();
							pCfgMemberManagedObject->setOwnerManagedObjectId(parentMObjectId);
							pZoneDbRemoveMessageContext->addManagedObjectForGarbageCollection(pCfgMemberManagedObject);
						}
						break;

					case ZONE:
						/* Convert WWN chars to LowerCase */
						numWWNChars = 0;

						/** First check if WWN Format is Valid. If so, then zone member
						 *  entry must be a WWN, so convert chars to lowercase.
						 *  Otherwise DCMD queries might not work due to
						 *  case-sensitivity miscompares. */
						numWWNChars = isWWNFormatValid((char *)pZoneDbRemoveMessage->getDbEntryList().c_str());
						if (numWWNChars > 0) {
							entryName = wwnStrToLower(pZoneDbRemoveMessage->getDbEntryList());
						}

						if ((pZoneMemberManagedObject = queryZoneMemberManagedObjectByName(parentMObjectId, pZoneDbRemoveMessage->getDbEntryList()))) {
							childMObjectId = pZoneMemberManagedObject->getObjectId();
							pZoneMemberManagedObject->setOwnerManagedObjectId(parentMObjectId);
							pZoneDbRemoveMessageContext->addManagedObjectForGarbageCollection(pZoneMemberManagedObject);
						}
						break;

					case ALIAS:
						if ((pAliasMemberManagedObject = queryAliasMemberManagedObjectByName(parentMObjectId, wwnStrToLower(pZoneDbRemoveMessage->getDbEntryList())))) {
							childMObjectId = pAliasMemberManagedObject->getObjectId();
							pAliasMemberManagedObject->setOwnerManagedObjectId(parentMObjectId);
							pZoneDbRemoveMessageContext->addManagedObjectForGarbageCollection(pAliasMemberManagedObject);
						}
						break;

					default:
						tracePrintf (TRACE_LEVEL_ERROR, false, false,"ZoneObj: dbRemove: ERR2! illegal Type %d\n", znObj_type);
						break;
					}
					
					/* The Member MO exists, so let's delete it */
					if (childMObjectId != ObjectId::NullObjectId) {
						deleteFromComposition(className, childClassName, compositionName, parentMObjectId, childMObjectId);
					} else {
						tracePrintf (TRACE_LEVEL_ERROR, false, false,
									 "ZoneObj: dbRemove: *** ERR! Could not find %smember (%s)\n",
									 znObjStr.c_str(), pZoneDbRemoveMessage->getDbEntryList().c_str());
					}
					
					pZoneDbRemoveMessageContext->addManagedObjectForGarbageCollection(pWaveManagedObject);

				} else {
					tracePrintf (TRACE_LEVEL_INFO, false, false,
								 "ZoneObj: dbRemove: no %s members left, delete ParentMO generic\n", znObjStr.c_str());
					delete (pWaveManagedObject);

					/* When deleting ParentMO, need to remove it from the Wyser Lookup Table */
					string targetNodeName = "zoning defined-configuration " + znObjStr + " " + pZoneDbRemoveMessage->getDbEntryName();
					WaveSystemManagementGatewayWorker::deleteTargetNodeNameObjectIdMap(targetNodeName);
					tracePrintf (TRACE_LEVEL_INFO, false, false,
								 "ZoneObj: dbRemove: Wyser removal - targetNodeName (%s)\n", targetNodeName.c_str());
				}
			} else {
				pZoneDbRemoveMessageContext->addManagedObjectForGarbageCollection(pWaveManagedObject);
			}

			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: dbRemove: %s: rc %d - (%s) (%s)\n",
						 znObjStr.c_str(),
						 api_rc,
						 (char *)pZoneDbRemoveMessage->getDbEntryName().c_str(),
						 (char *)pZoneDbRemoveMessage->getDbEntryList().c_str());

			pZoneDbRemoveMessage->setRespCode(api_rc);
		} else {
			tracePrintf (TRACE_LEVEL_ERROR, false, false,
						 "ZoneObj: dbRemove: %s: NULL ParentMO! - (%s) (%s)\n",
						 znObjStr.c_str(),
						 (char *)pZoneDbRemoveMessage->getDbEntryName().c_str(),
						 (char *)pZoneDbRemoveMessage->getDbEntryList().c_str());
		}

	    updateTransToken(pZoneDbRemoveMessageContext, trans_token);

		/* Need to handle first time transaction failure */
		if ((trans_token == 0) && (api_rc != 0)) {
			firstTimeTransFailure = TRUE;
			tracePrintf (TRACE_LEVEL_ERROR, false, false,"ZoneObj: dbRemove: firstTimeTransFailure = TRUE\n");
		}

	    return (zoneRespToStatus(api_rc));
			//ObjectId aliasMoObjectId = pZoneDbRemoveMessage->getParentMoObjectId ();
	}

    ResourceId ZoneObjectManager::dbDelete (PrismSynchronousLinearSequencerContext *pZoneDbDeleteMessageContext)
    {
	    ZoneDbDeleteMessage *pZoneDbDeleteMessage = reinterpret_cast<ZoneDbDeleteMessage *> (pZoneDbDeleteMessageContext->getPPrismMessage ());
	    int api_rc = 0;
		int trans_token = 0;
	    int abortable;

		/*
		 * First thing we want to do is check if there is an open transaction.  If so,
		 * then we can skip modifying the cfg-action and cfg-name fields. We want to
		 * do this to avoid having to query for the dbAttributes MO for every zoneDB
		 * edit for performance reasons (since query'ing MO's impacts performance)
		 */ 
		cfgTransShow(&trans_token, &abortable);

	    if (pZoneDbDeleteMessage->getDbEntryType() == db_type_cfg) {
	        CfgManagedObject *pCfgManagedObject;

	        if ((pCfgManagedObject = queryCfgManagedObjectByName(
	            pZoneDbDeleteMessage->getDbEntryName(), FALSE))) {
	            // I have the config entry in the database already,
	            // let's see if zone also exists. if it does, let's remove
	            // that entry. Once everything is gone, remove the cfg
	            api_rc = groupDelete_telnet(ZNCFG,
	                (char *)pZoneDbDeleteMessage->getDbEntryName().c_str());

	            tracePrintf (TRACE_LEVEL_INFO, false, false,
                             "ZoneObj: dbDelete: cfg: rc %d - (%s)\n",
                             api_rc,
                             (char *)pZoneDbDeleteMessage->getDbEntryName().c_str());

	            // if the api succeeded or the key is not found
	            // then the object should no longer reside within
	            // dcmd
	            if (api_rc == 0 ||
	                api_rc == ZN_ERR_INVALID_KEY) {
	                delete (pCfgManagedObject);
					/* WYSER Block --start ----------------------------------------------------------- */
					string targetNodeName = "zoning defined-configuration cfg " + pZoneDbDeleteMessage->getDbEntryName();
					WaveSystemManagementGatewayWorker::deleteTargetNodeNameObjectIdMap(targetNodeName);
					trace (TRACE_LEVEL_INFO, "ZoneObj: dbDelete: delete cfg from Wyser in backend now");
					/* WYSER Block --end ----------------------------------------------------------- */
	                api_rc = 0;
	            }

	            pZoneDbDeleteMessage->setRespCode(api_rc);
	        }
	    } else if (pZoneDbDeleteMessage->getDbEntryType()
	        == db_type_zone) {
	        ZoneManagedObject *pZoneManagedObject;

	        if ((pZoneManagedObject = queryZoneManagedObjectByName(
	            pZoneDbDeleteMessage->getDbEntryName(), FALSE))) {
	            // I have the config entry in the database already,
	            // let's see if zone also exists. if it does, let's remove
	            // that entry. Once everything is gone, remove the zone 
	            api_rc = groupDelete_telnet(ZONE,
	                (char *)pZoneDbDeleteMessage->getDbEntryName().c_str());

                tracePrintf (TRACE_LEVEL_INFO, false, false,
                             "ZoneObj: dbDelete: zone: rc %d - (%s)\n",
                             api_rc,
                             (char *)pZoneDbDeleteMessage->getDbEntryName().c_str());

	            // if the api succeeded or the key is not found
	            // then the object should no longer reside within
	            // dcmd
	            if (api_rc == 0 ||
	                api_rc == ZN_ERR_INVALID_KEY) {
	                delete (pZoneManagedObject);
					/* WYSER Block --start ----------------------------------------------------------- */
					string targetNodeName = "zoning defined-configuration zone " + pZoneDbDeleteMessage->getDbEntryName();
					WaveSystemManagementGatewayWorker::deleteTargetNodeNameObjectIdMap(targetNodeName);
					trace (TRACE_LEVEL_INFO, "ZoneObj: dbDelete: delete zone from Wyser in backend now");
					/* WYSER Block --end ----------------------------------------------------------- */
					api_rc = 0;
	            }

	            pZoneDbDeleteMessage->setRespCode(api_rc);
	        }
	    } else if (pZoneDbDeleteMessage->getDbEntryType()
	        == db_type_alias) {
	        AliasManagedObject *pAliasManagedObject;

	        if ((pAliasManagedObject = queryAliasManagedObjectByName(
	            pZoneDbDeleteMessage->getDbEntryName(), FALSE))) {
	            // I have the alias entry in the database already,
	            // let's see if alias member also exists. if it does, let's remove
	            // that entry. Once everything is gone, remove the alias 
	            api_rc = groupDelete_telnet(ALIAS,
	                (char *)pZoneDbDeleteMessage->getDbEntryName().c_str());

                tracePrintf (TRACE_LEVEL_INFO, false, false,
                             "ZoneObj: dbDelete: alias: rc %d - (%s)\n",
                             api_rc,
                             (char *)pZoneDbDeleteMessage->getDbEntryName().c_str());

	            // if the api succeeded or the key is not found
	            // then the object should no longer reside within
	            // dcmd
	            if (api_rc == 0 ||
	                api_rc == ZN_ERR_INVALID_KEY) {
	                delete (pAliasManagedObject);
					/* WYSER Block --start ----------------------------------------------------------- */
					string targetNodeName = "zoning defined-configuration alias " + pZoneDbDeleteMessage->getDbEntryName();
					WaveSystemManagementGatewayWorker::deleteTargetNodeNameObjectIdMap(targetNodeName);
					trace (TRACE_LEVEL_INFO, "ZoneObj: dbDelete: delete alias from Wyser in backend now");
					/* WYSER Block --end ----------------------------------------------------------- */
	                api_rc = 0;
	            }

	            pZoneDbDeleteMessage->setRespCode(api_rc);
	        }
	    }

	    updateTransToken(pZoneDbDeleteMessageContext, trans_token);

		/* Need to handle first time transaction failure */
		if ((trans_token == 0) && (api_rc != 0)) {
			firstTimeTransFailure = TRUE;
			tracePrintf (TRACE_LEVEL_ERROR, false, false,"ZoneObj: dbDelete: firstTimeTransFailure = TRUE\n");
		}

	    return (zoneRespToStatus(api_rc));
	}	

    ResourceId ZoneObjectManager::dbEnable (PrismSynchronousLinearSequencerContext *pZoneDbEnableMessageContext)
    {
	    ZoneDbEnableMessage *pZoneDbEnableMessage = reinterpret_cast<ZoneDbEnableMessage *> (pZoneDbEnableMessageContext->getPPrismMessage ());
	    int api_rc = 0;
		znIpcStatus_t 	znStatus = {0};
		string errmsgstr;

        tracePrintf (TRACE_LEVEL_INFO, false, false,
            "ZoneObj: dbEnable: \"%s\"\n",
            (char *)pZoneDbEnableMessage->getDbEntryName().c_str());

        if (pZoneDbEnableMessage->getDbEntryType() == db_type_cfg) {
	        api_rc = cfgEnable_telnet_no_prompt(
                (char *)pZoneDbEnableMessage->getDbEntryName().c_str(), &znStatus);
            tracePrintf (TRACE_LEVEL_INFO, false, false,
                         "ZoneObj: dbEnable: cfg: rc %d, \"%s\"\n",
                         api_rc,
                         (char *)pZoneDbEnableMessage->getDbEntryName().c_str());

	        pZoneDbEnableMessage->setRespCode(api_rc);

	        // reload only if we succeeded. Otherwise, keep it as is
	        if (api_rc == 0) {
				skipSCNReload = TRUE;
	            overwriteDcmdFromZoned(0, db_boot_type_normal, cfgenable_op);
	        } else {
				/** For now, only send back the znStatus.err_buf field to
				 *  parameterize ResourceId error messages for
				 *  ZN_ERR_EMPTY_ZONE_OBJECT, ZN_ERR_CFG_ALIAS_MEMBER, and
				 *  -(1000 + EAGAIN) errors. For now, just handle these two
				 *  error codes.  If we need to expand which ResourceIds can
				 *  handle error string parameters, we can revisit and look into
				 *  making a generic ResourceId to handle all cfgError() type
				 *  FOS Zoning errors. */
				if (((api_rc == ZN_ERR_EMPTY_ZONE_OBJECT) ||
					 (api_rc == ZN_ERR_CFG_ALIAS_MEMBER) ||
					 (api_rc == -(1000 + EAGAIN)))
					&& (strlen(znStatus.err_buf) > 0)) {
					errmsgstr = znStatus.err_buf;
					tracePrintf (TRACE_LEVEL_INFO, false, false,
                "ZoneObj: dbEnable: cfg enable errstr \"%s\"\n", errmsgstr.c_str());
					pZoneDbEnableMessage->setMessageString(errmsgstr);
				}
			}
        } else if (pZoneDbEnableMessage->getDbEntryType() == db_type_def_zone) {
	        api_rc = defZone_All_Access_telnet_no_prompt();
            tracePrintf (TRACE_LEVEL_INFO, false, false,
                "ZoneObj: dbEnable: def zone all access - rc %d\n", api_rc);

	        // don't set the database entry since it should require
	        // cfg action, which will reload the database

	        pZoneDbEnableMessage->setRespCode(api_rc);

	        overwriteDcmdFromZoned(1, db_boot_type_normal, defzone_mode_change_op);
        }

		return (zoneRespToStatus(api_rc));
	}

    ResourceId ZoneObjectManager::dbDisable (PrismSynchronousLinearSequencerContext *pZoneDbDisableMessageContext)
    {
	    ZoneDbDisableMessage *pZoneDbDisableMessage = reinterpret_cast<ZoneDbDisableMessage *> (pZoneDbDisableMessageContext->getPPrismMessage ());
	    int api_rc = 0;
		znIpcStatus_t 	znStatus = {0};
		string errmsgstr;

        if (pZoneDbDisableMessage->getDbEntryType() == db_type_cfg) {
            api_rc = cfgDisable_telnet_no_prompt(&znStatus);
            tracePrintf (TRACE_LEVEL_INFO, false, false,
                "ZoneObj: dbDisable: cfgdisable - rc %d\n", api_rc);

	        pZoneDbDisableMessage->setRespCode(api_rc);

	        // test code
	        // pZoneDbDisableMessage->setRespCode(ZN_ERR_INVALID_KEY);

	        if (api_rc == 0) {
				skipSCNReload = TRUE;
	            overwriteDcmdFromZoned(0, db_boot_type_normal, cfgdisable_op);
	        } else {
				/** For now, only send back the znStatus.err_buf field to
				 *  parameterize ResourceId error messages for
				 *  ZN_ERR_EMPTY_ZONE_OBJECT and -(1000 + EAGAIN) errors. For
				 *  now, just handle these two error codes.  If we need to
				 *  expand which ResourceIds can handle error string
				 *  parameters, we can revisit and look into making a generic
				 *  ResourceId to handle all cfgError() type FOS Zoning
				 *  errors. */
				if (((api_rc == ZN_ERR_EMPTY_ZONE_OBJECT) || (api_rc == -(1000 + EAGAIN)))
					&& (strlen(znStatus.err_buf) > 0)) {
					errmsgstr = znStatus.err_buf;
					tracePrintf (TRACE_LEVEL_INFO, false, false,
                "ZoneObj: dbDisable: cfgdisable errstr (%s)\n", errmsgstr.c_str());
					pZoneDbDisableMessage->setMessageString(errmsgstr);
				}
			}

        } else if (pZoneDbDisableMessage->getDbEntryType() ==
	        db_type_def_zone) {
	        api_rc = defZone_No_Access_telnet_no_prompt();
            tracePrintf (TRACE_LEVEL_INFO, false, false,
                "ZoneObj: dbDisable: def zone no access - rc %d\n", api_rc);

	        // don't set the database entry since it should require
	        // cfg action, which will reload the database

	        pZoneDbDisableMessage->setRespCode(api_rc);

	        overwriteDcmdFromZoned(1, db_boot_type_normal, defzone_mode_change_op);
        }

        return (zoneRespToStatus(api_rc));

	}

    ResourceId ZoneObjectManager::dbSave (PrismSynchronousLinearSequencerContext *pZoneDbSaveMessageContext)
    {
	    ZoneDbSaveMessage *pZoneDbSaveMessage = reinterpret_cast<ZoneDbSaveMessage *> (pZoneDbSaveMessageContext->getPPrismMessage ());
	    int api_rc = 0;
		znIpcStatus_t 	znStatus = {0};
		string errmsgstr;
        
        if (pZoneDbSaveMessage->getDbEntryType() == db_type_cfg) {
            api_rc = cfgSave_telnet_no_prompt(&znStatus);
            tracePrintf (TRACE_LEVEL_INFO, false, false,
                "ZoneObj: dbSave: cfgsave - rc %d\n", api_rc);

	        pZoneDbSaveMessage->setRespCode(api_rc);

	        // unless the db has been saved properly,
	        // no reason to reload. if reloading for some
	        // reason later, we need to give action to
	        // none.
	        if (api_rc == 0) {
				skipSCNReload = TRUE;
	            overwriteDcmdFromZoned(0, db_boot_type_normal, cfgsave_op);
	        } else {
				/** For now, only send back the znStatus.err_buf field to
				 *  parameterize ResourceId error messages for
				 *  ZN_ERR_EMPTY_ZONE_OBJECT and -(1000 + EAGAIN) errors. For
				 *  now, just handle these two error codes.  If we need to
				 *  expand which ResourceIds can handle error string
				 *  parameters, we can revisit and look into making a generic
				 *  ResourceId to handle all cfgError() type FOS Zoning
				 *  errors. */
				if (((api_rc == ZN_ERR_EMPTY_ZONE_OBJECT) || (api_rc == -(1000 + EAGAIN)))
					&& (strlen(znStatus.err_buf) > 0)) {
					errmsgstr = znStatus.err_buf;
					tracePrintf (TRACE_LEVEL_INFO, false, false,
								 "ZoneObj: dbSave: cfgsave errstr (%s)\n", errmsgstr.c_str());
					pZoneDbSaveMessage->setMessageString(errmsgstr);
				}
			}
        }

        return (zoneRespToStatus(api_rc));

	}

    ResourceId ZoneObjectManager::dbClear (PrismSynchronousLinearSequencerContext *pZoneDbClearMessageContext)
    {
        ZoneDbClearMessage *pZoneDbClearMessage = reinterpret_cast<ZoneDbClearMessage *> (pZoneDbClearMessageContext->getPPrismMessage ());
	    int api_rc = 0;
		int trans_token;
		int abortable;
		znIpcStatus_t 	znStatus = {0};
		string errmsgstr;

		/*
		 * First thing we want to do is check if there is an open transaction.  If so,
		 * then we can skip modifying the cfg-action and cfg-name fields. We want to
		 * do this to avoid having to query for the dbAttributes MO for every zoneDB
		 * edit for performance reasons (since query'ing MO's impacts performance)
		 */ 
		cfgTransShow(&trans_token, &abortable);

        if (pZoneDbClearMessage->getDbEntryType() == db_type_cfg) {
            api_rc = cfgClear_telnet_no_prompt(&znStatus);
            tracePrintf (TRACE_LEVEL_INFO, false, false,
                "ZoneObj: dbClear: cfgclear - rc %d\n", api_rc);

	        pZoneDbClearMessage->setRespCode(api_rc);

			/* Reload if cfg clear operation succeeded. */
	        if (api_rc == 0) {
				overwriteDcmdFromZoned(1, db_boot_type_normal, cfgclear_op);
				updateTransToken(pZoneDbClearMessageContext, trans_token);
	        } else {
				/** For now, only send back the znStatus.err_buf field to
				 *  parameterize ResourceId error messages for -(1000 + EAGAIN)
				 *  errors. If we need to expand which ResourceIds can handle
				 *  error string parameters, we can revisit and look into making
				 *  a generic ResourceId to handle all cfgError() type FOS
				 *  Zoning errors. */
				if ((api_rc == -(1000 + EAGAIN)) && (strlen(znStatus.err_buf) > 0)) {
					errmsgstr = znStatus.err_buf;
					tracePrintf (TRACE_LEVEL_INFO, false, false,
								 "ZoneObj: dbClear: cfgclear errstr (%s)\n", errmsgstr.c_str());
					pZoneDbClearMessage->setMessageString(errmsgstr);
				}

				/* Need to handle first time transaction failure */
				if ((trans_token == 0) && (api_rc != 0)) {
					firstTimeTransFailure = TRUE;
					tracePrintf (TRACE_LEVEL_ERROR, false, false,"ZoneObj: dbClear: firstTimeTransFailure = TRUE\n");
				}
			}

        }

        return (zoneRespToStatus(api_rc));
	}

    ResourceId ZoneObjectManager::dbAbort (PrismSynchronousLinearSequencerContext *pZoneDbTransAbortMessageContext)
    {
        ZoneDbTransAbortMessage *pZoneDbTransAbortMessage = reinterpret_cast<ZoneDbTransAbortMessage *> (pZoneDbTransAbortMessageContext->getPPrismMessage ());
	    int api_rc = 0;
		ResourceId status;

        api_rc = cfgTransAbort_telnet();
	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObj: dbAbort: cfgtransabort - rc %d\n", api_rc);

	    pZoneDbTransAbortMessage->setRespCode(api_rc);

		if (api_rc == 0) {
			skipSCNReload = TRUE;
	    overwriteDcmdFromZoned(0, db_boot_type_normal, cfgtransabort_op);
		}

		if (api_rc == -1) {
			status = ZONE_ERROR_ABORT_FAILED;
		} else {
			status = zoneRespToStatus(api_rc);
		}

        return (status);
	}

    ResourceId ZoneObjectManager::getOperationInfo (PrismSynchronousLinearSequencerContext *pZoneOperationInfoMessageContext)
    {
        ZoneOperationInfoMessage *pZoneOperationInfoMessage = reinterpret_cast<ZoneOperationInfoMessage *> (pZoneOperationInfoMessageContext->getPPrismMessage ());

	    UI32 dbTransToken;
	    UI32 dbMaxSize;
	    UI32 dbAvailableSize;
	    UI32 dbCommittedSize;
	    UI32 dbTransactionSize;
	    int token;
	    int abortable;
	    int zone_db_max;
	    int zone_db_avail;
	    int zone_commited;
	    int zone_transaction;
	    int rc1 = 0, rc2 = 0;
		char	lastZoneChangedTempStr[ZN_TIMESTAMPSTR_SZ] = {0};
		char	lastZoneCommittedTempStr[ZN_TIMESTAMPSTR_SZ] = {0};
		string	lastZoneChangedTimestampStr;
	    string	lastZoneCommittedTimestampStr;

        DBAttributesManagedObject *pDBAttributesManagedObject =
	                queryDBAttributesManagedObject(TRUE);
		if (pDBAttributesManagedObject) {
			updateWaveManagedObject(pDBAttributesManagedObject);
			pZoneOperationInfoMessageContext->addManagedObjectForGarbageCollection(pDBAttributesManagedObject);
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: getOperationInfo: pDBAttributesManagedObject"
						 " exists, add to garbageCollect\n");
		} else {
			/* No PDAttributes MO exists, we need to create it */
			pDBAttributesManagedObject = new DBAttributesManagedObject(this);

			/* Add MO Pointer to Free List that gets cleaned up after Commit Step */
			addDBAttributesManagedObjectToFree(pDBAttributesManagedObject);

			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: getOperationInfo: no pDBAttributesManagedObject"
						 " exists, create new\n");
		}

        cfgTransShow(&token, &abortable);
	    dbTransToken = token;

        cfgSize_telnet_no_prompt(&zone_db_max, &zone_db_avail,
            &zone_commited, &zone_transaction);
	    dbMaxSize = zone_db_max;
	    dbAvailableSize = zone_db_avail;
	    dbCommittedSize = zone_commited;
	    dbTransactionSize = zone_transaction;

		/* Get Time Zone info */
		rc1 = znGetLastZoneChangedTimestamp(lastZoneChangedTempStr);
		rc2 = znGetLastZoneCommittedTimestamp(lastZoneCommittedTempStr);

		if (strcmp(lastZoneChangedTempStr, "") == 0) {
			lastZoneChangedTimestampStr = pDBAttributesManagedObject->getLastZoneCommittedTimestamp();
			pDBAttributesManagedObject->setLastZoneChangedTimestamp(lastZoneChangedTimestampStr);
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: getOperationInfo: lastZoneChangedTempStr"
						 " is empty, must have rebooted, use DCMD committed time (%s)\n",
						 lastZoneChangedTimestampStr.c_str());

		} else {
			lastZoneChangedTimestampStr = lastZoneChangedTempStr;
			pDBAttributesManagedObject->setLastZoneChangedTimestamp(lastZoneChangedTimestampStr);
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: getOperationInfo: lastZoneChangedTempStr"
						 " valid, sync DCMD's copy (%s)\n",
						 lastZoneChangedTimestampStr.c_str());
		}

		if (strcmp(lastZoneCommittedTempStr, "") == 0) {
			lastZoneCommittedTimestampStr = pDBAttributesManagedObject->getLastZoneCommittedTimestamp();
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: getOperationInfo: lastZoneCommittedTimestampStr"
						 " is empty, use DCMD's copy (%s)\n",
						 lastZoneCommittedTimestampStr.c_str());
		} else {
			lastZoneCommittedTimestampStr = lastZoneCommittedTempStr;
			pDBAttributesManagedObject->setLastZoneCommittedTimestamp(lastZoneCommittedTimestampStr);
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: getOperationInfo: lastZoneCommittedTimestampStr"
						 " valid, sync DCMD's copy (%s)\n",
						 lastZoneCommittedTimestampStr.c_str());
		}

		/* Log if API returned an error */
		if ((rc1 != 0) || (rc2 != 0)) {
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: getOperationInfo: lastChangedTime (%s), rc1 %d,"
						 " lastCommittedTime (%s), rc2 %d\n",
						 lastZoneChangedTimestampStr.c_str(), rc1,
						 lastZoneCommittedTimestampStr.c_str(), rc2);
		}

	    pZoneOperationInfoMessage->setDbTransToken(dbTransToken);
	    pZoneOperationInfoMessage->setDbMaxSize(dbMaxSize);
	    pZoneOperationInfoMessage->setDbAvailableSize(dbAvailableSize);
	    pZoneOperationInfoMessage->setDbCommittedSize(dbCommittedSize);
	    pZoneOperationInfoMessage->setDbTransactionSize(dbTransactionSize);
	    pZoneOperationInfoMessage->setLastZoneChangedTimestamp(
	        lastZoneChangedTimestampStr);
		pZoneOperationInfoMessage->setLastZoneCommittedTimestamp(
	        lastZoneCommittedTimestampStr);

        return (WAVE_MESSAGE_SUCCESS);
	}

	int ZoneObjectManager::updateTransToken(PrismSynchronousLinearSequencerContext *pZoneMessageContext, int cached_token) {
        DBAttributesManagedObject *pDBAttributesManagedObject = NULL;
		int token = 0;
	    int rc = 0;
	    int abortable;
        char	lastZoneChangedTempStr[ZN_TIMESTAMPSTR_SZ] = {0};
		string	lastZoneChangedTimestampStr;

		/** If this is a first time zoning transaction, then query
		 *  the DBAttributes MO for updating.  Otherwise we want to
		 *  avoid querying the DCMD database when at all possible
		 *  for performance reasons. */
		if (cached_token == 0) {
			tracePrintf (TRACE_LEVEL_INFO, false, false,
							"ZoneObj: updateTransToken: first_time_trans\n");
			pDBAttributesManagedObject = queryDBAttributesManagedObject(FALSE);
	
			if (pDBAttributesManagedObject) {

				cfgTransShow(&token, &abortable);

				updateWaveManagedObject(pDBAttributesManagedObject);
				pDBAttributesManagedObject->setDbTransToken(token);

				/** If there is an open transaction in eyes of FOS Zoning,
				 *  and there is an effective cfg, then append '*' char to
				 *  cfg name and change cfg-action to 'cfg-none' */
				if (token > 0) {
					tracePrintf (TRACE_LEVEL_INFO, false, false,
								 "ZoneObj: updateTransToken: Token = %d, Append CFG_NAME_MARKER\n",
								 token);

					// if the current cfgname is non-null value,
					// add CFG_NAME_MARKER behind it and set the cfg-action to none
					string currentCfgName;
					size_t found;

					currentCfgName = pDBAttributesManagedObject->getEffectiveCfgName();
					if (!currentCfgName.empty() &&
						(found = currentCfgName.find(string(CFG_NAME_MARKER)))
						== string::npos) {
						currentCfgName.append(CFG_NAME_MARKER);
						pDBAttributesManagedObject->setEffectiveCfgName(currentCfgName);
					}

					pDBAttributesManagedObject->setEffectiveCfgAction(brocade_zone_cfg_none);

					/* Update lastZoneChangedTimestamp */
					rc = znUpdateLastZoneChangedTimestamp(token);
					if (rc != 0) {
						tracePrintf (TRACE_LEVEL_INFO, false, false,
									 "ZoneObj: updateTransToken: ERR! znUpdateLastZoneChangedTimestamp rc %d\n", rc);
					}

                    /* Get/Set Time Zone info */
                    rc = znGetLastZoneChangedTimestamp(lastZoneChangedTempStr);
                    if (rc != 0) {
						tracePrintf (TRACE_LEVEL_INFO, false, false,
									 "ZoneObj: updateTransToken: ERR! znGetLastZoneChangedTimestamp rc %d\n", rc);
					}

                    lastZoneChangedTimestampStr = lastZoneChangedTempStr;
                    tracePrintf (TRACE_LEVEL_INFO, false, false,
									 "ZoneObj: updateTransToken: znGetLastZoneChangedTimestamp = %s\n",
                        lastZoneChangedTimestampStr.c_str());
                    pDBAttributesManagedObject->setLastZoneChangedTimestamp(lastZoneChangedTimestampStr);

				} else {
					trace (TRACE_LEVEL_INFO, "updateTransToken: Token = 0, do not append CFG_NAME_MARKER");
				}

				if (firstTimeTransFailure) {
					tracePrintf (TRACE_LEVEL_INFO, false, false,
							"ZoneObj: updateTransToken: firstTimeTransFailure!\n");
					addDBAttributesManagedObjectToFree(pDBAttributesManagedObject);
				} else {
					pZoneMessageContext->addManagedObjectForGarbageCollection(pDBAttributesManagedObject);
				}
			} else {
				tracePrintf (TRACE_LEVEL_INFO, false, false,
							 "ZoneObj: updateTransToken: ERR! pDBAttributesManagedObject == NULL!\n");
			}
		} else {
			/** A transaction is already open.  No need to update
			 *  DBAttributes MO...only update theLastZoneChangedTimestamp */
			rc = znUpdateLastZoneChangedTimestamp(cached_token);
			if (rc != 0) {
				tracePrintf (TRACE_LEVEL_INFO, false, false,
							 "ZoneObj: updateTransToken: ERR! znUpdateLastZoneChangedTimestamp rc %d\n", rc);
			}
		}

	    return (0);
	}


	/** 
	 *  chkFirstTimeTransFailure()
	 *  
	 *  This function is used to handle firstTimeTransFailure
	 *  scenarios.  This function is called from certain
	 *  messagehandlers in messages where we employ the
	 *  updateTransToken() caching strategy (dbClear, dbCreate,
	 *  dbRemove, dbDelete). In the case of a
	 *  firstTimeTransactionFailure, if the message handler doesn't
	 *  return WAVE_MESSAGE_SUCCESS, the DCMD commit will not take
	 *  place.  So we call this function on the outside of the
	 *  LinearSequencerContext so the cfg-name and cfg-action fields
	 *  will get updated regardless of failure.  These fields need
	 *  to be in sync with FOS with respect to whether or not a
	 *  zoning transaction is open.  */
	int ZoneObjectManager::chkFirstTimeTransFailure(PrismSynchronousLinearSequencerContext *pZoneMessageContext) {

		if (firstTimeTransFailure) {
			ResourceId status = WAVE_MESSAGE_SUCCESS; 
			startTransaction();
			updateTransToken(pZoneMessageContext, 0);
			firstTimeTransFailure = FALSE;
			status = commitTransaction ();
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj: chkFirstTimeTransFailure: firstTimeTransFailure handling complete\n");
	        if (status == FRAMEWORK_SUCCESS) {
                status = WAVE_MESSAGE_SUCCESS;
                trace (TRACE_LEVEL_DEVEL,"Success ");
            } else {
                trace (TRACE_LEVEL_DEVEL,"Committ Failed ");
            }

			/* Need to free the MO pointer that we queried for in updateTransToken() */
			freeAllocatedManagedObjects();
		}

	    return (0);
	}

	void ZoneObjectManager::updateLastZoneCommittedTimestamp(DBAttributesManagedObject *pDBAttributesManagedObject) {
		char	lastZoneCommittedTempStr[ZN_TIMESTAMPSTR_SZ] = {0};
		string	lastZoneCommittedTimestampStr;
		int rc = 0;

		/* Get/Set Time Zone info */
		if (pDBAttributesManagedObject) {
			rc = znGetLastZoneCommittedTimestamp(lastZoneCommittedTempStr);
			if (rc != 0) {
				tracePrintf (TRACE_LEVEL_INFO, false, false,
							 "ZoneObj: updateLastZoneCommittedTimestamp: ERR! getTime rc %d\n", rc);
			}

			lastZoneCommittedTimestampStr = lastZoneCommittedTempStr;
			pDBAttributesManagedObject->setLastZoneCommittedTimestamp(lastZoneCommittedTimestampStr);
			tracePrintf (TRACE_LEVEL_INFO, false, false,
			"ZoneObj: updateLastZoneCommittedTimestamp: getTime = %s\n",
			lastZoneCommittedTimestampStr.c_str());
		}
	}

	CfgManagedObject *ZoneObjectManager::queryCfgManagedObjectByName(string cfgName, int loadRelationships) {

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (CfgManagedObject::getClassName ());

	    /* Get the object corresponding to this mapped ID */
	    synchronousQueryContext.addAndAttribute (new AttributeString (cfgName, "cfgName"));

		/* For Performance Reasons, want to avoid loading relationships unless we have to */
		if (loadRelationships  == FALSE) {
			synchronousQueryContext.setLoadOneToManyRelationships(false);
			synchronousQueryContext.setLoadCompositions(false);
		}

	    vector<WaveManagedObject *> *pResults =
	        querySynchronously (&synchronousQueryContext);

	    if (NULL != pResults) {
	        UI32 numberOfResults = pResults->size ();

	        if (0 == numberOfResults) {
	            trace (TRACE_LEVEL_INFO, "ZoneObj::queryCfgManagedObjectByName: Cannot find cfg entry");
                pResults->clear ();
                delete pResults;
	            return (NULL);
            } else if (1 == numberOfResults) {
	            CfgManagedObject *pCfgManagedObject =
                    dynamic_cast<CfgManagedObject *> ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return (pCfgManagedObject);
            } else {
                pResults->clear ();
                delete pResults;
                trace (TRACE_LEVEL_FATAL, "ZoneObj::queryCfgManagedObjectByName: There can be only one local node.");
                prismAssert (false, __FILE__, __LINE__);
	            return (NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "ZoneObj::queryCfgManagedObjectByName: System failure");
            prismAssert (false, __FILE__, __LINE__);
	        return (NULL);
        }

	    return (NULL);
    }

	int ZoneObjectManager::queryCfgMemberManagedObjectForMemberCount(ObjectId parentMoObjectId) {

		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (CfgMemberManagedObject::getClassName ());

		/* We only care about Member MO's who are children of the specified Parent MO */
		synchronousQueryContext.addAndAttribute (new AttributeObjectId (parentMoObjectId , "ownerManagedObjectId"));

		/* Avoid Loading 1-many composition relationships for better performance */
		synchronousQueryContext.setLoadOneToManyRelationships(false);

		/* Avoid Loading 1-1 composition relationships for better performance */
		synchronousQueryContext.setLoadCompositions(false);

		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 MOCount = 0;

		status = querySynchronouslyForCount (&synchronousQueryContext, MOCount);

		if (WAVE_MESSAGE_SUCCESS == status) {
				/*tracePrintf (TRACE_LEVEL_INFO, false, false,
	                    "queryCfgMemberManagedObjectForMemberCount: cfgname (%s), MOCount = %d\n",
							 pParentManagedObject->getCfgName().c_str(), MOCount);*/
		} else {
			trace (TRACE_LEVEL_FATAL, "ZoneObj:queryCfgMemberManagedObjectForMemberCount: Query Failed!");
		}

		return (MOCount);
    }

	CfgMemberManagedObject *ZoneObjectManager::queryCfgMemberManagedObjectByName(ObjectId parentMoObjectId /*CfgManagedObject *pParentManagedObject*/, string cfgEntry) {

		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (CfgMemberManagedObject::getClassName ());

		/* Get the object corresponding to this mapped ID */
		synchronousQueryContext.addAndAttribute (new AttributeObjectId (parentMoObjectId /*pParentManagedObject->getObjectId ()*/, "ownerManagedObjectId"));
		synchronousQueryContext.addAndAttribute (new AttributeString (cfgEntry, "cfgEntry"));

		/* Avoid Loading 1-many composition relationships for better performance */
		synchronousQueryContext.setLoadOneToManyRelationships(false);

		/* Avoid Loading 1-1 composition relationships for better performance */
		synchronousQueryContext.setLoadCompositions(false);

		vector<WaveManagedObject *> *pResults =
		querySynchronously (&synchronousQueryContext);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();

			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_INFO, "ZoneObj:queryCfgMemberManagedObjectByName: Cannot find defined cfg member entry");
				pResults->clear ();
				delete pResults;
				return(NULL);
			} else if (1 == numberOfResults) {
				CfgMemberManagedObject *pCfgMemberManagedObject =
				dynamic_cast<CfgMemberManagedObject *> ((*pResults)[0]);

				pResults->clear ();
				delete pResults;
				return(pCfgMemberManagedObject);
			} else {
				pResults->clear ();
				delete pResults;
				trace (TRACE_LEVEL_FATAL, "ZoneObj:queryCfgMemberManagedObjectByName: There can be only one local node.");
				prismAssert (false, __FILE__, __LINE__);
				return(NULL);
			}
		} else {
			trace (TRACE_LEVEL_FATAL, "ZoneObj:queryCfgMemberManagedObjectByName: System failure");
			prismAssert (false, __FILE__, __LINE__);
			return(NULL);
		}

		return(NULL);
	}

	ZoneManagedObject *ZoneObjectManager::queryZoneManagedObjectByName(string zoneName, int loadRelationships) {

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (ZoneManagedObject::getClassName ());

	    /* Get the object corresponding to this mapped ID */
	    synchronousQueryContext.addAndAttribute (new AttributeString (zoneName, "zoneName"));

		/* For Performance Reasons, want to avoid loading relationships unless we have to */
		if (loadRelationships  == FALSE) {
			synchronousQueryContext.setLoadOneToManyRelationships(false);
			synchronousQueryContext.setLoadCompositions(false);
		}

	    vector<WaveManagedObject *> *pResults =
	        querySynchronously (&synchronousQueryContext);

	    if (NULL != pResults) {
	        UI32 numberOfResults = pResults->size ();

	        if (0 == numberOfResults) {
	            trace (TRACE_LEVEL_INFO, "ZoneObjectManager::queryZoneManagedObjectByName: Cannot find zone entry");
                pResults->clear ();
                delete pResults;
	            return (NULL);
            } else if (1 == numberOfResults) {
	            ZoneManagedObject *pZoneManagedObject =
                    dynamic_cast<ZoneManagedObject *> ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return (pZoneManagedObject);
            } else {
                pResults->clear ();
                delete pResults;
                trace (TRACE_LEVEL_FATAL, "ZoneObjectManager::queryZoneManagedObjectByName: There can be only one local node.");
                prismAssert (false, __FILE__, __LINE__);
	            return (NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "ZoneObjectManager::queryZoneManagedObjectByName: System failure");
            prismAssert (false, __FILE__, __LINE__);
	        return (NULL);
        }

	    return (NULL);
    }

	int ZoneObjectManager::queryZoneMemberManagedObjectForMemberCount(ObjectId parentManagedObjectId) {

		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (ZoneMemberManagedObject::getClassName ());

		/* We only care about Member MO's who are children of the specified Parent MO */
		synchronousQueryContext.addAndAttribute (new AttributeObjectId (parentManagedObjectId, "ownerManagedObjectId"));

		/* Avoid Loading 1-many composition relationships for better performance */
		synchronousQueryContext.setLoadOneToManyRelationships(false);

		/* Avoid Loading 1-1 composition relationships for better performance */
		synchronousQueryContext.setLoadCompositions(false);

		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 MOCount = 0;

		status = querySynchronouslyForCount (&synchronousQueryContext, MOCount);

		if (WAVE_MESSAGE_SUCCESS == status) {
				/*tracePrintf (TRACE_LEVEL_INFO, false, false,
	                    "queryZoneMemberManagedObjectForMemberCount: zonename (%s), MOCount = %d\n",
							 pParentManagedObject->getZoneName().c_str(), MOCount);*/
		} else {
			trace (TRACE_LEVEL_FATAL, "ZoneObj:queryZoneMemberManagedObjectForMemberCount: Query Failed!");
		}

		return (MOCount);
    }

	ZoneMemberManagedObject *ZoneObjectManager::queryZoneMemberManagedObjectByName(ObjectId parentMoObjectId /*ZoneManagedObject *pParentManagedObject*/, string zoneEntry) {

		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (ZoneMemberManagedObject::getClassName ());

		/* Get the object corresponding to this mapped ID */
		synchronousQueryContext.addAndAttribute (new AttributeObjectId (parentMoObjectId /*pParentManagedObject->getObjectId ()*/, "ownerManagedObjectId"));
		synchronousQueryContext.addAndAttribute (new AttributeString (zoneEntry, "zoneEntry"));

		/* Avoid Loading 1-many composition relationships for better performance */
		synchronousQueryContext.setLoadOneToManyRelationships(false);

		/* Avoid Loading 1-1 composition relationships for better performance */
		synchronousQueryContext.setLoadCompositions(false);

		vector<WaveManagedObject *> *pResults =
		querySynchronously (&synchronousQueryContext);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();

			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_INFO, "ZoneObj:queryZoneMemberManagedObjectByName: Cannot find defined zone member entry");
				pResults->clear ();
				delete pResults;
				return(NULL);
			} else if (1 == numberOfResults) {
				ZoneMemberManagedObject *pZoneMemberManagedObject =
				dynamic_cast<ZoneMemberManagedObject *> ((*pResults)[0]);

				pResults->clear ();
				delete pResults;
				return(pZoneMemberManagedObject);
			} else {
				pResults->clear ();
				delete pResults;
				trace (TRACE_LEVEL_FATAL, "ZoneObj:queryZoneMemberManagedObjectByName: There can be only one local node.");
				prismAssert (false, __FILE__, __LINE__);
				return(NULL);
			}
		} else {
			trace (TRACE_LEVEL_FATAL, "ZoneObj:queryZoneMemberManagedObjectByName: System failure");
			prismAssert (false, __FILE__, __LINE__);
			return(NULL);
		}

		return(NULL);
	}

	DBAttributesManagedObject *ZoneObjectManager::queryDBAttributesManagedObject(int loadRelationships) {

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (DBAttributesManagedObject::getClassName ());

		/* For Performance Reasons, want to avoid loading relationships unless we have to */
		if (loadRelationships  == FALSE) {
			synchronousQueryContext.setLoadOneToManyRelationships(false);
			synchronousQueryContext.setLoadCompositions(false);
		}

	    vector<WaveManagedObject *> *pResults =
	        querySynchronously (&synchronousQueryContext);

	    if (NULL != pResults) {
	        UI32 numberOfResults = pResults->size ();

	        if (0 == numberOfResults) {
	            trace (TRACE_LEVEL_INFO, "ZoneObjectManager::queryDBAttributesManagedObjectByName: Cannot find an entry");
                pResults->clear ();
                delete pResults;
	            return (NULL);
            } else if (1 == numberOfResults) {
	            DBAttributesManagedObject
	                *pDBAttributesManagedObject =
                    dynamic_cast<DBAttributesManagedObject *>
	                ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return (pDBAttributesManagedObject);
            } else {
                pResults->clear ();
                delete pResults;
                trace (TRACE_LEVEL_FATAL, "ZoneObjectManager::queryDBAttributesManagedObjectByName: There can be only one local entry.");
                prismAssert (false, __FILE__, __LINE__);
	            return (NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "ZoneObjectManager::queryDBAttributesManagedObjectByName: System failure");
            prismAssert (false, __FILE__, __LINE__);
	        return (NULL);
        }

	    return (NULL);
    }

	AliasManagedObject *ZoneObjectManager::queryAliasManagedObjectByName(string aliasName, int loadRelationships) {

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (AliasManagedObject::getClassName ());

	    /* Get the object corresponding to this mapped ID */
	    synchronousQueryContext.addAndAttribute (new AttributeString (aliasName, "aliasName"));

		/* For Performance Reasons, want to avoid loading relationships unless we have to */
		if (loadRelationships  == FALSE) {
			synchronousQueryContext.setLoadOneToManyRelationships(false);
			synchronousQueryContext.setLoadCompositions(false);
		}

	    vector<WaveManagedObject *> *pResults =
	        querySynchronously (&synchronousQueryContext);

	    if (NULL != pResults) {
	        UI32 numberOfResults = pResults->size ();

	        if (0 == numberOfResults) {
	            trace (TRACE_LEVEL_INFO, "ZoneObj:queryAliasManagedObjectByName: Cannot find aliasName entry");
                pResults->clear ();
                delete pResults;
	            return (NULL);
            } else if (1 == numberOfResults) {
	            AliasManagedObject *pAliasManagedObject =
                    dynamic_cast<AliasManagedObject *> ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return (pAliasManagedObject);
            } else {
                pResults->clear ();
                delete pResults;
                trace (TRACE_LEVEL_FATAL, "ZoneObj:queryAliasManagedObjectByName: There can be only one local node.");
                prismAssert (false, __FILE__, __LINE__);
	            return (NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "ZoneObj:queryAliasManagedObjectByName: System failure");
            prismAssert (false, __FILE__, __LINE__);
	        return (NULL);
        }

	    return (NULL);
    }

	int ZoneObjectManager::queryAliasMemberManagedObjectForMemberCount(ObjectId parentMoObjectId) {

		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (AliasMemberManagedObject::getClassName ());

		/* We only care about Member MO's who are children of the specified Parent MO */
		synchronousQueryContext.addAndAttribute (new AttributeObjectId (parentMoObjectId, "ownerManagedObjectId"));

		/* Avoid Loading 1-many composition relationships for better performance */
		synchronousQueryContext.setLoadOneToManyRelationships(false);

		/* Avoid Loading 1-1 composition relationships for better performance */
		synchronousQueryContext.setLoadCompositions(false);

		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 MOCount = 0;

		status = querySynchronouslyForCount (&synchronousQueryContext, MOCount);

		if (WAVE_MESSAGE_SUCCESS == status) {
				/*tracePrintf (TRACE_LEVEL_INFO, false, false,
	                    "queryAliasMemberManagedObjectForMemberCount: aliasname (%s), MOCount = %d\n",
							 pParentManagedObject->getAliasName().c_str(), MOCount);*/
		} else {
			trace (TRACE_LEVEL_FATAL, "ZoneObj:queryAliasMemberManagedObjectForMemberCount: Query Failed!");
		}

		return (MOCount);
    }

	AliasMemberManagedObject *ZoneObjectManager::queryAliasMemberManagedObjectByName(ObjectId parentMoObjectId /*AliasManagedObject *pParentManagedObject*/, string aliasEntry) {

		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (AliasMemberManagedObject::getClassName ());

		/* Get the object corresponding to this mapped ID */
		synchronousQueryContext.addAndAttribute (new AttributeObjectId (parentMoObjectId /*pParentManagedObject->getObjectId ()*/, "ownerManagedObjectId"));
		synchronousQueryContext.addAndAttribute (new AttributeString (aliasEntry, "aliasEntry"));

		/* Avoid Loading 1-many composition relationships for better performance */
		synchronousQueryContext.setLoadOneToManyRelationships(false);

		/* Avoid Loading 1-1 composition relationships for better performance */
		synchronousQueryContext.setLoadCompositions(false);

		vector<WaveManagedObject *> *pResults =
		querySynchronously (&synchronousQueryContext);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();

			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_INFO, "ZoneObj:queryAliasMemberManagedObjectByName: Cannot find defined zone member entry");
				pResults->clear ();
				delete pResults;
				return(NULL);
			} else if (1 == numberOfResults) {
				AliasMemberManagedObject *pAliasMemberManagedObject =
				dynamic_cast<AliasMemberManagedObject *> ((*pResults)[0]);

				pResults->clear ();
				delete pResults;
				return(pAliasMemberManagedObject);
			} else {
				pResults->clear ();
				delete pResults;
				trace (TRACE_LEVEL_FATAL, "ZoneObj:queryAliasMemberManagedObjectByName: There can be only one local node.");
				prismAssert (false, __FILE__, __LINE__);
				return(NULL);
			}
		} else {
			trace (TRACE_LEVEL_FATAL, "ZoneObj:queryAliasMemberManagedObjectByName: System failure");
			prismAssert (false, __FILE__, __LINE__);
			return(NULL);
		}

		return(NULL);
	}

	int ZoneObjectManager::queryIfAliasMOsExist() {

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext(
	        AliasManagedObject::getClassName ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 aliasMOCount = 0;

		status = querySynchronouslyForCount (&synchronousQueryContext, aliasMOCount);

	    if (WAVE_MESSAGE_SUCCESS == status) {
			if (aliasMOCount > 0) {
				return (TRUE);
			}
		} else {
			trace (TRACE_LEVEL_FATAL, "ZoneObj:queryIfAliasMOsExist: Query Failed!");
			return (FALSE);
        }

	    return (FALSE);
    }

	int ZoneObjectManager::queryManagedObjectForMemberCount(ObjectId parentMoObjectId, int znObjType) {
		string znObjStr;
		string childClassName;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 MOCount = 0;

		switch (znObjType) {
		case ZNCFG:
			znObjStr = "cfg";
			childClassName = CfgMemberManagedObject::getClassName ();
			break;

		case ZONE:
			znObjStr = "zone";
			childClassName = ZoneMemberManagedObject::getClassName ();
			break;

		case ALIAS:
			znObjStr = "alias";
			childClassName = AliasMemberManagedObject::getClassName ();
			break;

		default:
			tracePrintf (TRACE_LEVEL_INFO, false, false,
						 "ZoneObj:queryManagedObjectForMemberCount: ERR! illegal type %d\n",
						 znObjType);
			return (MOCount);

		}

		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (childClassName);

		/* We only care about Member MO's who are children of the specified Parent MO */
		synchronousQueryContext.addAndAttribute (new AttributeObjectId (parentMoObjectId , "ownerManagedObjectId"));

		/* Avoid Loading 1-many composition relationships for better performance */
		synchronousQueryContext.setLoadOneToManyRelationships(false);

		/* Avoid Loading 1-1 composition relationships for better performance */
		synchronousQueryContext.setLoadCompositions(false);

		status = querySynchronouslyForCount (&synchronousQueryContext, MOCount);

		if (WAVE_MESSAGE_SUCCESS == status) {
				/*tracePrintf (TRACE_LEVEL_INFO, false, false,
	                    "queryCfgMemberManagedObjectForMemberCount: cfgname (%s), MOCount = %d\n",
							 pParentManagedObject->getCfgName().c_str(), MOCount);*/
		} else {
			tracePrintf (TRACE_LEVEL_ERROR, false, false,"ZoneObj:queryManagedObjectForMemberCount: %s Query Failed!", znObjStr.c_str());
		}

		return (MOCount);
    }

    void ZoneObjectManager::failover(FailoverAsynchronousContext *pFailoverAsynchronousContext)
    {
        // FrameworkObjectManagerFailoverReason failoverReason                     = pFailoverAsynchronousContext->getfailoverReason ();
        // vector<LocationId>                   failedLocationIds                  = pFailoverAsynchronousContext->getfailedLocationIds ();
        // bool                                 isPrincipalChangedWithThisFailover = pFailoverAsynchronousContext->getIsPrincipalChangedWithThisFailover ();

	    tracePrintf (TRACE_LEVEL_INFO, false, false,
	        "ZoneObjectManager::failover %d\n",
	        pFailoverAsynchronousContext->
	        getIsPrincipalChangedWithThisFailover ());

	    // if we had a failover because the primary went down,
	    // we need to reload the database since any transaction
	    // in progress would have been aborted.
	    if (pFailoverAsynchronousContext->
	        getIsPrincipalChangedWithThisFailover ()) {

	        ResourceId status = WAVE_MESSAGE_SUCCESS; 

	        startTransaction();

	        overwriteDcmdFromZoned(0, db_boot_type_normal, reloadAll_op);

	        status = commitTransaction ();
	        if (status == FRAMEWORK_SUCCESS) {
                status = WAVE_MESSAGE_SUCCESS;
                trace (TRACE_LEVEL_DEVEL,"Success ");
            } else {
                trace (TRACE_LEVEL_DEVEL,"Committ Failed ");
            }

			(void)backendWyserZonePluginCreate();
	        (void)freeAllocatedManagedObjects();
	    }

        pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pFailoverAsynchronousContext->callback ();
    }

	void ZoneObjectManager::addDBAttributesManagedObjectToFree(
	    DBAttributesManagedObject *pDBAttributesManagedObject)
	{
	    allocatedDBAttributesManagedObject = pDBAttributesManagedObject;
	}

	void ZoneObjectManager::addCfgManagedObjectToFree(
	    CfgManagedObject *pCfgManagedObject)
	{
        vector<CfgManagedObject *>
            &allCreatedCfgManagedObject =
            getCreatedCfgManagedObject();

	    allCreatedCfgManagedObject.push_back(pCfgManagedObject);
	}

	void ZoneObjectManager::addZoneManagedObjectToFree(
	    ZoneManagedObject *pZoneManagedObject)
	{
        vector<ZoneManagedObject *>
            &allCreatedZoneManagedObject =
            getCreatedZoneManagedObject();

	    allCreatedZoneManagedObject.push_back(pZoneManagedObject);
	}

	void ZoneObjectManager::addAliasManagedObjectToFree(
	    AliasManagedObject *pAliasManagedObject)
	{
        vector<AliasManagedObject *>
            &allCreatedAliasManagedObject =
            getCreatedAliasManagedObject();

	    allCreatedAliasManagedObject.push_back(pAliasManagedObject);
	}

	/** Starting in Leo (NOS v4.0.0), the Wyser Compatibility
	 *  mode was introduced. This effort was done to improve DCMD
	 *  database performance to avoid querying the database as much
	 *  as possible.  Instead of querying the database, a cached
	 *  ManagedObject ObjectID lookup table is kept via Wyser.
	 *  This lookup table is created on the frontend via the
	 *  WyserEa for zoneobject creation via NOSCLI and
	 *  BNA/Netconf, but a similar table needs to be created in the
	 *  Zone Plugin for backend-sourced database creation scenarios
	 *  (i.e. zoneChangedEvents, zoneAbort Events, install(),
	 *  failover(), etc).  The purpose of this function is to add
	 *  the backend-created zone Managed Objects to the Wyser
	 *  Lookup Table so future 'frontend' database editing can
	 *  work properly. */
	ResourceId ZoneObjectManager::backendWyserZonePluginCreate()
	{
	    u_int i;

		tracePrintf(TRACE_LEVEL_DEBUG, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: start...\n");

		vector<CfgManagedObject *>
	        &allCreatedCfgManagedObject =
	        getCreatedCfgManagedObject();

		if (allCreatedCfgManagedObject.size() > 0) {
			tracePrintf(TRACE_LEVEL_DEBUG, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: query cfgs (size %d)...\n",
						allCreatedCfgManagedObject.size());
		}

	    for (i = 0; i < allCreatedCfgManagedObject.size(); i++) {

			/* WYSER CREATE BLOCK --start ------------------------------------------------------------ */
			string currentCfgName = allCreatedCfgManagedObject[i]->getCfgName();
			string targetNodeName = "zoning defined-configuration cfg " + currentCfgName;
	
			// Get the Object Id of the created policy
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt ("CfgManagedObject");
			// set the objectId as addSelectedField in the query
			syncQueryCtxt.addSelectFieldForObjectId ("objectId");
			syncQueryCtxt.addAndAttribute (new AttributeString (currentCfgName, "cfgName"));
			syncQueryCtxt.setLoadOneToManyRelationships(false);
			vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	
			// tracePrintf(TRACE_LEVEL_INFO, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: targetNodeName = %s\n", targetNodeName.c_str());

			/** If create successful, store ObjectId in
			 *  WaveSystemManagementGatewayWorker
			 *  TargetNodeNameObjectIdMap.  This cached value will be
			 *  used for future member-zone creates to avoid having to
			 *  query for the parentMO in the Zone Plugin which will
			 *  result in improved performance since. */
			if (pResults->size () == 1) {
				// tracePrintf(TRACE_LEVEL_INFO, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: wyser cfg query success...addTargetNodeNameObjectIdMap\n");
				CfgManagedObject *cfgMO = NULL;
				cfgMO = dynamic_cast<CfgManagedObject *>((*pResults)[0]);
				ObjectId parentObjId = cfgMO->getObjectId();
	
				WaveSystemManagementGatewayWorker::addTargetNodeNameObjectIdMap (targetNodeName, parentObjId);
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			} else {
				tracePrintf(TRACE_LEVEL_ERROR, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: wyser cfg query FAILED\n");

			}
			/* WYSER CREATE BLOCK --end ------------------------------------------------------------ */

	    }

	    vector<ZoneManagedObject *>
	        &allCreatedZoneManagedObject =
	        getCreatedZoneManagedObject();

		if (allCreatedZoneManagedObject.size() > 0) {
			tracePrintf(TRACE_LEVEL_DEBUG, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: query zones (size %d)...\n",
						allCreatedZoneManagedObject.size());
		}

	    for (i = 0; i < allCreatedZoneManagedObject.size(); i++) {
			/* WYSER CREATE BLOCK --start ------------------------------------------------------------ */
			string currentZoneName = allCreatedZoneManagedObject[i]->getZoneName();
			string targetNodeName = "zoning defined-configuration zone " + currentZoneName;
	
			// Get the Object Id of the created policy
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt ("ZoneManagedObject");
			// set the objectId as addSelectedField in the query
			syncQueryCtxt.addSelectFieldForObjectId ("objectId");
			syncQueryCtxt.addAndAttribute (new AttributeString (currentZoneName, "zoneName"));
			syncQueryCtxt.setLoadOneToManyRelationships(false);
			vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	
			// tracePrintf(TRACE_LEVEL_INFO, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: targetNodeName = %s\n", targetNodeName.c_str());

			/** If create successful, store ObjectId in
			 *  WaveSystemManagementGatewayWorker
			 *  TargetNodeNameObjectIdMap.  This cached value will be
			 *  used for future member-zone creates to avoid having to
			 *  query for the parentMO in the Zone Plugin which will
			 *  result in improved performance since. */
			if (pResults->size () == 1) {
				// tracePrintf(TRACE_LEVEL_INFO, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: wyser zone query success...addTargetNodeNameObjectIdMap\n");
				ZoneManagedObject *zoneMO = NULL;
				zoneMO = dynamic_cast<ZoneManagedObject *>((*pResults)[0]);
				ObjectId parentObjId = zoneMO->getObjectId();
	
				WaveSystemManagementGatewayWorker::addTargetNodeNameObjectIdMap (targetNodeName, parentObjId);
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			} else {
				tracePrintf(TRACE_LEVEL_ERROR, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: wyser zone query FAILED\n");

			}
			/* WYSER CREATE BLOCK --end ------------------------------------------------------------ */

	    }

		vector<AliasManagedObject *>
	        &allCreatedAliasManagedObject =
	        getCreatedAliasManagedObject();

		if (allCreatedAliasManagedObject.size() > 0) {
			tracePrintf(TRACE_LEVEL_DEBUG, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: query aliases (size %d)...\n",
						allCreatedAliasManagedObject.size());
		}

	    for (i = 0; i < allCreatedAliasManagedObject.size(); i++) {

			/* WYSER CREATE BLOCK --start ------------------------------------------------------------ */
			string currentAliasName = allCreatedAliasManagedObject[i]->getAliasName();
			string targetNodeName = "zoning defined-configuration alias " + currentAliasName;
	
			// Get the Object Id of the created policy
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt ("AliasManagedObject");
			// set the objectId as addSelectedField in the query
			syncQueryCtxt.addSelectFieldForObjectId ("objectId");
			syncQueryCtxt.addAndAttribute (new AttributeString (currentAliasName, "aliasName"));
			syncQueryCtxt.setLoadOneToManyRelationships(false);
			vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
	
			// tracePrintf(TRACE_LEVEL_INFO, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: targetNodeName = %s\n", targetNodeName.c_str());

			/** If create successful, store ObjectId in
			 *  WaveSystemManagementGatewayWorker
			 *  TargetNodeNameObjectIdMap.  This cached value will be
			 *  used for future member-zone creates to avoid having to
			 *  query for the parentMO in the Zone Plugin which will
			 *  result in improved performance since. */
			if (pResults->size () == 1) {
				// tracePrintf(TRACE_LEVEL_INFO, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: wyser alias query success...addTargetNodeNameObjectIdMap\n");
				AliasManagedObject *aliasMO = NULL;
				aliasMO = dynamic_cast<AliasManagedObject *>((*pResults)[0]);
				ObjectId parentObjId = aliasMO->getObjectId();
	
				WaveSystemManagementGatewayWorker::addTargetNodeNameObjectIdMap (targetNodeName, parentObjId);
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			} else {
				tracePrintf(TRACE_LEVEL_ERROR, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: wyser alias query FAILED\n");

			}
			/* WYSER CREATE BLOCK --end ------------------------------------------------------------ */
	    }

		tracePrintf(TRACE_LEVEL_DEBUG, false, false, "ZoneObjectManager::backendWyserZonePluginCreate: complete\n");

		return (WAVE_MESSAGE_SUCCESS);
	}

	ResourceId ZoneObjectManager::freeAllocatedManagedObjects()
	{
	    u_int i;

	    if (allocatedDBAttributesManagedObject) {
	        delete(allocatedDBAttributesManagedObject);
	        allocatedDBAttributesManagedObject = NULL;
	    }

	    vector<CfgManagedObject *>
	        &allCreatedCfgManagedObject =
	        getCreatedCfgManagedObject();
	    for (i = 0; i < allCreatedCfgManagedObject.size(); i++) {
	        delete(allCreatedCfgManagedObject[i]);
	    }
	    allCreatedCfgManagedObject.clear();

	    vector<ZoneManagedObject *>
	        &allCreatedZoneManagedObject =
	        getCreatedZoneManagedObject();
	    for (i = 0; i < allCreatedZoneManagedObject.size(); i++) {
	        delete(allCreatedZoneManagedObject[i]);
	    }
	    allCreatedZoneManagedObject.clear();

		vector<AliasManagedObject *>
	        &allCreatedAliasManagedObject =
	        getCreatedAliasManagedObject();
	    for (i = 0; i < allCreatedAliasManagedObject.size(); i++) {
	        delete(allCreatedAliasManagedObject[i]);
	    }
	    allCreatedAliasManagedObject.clear();

		return (WAVE_MESSAGE_SUCCESS);
	}

	vector<CfgManagedObject *> &ZoneObjectManager::getCreatedCfgManagedObject()
	{
	    return (allocatedCfgManagedObjects);
	}

	vector<ZoneManagedObject *> &ZoneObjectManager::getCreatedZoneManagedObject()
	{
	    return (allocatedZoneManagedObjects);
	}

	vector<AliasManagedObject *> &ZoneObjectManager::getCreatedAliasManagedObject()
	{
	    return (allocatedAliasManagedObjects);
	}

	string ZoneObjectManager::wwnStrToLower(string strToConvert)
	{
		for(unsigned int i = 0 ; i < strToConvert.length(); i++) {
			strToConvert[i] = tolower(strToConvert[i]);
		}

		return strToConvert;
	}

	ResourceId ZoneObjectManager::zoneRespToStatus(UI32 respCode)
	{

		switch (respCode) {
			case 0:
				return (WAVE_MESSAGE_SUCCESS);
			case NOMEMORY:
				return (ZONE_ERROR_NOMEMORY);
			case EZACCEPT:
				return (ZONE_ERROR_EZACCEPT);
			case EZBADPORT:
				return (ZONE_ERROR_EZBADPORT);
			case FCTYPEMIX:
				return (ZONE_ERROR_FCTYPEMIX);
			case ERSINGLEDEV:
				return (ZONE_ERROR_ERSINGLEDEV);
			case EZLUNMIX:
				return (ZONE_ERROR_EZLUNMIX);
			case EZMENMIX:
				return (ZONE_ERROR_EZMENMIX);
			case EZHARDSOFTMIX:
				return (ZONE_ERROR_EZHARDSOFTMIX);
			case EZFAQLMIX:
				return (ZONE_ERROR_EZFAQLMIX);
			case EZLUNMENMIX:
				return (ZONE_ERROR_EZLUNMENMIX);
			case EZNEMENMIX:
				return (ZONE_ERROR_EZNEMENMIX);
			case EZLUNOFFSETFULL:
				return (ZONE_ERROR_EZLUNOFFSETFULL);
			case ZT_SOFTZONE:
				return (ZONE_ERROR_ZT_SOFTZONE);
			case ZT_FABASSIST:
				return (ZONE_ERROR_ZT_FABASSIST);
			case ZT_QLOOPZONE:
				return (ZONE_ERROR_ZT_QLOOPZONE);
			case ZT_DRIVERERR:
				return (ZONE_ERROR_ZT_DRIVERERR);
			case ZG_NO_MORE_CAM:
				return (ZONE_ERROR_ZG_NO_MORE_CAM);
			case ZCHECKBADWWN:
				return (ZONE_ERROR_ZCHECKBADWWN);
			case WWN_IN_PORTZONE:
				return (ZONE_ERROR_WWN_IN_PORTZONE);
			case OFFSET_MASK_FULL:
				return (ZONE_ERROR_OFFSET_MASK_FULL);
			case PORT_IS_OFFLINE:
				return (ZONE_ERROR_PORT_IS_OFFLINE);
			case ALL_PORTS_ARE_OFFLINE:
				return (ZONE_ERROR_ALL_PORTS_ARE_OFFLINE);
			case QLOOP_NOT_SUPPORTED:
				return (ZONE_ERROR_QLOOP_NOT_SUPPORTED);
			case PORT_EPORT:
				return (ZONE_ERROR_PORT_EPORT);
			case NO_CHANGE_NO_SAVE:
				return (ZONE_ERROR_NO_CHANGE_NO_SAVE);
			case NO_CHANGE_NO_ENABLE:
				return (ZONE_ERROR_NO_CHANGE_NO_ENABLE);
			case EMPTY_CFG_W_ENABLE:
				return (ZONE_ERROR_EMPTY_CFG_W_ENABLE);
			case INDEX_TOO_BIG:
				return (ZONE_ERROR_INDEX_TOO_BIG);
			case 99:
				return (ZONE_ERROR_VCS_DISABLED);
			case ZN_ERR_INVALID_KEY:
				return (ZONE_ERROR_INVALID_KEY);
			case ZN_ERR_TRANS_IN_PROGRESS:
				return (ZONE_ERROR_TRANS_IN_PROGRESS);
			case ZN_ERR_DUPLICATE_KEY:
				return (ZONE_ERROR_DUPLICATE_KEY);
			case ZN_ERR_NO_MEMORY:
				return (ZONE_ERROR_NO_MEMORY);
			case TRANS_ABORT_MERGE_RECEIVED:
				return (ZONE_ERROR_TRANS_ABORT_MERGE_RECEIVED);
			case TRANS_ABORT_CFG_UPD_RECEIVED:
				return (ZONE_ERROR_TRANS_ABORT_CFG_UPD_RECEIVED);
			case TRANS_ABORT_BAD_CFG:
				return (ZONE_ERROR_TRANS_ABORT_BAD_CFG);
			case TRANS_ABORT_OPERATION_FAILED:
				return (ZONE_ERROR_TRANS_ABORT_OPERATION_FAILED);
			case TRANS_ABORT_SHELL_EXITED:
				return (ZONE_ERROR_TRANS_ABORT_SHELL_EXITED);
			case TRANS_ABORT_USER_COMMAND:
				return (ZONE_ERROR_TRANS_ABORT_USER_COMMAND);
			case TRANS_ABORT_DANGLING_XACT:
				return (ZONE_ERROR_TRANS_ABORT_DANGLING_XACT);
			case TRANS_ABORT_SYNCDUMP_OCCURRED:
				return (ZONE_ERROR_TRANS_ABORT_SYNCDUMP_OCCURRED);
			case ZN_ERR_NO_EFF_CFG:
				return (ZONE_ERROR_NO_EFF_CFG);
			case ZN_ERR_INVALID_VALUE:
				return (ZONE_ERROR_INVALID_VALUE);
			case ZN_MBR_EXIST:
				return (ZONE_ERROR_MBR_EXIST);
			case ZN_DUPLICATE_NAME:
				return (ZONE_ERROR_DUPLICATE_NAME);
			case ZN_NON_EXIST:
				return (ZONE_ERROR_NON_EXIST);
			case ZN_INVALID_NAME:
				return (ZONE_ERROR_INVALID_NAME);
			case ZN_UNEXPECTED_ERR:
				return (ZONE_ERROR_UNEXPECTED_ERR);
			// default access set returns codes in the upper two bytes
			case ZONE_SAME_DEFAULT_ACCESS_ALL:
			case ZONE_SAME_DEFAULT_ACCESS_NONE:
				return (ZONE_WARN_DEFAULT_ZONE_NEED_SAVE);
			// Any -(1000 + EAGAIN) errors will display error string from FOS
			case -(1000 + EAGAIN):
				return (ZONE_ERROR_EAGAIN);
			case ZN_ERR_DUPLICATE_ENTRY:
				return (ZONE_ERROR_DUPLICATE_KEY);
			case ZN_ERR_NO_TRANS:
				return (ZONE_ERROR_NO_TRANS);
			case ZN_ERR_EMPTY_ZONE_OBJECT:
				return (ZONE_ERROR_EMPTY_ZONE_OBJECT);
			case ZN_ERR_CFG_ENABLE_INVALID_CFGNAME:
				return (ZONE_ERROR_CFG_ENABLE_INVALID_CFGNAME);
			case ZN_ERR_CFG_ALIAS_MEMBER:
				return (ZONE_ERROR_CFG_ALIAS_MEMBER);
			case ZN_ERR_MULTI_MEMBER:
				return (WRC_ZONE_ERROR_MULTI_MEMBER);
	
			default:
				return (WAVE_MESSAGE_ERROR);
		}
	}


	UI32 ZoneObjectManager::loadZoningBulkConfiguration (UI32 argc, vector<string> argv)
	{
	    ZoneBulkConfigurationMessage zoneBulkConfigurationMessage;

	    CliConfigurationFile cliFile (argv[1]);

	    vector<string> cfgs;
	    vector<string> cfgMembers;
        vector<string> zones;
        vector<string> zoneMembers;
        UI32           i                   = 0;
        UI32           j                   = 0;
        string         cfgCli              = "zoning defined-configuration cfg";
        UI32           numberOfCfgs        = 0;
        string         cfgMemberCli;
        UI32           numberOfCfgMembers  = 0;
        UI32           numberOfZones       = 0;
        UI32           numberOfZoneMembers = 0;
        string         zoneCli             = "zoning defined-configuration zone";
        string         zoneMemberCli;

        cliFile.getChildrenCliNodeNamesForCommandLine (cfgCli, cfgs);

        numberOfCfgs = cfgs.size ();

        for (i = 0; i < numberOfCfgs; i++)
        {
            cfgMemberCli = cfgCli + " " + cfgs[i] + " member-zone";

            cfgMembers.clear ();

            cliFile.getChildrenCliNodeNamesForCommandLine (cfgMemberCli, cfgMembers);

            numberOfCfgMembers = cfgMembers.size ();

            //WaveNs::trace (TRACE_LEVEL_INFO, cfgs[i]);

            for (j = 0; j < numberOfCfgMembers; j++)
            {
                //WaveNs::trace (TRACE_LEVEL_INFO, "    " + cfgMembers[j]);
            }

            zoneBulkConfigurationMessage.addCfg (cfgs[i], cfgMembers);
        }

        cliFile.getChildrenCliNodeNamesForCommandLine (zoneCli, zones);

        numberOfZones = zones.size ();

        for (i = 0; i < numberOfZones; i++)
        {
            zoneMemberCli = zoneCli + " " + zones[i] + " member-entry";

            zoneMembers.clear ();

            cliFile.getChildrenCliNodeNamesForCommandLine (zoneMemberCli, zoneMembers);

            numberOfZoneMembers = zoneMembers.size ();

            //WaveNs::trace (TRACE_LEVEL_INFO, zones[i]);

            for (j = 0; j < numberOfZoneMembers; j++)
            {
                //WaveNs::trace (TRACE_LEVEL_INFO, "    " + zoneMembers[j]);
            }

            zoneBulkConfigurationMessage.addZone (zones[i], zoneMembers);
        }

        ResourceId status = WaveObjectManagerToolKit::sendSynchronously (&zoneBulkConfigurationMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "ZoneObjectManager::loadZoningBulkConfiguration : Failed with Status : " + FrameworkToolKit::localize (status));
        }
        else
        {
            status = zoneBulkConfigurationMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                WaveNs::trace (TRACE_LEVEL_ERROR, "ZoneObjectManager::loadZoningBulkConfiguration : Failed with Status : " + FrameworkToolKit::localize (status));
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_SUCCESS, "ZoneObjectManager::loadZoningBulkConfiguration : Succeeded to load zoning bulk configuration from " + argv[1]);
            }
        }

	    return (0);
	}
}
