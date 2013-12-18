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

/******************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc. *
 *   All rights reserved.                                         *
 *   Author : jiyer                                               *
 ******************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "File/Local/FileLocalObjectManager.h"
#include "File/Local/FileTransferManagedObject.h"
#include "File/Local/FileLocalTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace WaveNs
{
     // Exactly one instance of this class is present, and the instance is member of FileLocalObjectManager.

     CFileTransferManagedObject::CFileTransferManagedObject()
     {
        m_nFTIndex = 0;
     }
     
     CFileTransferManagedObject::~CFileTransferManagedObject()
     {
          // TODO: Cleanup all the internal DS.
     }     

    // handle is a unique string (informative), but it is never parsed.
    string CFileTransferManagedObject::GenerateFTHandle(const LocationId srcLocationId, const string &srcFileName)
     {
          time_t     now = {0};
          char uuid_str[64] = {0};
          snprintf(uuid_str, 64, "%d", srcLocationId);
          string sFTHandle = "FTHandle#";
          sFTHandle += uuid_str;
          sFTHandle += "#";
          sFTHandle += srcFileName;
          sFTHandle += "#";
          now = time(NULL);   // TODO: Need to have a granular timestamp (upto millisecond level)
          snprintf(uuid_str, 64, "%ld", now);
          sFTHandle += string(uuid_str);
          sFTHandle += "#";
          snprintf(uuid_str, 64, "%d", m_nFTIndex);
          sFTHandle += string(uuid_str);
          sFTHandle += "#";
          m_nFTIndex++;  // Keeps track of the number of  transfers happening.
          return sFTHandle;
      }

     ResourceId   CFileTransferManagedObject::AddToIncomingList(const string &fthandle, const string  &sFilename)
     {
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::AddToIncomingList called for handle [") + fthandle + "] and filename [" + sFilename + "]\n");
          ResourceId status = WAVE_MESSAGE_SUCCESS;  
          std::map <string, string>::iterator itr_incominglist;
          itr_incominglist = m_IncomingActiveList.find(fthandle);
          if (itr_incominglist == m_IncomingActiveList.end())
          {
               m_IncomingActiveList[fthandle] = sFilename;
          } else {
               trace (TRACE_LEVEL_INFO, string("CFileTransferManagedObject::AddToIncomingList, Error, found entry for [") + fthandle + "] Destn Filename [" + sFilename + "]being written to from another location.");          
               status = WAVE_FILE_FILE_ALREADY_EXISTS_AT_DESTINATION;
          }          
          return (status);   
     }
     
     void  CFileTransferManagedObject::RemoveFromIncomingList(const string &fthandle)
     {
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::RemoveFromIncomingList called for FTHandle [") + fthandle + "]\n");
          std::map <string, string>::iterator itr_incominglist;
          itr_incominglist = m_IncomingActiveList.find(fthandle);
          if (itr_incominglist != m_IncomingActiveList.end())
          {
               m_IncomingActiveList.erase(itr_incominglist);
          }
     }
     
     ResourceId   CFileTransferManagedObject::AddToOutgoingList(const string &fthandle , const string &sFilename)
     {
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::AddToOutgoingList called for handle [") + fthandle + "] and filename [" + sFilename + "]\n");
          ResourceId status = WAVE_MESSAGE_SUCCESS;          
          std::map <string, string>::iterator itr_outgoinglist;                  
          itr_outgoinglist = m_OutgoingActiveList.find(fthandle);
          if (itr_outgoinglist == m_OutgoingActiveList.end())
          {
               m_OutgoingActiveList[fthandle] = sFilename;
          }        
          return (status);             
     }
     
     void   CFileTransferManagedObject::RemoveFromOutgoingList(const string &fthandle)
     {
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::RemoveFromOutgoingList called for FTHandle [") + fthandle + "]\n");
          std::map <string, string>::iterator itr_outgoinglist;
          itr_outgoinglist = m_OutgoingActiveList.find(fthandle);
          if (itr_outgoinglist != m_OutgoingActiveList.end())
          {
               m_OutgoingActiveList.erase(itr_outgoinglist);
          }
     }

     CTransferContext*  CFileTransferManagedObject::GetContextObject(const string &fthandle)
     {
          CTransferContext *pContextObject = NULL;
          std::map<string,  CTransferContext *>::iterator itr_contextlist = m_TransferContextList.find(fthandle);
          if (itr_contextlist != m_TransferContextList.end())
          {
               pContextObject = (*itr_contextlist).second;
          } 
          return (pContextObject);
     }
     
     ResourceId   CFileTransferManagedObject::AddToContextList(const string &fthandle, CTransferContext *pContextObj)
     {
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::AddToContextList called for handle [") + fthandle + "]\n");
          ResourceId status = WAVE_MESSAGE_SUCCESS;
          std::map<string,  CTransferContext *>::iterator itr_contextlist = m_TransferContextList.find(fthandle);
          if (itr_contextlist == m_TransferContextList.end())
          {
               m_TransferContextList[fthandle] = pContextObj; // Not found so add it.
          } 
          return (status);
     }
     
     
     void   CFileTransferManagedObject::RemoveFromContextList(const string &fthandle)
     {
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::RemoveFromContextList called for handle [") + fthandle + "]\n");
          std::map<string,  CTransferContext *>::iterator itr_contextlist = m_TransferContextList.find(fthandle);
          if (itr_contextlist != m_TransferContextList.end())
          {
               delete (*itr_contextlist).second;
               m_TransferContextList.erase(itr_contextlist);
          }    
     }
     
    // This method will be called during ProcessAbortFileTransferMessage i.e. failover.
    // This will be called once for each failed location.
    // output from this method will be used for cleaning Incoming queue and context.

     ResourceId   CFileTransferManagedObject::getAllTransfersFromLocation(const LocationId &locationId, vector<string> &vecFTHandle)
     {
          tracePrintf (TRACE_LEVEL_DEBUG, "CFileTransferManagedObject::getAllTransfersFromLocation called for location id %d", locationId);
          std::map<string,  CTransferContext *>::iterator itr_contextlist;     
          ResourceId status = WAVE_MESSAGE_SUCCESS;

          for(itr_contextlist = m_TransferContextList.begin(); itr_contextlist != m_TransferContextList.end(); itr_contextlist++)
          {
               CTransferContext *pContextObject = itr_contextlist->second;
               if (!pContextObject)
               {
                    continue;  // should never happen. Safety.
               }

               if (pContextObject->getSrcLocationId() == locationId)
               {
                    vecFTHandle.push_back(itr_contextlist->first);  // Save the FTHandle string to the output vector.
               }          
          }
          return (status);     
     }
     
     
     string   CFileTransferManagedObject::CreateTempFileName(const string &sFilename)
     {
        return(sFilename + ".tmp");
     }
     
     // Create a temp file on the target, memory map the same and return the CFile Object back.
     ResourceId   CFileTransferManagedObject::CreateTempFile(const string &stmpfilename, UI32 &nFileSize, CFile* &fileObj)
     {
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::CreateTempFile called for tempfilename [") + stmpfilename + "]\n");
          ResourceId status = WAVE_MESSAGE_SUCCESS;
          
          // Create a temp file, first.
          fileObj = NULL;
          // TODO: This needs to be optimized or done better.
          string stmpFileContent = ".";
          string sCmd = "echo -n \"";
          sCmd  += stmpFileContent;
          sCmd  += "\" > ";
          sCmd      += stmpfilename;
          system(sCmd.c_str());
          fileObj = new CFile(stmpfilename , true);
          WaveNs::prismAssert (NULL != fileObj, __FILE__, __LINE__);
          fileObj->Init();

          status = fileObj->ResizeMemoryMappedFile(nFileSize , sCmd.length());
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d] mmaped region remap failed with for filename [%s]  returning ....", __FUNCTION__, __LINE__, stmpfilename.c_str());
               delete fileObj;
               unlink(stmpfilename.c_str());
               return (status);
          }
    
          // Now create the fragment list for this file, after the new file has been fragmented.
          fileObj->FragmentFile();
          tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d] Total Fragments in the mmaped region for filename [%s] is = [%d]", __FUNCTION__, __LINE__, stmpfilename.c_str(), fileObj->GetFragmentCount());
          return(status);
     }


     void  CFileTransferManagedObject::setResponseStatus(const string &fthandle, const ResourceId &resStatus, const UI32 &nLocationId)
     {
          CTransferContext*  pContextObj = GetContextObject(fthandle);
          WaveNs::prismAssert (NULL != pContextObj, __FILE__, __LINE__);
          pContextObj->setResponseStatusCode(nLocationId, resStatus);
     }
     
     void  CFileTransferManagedObject::setFrameworkStatus(const string &fthandle, const FrameworkStatus &fwStatus, const UI32 &nLocationId)
     {
          CTransferContext*  pContextObj = GetContextObject(fthandle);
          WaveNs::prismAssert (NULL != pContextObj, __FILE__, __LINE__);
          pContextObj->setFrameworkStatusCode(nLocationId, fwStatus);     
     }
     
     FrameworkStatus  CFileTransferManagedObject::getFrameworkStatus(const string &fthandle, const UI32 &nLocationId)
     {
          CTransferContext*  pContextObj = GetContextObject(fthandle);
          WaveNs::prismAssert (NULL != pContextObj, __FILE__, __LINE__);
          return (pContextObj->getFrameworkStatusCode(nLocationId));
     }
     
     ResourceId          CFileTransferManagedObject::getResponseStatus(const string &fthandle, const UI32 &nLocationId)
     {
          CTransferContext*  pContextObj = GetContextObject(fthandle);
          WaveNs::prismAssert (NULL != pContextObj, __FILE__, __LINE__);
          return (pContextObj->getResponseStatusCode(nLocationId));     
     }

     ResourceId   CFileTransferManagedObject::AddToActiveRecepientList(FileTransferHandshakeMessage  *pFTHandShakeMessage, UI32 &nDestinationLocationId)
     {
          ResourceId status = WAVE_MESSAGE_SUCCESS;
          vector<UI32> vecLocation; 
          string fthandle = pFTHandShakeMessage->getTransferHandle();
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::AddToActiveRecepientList called for handle [") + fthandle + "]\n");


          do {

               std::map <string, vector<UI32> >:: iterator itr_map = m_ActiveReceiptList.begin(); 
               vector<UI32>::iterator itr_vecLocationId;
               itr_map = m_ActiveReceiptList.find(fthandle);
               if (itr_map == m_ActiveReceiptList.end())
               {
                    // First time filename addition.
                    trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::AddToActiveRecepientList called for handle [") + fthandle + "] , first time call.\n");
                    vecLocation.push_back(nDestinationLocationId);
                    m_ActiveReceiptList[fthandle] = vecLocation;

               } else {
                    // Update the active receipient list.
                    trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::AddToActiveRecepientList called for handle [") + fthandle + "] , called subsequently.\n");
                    vecLocation = itr_map->second;                    // Get the existing vector.
                    vecLocation.push_back(nDestinationLocationId);    // Append the new Destination LocationId
                    m_ActiveReceiptList[fthandle] = vecLocation;      // Save it back.
                    trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::AddToActiveRecepientList called for handle [") + fthandle + "] , Locationlist size = [" + vecLocation.size() + "]\n");
               }

          } while(0);
         
          return (status);
     }


    // This method will be called during 
    // TriggerFileTransferHandshakeCallback ()
    // Cleanup is done during callback for last fragment.

     ResourceId   CFileTransferManagedObject::AddToOutgoingQueueInFTTable(FileTransferHandshakeMessage  *pFTHandShakeMessage, UI32 &nDestinationLocationId)
     {
          ResourceId status = WAVE_MESSAGE_SUCCESS;
          CFile *pFileObj = NULL;
          CTransferContext *pContextObj = NULL;          
          
//          m_FTTAccessLock.lock();

          string sSourceFilename = pFTHandShakeMessage->getSourceFileName(); 
          string fthandle        = pFTHandShakeMessage->getTransferHandle();

          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::AddToOutgoingQueueInFTTable called for handle [") + fthandle + "]\n");

          do {

               // Check if the destination File is in use either as source or destination for another transfer. 
               if (true == IsInComingTransferList(sSourceFilename))
               {
                    trace (TRACE_LEVEL_INFO, string ("CFileTransferManagedObject::AddToOutgoingQueueInFTTable: Source file [") + sSourceFilename + "] involved in another incoming transfer. returning ... ");
                    status = WAVE_FILE_DESTINATION_FILE_BUSY_IN_ANOTHER_TRANSFER;
                    break;
               }

               // If an entry already exists for current file handle in Active receipt list, it means, context, cfile objects are already created.
               //    Then only add the location to RHS of active receipt list.
               // Else
               //    First create context, cfile objects.
               //    Then add new entry in active receipt list. LHS = file handle, RHS vector[0] = new receiver location.

               std::map <string, vector<UI32> >::iterator it1 = m_ActiveReceiptList.find (fthandle);
               std::map <string, vector<UI32> >::iterator it2 = m_ActiveReceiptList.end ();

               if (it1 == it2)
               {
                    // First time entry addition at File Transfer Origin.
                    pFileObj = new CFile(sSourceFilename, false);

                    WaveNs::prismAssert (NULL != pFileObj, __FILE__, __LINE__);

                    // Create a tempfilename first.
                    // This filename is not used here at sender location.
                    // Just being passed to context constructor.
                    string sDestinationFilename = pFTHandShakeMessage->getDestinationFileName(); 
                    string stmpFilename = CreateTempFileName(sDestinationFilename);               

                    pContextObj = new CTransferContext(sSourceFilename, pFTHandShakeMessage->getDestinationFileName(),
                                                       pFTHandShakeMessage->getSourceLocationId(), nDestinationLocationId,
                                                       stmpFilename, pFTHandShakeMessage->getFileSize(), pFileObj);
                                             
                    WaveNs::prismAssert (NULL != pContextObj, __FILE__, __LINE__);
                                             
                    // Following currently returns success always.
                    status = AddToContextList(fthandle, pContextObj);
                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                         break; 
                    }

                    // Following currently returns success always.
                    status = AddToOutgoingList(fthandle, sSourceFilename);              
                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                         break; 
                    }
               } 

               // Add the LocationList to the ActiveReceipientList now.
               // Following currently returns success always.
               status = AddToActiveRecepientList(pFTHandShakeMessage, nDestinationLocationId);
               if (WAVE_MESSAGE_SUCCESS != status)
               {
                    break; 
               }

          } while(0);

          if (WAVE_MESSAGE_SUCCESS != status)
          {
               // Here means IsInComingTransferList returned true.
               // Then following double deletes will not have problem.
               // TODO : Remove unnecessary delete statements.
               if (pFileObj)
                    delete pFileObj;
               pFileObj = NULL;
          
               // If error has occured. 
               if (pContextObj)
                    delete pContextObj;

               // Following will remove from map only if entry exists, otherwise, will simply return.
               RemoveFromContextList(fthandle);
          } 

  //        m_FTTAccessLock.unlock();
          return (status);
     }

     
     // This routine is called at the receiving end of a file transfer operation.          
     // (Inside FileTransferHandshakeMessageHandler.) 
     // Cleanup is done during failover, or during receipt of last fragment.

     ResourceId   CFileTransferManagedObject::AddToInComingQueueInFTTable(FileTransferHandshakeMessage  *pFTHandShakeMessage)
     {

          ResourceId status = WAVE_MESSAGE_SUCCESS;
          string sDestinationFilename , stmpFilename;
          UI32 nFileSize = 0;
          CFile *pFileObj = NULL;
          string fthandle = pFTHandShakeMessage->getTransferHandle();
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::AddToInComingQueueInFTTable called for handle [") + fthandle + "]\n");
          
//        m_FTTAccessLock.lock();
          do {
               // Check if the destination File is in use either as source or destination for another transfer. 
               sDestinationFilename = pFTHandShakeMessage->getDestinationFileName(); 
               if ((true == IsInComingTransferList(sDestinationFilename)) || (true == IsInOutGoingTransferList(sDestinationFilename)))
               {
                   trace (TRACE_LEVEL_INFO, string ("CFileTransferManagedObject::AddToInComingQueueInFTTable: Destination file [") + sDestinationFilename + "] involved in another transfer. returning ... ");
                   status = WAVE_FILE_DESTINATION_FILE_BUSY_IN_ANOTHER_TRANSFER;
                   break;
               }

               // Create a tempfilename first.
               stmpFilename = CreateTempFileName(sDestinationFilename);

               // Create a temp file to see if there is enough space.
               nFileSize = pFTHandShakeMessage->getFileSize();
               status = CreateTempFile(stmpFilename, nFileSize, pFileObj);
               if  (WAVE_MESSAGE_SUCCESS != status)
               {
                   trace (TRACE_LEVEL_INFO, string ("CFileTransferManagedObject::AddToInComingQueueInFTTable: Unable to create Destination file [") + sDestinationFilename + "] returning ... ");
                   status = WAVE_FILE_INSUFFICIENT_SPACE_AT_DESTINATION;
                   break;
               }
          
          } while (0);
          
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               return (status);
          }

          CTransferContext *pContextObj = NULL;
          do {

               pContextObj = new CTransferContext(pFTHandShakeMessage->getSourceFileName(), pFTHandShakeMessage->getDestinationFileName(), pFTHandShakeMessage->getSourceLocationId(), 
                                                  FrameworkToolKit::getThisLocationId(), stmpFilename, pFTHandShakeMessage->getFileSize(), pFileObj);
                                                 
               WaveNs::prismAssert (NULL != pContextObj, __FILE__, __LINE__);

               UI32 fileTransferFlag = pFTHandShakeMessage->getFileTransferFlag ();
               pContextObj->setFileTransferFlag (fileTransferFlag);

               // Following currently returns success always.
               status = AddToContextList(fthandle, pContextObj);
               if (WAVE_MESSAGE_SUCCESS != status)
               {
                    break; 
               }
               // This returns error if file is already a target of incoming transfer.
               // This error should not occur as IsInComingTransferList check is done before this.
               status = AddToIncomingList (fthandle, pFTHandShakeMessage->getDestinationFileName());
               if (WAVE_MESSAGE_SUCCESS != status)
               {
                    break; 
               }

          } while(0);

          if (WAVE_MESSAGE_SUCCESS != status)
          {
              // i.e. This error should never come per current logic.
              // If error has occured. 
              // RemoveFromContextList does delete on RHS context pointer.
              // if (pContextObj)
              //    delete pContextObj;

              // trace (TRACE_LEVEL_INFO, string ("CFileTransferManagedObject::AddToInComingQueueInFTTable: Removing temporary file [") + stmpFilename + "] ... ");
              // unlink is done by RemoveFromContextList => delete context.
              // unlink(stmpFilename.c_str());
              RemoveFromContextList(fthandle);
          } 

//        m_FTTAccessLock.unlock();
          return (status);
     }
     
     // Following function is called by file receiver, when last fragment for incoming file is received.
     bool CFileTransferManagedObject::doesFileNeedToBeSentToHaPeer (const string &fthandle)
     {
        // 1. Entry for fthandle must be present in m_IncomingActiveList.
        //    Verify/validate the same.

        map<string, string>::const_iterator itr_incomingList = m_IncomingActiveList.find (fthandle);
        map<string, string>::const_iterator end_incomingList = m_IncomingActiveList.end ();

        prismAssert (end_incomingList != itr_incomingList, __FILE__, __LINE__);


        // 2. Get context object corresponding to the fthandle.

        map<string, CTransferContext *>::const_iterator itr_contextList = m_TransferContextList.find (fthandle);
        map<string, CTransferContext *>::const_iterator end_contextList = m_TransferContextList.end ();

        prismAssert (end_contextList != itr_contextList, __FILE__, __LINE__);

        CTransferContext *pCTransferContext = itr_contextList->second;
        prismAssert (NULL != pCTransferContext, __FILE__, __LINE__);


        // 3. Get the value of the 'fileTransferFlag' from context object.

        UI32 fileTransferFlag = pCTransferContext->getFileTransferFlag ();

        bool fileNeedsToBeSentToHaPeer = false;

        if (0 != (FILE_PUSH_TO_HA_PEER & fileTransferFlag))
        {
            fileNeedsToBeSentToHaPeer = true;
        }


        return fileNeedsToBeSentToHaPeer;
     }

     // Following function is called only when 'doesFileNeedToBeSentToHaPeer' has returned true.
     void CFileTransferManagedObject::getFileDetailsForIncomingFile (const string &fthandle, string &fileName, UI32 &fileSize)
     {
        map<string, CTransferContext *>::const_iterator itr_contextList = m_TransferContextList.find (fthandle);
        map<string, CTransferContext *>::const_iterator end_contextList = m_TransferContextList.end ();

        prismAssert (end_contextList != itr_contextList, __FILE__, __LINE__);

        CTransferContext *pCTransferContext = itr_contextList->second;
        prismAssert (NULL != pCTransferContext, __FILE__, __LINE__);

        fileName            = pCTransferContext->getDestFilename ();
        fileSize            = pCTransferContext->getFileSize ();
     }

     void   CFileTransferManagedObject::RemoveFromFTTable(const string &fthandle) // Cleanup.
     {
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::RemoveFromFTTable called for handle [") + fthandle + "]\n");

//        m_FTTAccessLock.lock();
          
          RemoveFromContextList(fthandle);
          RemoveFromIncomingList(fthandle);        
          RemoveFromOutgoingList(fthandle);

  //      m_FTTAccessLock.unlock();
          
          return;     
     }

    // Removes location id from RHS of active recipient list.
    // Also removes handle entry from other lists when RHS vector of locations becomes empty.
     void    CFileTransferManagedObject::RemoveFromFTTable(const string &fthandle, const UI32 &nLocationId)
     {
          tracePrintf (TRACE_LEVEL_DEBUG, "CFileTransferManagedObject::RemoveFromFTTable called for fthandle [%s], for location id [%d]", fthandle.c_str (), nLocationId);

          do {

               std::map <string, vector<UI32> >:: iterator itr_map = m_ActiveReceiptList.begin(); 
               vector<UI32>::iterator itr_vecLocationId;
               itr_map = m_ActiveReceiptList.find(fthandle);
               if (itr_map != m_ActiveReceiptList.end())
               {
                    // Update the active receipient list.
                    vector<UI32> vecLocation = itr_map->second; // Get the existing vector.
                    vector<UI32>::iterator itr_vec =  vecLocation.begin();
                    vector<UI32> newVector;
                    for (;itr_vec != vecLocation.end(); itr_vec++)
                    {
                         if (*itr_vec != nLocationId)
                         {
                            newVector.push_back(*itr_vec);
                         } else {
                            tracePrintf (TRACE_LEVEL_DEBUG, "CFileTransferManagedObject::RemoveFromFTTable handle [%s], found LocationId [%d] in the list, cleaning up.", fthandle.c_str (), nLocationId);
                         }
                    }
                    
                    if (newVector.size())
                    {
                         m_ActiveReceiptList[fthandle] = newVector;      // Save it back.
                    } else {
                         RemoveFromFTTable(fthandle);  // No active receipients, so remove this entry.
                         // Remove from ActiveReceipientList as well.
                         m_ActiveReceiptList.erase(itr_map);
                    }
                    trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::RemoveFromFTTable called for handle [") + fthandle + "] , Locationlist size = [" + newVector.size() + "]\n");
               }
          } while(0);
     }


     bool    CFileTransferManagedObject::IsInComingTransferList(const string &sFilename) 
     {
          // trace (TRACE_LEVEL_INFO, string("CFileTransferManagedObject::IsInComingTransferActiveOnFile called for filename [") + sFilename + "]\n");
          std::map <string, string>::iterator itr_incominglist;
          for(itr_incominglist = m_IncomingActiveList.begin(); itr_incominglist != m_IncomingActiveList.end(); itr_incominglist++)
          {
               if (0 == ((*itr_incominglist).second).compare(sFilename))
               {
                    return true;
               }
          }
          return (false);
     }

     bool    CFileTransferManagedObject::IsInOutGoingTransferList(const string &sFilename)
     {
          trace (TRACE_LEVEL_DEBUG, string("CFileTransferManagedObject::IsOutGoingTransferActiveOnFile called for filename [") + sFilename + "]\n");
          std::map <string, string>::iterator itr_outgoinglist;
          for(itr_outgoinglist = m_OutgoingActiveList.begin(); itr_outgoinglist != m_OutgoingActiveList.end(); itr_outgoinglist++)
          {
               if (0 == ((*itr_outgoinglist).second).compare(sFilename))
               {
                    return true;
               }
          }
          return (false);
     }


     ResourceId   CFileTransferManagedObject::SaveFragmentToTempFile(const string fthandle, const UI32 nFragmentIndex, const unsigned char *pBuffer, const UI32 nBufferSize, bool bLastFragment)
     {
     
          const char *traceIsLastFragment = "FALSE";
          if (true == bLastFragment)
          {
            traceIsLastFragment = "TRUE";
          }

          tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d] called for handle [%s], FragmentIndex [%d], FragmentSize = [%d], BufferPtr = [0x%x], IsLastFragment [%s]\n", __FUNCTION__, __LINE__, fthandle.c_str(), nFragmentIndex, nBufferSize, pBuffer, traceIsLastFragment);
          ResourceId status = WAVE_MESSAGE_SUCCESS;
          CTransferContext *pContextObject = GetContextObject(fthandle);
          do {
          
               if (NULL == pContextObject)
               {
                    trace (TRACE_LEVEL_INFO, string("CFileTransferManagedObject::SaveFragmentToTempFile unable to find ContextObject for handle [") + fthandle + "] ... returning \n");
                    status = WAVE_FILE_INVALID_TRANSFER_CONTEXT_OBJECT;
                    break;
               }
              UI32 nExpectedFragmentIdx = pContextObject->getExpectedFragmentIndexAtReceiver();
              if (nExpectedFragmentIdx != nFragmentIndex)
              {
                    // We have missed some fragment in the interim, so report error and avoid any further processing.
                    tracePrintf (TRACE_LEVEL_ERROR, "[%s]:[%d]:Expected fragment idx [%d], received fragment idx [%d]\n", __FILE__, __LINE__, nExpectedFragmentIdx, nFragmentIndex);
                    status = WAVE_FILE_INVALID_TRANSFER_CONTEXT_OBJECT; // TODO: Need to have a seperate error code.
                    break;
              }

               // Retrieve the FileObj from context object.
               // Save this fragment to the FileObj.
               CFile *pFileObj = pContextObject->getFileObject();
               tracePrintf (TRACE_LEVEL_DEBUG, "[%s]:[%d] Handle [%s], FragmentObjectPtr =[0x%x], FragmentFilename = [%s]\n", __FUNCTION__, __LINE__, fthandle.c_str(), pFileObj, pFileObj->GetFileName().c_str());
               status = pFileObj->PutFragment(nFragmentIndex, pBuffer, nBufferSize);
               if (WAVE_MESSAGE_SUCCESS != status)
               {
                    tracePrintf (TRACE_LEVEL_INFO, "[%s][%d] Error: Unable to save file fragment [%d] to tempfile [%s] ", __FILE__, __LINE__, nFragmentIndex, pContextObject->getTmpDestFilename().c_str());
                    break;
               }

              // We have received the last fragment, reconstruct the file, and cleanup the FTTable.
              if (true == bLastFragment)
              {
                    pFileObj->UnmapTempFileAndClose();
                    // Now move the temp file to its destination location.
                    string sCmd = "/bin/mv " ;
                    sCmd += pContextObject->getTmpDestFilename();
                    sCmd += " " ;
                    sCmd += pContextObject->getDestFilename();
                    // tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d] Command to be executed for a final move is [%s]\n", __FILE__, __LINE__, sCmd.c_str());
                    system(sCmd.c_str()); // TODO: need to use popen.               
              } else {
                  // Increment the next FragmentIndex to expect.
                  pContextObject->incrementExpectedFragmentIndexAtReceiver();
              }
              
          } while(0);
     
          return (status);     
     }


     void CFileTransferManagedObject::DumpInfo()
     {
          if ((m_TransferContextList.size () == 0) && 
              (m_IncomingActiveList.size () == 0) && 
              (m_OutgoingActiveList.size () == 0) &&
              (m_ActiveReceiptList.size () == 0))
          {
              
              trace (TRACE_LEVEL_DEVEL, "CFileTransferManagedObject::FTTable is empty.");
              return;
          }
          std::map<string,  CTransferContext *>::iterator itr_ctxlist =  m_TransferContextList.begin();  // Map of FTHandle and Context structure.
          std::map <string, string>::iterator itr_inlist =  m_IncomingActiveList.begin();   // List to lookup incoming files to this node. map of IncomingFilename and FTHandle.
          std::map <string, string>::iterator itr_outlist =  m_OutgoingActiveList.begin();   // List to lookup the outgoing files from this node.  Map of outgoing Filename and FTHandle.
          std::map <string, vector<UI32> >:: iterator itr_ActiveReceipientList = m_ActiveReceiptList.begin(); // Map of active receipient list.
          string temp1, temp2;
          // tracePrintf(TRACE_LEVEL_DEBUG, " FileTransfer Table Contents \n");
          
          tracePrintf(TRACE_LEVEL_DEVEL, " ***********************************************************");
          tracePrintf(TRACE_LEVEL_DEVEL, " FileTransfer Table : Incoming File list **** , size [%d]", m_IncomingActiveList.size());
          for(; itr_inlist != m_IncomingActiveList.end() ; itr_inlist++)
          {
               temp1 = itr_inlist->first; temp2 = itr_inlist->second;
               trace (TRACE_LEVEL_DEVEL, string(" [") + temp1 + "]  =  [" + temp2 + "]");
          }
          // tracePrintf(TRACE_LEVEL_DEVEL, " ***********************************************************");
          
          tracePrintf(TRACE_LEVEL_DEVEL, " FileTransfer Table : Outgoing File list **** , size [%d]", m_OutgoingActiveList.size());
          for(; itr_outlist != m_OutgoingActiveList.end() ; itr_outlist++)
          {
               temp1 = itr_outlist->first; temp2 = itr_outlist->second;
               trace (TRACE_LEVEL_DEVEL, string(" [") + temp1 + "]  =  [" + temp2 + "]");
          }
          // tracePrintf(TRACE_LEVEL_DEVEL, " ************************************************************");

          tracePrintf(TRACE_LEVEL_DEVEL, " FileTransfer Table : Transfer Context Table **** , size [%d]", m_TransferContextList.size());
          for(; itr_ctxlist != m_TransferContextList.end() ; itr_ctxlist++)
          {
               tracePrintf(TRACE_LEVEL_DEVEL, " [%s]  =  [0x%p]", (itr_ctxlist->first).c_str() , (itr_ctxlist->second));
          }          
          // tracePrintf(TRACE_LEVEL_DEVEL, "**************************************************************");

          tracePrintf(TRACE_LEVEL_DEVEL, " FileTransfer Table : Active Receipient Table **** , size [%d]" , m_ActiveReceiptList.size());
          for (; itr_ActiveReceipientList != m_ActiveReceiptList.end(); itr_ActiveReceipientList++)
          {
              tracePrintf(TRACE_LEVEL_DEVEL, " FTHandle = [%s]", (itr_ActiveReceipientList->first).c_str() );
              vector<UI32>::iterator itr = (itr_ActiveReceipientList->second).begin();
              for(; itr != (itr_ActiveReceipientList->second).end(); itr++)
              {
                  tracePrintf(TRACE_LEVEL_DEVEL, "\t LocationId = [%d]", *itr);
              }
              tracePrintf(TRACE_LEVEL_DEVEL, "\t ------------------");
          }
          tracePrintf(TRACE_LEVEL_DEVEL, "**************************************************************");
           
          return;
     }

     // Following object is created at sender location. 
     //     - In this case destination location, and temp filename are not of much use.
     //     - m_pfileObj has source file.
     // Following object is created at receiver location. 
     //     - In this case, all members are useful.
     //     - m_pfileObj has tmp file. (Finally moved to destination filename/cleanued up.)

     // Note : deletion of context object also cleans file object properly.

     CTransferContext::CTransferContext(const string &srcFilename, const string &destFilename, const LocationId &srcLocationId, 
                                        const LocationId &dstLocationId, const string &stmpfilename, const UI32 &nFilesize, CFile *pFileObj)
     {
          m_sSrcFilename      = srcFilename;
          m_sDestFilename     = destFilename;
          m_nSrcLocationdId   = srcLocationId;
          m_nDstLocationdId   = dstLocationId;
          m_stmpDstFilename   = stmpfilename;
          m_nFileSize         = nFilesize;
          m_pfileObj          = pFileObj;
          m_nExpectedFragmentIndex = 0;
          m_fileTransferFlag       = 0;
     }
          
     
     
     CTransferContext::~CTransferContext()
     {
          // trace (TRACE_LEVEL_INFO, string("CTransferContext::~CTransferContext called for destination filename [") + m_sDestFilename + "]\n");
          if (m_pfileObj)
               delete m_pfileObj;
          m_pfileObj = NULL;
          // trace (TRACE_LEVEL_INFO, string ("CTransferContext::~CTransferContext: Removing temporary file [") + m_stmpDstFilename + "] ... ");          
          unlink(m_stmpDstFilename.c_str());
     }     

     string         CTransferContext::getSrcFilename()
     {
          return   m_sSrcFilename;                  
     }
     
     string         CTransferContext::getDestFilename()
     {
          return   m_sDestFilename;                  
     }
                         
     string         CTransferContext::getTmpDestFilename()
     {
          return  m_stmpDstFilename;                  
     }
                         
     LocationId     CTransferContext::getSrcLocationId()
     {
          return  m_nSrcLocationdId;                  
     }                    

     LocationId     CTransferContext::getDstLocationId()
     {
         return m_nDstLocationdId;
     }
               
     vector<LocationId>  CTransferContext::getDstLocationdIdList()
     {
          return  m_vecDstLocationId;                   
     }                    
     
     UI32  CTransferContext::getFileSize()
     {
          return   m_nFileSize;                  
     }                    

     CFile* CTransferContext::getFileObject(void)
     {
          return m_pfileObj;
     }
     
     UI32  CTransferContext::getExpectedFragmentIndexAtReceiver()
     {
            return (m_nExpectedFragmentIndex);
     }

     void   CTransferContext::incrementExpectedFragmentIndexAtReceiver()
     {
        ++m_nExpectedFragmentIndex;
     }

     FrameworkStatus CTransferContext::getFrameworkStatusCode(const UI32 &nLocationId)
     {
          std::map <UI32, FrameworkStatus>::iterator itr_map = m_FrameworkStatusList.find(nLocationId);
          FrameworkStatus status  = FRAMEWORK_ERROR;
          if (itr_map != m_FrameworkStatusList.end())
          { 
               status = itr_map->second;
          }
          return (status);
     }

     void   CTransferContext::setFrameworkStatusCode(const UI32 &nLocationId , const FrameworkStatus  &frameworkStatus)
     {
          m_FrameworkStatusList[nLocationId] = frameworkStatus;
     }

     ResourceId   CTransferContext::getResponseStatusCode(const UI32 &nLocationId)
     {
          std::map <UI32, ResourceId>::iterator itr_map = m_ResponsecodeList.find(nLocationId);
          ResourceId status  = WAVE_MESSAGE_ERROR;
          if (itr_map != m_ResponsecodeList.end())
          { 
               status = itr_map->second;
          }
          return (status);
     }

     void   CTransferContext::setResponseStatusCode(const UI32 &nLocationId , const ResourceId  &responseStatus)
     {
          m_ResponsecodeList[nLocationId] = responseStatus;
     }

     void CTransferContext::setFileTransferFlag (const UI32 &fileTransferFlag)
     {
        m_fileTransferFlag = fileTransferFlag;
     }

     UI32 CTransferContext::getFileTransferFlag () const
     {
        return m_fileTransferFlag;
     }

     
     
     
     
     

     CFile::CFile(const string &sFilename, bool bAllowUpdates)
     {
          m_Filename          = sFilename;
          m_fragmentsize      = 64 * 1024 ; // Currently set to 64K.
          // m_fragmentsize      = 8; // for simple x86 testing.
          m_filebuffer        = NULL;
          m_filesize          = 0;
          m_fd_handle         = -1;
          m_AllowUpdates      = bAllowUpdates;
          m_bFileMapped       = false;
          m_fragmentlist.clear();
     }

     CFile::~CFile()
     {
          vector<CFileFragment  *>::iterator itr;
          for(itr = m_fragmentlist.begin(); itr != m_fragmentlist.end(); itr++)
          {
              delete *itr;
          }
          m_fragmentlist.clear();
          UnmapTempFileAndClose();
     }
     
     ResourceId CFile::Init(void)
     {
          // Open the file.
          // memory map the file.
          // Create the fragment list and initialize the same.
          struct stat file_statbuf = {0};
          ResourceId status = WAVE_MESSAGE_SUCCESS;

          tracePrintf(TRACE_LEVEL_DEBUG, "CFile::Init  called for file [%s]\n", m_Filename.c_str());
          do {

               int rc = stat(m_Filename.c_str() , &file_statbuf);
               if (rc)
               {
                    // perror("stat");
                    trace (TRACE_LEVEL_INFO, string ("CFile::Init: Unable to stat file [") + m_Filename + "] returning... ");
                    status = WAVE_FILE_INVALID_FILENAME;
                    break;
               }

               m_fd_handle = open(m_Filename.c_str(), O_RDWR);
               if (m_fd_handle < 0)
               {
                    // perror("open");
                    trace (TRACE_LEVEL_INFO, string ("CFile::Init: Unable to open file [") + m_Filename + "] returning... ");
                    status = WAVE_FILE_INVALID_FILENAME;
                    break;
               }               

               // Set the file size.
               m_filesize = file_statbuf.st_size;

               // trace (TRACE_LEVEL_INFO, string ("CFile::Init reading the contents of the file [") + m_Filename + "] of size = [" + m_filesize + "]\n");

               int flags = PROT_READ; // default,
               if (true == m_AllowUpdates)
               {
                    // trace(TRACE_LEVEL_INFO, string("CFile::Init: Setting the remmap flag to PROT_WRITE   ...\n"));
                    flags = PROT_WRITE;
               }
               
               void *ptmpbuf = mmap((caddr_t)0, m_filesize , flags, MAP_SHARED, m_fd_handle, 0);
               if (ptmpbuf == (caddr_t)(-1)) {
                    trace (TRACE_LEVEL_INFO, string ("CFile::Init: Unable to mmap file [") + m_Filename + "] returning... ");
                    status = WAVE_FILE_INVALID_FILENAME;              
                    // perror("mmap");
                    break;
               }

               // All good now, re-cast and use it.               
               m_filebuffer = (unsigned char *)ptmpbuf;
               
               // Set the flag to indicate the memory mapping is successful.
               m_bFileMapped = true;
               
          } while(0);
                  
          // All good return from here.     
          return (status);          
     }

     void CFile::FragmentFile(void)
     {

          UI32 nTotalChunks = (m_filesize / m_fragmentsize);
          UI32 nLastChunkSize = (m_filesize % m_fragmentsize);
          unsigned char *pChunk = m_filebuffer; // Point to the head of the list.
          UI32 nIndex = 0;
          bool bLastFragment = false;
          // Build the fragment list.
          for(nIndex = 0 ; nIndex < nTotalChunks ; nIndex++)
          {
               tracePrintf (TRACE_LEVEL_DEBUG, "CFile::FragmentFile Creating FileFragment Object for index [%d] of size [%d]", nIndex, m_fragmentsize);
               pChunk = m_filebuffer + nIndex*m_fragmentsize; 
               if (((nTotalChunks - 1) == nIndex) && (0 == nLastChunkSize))
               {
                    // If this is the last fragment and LastChunkSize is zero,
                    bLastFragment = true;               
               } else {
                    bLastFragment = false;
               }
               CFileFragment *pFileFragmentObj = new CFileFragment(m_fragmentsize, pChunk, nIndex, bLastFragment);

               WaveNs::prismAssert (NULL != pFileFragmentObj, __FILE__, __LINE__);
               
               m_fragmentlist.push_back(pFileFragmentObj);
          }
          
          if (nLastChunkSize > 0)
          {
               pChunk  +=  m_fragmentsize;
               CFileFragment *pFileFragmentObj = new CFileFragment(nLastChunkSize, pChunk, nIndex, true);
               
               WaveNs::prismAssert (NULL != pFileFragmentObj, __FILE__, __LINE__);
                              
               tracePrintf (TRACE_LEVEL_DEBUG, "CFile::FragmentFile Creating FileFragment Object for index [%d] of size [%d]", nIndex, nLastChunkSize);
               m_fragmentlist.push_back(pFileFragmentObj);
          }     
     }
               
     ResourceId CFile::GetFragment(const unsigned int fragment_idx, unsigned char* &pBuf, unsigned int &fragmentsize)
     {
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called for Fragment Index [%d]", __FUNCTION__ , __LINE__ , fragment_idx);
          ResourceId status = WAVE_MESSAGE_SUCCESS;
          
          if (fragment_idx > m_fragmentlist.size())
          {
               tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d] Invalid fragment index [%d] passed in.", __FUNCTION__ , __LINE__, fragment_idx);
               status = WAVE_FILE_INVALID_FILE_FRAGMENT_INDEX;
               return (status);
          }          

          pBuf = m_filebuffer + fragment_idx*m_fragmentsize;
          fragmentsize =  m_fragmentlist[fragment_idx]->GetFragmentSize();
                    
          return(status);
     }
     
     ResourceId CFile::PutFragment(const unsigned int fragment_idx, const unsigned char* &pBuf, const unsigned int fragmentsize)
     {
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called for Fragment Index [%d], BufferPtr = [0x%x] Fragmentsize = [%d]\n", __FUNCTION__ , __LINE__ , fragment_idx, pBuf, fragmentsize);
          ResourceId status = WAVE_MESSAGE_SUCCESS;
          
          if (fragment_idx > m_fragmentlist.size())
          {
               tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d] Invalid fragment index [%d] passed in.", __FUNCTION__ , __LINE__, fragment_idx);
               status = WAVE_FILE_INVALID_FILE_FRAGMENT_INDEX;
               return (status);
          }          
          
          // unsigned char *pChunk = m_filebuffer + fragment_idx*m_fragmentsize;
          // TODO: Safety, check if fragmentsize is greater than m_fragmentsize;
          // tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Base Address [0x%x], Address to be copied [0x%x], Incoming buffer address [0x%x]\n", __FUNCTION__, __LINE__, m_filebuffer, pChunk, pBuf);

          // Write directly to the file.
          int retval = lseek(m_fd_handle, fragment_idx*m_fragmentsize, SEEK_SET);
		  
		  if (-1 == retval)
		  {
		  	
              tracePrintf(TRACE_LEVEL_FATAL, "File seek error error retval = %d \n", __FUNCTION__ , __LINE__ , retval);
              prismAssert (false, __FILE__, __LINE__);
		  }
		  
          write(m_fd_handle, pBuf, fragmentsize);
          // int nBytes = write(m_fd_handle, pBuf, fragmentsize);
          // tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Copied [%d] bytes to temp file from incoming buffer address [0x%x]\n", __FUNCTION__, __LINE__, nBytes, pBuf);          
          // memcpy(pChunk, pBuf, fragmentsize);
         
          return(status);          
     }
     
     ResourceId CFile::IsLastFileFragment(unsigned int fragment_idx, bool &bLastFragment)
     {
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
          ResourceId status = WAVE_MESSAGE_SUCCESS;

          if (fragment_idx > m_fragmentlist.size())
          {
               tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d] Invalid fragment index [%d] passed in.", __FUNCTION__ , __LINE__, fragment_idx);
               status = WAVE_FILE_INVALID_FILE_FRAGMENT_INDEX;
               return (status);
          }
          
          bLastFragment  = m_fragmentlist[fragment_idx]->IsLastFileFragment();

          const char *traceIsLastFragment = "FALSE";
          if (true == bLastFragment)
          {
            traceIsLastFragment = "TRUE";
          }

          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Fragment Index = [%d], LastSegment = [%s]\n", __FUNCTION__, __LINE__, fragment_idx, traceIsLastFragment);
                    
          return(status);     
     }
     
     UI32  CFile::GetFragmentCount(void)
     {
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
          return(m_fragmentlist.size());
     }

     string  CFile::GetFileName()
     {
          return (m_Filename);
     }

     ResourceId  CFile::ResizeMemoryMappedFile(unsigned int nNewFileSize , unsigned int nOldFilesize)
     {
          ResourceId status = WAVE_MESSAGE_SUCCESS;     
          tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  OldSize = [%d], newsize = [%d]\n", __FUNCTION__, __LINE__, nOldFilesize , nNewFileSize);
          unsigned long flags = MREMAP_MAYMOVE;
          if (true == m_bFileMapped)  //safeguard.
          {
               void *pNewBuf = mremap ((void *)m_filebuffer, nOldFilesize, nNewFileSize, flags);
               if (pNewBuf == (caddr_t)(-1)) {
                    tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d] Unable to remap file [%s] for size [%d]\n", __FUNCTION__, __LINE__, m_Filename.c_str(), nNewFileSize);
                    status = WAVE_FILE_MEMORY_MAP_OPERATION_FAILED;
                    // perror("mremap");
               } else {
                    // All good now, re-cast and use it.               
                    m_filebuffer = (unsigned char *)pNewBuf;
                    m_filesize = nNewFileSize;
                    tracePrintf(TRACE_LEVEL_DEBUG, "[%s]:[%d]  After Remap: Base Address of memory mapped file = [0x%x]\n", __FUNCTION__, __LINE__, m_filebuffer);
               }
           } else {
               status = WAVE_FILE_MEMORY_MAP_OPERATION_FAILED;
           }
          return (status);
     }

     void CFile::UnmapTempFileAndClose(void)
     {
          if (true == m_bFileMapped)
          {
               if (munmap((caddr_t)m_filebuffer, m_filesize) < 0)
               {
                    // perror("munmap");
                    tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d] munmap failed for file [%s] for size [%d]\n", __FUNCTION__, __LINE__, m_Filename.c_str(), m_filesize);
               }
               m_bFileMapped = false;
          }

          if (m_fd_handle > 0)
          {
               if (close(m_fd_handle) < 0)
               {
                    // perror("close");
               }
               m_fd_handle = -1; // Invalidate               
          }
     }




}



