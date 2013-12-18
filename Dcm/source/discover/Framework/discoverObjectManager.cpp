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
 *   Author : mmohan                                                       *
 **************************************************************************/

#include "discover/Framework/discoverObjectManager.h"
#include "discover/User/discoverWorkers.h"
#include "discover/User/DiscoverMultiWorker.h"
#include "discover/Framework/discoverTypes.h"
#include "discover/User/discoverPluginDebug.h"
#include "discover/User/discoverDcmDef.h"

#include "Framework/Utils/FrameworkToolKit.h"
#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"
#include <pthread.h>
#include <curl/curl.h>
#include <openssl/crypto.h>


namespace DcmNs
{

static pthread_mutex_t *lockarray;

static void lock_callback(int mode, int type, char *file, int line)
{

  (void)file;
  (void)line;
  if (mode & CRYPTO_LOCK) {
    pthread_mutex_lock(&(lockarray[type]));
  }
  else {
    pthread_mutex_unlock(&(lockarray[type]));
  }
}

static unsigned long thread_id(void)
{
  unsigned long ret;

  ret=(unsigned long)pthread_self();
  return(ret);
}

void DiscoverObjectManager::init_locks(void)
{
  int i;
  lockarray=(pthread_mutex_t *)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
  for (i=0; i<CRYPTO_num_locks(); i++) {
    pthread_mutex_init(&(lockarray[i]),NULL);
  }

  CRYPTO_set_id_callback((unsigned long (*)())thread_id);
  CRYPTO_set_locking_callback((void (*)(int,int,const char *,int))lock_callback);
}

static void kill_locks(void)
{
  int i;
 
  CRYPTO_set_locking_callback(NULL);
  for (i=0; i<CRYPTO_num_locks(); i++)
    pthread_mutex_destroy(&(lockarray[i]));
 
  OPENSSL_free(lockarray);
}
    DiscoverObjectManager::DiscoverObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
      init_locks();

 //   #ifdef _X86_COMPILE_
            m_pDiscoverMultiWorker = new DiscoverMultiWorker(this);
            prismAssert (NULL != m_pDiscoverMultiWorker, __FILE__, __LINE__);
 /*   #else

            m_pDiscoverWorker = new DiscoverWorker (this);
            prismAssert (NULL != m_pDiscoverWorker, __FILE__, __LINE__);
    #endif */


    }

    DiscoverObjectManager::~DiscoverObjectManager ()
    {
      kill_locks();
    }

    DiscoverObjectManager  *DiscoverObjectManager::getInstance()
    {
        static DiscoverObjectManager *pDiscoverObjectManager = new DiscoverObjectManager ();

        WaveNs::prismAssert (NULL != pDiscoverObjectManager, __FILE__, __LINE__);

        return (pDiscoverObjectManager);
    }

    PrismServiceId  DiscoverObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *DiscoverObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DiscoverObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }


/*  
	Comment out this portion of the code for SA drop, Uncomment for FC drop later
	after doing necessary testing in FC Mode.
	
	void
    DiscoverObjectManager::clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, 
                                                                 PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        dce_cluster_validation_data_t   *data_p;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        
        DISC_PLUG_DBG("Cluster Collect function called.");

        data_p = dce_cluster_validation_collect_init(DISC_PLUGIN_GLOBAL_VERSION);
        if (data_p == NULL) {
            DISC_PLUG_DBG_ERR("Failed to init ValidationData");
            pPrismAsynchronousContext->setCompletionStatus (DCE_CLUSTER_UTIL_ALLOC_FAILED);
            pPrismAsynchronousContext->callback ();
            return;
        }

        status = dce_cluster_validation_collect_done(pMessage, data_p);
        if (status != DCE_CLUSTER_UTIL_SUCCESS) {
            pPrismAsynchronousContext->setCompletionStatus (status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
    }

    void
    DiscoverObjectManager::clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage,
                                                    PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        void                            *payload_p;
        dce_cluster_validation_data_t   *data_p;
        UI32                            len;
        ResourceId                      status = WAVE_MESSAGE_SUCCESS;
        uint32_t                        version = 0, num_records = 0;
        uint64_t                        cap_bits = 0;
        
        DISC_PLUG_DBG("Cluster Validate function called.");

        pMessage->getValidationDetails(payload_p, len);

        DISC_PLUG_DBG(string("Received Validation Data of LEN=") + len);

        data_p = reinterpret_cast<dce_cluster_validation_data_t *>(payload_p);

        status = dce_cluster_validation_parse(data_p, &version, &cap_bits, &num_records);
        if (status != DCE_CLUSTER_UTIL_SUCCESS) {
            DISC_PLUG_DBG_ERR(string("Failed to parse ValidationData Err=") +
                                FrameworkToolKit::localize(status)); 
            pPrismAsynchronousContext->setCompletionStatus(status);
            pPrismAsynchronousContext->callback ();
            return;
        }

        DISC_PLUG_DBG_SUC(string("Recieved version = ") + version);
        
        if (version != DISC_PLUGIN_GLOBAL_VERSION) {
            DISC_PLUG_DBG_ERR(string("Version mismatch, My-Version=") + DISC_PLUGIN_GLOBAL_VERSION +
                                string(" Rcvd-Version=") + version);
            pPrismAsynchronousContext->setCompletionStatus(WRC_DISC_COMMON_ERR_CLUSTER_VALIDATE_VERSION_MISMATCH);
            pPrismAsynchronousContext->callback ();
            return;
        }

        pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pPrismAsynchronousContext->callback ();
        return;
    }
*/
}

