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
 *   Author : Ritesh Madapurath                                            *
 ***************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "discover/Framework/EventEngineThread.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"

#include <sys/wait.h>
#include <unistd.h>

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
#define INTERPRETER     ((char *) "/usr/apache/php")
#define PHPDIR_PATH     ((char *) "/fabos/webtools/htdocs/ws")
#define TEST            ((char *) "/usr/bin/ls")
//#define INTERPRETER     ((char *) "/bin/ls")

namespace DcmNs {

    EventEngineThread::EventEngineThread():running(false) {
    }

    EventEngineThread::~EventEngineThread() {
        trace(TRACE_LEVEL_INFO,
                string("EventsEngineWorker::Calling Destructor"));
    }

    WaveThreadStatus EventEngineThread::start() {
        running = true;
        trace(TRACE_LEVEL_INFO, string("EventsEngineWorker::start"));
        int ret = 0;
        //EventEngine plugin runs under CCMD
        SI32 serverPort = DcmToolKit::getMgmtPort();
        if (DCM_CCM == DcmToolKit::getDistributionMode() &&(VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == DcmToolKit::getThisVcsNodeClusterType())) {
            serverPort =
                    DcmToolKit::getManagementInterfaceReceiverPortForCcmd();
        }

        stringstream ss;
        ss << serverPort;
        tracePrintf(TRACE_LEVEL_INFO, "EventsEngineWorker::start connect to server Port %s",ss.str().c_str());
        m_pid = fork();

        if (m_pid == 0) {

             const string scriptName = "event";
             const string scriptPath = "/fabos/webtools/htdocs/ws/" + scriptName
             + ".php";


             trace(TRACE_LEVEL_INFO,
             string("EventsEngineWorker::Executing Script"));

             if ((ret = chdir(PHPDIR_PATH)) < 0) {
             trace(TRACE_LEVEL_INFO,
             string("Failed chdir to: ") + string(PHPDIR_PATH));
             }



             if ((ret = execl(INTERPRETER, INTERPRETER, scriptPath.c_str(),getVcenter().c_str(),
             getUrl().c_str(), getUsername().c_str(),getPassword().c_str(), ss.str().c_str(), NULL)) < 0) {
             trace(
             TRACE_LEVEL_INFO,
             string(
             "EventsEngineWorker::Events.php execution failed"));
             }


          /*  if ((ret == execl(TEST, TEST, NULL)) < 0) {
                trace(
                        TRACE_LEVEL_INFO,
                        string(
                                "EventsEngineWorker::Events.php execution failed"));
            }*/

            exit(0);
        }
        else {

            this->stopCallingThread();
        }

        return (WAVE_THREAD_SUCCESS);
    }

    void EventEngineThread::stopActivity() {
        int ret = 0, childstatus;

        if (m_pid != 0) {
            if ((ret = kill(m_pid, 9)) < 0) {
                trace(
                        TRACE_LEVEL_INFO,
                        string(
                                "EventsEngineWorker::Events.php Termination failed"));
            }
            else {
                trace(
                        TRACE_LEVEL_INFO,
                        string(
                                "EventsEngineWorker::Events.php Termination succeeded"));
            }
        }
        int tpid = wait4(m_pid, &childstatus, 0, NULL);
        if (tpid == m_pid) {
            if (WEXITSTATUS(childstatus) == 0) {
                tracePrintf(TRACE_LEVEL_INFO,

                "EventsEngineWorker::Events.php execution Succeded %d",
                        childstatus);
            }
            else {
                trace(
                        TRACE_LEVEL_INFO,
                        string(
                                "EventsEngineWorker::Events.php execution Failed Status"));
            }
        }
        running = false;
        m_pid = 0;

    }

    string EventEngineThread::getPassword() const {
        return m_password;
    }

    int EventEngineThread::getPid() const {
        return m_pid;
    }

    string EventEngineThread::getUrl() const {
        return m_url;
    }

    string EventEngineThread::getUsername() const {
        return m_username;
    }

    string EventEngineThread::getVcenter() const {
        return m_vcenter;
    }

    void EventEngineThread::setPassword(string password) {
        m_password = password;
    }

    void EventEngineThread::setPid(int pid) {
        m_pid = pid;
    }

    void EventEngineThread::setUrl(string url) {
        m_url = url;
    }

    void EventEngineThread::setUsername(string username) {
        m_username = username;
    }

    void EventEngineThread::setVcenter(string vcenter) {
        m_vcenter = vcenter;
    }

}

