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

#ifndef VRRPSESSIONCONFIGLOCALMANAGEDOBJECT_H
#define VRRPSESSIONCONFIGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class VRRPVirtualIPLocalManagedObject;
    class VRRPTrackPortLocalManagedObject;
    class VRRPSessionConfigLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            VRRPSessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            VRRPSessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI8 &vrid,const UI32 &advt_int,const UI8 &prio,const bool &enable,const bool &preempt,const UI32 &hold_time,const string &description,const InterfaceType &iftype,const string &ifname,const bool &owner);
            virtual    ~VRRPSessionConfigLocalManagedObject ();
            static string getClassName();
            void setVrid(const UI8 &vrid);
            UI8 getVrid()  const;

            void setVip(const vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> > &vip);
            vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> > getVip()  const;
            void settrack(const vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> > &track);
            vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> > gettrack()  const;

            void setAdvt_int(const UI32 &advt_int);
            UI32 getAdvt_int()  const;
            void setPrio(const UI8 &prio);
            UI8 getPrio()  const;
            void setEnable(const bool &enable);
            bool getEnable()  const;
            void setPreempt(const bool &preempt);
            bool getPreempt()  const;
            void setHold_time(const UI32 &hold_time);
            UI32 getHold_time()  const;
            void setDescription(const string &description);
            string getDescription()  const;
            void setifType(const InterfaceType &iftype);
            InterfaceType getifType()  const;
            void setifName(const string &ifname);
            string getifName()  const;
            void setOwner(const bool &owner);
            bool getOwner()  const;
			
			WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> searchVirtualIPAddr(IpV4Address ipaddr);
			bool addVirtualIPMO(VRRPVirtualIPLocalManagedObject *pMO);
			bool deleteVirtualIPMO(IpV4Address ipaddr);
			bool deleteVirtualIPMO(const string& ipaddr_with_mask);
			bool deleteVirtualIPAll();
			UI32 getVirtualIPListSize();

			WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> searchTrackPort(string m_track_ifname);
			bool addTrackPortMO(VRRPTrackPortLocalManagedObject *pMO);
			bool deleteTrackPortMO(string track_ifname);
			void deleteTrackPortAll();

// Now the data members

        private:
        protected:
        public:
            UI8  m_vrid;
            vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> >  m_vip;
            vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> >  m_track;
            UI32  m_advt_int;
            UI8  m_prio;
//			bool  m_enablePresence;
            bool  m_enable;
            bool  m_preempt;
            UI32  m_hold_time;
            string  m_description;
            InterfaceType m_iftype;
            string  m_ifname;
			bool  m_owner;
    };
}
#endif                                            //VRRPSESSIONCONFIGLOCALMANAGEDOBJECT_H
