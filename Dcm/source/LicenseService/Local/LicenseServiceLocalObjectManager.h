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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.		  *
 *   All rights reserved.												  *
 *   Author : vbagavat													  *
 **************************************************************************/

#ifndef LICENSESERVICELOCALOBJECTMANAGER_H
#define LICENSESERVICELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "LicensePodLocalManagedObject.h"

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
//#include "dcm/fos_headers.h"
extern "C"
{
#include "sysmod/syscomm.h"
#include "sys/fabos/fabsys.h"
#include "fabos/license.h"
}

/* MAX PORT for installing the DCMD DB */
#define ELARA2_DPOD_PORTS     24
#define CALLISTO_DPOD_PORTS   60
#define CASTOR_10G_DPOD_PORTS 48
#define CASTOR_40G_DPOD_PORTS 4     /* count at QSFP level */
#define  MAX_DPOD_PORTS	80  /* This should be changed later restricted to 80 to avoid unncessary loop */

/* POD License Configuration */
#define ELARA2_BASE_DPOD_PORTS   16
#define ELARA2_POD1_LICENSE       8
#define ELARA2_POD2_LICENSE       0
#define CALLISTO_BASE_DPOD_PORTS 40 
#define CALLISTO_POD1_LICENSE    10
#define CALLISTO_POD2_LICENSE    10
#define CASTOR_BASE_10G_DPOD_PORTS   24 
#define CASTOR_BASE_40G_DPOD_PORTS   0 

#define POD_QUERY_DEFAULT_PORT "0/1"
#define PORT_START_NUM         1

using namespace WaveNs;

namespace DcmNs
{

	class LicenseServiceRemoveMessage;
	class LicenseServiceAddMessage;
	class LicenseServiceShowMessage;
	class LicenseServicePodShowMessage;
	class LicenseServiceGetLicenseIdShowMessage;
	class LicenseServicePodMessage;
	class LicenseServiceUpdateLicensePodInfoMessage;
	class LicenseServiceUpdateLicenseFabsysInfoMessage;
	class LicenseServiceFileAddMessage;

	class LicenseServiceLocalObjectManager: public WaveLocalObjectManager
	{
		private:
			PrismMessage *createMessageInstance(const UI32 &operationCode);
			WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			ResourceId  LicenseAddEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceAddMessageContext);
			ResourceId  LicenseRemoveEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceRemoveMessageContext);
			ResourceId  LicenseShowEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceShowMessageContext);
			ResourceId  LicensePodShowEntries (PrismSynchronousLinearSequencerContext *pLicenseServicePodShowMessageContext);
			ResourceId  LicenseIdShowGetEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceGetLicenseIdShowMessageContext);
			ResourceId  LicensePodEntries (PrismSynchronousLinearSequencerContext *pLicenseServicePodMessageContext);
			ResourceId  LicensePodUpdateEntries(PrismSynchronousLinearSequencerContext *pLicenseServiceLicensePodUpdateMessageContext);
            ResourceId LicenseFabsysInfoUpdateEntries(PrismSynchronousLinearSequencerContext *pLicenseServiceUpdateLicenseFabsysInfoMessageContext);
			ResourceId  LicenseFileAddEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceFileAddMessageContext);

			void LicenseServiceAddMessageHandler( LicenseServiceAddMessage *pLicenseServiceAddMessage);
			void LicenseServiceRemoveMessageHandler( LicenseServiceRemoveMessage *pLicenseServiceRemoveMessage);
			void LicenseServiceShowMessageHandler( LicenseServiceShowMessage *pLicenseServiceShowMessage);
			void LicenseServicePodMessageHandler( LicenseServicePodMessage *pLicenseServicePodMessage);
			void LicenseServiceGetLicenseIdShowMessageHandler( LicenseServiceGetLicenseIdShowMessage *pLicenseServiceGetLicenseIdShowMessage);
			void LicenseServicePodShowMessageHandler( LicenseServicePodShowMessage *pLicenseServicePodShowMessage);
            void LicenseServiceUpdateLicenseFabsysInfoMessageHandler( LicenseServiceUpdateLicenseFabsysInfoMessage *pLicenseServiceUpdateLicenseFabsysInfoMessage);
			void LicenseServiceUpdateLicensePodInfoMessageHandler( LicenseServiceUpdateLicensePodInfoMessage *pLicenseServiceUpdateLicensePodInfoMessage);
			void LicenseServiceFileAddMessageHandler( LicenseServiceFileAddMessage *pLicenseServiceFileAddMessage);
            // method that is invoked at bringup -- will copy dpod configuration data from the switch driver to
            // the DCMd database
			ResourceId backendSyncUpLicense(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			void install(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			void externalStateSynchronization(WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);
			LicensePodLocalManagedObject *QueryLicensePodLocalManagedObject(string portId);
		protected:
		public:
			LicenseServiceLocalObjectManager ();
			virtual	~LicenseServiceLocalObjectManager ();
			static LicenseServiceLocalObjectManager *getInstance();
			void backendSyncUp (PrismAsynchronousContext *pPrismAsynchronousContext);
			static string getClassName();
			static PrismServiceId getPrismServiceId();

// Now the data members

		private:

		protected:
		public:
	};

    inline string  LicenseServiceLocalObjectManager::getClassName()
    {
        return ("LicenseService");
    }

}
#endif											//LICENSESERVICELOCALOBJECTMANAGER_H
