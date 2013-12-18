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

/*
 * MultiDiscoveryRoutine.cpp
 *
 *  Created on: Jun 5, 2012
 *      Author: rmadapur
 */

#include "discover/StateMachine/MultiDiscoveryRoutine.h"
#include "discover/StateMachine/DiscoveryStateMachine.h"
#include "discover/StateMachine/DiscoveryStateMachineCollection.h"
#include "discover/StateMachine/StateEnum.h"
#include "discover/Framework/discoverLocalMessages.h"
#include "File/Local/FileLocalTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "File/Local/FilePushFileMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "vCenter/Framework/vCenterCrypto.h"
#include "vCenter/User/vCenterToolkit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"

namespace DcmNs {

    class DeleteVcenterTask: public WorkerTask {
        public:
            void *data;
            DiscoveryStateMachine *sm;
            DeleteVcenterTask(void *_data, DiscoveryStateMachine *_sm) :
                    data(_data), sm(_sm) {

            }
            void cleanStateMachineOnly(string vcenter) {
                DiscoveryStateMachineCollection * _smlColl =
                        DiscoverMultiWorker::getStateMachineCollection();
                _smlColl->deleteStateMachine(vcenter);
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Clean StateMachine for vCenter %s  StateMachine ",
                        vcenter.c_str());

            }

            void  deleteVcenterManagedObject(string vcenter){
                DiscoverUpdateMessage *dmsg = new DiscoverUpdateMessage();
                int ret;

                dmsg->setVcenter(vcenter);

                dmsg->setCmdcode(DISC_CMD_DELETE_VCENTER_MO);

                ret = WaveObjectManagerToolKit::sendSynchronously(dmsg);
                if (0 != ret)
                {
                    // Comment from Sagar : please use the ret variable result.
                    // Otherwise remove the ret.  GCC 4.7 complains set-no-used error.
                }
				delete(dmsg);
            }
            virtual void run() {
                StateActivateMessage *stateMsg = (StateActivateMessage *) data;
                DistributionSender *ds = DistributionSender::getInstance();
                string vcenter = sm->state->vcenterName;

                if (sm->cleanStateMachineOnly) {
                    cleanStateMachineOnly(vcenter);
                    this->stop();
                    return;
                }

                VIC_PLUG_LOG(VIC_TRACE_INFO, "%s",
                        "Delete vCenter  on all Nodes ");

                ds->distribute(stateMsg->vcenter, VNM_FULL_DISCOVERY,
                        DISC_CMD_CORELATION_DELETE_VCENTER);

                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Delete vCenter %s on all Nodes completed ",
                        vcenter.c_str());
                if (sm->collector != NULL) {
                    VIC_PLUG_LOG(
                            VIC_TRACE_INFO,
                            "Discovery collector for %s should have been deleted ",
                            vcenter.c_str());
                }
                DiscoveryStateMachineCollection * _smlColl =
                        DiscoverMultiWorker::getStateMachineCollection();
                _smlColl->deleteStateMachine(vcenter);
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Deleted vCenter %s  StateMachine ", vcenter.c_str());

                deleteVcenterManagedObject(vcenter);
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Deleted vCenter %s  Discovered Maanged Object",
                        vcenter.c_str());

                this->stop();
            }
            virtual void cleanup() {
                delete this;
            }

    };

    class SendDiscoveryStatusTask: public WorkerTask {
        public:

			string vcenter;
            UI32 cmdCode;
			UI32 state;
			UI32 exitStatus;

            SendDiscoveryStatusTask(string _vcenter,UI32 _cmdCode,UI32 _state,
                    UI32 _exitStatus) :
                    vcenter(_vcenter),cmdCode(_cmdCode), state(_state), exitStatus(_exitStatus) {

            }
            virtual void run() {

			DiscoverUpdateMessage *dmsg = new DiscoverUpdateMessage();
			int ret;

	        dmsg->setVcenter(vcenter);
    	    dmsg->setState(state);
        	dmsg->setExitStatus(exitStatus);
                dmsg->setCmdcode(cmdCode);

			ret = WaveObjectManagerToolKit :: sendSynchronously(dmsg);
				delete(dmsg);

            if (WAVE_MESSAGE_SUCCESS != ret)
            {
                // FIXME : Sagar : Handle the messaging failure case.
            }

                this->stop();
            }
            virtual void cleanup() {
                delete this;
            }

    };

	void updateDiscoveryStatus (string vcenter, UI32 state, UI32 exitStatus)

	{

        SendDiscoveryStatusTask* statusTask = new SendDiscoveryStatusTask(
                vcenter,DISC_CMD_UPDATE_DISCOVER_STATUS, state, exitStatus);
        DiscoverMultiWorker::getWorkerPoolInstance()->addTask(statusTask);

	}




    MultiDiscoveryRoutine::MultiDiscoveryRoutine() {
        // TODO Auto-generated constructor stub

    }

    MultiDiscoveryRoutine::~MultiDiscoveryRoutine() {
        // TODO Auto-generated destructor stub
    }

    void MultiDiscoveryRoutine::scheduleCollector(void *data,
            DiscoveryStateMachine *sm, StateActivateMessage *stateMsg,
            DISCOVERY_STATE nextState) {
        //Delete the collector if it had not happened before.
        if (sm->collector != NULL) {
            VIC_PLUG_LOG(
                    VIC_TRACE_INFO,
                    "Discovery collector for %s already running,cannot  schedule another ",
                    stateMsg->vcenter.c_str());
            return;
        }
        updatevCenterData(stateMsg->vcenter, sm);

        vCenterSourceCollector *col = new vCenterSourceCollector(
                stateMsg->vcenter, stateMsg->url, stateMsg->userName,
                stateMsg->password,stateMsg->ignoreValue,stateMsg->always,sm->uuid, sm->currentDiscoveryType, nextState);
        sm->collector = col;
        if (stateMsg->activated) {
            DiscoverMultiWorker::getWorkerPoolInstance()->addTask(col);
        }
        else {
            //De-activated
            VIC_PLUG_LOG(TRACE_LEVEL_INFO, "vCenter De-activated %s",
                    sm->vCenterdata->vcenter.c_str());
            /*DiscoverMultiWorker::getInstance()->updateDiscoveryStatus(
                    stateMsg->vcenter, DISC_STATE_END,
                    DISC_EXIT_STATUS_NOT_ACTIVATE);*/
		updateDiscoveryStatus(stateMsg->vcenter,UI32(DISC_STATE_END), UI32(DISC_EXIT_STATUS_NOT_ACTIVATE));
            sm->updateSuccess(DISC_IDLE);

        }
    }

    void MultiDiscoveryRoutine::updatevCenterData(string vcenterName,
            DiscoveryStateMachine *sm) {
        VcenterManagedObject* pMO =
                vCenterToolkit::getVcenterConfigFromMo(vcenterName);
        if (pMO != NULL) {
            StateActivateMessage *stateMsg = sm->vCenterdata;
            stateMsg->password = vCenterCrypto::decryptPassword(
                    pMO->m_password);
            stateMsg->ignoreValue = pMO->m_ignore_value;
            stateMsg->always 	  = pMO->m_always;
            stateMsg->activated   = pMO->m_activate;

            delete pMO;
        }
    }

    void MultiDiscoveryRoutine::stopDiscoveryCollector(void *data,
            DiscoveryStateMachine *sm) {
        if (sm->collector != NULL) {
            VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Thread stopping  for %s",
                    sm->vCenterdata->vcenter.c_str());
            sm->stopRequest = true;
            sm->collector->stop();

        }
        else {
            VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                    "No collector running,Trigger Shutdown %s",
                    sm->vCenterdata->vcenter.c_str());
            sm->trigger(SHUTDOWN, sm->vCenterdata);
        }
    }

    //Various Discovery Routines are listed here.
    /**
     * Schedule Discovery of vCenter
     */
    void MultiDiscoveryRoutine::scheduleDiscovery(void *data,
            DiscoveryStateMachine *sm, DISCOVERY_STATE nextState) {
        StateActivateMessage *stateMsg = (StateActivateMessage *) data;
        sm->vCenterdata = stateMsg;

        if (sm->collector != NULL) {
            VIC_PLUG_LOG(
                    VIC_TRACE_INFO,
                    "Discovery collector for %s already running,cannot  schedule another ",
                    stateMsg->vcenter.c_str());
            return;
        }
        this->stopFullDiscoveryTimer(sm->vCenterdata, sm);

        VIC_PLUG_LOG(VIC_TRACE_INFO, "schedule Discovery for %s",
                stateMsg->vcenter.c_str());
        /*DiscoverMultiWorker::getInstance()->updateDiscoveryStatus(
                sm->vCenterdata->vcenter, DISC_STATE_START); */
		updateDiscoveryStatus(sm->vCenterdata->vcenter,UI32(DISC_STATE_START), UI32(DISC_EXIT_STATUS_SUCCESS));
        scheduleCollector(data, sm, stateMsg, nextState);
    }

    void MultiDiscoveryRoutine::startFullDiscoveryTimer(void *data,
            DiscoveryStateMachine *sm) {

        StateActivateMessage *stateMsg = (StateActivateMessage *) data;
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Start Full Discovery Timer for %s",
                stateMsg->vcenter.c_str());
        if (sm->fullDiscoveryTimerInterval != 0) {
            DiscoverMultiWorker::getInstance()->startDiscoveryTimer(
                    sm->fullDiscoveryTimerHandle,
                    sm->fullDiscoveryTimerInterval,
                    reinterpret_cast<PrismTimerExpirationHandler>(&MultiDiscoveryRoutine::processFullDiscoverTimerExpiry),
                    (void *) data, NULL);
        }
        else {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Discovery Timer is set to zero, not starting timer",
                    stateMsg->vcenter.c_str());
        }

    }

    void MultiDiscoveryRoutine::scheduleFullDiscoveryMethod(void *data,
            DiscoveryStateMachine *sm) {
        StateActivateMessage *stateMsg = (StateActivateMessage *) data;
        sm->vCenterdata = stateMsg;

        this->stopFullDiscoveryTimer(sm->vCenterdata, sm);

        VIC_PLUG_LOG(VIC_TRACE_INFO, "schedule Full Discovery for %s",
                stateMsg->vcenter.c_str());
        /*DiscoverMultiWorker::getInstance()->updateDiscoveryStatus(
                sm->vCenterdata->vcenter, DISC_STATE_START); */
		updateDiscoveryStatus(sm->vCenterdata->vcenter,UI32(DISC_STATE_START), UI32(DISC_EXIT_STATUS_SUCCESS));
        sm->currentDiscoveryType = VNM_FULL_DISCOVERY;
        scheduleCollector(data, sm, stateMsg, DISC_IN_PRG);

    }

    void MultiDiscoveryRoutine::shutdownVcenter(void *data,
            DiscoveryStateMachine *sm) {
        stopFullDiscoveryTimer(data, sm);
        this->deleteVCenter((void *) sm->vCenterdata, sm);
    }

    void MultiDiscoveryRoutine::executePostDiscoveryComplete(void *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(
                VIC_TRACE_INFO,
                "executePostDiscoveryComplet DiscoveryType= %d ,  scheduleFullDiscovery=%d",
                sm->currentDiscoveryType, sm->scheduleFullDiscovery);
        switch (sm->currentDiscoveryType) {
            case VNM_FULL_DISCOVERY:
                //Full Discovery is Scheduled
                if (sm->scheduleFullDiscovery == true) {
                    sm->scheduleFullDiscovery = false;
                    //Schedule Full Discovery
                    this->scheduleFullDiscoveryMethod((void *) sm->vCenterdata,
                            sm);
                }

                else {
                    this->startFullDiscoveryTimer((void *) sm->vCenterdata, sm);
                    sm->currentDiscoveryType = VNM_NONE_DISCOVERY;
                }

                break;

                //No discovery is currently in progress
            case VNM_NONE_DISCOVERY:
                if (sm->scheduleFullDiscovery == true) {
                    sm->scheduleFullDiscovery = false;

                    //Schedule Full Discovery Method
                    this->scheduleFullDiscoveryMethod((void *) sm->vCenterdata,
                            sm);
                }

                break;

        }
    }
    void MultiDiscoveryRoutine::processFullDiscoverTimerExpiry(
            TimerHandle timerHandle, void * pContext) {

        StateActivateMessage *stateMsg = (StateActivateMessage *) pContext;
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Full Discovery Timer expired handler for %s",
                stateMsg->vcenter.c_str());
        DiscoveryStateMachineCollection * _smlColl =
                DiscoverMultiWorker::getStateMachineCollection();
        DiscoveryStateMachine *sm = _smlColl->findStateMachine(
                stateMsg->vcenter);
        if(sm!=NULL){
            sm->scheduleFullDiscovery = true;
            this->stopFullDiscoveryTimer(sm->vCenterdata, sm);
            sm->trigger(TIMER_EXPIRED, stateMsg);
        }else{
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                            "Full Discovery Timer expired handler for %s: could not find StateMachine",
                            stateMsg->vcenter.c_str());
        }
    }

    void MultiDiscoveryRoutine::stopFullDiscoveryTimer(void *data,
            DiscoveryStateMachine *sm) {
        StateActivateMessage *stateMsg = (StateActivateMessage *) data;
        if (sm->fullDiscoveryTimerHandle != 0) {
            DiscoverMultiWorker::getInstance()->stopDiscoveryTimer(
                    sm->fullDiscoveryTimerHandle);
            VIC_PLUG_LOG(VIC_TRACE_INFO, "Stop Full Discovery Timer for %s",
                    stateMsg->vcenter.c_str());
        }
        else {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Full Discovery Timer already stopped for %s",
                    stateMsg->vcenter.c_str());
        }

    }

    void MultiDiscoveryRoutine::distributeToLocalPlugins(void *data,
            DiscoveryStateMachine *sm) {
        StateActivateMessage *stateMsg = (StateActivateMessage *) data;
        DistributionSender *ds = DistributionSender::getInstance();
        if(stateMsg->runReconcile){
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                                "Distribute DB for reconcilation %s",
                                stateMsg->vcenter.c_str());
            ds->distribute(stateMsg->vcenter, VNM_FULL_DISCOVERY,
                    DISC_CMD_START_RECONCILE);
            stateMsg->runReconcile =false;
        }else{
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                                            "Distribute DB for reconcilation %s",
                                            stateMsg->vcenter.c_str());
            ds->distribute(stateMsg->vcenter, VNM_FULL_DISCOVERY,
                   DISC_CMD_START_CORELATION);
        }

        /*DiscoverMultiWorker::getInstance()->updateDiscoveryStatus(
                sm->vCenterdata->vcenter, DISC_STATE_END,
                DISC_EXIT_STATUS_SUCCESS);*/
		updateDiscoveryStatus(sm->vCenterdata->vcenter,UI32(DISC_STATE_END), UI32(DISC_EXIT_STATUS_SUCCESS));

    }
    void MultiDiscoveryRoutine::deleteVCenter(void *data,
            DiscoveryStateMachine *sm) {
        StateActivateMessage *stateMsg = (StateActivateMessage *) data;
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Delete vCener for %s",
                stateMsg->vcenter.c_str());
        DeleteVcenterTask * deleteTask = new DeleteVcenterTask(data, sm);
        DiscoverMultiWorker::getWorkerPoolInstance()->addTask(deleteTask);

    }

    void MultiDiscoveryRoutine::no_op_log(void *data) {
        StateActivateMessage *stateMsg = (StateActivateMessage *) data;

        VIC_PLUG_LOG(VIC_TRACE_INFO, "Invalid Trigger received for %s",
                stateMsg->vcenter.c_str());
    }

    void MultiDiscoveryRoutine::updateTimerInterval(void *data,
            DiscoveryStateMachine *sm, bool restartTimer) {
        StateActivateMessage *stateMsg = (StateActivateMessage *) data;

        VcenterManagedObject* pMO =
                vCenterToolkit::getVcenterConfigFromMo(stateMsg->vcenter);
        if (pMO != NULL) {

            UI32 interval = VCENTER_MIN_TO_MILLISEC(pMO->getInterval());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Updating the discovery timer  for %s ",
                    stateMsg->vcenter.c_str());
            UI32 currentDiscoveryTimerInterval = sm->fullDiscoveryTimerInterval;
            sm->fullDiscoveryTimerInterval = interval;
            if (!restartTimer) {
                return;
            }

            if (interval != 0) { //user requested for changing discovery timer
                if (sm->fullDiscoveryTimerHandle != 0) { //If full discovery timer already running(indicates there is no current discovery) then stop and restart.
                    VIC_PLUG_LOG(VIC_TRACE_INFO,
                            "Restarting discovery timer  for %s ",
                            stateMsg->vcenter.c_str());
                    this->stopFullDiscoveryTimer(sm->vCenterdata, sm);
                    this->startFullDiscoveryTimer(sm->vCenterdata, sm);
                }
                else if (currentDiscoveryTimerInterval == 0) { //No discovery in progress, currentDiscoveryTimer==0 indicates that discovery was stopped earlier

                    this->startFullDiscoveryTimer(sm->vCenterdata, sm);

                }

            }
            else { //interval 0 , means no re-discovery is required
                if (sm->fullDiscoveryTimerHandle != 0) { //If full discovery timer already running then stop(don't restart
                    VIC_PLUG_LOG(VIC_TRACE_INFO,
                            "Stopping discovery timer for %s ",
                            stateMsg->vcenter.c_str());
                    this->stopFullDiscoveryTimer(sm->vCenterdata, sm);
                }
            }
            delete pMO;
        }

    }

} /* namespace DcmNs */


