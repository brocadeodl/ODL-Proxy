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
 *   Author : skrastog                                                     *
 **************************************************************************/

#ifndef PORTSECURITYLOCALMANAGEDOBJECT_H
#define PORTSECURITYLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
    class PortSecAllowedMacsLocalManagedObject;
    class PortSecStickyMacsLocalManagedObject;
	class PortSecOuiLocalManagedObject;
    class PortSecurityLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PortSecurityLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            PortSecurityLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
#if 0
            PortSecurityLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager, const bool &isEnabled, const UI32 &numAllowedMacs, const vector<WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject> > &allowedMacs, const SI32 &action);
            PortSecurityLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager, const bool &isEnabled, const UI32 &numAllowedMacs, const vector<WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject> > &allowedMacs, const SI32 &action);
#endif
            virtual    ~PortSecurityLocalManagedObject ();
            static string getClassName();
            void setEnabled(const bool &isEnabled);
            bool getEnabled()  const;
			void setIfName(const string &ifName);
			string getIfName() const;
			void setIfType(const InterfaceType &ifType);
			InterfaceType getIfType() const;
            void setNumAllowedMacs(const UI32 &numAllowedMacs);
            UI32 getNumAllowedMacs()  const;
            void setAllowedMacs(const vector<WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject> > &allowedMacs);
            vector<WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject> > getAllowedMacs()  const;
			void addAllowedMac(PortSecAllowedMacsLocalManagedObject *allowedMacMO);
			void deleteAllowedMacs();
            void setStickyMacs(const vector<WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject> > &stickyMacs);
            vector<WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject> > getStickyMacs()  const;
			void addStickyMac(PortSecStickyMacsLocalManagedObject *stickyMacMO);
            void setNumOuis(const UI32 &numOuis);
            UI32 getNumOuis()  const;
            void setOuis(const vector<WaveManagedObjectPointer<PortSecOuiLocalManagedObject> > &ouis);
            vector<WaveManagedObjectPointer<PortSecOuiLocalManagedObject> > getOuis()  const;
			void addOui(PortSecOuiLocalManagedObject *oui);
			//void deleteOui(PortSecOuiLocalManagedObject *oui);
			void deleteAllOuis();
            void setAction(const UI32 &action);
            UI32 getAction()  const;
            void setSticky(const bool &sticky);
            bool getSticky() const;
            void setShutdownTime(const UI32 &time);
            UI32 getShutdownTime()  const;
            void setStaticSecureMacs(const UI32 &macs);
            UI32 getStaticSecureMacs()  const;

// Now the data members

        private:
        protected:
        public:
            bool  m_enabled;
			string m_ifName;
			InterfaceType m_ifType;
            UI32  m_numAllowedMacs;
            vector<WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject> >  m_allowedMacs;
            vector<WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject> >  m_stickyMacs;
			UI32  m_numOuis;
            vector<WaveManagedObjectPointer<PortSecOuiLocalManagedObject> >  m_ouis;
            SI32  m_action;
            UI32  m_newAction;
			bool  m_sticky;
            UI32  m_shutdownTime;
			UI32  m_numStaticSecureMacs;
    };
}
#endif                                            //PORTSECURITYLOCALMANAGEDOBJECT_H
