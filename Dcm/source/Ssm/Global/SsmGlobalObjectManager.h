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
 *   Author : pasu                                                     *
 **************************************************************************/

#ifndef SSMGLOBALOBJECTMANAGER_H
#define SSMGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmGlobalAddMacAclPolicyMessage;
    class SsmGlobalRemMacAclPolicyMessage;
    class SsmGlobalAssocRuleToPolicyMessage;
    class SsmGlobalAssocPolicyToInterfaceMessage;
    class SsmMacAclWorker;
	class SsmIpAclWorker;
	class SsmIp6AclWorker;
    class MacAclConfigurationHandler;
    class MacAclRuleConfigurationHandler;
    class ExtMacAclRuleConfigurationHandler;
    class StdIpAclRuleConfigurationHandler;
    class ExtIpAclRuleConfigurationHandler;

    class SsmGlobalObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SsmGlobalAddMacAclPolicyMessageHandler( SsmGlobalAddMacAclPolicyMessage *pSsmGlobalAddMacAclPolicyMessage);
            void SsmGlobalAssocPolicyToInterfaceMessageHandler( SsmGlobalAssocPolicyToInterfaceMessage *pSsmGlobalAssocPolicyToInterfaceMessage);
            void SsmGlobalAssocRuleToPolicyMessageHandler( SsmGlobalAssocRuleToPolicyMessage *pSsmGlobalAssocRuleToPolicyMessage);
            void SsmGlobalRemMacAclPolicyMessageHandler( SsmGlobalRemMacAclPolicyMessage *pSsmGlobalRemMacAclPolicyMessage);
        protected:
        public:
            SsmGlobalObjectManager ();
            virtual    ~SsmGlobalObjectManager ();
            static SsmGlobalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			void clusterCreateCollectValidationData(WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
            void clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
// Now the data members

        private:
            SsmMacAclWorker  *m_pSsmMacAclWorker;
			SsmIpAclWorker	 *m_pSsmIpAclWorker;
			SsmIp6AclWorker	 *m_pSsmIp6AclWorker;
            MacAclConfigurationHandler      *m_pMacAclConfigurationHandler;
            MacAclRuleConfigurationHandler  *m_pMacAclRuleConfigurationHandler;
            ExtMacAclRuleConfigurationHandler  *m_pExtMacAclRuleConfigurationHandler;
            StdIpAclRuleConfigurationHandler   *m_pStdIpAclRuleConfigurationHandler;
            ExtIpAclRuleConfigurationHandler   *m_pExtIpAclRuleConfigurationHandler;
        protected:
        public:
    };

    inline string  SsmGlobalObjectManager::getClassName()
    {
        return ("SsmGlobal");
    }

}
#endif                                            //SSMGLOBALOBJECTMANAGER_H
