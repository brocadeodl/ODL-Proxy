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
 *   Author : jiyer                                                        *
 **************************************************************************/

#include "File/Local/FilePushFileMessage.h"
#include "File/Local/FileLocalObjectManager.h"
#include "File/Local/FileLocalTypes.h"
#include "Framework/Attributes/AttributeResourceId.h"


namespace WaveNs
{

    FilePushFileMessage::FilePushFileMessage ()
        : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCPUSHFILE),
         m_sourceLocationId(0),        
         m_fileTransferFlag(0)
    {
    }

    FilePushFileMessage::FilePushFileMessage (const string &SourceFileName,const string &DestinationFileName,const UI32 &SourceLocationId, vector<UI32> &DestinationLocationIdList, UI32 &fileTransferFlag)
        : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCPUSHFILE),
        m_sourceFileName           (SourceFileName),
        m_destinationFileName      (DestinationFileName),
        m_sourceLocationId         (SourceLocationId),
        m_vecDestinationLocationId (DestinationLocationIdList),
        m_vecLocationToReceiveNextMessage (DestinationLocationIdList),
        m_fileTransferFlag         (fileTransferFlag),
        m_transferHandle           ("")
    {
          InitInternalTables(DestinationLocationIdList);
    }
    
            
    FilePushFileMessage::FilePushFileMessage (const string &SourceFileName,const string &DestinationFileName,const UI32 &SourceLocationId, vector<UI32> &DestinationLocationIdList)
        : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCPUSHFILE),
        m_sourceFileName           (SourceFileName),
        m_destinationFileName      (DestinationFileName),
        m_sourceLocationId         (SourceLocationId),
        m_vecDestinationLocationId (DestinationLocationIdList),
        m_vecLocationToReceiveNextMessage (DestinationLocationIdList),
        m_fileTransferFlag         (0),
        m_transferHandle           ("")
    {
          InitInternalTables(DestinationLocationIdList);
    }
    
    FilePushFileMessage::~FilePushFileMessage ()
    {
    }

    void  FilePushFileMessage::InitInternalTables(vector<UI32> &DestinationLocationIdList)
    {
          // Set the ResponseStatus and FrameworkStatus Table to Success for all locations.
          vector<UI32>::iterator itr = DestinationLocationIdList.begin();
          for (; DestinationLocationIdList.end() != itr ; itr++)
          {
               m_ResponseCodeList.push_back(WAVE_MESSAGE_SUCCESS);
               m_FrameworkStatusList.push_back(FRAMEWORK_SUCCESS);
          }
    }

    void  FilePushFileMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_sourceFileName,"SourceFileName"));
        addSerializableAttribute (new AttributeString(&m_destinationFileName,"DestinationFileName"));
        addSerializableAttribute (new AttributeUI32(&m_sourceLocationId,"SourceLocationId"));
        addSerializableAttribute (new AttributeUI32(&m_fileTransferFlag,"FileTransferFlag"));
        addSerializableAttribute (new AttributeString(&m_transferHandle,"TransferHandle"));
        addSerializableAttribute (new AttributeLocationIdVector(&m_vecDestinationLocationId, "LocationIdListFromUser"));
        addSerializableAttribute (new AttributeLocationIdVector(&m_vecLocationToReceiveNextMessage, "LocationIdToReceiveNextMessage"));
        addSerializableAttribute (new AttributeResourceIdVector(&m_ResponseCodeList, "ResponseCodeList"));
//      addSerializableAttribute (new AttributeResourceIdVector(&m_FrameworkStatusList, "FrameworkStatusList"));
    }

    void  FilePushFileMessage::setSourceFileName(const string &SourceFileName)
    {
        m_sourceFileName  =  SourceFileName;
    }

    string  FilePushFileMessage::getSourceFileName() const
    {
        return (m_sourceFileName);
    }

    void  FilePushFileMessage::setDestinationFileName(const string &DestinationFileName)
    {
        m_destinationFileName  =  DestinationFileName;
    }

    string  FilePushFileMessage::getDestinationFileName() const
    {
        return (m_destinationFileName);
    }

    void  FilePushFileMessage::setSourceLocationId(const UI32 &SourceLocationId)
    {
        m_sourceLocationId  =  SourceLocationId;
    }

    UI32  FilePushFileMessage::getSourceLocationId() const
    {
        return (m_sourceLocationId);
    }

     void   FilePushFileMessage::setDestinationLocationIdList  (const vector<UI32> &vecLocationIdList)
     {
          m_vecDestinationLocationId = vecLocationIdList;
     }

     vector<UI32>  FilePushFileMessage::getDestinationLocationIdList  ()
     {
          return m_vecDestinationLocationId;
     }

    void  FilePushFileMessage::setFileTransferFlag(const UI32 &fileTransferFlag)
    {
        m_fileTransferFlag  =  fileTransferFlag;
    }

    UI32  FilePushFileMessage::getFileTransferFlag() const
    {
        return (m_fileTransferFlag);
    }

    void   FilePushFileMessage::setTransferHandle(string &TransferHandle)
    {
          m_transferHandle = TransferHandle;
    }

    string  FilePushFileMessage::getTransferHandle()
    {
          return m_transferHandle;
    }

     UI32  FilePushFileMessage::getLocationCountToReceiveNextMessage() const
     {
          return(m_vecLocationToReceiveNextMessage.size());
     }

     vector<UI32>   FilePushFileMessage::getDestinationLocationIdListForNextMessage() 
     {
          return (m_vecLocationToReceiveNextMessage);
     }

     void  FilePushFileMessage::removeLocationForSubsequentSends(const UI32 &nLocationId)
     {
          bool bLocationFound = false;
          vector<UI32>::iterator itr =  m_vecLocationToReceiveNextMessage.begin();
          for(; itr != m_vecLocationToReceiveNextMessage.end(); itr++)
          {
               if ((*itr) == nLocationId)
               {
                    bLocationFound = true;
                    break;
               }
          }
          // Remove from the list.
          if (true == bLocationFound)
          {
               m_vecLocationToReceiveNextMessage.erase(itr);
          }
     }

     vector<ResourceId>     FilePushFileMessage::getResponseCodeList           ()
     {
          return m_ResponseCodeList;     
     }

     void   FilePushFileMessage::setResponseCodeList(vector<ResourceId>  &responseStatus)
     {
          m_ResponseCodeList = responseStatus;
     }
     
     vector<FrameworkStatus> FilePushFileMessage::getFrameworkStatusList       ()
     {
          return m_FrameworkStatusList;     
     }


FilePushFileToHaPeerMessage::FilePushFileToHaPeerMessage ()
    : PrismMessage          (FileLocalObjectManager::getPrismServiceId (), FILESVCFILEPUSHFILETOHAPEER),
      m_fileSize            (0)
{
}

FilePushFileToHaPeerMessage::FilePushFileToHaPeerMessage (const string &sourceFileName, const string &destinationFileName)
    : PrismMessage          (FileLocalObjectManager::getPrismServiceId (), FILESVCFILEPUSHFILETOHAPEER),
      m_sourceFileName      (sourceFileName),
      m_destinationFileName (destinationFileName),
      m_fileSize            (0)
{
}

FilePushFileToHaPeerMessage::~FilePushFileToHaPeerMessage ()
{
}

void FilePushFileToHaPeerMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_sourceFileName,       "SourceFileName"));
    addSerializableAttribute (new AttributeString (&m_destinationFileName,  "DestinationFileName"));
    // (Used only inside handler as temporary storage between sequencer steps.)
    // addSerializableAttribute (new AttributeUI32   (&m_fileSize,             "FileSize"));
}

void FilePushFileToHaPeerMessage::setSourceFileName (const string &sourceFileName)
{
    m_sourceFileName = sourceFileName;
}

string FilePushFileToHaPeerMessage::getSourceFileName () const
{
    return m_sourceFileName;
}

void FilePushFileToHaPeerMessage::setDestinationFileName (const string &destinationFileName)
{
    m_destinationFileName = destinationFileName;
}

string FilePushFileToHaPeerMessage::getDestinationFileName () const
{
    return m_destinationFileName;
}

void FilePushFileToHaPeerMessage::setFileSize (const UI32 &fileSize)
{
    m_fileSize = fileSize;
}

UI32 FilePushFileToHaPeerMessage::getFileSize () const
{
    return m_fileSize;
}

}
