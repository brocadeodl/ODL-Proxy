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
 * vCenterEventCollector.cpp
 *
 *  Created on: Apr 24, 2012
 *      Author: smalladi
 */

#include "discover/collector/vCenterEventCollector.h"
#include "discover/events/EventHandler.h"
#include "discover/events/DatacenterEventHandler.h"
#include "discover/events/EventHandlerFactory.h"
#include "discover/utils/StopWatch.h"
#include "discover/utils/FileUtils.h"
#include <curl/curl.h>
#include "discover/vsphere/Debug.h"
#include "discover/events/EventDefines.h"
#include "discover/StateMachine/DiscoveryStateMachineCollection.h"
#include "discover/StateMachine/DiscoveryStateMachine.h"
#include "vCenter/Framework/vCenterCrypto.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "discover/vsphere/vim25/ListUtil.h"
#include "vCenter/User/vCenterToolkit.h"

#include <raslog/raslog.h>
#include <raslog/raslog_vcenter.h>
#define EVALUATE_EVENT_STOP(vcenter) if(stopTask){ \
    VIC_PLUG_LOG(VIC_TRACE_INFO, "Stop Request recieved for %s",\
            vcenter.c_str());\
    return;\
}

#define LOGIN_PHASE 1

namespace DcmNs {

    class ProcessVcenterEventsTask: public WorkerTask {

        public:
            UI32 opcode;
            vector<Event *> evtVector;
            DiscoveryStateMachine *sm;
            string m_vcenter;
            string m_user;
            string m_pass;
            string m_url;


            ProcessVcenterEventsTask(UI32 _opcode, vector<Event *>_evtVector, DiscoveryStateMachine *_sm, string _vcenter, string _user,string _pass, string _url) :
                opcode(_opcode), evtVector(_evtVector ),  sm(_sm), m_vcenter(_vcenter), m_user(_user), m_pass(_pass), m_url(_url) {

                }


            void processEventAsync(string vcenter) {

                ServiceInstance *serviceInstance;
                auto_ptr<ServiceInstance> pautoServiceInstance;

                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Creating Service instance %s : %s : %s : %s",
                        vcenter.c_str(),m_url.c_str(), m_user.c_str(), m_pass.c_str());

                serviceInstance = new ServiceInstance(m_url,EVENT_CONNECTION_TIMEOUT);
                pautoServiceInstance  = auto_ptr<ServiceInstance> (serviceInstance);
                int code = pautoServiceInstance->login(m_user, m_pass);
                if (code == VIC_WSCLIENT_SUCCESS) {
                    for (size_t s = 0; s < evtVector.size(); s++) {
                        Event * event = evtVector.at(s);
                        EventHandler *handler = EventHandlerFactory::getHandler(vcenter, event);
                        if(handler != NULL)
                            handler->handle(serviceInstance);
						delete (event);
                    }
                }
                pautoServiceInstance->logout();

            }

            void startDiscovery(string vcenter) {

                DiscoverDiscoverMessage *dmsg_d = new DiscoverDiscoverMessage();
                int ret = WAVE_MESSAGE_SUCCESS;


                dmsg_d->setVcenter(vcenter);
                dmsg_d->setMsgType(DISC_MSG_TYPE_UPDATE);
                dmsg_d->setCmdcode(DISC_CMD_DISCOVER_VCENTER);
                dmsg_d->setUrl(m_url);
                dmsg_d->setPassword(m_pass);
                dmsg_d->setUsername(m_user);

                ret = WaveObjectManagerToolKit :: sendSynchronously(dmsg_d);

                if (WAVE_MESSAGE_SUCCESS != ret)
                {
                    // FIXME : Sagar : If we are not checking the return code, may be we should not have it alltogether ...
                }

                delete dmsg_d;
                ListUtil<Event>::clearPointersFromList(evtVector);
                this->stop();

            }

            virtual void run() {
                string vcenter = sm->state->vcenterName;

                VIC_PLUG_LOG(VIC_TRACE_INFO, "%s : %d",
                        "Process vCenter event", opcode);

                switch (opcode) {

                    case EVT_DISCOVER:
                        startDiscovery(vcenter);
                        break;
                    case EVT_EVENT:
                        processEventAsync(vcenter);
                        break;
                }

                sm->eventProcessingInProgress = false;
            }

    };

    vCenterEventCollector::vCenterEventCollector(string _vcenter, string _url,
            string _user, string _password,string _sm_uuid) :
        vcenter(_vcenter), url(_url), user(_user),
        password(_password),sm_uuid(_sm_uuid), eventTimerHandle(0) {

        }

    vCenterEventCollector::~vCenterEventCollector() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "EventCollector Destructor for %s ", vcenter.c_str());
        stopTimer();
        eventsQueue.stopQueue();

        DiscoveryStateMachineCollection * _smlColl =
            DiscoverMultiWorker::getStateMachineCollection();
        DiscoveryStateMachine *sm = _smlColl->findStateMachine(vcenter);
        if (sm != NULL) {
            sm->ecol = NULL;
        }

        //cleanup();
    }

    void vCenterEventCollector::stopEvents() {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "EventCollector Stop Events  for %s ", vcenter.c_str());
        stopTimer();
        eventsQueue.stopQueue();

        DiscoveryStateMachineCollection * _smlColl =
            DiscoverMultiWorker::getStateMachineCollection();
        DiscoveryStateMachine *sm = _smlColl->findStateMachine(vcenter);
        if (sm != NULL) {
            sm->ecol = NULL;
        }

    }

    UI32 vCenterEventCollector::sendUpdateDiscoveryStatus(string vcentername, UI32 state, UI32 exitStatus)

    {
        DiscoverUpdateMessage *dmsg = new DiscoverUpdateMessage();
        int ret = WAVE_MESSAGE_SUCCESS;

        VIC_PLUG_LOG(VIC_TRACE_INFO, "Sending Discover status update message for %s ",vcentername.c_str());
        dmsg->setVcenter(vcentername);
        dmsg->setState(UI32(state));
        dmsg->setCmdcode(DISC_CMD_UPDATE_DISCOVER_STATUS);
        dmsg->setExitStatus(UI32(exitStatus));

        ret = WaveObjectManagerToolKit :: sendSynchronously(dmsg);
        delete dmsg;
        return ret;

    }

    UI32 vCenterEventCollector::startDiscoveryAfterReconnect (string vcentername, string _url, string _user, string _password) 
    {

        DiscoverDiscoverMessage *dmsg_d = new DiscoverDiscoverMessage();
        int ret = WAVE_MESSAGE_SUCCESS;

        VIC_PLUG_LOG(VIC_TRACE_INFO, "Sending Discover message to run discovery %s ",vcentername.c_str());
        dmsg_d->setVcenter(vcentername);
        dmsg_d->setMsgType(DISC_MSG_TYPE_UPDATE);
        dmsg_d->setCmdcode(DISC_CMD_DISCOVER_VCENTER);
        dmsg_d->setUrl(_url);
        dmsg_d->setPassword(_password);
        dmsg_d->setUsername(_user);

        ret = WaveObjectManagerToolKit :: sendSynchronously(dmsg_d);
        delete dmsg_d;
        return ret;

    }

    void vCenterEventCollector::stopTimer() {
        if ( eventTimerHandle != 0)
            DiscoverMultiWorker::getInstance()->stopDiscoveryTimer( eventTimerHandle);
       // eventTimerHandle = 0;
    }

    void vCenterEventCollector::cleanup() {
        if (stopTask) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Cleaning up Event collector for %s ", vcenter.c_str());

            stopTimer();
            eventsQueue.stopQueue();

            DiscoveryStateMachineCollection * _smlColl =
                DiscoverMultiWorker::getStateMachineCollection();
            DiscoveryStateMachine *sm = _smlColl->findStateMachine(vcenter);
            if (sm != NULL) {
                sm->ecol = NULL;
            }
        }
        delete (this);

    }


    void vCenterEventCollector::run() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "EVENT Discovery stopTask %d ",
                stopTask);
        bool connectionLost = false;
        string unencryptPass;

        while(stopTask == false ) {
            VIC_PLUG_LOG(VIC_TRACE_INFO, "Starting EVENT Discovery for %s ",
                    vcenter.c_str());
            EVALUATE_EVENT_STOP(vcenter);
            int code =0;
            bool activated = true;
            int exitStatus = DISC_EXIT_STATUS_NO_VCENTER_SERVICE;
            ServiceInstance *serviceInstance;
            auto_ptr<ServiceInstance> pautoServiceInstance;
            VcenterManagedObject* pMO =
                vCenterToolkit::getVcenterConfigFromMo(vcenter);
            if (pMO != NULL) {
                user        = pMO->m_username;
                password    = vCenterCrypto::decryptPassword(
                        pMO->m_password);
                unencryptPass = pMO->m_password;
                activated = pMO->m_activate;

                VIC_PLUG_LOG(VIC_TRACE_INFO, "Using updated credentials %s:%s.",
                        vcenter.c_str(),user.c_str());
                delete pMO;
            } else {
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Failed to get vcenter managed object for %s",
                        vcenter.c_str());
                goto DISC_ERROR;
            }

            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Creating Service instance %s:%s:%s",
                    vcenter.c_str(),url.c_str(), user.c_str());
            serviceInstance = new ServiceInstance(url,EVENT_CONNECTION_TIMEOUT);
            pautoServiceInstance  = auto_ptr<ServiceInstance> (serviceInstance);
            code = pautoServiceInstance->login(user, password);
            if (code != VIC_WSCLIENT_SUCCESS) {
                checkAndPrintError(code, serviceInstance->getLastFault());
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Connection failed: updating status%s", vcenter.c_str());
                switch (code) {
                    case VIC_WSCLIENT_CURL_CURLE_COULDNT_CONNECT:
                        exitStatus = DISC_EXIT_STATUS_NO_VCENTER_SERVICE;
                        break;
                    case VIC_WSCLIENT_CURL_FAULT_RESPONSE_ERROR:
                        exitStatus = DISC_EXIT_STATUS_BAD_CREDENTIAL;
                        break;
                    default:
                        exitStatus = DISC_EXIT_STATUS_NO_VCENTER_SERVICE;
                        break;
                }
                sendUpdateDiscoveryStatus(vcenter,DISC_STATE_END,exitStatus);
                connectionLost = true;
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Failed to execute Login for %s",
                        vcenter.c_str());
                goto DISC_ERROR;
            }
            EVALUATE_EVENT_STOP(vcenter);
            if(discoveryInProgress == false && activated == true )
            {
                if(connectionLost == true) //connection has just recovered, run discovery
                {
                    startDiscoveryAfterReconnect(vcenter,url,user,unencryptPass);
                    connectionLost = false;
                }
                queryEvents(serviceInstance);
            }
            else
                VIC_PLUG_LOG(VIC_TRACE_INFO,"Event Discovery paused for vCenter %s as discovery is in progress. Re-schedulng in 30 %sec",vcenter.c_str());

DISC_ERROR:
            if(stopTask == true) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Event Discovery stopped for %s doing cleanup ",
                        vcenter.c_str());
                //cleanup();
                return;
            }
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Event Discovery failed for vCenter %s. Re-schedulng in 30 sec",
                    vcenter.c_str());
            sleep(30);
        }   
        //        DAO::closeDb();
        //In case of login failures, we cannot logout as well
        //pautoServiceInstance->logout();

        return;

    }

    EventFilterSpec vCenterEventCollector::getEventFilterSpec() {

        EventFilterSpec eventFilterSpec;

        eventFilterSpec.type = getSupportedEvents();
        return eventFilterSpec;
    }

    PropertyFilterSpec vCenterEventCollector::createEventFilterSpec(
            ManagedObjectReference *eventHistoryCollector) {

        PropertySpec propertyspec;
        propertyspec.all = false;
        propertyspec.type = eventHistoryCollector->type;
        propertyspec.pathSet.push_back("latestPage");

        ObjectSpec objspec;
        objspec.obj = *eventHistoryCollector;
        objspec.skip = false;

        PropertyFilterSpec propertyFilterSpec;
        propertyFilterSpec.propSet.push_back(propertyspec);
        propertyFilterSpec.objectSet.push_back(objspec);

        return propertyFilterSpec;
    }

    void vCenterEventCollector::handleEvent(Event* event, ServiceInstance *serviceInstance) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,"vCenter: %s Event: %s : %s ",
                vcenter.c_str(), event->type.c_str(),  event->fullFormattedMessage.c_str());

        DiscoveryStateMachineCollection * _smlColl = DiscoverMultiWorker::getStateMachineCollection();
        DiscoveryStateMachine *sm = _smlColl->findStateMachine(vcenter);

        if (sm != NULL) {
            if(sm->isVcenterActivated == false )
            {
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Vcenter %s not activated, ignoring the event received", vcenter.c_str());
                return;
            }
        }
        handleEventAsynchronously( event);

    }

    void vCenterEventCollector::handleEventAsynchronously(Event * event) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s", "handleEventAsync ");
        if (event->type.compare("TaskEvent") == 0) {
            if(!isSupportedTaskEvent (event->fullFormattedMessage)) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s : %s", "Unsupported Task Event, Skipping.  ", event->fullFormattedMessage.c_str());
                return;
            }
        }


        eventsQueue.pushAndNotify(event);
        restartEventTimer();
    }

    void vCenterEventCollector::restartEventTimer() {
        stopTimer();

        DiscoverMultiWorker::getInstance()->startDiscoveryTimer(
                eventTimerHandle,
                //fullDiscoveryTimerInterval,
                EVENT_QUEUE_TIMEOUT,
                reinterpret_cast<PrismTimerExpirationHandler>(&vCenterEventCollector::processEventTimerExpiry),
                (void *)this, NULL);
        //(void *) this, NULL);

    }

    void vCenterEventCollector::processEventTimerExpiry( TimerHandle timerHandle, void * pContext) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                "Event processor timer time out");
        vCenterEventCollector *coll = (vCenterEventCollector *) pContext;
        if (coll == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "COLL is null skipping the timer");
            return;
        }

        DiscoveryStateMachineCollection * _smlColl = DiscoverMultiWorker::getStateMachineCollection();
        DiscoveryStateMachine *sm = _smlColl->findStateMachine(coll->vcenter);

        if (sm == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "SM is null skipping the timer");
            return;
        }
        //if (sm->eventProcessingInProgress || coll->discoveryInProgress) {
        if (sm->eventProcessingInProgress ) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "Another Worker or discovery is already processing events so restarting the timer");
            coll->restartEventTimer();
            return;
        }

        vector<Event *> evtVector;
        /*
        if (coll->eventsQueue.size() > MAX_EVENT_QUEUE_SIZE) {
            // do the discovery
            VIC_PLUG_LOG(VIC_TRACE_INFO, "More than %d events received : Discovery",MAX_EVENT_QUEUE_SIZE);
            while(!coll->eventsQueue.empty()) {
                Event * m_event = coll->eventsQueue.waitAndPop();
                if (m_event != NULL)
                  Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO, "Event Received :: %s ", m_event->type.c_str());
                else
                  Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"%s", " NULL Event Received ::  ");

            }

            if (sm != NULL) {
                ProcessVcenterEventsTask* runDiscoveryTask = new ProcessVcenterEventsTask(EVT_DISCOVER, evtVector,  sm, coll->vcenter, coll->user, coll->password, coll->url);
                sm->eventProcessingInProgress = true;
                DiscoverMultiWorker::getWorkerPoolInstance()->addTask(runDiscoveryTask);
            }

        }else {
            VIC_PLUG_LOG(VIC_TRACE_INFO, "%s : %d", "Processing individual Events ",coll->eventsQueue.size());
            while(!coll->eventsQueue.empty()) {
                Event * m_event = coll->eventsQueue.waitAndPop();
                if (m_event != NULL) {
                Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO, "Event Received :: %s ", m_event->type.c_str());
                evtVector.push_back(m_event);
                }
                else
                  Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"%s", " NULL Event Received ::  ");
            }
            ProcessVcenterEventsTask* runDiscoveryTask = new ProcessVcenterEventsTask(EVT_EVENT, evtVector,  sm, coll->vcenter, coll->user, coll->password, coll->url);
            sm->eventProcessingInProgress = true;
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Processing individual Events %d ",evtVector.size());
            DiscoverMultiWorker::getWorkerPoolInstance()->addTask(runDiscoveryTask);
        }
        coll->eventsQueue.clear();*/
        //Get current Events from the Queue
        coll->eventsQueue.getCurrentEvents(evtVector);
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s:%d", "Timeout Event processor received events size: ", evtVector.size());
        if(evtVector.size() > MAX_EVENT_QUEUE_SIZE){
            VIC_PLUG_LOG(VIC_TRACE_INFO, "More than %d events received : Discovery",MAX_EVENT_QUEUE_SIZE);
            for(vector<Event *>::iterator itr = evtVector.begin();itr!=evtVector.end(); itr++)
            {
                Event * m_event = *itr;
                if (m_event != NULL){
                  Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO, "Event Received :: %s ", m_event->type.c_str());
                }
                else{
                  Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"%s", " NULL Event Received ::  ");
                }
            }
                if ( sm != NULL) {
                    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"%s", "Starting task to initiate Discovery 1");
                    ProcessVcenterEventsTask* runDiscoveryTask = new ProcessVcenterEventsTask(EVT_DISCOVER, evtVector,  sm, coll->vcenter, coll->user, coll->password, coll->url);
                    sm->eventProcessingInProgress = true;
                    DiscoverMultiWorker::getWorkerPoolInstance()->addTask(runDiscoveryTask);
                }
            
        }else{
         VIC_PLUG_LOG(VIC_TRACE_INFO, "%s : %d", "Processing individual Events ",evtVector.size());
            for(vector<Event *>::iterator itr = evtVector.begin();itr!=evtVector.end(); itr++){
                Event * m_event = *itr;
                if (m_event != NULL) {
                    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO, "Event Received :: %s ", m_event->type.c_str());
                }
                else{
                  Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"%s", " NULL Event Received ::  ");
                }
             }
                if ((sm != NULL) && (evtVector.size()>0)) {
                    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"%s", "Starting task to initiate Discovery 2");
                    ProcessVcenterEventsTask* runDiscoveryTask = new ProcessVcenterEventsTask(EVT_EVENT, evtVector,  sm, coll->vcenter, coll->user, coll->password, coll->url);
                    sm->eventProcessingInProgress = true;
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Processing individual Events %d ",evtVector.size());
                    DiscoverMultiWorker::getWorkerPoolInstance()->addTask(runDiscoveryTask);
                }
        }
//        coll->eventTimerHandle = 0;
    }

    string vCenterEventCollector::getVCPass(string _vcenter) {

        string pass;
    	VcenterManagedObject* pMO =
            vCenterToolkit::getVcenterConfigFromMo(_vcenter);
        if (pMO != NULL) {
        	pass = pMO->m_password;
        	delete pMO;
        	return pass;
        }
        VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s : %s", "MO not found for vcenter ", _vcenter.c_str());
        return "";
    }
    void vCenterEventCollector::handleUpdates(UpdateSet *updateSet, ServiceInstance *serviceInstance) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "HU----------- updateSet->filterSet-> size : %d",
                updateSet->filterSet->size());
        for (size_t i = 0; i < updateSet->filterSet->size(); i++) {
            PropertyFilterUpdate * propertyFilterUpdate =
                updateSet->filterSet->at(i);
            ManagedObjectReference filter = propertyFilterUpdate->filter;
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Filter type %s", filter.type.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Filter val %s", filter.val.c_str());
            vector<ObjectUpdate *> *objectSet =
                propertyFilterUpdate->objectSet;

            for (size_t s = 0; s < objectSet->size(); s++) {
                ObjectUpdate * objectUpdate = objectSet->at(s);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"ObjectUpdateKind %s",
                        objectUpdate->kind.val.c_str());
                ManagedObjectReference _obj = objectUpdate->obj;
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"ObjectUpdate obj type %s", _obj.type.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"ObjectUpdate obj val %s", _obj.val.c_str());
                vector<PropertyChange *> * changeSet =
                    objectUpdate->changeSet;

                for (size_t k = 0; k < changeSet->size(); k++) {
                    PropertyChange *propertyChange = changeSet->at(k);
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"PropertyChange Name %s",
                            propertyChange->name.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"PropertyChange valType %s",
                            propertyChange->valType.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"PropertyChange op val %s",
                            propertyChange->op.val.c_str());

                    if ((propertyChange->valType.compare("ArrayOfEvent")
                                == 0)
                            || (propertyChange->valType.find("Event")
                                != std::string::npos)) {
                        vector<Event *> *eventList =
                            (vector<Event *> *) propertyChange->val;
                        for (size_t m = 0; m < eventList->size(); m++) {
                            Event *event = eventList->at(m);
                            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Event chain Id %s",
                                    event->chainId.c_str());
                            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Event key %s", event->key.c_str());
                            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Event userName %s",
                                    event->userName.c_str());
                            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Event createdTime  %s",
                                    event->createdTime.c_str());

                            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                                    "Event DatacEnter %s",
                                    event->datacenter.datacenter.val.c_str());
                            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                                    "----------- valType : %s", event->type.c_str());
                            if (event->type.empty()) {
                                // handle empty event
                                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","EmptyEvent ");
                                delete(event);
                            }else {
                                handleEvent(event, serviceInstance);
                            }

                        }
                    }

                }
            }
        }
    }

    void vCenterEventCollector::queryEvents(ServiceInstance *serviceInstance) {

        EVALUATE_EVENT_STOP(vcenter);
        EventFilterSpec eventFilterSpec = getEventFilterSpec();
        eventsQueue.clear();
        EventManager *eventManager = NULL;
        serviceInstance->getEventManager(&eventManager);
        auto_ptr<EventManager> pautoEventManager(eventManager);

        ManagedObjectReference *eventHistoryCollector = NULL;

        int vcRes = eventManager->createCollectorForEvents(&eventFilterSpec,
                &eventHistoryCollector);

        if (vcRes != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(vcRes, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "Failed to create Event History Collector");
            return;
        }
        EVALUATE_EVENT_STOP(vcenter);
        auto_ptr<ManagedObjectReference> pautoEventHistoryCollector(
                eventHistoryCollector);

        PropertyFilterSpec propertyFilterSpec = createEventFilterSpec(
                eventHistoryCollector);

        //Get Property Collector
        PropertyCollector *propCollector = NULL;
        int code = serviceInstance->getPropertyCollector(&propCollector);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO, "%s",
                    "Unable to get Property Collector");
            return;
        }
        EVALUATE_EVENT_STOP(vcenter);
        auto_ptr<PropertyCollector> pautoPropertyCollector(propCollector);

        ManagedObjectReference *propFilter = NULL;
        vcRes = propCollector->createFilter(&propertyFilterSpec, true, &propFilter);
        if (vcRes != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(vcRes, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO, "%s",
                    "Failed to create Event Filter ");
            return;
        }
        EVALUATE_EVENT_STOP(vcenter);
        auto_ptr<ManagedObjectReference> pautoPropFilter(propFilter);

        string version = "";
        while (stopTask == false) {
            EVALUATE_EVENT_STOP(vcenter);

        DiscoveryStateMachineCollection * _smlColl =  DiscoverMultiWorker::getStateMachineCollection();
        DiscoveryStateMachine *sm = _smlColl->findStateMachine(vcenter);
        if ( (sm == NULL) || (sm->uuid != sm_uuid)) 
        {
            stopTask = true;
            VIC_PLUG_LOG(VIC_TRACE_INFO, "%s","StateMachine IDs do not match");
            return;
        }

            UpdateSet *updateSet = NULL;
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    "waitForUpdates vcenter: %s version :%s  ",
                    vcenter.c_str(), version.c_str());
            vcRes = propCollector->waitForUpdates(version, &updateSet);
            EVALUATE_EVENT_STOP(vcenter);
            if (stopTask == true) break;
            if (vcRes != VIC_WSCLIENT_SUCCESS) {

                checkAndPrintError(vcRes, serviceInstance->getLastFault());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "Failed to execute waitForupdates : %s", vcenter.c_str());
                if(vcRes ==VIC_WSCLIENT_CURL_CURLE_TIMEDOUT){
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Connection Timedout.Restarting connection %s", vcenter.c_str());
                    continue;
                }
                return;
            }
            EVALUATE_EVENT_STOP(vcenter);
            auto_ptr<UpdateSet> pautoUpdateSet(updateSet);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "UpdateSet version %s",
                    version.c_str());
            if (!version.empty())
                handleUpdates(updateSet, serviceInstance);
            version = updateSet->version;

            EVALUATE_EVENT_STOP(vcenter);
            /*
               VIC_PLUG_LOG(VIC_TRACE_INFO, "Sleeping... %s  ",
               version.c_str());
             */
            //      sleep(2);
        }

        //Cancel Wait for Updates
        vcRes = propCollector->cancelWaitForUpdates();
        if (vcRes != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(vcRes, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "Failed to Cancel Wait For Updates");
            return;
        }
        EVALUATE_EVENT_STOP(vcenter);
        //Destroy Property Filter
        vcRes = propCollector->destroyPropertyFilter(propFilter);
        if (vcRes != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(vcRes, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO, "%s",
                    "Failed to Destroy Property Filter");
            return;
        }
        EVALUATE_EVENT_STOP(vcenter);

    }

    void vCenterEventCollector::checkAndPrintError(int vcRes, Fault *fault) {
        if (vcRes == VIC_WSCLIENT_CURL_FAULT_RESPONSE_ERROR) {
            VIC_PLUG_LOG(VIC_TRACE_INFO, "Fault Error String %s",
                    fault->faultstring.c_str());
        }
    }

    } /* namespace DcmNs */
