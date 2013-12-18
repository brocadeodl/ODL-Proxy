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
 *   Author : kghanta                                                     *
 **************************************************************************/

#ifndef RULEMANAGEDOBJECT_H
#define RULEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

#include "AAAGlobalTypes.h"
using namespace WaveNs;

namespace DcmNs
{

    class AAAGlobalObjectManager;
    class RuleManagedObject : virtual public DcmManagedObject
    {
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
			virtual void setupKeys();
        public:
            RuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
            RuleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const UI32 &index, const UI32 &cmdTypeChoice, const SI32 &command, const string &interfaceTE, const string &interfaceGE, const string &interfacePC, const string &interfaceVLAN, const string &interfaceFCOE, const ObjectId &role, const RBACOperation &operation, const RBACAction &action, const string &group, const bool &isPredefined, const bool &interfaceInt, const bool &interfaceTe, const bool &interfaceGe, const bool &interfacePc, const bool &interfaceVlan, const bool &interfaceFcoe, const bool &interfaceMgmt, const bool &copyCp, const bool &copyRunning, const bool &copySupport, const bool &clearCl, const bool &clearLogging, const bool &clearSupport, const bool &protocolPr, const bool &protocolSpanning, const bool &protocolLldp, const UI32 &cmdRuleCounter, const UI32 &dataRuleCounter);
            virtual    ~RuleManagedObject ();
            static string getClassName();
            void setIndex(const UI32 &index);
            UI32 getIndex()  const;
			void setCmdTypeChoice (const UI32 &cmdTypeChoice);
			UI32 getCmdTypeChoice () const;
            void setCommand(const SI32 &command);
            SI32 getCommand()  const;
            void setInterfaceTELeaf(const string &interface);
            string getInterfaceTELeaf()  const;
			void setInterfaceGELeaf(const string &interface);
			string getInterfaceGELeaf()  const;
			void setInterfacePCLeaf(const string &interface);
			string getInterfacePCLeaf() const;
			void setInterfaceVLANLeaf(const string &interface);
			string getInterfaceVLANLeaf() const;
            void setInterfaceFCOELeaf(const string &interface);
            string getInterfaceFCOELeaf()  const;
            void setRole(const ObjectId &role);
            ObjectId getRole()  const;
            void setOperation(const RBACOperation &operation);
            RBACOperation getOperation()  const;
            void setAction(const RBACAction &action);
            RBACAction getAction()  const;
            void setGroup(const string &group);
            string getGroup()  const;
            void setIsPredefined(const  bool &isPredefined);
            bool getIsPredefined()  const;
			UI32 getCase (const UI32 &wyserCmdXmlTag);

            void setInterfaceInt(const bool &interfaceInt);
            bool getInterfaceInt()  const;
            void setInterfaceTe(const bool &interfaceTe);
            bool getInterfaceTe()  const;
			void setInterfaceGe(const bool &interfaceGe);
			bool getInterfaceGe()  const;
			void setInterfacePc(const bool &interfacePc);
			bool getInterfacePc() const;
			void setInterfaceVlan(const bool &interfaceVlan);
			bool getInterfaceVlan() const;
            void setInterfaceFcoe(const bool &interfaceFcoe);
            bool getInterfaceFcoe()  const;
            void setInterfaceMgmt(const bool &interfaceMgmt);
            bool getInterfaceMgmt()  const;
			
            void setCopyCp(const bool &copyCp);
            bool getCopyCp()  const;
            void setCopyRunning(const bool &copyRunning);
            bool getCopyRunning()  const;
            void setCopySupport(const bool &copySupport);
            bool getCopySupport()  const;
            
			void setClearCl(const bool &clearCl);
            bool getClearCl()  const;
            void setClearLogging(const bool &clearLogging);
            bool getClearLogging()  const;
            void setClearSupport(const bool &clearSupport);
            bool getClearSupport()  const;
            
			void setProtocolPr(const bool &protocolPr);
            bool getProtocolPr()  const;
			void setProtocolSpanning(const bool &protocolSpanning);
            bool getProtocolSpanning()  const;
			void setProtocolLldp(const bool &protocolLldp);
            bool getProtocolLldp()  const;
            void setCmdRuleCounter(const UI32 &counter);
            UI32 getCmdRuleCounter()  const;
            void setDataRuleCounter(const UI32 &counter);
            UI32 getDataRuleCounter()  const;
// Now the data members
        private:
            UI32  m_index;
			UI32  m_cmdTypeChoice;
            SI32  m_command;
			string m_interfaceTELeaf;
			string m_interfaceGELeaf;
			string m_interfacePCLeaf;
			string m_interfaceVLANLeaf;
			string m_interfaceFCOELeaf;
            ObjectId  m_role;
            RBACOperation  m_operation;
            RBACAction  m_action;
			string m_group;
			bool m_isPredefined;
			bool m_interfaceInt;
			bool m_interfaceTe;
			bool m_interfaceGe;
			bool m_interfacePc;
			bool m_interfaceVlan;
			bool m_interfaceFcoe;
			bool m_interfaceMgmt;
			bool m_copyCp;
			bool m_copyRunning;
			bool m_copySupport;
			bool m_clearCl;
			bool m_clearLogging;
			bool m_clearSupport;
			bool m_protocolPr;
			bool m_protocolSpanning;
			bool m_protocolLldp;
            UI32 m_cmdRuleCounter;
			UI32 m_dataRuleCounter;
    };
}
#endif                                            //RULEMANAGEDOBJECT_H
