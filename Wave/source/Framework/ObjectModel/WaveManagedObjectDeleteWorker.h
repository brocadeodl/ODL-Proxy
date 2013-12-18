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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef WAVEMANAGEDOBJECTDELETEWORKER_H
#define WAVEMANAGEDOBJECTDELETEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveManagedObjectAsynchronousPluginContext.h"


namespace WaveNs
{

class WaveObjectManagerDeleteWaveManagedObjectMessage;
class WaveObjectManagerDeleteWaveManagedObjectsMessage;
class WaveManagedObjectDeleteContext;
class UnifiedClientDeleteMessage;

class WaveManagedObjectDeleteWorker : public WaveWorker
{
    private :
        PrismMessage   *createMessageInstance         (const UI32 &operationCode);

        void            deleteHandlerForMultipleDelete                      (WaveObjectManagerDeleteWaveManagedObjectsMessage *pWaveObjectManagerDeleteWaveManagedObjectsMessage);
        void            sendAsynchronousDeleteMessagesStep              (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deleteHandlerForMultipleDeleteCallback              (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void* pCallerContext);
        void            deleteHandler                                   (WaveObjectManagerDeleteWaveManagedObjectMessage *pWaveObjectManagerDeleteWaveManagedObjectMessage);
        void            deleteQueryManagedObjectStep                    (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deleteValidateManagedObjectStep                 (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            validateForDeleteAtThisNodeCallBack             (ValidateForDeleteAtThisNodeContext *pValidateForDeleteAtThisNodeContext);
        void            deletePreDeleteManagedObjectStep                (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deleteDeleteManagedObjectStep                   (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deletePostDeleteManagedObjectStep               (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deletePreUpdateHardwareStep                     (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deletePreUpdateHardwareStepCallback             (PrismAsynchronousContext *pPrismAsynchronousContext);
        void            deleteUpdateHardwareStep                        (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deleteUpdateHardwareStepCallBack                (GetHardwareConfigurationDetailsForDeleteContext *pGetHardwareConfigurationDetailsForDeleteContext);
        void            deletePostUpdateHardwareStep                    (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        ResourceId      updateRelationship                              (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext, const string &parentClassName, const string &childClassName, const string &relationshipName, const ObjectId &parentObjectId, const ObjectId &childObjectId);
        void            commitWaveManagedObjectBeforeUpdatingBackend    (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            commitWaveManagedObjectAfterUpdatingBackend     (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deleteUpdateNewAttributeInManagedObject         (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deleteUpdateNewAttributeInManagedObjectCallback (UpdateNewAttributeInManagedObjectContext *pUpdateNewAttributeInManagedObjectContext);
        void            populateManagedObjectKeyAttributes              (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            populateParentKeyAttributes                     (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        string          getClientNameToSendAndPopulateRelatedAttributes (GetHardwareConfigurationDetailsForDeleteContext *pGetHardwareConfigurationDetailsForDeleteContext);
        
        ResourceId      deleteAssociations                              (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        void            deleteAssociationCallback                       (WaveSendToClusterContext *pWaveSendToClusterContext);
        void            WaveManagedObjectStartTransactionStep           (WaveManagedObjectDeleteContext *pWaveManagedObjectDeleteContext);
        
    protected :
    public :
                 WaveManagedObjectDeleteWorker      (WaveObjectManager *pWaveObjectManger);
        virtual ~WaveManagedObjectDeleteWorker      ();

    // Now the data members

    private :
    protected :
    public :
};

}
#endif // WAVEMANAGEDOBJECTDELETEWORKER_H

