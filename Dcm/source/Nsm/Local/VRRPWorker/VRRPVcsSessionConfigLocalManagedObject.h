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

#ifndef VRRPVCSSESSIONCONFIGLOCALMANAGEDOBJECT_H
#define VRRPVCSSESSIONCONFIGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    //class VRRPVirtualIPLocalManagedObject;
    //class VRRPTrackPortLocalManagedObject;
    class VRRPVcsSessionConfigLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            VRRPVcsSessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            VRRPVcsSessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI8 &vrid,const UI16 &advt_int,const UI8 &prio,const bool &enable,const bool &preempt,const UI16 &hold_time,const string &description);
            virtual    ~VRRPVcsSessionConfigLocalManagedObject ();
            static string getClassName();
            void setVrid(const UI8 &vrid);
            UI8 getVrid()  const;
            /*
            void setVip(const vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> > &vip);
            vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> > getVip()  const;
            void settrack(const vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> > &track);
            vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> > gettrack()  const;
            */
            void setPrio(const UI8 &prio);
            UI8 getPrio()  const;
            void setDescription(const string &description);
            string getDescription()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_vrid;
            /*
            vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> >  m_vip;
            vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> >  m_track;
            */
            UI8  m_prio;
            string  m_description;
    };
}
#endif                                            //VRRPVCSSESSIONCONFIGLOCALMANAGEDOBJECT_H
