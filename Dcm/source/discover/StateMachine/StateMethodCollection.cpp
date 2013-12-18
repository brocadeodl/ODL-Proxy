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

#include "discover/StateMachine/DiscoveryStateMachine.h"
#include "discover/collector/vCenterSourceCollector.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/User/DiscoverMultiWorker.h"
#include "discover/vsphere/Debug.h"
/*

 Describes the State Machine and transitions on specific trigger

             +--------------------+--------------------------------+----------------------------+------------------------------+----------------------------------+---------------+----------------------------+--------------+--------------+
             |      Activate      |   Timer_Expired                | Asset_Collection_Completed | Distribute                   | Distributed_Completed            | De-Activate   | Warm_Restart               | Shutdown     |TimerInterval |
             +--------------------+--------------------------------+----------------------------+------------------------------+----------------------------------+---------------+----------------------------+--------------+--------------+
 DISC_Idle   | Schedule_Discovery |executePostDiscoveryComplete()  |  no_op_log()               |   no_op_log()                |    no_op_log()                   | deActivate()  | startTimer()               | cleanup()    |updateTimer() |
             | DISC_IN_PRG        | SAME_STATE                    |  SAME_STATE                |   SAME_STATE                 |    SAME_STATE                    | DISC_Idle     |  SAME_STATE                | SAME_STATE    | SAME_STATE   |
             +--------------------+--------------------------------+----------------------------+------------------------------+----------------------------------+---------------+----------------------------+--------------+--------------+
 DISC_IN_    | no_op_log()        | no_op_log()                    |  T -ISTRIBUTE              |   no_op_log()                |    no_op_log()                   | deActivate()  | Schedule_Discovery         | cleanup()    |updateTimer() |
 PRG         | SAME_STATE         | SAME_STATE                     | DISC_ASSET_COMPLETED       |   SAME_STATE                 |    SAME_STATE                    | DISC_Idle     | DISC_IN_PRG                | SAME_STATE   | SAME_STATE   |
             +--------------------+--------------------------------+----------------------------+------------------------------+----------------------------------+---------------+----------------------------+--------------+--------------+
 DISC_ASSET  | no_op_log()        | no_op_log()                    |  no_op_log()               |   distributeToLocalPlugins() |    no_op_log()                   | deActivate()  | distributeToLocalPlugins() | cleanup()    |updateTimer() |
 _COMPLETED  | SAME_STATE         | SAME_STATE                     |  SAME_STATE                |   DISC_DIST_IN_PRG           |    SAME_STATE                    | DISC_Idle     | DISC_DIST_IN_PRG           | SAME_STATE   |SAME_STATE    |
             +--------------------+--------------------------------+----------------------------+------------------------------+----------------------------------+---------------+----------------------------+--------------+--------------+
 DISC_DIST   | no_op_log()        | no_op_log()                    |  no_op_log()               |   no_op_log()                |    executePostDiscoveryComplete()| deActivate()  | distributeToLocalPlugins() | cleanup()    |updateTimer() |
 _IN_PRG     | SAME_STATE         | SAME_STATE                     |  SAME_STATE                |   SAME_STATE                 |    DISC_DIST_COMPLETED           | DISC_Idle     | DISC_DIST_IN_PRG           | SAME_STATE   |SAME_STATE    |
             +--------------------+--------------------------------+----------------------------+------------------------------+----------------------------------+---------------+----------------------------+--------------+--------------+
 DISC_DIST   |Schedule_Discovery  | executePostDiscoveryComplete() |  no_op_log()               |   no_op_log()                |    no_op_log()                   | deActivate()  |startTimer()                | cleanup()    |updateTimer() |
 _COMPLETED  | DISC_IN_PRG        | DISC_IN_PRG                    |  SAME_STATE                |   SAME_STATE                 |    SAME_STATE                    | DISC_Idle     | DISC_DIST_IN_PRG           | SAME_STATE   |SAME_STATE    |
             +--------------------+--------------------------------+----------------------------+------------------------------+----------------------------------+---------------+----------------------------+--------------+--------------+
 */
namespace DcmNs {

    //Idle state receiving Activate Trigger
    void DISC_IDLE_ACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        //On Activate, schedule Discovery
        scheduleDiscovery(data, sm, this->nextState);

    }

    //Idle state receiving timer Expired
    void DISC_IDLE_TIMER_EXPIRED::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {

        this->executePostDiscoveryComplete((void *) sm->vCenterdata, sm);
        sm->updateSuccess(nextState);

    }
    //Idle state receiving Asset collection completed
    void DISC_IDLE_ASSET_COLLECTION_COMPLETED::update(
            StateActivateMessage *data, DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }
    //Idle state receiving Distribute
    void DISC_IDLE_DISTRIBUTE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }
    //Idle state receiving Distribute Completed
    void DISC_IDLE_DISTRIBUTION_COMPLETED::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }
    //Idle state receiving De-activate
    void DISC_IDLE_DEACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {

        //this->stopDiscoveryCollector(data, sm);

        stopFullDiscoveryTimer(data, sm);
        sm->updateSuccess(nextState);
        sm->trigger(SHUTDOWN, sm->vCenterdata);
    }
    //Idle state receiving Warm-Restart(HA)
    void DISC_IDLE_WARM_RESTART::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "restart Timer");
        sm->updateSuccess(nextState);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d",
                sm->state->discState);
    }
    //Idle state receiving Shutdown
    void DISC_IDLE_SHUTDOWN::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s","Clean State Machine");
        sm->updateSuccess(nextState);
        this->shutdownVcenter(data, sm);

        //VIC_PLUG_LOG(VIC_TRACE_INFO, "Next State %d\n",
        //        sm->state->discState);
    }

    void DISC_IDLE_TIMER_INTERVAL::update(StateActivateMessage *data,
                DiscoveryStateMachine *sm) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s","Discovery Timer update,Restart Timer");
            updateTimerInterval(data,sm,true);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d",
                    sm->state->discState);
     }

    //DISC_IN_PRG

    void DISC_IN_PRG_ACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);

    }

    void DISC_IN_PRG_TIMER_EXPIRED::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);

    }
    void DISC_IN_PRG_ASSET_COLLECTION_COMPLETED::update(
            StateActivateMessage *data, DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                "synchDBWithStandby Trigger Distribute\n");

        sm->updateSuccess(nextState);
        sm->trigger(DISTRIBUTE, data);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d\n",
                sm->state->discState);
    }
    void DISC_IN_PRG_DISTRIBUTE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }
    void DISC_IN_PRG_DISTRIBUTION_COMPLETED::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }

    void DISC_IN_PRG_DEACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        //TODO De-activate

        this->stopDiscoveryCollector(data, sm);
        stopFullDiscoveryTimer(data, sm);
        sm->updateSuccess(nextState);
    }
    void DISC_IN_PRG_WARM_RESTART::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s","Schedule Discovery");
        sm->updateSuccess(nextState);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d",
                sm->state->discState);
    }
    void DISC_IN_PRG_SHUTDOWN::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s","Stop Discovery + Cleanup");
        sm->updateSuccess(nextState);
        this->shutdownVcenter(data, sm);

        //VIC_PLUG_LOG(VIC_TRACE_INFO, "Next State %d\n",
         //       sm->state->discState);
    }

    void DISC_IN_PRG_TIMER_INTERVAL::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s", "Discovery Timer update");
        updateTimerInterval(data,sm,false);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d\n", sm->state->discState);
    }

    //DISC_ASSET_COMPLETED

    void DISC_ASSET_COMPLETED_ACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);

    }

    void DISC_ASSET_COMPLETED_TIMER_EXPIRED::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }
    void DISC_ASSET_COMPLETED_ASSET_COLLECTION_COMPLETED::update(
            StateActivateMessage *data, DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }
    void DISC_ASSET_COMPLETED_DISTRIBUTE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Distribute to Local Plugins\n");
        this->distributeToLocalPlugins((void *) sm->vCenterdata, sm);
        sm->updateSuccess(nextState);
        sm->trigger(DISTRIBUTION_COMPLETED, data);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d\n",
                sm->state->discState);
    }
    void DISC_ASSET_COMPLETED_DISTRIBUTION_COMPLETED::update(
            StateActivateMessage *data, DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }

    void DISC_ASSET_COMPLETED_DEACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        this->stopDiscoveryCollector(data, sm);

        stopFullDiscoveryTimer(data, sm);
        sm->updateSuccess(nextState);
        ;
    }
    void DISC_ASSET_COMPLETED_WARM_RESTART::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Distribute to Local Plugins");
        sm->updateSuccess(nextState);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d",
                sm->state->discState);
    }
    void DISC_ASSET_COMPLETED_SHUTDOWN::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Stop Discovery + Cleanup");
        sm->updateSuccess(nextState);
        this->shutdownVcenter(data, sm);

       // VIC_PLUG_LOG(VIC_TRACE_INFO, "Next State %d\n",
        //        sm->state->discState);
    }

    void DISC_ASSET_COMPLETED_TIMER_INTERVAL::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s", "Discovery Timer update");
        updateTimerInterval(data,sm,false);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d", sm->state->discState);
    }

    //DISC_DIST_IN_PRG

    void DISC_DIST_IN_PRG_ACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);

    }

    void DISC_DIST_IN_PRG_TIMER_EXPIRED::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);

    }
    void DISC_DIST_IN_PRG_ASSET_COLLECTION_COMPLETED::update(
            StateActivateMessage *data, DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }
    void DISC_DIST_IN_PRG_DISTRIBUTE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }

    void DISC_DIST_IN_PRG_DISTRIBUTION_COMPLETED::update(
            StateActivateMessage *data, DiscoveryStateMachine *sm) {

        //this->startFullDiscoveryTimer((void *) sm->vCenterdata, sm);
        this->executePostDiscoveryComplete((void *) sm->vCenterdata, sm);
        sm->updateSuccess(nextState);
    }
    void DISC_DIST_IN_PRG_DEACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        //TODO deActivate();

        this->stopDiscoveryCollector(data, sm);

        stopFullDiscoveryTimer(data, sm);
        sm->updateSuccess(nextState);
    }
    void DISC_DIST_IN_PRG_WARM_RESTART::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Distribute to Local Plugins");
        sm->updateSuccess(nextState);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d",
                sm->state->discState);
    }
    void DISC_DIST_IN_PRG_SHUTDOWN::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Stop Discovery + Cleanup");
        sm->updateSuccess(nextState);
        this->shutdownVcenter(data, sm);

       // VIC_PLUG_LOG(VIC_TRACE_INFO, "Next State %d\n",
         //       sm->state->discState);
    }
    void DISC_DIST_IN_PRG_TIMER_INTERVAL::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s", "Discovery Timer update");
        updateTimerInterval(data,sm,false);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d", sm->state->discState);
    }

    // DISC_DIST_COMPLETED

    void DISC_DIST_COMPLETED_ACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        //no_op_log(data);

        //Stop the existing timer and re-start discovery
        this->stopFullDiscoveryTimer(data, sm);
        scheduleDiscovery(data, sm, this->nextState);

        //sm->updateSuccess(nextState);

    }

    void DISC_DIST_COMPLETED_TIMER_EXPIRED::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        //Since collector gets scheduled,updateSuccess happens with in the
        //collector

        //scheduleDiscovery(data, sm, this->nextState);
        this->executePostDiscoveryComplete((void *) sm->vCenterdata, sm);

    }
    void DISC_DIST_COMPLETED_ASSET_COLLECTION_COMPLETED::update(
            StateActivateMessage *data, DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }
    void DISC_DIST_COMPLETED_DISTRIBUTE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }
    void DISC_DIST_COMPLETED_DISTRIBUTION_COMPLETED::update(
            StateActivateMessage *data, DiscoveryStateMachine *sm) {
        no_op_log(data);
        sm->updateSuccess(nextState);
    }

    void DISC_DIST_COMPLETED_DEACTIVATE::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        //this->stopDiscoveryCollector(data, sm);

        stopFullDiscoveryTimer(data, sm);

        sm->updateSuccess(nextState);
        sm->trigger(SHUTDOWN, sm->vCenterdata);
    }
    void DISC_DIST_COMPLETED_WARM_RESTART::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "restart Timer\n");
        sm->updateSuccess(nextState);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d\n",
                sm->state->discState);
    }
    void DISC_DIST_COMPLETED_SHUTDOWN::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s","Stop Discovery + Cleanup\n");
        sm->updateSuccess(nextState);
        this->shutdownVcenter(data, sm);

       // VIC_PLUG_LOG(VIC_TRACE_INFO, "Next State %d\n",
         //       sm->state->discState);
    }
    void DISC_DIST_COMPLETED_TIMER_INTERVAL::update(StateActivateMessage *data,
            DiscoveryStateMachine *sm) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s", "Discovery Timer update,Restart Timer\n");
        updateTimerInterval(data,sm,true);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Next State %d\n", sm->state->discState);
    }
}
