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
 *   Author : sharis                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "discover/Framework/discoverLocalObjectManager.h"
#include "discover/User/discoverLocalWorkers.h"
#include "discover/Framework/discoverLocalMessages.h"
#include "discover/Framework/discoverLocalTypes.h"
#include "discover/User/discoverPluginDebug.h"
#include "discover/User/discoverDcmDef.h"
#include "DcmResourceIdEnums.h"
#include "discover/utils/FileUtils.h"

#include <sys/fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "vcs.h"

#include <sstream>

#define DBFILE          ((char *) "/tmp/dc.db")
#define DBFILE_NEW      ((char *) "/tmp/dc.db.new")
#define PER_DBFILE      ((char *) "/var/tmp/dc.db")
#define PER_DBFILE_NEW  ((char *) "/var/tmp/dc.db.new")
#define DEBUG_CONF_TMP  ((char *) "/tmp/vcdebug_config.php")
//#define DBFILE_ORIG     ((char *) "/fabos/webtools/htdocs/ws/config/dc.db")
#define PHPDIR_PATH     ((char *) "/fabos/webtools/htdocs/ws")
#define INTERPRETER     ((char *) "/usr/apache/php")
#define DISCOVER_SCRIPT ((char *) "/fabos/webtools/htdocs/ws/discover.php")
#define PPCONFIG_SCRIPT ((char *) "/fabos/webtools/htdocs/ws/pp_create.php")
#define DBRENAME_SCRIPT ((char *) "/fabos/webtools/htdocs/ws/copy.php")
#define DEBUG_CONF_DEFAULT ((char *) "/fabos/webtools/htdocs/ws/config/vcdebug_config.php")

#define PHP_CMD_CODE_DISCOVER_VCENTER       1
#define PHP_CMD_CODE_DISCOVER_ALL_VCENTER   2
#define PHP_CMD_CODE_NODE_ADD               3
#define PHP_CMD_NODE_COPY_RUNNING_TO_STARTUP   4
#define PHP_CMD_SYNC_PERSISTED_WITH_RUNNING    5
		
using namespace std;





namespace DcmNs
{
    DiscoverLocalWorker::DiscoverLocalWorker ( DiscoverLocalObjectManager *pDiscoverLocalObjectManager)
        : WaveWorker  (pDiscoverLocalObjectManager)
    {
        addOperationMap (DISCOVERLOCALDISCOVER, reinterpret_cast<PrismMessageHandler> (&DiscoverLocalWorker::DiscoverLocalDiscoverMessageHandler));
       
		//On boot-up pick up the persisted dc.db abd dc.db.new files
        if (FileUtils::fileexists(PER_DBFILE) &&(!FileUtils::fileexists(DBFILE))) {
            if ((FileUtils::copyfile(PER_DBFILE, DBFILE)) < 0) {
                DISC_PLUG_DBG_ERR(string("Failed copy: ") + string(PER_DBFILE) + " -> " + string(DBFILE));
            } else {
                DISC_PLUG_DBG_SUC(string("copy: ") + string(PER_DBFILE) + " -> " + string(DBFILE));
            }
        }
         
        if (FileUtils::fileexists(PER_DBFILE_NEW) && (!FileUtils::fileexists(DBFILE_NEW))) {
            if ((FileUtils::copyfile(PER_DBFILE_NEW, DBFILE_NEW)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(PER_DBFILE_NEW) + " -> " + string(DBFILE_NEW));
            } else {
                DISC_PLUG_DBG_SUC(string("copy: ") + string(PER_DBFILE_NEW) + " -> " + string(DBFILE_NEW));
            }
        }
        if (!FileUtils::fileexists(DBFILE)) {
            if ((FileUtils::copyfile(DBFILE_ORIG, DBFILE)) < 0) {
                DISC_PLUG_DBG_ERR(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
            } else {
                DISC_PLUG_DBG_SUC(string("copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
            }
        }
        
        if (!FileUtils::fileexists(DBFILE_NEW)) {
            if ((FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
            } else {
                DISC_PLUG_DBG_SUC(string("copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
            }
        }
        
        if (!FileUtils::fileexists(DEBUG_CONF_TMP)) {
            if ((FileUtils::copyfile(DEBUG_CONF_DEFAULT, DEBUG_CONF_TMP)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DEBUG_CONF_DEFAULT) + " -> " + string(DEBUG_CONF_TMP));
            } else {
                DISC_PLUG_DBG_SUC(string("copy: ") + string(DEBUG_CONF_DEFAULT) + " -> " + string(DEBUG_CONF_TMP));
            }
        }
    }

    DiscoverLocalWorker::~DiscoverLocalWorker ()
    {
    }

    PrismMessage  *DiscoverLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case DISCOVERLOCALDISCOVER :
                pPrismMessage = new DiscoverLocalDiscoverMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DiscoverLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        trace(TRACE_LEVEL_INFO, string(__FUNCTION__));
        return NULL;
    }

    void DiscoverLocalWorker::extractScriptNames (PrismSynchronousLinearSequencerContext
                    *pPrismSynchronousLinearSequencerContext, vector<string> &words) {
		
		DISC_PLUG_DBG_FUNC_BEG();

		DiscoverLocalDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverLocalDiscoverMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		string scriptNames = dmsg->getScriptNames();
        stringstream ssNames(scriptNames);
        string word;
        while (ssNames >> word) {
            words.push_back(word);
        }
		
		DISC_PLUG_DBG_FUNC_END();
    }


    void  DiscoverLocalWorker::DiscoverLocalDiscoverMessageHandler( DiscoverLocalDiscoverMessage *pDiscoverLocalDiscoverMessage)
    {
        trace(TRACE_LEVEL_INFO, string(__FUNCTION__));
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiscoverLocalWorker::doPhpScriptsTriggerStep),
//            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiscoverLocalWorker::doPPConfigStep),
//            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiscoverLocalWorker::doDBRenameStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiscoverLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiscoverLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
            new PrismSynchronousLinearSequencerContext (pDiscoverLocalDiscoverMessage,
                                             this, sequencerSteps,
                                             sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

#ifdef USE_POPEN
	ResourceId DiscoverLocalWorker::doPhpScriptsTriggerStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

        int ret;
        FILE *fp;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		vector<string> scripts;
		vector<string>::size_type i;
	    
		/* Extract php script names */
		extractScriptNames(pPrismSynchronousLinearSequencerContext, scripts);
    	
		DiscoverLocalDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverLocalDiscoverMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		
		ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
		LocationRole locationRole = FrameworkToolKit::getThisLocationRole();
			
		if (!FileUtils::fileexists(DBFILE_ORIG)||!FileUtils::fileexists(INTERPRETER)) {
				DISC_PLUG_DBG_ERR(string(" discover env. does not exist"));
				dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
				return (WAVE_MESSAGE_ERROR);
			}
		
		if (!FileUtils::fileexists(DBFILE)) {
            if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
                return (WAVE_MESSAGE_ERROR);
            }
        } 

		if (!FileUtils::fileexists(DBFILE_NEW)) {
        	if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
            	DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
    	        return (WAVE_MESSAGE_ERROR);
        	}
		}

		for (i = 0; i != scripts.size(); ++i) {
			DISC_PLUG_DBG(string("Handling request to invoke ") + scripts[i]);
			if ((scripts[i] == "pp_create") && 
				(vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) && 
				(locationRole != LOCATION_PRIMARY)) {
           		DISC_PLUG_DBG_SUC(string("MGMT Cluster non-Primary node"));
				continue;
			}

			string scriptPath = "/fabos/webtools/htdocs/ws/" + scripts[i] + ".php";
			
			if (!FileUtils::fileexists((char *)&scriptPath[0])) {
				DISC_PLUG_DBG_ERR(scripts[i] + string(" script does not exist"));
				dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
				return (WAVE_MESSAGE_ERROR);
			}
			
			string cmd = string("(cd ") + string(PHPDIR_PATH) + ";" +
            	string(INTERPRETER) + string(" ") + scriptPath + 
				string(" ") + dmsg->getVcenter() + ";)";
			
			DISC_PLUG_DBG(string(cmd));
			if ((fp = popen(cmd.c_str(), "r")) == NULL) {
				DISC_PLUG_DBG_ERR(scripts[i] + string(" failed"));
				status = WAVE_MESSAGE_ERROR;
			} else {
				DISC_PLUG_DBG_SUC(scripts[i] + string(" done"));
				status = WAVE_MESSAGE_SUCCESS;
				pclose(fp);
			}
		}
		DISC_PLUG_DBG_FUNC_END();
		dmsg->setCompletionStatus(status);
		return (status);
    }

#else
	ResourceId DiscoverLocalWorker::doPhpScriptsTriggerStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

        int ret, pid, tpid, childstatus = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		vector<string> scripts;
		vector<string>::size_type i;
    
    	DiscoverLocalDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverLocalDiscoverMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        
        if (!(FrameworkToolKit::isNodeReadyForAllCommands ())) {
            DISC_PLUG_DBG (string("Node not ready for commands "));
            dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
            return (WAVE_MESSAGE_ERROR);
        } 

		ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
		LocationRole locationRole = FrameworkToolKit::getThisLocationRole();
		ResourceId distributionMode = DcmToolKit::getDistributionMode();

		string mode = ((vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE)? "Standalone" :
	            ((vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC) ? "Fabric Cluster" :
				((vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) ? "Managed Cluster":"Unknown")));
		string role = ((locationRole == LOCATION_PRIMARY) ? "Primary" :
				((locationRole == LOCATION_SECONDARY) ? "Secondary":"Unknown"));
		
        string dmode = ((distributionMode == DCM_ONLY)? "DCM_ONLY" :
	            ((distributionMode == CCM_ONLY) ? "CCM_ONLY" :
	            ((distributionMode == DCM_CCM) ?  "DCM_CCM" :
				((distributionMode == CCM_NOOP) ? "CCM_NOOP":"Unknown"))));
		
		DISC_PLUG_DBG(string("Node is in ") + mode + string(" mode"));
		DISC_PLUG_DBG(string("Node is in ") + role + string(" role"));
		DISC_PLUG_DBG(string("Node is in ") + dmode + string(" distribution mode"));
			
		if (!FileUtils::fileexists(DBFILE_ORIG) || !FileUtils::fileexists(INTERPRETER)) {
			DISC_PLUG_DBG_ERR(string("discover env. does not exist"));
			dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
			return (WAVE_MESSAGE_ERROR);
		}
		
		if (!FileUtils::fileexists(DBFILE)) {
            if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
                return (WAVE_MESSAGE_ERROR);
            }
        } 

		if (!FileUtils::fileexists(DBFILE_NEW)) {
        	if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
            	DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
    	        return (WAVE_MESSAGE_ERROR);
        	}
		}
        UI32 cmd = dmsg->getCmdcode();
        /* Extract php script names */
		extractScriptNames(pPrismSynchronousLinearSequencerContext, scripts);
       
        /* Pass cmdCode for special handling of node add in the script */

        stringstream ss;
        if (cmd == DISC_CMD_NODE_ADD) {
            ss << PHP_CMD_CODE_NODE_ADD;
        } else if(cmd == DISC_CMD_NODE_COPY_RUNNING_TO_STARTUP){
        	ss << PHP_CMD_NODE_COPY_RUNNING_TO_STARTUP;
        }else if(cmd == DISC_CMD_SYNC_PERSISTED_WITH_RUNNING){
        	ss << PHP_CMD_SYNC_PERSISTED_WITH_RUNNING;
        }
        	else {
               ss << PHP_CMD_CODE_DISCOVER_VCENTER;
        }
        string cmdCode = ss.str();
        DISC_PLUG_DBG(string("PHP Command Code ") + cmdCode);
        
        /* Pass msgType for enable/disable cdp in the script */
        UI32 msg = dmsg->getMsgType();
        stringstream msgTypeSs;
        msgTypeSs << msg;
        string msgType = msgTypeSs.str();
        DISC_PLUG_DBG(string("PHP Msg Type ") + msgType);
        
		/* invoke php scripts */
		for (i = 0; i != scripts.size(); ++i) {
			DISC_PLUG_DBG(string("Handling request to invoke ") + scripts[i]);
			if ((scripts[i] == DISC_PPCONFIG_SCRIPTS) &&
                (distributionMode == DCM_CCM) &&
				(vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) && 
				(locationRole != LOCATION_PRIMARY)) {
           		DISC_PLUG_DBG_SUC(string("MGMT Cluster non-Primary node"));
				continue;
			}

			const string scriptPath = "/fabos/webtools/htdocs/ws/" + scripts[i] + ".php";
			
			if (!FileUtils::fileexists(INTERPRETER) || !FileUtils::fileexists((char *)&scriptPath[0])) {
				DISC_PLUG_DBG_ERR(scripts[i] + string(" script does not exist (ignoring)"));
				dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
				return (WAVE_MESSAGE_ERROR);
			}
			
			pid = fork();
			if (pid == 0) {
				if ((ret = chdir(PHPDIR_PATH)) < 0) {
					DISC_PLUG_DBG(string("Failed chdir to: ") + string(PHPDIR_PATH));
				}
				else if ((ret = execl(INTERPRETER, INTERPRETER, scriptPath.c_str(), 
                                dmsg->getVcenter().c_str(), cmdCode.c_str(),  msgType.c_str(), NULL)) < 0) {
					DISC_PLUG_DBG(string("Failed to execute: ") + scriptPath);
				}
				exit(0);
			} else {
				tpid = wait4(pid, &childstatus, 0, NULL);
				if (tpid == pid) {
					if (WEXITSTATUS(childstatus) == 0) {
						status = WAVE_MESSAGE_SUCCESS;
						DISC_PLUG_DBG_SUC(scripts[i] + string(" succeeded"));
					} else {
						status = WAVE_MESSAGE_ERROR;
						DISC_PLUG_DBG_ERR(scripts[i] + string(" failed (check logs)"));
					}
				}
			}
		}
		
		DISC_PLUG_DBG_FUNC_END();
        dmsg->setCompletionStatus(status);
        return (status);
    }
#endif
#if 0
#ifdef USE_POPEN
	ResourceId DiscoverLocalWorker::doPPConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

        int ret;
        FILE *fp;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

    	DiscoverLocalDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverLocalDiscoverMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
		LocationRole locationRole = FrameworkToolKit::getThisLocationRole();
           	
		if ((vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) && (locationRole != LOCATION_PRIMARY)) {
           	DISC_PLUG_DBG_SUC(string("MGMT Cluster non-Primary node"));
        	dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
			return (WAVE_MESSAGE_ERROR);
		}
        
		if (!FileUtils::fileexists(DBFILE_NEW) || !FileUtils::fileexists(INTERPRETER) || !FileUtils::fileexists(PPCONFIG_SCRIPT)) {
           	DISC_PLUG_DBG_ERR(string("ppconfig env. does not exist (ignoring)"));
        	dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
            return (WAVE_MESSAGE_ERROR);
        }
        
		if (!FileUtils::fileexists(DBFILE)) {
            if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
                return (WAVE_MESSAGE_ERROR);
            }
        } 

		if (!FileUtils::fileexists(DBFILE_NEW)) {
        	if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
            	DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
    	        return (WAVE_MESSAGE_ERROR);
        	}
		}

        string cmd = string("(cd ") + string(PHPDIR_PATH) + ";" +
            string(INTERPRETER) + string(" ") + string(PPCONFIG_SCRIPT) + 
			string(" ") + dmsg->getVcenter() + ";)";

        DISC_PLUG_DBG(string(cmd));
        if ((fp = popen(cmd.c_str(), "r")) == NULL) {
            DISC_PLUG_DBG_ERR(string("ppconfig failed: "));
            status = WAVE_MESSAGE_ERROR;
        } else {
            DISC_PLUG_DBG_SUC(string("ppconfig done"));
            status = WAVE_MESSAGE_SUCCESS;
            pclose(fp);
        }
		DISC_PLUG_DBG_FUNC_END();
        dmsg->setCompletionStatus(status);
        return (status);
    }

#else
	ResourceId DiscoverLocalWorker::doPPConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

        int ret, pid, tpid, childstatus = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

    	DiscoverLocalDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverLocalDiscoverMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

		ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
		LocationRole locationRole = FrameworkToolKit::getThisLocationRole();

		string mode = ((vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE)? "Standalone" :
	            ((vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC) ? "Fabric Cluster" :
				((vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) ? "Managed Cluster":"Unknown")));
		string role = ((locationRole == LOCATION_PRIMARY) ? "Primary" :
				((locationRole == LOCATION_SECONDARY) ? "Secondary":"Unknown"));
           	
		DISC_PLUG_DBG(string("Node is in ") + mode + string(" mode"));
		DISC_PLUG_DBG(string("Node is in ") + role + string(" role"));
		
		if ((vcsMode == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) && (locationRole != LOCATION_PRIMARY)) {
           	DISC_PLUG_DBG_SUC(string("MGMT Cluster non-Primary node"));
        	dmsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			return (WAVE_MESSAGE_SUCCESS);
		}
        if (!FileUtils::fileexists(DBFILE_NEW) || !FileUtils::fileexists(INTERPRETER) || !FileUtils::fileexists(PPCONFIG_SCRIPT)) {
            DISC_PLUG_DBG(string("ppconfig env. does not exist (ignoring)"));
        	dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
            return (WAVE_MESSAGE_ERROR);
        }
		
		if (!FileUtils::fileexists(DBFILE)) {
            if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
                return (WAVE_MESSAGE_ERROR);
            }
        } 

		if (!FileUtils::fileexists(DBFILE_NEW)) {
        	if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
            	DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
    	        return (WAVE_MESSAGE_ERROR);
        	}
		}
        
		pid = fork();
        if (pid == 0) {
            if ((ret = chdir(PHPDIR_PATH)) < 0) {
                DISC_PLUG_DBG(string("Failed chdir to: ") + string(PHPDIR_PATH));
            }
            else if ((ret = execl(INTERPRETER, INTERPRETER, PPCONFIG_SCRIPT, dmsg->getVcenter().c_str(), NULL)) < 0) {
                DISC_PLUG_DBG(string("Failed to execute: ") + PPCONFIG_SCRIPT);
            }
            exit(0);
        } else {
            tpid = wait4(pid, &childstatus, 0, NULL);
            if (tpid == pid) {
                if (WEXITSTATUS(childstatus) == 0) {
                    status = WAVE_MESSAGE_SUCCESS;
                    DISC_PLUG_DBG_SUC(string("ppconfig succeeded"));
                } else {
                    status = WAVE_MESSAGE_ERROR;
                    DISC_PLUG_DBG_ERR(string("ppconfig failed (check logs)"));
                }
            }
        }
		DISC_PLUG_DBG_FUNC_END();
        dmsg->setCompletionStatus(status);
        return (status);
    }
#endif

#ifdef USE_POPEN
	ResourceId DiscoverLocalWorker::doDBRenameStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

        int ret;
        FILE *fp;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

    	DiscoverLocalDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverLocalDiscoverMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        
		if (!FileUtils::fileexists(DBFILE_NEW) || !FileUtils::fileexists(INTERPRETER) || !FileUtils::fileexists(DBRENAME_SCRIPT)) {
           	DISC_PLUG_DBG(string("dbrename env. does not exist (ignoring)"));
        	dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
            return (WAVE_MESSAGE_ERROR);
        }
		
		if (!FileUtils::fileexists(DBFILE)) {
            if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
                return (WAVE_MESSAGE_ERROR);
            }
        } 

		if (!FileUtils::fileexists(DBFILE_NEW)) {
        	if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
            	DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
    	        return (WAVE_MESSAGE_ERROR);
        	}
		}

        string cmd = string("(cd ") + string(PHPDIR_PATH) + ";" +
            string(INTERPRETER) + string(" ") + string(DBRENAME_SCRIPT) + 
			string(" ") + dmsg->getVcenter() + ";)";

        DISC_PLUG_DBG(string(cmd));
        if ((fp = popen(cmd.c_str(), "r")) == NULL) {
            DISC_PLUG_DBG_ERR(string("ppconfig failed: "));
            status = WAVE_MESSAGE_ERROR;
        } else {
            DISC_PLUG_DBG_SUC(string("ppconfig done"));
            status = WAVE_MESSAGE_SUCCESS;
            pclose(fp);
        }
		DISC_PLUG_DBG_FUNC_END();
        dmsg->setCompletionStatus(status);
        return (status);
    }

#else
	ResourceId DiscoverLocalWorker::doDBRenameStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		DISC_PLUG_DBG_FUNC_BEG();

        int ret, pid, tpid, childstatus = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

    	DiscoverLocalDiscoverMessage *dmsg = 
            dynamic_cast<DiscoverLocalDiscoverMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        if (!FileUtils::fileexists(DBFILE_NEW) || !FileUtils::fileexists(INTERPRETER) || !FileUtils::fileexists(DBRENAME_SCRIPT)) {
            DISC_PLUG_DBG(string("DBRENAME env. does not exist (ignoring)"));
        	dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
            return (WAVE_MESSAGE_ERROR);
        }
		if (!FileUtils::fileexists(DBFILE)) {
            if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE)) < 0) {
                DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
                return (WAVE_MESSAGE_ERROR);
            }
        } 

		if (!FileUtils::fileexists(DBFILE_NEW)) {
        	if ( (ret = FileUtils::copyfile(DBFILE_ORIG, DBFILE_NEW)) < 0) {
            	DISC_PLUG_DBG(string("Failed copy: ") + string(DBFILE_ORIG) + " -> " + string(DBFILE_NEW));
        		dmsg->setCompletionStatus(WAVE_MESSAGE_ERROR);
    	        return (WAVE_MESSAGE_ERROR);
        	}
		}
        
		pid = fork();
        if (pid == 0) {
            if ((ret = chdir(PHPDIR_PATH)) < 0) {
                DISC_PLUG_DBG(string("Failed chdir to: ") + string(PHPDIR_PATH));
            }
            else if ((ret = execl(INTERPRETER, INTERPRETER, DBRENAME_SCRIPT, dmsg->getVcenter().c_str(), NULL)) < 0) {
                DISC_PLUG_DBG(string("Failed to execute: ") + DBRENAME_SCRIPT);
            }
            exit(0);
        } else {
            tpid = wait4(pid, &childstatus, 0, NULL);
            if (tpid == pid) {
                if (WEXITSTATUS(childstatus) == 0) {
                    status = WAVE_MESSAGE_SUCCESS;
                    DISC_PLUG_DBG_SUC(string("ppconfig succeeded"));
                } else {
                    status = WAVE_MESSAGE_ERROR;
                    DISC_PLUG_DBG_ERR(string("ppconfig failed (check logs)"));
                }
            }
        }
		DISC_PLUG_DBG_FUNC_END();
        dmsg->setCompletionStatus(status);
        return (status);
    }
#endif
#endif

}
