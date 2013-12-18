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

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "discover/Framework/discoverObjectManager.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/Framework/discoverManagementInterfaceMessages.h"
#include "discover/Framework/discoverTypes.h"
#include "discover/Framework/discoverLocalMessages.h"
#include "discover/Framework/discoverManagedObjects.h"
#include "discover/User/discoverWorkers.h"
#include "discover/User/discoverTimers.h"
#include "discover/User/discoverPluginDebug.h"
#include "VirtualIp/Global/VirtualIpGlobalManagedObject.h"
#include "WyserEaGateway/Virtual/sq.h"
#include "vCenter/Framework/vCenterManagedObjects.h"
#include "vCenter/Framework/vCenterCrypto.h"
#include "File/Local/Test/FileLocalMessagingTestObjectManager.h"
#include "File/Local/FileLocalTypes.h"

#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <raslog/raslog.h>
#include <raslog/raslog_vcenter.h>

#include "vcs.h"
#include "discover/utils/FileUtils.h"

#include <sstream>

#define DBFILE          ((char *) "/tmp/dc.db")
#define DBFILE_NEW      ((char *) "/tmp/dc.db.new")
//#define DBFILE_ORIG     ((char *) "/fabos/webtools/htdocs/ws/config/dc.db")
#define PHPDIR_PATH     ((char *) "/fabos/webtools/htdocs/ws")
#define INTERPRETER     ((char *) "/usr/apache/php")
#define DISCOVER_SCRIPT ((char *) "/fabos/webtools/htdocs/ws/discover.php")
#define DISCOVERY_CHECK ((char *) "/fabos/webtools/htdocs/ws/check_vcenter_response.php")

namespace DcmNs
{
	vector<discoverStatusRecord_t *> dStatusRecords;

    DiscoverWorker::DiscoverWorker (DiscoverObjectManager *pDiscoverObjectManager)
        : WaveWorker  (pDiscoverObjectManager)
    {
        DiscoverManagedObject    DiscoverManagedObject    (pDiscoverObjectManager);
        DiscoverManagedObject.setupOrm ();
        addManagedClass (DiscoverManagedObject::getClassName (), this);
        addOperationMap (DISCOVERDISCOVER, reinterpret_cast<PrismMessageHandler> (&DiscoverWorker::DiscoverDiscoverMessageHandler));
        addOperationMap (DISCOVERSTATUS, reinterpret_cast<PrismMessageHandler> (&DiscoverWorker::DiscoverStatusMessageHandler));
		pDiscoverStartDiscoverTimer = DiscoverStartDiscoverTimer::getInstance();
    }

    DiscoverWorker::~DiscoverWorker ()
    {
    }

    PrismMessage  *DiscoverWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case DISCOVERDISCOVER :
                pPrismMessage = new DiscoverDiscoverMessage ();
                break;
            case DISCOVERSTATUS :
                pPrismMessage = new DiscoverStatusMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DiscoverWorker::createManagedObjectInstance(const string &managedClassName)
    {
        trace(TRACE_LEVEL_INFO, string(__FUNCTION__));
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((DiscoverManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new DiscoverManagedObject(dynamic_cast<DiscoverObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "DiscoverWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }


    void  DiscoverWorker::DiscoverDiscoverMessageHandler( DiscoverDiscoverMessage *pDiscoverDiscoverMessage)
    {
        trace(TRACE_LEVEL_INFO, string(__FUNCTION__));
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&DiscoverWorker::setDiscoverStateBeginStep),
            reinterpret_cast<PrismLinearSequencerStep>(&DiscoverWorker::stopDiscoverTimerStep),
            reinterpret_cast<PrismLinearSequencerStep>(&DiscoverWorker::doDiscoverStep),
			reinterpret_cast<PrismLinearSequencerStep>(&DiscoverWorker::doCheckDiscoverStep),
            reinterpret_cast<PrismLinearSequencerStep>(&DiscoverWorker::doDistributionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&DiscoverWorker::sendMessageToLocalPluginStep),
            reinterpret_cast<PrismLinearSequencerStep>(&DiscoverWorker::discoverDiscoverSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&DiscoverWorker::discoverDiscoverSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext =
            new PrismLinearSequencerContext (pDiscoverDiscoverMessage,
                                             this, sequencerSteps,
                                             sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }
    
    /* update the discover status*/
	ResourceId DiscoverWorker::setDiscoverStateBeginStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

        DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        
        if((dmsg != NULL) && (dmsg->getVcenter().length() == 0)) {
            DISC_PLUG_DBG(string("No Vcenter name in message"));
		    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			return WAVE_MESSAGE_SUCCESS;
		}
        
        if ((dmsg->getCmdcode() == DISC_CMD_DISCOVER_VCENTER) ||
            (dmsg->getCmdcode() == DISC_CMD_DISCOVER_ALL_VCENTER)) {
            
            /* Set State as START */
            if (dmsg->getState() == DISC_STATE_UNKNOWN)
                dmsg->setState(DISC_STATE_START);
             
            DiscoverManagedObject* pMO = NULL;
            /* create the managed object if not present */
            if ((pMO = getDiscoverManagedObject(dmsg->getVcenter())) == NULL) {
                startTransaction();
                pMO = new DiscoverManagedObject(dynamic_cast<DiscoverObjectManager*>(getPWaveObjectManager()));
                pMO->setId(dmsg->getVcenter());
                DISC_PLUG_DBG(string("Created Managed Object for ") + dmsg->getVcenter());
                commitTransaction();
            }
            
            /* Release Memory */
            delete pMO;
            
            /* State Machine Update */
            markDiscoverStatusStep(pPrismLinearSequencerContext);
        }

		DISC_PLUG_DBG_FUNC_END();
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return (WAVE_MESSAGE_SUCCESS);
	}
    
    /* delete discover status*/
	ResourceId DiscoverWorker::deleteDiscoverManagedObject (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

        DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        
        if((dmsg != NULL) && (dmsg->getVcenter().length() == 0)) {
            DISC_PLUG_DBG(string("No Vcenter name in message"));
			return WAVE_MESSAGE_SUCCESS;
		}
        
        if ((dmsg->getCmdcode() == DISC_CMD_DISCOVER_VCENTER) ||
            (dmsg->getCmdcode() == DISC_CMD_DISCOVER_ALL_VCENTER)) {
            
            if (dmsg->getMsgType() == DISC_MSG_TYPE_DELETE) {
                
                DiscoverManagedObject* pMO = NULL;
                if ((pMO =  getDiscoverManagedObject(dmsg->getVcenter())) != NULL) {
                    startTransaction();
                    delete pMO;
                    DISC_PLUG_DBG(string("Deleted Managed Object for ") + dmsg->getVcenter());
                    commitTransaction();                 
                }
            }
        }

		DISC_PLUG_DBG_FUNC_END();
        return (WAVE_MESSAGE_SUCCESS);
	}
	
    DiscoverManagedObject*
	DiscoverWorker::getDiscoverManagedObject (string vcenterName)
	{
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DiscoverManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&vcenterName,"id"));
    	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		DiscoverManagedObject* pMO = NULL;
        
		if (NULL != pResults) {
	        UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                DISC_PLUG_DBG(string("MO doesn't exist"));
	        } else if (1 == numberOfResults) {
				pMO = dynamic_cast<DiscoverManagedObject*>((*pResults)[0]);
                DISC_PLUG_DBG(string("Found Managed Object for ") + vcenterName);
            } else {
				 /* Managed object inconsistent by having multiple entries for
				  * same key
				  */
			}
        }
		pResults->clear();
		delete pResults;
		return pMO;
	}
    
    /* update the discover status*/
	ResourceId DiscoverWorker::markDiscoverStatusStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

        DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
		
        if((dmsg != NULL) && (dmsg->getVcenter().length() == 0)) {
            DISC_PLUG_DBG(string("No Vcenter name in message"));
			return WAVE_MESSAGE_SUCCESS;
		}
        struct timeval tv;
		time_t currentTime = 0;
		
        /* Get current time */
        gettimeofday(&tv, NULL);
        currentTime = tv.tv_sec;

        if ((dmsg->getCmdcode() == DISC_CMD_DISCOVER_VCENTER) ||
            (dmsg->getCmdcode() == DISC_CMD_DISCOVER_ALL_VCENTER)) {
            

            DiscoverManagedObject* pMO = getDiscoverManagedObject(dmsg->getVcenter());
            if(pMO == NULL) {
                DISC_PLUG_DBG(string("Couldn't find the MO"));
                return WAVE_MESSAGE_SUCCESS;
            }

            startTransaction();
            
            switch(dmsg->getState()) {
            case DISC_STATE_START:
                pMO->setStartTS(currentTime);
                pMO->setDiscStartTS(0);
                pMO->setDiscEndTS(0);
                pMO->setPpStartTS(0);
                pMO->setPpEndTS(0);
                pMO->setExitCode(0);
                pMO->setEndTS(0);
                break;
            case DISC_STATE_STOP_TIMER:
            case DISC_STATE_DISCOVER_START:
                pMO->setDiscStartTS(currentTime);
                break;
            case DISC_STATE_DISCOVER_END:
                pMO->setDiscEndTS(currentTime);
                pMO->setExitCode(dmsg->getExitStatus());
                break;
            case DISC_STATE_DISTRIBUTION:
                break;
            case DISC_STATE_PPCONFIG_COPY_START:
                pMO->setPpStartTS(currentTime);
                break;
            case DISC_STATE_PPCONFIG_COPY_END:
                pMO->setPpEndTS(currentTime);
                break;
            case DISC_STATE_START_TIMER:
                break;
            case DISC_STATE_END:
                pMO->setEndTS(currentTime);
                break;
            default :
                break;
            }
            
            pMO->setState(dmsg->getState());

            updateWaveManagedObject(pMO);
            commitTransaction();
            delete pMO;

        }
		
		DISC_PLUG_DBG_FUNC_END();
        return (WAVE_MESSAGE_SUCCESS);
	}
	
	void DiscoverWorker::discoverDiscoverSequencerSucceededStep (PrismLinearSequencerContext *pPrismLinearSequencerContext) 
	{
			
		startDiscoverTimerStep (pPrismLinearSequencerContext);
        
        DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());

        /* State Machine Update */
        dmsg->setState(DISC_STATE_END);
        markDiscoverStatusStep (pPrismLinearSequencerContext);
            
        /* Delete Mo on discovered asset deletion */
        deleteDiscoverManagedObject(pPrismLinearSequencerContext);
		
		prismLinearSequencerSucceededStep (pPrismLinearSequencerContext);
	}

	void DiscoverWorker::discoverDiscoverSequencerFailedStep (PrismLinearSequencerContext *pPrismLinearSequencerContext) 
	{
			
		startDiscoverTimerStep (pPrismLinearSequencerContext);
        
        DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        
        /* State Machine Update */
        dmsg->setState(DISC_STATE_END);
        markDiscoverStatusStep (pPrismLinearSequencerContext);
        
        /* Delete Mo on discovered asset deletion */
        deleteDiscoverManagedObject(pPrismLinearSequencerContext);
        
		prismLinearSequencerFailedStep (pPrismLinearSequencerContext);
	}

	void DiscoverWorker::extractScriptNames (PrismLinearSequencerContext 
					*pPrismLinearSequencerContext, vector<string> &words) {
		
		DISC_PLUG_DBG_FUNC_BEG();

		DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
		stringstream ssNames(dmsg->getGlobalScriptNames());
		string word;
		while (ssNames >> word) {
			words.push_back(word);
		}
		
		DISC_PLUG_DBG_FUNC_END();
	}

#ifdef USE_POPEN
	ResourceId DiscoverWorker::doFreshDiscovery (PrismLinearSequencerContext *pPrismLinearSequencerContext) {
		
		DISC_PLUG_DBG_FUNC_BEG();

		FILE *fp;
		int ret;
		struct timeval tv;
		time_t sTime, eTime;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		vector<string> scripts;
		vector<string>::size_type i;
    
		/* Extract php script names */
		extractScriptNames(pPrismSynchronousLinearSequencerContext, scripts);
    	
		DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
		
		if (!FileUtils::fileexists(DBFILE_ORIG) || !FileUtils::fileexists(INTERPRETER)) {
            DISC_PLUG_DBG(string("vCenter discovery env. does not exist"));
            return (WAVE_MESSAGE_ERROR);
        }
        if (!FileUtils::fileexists(DBFILE)) {
            if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
                return (WAVE_MESSAGE_ERROR);
            }
        } 
        if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
            DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
            return (WAVE_MESSAGE_ERROR);
        }
		
		/* invoke php scripts */
		for (i = 0; i != scripts.size(); i++) {
			DISC_PLUG_DBG(string("Handling request to invoke ") + scripts[i]);
			const string scriptPath = "/fabos/webtools/htdocs/ws/" + scripts[i] + ".php";
			
			if (!FileUtils::fileexists((char *)&scriptPath[0])) {
				DISC_PLUG_DBG_ERR(scripts[i] + string(" script does not exist (ignoring)"));
				return (WAVE_MESSAGE_ERROR);
			}
			/* get the virtual ip and pass it to the script
			   Enable this code for FC & MC support

			   string vIpAddress
			   ret = getVirtualIpAddress(vIpAddress);
			*/
		
			string cmd = string("(cd ") + string(PHPDIR_PATH) + ";" +
            	string(INTERPRETER) + string(" ") + scriptPath + string(" ") + dmsg->getVcenter() + ";)";
			/* update discover status */
			gettimeofday(&tv, NULL);
			sTime = tv.tv_sec;
			updateDiscoverStatus(dmsg, DISC_STAGE_DISCOVERY_BEGIN, sTime, 0);
			
			if ((fp = popen(cmd.c_str(), "r")) == NULL) {
				gettimeofday(&tv, NULL);
				eTime = tv.tv_sec;
				updateDiscoverStatus(dmsg, DISC_STAGE_DISCOVERY_FAILURE, sTime, eTime);
				DISC_PLUG_DBG_ERR(string("discovery failed: "));
				status = WAVE_MESSAGE_ERROR;
			} else {
				gettimeofday(&tv, NULL);
				eTime = tv.tv_sec;
				updateDiscoverStatus(dmsg, DISC_STAGE_DISCOVERY_SUCCESS, sTime, eTime);
				DISC_PLUG_DBG_SUC(string("discovery done"));
				status = WAVE_MESSAGE_SUCCESS;
				pclose(fp);
			}
		}
        
		DISC_PLUG_DBG_FUNC_END();
        return (status);
	}

#else

	ResourceId DiscoverWorker::doFreshDiscovery (PrismLinearSequencerContext *pPrismLinearSequencerContext) {
		DISC_PLUG_DBG_FUNC_BEG();
	
		int ret, pid, tpid, childstatus = 0, exitStatus = -1;
		struct timeval tv;
		time_t sTime=0, eTime=0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		vector<string> scripts;
		vector<string>::size_type i;
    
		DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
		
		
		if (!FileUtils::fileexists(DBFILE_ORIG) || !FileUtils::fileexists(INTERPRETER)) {
            DISC_PLUG_DBG(string("vCenter discovery env. does not exist (ignoring)"));
            return (WAVE_MESSAGE_ERROR);
        }
        if (!FileUtils::fileexists(DBFILE)) {
            if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
                return (WAVE_MESSAGE_ERROR);
            }
        } 
		
		/* check if the vcenter name is specified in the msg 
		 */
		if((dmsg != NULL) && (dmsg->getVcenter().length() == 0)) {
            DISC_PLUG_DBG(string("No Vcenter configured"));
			return WAVE_MESSAGE_ERROR;
		}
		
        if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
            DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
            return (WAVE_MESSAGE_ERROR);
        }
       
        /* State Machine Update */
        dmsg->setState(DISC_STATE_DISCOVER_START);
        markDiscoverStatusStep (pPrismLinearSequencerContext);
		
		/* Extract php script names */
		extractScriptNames(pPrismLinearSequencerContext, scripts);
        
        string vCenter  = dmsg->getVcenter();
        string url      = dmsg->getUrl();
        string username = dmsg->getUsername();
        string password = vCenterCrypto::decryptPassword(dmsg->getPassword());
        
    	
		/* invoke php scripts */
		for (i = 0; i != scripts.size(); i++) {
			DISC_PLUG_DBG(string("Handling request to invoke ") + scripts[i]);
			const string scriptPath = "/fabos/webtools/htdocs/ws/" + scripts[i] + ".php";
			
			if (!FileUtils::fileexists((char *)&scriptPath[0])) {
				DISC_PLUG_DBG_ERR(scripts[i] + string(" script does not exist (ignoring)"));
				return (WAVE_MESSAGE_ERROR);
			}
			
			pid = fork();
			if (pid == 0) {
				/* get the virtual ip and pass it to the script
				   Enable this code for FC & MC support

				   string vIpAddress
				   ret = getVirtualIpAddress(vIpAddress);

				 */
				if ((ret = chdir(PHPDIR_PATH)) < 0) {
					DISC_PLUG_DBG_ERR(string("Failed chdir to: ") + string(PHPDIR_PATH));
				}
				else if ((ret = execl(INTERPRETER, INTERPRETER, scriptPath.c_str(), vCenter.c_str(), 
                                url.c_str(), username.c_str(), password.c_str(), NULL)) < 0) {
					DISC_PLUG_DBG_ERR(string("Failed to execute: ") + scriptPath);
				}
				exit(0);
			} else {
				if (scriptPath == string(DISCOVER_SCRIPT)) {
					gettimeofday(&tv, NULL);
					sTime = tv.tv_sec;
					updateDiscoverStatus(dmsg, DISC_STAGE_DISCOVERY_BEGIN, sTime, 0);
				}

                    tpid = wait4(pid, &childstatus, WNOHANG, NULL);              

				if (tpid == pid) {
    					exitStatus = WEXITSTATUS(childstatus);
					if (exitStatus == 0) {
						if (scriptPath == string(DISCOVER_SCRIPT)) {
							gettimeofday(&tv, NULL);
							eTime = tv.tv_sec;
							updateDiscoverStatus(dmsg, DISC_STAGE_DISCOVERY_SUCCESS, sTime, eTime);
                            dmsg->setExitStatus(exitStatus);
                            resetDiscoverFailureCounters();
						}
						status = WAVE_MESSAGE_SUCCESS;
						DISC_PLUG_DBG_SUC(string("Discover succeeded"));
					} else {
						DISC_PLUG_DBG(string("Exit Status ") + exitStatus/* WEXITSTATUS(childstatus) */);
						if (scriptPath == string(DISCOVER_SCRIPT)) {
							gettimeofday(&tv, NULL);
							eTime = tv.tv_sec;
							updateDiscoverStatus(dmsg, DISC_STAGE_DISCOVERY_FAILURE, sTime, eTime);
                            dmsg->setExitStatus(exitStatus);
                            incrDiscoverFailureCount((discoverExitStatus_t)exitStatus);
                            generateDiscoverFailureRaslog(pPrismLinearSequencerContext, (discoverExitStatus_t)exitStatus);
                        }
						status = WAVE_MESSAGE_ERROR;
						DISC_PLUG_DBG_ERR(string("Discover failed (check logs)"));
					}
                    
                    /* State Machine Update */
                    dmsg->setState(DISC_STATE_DISCOVER_END);
                    markDiscoverStatusStep (pPrismLinearSequencerContext);
                    
				}
			}
		}
		DISC_PLUG_DBG_FUNC_END();
		status = getReturnStatusForExitStatus (status, exitStatus);
		return (status);
	}
#endif

    ResourceId DiscoverWorker::doCheckDiscoverStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {

    int childstatus = 0;
    int ret = 0;
    int wexitStatus = 0;
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    const string scriptPath = "/fabos/webtools/htdocs/ws/check_vcenter_response.php";
    //struct timeval tv;
    //time_t sTime, eTime;

    DiscoverDiscoverMessage *dmsg = dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());

    UI32 msgType = dmsg->getMsgType();
    string url = dmsg->getUrl();
    tracePrintf(TRACE_LEVEL_INFO, "DiscoverDiscoverMessage cmdcode = %d",dmsg->getCmdcode());

    if (msgType == DISC_MSG_TYPE_DELETE)
    {
      pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
      return WAVE_MESSAGE_SUCCESS;
    }

    //sTime = tv.tv_sec;
    if (!FileUtils::fileexists(DISCOVERY_CHECK)) {
                DISC_PLUG_DBG_ERR(scriptPath + string(" script does not exist (returning)"));
                //eTime = tv.tv_sec;
                return (WAVE_MESSAGE_ERROR);
            }

            int pid = fork();
            if (pid == 0) {
                if ((ret = chdir(PHPDIR_PATH)) < 0) {
                    DISC_PLUG_DBG_ERR(string("Failed chdir to: ") + string(PHPDIR_PATH));
                }
                else if ((ret = execl(INTERPRETER, INTERPRETER, scriptPath.c_str())) < 0) {
                    DISC_PLUG_DBG_ERR(string("Failed to execute: ") + scriptPath);
                }
                exit(0);
            } else {
               int tpid = wait4(pid, &childstatus, 0, NULL);
                if (tpid == pid) {
                        wexitStatus = WEXITSTATUS(childstatus);
                        if ((wexitStatus == 0))
                        {
                            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
                            return (WAVE_MESSAGE_SUCCESS);
                        }
                        else if (wexitStatus == 1)
                            {
                                  tracePrintf(TRACE_LEVEL_INFO, "Invalid response from vCenter %d", WEXITSTATUS(childstatus));
                                  //eTime = tv.tv_sec;
                                      dmsg->setExitStatus(6);
                                      dmsg->setState(DISC_STATE_DISCOVER_END);
                                      markDiscoverStatusStep (pPrismLinearSequencerContext);
                                      pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
                                      return (WAVE_MESSAGE_ERROR);
                            }
                }
            }
    status = getReturnStatusForExitStatus (status, wexitStatus);
    return (status);
    }
	
	ResourceId
	DiscoverWorker::getReturnStatusForExitStatus (ResourceId status, int exitStatus)
	{
		DISC_PLUG_DBG_FUNC_BEG();
		ResourceId retStatus = status;

		switch (exitStatus)
		{
			case -1:
			case  0:
				break;
			case  1:
				retStatus = DISC_COMMON_ERR_NOT_ACTIVATER;
				break;
			case  2:
				retStatus = DISC_COMMON_ERR_LOST_CONNECTION;
				break;
			case  3:
				retStatus = DISC_COMMON_ERR_BAD_CREDENTIALS;
				break;
			default:
				break;
		}

		DISC_PLUG_DBG_FUNC_END();
		return retStatus;
	}
	
    void DiscoverWorker::incrDiscoverFailureCount (discoverExitStatus_t failureType) 
    {
        DISC_PLUG_DBG_FUNC_BEG();
        
        switch (failureType) {
            case  0:
                break;
            case  1:
                break;
            case  2:
                break;
            case  3:
                m_discInvalidCredCounter++;
                break;
            default:
                break;
        }
        DISC_PLUG_DBG_FUNC_END();
    }
    
    void DiscoverWorker::generateDiscoverFailureRaslog(PrismLinearSequencerContext *pPrismLinearSequencerContext,
                                                    discoverExitStatus_t failureType) 
    {
        DISC_PLUG_DBG_FUNC_BEG();
        DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        string vCenter  = dmsg->getVcenter();
        string userName = dmsg->getUsername();

        switch (failureType) {
            case  0:
                break;
            case  1:
                break;
            case  2:
                break;
            case  3:
                if ((m_discInvalidCredCounter == 1) || (m_discInvalidCredCounter%100 == 0)) {
                    raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID,
                        RASLOG_NOFLAGS, VC_1005, vCenter.c_str(), m_discInvalidCredCounter, userName.c_str());
                }
                break;
            default:
                break;
        }
        
        DISC_PLUG_DBG_FUNC_END();
    }
    
    void DiscoverWorker::resetDiscoverFailureCounters () 
    {
        DISC_PLUG_DBG_FUNC_BEG();
       
        m_discInvalidCredCounter = 0;
        
        DISC_PLUG_DBG_FUNC_END();
    }
    
	ResourceId DiscoverWorker::deleteDiscoverAssets (PrismLinearSequencerContext *pPrismLinearSequencerContext) 
    {
	
		DISC_PLUG_DBG_FUNC_BEG();
		int ret;
    	
		if (!FileUtils::fileexists(DBFILE_ORIG) || !FileUtils::fileexists(INTERPRETER)) {
            DISC_PLUG_DBG(string("vCenter discovery env. does not exist (ignoring)"));
            return (WAVE_MESSAGE_ERROR);
        }
		
        if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
            DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
            return (WAVE_MESSAGE_ERROR);
        }
    	
		DISC_PLUG_DBG_FUNC_END();
        return (WAVE_MESSAGE_SUCCESS);
	}

	ResourceId DiscoverWorker::doDiscoverStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
        DISC_PLUG_DBG_FUNC_BEG();

        ResourceId status = WAVE_MESSAGE_SUCCESS;
		
    	DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
		
        UI32 cmdCode = dmsg->getCmdcode();
        if (cmdCode == DISC_CMD_DISCOVER_TIMER) {
            DISC_PLUG_DBG(string("Timer cmdcode return error"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            DISC_PLUG_DBG_FUNC_END();
            return (WAVE_MESSAGE_ERROR);
        } else if (cmdCode == DISC_CMD_NODE_ADD) {
            DISC_PLUG_DBG(string("Clean the pp on the new node and sync"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            DISC_PLUG_DBG_FUNC_END();
            return (WAVE_MESSAGE_SUCCESS);
        }else if (cmdCode == DISC_CMD_SYNC_PERSISTED_WITH_RUNNING) {
            DISC_PLUG_DBG(string("Sync persisted DB file with Runing on bootup"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            DISC_PLUG_DBG_FUNC_END();
            return (WAVE_MESSAGE_SUCCESS);
        }
        
		UI32 msgType = dmsg->getMsgType();

		switch (msgType) {
			case DISC_MSG_TYPE_CREATE:
			case DISC_MSG_TYPE_UPDATE:
					status = doFreshDiscovery(pPrismLinearSequencerContext);
					break;
			
			case DISC_MSG_TYPE_DELETE:
					status = deleteDiscoverAssets(pPrismLinearSequencerContext);
					break;
		}

		pPrismLinearSequencerContext->executeNextStep(status);
        DISC_PLUG_DBG_FUNC_END();
        return (status);
    }
	
	ResourceId DiscoverWorker::getVirtualIpAddress (string &vIPAddress)
	{
        DISC_PLUG_DBG_FUNC_BEG();
		ResourceId status = WAVE_MESSAGE_ERROR;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
						VirtualIpGlobalManagedObject::getClassName());
		vector<WaveManagedObject *> *pVirtualIpResults = querySynchronously (&syncQueryCtxt);
		
		if (NULL != pVirtualIpResults) {
			UI32 numberOfResults = pVirtualIpResults->size ();
			if (1 == numberOfResults) {
				VirtualIpGlobalManagedObject *pVirtualIpGlobalManagedObject = 
						dynamic_cast<VirtualIpGlobalManagedObject *> ((*pVirtualIpResults)[0]);
				vIPAddress = pVirtualIpGlobalManagedObject->getVipAddress ();
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				DISC_PLUG_DBG_ERR(string("VirtualIpGlobalManagedObject : object count:- ") 
								+ numberOfResults);
			}
		} else {
			DISC_PLUG_DBG_ERR("query for VirtualIpGlobalManagedObject returned NULL.");
		}
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pVirtualIpResults);
        DISC_PLUG_DBG_FUNC_END();
		return status;
	}
	
	void DiscoverWorker::showStatusVector ()
	{
        DISC_PLUG_DBG_FUNC_BEG();

		unsigned int iter;
		unsigned int siter;
		discoverStatusRecord_t *recordTmp = NULL;
		discoverStatus_t *statusTmp = NULL;
		
		for (iter = 0; iter < dStatusRecords.size(); iter++) {
			DISC_PLUG_DBG(iter + string(" iter : size: ") + dStatusRecords.size());
			if (dStatusRecords.size() == 0) {
				DISC_PLUG_DBG(iter + string("dStatusRecords NULL "));
				continue;
			}
			recordTmp = dStatusRecords[iter];
			if (recordTmp == NULL) {
				DISC_PLUG_DBG(iter + string("NULL record "));
				continue;
			}
			DISC_PLUG_DBG(string("vCenter Name: ")+ recordTmp->vCenter);
			
			vector<discoverStatus_t *> &vStatus = recordTmp->status;
			for (siter = 0; siter < vStatus.size(); siter++) {
					statusTmp = vStatus[siter];
					if (statusTmp == NULL) {
						DISC_PLUG_DBG(iter + siter + string("NULL status "));
						continue;
					}
					//DISC_PLUG_DBG(siter + string(" sTime: ")+ statusTmp->sTime);
					//DISC_PLUG_DBG(siter + string(" eTime: ")+ statusTmp->eTime);
			}
		}
        DISC_PLUG_DBG_FUNC_END();
	}
	
	
	ResourceId DiscoverWorker::updateDbDiscoverStatus (DiscoverDiscoverMessage *msg, 
								discoverStages_t dStage, time_t sTime, time_t eTime, int exitStatus)
	{
		char  *table = (char *) "vcenters";
    	char  *colname = (char *) "id, name";
		char  vals[100] = {0};
		char			*updtCols[50];
		char			*updtVals[50];
		int ret;
		char vCenter[100] = {0};
		
		if (msg->getVcenter().length() == 0)
			return (WAVE_MESSAGE_SUCCESS);
		
		strcpy(vCenter, msg->getVcenter().c_str());
		ret = SQ_init();
		DISC_PLUG_DBG(string("Db init: ") + ret);
		
		if (dStage == DISC_STAGE_DISCOVERY_BEGIN) {
			sprintf(vals,"'%s', '%s'", vCenter, vCenter);
			ret = SQ_setRow(table, colname, vals);
			DISC_PLUG_DBG(string("Create New row entry: ") + ret);
		
			memset (vals, 0, sizeof(vals));
			sprintf(vals,"%d", (int)sTime);
			updtCols[0] = strdup("discStartTS");
			updtVals[0] = strdup(vals);
			ret = SQ_updateRow(table, &updtCols[0], &updtVals[0], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover start time: ") + ret);
			
			free(updtCols[0]);
			free(updtVals[0]);
		} else if (dStage == DISC_STAGE_DISCOVERY_SUCCESS) {
			sprintf(vals,"%d", (int)(eTime - sTime));
			updtCols[0] = strdup("discElapTS");
			updtVals[0] = strdup(vals);
			memset (vals, 0, sizeof(vals));
			sprintf(vals,"%d", exitStatus);
			updtCols[1] = strdup("discStatus");
			updtVals[1] = strdup(vals);
			ret = SQ_updateRow(table, &updtCols[0], &updtVals[0], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover elapsed time: ret ") + ret);
			ret = SQ_updateRow(table, &updtCols[1], &updtVals[1], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover status: ret ") + ret);
			
			free(updtCols[0]);
			free(updtVals[0]);
			free(updtCols[1]);
			free(updtVals[1]);
			
		} else if (dStage == DISC_STAGE_DISCOVERY_FAILURE) {
			sprintf(vals,"%d", (int)(eTime - sTime));
			updtCols[0] = strdup("discElapTS");
			updtVals[0] = strdup(vals);
			memset (vals, 0, sizeof(vals));
			sprintf(vals,"%d", exitStatus);
			updtCols[1] = strdup("discStatus");
			updtVals[1] = strdup(vals);
			ret = SQ_updateRow(table, &updtCols[0], &updtVals[0], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover elapsed time: ret ") + ret);
			ret = SQ_updateRow(table, &updtCols[1], &updtVals[1], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover status: ret ") + ret);
			
			free(updtCols[0]);
			free(updtVals[0]);
			free(updtCols[1]);
			free(updtVals[1]);
		}
    	ret = SQ_closedb();
		DISC_PLUG_DBG(string("Db new close: ") + ret);
		return (WAVE_MESSAGE_SUCCESS);
	}

	ResourceId DiscoverWorker::updateDbNewDiscoverStatus (DiscoverDiscoverMessage *msg, 
								discoverStages_t dStage, time_t sTime, time_t eTime, int exitStatus)
	{
		char  *table = (char *) "vcenters";
    	char  *colname = (char *) "id, name";
		char  vals[100] = {0};
		char			*updtCols[50];
		char			*updtVals[50];
		int ret;
		char vCenter[100] = {0};
		
		if (msg->getVcenter().length() == 0)
			return (WAVE_MESSAGE_SUCCESS);
		
		strcpy(vCenter, msg->getVcenter().c_str());
		ret = SQ_initDbnew();
		DISC_PLUG_DBG(string("Db new init: ") + ret);
		
		if (dStage == DISC_STAGE_DISCOVERY_BEGIN) {
			sprintf(vals,"'%s', '%s'", vCenter, vCenter);
			ret = SQ_setRowDbnew(table, colname, vals);
			DISC_PLUG_DBG(string("Create New row entry: ") + ret);
		
			memset (vals, 0, sizeof(vals));
			sprintf(vals,"%d", (int)sTime);
			updtCols[0] = strdup("discStartTS");
			updtVals[0] = strdup(vals);
			ret = SQ_updateRowDbnew(table, &updtCols[0], &updtVals[0], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover start time: ") + ret);
			
			free(updtCols[0]);
			free(updtVals[0]);
		} else if (dStage == DISC_STAGE_DISCOVERY_SUCCESS) {
			sprintf(vals,"%d", (int)(eTime - sTime));
			updtCols[0] = strdup("discElapTS");
			updtVals[0] = strdup(vals);
			memset (vals, 0, sizeof(vals));
			sprintf(vals,"%d", exitStatus);
			updtCols[1] = strdup("discStatus");
			updtVals[1] = strdup(vals);
			ret = SQ_updateRowDbnew(table, &updtCols[0], &updtVals[0], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover elapsed time: ret ") + ret);
			ret = SQ_updateRowDbnew(table, &updtCols[1], &updtVals[1], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover status: ret ") + ret);
			
			free(updtCols[0]);
			free(updtVals[0]);
			free(updtCols[1]);
			free(updtVals[1]);
			
		} else if (dStage == DISC_STAGE_DISCOVERY_FAILURE) {
			sprintf(vals,"%d", (int)(eTime - sTime));
			updtCols[0] = strdup("discElapTS");
			updtVals[0] = strdup(vals);
			memset (vals, 0, sizeof(vals));
			sprintf(vals,"%d", exitStatus);
			updtCols[1] = strdup("discStatus");
			updtVals[1] = strdup(vals);
			ret = SQ_updateRowDbnew(table, &updtCols[0], &updtVals[0], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover elapsed time: ret ") + ret);
			ret = SQ_updateRowDbnew(table, &updtCols[1], &updtVals[1], 1, (char*)"name", vCenter);
			DISC_PLUG_DBG(string("Updated discover status: ret ") + ret);
			
			free(updtCols[0]);
			free(updtVals[0]);
			free(updtCols[1]);
			free(updtVals[1]);
		}
    	ret = SQ_closeDbnew();
		DISC_PLUG_DBG(string("Db new close: ") + ret);
		return (WAVE_MESSAGE_SUCCESS);
	}
	ResourceId DiscoverWorker::updateDiscoverStatus (DiscoverDiscoverMessage *msg, 
								discoverStages_t dStage, time_t sTime, time_t eTime)
	{
		DISC_PLUG_DBG_FUNC_BEG();
		vector<discoverStatusRecord_t *>::iterator dRecordIter;
		vector<discoverStatus_t *>::iterator dStatusIter;
		vector<discoverStatus_t *>::iterator dStatusOld;
		vector<discoverStatus_t *> *dStatus = NULL;
		discoverStatusRecord_t *recordTmp = NULL;
		discoverStatusRecord_t *vRecord = NULL;
		discoverStatus_t *nStatus = NULL;
		
		if (msg->getVcenter().length() == 0)
			return (WAVE_MESSAGE_SUCCESS);

		/* Find status record of the vCenter */
		for (dRecordIter = dStatusRecords.begin(); 
						dRecordIter != dStatusRecords.end(); dRecordIter++) {
			recordTmp = *dRecordIter;
			if (recordTmp->vCenter == msg->getVcenter().c_str()) {
				DISC_PLUG_DBG(string("Record match found"));
				vRecord = recordTmp;
				break;
			}
		}

		/* create new record */
		if (vRecord == NULL) {
			DISC_PLUG_DBG(string("Create New Record"));
			vRecord = new discoverStatusRecord_t;
			vRecord->vCenter = msg->getVcenter().c_str();
			dStatusRecords.push_back(vRecord);
		}
			
		if (dStage == DISC_STAGE_DISCOVERY_BEGIN) {
				
			/* Look if the status entries exceeded the limit */
			dStatus = &(vRecord->status);
			if ((*dStatus).size() == 5) {
				/* erase the old status */
				DISC_PLUG_DBG(string("Delete old Status entry"));
				(*dStatus).erase((*dStatus).begin());
			}
			DISC_PLUG_DBG(string("Create New Status entry"));
			/* create new status entry */
			nStatus = new discoverStatus_t();

			nStatus->sTime = sTime;
			nStatus->eTime = 0;
			nStatus->dStage = dStage;
			(*dStatus).push_back(nStatus);
		} else if (dStage == DISC_STAGE_DISCOVERY_SUCCESS) {
			/* Find the status entry using sTime */
			dStatus = &(vRecord->status);
			for (dStatusIter = (*dStatus).begin(); dStatusIter != (*dStatus).end(); dStatusIter++) {
				if (sTime == (*dStatusIter)->sTime) {
					DISC_PLUG_DBG(string("Status entry match found"));
					nStatus = *dStatusIter;
				}
			}
			nStatus->eTime = eTime;
			nStatus->dStage = dStage;
		} else if (dStage == DISC_STAGE_DISCOVERY_FAILURE) {
			/* Find the status entry using sTime */
			dStatus = &(vRecord->status);
			for (dStatusIter = (*dStatus).begin(); dStatusIter != (*dStatus).end(); dStatusIter++) {
				if (sTime == (*dStatusIter)->sTime) {
					DISC_PLUG_DBG(string("Status entry match found"));
					nStatus = *dStatusIter;
				}
			}
			nStatus->eTime = eTime;
			nStatus->dStage = dStage;
		}
			
		return (WAVE_MESSAGE_SUCCESS);
    }
	
    ResourceId DiscoverWorker::doDbNewDistributionStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        DISC_PLUG_DBG_FUNC_BEG();
        
        string sSourceFileName = DBFILE_NEW;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<LocationId> connectedLocationsVector;
        string sDestFilename = DBFILE_NEW;
        
        FrameworkToolKit::getConnectedLocations(connectedLocationsVector);
        if (connectedLocationsVector.empty()) {
            DISC_PLUG_DBG_SUC(string("LocationIds empty"));
            return (status);
        }

        FilePushFileMessage *pMessage = new FilePushFileMessage(sSourceFileName, 
            sDestFilename,  FrameworkToolKit::getThisLocationId(), connectedLocationsVector);
        pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
        status = sendSynchronously (pMessage);
        if (WAVE_MESSAGE_SUCCESS != status) {
            DISC_PLUG_DBG_ERR(string("Sending message to") + 
                PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + 
                " service] failed, Error [" + FrameworkToolKit::localize(status) + "]\n");
            status = WAVE_MESSAGE_ERROR;
        } else {
            if (WAVE_MESSAGE_SUCCESS != pMessage->getCompletionStatus()) {
                DISC_PLUG_DBG_ERR(string("Response processing: CompletionStatus Reason ") + 
                        FrameworkToolKit::localize(pMessage->getCompletionStatus()));
                status = WAVE_MESSAGE_ERROR;
            } else {
                DISC_PLUG_DBG_SUC(string("Distributed to all nodes"));
            }
        }
        delete pMessage;
        
        DISC_PLUG_DBG_FUNC_END();
        return (status);
    }
    
    ResourceId DiscoverWorker::doFcNodeAddDistributionStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        DISC_PLUG_DBG_FUNC_BEG();
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<LocationId> connectedLocationsVector;
        
        DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        connectedLocationsVector = dmsg->getLocationIds();

        if (connectedLocationsVector.empty()) {
            DISC_PLUG_DBG_SUC(string("LocationIds empty"));
            return (status);
        }
        
        string sSourceFileName = DBFILE;
        string sDestFilename = DBFILE_NEW;
        FilePushFileMessage *pMessage = new FilePushFileMessage(sSourceFileName, 
            sDestFilename,  FrameworkToolKit::getThisLocationId(), connectedLocationsVector);
        pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
        status = sendSynchronously (pMessage);
        if (WAVE_MESSAGE_SUCCESS != status) {
            DISC_PLUG_DBG_ERR(string("Sending message to") + 
                PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + 
                " service] failed, Error [" + FrameworkToolKit::localize(status) + "]\n");
            status = WAVE_MESSAGE_ERROR;
        } else {
            if (WAVE_MESSAGE_SUCCESS != pMessage->getCompletionStatus()) {
                DISC_PLUG_DBG_ERR(string("Response processing: CompletionStatus Reason ") + 
                        FrameworkToolKit::localize(pMessage->getCompletionStatus()));
                status = WAVE_MESSAGE_ERROR;
            } else {
                DISC_PLUG_DBG_SUC(string("dc.db distributed to new nodes as dc.db.new"));
            }
        }
        delete pMessage;
        
        DISC_PLUG_DBG_FUNC_END();
        return (status);
    }

    ResourceId DiscoverWorker::doDistributionStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        DISC_PLUG_DBG_FUNC_BEG();
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        /* No operation on SA */
        ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
        if (vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE) {
            DISC_PLUG_DBG_SUC(string("Standalone Mode"));
            DISC_PLUG_DBG_FUNC_END();
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return (WAVE_MESSAGE_SUCCESS);
        }
        
        DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        UI32 cmdCode = dmsg->getCmdcode();
        if (cmdCode == DISC_CMD_SYNC_PERSISTED_WITH_RUNNING) {
			DISC_PLUG_DBG(string("Sync persisted DB file with Runing on bootup"));
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			DISC_PLUG_DBG_FUNC_END();
			return (WAVE_MESSAGE_SUCCESS);
         }
        ResourceId distributionMode = DcmToolKit::getDistributionMode();
        /* FC on Node ADD */
        if((distributionMode == CCM_ONLY)&&(cmdCode == DISC_CMD_NODE_ADD)) {
            status = doFcNodeAddDistributionStep(pPrismLinearSequencerContext);
            /* FC and MC cluster wide dc.db.new distribution */
        } else {
            status = doDbNewDistributionStep(pPrismLinearSequencerContext);
        }
        
        DISC_PLUG_DBG_FUNC_END();
        pPrismLinearSequencerContext->executeNextStep(status);
        return (status);
    }
	
	void
	DiscoverWorker::sendMessageToLocalPluginCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
		DISC_PLUG_DBG_FUNC_BEG();
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
	    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());
		if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS) {
			DISC_PLUG_DBG(string("Phase 1 message succeeded on all locations"));
		} else if(pPrismLinearSequencerContext->getCompletionStatus() == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES) {
			DISC_PLUG_DBG(string("Phase 1 message failed on all locations"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED) {
			DISC_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback succeeded on all of the nodes"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_FAILED) {
			DISC_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback attempt on the succeeded nodes failed on some or all nodes"));
		} else {
			DISC_PLUG_DBG(string("CompletionStatus::Unknown Error:- ") + FrameworkToolKit::localize(sendToClusterCompletionStatus));
		}
        
        DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());
        
        /* State Machine Update */
        dmsg->setState(DISC_STATE_PPCONFIG_COPY_END);
        markDiscoverStatusStep (pPrismLinearSequencerContext);

        delete pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete pWaveSendToClusterContext;
		pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
		DISC_PLUG_DBG_FUNC_END();
        return;
     }
	
    void
	DiscoverWorker::sendMessageToLocalPluginStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		DISC_PLUG_DBG_FUNC_BEG();
    	DiscoverDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverDiscoverMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());

        /* State Machine Update */
        dmsg->setState(DISC_STATE_PPCONFIG_COPY_START);
        markDiscoverStatusStep (pPrismLinearSequencerContext);

        DiscoverLocalDiscoverMessage *m = new DiscoverLocalDiscoverMessage();
		WaveSendToClusterContext    *pWaveSendToClusterContext;
		
		/* create clustercontext and initialize with callback func */
		pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&DiscoverWorker::sendMessageToLocalPluginCallback), pPrismLinearSequencerContext);

        /* Read the header from received message and populate 
		 * sending message
		 */
        m->setMsgType (dmsg->getMsgType());
        m->setCmdcode (dmsg->getCmdcode());

        /* Read the data from received message and populate 
		 * sending message
		 */
		m->setVcenter (dmsg->getVcenter());
		
		/* set scripts names to be executed in local plugin */
		m->setScriptNames (dmsg->getLocalScriptNames());
        
		/* Set Phase 1 message */
		pWaveSendToClusterContext->setPPrismMessageForPhase1(m);

        /* Set Location Ids for node add */
        if ((dmsg->getCmdcode() == DISC_CMD_NODE_ADD)||(dmsg->getCmdcode() == DISC_CMD_SYNC_PERSISTED_WITH_RUNNING)){
			DISC_PLUG_DBG(string("Special handling for locations"));
			if(dmsg->getLocationIds().size()>0){
				pWaveSendToClusterContext->setLocationsToSendToForPhase1(dmsg->getLocationIds());
			}
        }
        
		/* Enable partial success flag */
		pWaveSendToClusterContext->setPartialSuccessFlag(true);
		/* Enable flag to treat msg to failing over node as success */
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
		/* Send the message to all nodes in cluster */
        sendToWaveCluster (pWaveSendToClusterContext);

        dmsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        return;

    }
	
	VcenterManagedObject*
	DiscoverWorker::getVcenterConfigFromMo ()
	{
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VcenterManagedObject::getClassName());
        //syncQueryCtxt.addAndAttribute (new AttributeString(&vcenterName,"id"));
    	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		VcenterManagedObject* pMO = NULL;
        
		if (NULL != pResults) {
	        UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
	        } else if (1 == numberOfResults) {
				pMO = dynamic_cast<VcenterManagedObject*>((*pResults)[0]);
            } else {
				 /* Managed object inconsistent by having multiple entries for
				  * same key
				  */
			}
        }
		pResults->clear();
		delete pResults;
		return pMO;
	}
	
	void DiscoverWorker::discoverTimerHandlerCallback (TimerHandle timerHandle, void * pContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();
		string vnames;

		if (pDiscoverStartDiscoverTimer->getTimerHandle() == timerHandle) {
			DISC_PLUG_DBG(string("Timer handles are same. Handle ") + timerHandle);
		} else {
			DISC_PLUG_DBG(string("Timer handles are not same. Received Handle ") + timerHandle
						+ string(" Original Handle ") + pDiscoverStartDiscoverTimer->getTimerHandle());
		}
		
		/* 
		 * Handle is already got deleted on timer expiry,
		 * set the local handle value to zero.
		 */
		 pDiscoverStartDiscoverTimer->setTimerHandle(0);

		VcenterManagedObject* pVcenterManagedObject = getVcenterConfigFromMo();
		
		/* create new discover message and do async send (to the same OM) */
		DiscoverDiscoverMessage *dmsg = new DiscoverDiscoverMessage();
		
        if (pVcenterManagedObject) {
            if((pVcenterManagedObject->getActivate() == true)) {
                vnames = pVcenterManagedObject->getId();
                dmsg->setVcenter(vnames);
                dmsg->setUrl(pVcenterManagedObject->getIpaddress());
                dmsg->setUsername(pVcenterManagedObject->getUsername());
                dmsg->setPassword(pVcenterManagedObject->getPassword());
                dmsg->setGlobalScriptNames(DISC_GLOBAL_PLUGIN_SCRIPTS);
                dmsg->setLocalScriptNames(DISC_LOCAL_PLUGIN_SCRIPTS);
                dmsg->setCmdcode(DISC_CMD_DISCOVER_ALL_VCENTER);
                DISC_PLUG_DBG(string("Config available and activated in MO for vcenter ") + vnames);
            } else {
                dmsg->setCmdcode(DISC_CMD_DISCOVER_TIMER);
                DISC_PLUG_DBG(string("Config available, but deactivated in MO for vcenter ") + vnames 
                    + string(" Restart Timer"));
            }
            delete pVcenterManagedObject;
        } else {
            dmsg->setCmdcode(DISC_CMD_DISCOVER_TIMER);
            DISC_PLUG_DBG(string("Vcenter config not available in MO, Restart Timer"));
        }
        
        /* if vnames is (none) or deactivated, the msg will restart Timer */
        sendOneWay(dmsg);
		
		DISC_PLUG_DBG_FUNC_END();

	}

	/* stop the timer if it is not expired */
	ResourceId DiscoverWorker::stopDiscoverTimerStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

		ResourceId status;
		
		/* Handle will be zero, if the time has already expired */
		if (pDiscoverStartDiscoverTimer->getTimerHandle() != 0) {
			status = deleteTimer (pDiscoverStartDiscoverTimer->m_timerHandle);
			if (status == FRAMEWORK_SUCCESS) {
				DISC_PLUG_DBG_SUC(string("Delete Timer, handle ") + 
								pDiscoverStartDiscoverTimer->m_timerHandle);
			} else {
				DISC_PLUG_DBG_ERR(string("Delete Timer"));
			}
			pDiscoverStartDiscoverTimer->setTimerHandle(0);
		}
		
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
		DISC_PLUG_DBG_FUNC_END();
        return (WAVE_MESSAGE_SUCCESS);
	}
	
	ResourceId DiscoverWorker::startDiscoverTimerStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();
		ResourceId status;
        int interval = 0;
        
        VcenterManagedObject* pVcenterManagedObject = getVcenterConfigFromMo();
		
        if (pVcenterManagedObject) {
            interval = MIN_TO_MILLISEC(pVcenterManagedObject->getInterval());
            DISC_PLUG_DBG(string("Timer interval is converted to milli sec: ") + interval);
            delete pVcenterManagedObject;
        }
        
        if (interval == 0) {
            DISC_PLUG_DBG(string("Timer Interval is 0, do not restart Timer"));
            DISC_PLUG_DBG_FUNC_END();
            return (WAVE_MESSAGE_SUCCESS);
        }
        
        pDiscoverStartDiscoverTimer->setTimerInterval(interval);
        
		/* Start the timer */
		status = startTimer (pDiscoverStartDiscoverTimer->m_timerHandle, 
			pDiscoverStartDiscoverTimer->m_timerInterval, 
			reinterpret_cast<PrismTimerExpirationHandler> (&DiscoverWorker::discoverTimerHandlerCallback));
		
		if (status == FRAMEWORK_SUCCESS) {
			DISC_PLUG_DBG_SUC(string("Start Timer, Handle ") + pDiscoverStartDiscoverTimer->getTimerHandle());
		} else {
			DISC_PLUG_DBG_ERR(string("Start Timer"));
		}
		
		
		DISC_PLUG_DBG_FUNC_END();
        return (WAVE_MESSAGE_SUCCESS);
    }
    
void  DiscoverWorker::DiscoverStatusMessageHandler( DiscoverStatusMessage *pDiscoverStatusMessage)
    {
        trace(TRACE_LEVEL_INFO, string(__FUNCTION__));
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiscoverWorker::getDiscoverStatusStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiscoverWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiscoverWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
            new PrismSynchronousLinearSequencerContext (pDiscoverStatusMessage,
                                             this, sequencerSteps,
                                             sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
	
	ResourceId DiscoverWorker::getDiscoverStatusStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();
    	DiscoverStatusMessage *msg = 
            dynamic_cast<DiscoverStatusMessage*>(pPrismLinearSequencerContext->getPPrismMessage ());

		showStatusVector();
		//int nRecords;
		int i;
		vector<discoverStatusRecord_t *>::iterator dRecordIter;
		vector<discoverStatus_t *>::iterator vStatusIter;
		discoverStatusRecord_t *recordTmp = NULL;
		discoverStatusRecord_t *matchedRecord = NULL;
		discoverStatus_t *statusTmp = NULL;
		discoverShowStatusRecord_t *dataRecord;
		discoverShowStatusResp_t *data_p = new discoverShowStatusResp_t;

		if (msg->getCmdcode() == DISC_CMD_SHOW_STATUS_ALL_VCENTER) {
			//nRecords = dStatusRecords.size();
			/* TODO for later release */
		
		} else if (msg->getCmdcode() == DISC_CMD_SHOW_VCENTER_INTERNAL_HIST) {
			for (dRecordIter = dStatusRecords.begin(); dRecordIter != dStatusRecords.end(); dRecordIter++) {
				recordTmp = *dRecordIter;
				if (recordTmp->vCenter == msg->getVcenter().c_str()) {
					matchedRecord = recordTmp;
					DISC_PLUG_DBG(string("Matched Record found for ") + msg->getVcenter().c_str());
					break;
				}
			}
			
			data_p->len = sizeof(discoverShowStatusResp_t);
			data_p->nRecords = 1;
			if (matchedRecord == NULL) {
				data_p->retCode = DISC_MGMT_ERR_UNKNOWN_VCENTER;
				DISC_PLUG_DBG(string("Matched Record couldn't find for ") + 
								msg->getVcenter().c_str());
			} else {
				data_p->retCode = DISC_MGMT_ERR_SUCCESS;
				dataRecord = &(data_p->records[0]);
				
				dataRecord->vCenter = matchedRecord->vCenter;
				vector<discoverStatus_t *> &matchedRecStat = matchedRecord->status;
				for (vStatusIter = matchedRecStat.begin(), i=0; vStatusIter != matchedRecStat.end(); 
															vStatusIter++, i++) {
					statusTmp = *vStatusIter;
					dataRecord->status[i].sTime = statusTmp->sTime;
					dataRecord->status[i].eTime = statusTmp->eTime;
					dataRecord->status[i].dStage = statusTmp->dStage;

				}
				dataRecord->nStatus = i;
				
			}
		}
				
		msg->addBuffer(msg->getCmdcode(), data_p->len, (unsigned char *)data_p, false);
        
		return (WAVE_MESSAGE_SUCCESS);
    }
	
    void DiscoverWorker::nodeReadyForAllCommands (const NodeReadyForAllCommandsEvent* &pEvent)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DiscoverManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        DiscoverManagedObject* pMO = NULL;
        
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                DISC_PLUG_DBG(string("MO doesn't exist"));
            } else if (0 < numberOfResults) {
                for (UI32 i=0; i<numberOfResults; i++) {
                    pMO = dynamic_cast<DiscoverManagedObject*>((*pResults)[i]);
                    DISC_PLUG_DBG(string("Found Status Managed Object for ") + pMO->getId());
                    startTransaction();
                    delete pMO;
                    status = commitTransaction ();

                    if (FRAMEWORK_SUCCESS != status)
                    {
                        // FIXME : sagar : Please handle this condition.
                    }
                }
            }
        }
        pResults->clear();
        delete pResults;
        
        /* Event reply to framework */
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
        
    }
	
    void DiscoverWorker::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {

        listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (),
                        FRAMEWORK_OBJECT_MANAGER_NODE_READY_FOR_ALL_COMMANDS_EVENT,
                        reinterpret_cast<PrismEventHandler> (&DiscoverWorker::nodeReadyForAllCommands)
            );
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }
    
}
