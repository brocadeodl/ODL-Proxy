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

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "File/Local/FileLocalObjectManager.h"
#include "File/Local/FileLocalTypes.h"
#include "File/Local/FileLocalMessages.h"
#include "Framework/Utils/StringUtils.h"
#include "WaveResourceIds.h"

#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace WaveNs
{

    FileLocalObjectManager::FileLocalObjectManager ()  : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (FILESVCPUSHFILE,              reinterpret_cast<PrismMessageHandler> (&FileLocalObjectManager::PushFileMessageHandler));    
        addOperationMap (FILESVCFILETRANSFERHANDSHAKE, reinterpret_cast<PrismMessageHandler> (&FileLocalObjectManager::FileTransferHandshakeMessageHandler));
        addOperationMap (FILESVCPUSHFILEFRAGMENT,      reinterpret_cast<PrismMessageHandler> (&FileLocalObjectManager::PushFileFragmentMessageHandler));
        addOperationMap (FILESVCABORTFILETRANSFER,     reinterpret_cast<PrismMessageHandler> (&FileLocalObjectManager::AbortFileTransferMessageHandler));
        addOperationMap (FILESVCPUSHFILETOHAPEER,      reinterpret_cast<PrismMessageHandler> (&FileLocalObjectManager::PushFileToHaPeerMessageHandler));
        addOperationMap (FILESVCFILEPUSHFILETOHAPEER,  reinterpret_cast<PrismMessageHandler> (&FileLocalObjectManager::FilePushFileToHaPeerMessageHandler));
        addOperationMap (FILESVCREQUESTDELETEFILE,     reinterpret_cast<PrismMessageHandler> (&FileLocalObjectManager::FileDeleteRequestFromServiceHandler));
        addOperationMap (FILESVCDELETEFILE,            reinterpret_cast<PrismMessageHandler> (&FileLocalObjectManager::DeleteFileMessageHandler));
        addOperationMap (FILESVCDELETEFILETOHAPEER,    reinterpret_cast<PrismMessageHandler> (&FileLocalObjectManager::DeleteFileOnHaPeerMessageHandler));     

        // Set the FileService Version.
        m_nFileServiceVersion = sFILESERVICEVERSION;

        // TraceLevel traceLevel = TRACE_LEVEL_DEVEL; setTraceLevel (traceLevel);
    }

    FileLocalObjectManager::~FileLocalObjectManager ()
    {
    }

    FileLocalObjectManager  *FileLocalObjectManager::getInstance()
    {
        static FileLocalObjectManager *pLocalObjectManager = new FileLocalObjectManager ();

        WaveNs::prismAssert (NULL != pLocalObjectManager, __FILE__, __LINE__);

        return (pLocalObjectManager);
    }

    UI32   FileLocalObjectManager::getFileServiceVersion()  const
    {
        return (m_nFileServiceVersion);
    }
    
    string  FileLocalObjectManager::getClassName()
    {
        return ("File Local Service");
    }

    PrismServiceId  FileLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FileLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FILESVCFILETRANSFERHANDSHAKE :
                pPrismMessage = new FileTransferHandshakeMessage ();
                break;
            case FILESVCPUSHFILE :
                pPrismMessage = new FilePushFileMessage ();
                break;
            case FILESVCPUSHFILEFRAGMENT :
                pPrismMessage = new FilePushFileFragmentMessage ();
                break;
            case FILESVCABORTFILETRANSFER :
                pPrismMessage = new FileAbortFileTransferMessage ();
                break;
            case FILESVCPUSHFILETOHAPEER :
                pPrismMessage = new PushFileToHaPeerMessage ();
                break;
            case FILESVCFILEPUSHFILETOHAPEER :
                pPrismMessage = new FilePushFileToHaPeerMessage ();
                break;
            case FILESVCREQUESTDELETEFILE :
                pPrismMessage = new FileDeleteRequestMessage ();
                break;
            case FILESVCDELETEFILE :
                pPrismMessage = new FileDeleteFileMessage ();
                break;
            case FILESVCDELETEFILETOHAPEER :
                pPrismMessage = new FileDeleteFileToHaPeerMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FileLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }
    
    
    void  FileLocalObjectManager::AbortFileTransferMessageHandler( FileAbortFileTransferMessage *pAbortFileTransferMessage)
    {
     // Validations and Queries should go here

          PrismLinearSequencerStep sequencerSteps[] =
          {
               // reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerStartTransactionStep),
               // Your configuration change code goes here

               // reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerCommitTransactionStep),
               // Programming Protocol Daemons goes here

               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::ProcessAbortFileTransferMessage),

               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerSucceededStep),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerFailedStep)
          };

          PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pAbortFileTransferMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

          WaveNs::prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);
        
          //        pPrismLinearSequencerContext->holdAll ();
          pPrismLinearSequencerContext->start (); 
     }


     ResourceId   FileLocalObjectManager::ProcessAbortFileTransferMessage (PrismLinearSequencerContext *pPrismLinearSequencerContext) 
     {
         ResourceId status = WAVE_MESSAGE_SUCCESS;

         tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

         FileAbortFileTransferMessage    *pAbortFTMessage = reinterpret_cast<FileAbortFileTransferMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
         WaveNs::prismAssert (NULL != pAbortFTMessage, __FILE__, __LINE__);
         
         // Before cleanup of incoming transfers.
         m_FTTable.DumpInfo ();

         vector<LocationId>  vecLocationId ; 
         do {
         
              vecLocationId    = pAbortFTMessage->getLocationdIdList();
              vector<LocationId>::iterator itr;
              for(itr = vecLocationId.begin(); itr != vecLocationId.end(); itr++)
              {
                    // Look up each locationID in the Context List on FT table, and if found, cleanup the corresponding file transfer request related data
                    tracePrintf (TRACE_LEVEL_INFO, "[%s][%d] LocationId = [%d] is un-reachable, any transfer to this node needs to be cleaned up ... ", __FILE__, __LINE__, *itr);
                    vector<string> vecFTHandle;
                    status = m_FTTable.getAllTransfersFromLocation(*itr, vecFTHandle);
                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                         continue; // shouldn't happen.
                    }
                    vector<string>::iterator itr_fthandle;
                    for(itr_fthandle = vecFTHandle.begin(); itr_fthandle != vecFTHandle.end(); itr_fthandle++)
                    {
                         tracePrintf (TRACE_LEVEL_INFO, "[%s][%d] Cleanup of fthandle = [%s] in progress ... ", __FILE__, __LINE__, itr_fthandle->c_str());
                         // m_FTTable.RemoveFromFTTable(*itr_fthandle);
                         m_FTTable.RemoveFromIncomingList(*itr_fthandle);
                         m_FTTable.RemoveFromContextList(*itr_fthandle);
                    }
                    // Information related to transfers to failed locations is not cleaned, 
                    // as that gets done during remaining failed callbacks from those locations.
              }
         
         } while(0);

         tracePrintf (TRACE_LEVEL_DEVEL, "[%s]:[%d] : After cleaning incoming transfers (if any.)", __FUNCTION__ , __LINE__);
         m_FTTable.DumpInfo ();
     
          // Respond back.
         pAbortFTMessage->setCompletionStatus(status);          
         pPrismLinearSequencerContext->executeNextStep (status);
         return (status);     
     }


     void  FileLocalObjectManager::FileTransferHandshakeMessageHandler( FileTransferHandshakeMessage *pFileTransferHandshakeMessage)
     {
          // Validations and Queries should go here
    	  tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
           
          PrismLinearSequencerStep sequencerSteps[] =
          {
//             reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerStartTransactionStep),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::validateLocationIdAndVersion),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::validateTransferFlag),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::addToIncomingQueue),
//             reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerCommitTransactionStep),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerSucceededStep),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerFailedStep)
          };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFileTransferHandshakeMessage, this, 
                                                                                                     sequencerSteps, 
                                                                                                     sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
                                                                                                     
        WaveNs::prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);
        
//      pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }

     void  FileLocalObjectManager::validateLocationIdAndVersion(PrismLinearSequencerContext *pPrismLinearSequencerContext)
     {
    	 tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
         FileTransferHandshakeMessage    *pFTHandShakeMessage = reinterpret_cast<FileTransferHandshakeMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
         ResourceId status = WAVE_MESSAGE_SUCCESS;
         WaveNs::prismAssert (NULL != pFTHandShakeMessage, __FILE__, __LINE__);
         do {

			// If the handshake request is coming from the current node, do nothing but return success.
			if (pFTHandShakeMessage->getSourceLocationId() == FrameworkToolKit::getThisLocationId())
			{
			   trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::ProcessFileTransferHandshakeRequest - Handshake request received for local node, returning success."));
			   break;
			}

			if (this->getFileServiceVersion() != pFTHandShakeMessage->getServiceVersion())
			{
			  trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::ProcessFileTransferHandshakeRequest - Service Version mismatch detected."));
			  status = FRAMEWORK_ERROR_CANNOT_BECOME_SECONDARY_WAVE_VERSION_MISMATCH;
			  break;
			}

         } while(0);

         // Respond back.
        pFTHandShakeMessage->setCompletionStatus(status);
        pPrismLinearSequencerContext->executeNextStep (status);

     }

     void  FileLocalObjectManager::validateTransferFlag(PrismLinearSequencerContext *pPrismLinearSequencerContext)
     {
         ResourceId status = WAVE_MESSAGE_SUCCESS;
         tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
         FileTransferHandshakeMessage    *pFTHandShakeMessage = reinterpret_cast<FileTransferHandshakeMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

         WaveNs::prismAssert (NULL != pFTHandShakeMessage, __FILE__, __LINE__);
         string sDestinationFileName = pFTHandShakeMessage->getDestinationFileName();
         string srcFileName = pFTHandShakeMessage->getSourceFileName ();
         string sDstFilename;
         UI32 nTransferFlag = pFTHandShakeMessage->getFileTransferFlag();

         if ( (true == srcFileName.empty()) && (false == sDestinationFileName.empty()) && ( 0 == pFTHandShakeMessage->getFileSize()) )
         {
            //This handshake message is for delete file
             status = WAVE_MESSAGE_SUCCESS;
             pPrismLinearSequencerContext->executeNextStep (status);
             return ;
         }
   
         do {

        	 if (nTransferFlag  & FILE_DEST_PATH_ABSOLUTE)
        	 {
        		  // If absolute path transfer flag is set then FILE_DEST_PATH_IWD & FILE_DEST_PATH_CWD cannot be set.
        		  if ((nTransferFlag & FILE_DEST_PATH_IWD) || (nTransferFlag & FILE_DEST_PATH_CWD))
        		  {
                      status = WAVE_FILE_INVALID_FILE_TRANSFER_FLAG_SPECIFIED;
                      tracePrintf (TRACE_LEVEL_INFO, "[%s][%d] Invalid TransferFlag encountered error [%d] for file [%s] , returning ... ", __FILE__, __LINE__, status, sDestinationFileName.c_str());
                      break;
        		  }
        	 }

        	 if (nTransferFlag  & FILE_DEST_PATH_IWD)
        	 {
        		  // If Initial_working_dir transfer flag is set then FILE_DEST_PATH_ABSOLUTE & FILE_DEST_PATH_CWD cannot be set.
        		  if ((nTransferFlag & FILE_DEST_PATH_ABSOLUTE) || (nTransferFlag & FILE_DEST_PATH_CWD))
        		  {
                      status = WAVE_FILE_INVALID_FILE_TRANSFER_FLAG_SPECIFIED;
                      tracePrintf (TRACE_LEVEL_INFO, "[%s][%d] Invalid TransferFlag encountered error [%d] for file [%s] , returning ... ", __FILE__, __LINE__, status, sDestinationFileName.c_str());
                      break;
        		  }

  				// Prepend the current working directory to the destination filename.
  				sDstFilename = FrameworkToolKit::getProcessInitialWorkingDirectory() + "/" + sDestinationFileName;
  				pFTHandShakeMessage->setDestinationFileName(sDstFilename);

        	 }

        	 if (nTransferFlag  & FILE_DEST_PATH_CWD)
        	 {
        		  // If current_working_dir transfer flag is set then FILE_DEST_PATH_ABSOLUTE & FILE_DEST_PATH_IWD cannot be set.
        		  if ((nTransferFlag & FILE_DEST_PATH_ABSOLUTE) || (nTransferFlag & FILE_DEST_PATH_IWD))
        		  {
                      status = WAVE_FILE_INVALID_FILE_TRANSFER_FLAG_SPECIFIED;
                      tracePrintf (TRACE_LEVEL_INFO, "[%s][%d] Invalid TransferFlag encountered error [%d] for file [%s] , returning ... ", __FILE__, __LINE__, status, sDestinationFileName.c_str());
                      break;
        		  }

        		  // Prepend the current working directory to the destination filename.
        		  sDstFilename = FrameworkToolKit::getProcessCurrentWorkingDirectory() + "/" + sDestinationFileName;
        		  pFTHandShakeMessage->setDestinationFileName(sDstFilename);
        	 }

            if ( (true == srcFileName.empty()) && (false == sDestinationFileName.empty()) && ( 0 == pFTHandShakeMessage->getFileSize()) )
            {
                //This handshake message is for delete file
                status = WAVE_MESSAGE_SUCCESS;
                break;
            }
      
             // Now check Overwrite flag.
             struct stat file_statbuf = {0};
             int rc = stat(sDstFilename.c_str() , &file_statbuf);
             if (!rc)
             {
                   // If file is present and overwrite flag is not set then return an error.
                   // == OR & ??
                   if (nTransferFlag == FILE_FAIL_IF_DEST_FILE_EXIST) {
                        status = WAVE_FILE_FILE_ALREADY_EXISTS_AT_DESTINATION;
                        tracePrintf (TRACE_LEVEL_INFO, "[%s][%d] TransferFlag is set to fail file transfer if destination file [%s] exist. Error [%d] returning ... ", __FILE__, __LINE__, sDstFilename.c_str(), status);
                        break;
                   }
                   break;
             } else {
                   // TODO: Need to look at the exact reason for stat failure and act appropriately.
                   // status = WAVE_FILE_STAT_OPERATION_FAILED;
                   // tracePrintf (TRACE_LEVEL_INFO, "[%s][%d] Unable to stat the file [%s]. Error [%d] returning ... ", __FILE__, __LINE__, sDstFilename.c_str(), status);
                   // break;
             }

         } while(0);

         pFTHandShakeMessage->setCompletionStatus(status);
         pPrismLinearSequencerContext->executeNextStep (status);
     }

     void  FileLocalObjectManager::addToIncomingQueue(PrismLinearSequencerContext *pPrismLinearSequencerContext)
     {
    	tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
		FileTransferHandshakeMessage    *pFTHandShakeMessage = reinterpret_cast<FileTransferHandshakeMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		WaveNs::prismAssert (NULL != pFTHandShakeMessage, __FILE__, __LINE__);
        string sDestinationFileName = pFTHandShakeMessage->getDestinationFileName();
        string srcFileName = pFTHandShakeMessage->getSourceFileName ();

        if ( (true == srcFileName.empty()) && (false == sDestinationFileName.empty()) && ( 0 == pFTHandShakeMessage->getFileSize()) )
        {
            //This handshake message is for delete file
            status = WAVE_MESSAGE_SUCCESS;
            pPrismLinearSequencerContext->executeNextStep (status);
            return ;
        }        

        // Before adding to incoming queue.
        m_FTTable.DumpInfo ();

		// Add to the active transfer table.
		status = m_FTTable.AddToInComingQueueInFTTable(pFTHandShakeMessage);
		pFTHandShakeMessage->setCompletionStatus(status);

        tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d] Added transfer to incoming queue.", __FUNCTION__, __LINE__);
        m_FTTable.DumpInfo ();

		pPrismLinearSequencerContext->executeNextStep (status);
     }
     
     void  FileLocalObjectManager::PushFileFragmentMessageHandler( FilePushFileFragmentMessage *pPushFileFragmentMessage)
     {

    	 tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

          PrismLinearSequencerStep sequencerSteps[] =
          {
//             reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerStartTransactionStep),
               // Your configuration change code goes here

               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::ProcessFileFragmentMessage),
//             reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerCommitTransactionStep),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerSucceededStep),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerFailedStep)
          };

          PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPushFileFragmentMessage, this, 
                                                                                                       sequencerSteps, 
                                                                                                       sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
                                                                                                       
          WaveNs::prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);
                                                                                                                                                                                                              
//          pPrismLinearSequencerContext->holdAll ();
          pPrismLinearSequencerContext->start ();
     }



     ResourceId FileLocalObjectManager::ProcessFileFragmentMessage (PrismLinearSequencerContext *pPrismLinearSequencerContext)
     {

         ResourceId status = WAVE_MESSAGE_SUCCESS;
         tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
                  
         FilePushFileFragmentMessage *pPushFileFragmentMessage = reinterpret_cast<FilePushFileFragmentMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
         WaveNs::prismAssert (NULL != pPushFileFragmentMessage, __FILE__, __LINE__);         

          do {

               // Get the FTHandle and look the transfer table.
               string sFTHandle = pPushFileFragmentMessage->getTransferHandle();
               UI32  nFragmentSize = pPushFileFragmentMessage->getFragmentSize();

               tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d] Received fragment index [%d] for fthandle = [%s], fragment size = [%d] \n", __FUNCTION__, __LINE__, pPushFileFragmentMessage->getFileFragmentIndex(), sFTHandle.c_str(), nFragmentSize);

               unsigned char *pDataBuffer = reinterpret_cast<unsigned char *> (pPushFileFragmentMessage->findBuffer((UI32)FILESVC_FILE_FRAGMENT_BUFFER_TAG, nFragmentSize));

               WaveNs::prismAssert (NULL != pDataBuffer, __FILE__, __LINE__);

               status = m_FTTable.SaveFragmentToTempFile(pPushFileFragmentMessage->getTransferHandle(),
                                                       pPushFileFragmentMessage->getFileFragmentIndex(),
                                                       pDataBuffer, nFragmentSize,
                                                       pPushFileFragmentMessage->IsLastFragment());

               // If this was the last file fragment received for this transfer handle, then cleanup the FT table.
               if (true == pPushFileFragmentMessage->IsLastFragment())
               {
                    bool fileNeedsToBeSentToHaPeer = m_FTTable.doesFileNeedToBeSentToHaPeer (sFTHandle);

                    if (true == fileNeedsToBeSentToHaPeer)
                    {
                        string  fileName                = "";
                        UI32    fileSize                = 0;

                        m_FTTable.getFileDetailsForIncomingFile (sFTHandle, fileName, fileSize);
                        prismAssert (0 != fileSize, __FILE__, __LINE__);
                        prismAssert (false == fileName.empty (), __FILE__, __LINE__);

                        status = sendFileToHaPeer (sFTHandle, fileName, fileName, fileSize);
                    }

                    m_FTTable.RemoveFromFTTable(pPushFileFragmentMessage->getTransferHandle());
                    // Table should not have entries for current file handle.
                    // This will get printed at each receiver location while processing last fragment.
                    tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d] Processing of received last fragment is over.", __FUNCTION__, __LINE__);
                    m_FTTable.DumpInfo ();
               }

         }  while(0);

         // for failover testing.
         // WaveNs::prismSleep (1);
	 
         pPushFileFragmentMessage->setCompletionStatus(status);
         pPrismLinearSequencerContext->executeNextStep (status);
         return (status);
     }

     ResourceId FileLocalObjectManager::sendFileToHaPeer (const string &fthandle, const string &sourceFileName, const string &destinationFileName, const UI32 &fileSize)
     {
        ResourceId returnStatus = WAVE_MESSAGE_SUCCESS;

        do
        {
            // 1. Check if Ha Peer is connected to this Wave server.

            ResourceId haPeerConnectionStatus = FrameworkToolKit::isConnectedToHaPeerLocation ();

            if (WAVE_MESSAGE_SUCCESS != haPeerConnectionStatus)
            {
                // So, this Wave server does not have/know about any Ha Peer.
                // No need to send file, since Ha Peer is not present/available.
                // Treat this as success.

                // FIXME However, if a standby module/machine/switch is present, but connection is not available, then need to return error.
                // (Need to call API provided by 'Wave HA infrastructure'.)

                // returnStatus = haPeerConnectionStatus;

                break;
            }

            LocationId  haPeerLocationId = FrameworkToolKit::getHaPeerLocationId ();

            if (0 == haPeerLocationId)
            {
                trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::sendFileToHaPeer : FrameworkToolKit::getHaPeerLocationId returned [0], file handle [") + fthandle + string ("]"));
                returnStatus = WAVE_MESSAGE_ERROR;

                break;
            }

            // 2. Verify if Ha Peer is capable of handling this message. (Ha Peer version >= this version)

            // FIXME This should be handled in general for HA Peer connection.
            // (This is required even for non-clustered Wave Servers having incapable/mismatching Ha Peers, though not for Cluster-Push-File)
            // (Need to call API provided by 'Wave HA infrastructure'.)


            // 3. Prepare PushFileToHaPeerMessage.

            ifstream srcFile;
            srcFile.open (sourceFileName.c_str (), ios::binary);

            if (false == srcFile.is_open ())
            {
                trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::sendFileToHaPeer : could not open ifstream for [") + sourceFileName + string ("]"));
                returnStatus = WAVE_MESSAGE_ERROR;
                break;
            }

            char *pFileBuffer = (char *) new UI8[fileSize];
            prismAssert (NULL != pFileBuffer, __FILE__, __LINE__);

            srcFile.read (pFileBuffer, fileSize);
            srcFile.close ();

            PushFileToHaPeerMessage pushFileToHaPeerMessage (getFileServiceVersion (), fthandle, sourceFileName, destinationFileName, fileSize);

            // Let destructor of pushFileToHaPeerMessage object delete the buffer.
            // This will also avoid copying of file buffer.
            bool bufferWillBeOwnedByMessage = true;

            pushFileToHaPeerMessage.addBuffer (FILESVC_COMPLETE_FILE_BUFFER_FOR_HA_PEER, fileSize, pFileBuffer, bufferWillBeOwnedByMessage);

            // 4. Send the message to Ha Peer.

            ResourceId sendStatus = sendSynchronously (&pushFileToHaPeerMessage, haPeerLocationId);

            if (WAVE_MESSAGE_SUCCESS != sendStatus)
            {
                trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::sendFileToHaPeer : sendSynchronously to haPeerLocationId [") + haPeerLocationId + string ("], returned [") + FrameworkToolKit::localizeToSourceCodeEnum (sendStatus) + string ("]"));
                returnStatus = sendStatus;

                break;
            }

            ResourceId completionStatus = pushFileToHaPeerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::sendFileToHaPeer : message processing failed on haPeerLocationId [") + haPeerLocationId + string ("], completionStatus [") + FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
                returnStatus = completionStatus;

                break;
            }

        } while (0);

        return returnStatus;
     }

    void FileLocalObjectManager::DeleteFileOnHaPeerMessageHandler (FileDeleteFileToHaPeerMessage *pFileDeleteFileToHaPeerMessage)
    {
        // Validations and Queries should go here
        tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::DeleteFileHaPeerStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFileDeleteFileToHaPeerMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        WaveNs::prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);
        pPrismLinearSequencerContext->start ();
    }    

    void FileLocalObjectManager::DeleteFileHaPeerStep ( PrismLinearSequencerContext *pPrismLinearSequencerContext )
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        FileDeleteFileToHaPeerMessage *pFileDeleteFileToHaPeerMessage = reinterpret_cast<FileDeleteFileToHaPeerMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
        string filename = pFileDeleteFileToHaPeerMessage->getDestinationFileName();

        SI32 ret = DeleteFile (filename);

        if ( 0 != ret )
        {
            status = WAVE_MESSAGE_ERROR;
        }

        tracePrintf(TRACE_LEVEL_INFO, "[%s]::[%d] : status -%s", __FUNCTION__, __LINE__,(FrameworkToolKit::localize(status)).c_str());

        pPrismLinearSequencerContext->executeNextStep (status);
        return ;
    }  

    void FileLocalObjectManager::DeleteFileMessageHandler ( FileDeleteFileMessage *pFileDeleteFileMessage )
    {
        // Validations and Queries should go here
        tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::DeleteFileStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerSucceededStep),                                      reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFileDeleteFileMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        WaveNs::prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);
        pPrismLinearSequencerContext->start ();
    }

    void FileLocalObjectManager::DeleteFileStep ( PrismLinearSequencerContext *pPrismLinearSequencerContext )
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

        FileDeleteFileMessage *pFileDeleteFileMessage = reinterpret_cast<FileDeleteFileMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
        string filename = pFileDeleteFileMessage->getDestinationFileName();

        SI32 ret = DeleteFile (filename); 
    
        if ( 0 != ret )
        {
            status = WAVE_MESSAGE_ERROR;
            pPrismLinearSequencerContext->executeNextStep (status);
            return ;            
        }
    
        UI32 fileTransferFlag = pFileDeleteFileMessage->getFileTransferFlag();

        if (fileTransferFlag & FILE_PUSH_TO_HA_PEER) 
        {
            status = sendDeleteFileToHAPeer(filename, fileTransferFlag);
        }
        tracePrintf(TRACE_LEVEL_INFO, "[%s]::[%d] : status -%s", __FUNCTION__ , __LINE__,(FrameworkToolKit::localize(status)).c_str());

        pPrismLinearSequencerContext->executeNextStep (status);
        return ;
    }

    SI32 FileLocalObjectManager::DeleteFile( const string &filename)
    {
        vector<string>  commandOutput;
        SI32            ret = 0;
        string cmd = "/bin/rm -f " + filename ;

        ret = FrameworkToolKit::systemCommandOutput (cmd, commandOutput );

        // Should we treat file not present as success    
        if ( 0 != ret )
        {   
            tracePrintf(TRACE_LEVEL_ERROR, "[%s]::[%d] : Failed to delete file -%s with ret code - %d", __FUNCTION__ , __LINE__, filename.c_str(), ret);
        }
        return (ret);
    }

    ResourceId FileLocalObjectManager::sendDeleteFileToHAPeer ( const string &filename, const UI32 &transferFlag )
    {

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        status = FrameworkToolKit::isConnectedToHaPeerLocation ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
                // So, this Wave server does not have/know about any Ha Peer.
                // No need to send file, since Ha Peer is not present/available.
                // Treat this as success.

                // FIXME However, if a standby module/machine/switch is present, but connection is not available, then need to return error.
                // (Need to call API provided by 'Wave HA infrastructure'.)

            return (WAVE_MESSAGE_SUCCESS);
        }

        LocationId  haPeerLocationId = FrameworkToolKit::getHaPeerLocationId ();

        if (0 == haPeerLocationId)
        {
            trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::sendDelteFileToHaPeer : FrameworkToolKit::getHaPeerLocationId returned [0], filename -") + filename.c_str());
            return (WAVE_MESSAGE_ERROR);
        }

        FileDeleteFileToHaPeerMessage *pFileDeleteFileToHaPeerMessage = new FileDeleteFileToHaPeerMessage (filename, transferFlag);
        status = sendSynchronously (pFileDeleteFileToHaPeerMessage, haPeerLocationId); 

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("FileLocalObjectManager::sendDeleteFileToHaPeer : sendSynchronously to haPeerLocationId [") + haPeerLocationId + string ("], returned [") + FrameworkToolKit::localizeToSourceCodeEnum (status) + string ("]"));
        }
        else
        {
            status = pFileDeleteFileToHaPeerMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, string ("FileLocalObjectManager::sendDeleteFileToHaPeer : message processing failed on haPeerLocationId [") + haPeerLocationId + string ("], completionStatus [") + FrameworkToolKit::localizeToSourceCodeEnum (status) + string ("]"));
            }
        }

        if (NULL != pFileDeleteFileToHaPeerMessage)
        {
            delete (pFileDeleteFileToHaPeerMessage);
            pFileDeleteFileToHaPeerMessage = NULL;
        }

        return (status);
    }

    void FileLocalObjectManager::FileDeleteRequestFromServiceHandler ( FileDeleteRequestMessage *pFileDeleteRequestMessage )    
    {
        // Validations and Queries should go here
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::DeleteValidateStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::DeleteRequestFileTransferHandshake),
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::sendDeleteMessageToAll),
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pFileDeleteRequestMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
                                                                                                       
        WaveNs::prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);
        pPrismLinearSequencerContext->start ();
    }

    void FileLocalObjectManager::DeleteValidateStep ( PrismLinearSequencerContext *pPrismLinearSequencerContext )
    {
         ResourceId status = WAVE_MESSAGE_SUCCESS;
         tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
    
         FileDeleteRequestMessage *pFileDeleteRequestMessage = dynamic_cast<FileDeleteRequestMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        
         WaveNs::prismAssert (NULL != pFileDeleteRequestMessage, __FILE__, __LINE__);
 
         do
         {
            string filename = pFileDeleteRequestMessage->getDestinationFileName();

            if ( true == filename.empty())
            {
                status = WAVE_FILE_INVALID_FILENAME;
                trace (TRACE_LEVEL_ERROR, string ("FileLocalObjectManager::DeleteValidateStep :  Empty Destination filename for delete"));
                break;
            }
            
            UI32 transferFlag = pFileDeleteRequestMessage->getFileTransferFlag();

            if ( !(transferFlag & FILE_DEST_PATH_ABSOLUTE) )
            {
                status = WAVE_FILE_INVALID_FILE_TRANSFER_FLAG_SPECIFIED;
                trace (TRACE_LEVEL_ERROR, string ("FileLocalObjectManager::DeleteValidateStep : To Delete a file specify absolute path, given filename -") + filename );  
                break;
            }
    
            vector<UI32> vecDestinationList = pFileDeleteRequestMessage->getDestinationLocationIdListForNextMessage();

            // If this list is empty, return an error.
            if (0 == vecDestinationList.size())
            {
                trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::DeleteValidateStep - Destination Location List is empty. Cannot proceed."));
                status = WAVE_FILE_INVALID_DESTINATION_NODE;
                break;
            }

            // Src filename is good, check if source is not this location.
            UI32 nSrcLocationId = FrameworkToolKit::getMyLocationId(); 
            vector<UI32>::iterator itr = vecDestinationList.begin();

            for(; itr != vecDestinationList.end() ; itr++)
            {
                if (*itr == nSrcLocationId)  // source node cannot be in the destination list.
                {
                    trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::DeleteValidateStep - Current Node LocationdId [") + nSrcLocationId + "] is among the destination node list also. This is not supported.\n");
                    status = WAVE_FILE_INVALID_SOURCE_NODE;
                    break;         
                }

                if (false == FrameworkToolKit::isAConnectedLocation(*itr))
                {
                    trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::DeleteValidateStep - Bad Input Destination LocationdId, [") + *itr + "], not connected to this locationId." );
                    status = WAVE_FILE_INVALID_DESTINATION_NODE;
                    break;
                }
            }
         }while (0);

         pPrismLinearSequencerContext->executeNextStep (status);
    }

    void FileLocalObjectManager::DeleteRequestFileTransferHandshake ( PrismLinearSequencerContext *pPrismLinearSequencerContext )
    {
        tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

        ++(*pPrismLinearSequencerContext);

        FileDeleteRequestMessage *pFileDeleteRequestMessage = reinterpret_cast<FileDeleteRequestMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

        FileTransferHandshakeMessage  *pHandShakeMessage = new FileTransferHandshakeMessage(this->getFileServiceVersion(), string(""), pFileDeleteRequestMessage->getDestinationFileName(), (FrameworkToolKit::getMyLocationId ()), 0, pFileDeleteRequestMessage->getFileTransferFlag());

        WaveNs::prismAssert (NULL != pHandShakeMessage, __FILE__, __LINE__);   

        WaveSendToClusterContext     *pWaveSendToClusterContext   = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FileLocalObjectManager::DeleteRequestFileTransferHandshakeCallback), pPrismLinearSequencerContext);

        pWaveSendToClusterContext->setPPrismMessageForPhase1 (pHandShakeMessage);
        pWaveSendToClusterContext->setLocationsToSendToForPhase1( pFileDeleteRequestMessage->getDestinationLocationIdListForNextMessage());
        sendToWaveCluster (pWaveSendToClusterContext);
        tracePrintf(TRACE_LEVEL_DEBUG, "FileLocalObjectManager::DeleteRequestFileTransferHandshake : After sending to cluster status "); 
    }
    
    void FileLocalObjectManager::DeleteRequestFileTransferHandshakeCallback ( WaveSendToClusterContext *pWaveSendToClusterContext )
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

        PrismLinearSequencerContext *pPrismLinearSequencerContext  = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
        prismAssert (pPrismLinearSequencerContext, __FILE__, __LINE__);

        FileDeleteRequestMessage *pFileDeleteRequestMessage = reinterpret_cast<FileDeleteRequestMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
        prismAssert (NULL != pFileDeleteRequestMessage, __FILE__, __LINE__);

        --(*pPrismLinearSequencerContext);

        string filename = pFileDeleteRequestMessage->getDestinationFileName();

        vector<LocationId>  vecLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Total LocationId that responded is [%d]. ", __FUNCTION__ , __LINE__ , vecLocations.size());

        for (vector<LocationId>::iterator itr = vecLocations.begin(); itr != vecLocations.end(); itr++)
        {
              ResourceId sendStatus = pWaveSendToClusterContext->getSendStatusForPhase1 (*itr);

              if (WAVE_MESSAGE_SUCCESS != sendStatus)
              {
                  tracePrintf (TRACE_LEVEL_ERROR, "[%s]:[%d]: Handshake response from [%d] for destFilename [%s], sendStatus [0x%x]", __FILE__, __LINE__ , *itr, filename.c_str(), sendStatus);
                  // Remove this node from the list to which subsequent messages are sent (Fragment send)
                  pFileDeleteRequestMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              // Check the forward send status, if success the proceed further, else remove this location from the list for next phase.
              FrameworkStatus frameworkStatus  = pWaveSendToClusterContext->getFrameworkStatusForPhase1 (*itr);

              if (FRAMEWORK_SUCCESS != frameworkStatus)
              {
                  tracePrintf (TRACE_LEVEL_ERROR, "[%s]:[%d]: Handshake response from [%d] for Filename [%s], FrameworkStatus [0x%x]", __FILE__, __LINE__ , *itr, filename.c_str(), frameworkStatus);
                  // Remove this node from the list to which subsequent messages are sent 
                  pFileDeleteRequestMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              ResourceId completionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(*itr);

              if (WAVE_MESSAGE_SUCCESS != completionStatus)
              {
                  tracePrintf (TRACE_LEVEL_ERROR, "[%s]:[%d]: Handshake response from [%d] for Filename [%s], completionStatus [0x%x]", __FILE__, __LINE__ , *itr, filename.c_str(), completionStatus);
                  // Add this node to the failed list,
                  pFileDeleteRequestMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d]: Handshake response from [%d] for Filename [%s], CompletionCode [0x%x], FrameworkStatus [0x%x]\n", __FILE__, __LINE__ , *itr, filename.c_str(), completionStatus, frameworkStatus);

        }    

        delete pWaveSendToClusterContext->getPPrismMessageForPhase1 ();  // Delete the handshake message we created.
        delete pWaveSendToClusterContext;

        status = ((pFileDeleteRequestMessage->getLocationCountToReceiveNextMessage() > 0) ? WAVE_MESSAGE_SUCCESS : WAVE_MESSAGE_ERROR);

        if (WAVE_MESSAGE_SUCCESS != status)  
        {
            tracePrintf (TRACE_LEVEL_ERROR, "[%s]:[%d]: No Location to receive next message\n", __FILE__, __LINE__);
        }

        pPrismLinearSequencerContext->executeNextStep (status);
        return ;
    }

    void FileLocalObjectManager::sendDeleteMessageToAll ( PrismLinearSequencerContext *pPrismLinearSequencerContext )
    {
        tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
        
        ++(*pPrismLinearSequencerContext);

        FileDeleteRequestMessage *pFileDeleteRequestMessage = reinterpret_cast<FileDeleteRequestMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
        prismAssert (NULL != pFileDeleteRequestMessage, __FILE__, __LINE__);

        FileDeleteFileMessage *pMessage = new FileDeleteFileMessage (pFileDeleteRequestMessage->getDestinationFileName(), pFileDeleteRequestMessage->getFileTransferFlag());
        prismAssert (NULL != pMessage, __FILE__, __LINE__);

        WaveSendToClusterContext     *pWaveSendToClusterContext   = new WaveSendToClusterContext (this,
                                                                           reinterpret_cast<PrismAsynchronousCallback> (&FileLocalObjectManager::sendDeleteMessageToAllCallback),
                                                                           pPrismLinearSequencerContext);

        pWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage);
        pWaveSendToClusterContext->setLocationsToSendToForPhase1( pFileDeleteRequestMessage->getDestinationLocationIdListForNextMessage());
        sendToWaveCluster (pWaveSendToClusterContext);
        tracePrintf(TRACE_LEVEL_INFO, "FileLocalObjectManager::sendDeleteMessageToAll : After sending to cluster");
    }    

    void FileLocalObjectManager::sendDeleteMessageToAllCallback ( WaveSendToClusterContext *pWaveSendToClusterContext )
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

        PrismLinearSequencerContext *pPrismLinearSequencerContext  = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
        prismAssert (pPrismLinearSequencerContext, __FILE__, __LINE__);

        FileDeleteRequestMessage *pFileDeleteRequestMessage = reinterpret_cast<FileDeleteRequestMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
        prismAssert (NULL != pFileDeleteRequestMessage, __FILE__, __LINE__);

        --(*pPrismLinearSequencerContext);

        string filename = pFileDeleteRequestMessage->getDestinationFileName();

        vector<LocationId>  vecLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Total LocationId that responded is [%d]. ", __FUNCTION__ , __LINE__ , vecLocations.size());

        for (vector<LocationId>::iterator itr = vecLocations.begin(); itr != vecLocations.end(); itr++)
        {
              ResourceId sendStatus = pWaveSendToClusterContext->getSendStatusForPhase1 (*itr);

              if (WAVE_MESSAGE_SUCCESS != sendStatus)
              {   
                  tracePrintf (TRACE_LEVEL_ERROR, "[%s]:[%d]: Message response from [%d] for destFilename [%s], sendStatus [0x%x]", __FILE__, __LINE__ , *itr, filename.c_str(), sendStatus);
                  // Remove this node from the list to which subsequent messages are sent (Fragment send)
                  pFileDeleteRequestMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              // Check the forward send status, if success the proceed further, else remove this location from the list for next phase.
              FrameworkStatus frameworkStatus  = pWaveSendToClusterContext->getFrameworkStatusForPhase1 (*itr);

              if (FRAMEWORK_SUCCESS != frameworkStatus)
              {
                  tracePrintf (TRACE_LEVEL_ERROR, "[%s]:[%d]: Message response from [%d] for Filename [%s], FrameworkStatus [0x%x]", __FILE__, __LINE__ , *itr, filename.c_str(), frameworkStatus);
                  // Remove this node from the list to which subsequent messages are sent 
                  pFileDeleteRequestMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              ResourceId completionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(*itr);

              if (WAVE_MESSAGE_SUCCESS != completionStatus)
              {   
                  tracePrintf (TRACE_LEVEL_ERROR, "[%s]:[%d]: Message response from [%d] for Filename [%s], completionStatus [0x%x]", __FILE__, __LINE__ , *itr, filename.c_str(), completionStatus);
                  // Add this node to the failed list,
                  pFileDeleteRequestMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d]: Message response from [%d] for Filename [%s], CompletionCode [0x%x], FrameworkStatus [0x%x]\n", __FILE__, __LINE__ , *itr, filename.c_str(), completionStatus, frameworkStatus);

        }        
        
        delete pWaveSendToClusterContext->getPPrismMessageForPhase1 ();  // Delete the message we created
        delete pWaveSendToClusterContext;

        vector<UI32> destinationList = pFileDeleteRequestMessage->getDestinationLocationIdList ();
        vector<UI32> locationSucceeded = pFileDeleteRequestMessage->getDestinationLocationIdListForNextMessage();
        vector<ResourceId> resp;

        for ( UI32 j = 0; j < destinationList.size(); j++ )
        {
            bool found = false;
            for ( UI32 k = 0; k < locationSucceeded.size(); k++)
            {    
                if ( locationSucceeded[k] == destinationList[j] )
                {
                    found = true;
                    break;
                }   
            }     

            if (found)
            {
                resp.push_back( WAVE_MESSAGE_SUCCESS );
            }
            else
            {
                tracePrintf (TRACE_LEVEL_ERROR, "[%s]:[%d]: Sending delete message has failed for Location = %d\n",__FILE__, __LINE__ , destinationList[j]);
                
                resp.push_back( WAVE_MESSAGE_ERROR );
            }
        }

        pFileDeleteRequestMessage->setResponseCodeList(resp);
        pPrismLinearSequencerContext->executeNextStep (status);
        return ;
    }

    void  FileLocalObjectManager::PushFileMessageHandler( FilePushFileMessage *pPushFileMessage)
    {
          // Validations and Queries should go here
    	  tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

          PrismLinearSequencerStep sequencerSteps[] =
          {
               // Data validation first, if it fails, we bail out.
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::ValidateInputs),
//             reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerStartTransactionStep),
               // Your configuration change code goes here
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::TriggerAsyncFileTransferHandshake),
//             reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::TriggerFileTransferHandshake),               
//             reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::StartFileTransfer),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::StartAsyncFileTransfer),
//             reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerCommitTransactionStep),
               // Programming Protocol Daemons goes here
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerSucceededStep),
               reinterpret_cast<PrismLinearSequencerStep>(&FileLocalObjectManager::prismLinearSequencerFailedStep)
          };

          PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pPushFileMessage, this, 
                                                                                                       sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
                                                                                                       
          WaveNs::prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);
                                                                                                                                                                                                              
          // pPrismLinearSequencerContext->holdAll ();
          pPrismLinearSequencerContext->start ();
    }

     ResourceId FileLocalObjectManager::ValidateInputs (PrismLinearSequencerContext *pPrismLinearSequencerContext)
     {

         ResourceId status = WAVE_MESSAGE_SUCCESS;
         tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
    
         FilePushFileMessage *pPushFileMessage = reinterpret_cast<FilePushFileMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
         
         WaveNs::prismAssert (NULL != pPushFileMessage, __FILE__, __LINE__);

         do {
              // check if the source filename is good.
              string sSrcFilename = pPushFileMessage->getSourceFileName();
              if (sSrcFilename.empty()) {
                    trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::ValidateInputs - Empty input source filename, "));
                    status = WAVE_FILE_INVALID_FILENAME;
                    break;
              }
              
              struct stat file_statbuf = {0};
              int rc = stat(sSrcFilename.c_str() , &file_statbuf);
              if (rc) {
                    // Input file is not good. return error here.
                    trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::ValidateInputs - Bad input source filename, stat() returned:[") + rc + "]  for filename " + sSrcFilename);
                    status = WAVE_FILE_INVALID_FILENAME;              
                    break;
              }

               if (0 == file_statbuf.st_size) 
               {
                    // If it's an empty file, we don't allow its transfer.
                    trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::ValidateInputs - Filesize returned by stat() is ZERO for filename ") + sSrcFilename);
                    status = WAVE_FILE_INVALID_FILESIZE;
                    break;
               }                             
              
               // Src filename is good, check if source is not this location.
               UI32 nSrcLocationId = pPushFileMessage->getSourceLocationId();
               if (nSrcLocationId != FrameworkToolKit::getThisLocationId())
               {
                    trace(TRACE_LEVEL_INFO, string("FileLocalObjectManager::ValidateInputs - Bad input source LocationdId, [") + nSrcLocationId  + "] is not current node [" + FrameworkToolKit::getThisLocationId() + "]\n");
                    status = WAVE_FILE_INVALID_SOURCE_NODE;
                    break;
               }

               vector<UI32> vecDestinationList = pPushFileMessage->getDestinationLocationIdListForNextMessage();

               // If this list is empty, return an error.
               if (0 == vecDestinationList.size())
               {
                    trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::ValidateInputs - Destination Location List is empty. Cannot proceed."));
                    status = WAVE_FILE_INVALID_DESTINATION_NODE;
                    break;
               }

               vector<UI32>::iterator itr = vecDestinationList.begin();
               for(; itr != vecDestinationList.end() ; itr++)
               {
                    if (*itr == nSrcLocationId)  // source node cannot be in the destination list.
                    {
                         trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::ValidateInputs - Current Node LocationdId [") + nSrcLocationId + "] is among the destination node list also. This is not supported.\n");
                         status = WAVE_FILE_INVALID_SOURCE_NODE;
                         break;			
                    }

                    if (false == FrameworkToolKit::isAConnectedLocation(*itr))
                    {
                         trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::ValidateInputs - Bad Input Destination LocationdId, [") + *itr + "], not connected to this locationId." );
                         status = WAVE_FILE_INVALID_DESTINATION_NODE;
                         break;
                    }
               }

         } while(0);

         // The transferflag and the DestinationFilename validation will happen as part of the filetransfer handshake in the next step.
         pPushFileMessage->setCompletionStatus(status);
         pPrismLinearSequencerContext->executeNextStep (status);
         return (status);
     }

               
      ResourceId   FileLocalObjectManager::TriggerAsyncFileTransferHandshake(PrismLinearSequencerContext *pPrismLinearSequencerContext)
      {
          ResourceId status = WAVE_MESSAGE_SUCCESS;
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
          
       	  ++(*pPrismLinearSequencerContext);
       	
          do {
               FilePushFileMessage *pPushFileMessage = reinterpret_cast<FilePushFileMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
               WaveNs::prismAssert (NULL != pPushFileMessage, __FILE__, __LINE__);
               string sSrcFilename = pPushFileMessage->getSourceFileName();
               // Get the Source file size first.
               struct stat file_statbuf = {0};
               int rc = stat(sSrcFilename.c_str() , &file_statbuf);
               if (rc)
               {
                    // Input file is not good. return error here.
                    trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::TriggerAsyncFileTransferHandshake - Bad input source filename, stat() returned:[") + rc + "]  for filename " + sSrcFilename);
                    status = WAVE_FILE_INVALID_FILENAME;              
                    break;               
               }
               
               UI32 nFileSize = file_statbuf.st_size;
               // trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::ATriggerFileTransferHandshake - stat() succeeded. Filesize = [") + nFileSize + "]  for filename " + sSrcFilename);
               // Need to check if this file to be sent out is already being received by this node from somewhere else, in which case it should be in the Incoming FTList.
               if (true == m_FTTable.IsInComingTransferList(sSrcFilename))
               {
                    trace (TRACE_LEVEL_INFO, string ("CFileTransferTable::TriggerFileTransferHandshake: Source file [") + sSrcFilename + "] involved in another incoming transfer. returning ... ");
                    status = WAVE_FILE_DESTINATION_FILE_BUSY_IN_ANOTHER_TRANSFER;
                    break;
               }

               FileTransferHandshakeMessage  *pHandShakeMessage = new FileTransferHandshakeMessage(this->getFileServiceVersion(), pPushFileMessage->getSourceFileName(), 
                                                                      pPushFileMessage->getDestinationFileName(), pPushFileMessage->getSourceLocationId(), 
                                                                      nFileSize, pPushFileMessage->getFileTransferFlag());

               WaveNs::prismAssert (NULL != pHandShakeMessage, __FILE__, __LINE__);
               string sFTHandle = m_FTTable.GenerateFTHandle(pPushFileMessage->getSourceLocationId() , pPushFileMessage->getSourceFileName());
               pHandShakeMessage->setTransferHandle(sFTHandle);
               pPushFileMessage->setTransferHandle(sFTHandle);

               // trace (TRACE_LEVEL_INFO, "FileLocalObjectManager::TriggerAsyncFileTransferHandshake : Before creating the cluster context object. ");
               WaveSendToClusterContext     *pWaveSendToClusterContext   = new WaveSendToClusterContext (this, 
                                                                           reinterpret_cast<PrismAsynchronousCallback> (&FileLocalObjectManager::TriggerFileTransferHandshakeCallback),
                                                                           pPrismLinearSequencerContext);

               pWaveSendToClusterContext->setPPrismMessageForPhase1 (pHandShakeMessage);
               pWaveSendToClusterContext->setLocationsToSendToForPhase1(pPushFileMessage->getDestinationLocationIdListForNextMessage());
               sendToWaveCluster (pWaveSendToClusterContext);
               tracePrintf(TRACE_LEVEL_DEBUG, "FileLocalObjectManager::TriggerAsyncFileTransferHandshake : After sending to cluster status = [0x%x]", status);
 	          
          } while (0);
          
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               pPrismLinearSequencerContext->incrementNumberOfFailures ();
          }

        	--(*pPrismLinearSequencerContext);

   	      status = (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

          if (WAVE_MESSAGE_SUCCESS != status)
          {
              pPrismLinearSequencerContext->executeNextStep (status);
          }
          else
          {
              // status is WAVE_MESSAGE_SUCCESS means -
              //  - send to wave cluster is done, and callback is expected.
              // So, executeNextStep will be done in the callback.
          }

          return (status);
    }
 

     void     FileLocalObjectManager::TriggerFileTransferHandshakeCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
     {
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

          PrismLinearSequencerContext *pPrismLinearSequencerContext  = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
          prismAssert (pPrismLinearSequencerContext, __FILE__, __LINE__);

          FilePushFileMessage *pPushFileMessage = reinterpret_cast<FilePushFileMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
          prismAssert (NULL != pPushFileMessage, __FILE__, __LINE__);

          //--(*pPrismLinearSequencerContext);

          vector<LocationId>  vecLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Total LocationId that responded is [%d]. ", __FUNCTION__ , __LINE__ , vecLocations.size());

          string sFTHandle    = pPushFileMessage->getTransferHandle();
          string sSrcFilename = pPushFileMessage->getSourceFileName();

          ResourceId status = WAVE_MESSAGE_SUCCESS;

          // before actual processing of handshake callback.
          m_FTTable.DumpInfo();

          for (vector<LocationId>::iterator itr = vecLocations.begin(); itr != vecLocations.end(); itr++)
          {
              ResourceId sendStatus = pWaveSendToClusterContext->getSendStatusForPhase1 (*itr);

              if (WAVE_MESSAGE_SUCCESS != sendStatus)
              {
                  tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d]: Handle [%s], Handshake response from [%d] for Filename [%s], sendStatus [0x%x]", __FILE__, __LINE__ , sFTHandle.c_str(), *itr, sSrcFilename.c_str(), sendStatus);
                  // Remove this node from the list to which subsequent messages are sent (Fragment send)
                  pPushFileMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              // Check the forward send status, if success the proceed further, else remove this location from the list for next phase.
              FrameworkStatus frameworkStatus  = pWaveSendToClusterContext->getFrameworkStatusForPhase1 (*itr);

              if (FRAMEWORK_SUCCESS != frameworkStatus)
              {
                  tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d]: Handle [%s], Handshake response from [%d] for Filename [%s], FrameworkStatus [0x%x]", __FILE__, __LINE__ , sFTHandle.c_str(), *itr, sSrcFilename.c_str(), frameworkStatus);
                  // Remove this node from the list to which subsequent messages are sent (Fragment send)
                  pPushFileMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              ResourceId completionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(*itr);

              if (WAVE_MESSAGE_SUCCESS != completionStatus)
              {
                  tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d]: Handle [%s], Handshake response from [%d] for Filename [%s], completionStatus [0x%x]", __FILE__, __LINE__ , sFTHandle.c_str(), *itr, sSrcFilename.c_str(), completionStatus);
                  // Add this node to the failed list,
                  pPushFileMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              FileTransferHandshakeMessage *pMessage  = dynamic_cast<FileTransferHandshakeMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1 (*itr) );
              prismAssert (pMessage, __FILE__, __LINE__);

              tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d]: Handle [%s], Handshake response from [%d] for Filename [%s], CompletionCode [0x%x], FrameworkStatus [0x%x]\n", __FILE__, __LINE__ , sFTHandle.c_str(), *itr, sSrcFilename.c_str(), completionStatus, frameworkStatus);

              // Do the actual process of handshake response here.
              tracePrintf(TRACE_LEVEL_DEBUG, "FileLocalObjectManager::TriggerFileTransferHandshakeCallback: Add Transaction to  FTTable , FTHandle = [%s]", sFTHandle.c_str());
              status = m_FTTable.AddToOutgoingQueueInFTTable(pMessage, *itr);
              m_FTTable.setFrameworkStatus(sFTHandle, frameworkStatus, (*itr));
              m_FTTable.setResponseStatus(sFTHandle, completionStatus, (*itr));
          }

          delete pWaveSendToClusterContext->getPPrismMessageForPhase1 ();  // Delete the handshake message we created.
          delete pWaveSendToClusterContext;

          // Need to call next step if there is atleast one node to which the fragments can be sent to. So this check needs to be updated for the same.
          status = ((pPushFileMessage->getLocationCountToReceiveNextMessage() > 0) ? WAVE_MESSAGE_SUCCESS : WAVE_MESSAGE_ERROR);

          //
          tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d]: After processing handshake callback", __FILE__, __LINE__);
          m_FTTable.DumpInfo();

          if (WAVE_MESSAGE_SUCCESS != status)  
          {
              // Populate the Response code in the PushFileMessage.
              vector<UI32> vecDestList = pPushFileMessage->getDestinationLocationIdList();
              vector<ResourceId> vecResponse;
              for (vector<UI32>::iterator itr_node = vecDestList.begin(); itr_node != vecDestList.end(); itr_node++)
              {
                  vecResponse.push_back(WAVE_MESSAGE_ERROR);
              }
              tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d]: Total Response Vector Length for fthandle [%s] and vecResponse.size() [%d] \n", __FILE__, __LINE__, sFTHandle.c_str(), vecResponse.size());
              pPushFileMessage->setResponseCodeList(vecResponse);
          }

          pPrismLinearSequencerContext->executeNextStep (status);
          return ; 
     }
          
     ResourceId   FileLocalObjectManager::StartAsyncFileTransfer(PrismLinearSequencerContext *pPrismLinearSequencerContext)
     {
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

          FilePushFileMessage *pPushFileMessage = reinterpret_cast<FilePushFileMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
          WaveNs::prismAssert (NULL != pPushFileMessage, __FILE__, __LINE__);          
          
          //1. Call a function to create a fragment list of the source file.
          CTransferContext *pContextObj = m_FTTable.GetContextObject(pPushFileMessage->getTransferHandle());
          WaveNs::prismAssert (NULL != pContextObj , __FILE__, __LINE__);          

          // file object is created during Handshake callback.
          CFile *pFileObj = pContextObj->getFileObject();
          WaveNs::prismAssert (NULL != pFileObj , __FILE__, __LINE__);          

          string     sSrcFilename   = pPushFileMessage->getSourceFileName();          
          ResourceId status         = pFileObj->Init(); 

          if (WAVE_MESSAGE_SUCCESS == status)
          {
               pFileObj->FragmentFile();

               unsigned int nFragmentCount = pFileObj->GetFragmentCount();
               tracePrintf(TRACE_LEVEL_DEBUG, "[%s] Total Fragments [%d], for source file [%s] being sent out. ", __FUNCTION__ , nFragmentCount, sSrcFilename.c_str());
               FilePushFileFragmentMessage  *pMessage = NULL;

          	++(*pPrismLinearSequencerContext);
               
               for (unsigned int idx = 0; idx < nFragmentCount ; idx++)
               {
                    // Create a message and send each fragment to the other node.
            	    tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Fragment Index [%d], for source file [%s] being sent out. ", __FUNCTION__ , __LINE__ , idx, sSrcFilename.c_str());

                    do {

                         unsigned char *pBuf = NULL;
                         UI32 fragmentsize = 0;  // populated by GetFragment() call.

                         status = pFileObj->GetFragment(idx, pBuf, fragmentsize);
                         
                         if (WAVE_MESSAGE_SUCCESS != status)
                         {
                              // Need Assert ?
                              tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Failed to retrieve File fragment for index [%d], for destination file [%s] returning .... ", __FUNCTION__ , __LINE__ , idx, sSrcFilename.c_str());
                              break;
                         }

                         tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  GetFragment() returned pBuf=[0x%x], buffersize = [%d] for index [%d], destination file [%s] created .... ", __FUNCTION__ , __LINE__ , pBuf, fragmentsize, idx, sSrcFilename.c_str());
                                                  
                         bool bIsLastFragment = false;
                         status = pFileObj->IsLastFileFragment(idx, bIsLastFragment);

                         if (WAVE_MESSAGE_SUCCESS != status)
                         {
                              // Need Assert ?
                              tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Failed to retrieve Last fragment flag for index [%d], for destination file [%s] returning .... ", __FUNCTION__ , __LINE__ , idx, sSrcFilename.c_str());
                              break;
                         }

                         pMessage = new FilePushFileFragmentMessage(pPushFileMessage->getTransferHandle(), pPushFileMessage->getSourceLocationId(), idx, fragmentsize, bIsLastFragment);
                         WaveNs::prismAssert (NULL != pMessage, __FILE__, __LINE__);

                         tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Fragment send message create for index [%d], destination file [%s] created .... ", __FUNCTION__ , __LINE__ , idx, sSrcFilename.c_str());

                         status = pMessage->addBuffer(FILESVC_FILE_FRAGMENT_BUFFER_TAG, fragmentsize, pBuf);

                         if (WAVE_MESSAGE_SUCCESS != status)
                         {
                              // TODO: Look at the status for error and abort this transfer.
                              // Need Assert ?
                              delete pMessage;
                              break;
                         }
                         
                         WaveSendToClusterContext  *pWaveSendToClusterContext = new WaveSendToClusterContext (this, 
                                                                                                              reinterpret_cast<PrismAsynchronousCallback> (&FileLocalObjectManager::StartFileTransferCallback),
                                                                                                              pPrismLinearSequencerContext);
                         WaveNs::prismAssert (NULL != pWaveSendToClusterContext , __FILE__, __LINE__);          

                         pWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage);
                         pWaveSendToClusterContext->setLocationsToSendToForPhase1(pPushFileMessage->getDestinationLocationIdListForNextMessage());

                         trace (TRACE_LEVEL_DEBUG, "FileLocalObjectManager::StartAsyncFileTransfer : sending to cluster");
                         sendToWaveCluster (pWaveSendToClusterContext);

                         ++(*pPrismLinearSequencerContext);

                    } while(0);

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                         // TODO: Abort this transfer if we have encountered failure at any point of time and got here.
                         // How to cleanup the remote node, if it hasn't gone down, but this failure is due to a network issue. ??
                         break;// Get out of the for-loop.
                    }

                    // for failover testing.
                    // WaveNs::prismSleep (1);

               }  // for - loop.
               
        	--(*pPrismLinearSequencerContext);
                              
          } else {
               tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d] Unable to retrieve file Object for file [%s] , error [%d]\n", __FILE__, __LINE__, sSrcFilename.c_str(), status);
          }
          
          if (WAVE_MESSAGE_SUCCESS != status)
          {
              // TODO: Do the cleanup  here.
              // 1. Outgoing list.
              // 2. Context Object.
              // 3. Active Receipient List.

              // Need Assert instead ?
          }

          pPrismLinearSequencerContext->executeNextStep (status);
          return (status);
     }

void   FileLocalObjectManager::StartFileTransferCallback( WaveSendToClusterContext *pWaveSendToClusterContext)
     {
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );

          PrismLinearSequencerContext  *pPrismLinearSequencerContext  = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());
          prismAssert (NULL != pPrismLinearSequencerContext , __FILE__, __LINE__);

          FilePushFileMessage *pPushFileMessage = reinterpret_cast<FilePushFileMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
          prismAssert (NULL != pPushFileMessage, __FILE__, __LINE__);

          --(*pPrismLinearSequencerContext);

          vector<LocationId>  vecLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Total LocationId that responded is [%d]. ", __FUNCTION__ , __LINE__ , vecLocations.size());

          string sSrcFilename = pPushFileMessage->getSourceFileName();
          string sFTHandle    = pPushFileMessage->getTransferHandle();


          for (vector<LocationId>::iterator itr = vecLocations.begin(); itr != vecLocations.end(); itr++)
          {
              // Check the forward send status, if success the proceed further, else remove this location from the list for next phase.
              ResourceId sendStatus = pWaveSendToClusterContext->getSendStatusForPhase1 (*itr);
              
              if (WAVE_MESSAGE_SUCCESS != sendStatus)
              {                     
                  tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d]: Response from [%d] for Filename [%s], sendStatus [0x%x]", __FILE__, __LINE__, *itr, sSrcFilename.c_str (), sendStatus);
                  
                  // Keep updating status.
                  m_FTTable.setFrameworkStatus(sFTHandle, FRAMEWORK_ERROR, (*itr));
                  m_FTTable.setResponseStatus(sFTHandle, WAVE_MESSAGE_ERROR, (*itr));
                  
                  // Add this node to the failed list,
                  //   - update the response code for this node on the PushFile message,
                  pPushFileMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }
              
              // Check the forward call status first and if successful, check the framework status in the callback sequence.
              FrameworkStatus frameworkStatus = pWaveSendToClusterContext->getFrameworkStatusForPhase1 (*itr);
              
              if (FRAMEWORK_SUCCESS != frameworkStatus)
              {   
                  tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d]: Response from [%d] for Filename [%s], frameworkStatus [0x%x]", __FILE__, __LINE__, *itr, sSrcFilename.c_str (), frameworkStatus);
                  
                  // Keep updating status.
                  m_FTTable.setFrameworkStatus(sFTHandle, frameworkStatus, (*itr));
                  m_FTTable.setResponseStatus(sFTHandle, WAVE_MESSAGE_ERROR, (*itr));
                  
                  // Remove this node from the list to which subsequent messages are sent (Fragment send)
                  pPushFileMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }
              
              // Check the completion status, if success the proceed further, else remove this location from the list for next phase.
              ResourceId completionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(*itr);
              
              if (WAVE_MESSAGE_SUCCESS != completionStatus)
              {
                  tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d]: Response from [%d] for Filename [%s], completionStatus [0x%x]", __FILE__, __LINE__, *itr, sSrcFilename.c_str (), completionStatus);

                  // Keep updating status.
                  m_FTTable.setFrameworkStatus(sFTHandle, frameworkStatus, (*itr));
                  m_FTTable.setResponseStatus(sFTHandle, completionStatus, (*itr));

                  // Add this node to the failed list,
                  //   - update the response code for this node on the PushFile message,
                  pPushFileMessage->removeLocationForSubsequentSends(*itr);
                  continue;
              }

              FilePushFileFragmentMessage *pMessage  = dynamic_cast<FilePushFileFragmentMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1 (*itr) );
              prismAssert (pMessage, __FILE__, __LINE__);
              UI32 fragmentIndex = pMessage->getFileFragmentIndex ();

              tracePrintf(TRACE_LEVEL_DEBUG, "FileLocalObjectManager::StartFileTransferCallback: Process FragmentSend Callback for FTHandle = [%s], Response from [%d], for Filename [%s], for FragmentIndex [%d]", sFTHandle.c_str(), *itr, sSrcFilename.c_str(), fragmentIndex);

              // Keep updating status.
              m_FTTable.setFrameworkStatus(sFTHandle, frameworkStatus, (*itr));
              m_FTTable.setResponseStatus(sFTHandle, completionStatus, (*itr));
          }  // End of for loop.

          // Now cleanup if it is Last fragment. 

          FilePushFileFragmentMessage *pPrismMessageForPhase1 = dynamic_cast<FilePushFileFragmentMessage *> (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());

          bool isLastFragment = pPrismMessageForPhase1->IsLastFragment ();

          if (true == isLastFragment)
          {
              // Populate the Response code in the PushFileMessage.
              // This list should contain all the receiver locations.
              vector<UI32>       vecDestList = pPushFileMessage->getDestinationLocationIdList();
              vector<ResourceId> vecResponse;              
              trace (TRACE_LEVEL_DEBUG, "FileLocalObjectManager::StartFileTransferCallback: Last Fragment : starting cleanup");
              m_FTTable.DumpInfo();

              for (vector<UI32>::iterator itr_node = vecDestList.begin(); itr_node != vecDestList.end(); itr_node++)
              {
                  ResourceId statusForAReceiverLocation = m_FTTable.getResponseStatus(sFTHandle , *itr_node);

                  trace (TRACE_LEVEL_DEBUG, string("FileLocalObjectManager::StartFileTransferCallback: Last Fragment : Response Code from Node [") + *itr_node + "] is [" + statusForAReceiverLocation + "]\n");
                  vecResponse.push_back(statusForAReceiverLocation);
              }

              trace (TRACE_LEVEL_DEBUG, string("FileLocalObjectManager::StartFileTransferCallback: Last Fragment : Total Response Vector Length for fthandle [") + sFTHandle + "] is [" + vecResponse.size() + "]\n");

              pPushFileMessage->setResponseCodeList(vecResponse);

              // Currently, all fragements are 'sendToWaveCluster'ed to same set of locations (as decided after handshake.)
              // Only those set of locations are present in fttable. i.e. It can be smaller than 'set of user specified location ids'.
              // So, remove only the 'sendToWaveCluster'ed locations from fttable.

              for (vector<LocationId>::iterator vecLocationsItr = vecLocations.begin (); vecLocations.end () != vecLocationsItr; vecLocationsItr++)
              {
                  // Entry from active recipient list (handle<=>vector<locations> map) will get removed when RHS vector becomes empty.
                  // At the same time, entry from context list (handle<=>context*), and from outgoing queue (handle<=>filename) will be removed.
                  // (i.e. during last iteration of this for loop.)

                  m_FTTable.RemoveFromFTTable(sFTHandle, *vecLocationsItr);
              }

              // table should not have entry for current file handle.
              m_FTTable.DumpInfo();
          }

          // Cleanup.
          delete pPrismMessageForPhase1;  // Delete the handshake message we created.
          delete pWaveSendToClusterContext;

          // Need to call next step if there is atleast one node to which the fragments can be sent to. So this check needs to be updated for the same.
          // Currently, All fragments are sent in a for loop without waiting for any response.
          // So, actual success/failed step will get executed after receipt of callback for last fragment.
          pPrismLinearSequencerContext->executeNextStep ((pPushFileMessage->getLocationCountToReceiveNextMessage() > 0) ? WAVE_MESSAGE_SUCCESS : WAVE_MESSAGE_ERROR);
          return ;
     }                                                                                                                                                         

     CFileFragment::CFileFragment(UI32 FragmentSize, unsigned char* fragment_block, UI32 FragmentIndex, bool bLastFragment)
     {
          m_nFragmentSize = FragmentSize;
          m_nFragmentIndex = FragmentIndex;
          m_pFragmentBlock = fragment_block;
          m_bLastFragment = bLastFragment;
     }

     CFileFragment::~CFileFragment()
     {
//       trace (TRACE_LEVEL_INFO, string ("CFileFragment::~CFileFragment: called for index [") + m_nFragmentIndex + "]\n");
     }

     UI32 CFileFragment::GetFragmentSize()
     {
          return m_nFragmentSize;
     }
     
     UI32 CFileFragment::GetFragmentIndex() 
     {
          return m_nFragmentIndex;
     }
     
     unsigned char *CFileFragment::GetFragmentBlock()
     {
          return m_pFragmentBlock;
     }
     

     bool  CFileFragment::IsLastFileFragment()
     {
         return m_bLastFragment;
     }


#if 0
        
     ResourceId   FileLocalObjectManager::StartFileTransfer(PrismLinearSequencerContext *pPrismLinearSequencerContext)
     {

          ResourceId status = WAVE_MESSAGE_SUCCESS;
          tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
          FilePushFileMessage *pPushFileMessage = reinterpret_cast<FilePushFileMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

          WaveNs::prismAssert (NULL != pPushFileMessage, __FILE__, __LINE__);          

          string sSrcFilename = pPushFileMessage->getSourceFileName();          
          CFile  fileObj(sSrcFilename, false); // Create a file object for reading only.
          status = fileObj.Init();
          if (WAVE_MESSAGE_SUCCESS == status)
          {
               fileObj.FragmentFile();
               unsigned int nFragmentCount = fileObj.GetFragmentCount();
               tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Total Fragments [%d], for source file [%s] being sent out. ", __FUNCTION__ , __LINE__ , nFragmentCount, sSrcFilename.c_str());               
               FilePushFileFragmentMessage  *pMessage = NULL;
               for (unsigned int idx = 0; idx < nFragmentCount ; idx++) // TODO: This needs to be optimized for asynchronous sends.
               {
                    // Create a message and send each fragment to the other node.
                    
                    // tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Fragment Index [%d], for source file [%s] being sent out. ", __FUNCTION__ , __LINE__ , idx, sSrcFilename.c_str());

                    do {

                         unsigned char *pBuf = NULL;
                         UI32 fragmentsize = 0;  // populated by GetFragment() call.
                         status = fileObj.GetFragment(idx, pBuf, fragmentsize);
                         
                         if (WAVE_MESSAGE_SUCCESS != status)
                         {
                              tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Failed to retrieve File fragment for index [%d], for destination file [%s] returning .... ", __FUNCTION__ , __LINE__ , idx, sSrcFilename.c_str());
                              break;
                         }

                         // tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  GetFragment() returned pBuf=[0x%x], buffersize = [%d] for index [%d], destination file [%s] created .... ", __FUNCTION__ , __LINE__ , pBuf, fragmentsize, idx, sSrcFilename.c_str());
                                                  
                         bool bIsLastFragment = false;
                         status = fileObj.IsLastFileFragment(idx, bIsLastFragment);
                         if (WAVE_MESSAGE_SUCCESS != status)
                         {
                              tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Failed to retrieve Last fragment flag for index [%d], for destination file [%s] returning .... ", __FUNCTION__ , __LINE__ , idx, sSrcFilename.c_str());
                              break;
                         }

                         // tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Last fragment flag is not set for index [%d], for destination file [%s] proceeding .... ", __FUNCTION__ , __LINE__ , idx, sSrcFilename.c_str());

                         pMessage = new FilePushFileFragmentMessage(pPushFileMessage->getTransferHandle(), idx, fragmentsize, bIsLastFragment);

                         WaveNs::prismAssert (NULL != pMessage, __FILE__, __LINE__);

                         // tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Fragment send message create for index [%d], destination file [%s] created .... ", __FUNCTION__ , __LINE__ , idx, sSrcFilename.c_str());

                         status = pMessage->addBuffer(FILESVC_FILE_FRAGMENT_BUFFER_TAG, fragmentsize, pBuf);
                         if (WAVE_MESSAGE_SUCCESS != status)
                         {
                              // TODO: Look at the status for error and abort this transfer.
                              break;
                         }

                         status = sendSynchronously(pMessage, pPushFileMessage->getDestinationLocationId());
                         if (WAVE_MESSAGE_SUCCESS != status)
                         {
                              trace (TRACE_LEVEL_INFO, string("FileLocalObjectManager::StartFileTransfer: Fragment send failed. Error code [") + FrameworkToolKit::localize(status) + "]\n");
                              // TODO: This should trigger a local cleanup.  This needs to be handled as part of async send processing.
                              break;
                         }

                         status =  pMessage->getCompletionStatus();
                         if (WAVE_MESSAGE_SUCCESS == status)
                         {
                              // trace (TRACE_LEVEL_INFO, string("FileLocalObjectManager::StartFileTransfer: FileTransfer succeeded for index [") + idx + "]\n");
                         } else {
                              trace (TRACE_LEVEL_INFO, string("FileLocalObjectManager::StartFileTransfer: FileTransfer failed. Error code [") + FrameworkToolKit::localize(status)+ "]\n");
                              break;
                         }
                         
                         // If this was the last response, cleanup the local FTTable and move on.
                         if (true == pMessage->IsLastFragment())
                         {
                              m_FTTable.RemoveFromFTTable(pPushFileMessage->getTransferHandle());
                         }
                    
                    } while(0);

                    if (pMessage)
                         delete pMessage;
                    pMessage = NULL;

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                         // TODO: Abort this transfer if we have encountered failure at any point of time and got here.
                         // How to cleanup the remote node, if it hasn't gone down, but this failure is due to a network issue. ??
                         break;// Get out of the for-loop.
                    }

               }  // for -loop.
          } else {
               tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d] Unable to create file Object for file [%s] , error [%d]\n", sSrcFilename.c_str(), status);
          }
          
          // cleanup from the FTTable now.
          m_FTTable.RemoveFromFTTable(pPushFileMessage->getTransferHandle());          

          pPrismLinearSequencerContext->executeNextStep (status);
          return (status);
     }

#endif

     
#if 0
     
     ResourceId   FileLocalObjectManager::TriggerFileTransferHandshake(PrismLinearSequencerContext *pPrismLinearSequencerContext)
     {

          ResourceId status = WAVE_MESSAGE_SUCCESS;
          ResourceId HandShakeStatus = WAVE_MESSAGE_SUCCESS;

          tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
          FilePushFileMessage *pPushFileMessage = reinterpret_cast<FilePushFileMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
          
         WaveNs::prismAssert (NULL != pPushFileMessage, __FILE__, __LINE__);          

          FileTransferHandshakeMessage   *pHandShakeMessage = NULL;          
          do {
          
               string sSrcFilename = pPushFileMessage->getSourceFileName();
               // Get the Source file size first.
               struct stat file_statbuf = {0};
               int rc = stat(sSrcFilename.c_str() , &file_statbuf);
               if (rc)
               {
                    // Stat call failed, return error here.
                    trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::TriggerFileTransferHandshake - stat() failed with rc = [") + rc + "]  for filename " + sSrcFilename);
                    status = WAVE_FILE_INVALID_FILENAME;
                    break;
               }          
               
               // Need to check if this file to be sent out is already being received by this node from somewhere else,
               // in which case it should be in the Incoming FTList.
               if (true == m_FTTable.IsInComingTransferList(sSrcFilename))
               {
                    trace (TRACE_LEVEL_INFO, string ("CFileTransferTable::TriggerFileTransferHandshake: Source file [") + sSrcFilename + "] involved in another incoming transfer. returning ... ");
                    status = WAVE_FILE_DESTINATION_FILE_BUSY_IN_ANOTHER_TRANSFER;
                    break;
               }

              UI32 nFileSize = file_statbuf.st_size;
              // trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::TriggerFileTransferHandshake - stat() succeeded. Filesize = [") + nFileSize + "]  for filename " + sSrcFilename);

              pHandShakeMessage = new FileTransferHandshakeMessage(this->getFileServiceVersion(), pPushFileMessage->getSourceFileName(), 
                                                                          pPushFileMessage->getDestinationFileName(), pPushFileMessage->getSourceLocationId(), 
                                                                          nFileSize, pPushFileMessage->getFileTransferFlag());
                                                                          
              WaveNs::prismAssert (NULL != pHandShakeMessage, __FILE__, __LINE__);

              // Send this message to a particular location as in DestinationLocationId.
              // trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::TriggerFileTransferHandshake - Sending handshake message to [") + pPushFileMessage->getDestinationLocationId() + "]  for filename " + sSrcFilename);

              status = sendSynchronously (pHandShakeMessage, pPushFileMessage->getDestinationLocationId());
              if (WAVE_MESSAGE_SUCCESS != status)
              {
                    trace (TRACE_LEVEL_INFO, "FileLocalObjectManager::TriggerFileTransferHandshake: Handshake message failed. Status: " + FrameworkToolKit::localize(status));
                    break;
              }                    

              trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::TriggerFileTransferHandshake - Send handshake message to [") + pPushFileMessage->getDestinationLocationId() + "]  for filename " + sSrcFilename + " is successful.\n");

               HandShakeStatus =  pHandShakeMessage->getCompletionStatus(); 
               if (WAVE_MESSAGE_SUCCESS != HandShakeStatus)               
               {
                    trace (TRACE_LEVEL_INFO, string("FileLocalObjectManager::TriggerFileTransferHandshake: FileTransfer handshake failed. Error code [") + FrameworkToolKit::localize(HandShakeStatus) + "]\n");
                    status = HandShakeStatus;
                    break;
               }               

              trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::TriggerFileTransferHandshake - Handshake response from [") + pPushFileMessage->getDestinationLocationId() + "]  for filename " + sSrcFilename + " is = [" + HandShakeStatus + "]\n.");

               // 1. Retrieve the TransferHandle from the message.
               string sFileTransferHandle = pHandShakeMessage->getTransferHandle();
               pPushFileMessage->setTransferHandle(sFileTransferHandle); 

               // 2. Add handle to the local FileTransferTable.
               m_FTTable.AddToOutgoingQueueInFTTable(sFileTransferHandle, pHandShakeMessage);
               m_FTTable.DumpInfo();
          } while (0);

          if (pHandShakeMessage)
          {
               delete pHandShakeMessage;
               pHandShakeMessage = NULL;
          }

          // 3. Populate Response code and return.
          pPrismLinearSequencerContext->executeNextStep (status);
          return (status);
     }

#endif     

void FileLocalObjectManager::PushFileToHaPeerMessageHandler (PushFileToHaPeerMessage *pPushFileToHaPeerMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileLocalObjectManager::pushFileToHaPeerValidateReceiptStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileLocalObjectManager::pushFileToHaPeerReceiveFileStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPushFileToHaPeerMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
}

ResourceId FileLocalObjectManager::pushFileToHaPeerValidateReceiptStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    PushFileToHaPeerMessage *pPushFileToHaPeerMessage = dynamic_cast<PushFileToHaPeerMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pPushFileToHaPeerMessage, __FILE__, __LINE__);

    UI32        fileSize            = pPushFileToHaPeerMessage->getFileSize ();
    string      destinationFileName = pPushFileToHaPeerMessage->getDestinationFileName ();

    ResourceId  returnStatus        = WAVE_MESSAGE_SUCCESS;

    do
    {
        // TODO : Validate that this wave server is running on standby machine/switch/module.
        //        Add similar (but opposite) check in all other handlers.
        //        i.e. Wave Server on standby (and only Wave Server on standby) should only process this message.
        //        (Need to call API provided by 'Wave HA infrastructure'.)

        if (0 == fileSize)
        {
            returnStatus = WAVE_FILE_INVALID_FILESIZE;
            break;
        }
        else if (true == destinationFileName.empty ())
        {
            returnStatus = WAVE_FILE_INVALID_FILENAME;
            break;
        }

    } while (0);

    return returnStatus;
}

ResourceId FileLocalObjectManager::pushFileToHaPeerReceiveFileStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    PushFileToHaPeerMessage *pPushFileToHaPeerMessage = dynamic_cast<PushFileToHaPeerMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pPushFileToHaPeerMessage, __FILE__, __LINE__);

    // 1. Get 'file buffer' from message.

    UI32    fileSize            = pPushFileToHaPeerMessage->getFileSize ();
    UI32    bufferSize          = 0;
    char   *pBuffer             = (char *) pPushFileToHaPeerMessage->findBuffer (FILESVC_COMPLETE_FILE_BUFFER_FOR_HA_PEER, bufferSize);
    prismAssert (fileSize == bufferSize, __FILE__, __LINE__);

    // 2. Write buffer to destination tmp file.
    //    Note : If "<destination file name>.tmp" exists, it will get overwritten, and finally deleted.
    //           This is same as existing 'Push File' handling.
    //           Maybe, 
    //              1. ".tmp" can be changed to something unique string like ".tmp_WaveFileServiceTempFile"    OR
    //              2. a file may be created in /tmp using tmpfile.

    string      destinationFileName = pPushFileToHaPeerMessage->getDestinationFileName ();

    ofstream    destinationTempFileStream;
    destinationTempFileStream.open  ((destinationFileName + ".tmp").c_str (), ios::binary);

    destinationTempFileStream.write (pBuffer, fileSize);
    destinationTempFileStream.close ();

    // 3. Move temporary file to destination file.

    string sCmd = string ("/bin/mv ") + (destinationFileName + ".tmp") + string (" ") + destinationFileName;
    system(sCmd.c_str()); // TODO: need to use popen.

    return WAVE_MESSAGE_SUCCESS;
}

void FileLocalObjectManager::FilePushFileToHaPeerMessageHandler (FilePushFileToHaPeerMessage *pFilePushFileToHaPeerMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileLocalObjectManager::pushFileToHaPeerValidateStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileLocalObjectManager::pushFileToHaPeerSendFileStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&FileLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFilePushFileToHaPeerMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
}

ResourceId FileLocalObjectManager::pushFileToHaPeerValidateStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    FilePushFileToHaPeerMessage *pFilePushFileToHaPeerMessage = dynamic_cast<FilePushFileToHaPeerMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pFilePushFileToHaPeerMessage, __FILE__, __LINE__);

    string      sourceFileName          = pFilePushFileToHaPeerMessage->getSourceFileName ();
    string      destinationFileName     = pFilePushFileToHaPeerMessage->getDestinationFileName ();

    ResourceId  returnStatus            = WAVE_MESSAGE_SUCCESS;

    do
    {
        // 1. check for non-empty source and destination filename

        if (true == sourceFileName.empty ())
        {
            trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::pushFileToHaPeerValidateStep : source filename is empty."));
            returnStatus = WAVE_FILE_INVALID_FILENAME;
            break;
        }

        if (true == destinationFileName.empty ())
        {
            trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::pushFileToHaPeerValidateStep : destination filename is empty."));
            returnStatus = WAVE_FILE_INVALID_FILENAME;
            break;
        }

        // 2. Check if file exists.

        struct stat fileStatBuf = {0};

        int statRet = stat (sourceFileName.c_str() , &fileStatBuf);

        if (0 != statRet)
        {
            // Input file is not good. return error here.
            trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::pushFileToHaPeerValidateStep - Bad input source filename, stat() returned:[") + statRet + "]  for filename " + sourceFileName);
            returnStatus = WAVE_FILE_INVALID_FILENAME;              
            break;
        }

        // 3. Check if file is not empty.

        if (0 == fileStatBuf.st_size) 
        {
            // If it's an empty file, we don't allow its transfer.
            trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::pushFileToHaPeerValidateStep - Filesize returned by stat() is ZERO for filename ") + sourceFileName);
            returnStatus = WAVE_FILE_INVALID_FILESIZE;
            break;
        }

        pFilePushFileToHaPeerMessage->setFileSize (fileStatBuf.st_size);

        // 4. Verify that source file is currently not being received.

        if (true == m_FTTable.IsInComingTransferList (sourceFileName))
        {
             trace (TRACE_LEVEL_INFO, string ("FileLocalObjectManager::pushFileToHaPeerValidateStep: Source file [") + sourceFileName + "] involved in another incoming transfer. returning ... ");
             returnStatus = WAVE_FILE_DESTINATION_FILE_BUSY_IN_ANOTHER_TRANSFER;
             break;
        }

    } while (0);

    return returnStatus;
}

ResourceId FileLocalObjectManager::pushFileToHaPeerSendFileStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    FilePushFileToHaPeerMessage *pFilePushFileToHaPeerMessage = dynamic_cast<FilePushFileToHaPeerMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pFilePushFileToHaPeerMessage, __FILE__, __LINE__);

    string      fileTransferHandle      = "";
    string      sourceFileName          = pFilePushFileToHaPeerMessage->getSourceFileName ();
    string      destinationFileName     = pFilePushFileToHaPeerMessage->getDestinationFileName ();
    UI32        fileSize                = pFilePushFileToHaPeerMessage->getFileSize ();

    ResourceId  returnStatus            = sendFileToHaPeer (fileTransferHandle, sourceFileName, destinationFileName, fileSize);

    return returnStatus;
}

}

