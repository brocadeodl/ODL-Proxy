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

/*************************************************************
 *   Copyright (C) 2010 Brocade Communications Systems, Inc  *
 *   All rights reserved.                                    *
 *   Author : Jayaraman Iyer                                 *
 *************************************************************/

#ifndef FILESVCLOCALMESSAGINGTESTOBJECTMANAGER_H
#define FILESVCLOCALMESSAGINGTESTOBJECTMANAGER_H

#include "Regression/PrismTestObjectManager.h"
#include "File/Local/FilePushFileMessage.h"

namespace WaveNs
{
class CopyFileWaveClientMessage;
class CopyFileToHaPeerWaveClientMessage;

class FileLocalMessagingTestObjectManager : public PrismTestObjectManager
{
     private :
          FileLocalMessagingTestObjectManager ();
          WaveMessageStatus                                                          simpleSynchronousMessageTest            (PrismMessage *message);
          WaveMessageStatus                                                          simpleAsynchronousMessageTest           (PrismLinearSequencerContext *&pPrismLinearSequencerContext, vector<PrismMessage *>&sagesVector);
          void                                                                       CleanupTempFiles                        (void);
          void                                                                       SetupTempFileForTransfer                (UI32 nFileSize, const string &sFileName);
          void                                                                       ASynchronousDistributeFileWith1MBDatatoAllGoodLocationIdTestCallback(FrameworkStatus frameworkStatus, FilePushFileMessage *pMessage, PrismLinearSequencerContext          
                                                                                                                                                           *pPrismLinearSequencerContext);
          void                                                                       ASyncPushFileWith1KDatatoGoodLocationIdTestCallback(FrameworkStatus frameworkStatus, FilePushFileMessage *pMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext);
          
     protected :
     public :
          virtual                                                                   ~FileLocalMessagingTestObjectManager     ();
          static  PrismServiceId                                                     getPrismServiceId                       ();
          void                                                                       testRequestHandler                      (RegressionTestMessage *pMessage);
          static  FileLocalMessagingTestObjectManager                               *getInstance                             ();
          static  string                                                             getServiceName                          ();
                  PrismMessage                                                      *createMessageInstance                   (const UI32 &operationCode);


          void    simplePushFileWithInvalidFilenameTest                              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
          void    simplePushFileWithInvalidFilesizeTest                              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
          void    simplePushFileWithInvalidLocationIdTest                            (PrismLinearSequencerContext *pPrismLinearSequencerContext);
          
          void    simpleSyncPushFileWith1KDatatoGoodLocationIdTest                   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
          void    simpleASyncPushFileWith1KDatatoGoodLocationIdTest                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
          
          void    SynchronousDistributeFileWith1MBDatatoAllGoodLocationIdTest        (PrismLinearSequencerContext *pPrismLinearSequencerContext);
          void    ASynchronousDistributeFileWith1MBDatatoAllGoodLocationIdTest       (PrismLinearSequencerContext *pPrismLinearSequencerContext);
          void    CleanupTempTestFiles                                               (PrismLinearSequencerContext *pPrismLinearSequencerContext);

          void    copyFileWaveClientMessageHandler                                   (CopyFileWaveClientMessage *pCopyFileWaveClientMessage);
          void    copyFileToHaPeerWaveClientMessageHandler                           (CopyFileToHaPeerWaveClientMessage *pCopyFileToHaPeerWaveClientMessage);

     // Now the data members
     private :
          static FileLocalMessagingTestObjectManager                             *m_pVcsNodeLocalMessagingTestObjectManager;

          

     protected :
     public:
};

}

#endif // FILESVCLOCALMESSAGINGTESTOBJECTMANAGER_H
