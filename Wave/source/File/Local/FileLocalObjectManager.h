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

/*********************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.    *
 *   All rights reserved.                                            *
 *   Author : jiyer                                                  *
 *********************************************************************/

#ifndef FILELOCALOBJECTMANAGER_H
#define FILELOCALOBJECTMANAGER_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "File/Local/FilePushFileMessage.h"
#include "File/Local/FileAbortFileTransferMessage.h"
#include "File/Local/FilePushFileFragmentMessage.h"
#include "File/Local/FileTransferHandshakeMessage.h"
#include "File/Local/FileTransferManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"

using namespace WaveNs;

namespace WaveNs
{
     class CFileFragment;
     class FilePushFileMessage;
     class FilePushFileFragmentMessage;
     class FileAbortFileTransferMessage;
     class FileTransferHandshakeMessage;
     class CFileTransferManagedObject; 
     class PushFileToHaPeerMessage;
     class FileDeleteRequestMessage;
     class FileDeleteFileMessage;
     class FileDeleteFileToHaPeerMessage; 

    class FileLocalObjectManager: public WaveLocalObjectManager
    {
        private:
               FileLocalObjectManager                                         ();
               PrismMessage      *createMessageInstance                       (const UI32 &operationCode);
               WaveManagedObject *createManagedObjectInstance                 (const string &managedClassName);
               void               AbortFileTransferMessageHandler             (FileAbortFileTransferMessage *pFileAbortFileTransferMessage);
               void               PushFileMessageHandler                      (FilePushFileMessage *pPushFileMessage);
               void               FileTransferHandshakeMessageHandler         (FileTransferHandshakeMessage *pFileTransferHandshakeMessage);
               void               PushFileFragmentMessageHandler              (FilePushFileFragmentMessage *pPushFileFragmentMessage);
            // ResourceId         StartFileTransferCallback                   (FrameworkStatus frameworkStatus, FilePushFileFragmentMessage *pMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext);          
            // ResourceId         TriggerFileTransferHandshake                (PrismLinearSequencerContext *pPrismLinearSequencerContext);            
            // ResourceId         TriggerFileTransferHandshakeCallback        (FrameworkStatus frameworkStatus, FileTransferHandshakeMessage *pMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext);
                        
               void               StartFileTransferCallback                   (WaveSendToClusterContext *pWaveSendToClusterContext);
               ResourceId         TriggerAsyncFileTransferHandshake           (PrismLinearSequencerContext *pPrismLinearSequencerContext);
               void               TriggerFileTransferHandshakeCallback        (WaveSendToClusterContext *pWaveSendToClusterContext);

               // More private methods.
               ResourceId         ValidateInputs                              (PrismLinearSequencerContext *pPrismLinearSequencerContext);

               void               validateLocationIdAndVersion				  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
               void               validateTransferFlag						  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
               void               addToIncomingQueue						  (PrismLinearSequencerContext *pPrismLinearSequencerContext);

               ResourceId         StartAsyncFileTransfer                      (PrismLinearSequencerContext *pPrismLinearSequencerContext);
               ResourceId         ProcessFileFragmentMessage                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
               ResourceId         ProcessAbortFileTransferMessage             (PrismLinearSequencerContext *pPrismLinearSequencerContext);

               ResourceId         sendFileToHaPeer                            (const string &fthandle, const string &sourceFileName, const string &destinationFileName, const UI32 &fileSize);

               void               PushFileToHaPeerMessageHandler              (PushFileToHaPeerMessage *pPushFileToHaPeerMessage);
               ResourceId         pushFileToHaPeerValidateReceiptStep         (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
               ResourceId         pushFileToHaPeerReceiveFileStep             (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
               void               FilePushFileToHaPeerMessageHandler          (FilePushFileToHaPeerMessage *pFilePushFileToHaPeerMessage);
               ResourceId         pushFileToHaPeerValidateStep                (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
               ResourceId         pushFileToHaPeerSendFileStep                (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

               void               FileDeleteRequestFromServiceHandler         (FileDeleteRequestMessage *pFileDeleteRequestMessage);
               void               DeleteValidateStep                          ( PrismLinearSequencerContext *pPrismLinearSequencerContext ); 
               void               DeleteRequestFileTransferHandshake          (PrismLinearSequencerContext *pPrismLinearSequencerContext);
               void               DeleteRequestFileTransferHandshakeCallback  (WaveSendToClusterContext *pWaveSendToClusterContext);    
               void               sendDeleteMessageToAll                      (PrismLinearSequencerContext *pPrismLinearSequencerContext);  
               void               sendDeleteMessageToAllCallback              (WaveSendToClusterContext *pWaveSendToClusterContext); 

               void               DeleteFileMessageHandler                    (FileDeleteFileMessage *pFileDeleteFileMessage);
               void               DeleteFileStep                              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
               ResourceId         sendDeleteFileToHAPeer                      ( const string &filename, const UI32 &transferFlag );   

               void               DeleteFileOnHaPeerMessageHandler            (FileDeleteFileToHaPeerMessage *pFileDeleteFileToHaPeerMessage);
               void               DeleteFileHaPeerStep                        (PrismLinearSequencerContext *pPrismLinearSequencerContext);
               SI32               DeleteFile                                  (const string &filename);        

        protected:
        public:
               virtual                               ~FileLocalObjectManager                      ();
               static FileLocalObjectManager         *getInstance                                 ();
               static string                          getClassName                                ();
               static PrismServiceId                  getPrismServiceId                           ();
               UI32                                   getFileServiceVersion                       () const;

        // Now the data members
        private:
               UI32                                  m_nFileServiceVersion;                  // For future compatibility.
               class  CFileTransferManagedObject     m_FTTable;                              // File transfer table.

        protected:
        public:
    };

}
#endif    //  FILELOCALOBJECTMANAGER_H
