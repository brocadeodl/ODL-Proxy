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
 *   Author : krao                                                     *
 **************************************************************************/

#ifndef VRRPTRACKPORTLOCALMANAGEDOBJECT_H
#define VRRPTRACKPORTLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class VRRPTrackPortLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            VRRPTrackPortLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            VRRPTrackPortLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const InterfaceType &track_iftype,const string &track_ifname,const UI8 &track_prio);
            virtual    ~VRRPTrackPortLocalManagedObject ();
            static string getClassName();
            void setTrack_iftype(const InterfaceType &track_iftype);
            InterfaceType getTrack_iftype()  const;
            void setTrack_ifname(const string &track_ifname);
            string getTrack_ifname()  const;
            void setTrack_prio(const UI8 &track_prio);
            UI8 getTrack_prio()  const;

// Now the data members

        private:
        protected:
        public:
            InterfaceType m_track_iftype;
            string  m_track_ifname;
            UI8  m_track_prio;
    };
}
#endif                                            //VRRPTRACKPORTLOCALMANAGEDOBJECT_H
