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
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "LockManagement/LockManagedObject.h"
#include "LockManagement/LockManagementObjectManagerAcquireLockMessage.h"
#include "LockManagement/LockManagementObjectManagerReleaseLockMessage.h"
#include "LockManagement/LockManagementMessagingContext.h"

#ifndef LOCKMANAGEMENTOBJECTMANAGER_H
#define LOCKMANAGEMENTOBJECTMANAGER_H

namespace WaveNs
{

class LockManagementObjectManager : public WaveObjectManager
{
    private :
                              LockManagementObjectManager ();

                virtual       PrismMessage             *createMessageInstance    (const UI32 &operationCode);
                virtual       WaveManagedObject        *createManagedObjectInstance (const string &managedClassName);


                void          failover                    (FailoverAsynchronousContext *pFailoverAsynchronousContext);

    protected :
    public :
        virtual                                 ~LockManagementObjectManager        ();
        static  string                           getClassName                       ();
        static  LockManagementObjectManager     *getInstance                        ();
        static  PrismServiceId                   getPrismServiceId                  ();
        void    boot                             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void    executeAcquireLockMessageHandler (LockManagementObjectManagerAcquireLockMessage *pLockManagementObjectManagerAcquireLockMessage);
        void    executeReleaseLockMessageHandler (LockManagementObjectManagerReleaseLockMessage *pLockManagementObjectManagerReleaseLockMessage);

        ResourceId validateAcquireLockStep                      (LockManagementMessagingContext *pLockManagementMessagingContext);
        ResourceId queryLockManagedObjectFromServiceStringStep  (LockManagementMessagingContext *pLockManagementMessagingContext);
        ResourceId createLockManagedObjectInDataBaseStep        (LockManagementMessagingContext *pLockManagementMessagingContext);
        ResourceId deleteLockManagedObjectFromDataBaseStep      (LockManagementMessagingContext *pLockManagementMessagingContext);
        ResourceId setServiceStringFromAcquireMessageStep       (LockManagementMessagingContext *pLockManagementMessagingContext);
        ResourceId setServiceStringFromReleaseMessageStep       (LockManagementMessagingContext *pLockManagementMessagingContext);
        
    // Now the data members

    private :
    protected :
    public :
};

}

#endif // LOCKMANAGEMENTOBJECTMANAGER_H
