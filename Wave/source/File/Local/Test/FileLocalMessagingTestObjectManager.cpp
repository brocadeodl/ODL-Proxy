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
 *   Author : Jiyer                                          *
 *************************************************************/

#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "File/Local/Test/FileLocalMessagingTestObjectManager.h"
#include "File/Local/Test/FileLocalMessagingTestObjectManagerTypes.h"
#include "File/Local/Test/FileTestMessages.h"
#include "File/Local/FileLocalTypes.h"

namespace WaveNs
{


FileLocalMessagingTestObjectManager::FileLocalMessagingTestObjectManager ()
    : PrismTestObjectManager (getServiceName ())
{
    addOperationMap (FILE_LOCAL_TEST_COPY_FILE_WAVE_CLIENT_MESSAGE,reinterpret_cast<PrismMessageHandler> (&FileLocalMessagingTestObjectManager::copyFileWaveClientMessageHandler));
    addOperationMap (FILE_LOCAL_TEST_COPY_FILE_TO_HA_PEER_WAVE_CLIENT_MESSAGE, reinterpret_cast<PrismMessageHandler> (&FileLocalMessagingTestObjectManager::copyFileToHaPeerWaveClientMessageHandler));
    // TraceLevel traceLevel = TRACE_LEVEL_DEVEL; setTraceLevel (traceLevel);
}

FileLocalMessagingTestObjectManager *FileLocalMessagingTestObjectManager::getInstance ()
{
    static FileLocalMessagingTestObjectManager *pFileLocalMessagingTestObjectManager = NULL;

    if (NULL == pFileLocalMessagingTestObjectManager)
    {
        pFileLocalMessagingTestObjectManager = new FileLocalMessagingTestObjectManager ();
        WaveNs::prismAssert (NULL != pFileLocalMessagingTestObjectManager, __FILE__, __LINE__);
    }

    return (pFileLocalMessagingTestObjectManager);
}

FileLocalMessagingTestObjectManager::~FileLocalMessagingTestObjectManager ()
{
}

PrismServiceId FileLocalMessagingTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string FileLocalMessagingTestObjectManager::getServiceName ()
{
    return ("File Service Local Messaging Test");
}

PrismMessage *FileLocalMessagingTestObjectManager::createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {

        case FILE_LOCAL_TEST_COPY_FILE_WAVE_CLIENT_MESSAGE :
            pPrismMessage = new CopyFileWaveClientMessage ();
            break;
        case FILE_LOCAL_TEST_COPY_FILE_TO_HA_PEER_WAVE_CLIENT_MESSAGE :
            pPrismMessage = new CopyFileToHaPeerWaveClientMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}
void FileLocalMessagingTestObjectManager::testRequestHandler (RegressionTestMessage *pMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        // Tests expected to fail.
        // If these tests return correct error code, they are considered as passed test cases.
        reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::simplePushFileWithInvalidFilenameTest),
        reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::simplePushFileWithInvalidFilesizeTest),
        reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::simplePushFileWithInvalidLocationIdTest),
        reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::simpleSyncPushFileWith1KDatatoGoodLocationIdTest),
        reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::simpleASyncPushFileWith1KDatatoGoodLocationIdTest),
        //Test ASyncAbort
        //Test SyncAbort        
        reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::SynchronousDistributeFileWith1MBDatatoAllGoodLocationIdTest),
        //reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::ASynchronousDistributeFileWith1MBDatatoAllGoodLocationIdTest),

        // TODO:Add more tests here for other messages, and messages expected to succeed for correct inputs.
        reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::CleanupTempTestFiles),
        reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&FileLocalMessagingTestObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void FileLocalMessagingTestObjectManager::simplePushFileWithInvalidFilenameTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
          
          trace (TRACE_LEVEL_INFO, "Starting Synchronous Messaging for Push File with Invalid Filename.");

          WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
          vector<LocationId>  vecDestinationId;
          vecDestinationId.push_back(FrameworkToolKit::getThisLocationId());

          string sSourceFileName = "/tmp/123456789987654321" ;  // A random filename.
          FilePushFileMessage *pMessage = new FilePushFileMessage(sSourceFileName, sSourceFileName,  FrameworkToolKit::getThisLocationId(), vecDestinationId);
          pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
          status = sendSynchronously (pMessage);
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               ResourceId completionStatus = pMessage->getCompletionStatus();

               trace(TRACE_LEVEL_INFO, string (" Expected Result = [WAVE_FILE_INVALID_FILENAME], Actual Result = [") + FrameworkToolKit::localize(completionStatus) + "]");               
               if (completionStatus == WAVE_FILE_INVALID_FILENAME)
               {
                    // We are getting the correct error code, so this negative test case is actually a pass.
                    status = WAVE_MESSAGE_SUCCESS;
               }
          }
          else
          {
               trace (TRACE_LEVEL_INFO, "FileLocalMessagingTestObjectManager::simplePushFileWithInvalidFilenameTest: Failure to process PushFile message. Status: " + FrameworkToolKit::localize(status));
          }

          delete pMessage;
          pPrismLinearSequencerContext->executeNextStep (status);
}

void FileLocalMessagingTestObjectManager::simplePushFileWithInvalidFilesizeTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

          trace (TRACE_LEVEL_INFO, "Starting PushFile with a Invalid File size test...");

          WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
          vector<LocationId>  vecDestinationId;
          vecDestinationId.push_back(FrameworkToolKit::getThisLocationId());

          string sSourceFileName = "/tmp/testfile.txt" ;  // A non-existent file,
          system("touch /tmp/testfile.txt"); // This will create a tempfile with size zero in the /tmp folder. Assuming there is no 
          FilePushFileMessage *pMessage = new FilePushFileMessage(sSourceFileName, sSourceFileName,  FrameworkToolKit::getThisLocationId(), vecDestinationId);
          pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
          status = sendSynchronously (pMessage);
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               ResourceId completionStatus = pMessage->getCompletionStatus();

               trace(TRACE_LEVEL_INFO, string (" Expected Result = [WAVE_FILE_INVALID_FILESIZE], Actual Result = [") + FrameworkToolKit::localize(completionStatus) + "]");
               if (completionStatus == WAVE_FILE_INVALID_FILESIZE)
               {
                    // We are getting the correct error code, so this negative test case is actually a pass.
                    status = WAVE_MESSAGE_SUCCESS;
               }
          }
          else
          {
               trace (TRACE_LEVEL_INFO, "FileLocalMessagingTestObjectManager::simplePushFileWithInvalidFilesizeTest: Failure to process PushFile message. Status: " + FrameworkToolKit::localize(status));
          }

          delete pMessage;
          pPrismLinearSequencerContext->executeNextStep (status);
}

void FileLocalMessagingTestObjectManager::simplePushFileWithInvalidLocationIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

          trace (TRACE_LEVEL_INFO, "Starting PushFile with a Invalid LocationId test...");
          vector<LocationId>  vecDestinationId;
          UI32 nDestLocationID = 500;
          vecDestinationId.push_back(nDestLocationID);
          
          WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

          string sSourceFileName = "/tmp/testfile.txt" ;  // A non-existent file, 
          SetupTempFileForTransfer(2045, sSourceFileName);
          FilePushFileMessage *pMessage = new FilePushFileMessage(sSourceFileName, sSourceFileName,  FrameworkToolKit::getThisLocationId(), vecDestinationId);
          pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
          status = sendSynchronously (pMessage);
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               ResourceId completionStatus = pMessage->getCompletionStatus();

               trace(TRACE_LEVEL_INFO, string (" Expected Result = [WAVE_FILE_INVALID_DESTINATION_NODE], Actual Result = [") + FrameworkToolKit::localize(completionStatus) + "]");
               if (completionStatus == WAVE_FILE_INVALID_DESTINATION_NODE)
               {
                    // We are getting the correct error code, so this negative test case is actually a pass.
                    status = WAVE_MESSAGE_SUCCESS;
               }
          }
          else
          {
               trace (TRACE_LEVEL_INFO, "FileLocalMessagingTestObjectManager::simplePushFileWithInvalidLocationIdTest: Failure to process PushFile message. Status: " + FrameworkToolKit::localize(status));
          }
          delete pMessage;
          pPrismLinearSequencerContext->executeNextStep (status);
}

void FileLocalMessagingTestObjectManager::simpleASyncPushFileWith1KDatatoGoodLocationIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

     // Send a file which is smaller than a single fragment.
     trace (TRACE_LEVEL_INFO, "Starting Asynchronous pushFile of size ~2kb to a list of known good location...");
     char buff[64] = {0};
     string sSourceFileName = "/tmp/testfile.txt" ;  // A non-existent file, 
     WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
     vector<LocationId> connectedLocationsVector;
     SetupTempFileForTransfer(2045, sSourceFileName);
     
     FrameworkToolKit::getConnectedLocations(connectedLocationsVector);

     ++(*pPrismLinearSequencerContext);
     int i = 0;
     for(i=0; i< 10; i++) // Send 10 messages
     {     
          string sDestFilename = "/tmp/tempfile-2KB";
          snprintf(buff, 64, "-%d-%d.log", FrameworkToolKit::getThisLocationId(), i);
          sDestFilename += buff;     
          FilePushFileMessage *pMessage = new FilePushFileMessage(sSourceFileName, sDestFilename,  FrameworkToolKit::getThisLocationId(), connectedLocationsVector);
          pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
          status = send(pMessage, reinterpret_cast<PrismMessageResponseHandler> (&FileLocalMessagingTestObjectManager::ASyncPushFileWith1KDatatoGoodLocationIdTestCallback), pPrismLinearSequencerContext, 0, FrameworkToolKit::getThisLocationId());                    
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               pPrismLinearSequencerContext->incrementNumberOfFailures ();
               trace (TRACE_LEVEL_DEBUG, string ("FrameworkLocalMessagingTestObjectManager::simpleASyncPushFileWith1KDatatoGoodLocationIdTest : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
               delete pMessage;
          }
          else
          {
               ++(*pPrismLinearSequencerContext);
          }                    
          sleep(1);  // Sleep for 1 sec before next request.
     }
     --(*pPrismLinearSequencerContext);     
     pPrismLinearSequencerContext->executeNextStep (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);                       

}

void    FileLocalMessagingTestObjectManager::ASyncPushFileWith1KDatatoGoodLocationIdTestCallback(FrameworkStatus frameworkStatus, FilePushFileMessage *pMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

    --(*pPrismLinearSequencerContext);
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage, __FILE__, __LINE__);
        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
        }
    }
    else
    {
        pPrismLinearSequencerContext->incrementNumberOfFailures ();
    }
     if (NULL != pMessage)
     {
          delete pMessage;
     }    
    pPrismLinearSequencerContext->executeNextStep (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

void FileLocalMessagingTestObjectManager::simpleSyncPushFileWith1KDatatoGoodLocationIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
     // Send a file which is smaller than a single fragment.
     trace (TRACE_LEVEL_INFO, "Starting PushFile of size 2k to a good known location...");
     WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
     char buff[64] = {0};
     vector<LocationId> connectedLocationsVector;
     string sSourceFileName = "/tmp/testfile-2K.txt" ;  // A non-existent file, 
     SetupTempFileForTransfer(2045, sSourceFileName);
          
     FrameworkToolKit::getConnectedLocations(connectedLocationsVector);
     vector<LocationId>::iterator itr;      
     for (itr = connectedLocationsVector.begin(); itr != connectedLocationsVector.end(); itr++)
     {
          tracePrintf(TRACE_LEVEL_INFO, "FileLocalMessagingTestObjectManager::simpleSyncPushFileWith1KDatatoGoodLocationIdTest  Node [%d] in connected list.\n", *itr);
     }

     tracePrintf(TRACE_LEVEL_INFO, "FileLocalMessagingTestObjectManager::simpleSyncPushFileWith1KDatatoGoodLocationIdTest  size of connected list vector = [%d]\n", connectedLocationsVector.size());
       int i = 0;     
     for(i=0; i< 10; i++) // Send 10 messages
     {     
          string sDestFilename = "/tmp/tempfile-2KB";
          snprintf(buff, 64, "-%d-%d.log", FrameworkToolKit::getThisLocationId(), i);
          sDestFilename += buff;     
          FilePushFileMessage *pMessage = new FilePushFileMessage(sSourceFileName, sDestFilename,  FrameworkToolKit::getThisLocationId(), connectedLocationsVector);
          pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
          trace (TRACE_LEVEL_INFO, string("FileLocalMessagingTestObjectManager::simpleSyncPushFileWith1KDatatoGoodLocationIdTest: Before doing the sendSynchronous call....\n"));
          status = sendSynchronously (pMessage);
          trace (TRACE_LEVEL_INFO, string("FileLocalMessagingTestObjectManager::simpleSyncPushFileWith1KDatatoGoodLocationIdTest: After doing the sendSynchronous call....\n"));
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               trace (TRACE_LEVEL_DEBUG, string ("FSVCLocalMessagingTestOMgr::simpleSyncPushFileWith1KDatatoGoodLocationIdTest: Sending a message to [") +         
                                                                                PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed, Error [" + FrameworkToolKit::localize(status) + "]\n");
          }
          trace (TRACE_LEVEL_INFO, string("FileLocalMessagingTestObjectManager::simpleSyncPushFileWith1KDatatoGoodLocationIdTest: After doing the sendSynchronous call and before deleting the PushFileMessage object....\n"));
          delete pMessage;
          pMessage = NULL;
          trace (TRACE_LEVEL_INFO, string("FileLocalMessagingTestObjectManager::simpleSyncPushFileWith1KDatatoGoodLocationIdTest: After doing the sendSynchronous call and after deleting the PushFileMessage object....\n"));
          sleep(1);  // Sleep for 1 sec before next request.
     }
     
     pPrismLinearSequencerContext->executeNextStep (status);
}

void    FileLocalMessagingTestObjectManager::ASynchronousDistributeFileWith1MBDatatoAllGoodLocationIdTest      (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
     trace (TRACE_LEVEL_INFO, "Starting Asynchronous pushFile of size 1MB to a list of known good location...");
     char buff[64] = {0};
     string sSourceFileName = "/tmp/testfile.txt" ;  // A non-existent file, 
     WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
     vector<LocationId> connectedLocationsVector;
     SetupTempFileForTransfer(1200000, "/tmp/testfile-1MB.txt");
     
     FrameworkToolKit::getConnectedLocations(connectedLocationsVector);

     ++(*pPrismLinearSequencerContext);  
     int i = 0;          
     for(i=0; i< 10; i++) // Send 10 messages
     {     
          string sDestFilename = "/tmp/tempfile-1MB-async";
          snprintf(buff, 64, "-%d-%d.log", FrameworkToolKit::getThisLocationId(), i);
          sDestFilename += buff;     
          FilePushFileMessage *pMessage = new FilePushFileMessage(sSourceFileName, sDestFilename,  FrameworkToolKit::getThisLocationId(), connectedLocationsVector);
          pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
          status = send(pMessage, reinterpret_cast<PrismMessageResponseHandler> (&FileLocalMessagingTestObjectManager::ASynchronousDistributeFileWith1MBDatatoAllGoodLocationIdTestCallback), pPrismLinearSequencerContext, 0, FrameworkToolKit::getThisLocationId());                    
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               pPrismLinearSequencerContext->incrementNumberOfFailures ();
               trace (TRACE_LEVEL_DEBUG, string ("FrameworkLocalMessagingTestObjectManager::simpleAsynchronousMessageTestStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
               delete pMessage;
          }
          else
          {
               ++(*pPrismLinearSequencerContext);
          }
          sleep(1);  // Sleep for 1 sec before next request.
     }

     --(*pPrismLinearSequencerContext);     
       
     pPrismLinearSequencerContext->executeNextStep (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);                       

}

void    FileLocalMessagingTestObjectManager::ASynchronousDistributeFileWith1MBDatatoAllGoodLocationIdTestCallback(FrameworkStatus frameworkStatus, FilePushFileMessage *pMessage, PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

    --(*pPrismLinearSequencerContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pMessage, __FILE__, __LINE__);
        if (WAVE_MESSAGE_SUCCESS != (pMessage->getCompletionStatus ()))
        {
            pPrismLinearSequencerContext->incrementNumberOfFailures ();
        }
        delete pMessage;
    }
    else
    {
        pPrismLinearSequencerContext->incrementNumberOfFailures ();
        if (NULL != pMessage)
        {
            delete pMessage;
        }
    }

    pPrismLinearSequencerContext->executeNextStep (((pPrismLinearSequencerContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);

}

void FileLocalMessagingTestObjectManager::SynchronousDistributeFileWith1MBDatatoAllGoodLocationIdTest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

     trace (TRACE_LEVEL_INFO, "Starting PushFile of size 1MB to a list of known good location...");
     
     char buff[64] = {0};
     string sSourceFileName = "/tmp/testfile-1MB.txt" ;  // A non-existent file, 
     WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
     vector<LocationId> connectedLocationsVector;
     SetupTempFileForTransfer(1200000, sSourceFileName);
     
     FrameworkToolKit::getConnectedLocations(connectedLocationsVector);
     int i = 0;     
     for(i=0; i< 10; i++) // Send 10 messages
     {     
          string sDestFilename = "/tmp/tempfile-sync";
          snprintf(buff, 64, "-%d-%d.log", FrameworkToolKit::getThisLocationId(), i);
          sDestFilename += buff;     
          FilePushFileMessage *pMessage = new FilePushFileMessage(sSourceFileName, sDestFilename,  FrameworkToolKit::getThisLocationId(), connectedLocationsVector);
          pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
          status = sendSynchronously (pMessage);
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               trace (TRACE_LEVEL_DEBUG, string ("FSVCLocalMessagingTestOMgr::DistributeFileWith1MBDatatoAllGoodLocationIdTest : Sending a message to [") +         
                                                                                PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed, Error [" + FrameworkToolKit::localize(status) + "]\n");
          }
          delete pMessage;
          pMessage = NULL;
          sleep(1);  // Sleep for 1 sec before next request.
     }
    
     pPrismLinearSequencerContext->executeNextStep (status);
     
}


void FileLocalMessagingTestObjectManager::CleanupTempTestFiles (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
//   system("/bin/rm -rf /tmp/tempfile*");  // For now we know all the temp files are being created under /tmp folder with the file name starting with "tempfile*" signature.
     pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);     
}

void FileLocalMessagingTestObjectManager::SetupTempFileForTransfer(UI32 nFileSize, const string &sFileName)
{
     char buff[64] = {0};
     UI32 nBlockCount = 0;
     UI32 nBlockSize = 2048;
     if (nFileSize < nBlockSize)
     {   
        nBlockCount = 1 ;
     } else {
        nBlockCount = nFileSize/nBlockSize;
     }
     
     string sCmd = "/bin/dd if=/dev/urandom bs=";
     snprintf(buff, 64, "%d count=%d of=%s", nBlockSize, nBlockCount, sFileName.c_str());
     sCmd += buff;
     system(sCmd.c_str());
}

void FileLocalMessagingTestObjectManager::copyFileWaveClientMessageHandler (CopyFileWaveClientMessage *pCopyFileWaveClientMessage)
{
    // Note : All validations will be done by FileLocalObjectManager.

    // Get vector of location ids.
    
    vector<string>  vectorOfDestinationIpAddressesAndPorts  = pCopyFileWaveClientMessage->getVectorOfDestinationIpAddressesAndPorts ();

    vector<UI32> destinationLocationIdVector;

    for (UI32 i = 0; i < vectorOfDestinationIpAddressesAndPorts.size (); i++)
    {
        string          ipAddressPortString = vectorOfDestinationIpAddressesAndPorts[i];
        vector<string>  tokens;

        tokenize (ipAddressPortString, tokens, ':');

        string  ipAddress   = "";
        SI32    port        = 0;

        if (0 < tokens.size ())
        {
            ipAddress = tokens[0];
        }

        if (1 < tokens.size ())
        {
            port = atoi (tokens[1].c_str ());
        }

        LocationId locationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (ipAddress, port);

        destinationLocationIdVector.push_back (locationId);
    }

    string          sourceFileName                  = pCopyFileWaveClientMessage->getSourceFileName ();
    string          destinationFilename             = pCopyFileWaveClientMessage->getDestinationFileName ();
    UI32            copyFileFlag                    = pCopyFileWaveClientMessage->getCopyFileFlag ();
    UI32            sourceLocationId                = FrameworkToolKit::getThisLocationId ();

    FilePushFileMessage filePushFileMessage (sourceFileName, destinationFilename, sourceLocationId, destinationLocationIdVector, copyFileFlag);

    tracePrintf (TRACE_LEVEL_INFO, "FileLocalMessagingTestObjectManager::copyFileWaveClientMessageHandler : copyFileFlag = [%u]", copyFileFlag);

    ResourceId sendStatus = sendSynchronously (&filePushFileMessage);

    if (WAVE_MESSAGE_SUCCESS == sendStatus)
    {
        ResourceId completionStatus = filePushFileMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("FileLocalMessagingTestObjectManager::copyFileWaveClientMessageHandler : FilePushFileMessage processing failed. Status : ") + FrameworkToolKit::localize (completionStatus));
        }

        pCopyFileWaveClientMessage->setCompletionStatus (completionStatus);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("FileLocalMessagingTestObjectManager::copyFileWaveClientMessageHandler : Failure in sending FilePushFileMessage. Status : ") + FrameworkToolKit::localize (sendStatus));
        pCopyFileWaveClientMessage->setCompletionStatus (sendStatus);
    }

    reply (pCopyFileWaveClientMessage);
}

void FileLocalMessagingTestObjectManager::copyFileToHaPeerWaveClientMessageHandler (CopyFileToHaPeerWaveClientMessage *pCopyFileToHaPeerWaveClientMessage)
{
    // Note : All validations will be done by FileLocalObjectManager.

    string          sourceFileName                  = pCopyFileToHaPeerWaveClientMessage->getSourceFileName ();
    string          destinationFilename             = pCopyFileToHaPeerWaveClientMessage->getDestinationFileName ();

    FilePushFileToHaPeerMessage filePushFileToHaPeerMessage (sourceFileName, destinationFilename);

    ResourceId sendStatus = sendSynchronously (&filePushFileToHaPeerMessage);

    if (WAVE_MESSAGE_SUCCESS == sendStatus)
    {
        ResourceId completionStatus = filePushFileToHaPeerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("FileLocalMessagingTestObjectManager::copyFileToHaPeerWaveClientMessageHandler : FilePushFileToHaPeerMessage processing failed. Status : ") + FrameworkToolKit::localize (completionStatus));
        }

        pCopyFileToHaPeerWaveClientMessage->setCompletionStatus (completionStatus);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("FileLocalMessagingTestObjectManager::copyFileToHaPeerWaveClientMessageHandler : Failure in sending FilePushFileToHaPeerMessage. Status : ") + FrameworkToolKit::localize (sendStatus));
        pCopyFileToHaPeerWaveClientMessage->setCompletionStatus (sendStatus);
    }

    reply (pCopyFileToHaPeerWaveClientMessage);
}

}
