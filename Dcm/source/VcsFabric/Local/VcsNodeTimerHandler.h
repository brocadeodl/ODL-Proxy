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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.           *
 *   All rights reserved.                                                                               *
 *   Author : Bao Vu                                                                                    *
 ***************************************************************************/

#ifndef VCSNODETIMERHANDLER_H
#define VCSNODETIMERHANDLER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"
#include "VcsFabric/Local/VcsNodePSMessagingContext.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsAddSwitchMessage.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

class VcsNodeTimerHandler
{
	friend class VcsNodePrincipalSelectionWorker;
	
    private :
    protected :
    public :
                            VcsNodeTimerHandler (UI32 domainId, LocationId locationId);
                           ~VcsNodeTimerHandler ();

		    UI32            getDomainId         ();
		    TimerHandle     getTimerHandle      ();
		    void            setTimerHandle      (TimerHandle timerHandle);
		    LocationId	    getLocationId       ();

            string          getServerIpAddress  () const;
            void            setServerIpAddress  (const string &serverIpAddress);
            UI32            getServerPort       () const;
            void            setServerPort       (const UI32 &serverPort);
		
    private :
            TimerHandle		m_timerHandle;
		    UI32			m_domainId;
		    LocationId		m_locationId;
            string          m_serverIpAddress;
            UI32            m_serverPort;
};
}

#endif // VCSNODETIMERHANDLER_H
