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
 *   Author : Himanshu Varshney                                            *
 **************************************************************************/

#ifndef SUBGROUPMANAGEDOBJECT_H
#define SUBGROUPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{
    class GroupLocalObjectManager;

    class SubGroupLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
                    void    getAttributeGroupsInformation   (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeattributeGroupMap);
        protected:
            virtual void    setupAttributesForPersistence   ();
            virtual void    setupAttributesForCreate        ();
        public:
                            SubGroupLocalManagedObject      (GroupLocalObjectManager *pGroupLocalObjectManager);
            virtual        ~SubGroupLocalManagedObject      ();
            virtual void    getHardwareConfigurationDetailsForPostboot  (GetHardwareConfigurationDetailsForPostbootContext *pContext);    
            static  string  getClassName                    ();

                    void    setGroupName                    (const string &groupName);
                    string  getGroupName                    ()  const;
            virtual void    validateForCreateAtThisNode     (ValidateForCreateAtThisNodeContext *pContext);            

            virtual void    getHardwareConfigurationDetailsForUpdate    (GetHardwareConfigurationDetailsForUpdateContext *pContext);
            virtual void    getHardwareConfigurationDetailsForCreate    (GetHardwareConfigurationDetailsForCreateContext *pContext);
            virtual void    getHardwareConfigurationDetailsForDelete    (GetHardwareConfigurationDetailsForDeleteContext *pContext);            
// Now the data members

        private:
                    string                                                           m_groupName;
                    vector<ObjectId>                                                 m_employeeObjectId;
                    ObjectId                                                         m_locationObjectId;

        protected:
        public:
    };
}
#endif //SUBGROUPMANAGEDOBJECT_H

