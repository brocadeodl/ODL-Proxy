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

#include "File/Local/FileLocalObjectManager.h"
#include "File/Local/FileLocalMessages.h"
#include "File/Local/FileLocalTypes.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

PushFileToHaPeerMessage::PushFileToHaPeerMessage ()
    : PrismMessage          (FileLocalObjectManager::getPrismServiceId (), FILESVCPUSHFILETOHAPEER),
      m_serviceVersion      (sFILESERVICEVERSION),
      m_fileSize            (0)
{
}

PushFileToHaPeerMessage::PushFileToHaPeerMessage (const UI32 &serviceVersion, const string &transferHandle, const string &sourceFileName, const string &destinationFileName, const UI32 &fileSize)
    : PrismMessage          (FileLocalObjectManager::getPrismServiceId (), FILESVCPUSHFILETOHAPEER),
      m_serviceVersion      (serviceVersion),
      m_sourceFileName      (sourceFileName),
      m_destinationFileName (destinationFileName),
      m_fileSize            (fileSize),
      m_transferHandle      (transferHandle)
{
}

PushFileToHaPeerMessage::~PushFileToHaPeerMessage ()
{
}

void PushFileToHaPeerMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32     (&m_serviceVersion,     "ServiceVersion"));
    addSerializableAttribute (new AttributeString   (&m_sourceFileName,     "SourceFileName"));        
    addSerializableAttribute (new AttributeString   (&m_destinationFileName,"DestinationFileName"));
    addSerializableAttribute (new AttributeUI32     (&m_fileSize,           "FileSize"));
    addSerializableAttribute (new AttributeString   (&m_transferHandle,     "TransferHandle"));
}

void PushFileToHaPeerMessage::setServiceVersion (const UI32 &serviceVersion)
{
    m_serviceVersion = serviceVersion;
}

UI32 PushFileToHaPeerMessage::getServiceVersion () const
{
    return m_serviceVersion;
}

void PushFileToHaPeerMessage::setSourceFileName (const string &sourceFileName)
{
    m_sourceFileName = sourceFileName;
}

string PushFileToHaPeerMessage::getSourceFileName () const
{
    return m_sourceFileName;
}

void PushFileToHaPeerMessage::setDestinationFileName (const string &destinationFileName)
{
    m_destinationFileName = destinationFileName;
}

string PushFileToHaPeerMessage::getDestinationFileName() const
{
    return m_destinationFileName;
}

void PushFileToHaPeerMessage::setFileSize (const UI32 &fileSize)
{
    m_fileSize = fileSize;
}

UI32 PushFileToHaPeerMessage::getFileSize () const
{
    return m_fileSize;
}

void PushFileToHaPeerMessage::setTransferHandle (const string &transferHandle)
{
    m_transferHandle = transferHandle;
}

string PushFileToHaPeerMessage::getTransferHandle () const
{
    return m_transferHandle;
}

FileDeleteRequestMessage::FileDeleteRequestMessage ()
    : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCREQUESTDELETEFILE),
      m_destinationFileName     (""),
      m_fileTransferFlag        (0)  
{
}

FileDeleteRequestMessage::FileDeleteRequestMessage (const string &DestinationFileName, vector<UI32> &DestinationLocationIdList)
    : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCREQUESTDELETEFILE),
      m_destinationFileName      (DestinationFileName),
      m_vecDestinationLocationId (DestinationLocationIdList),
      m_vecLocationToReceiveNextMessage (DestinationLocationIdList),
      m_fileTransferFlag        (0)  
{
}

FileDeleteRequestMessage::FileDeleteRequestMessage (const string &DestinationFileName, vector<UI32> &DestinationLocationIdList, const UI32 &fileTransferFlag)
    : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCREQUESTDELETEFILE),
      m_destinationFileName      (DestinationFileName),
      m_vecDestinationLocationId (DestinationLocationIdList),
      m_vecLocationToReceiveNextMessage (DestinationLocationIdList),
      m_fileTransferFlag         (fileTransferFlag)
{
}

FileDeleteRequestMessage::~FileDeleteRequestMessage ()
{
}

void FileDeleteRequestMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeString(&m_destinationFileName,"DestinationFileName"));
    addSerializableAttribute (new AttributeUI32(&m_fileTransferFlag,"FileTransferFlag"));
    addSerializableAttribute (new AttributeResourceIdVector(&m_ResponseCodeList, "ResponseCodeList"));
}

void FileDeleteRequestMessage::setDestinationFileName(const string &DestinationFileName)
{   
    m_destinationFileName  =  DestinationFileName;
}

string FileDeleteRequestMessage::getDestinationFileName() const
{
    return (m_destinationFileName);
}

void FileDeleteRequestMessage::setDestinationLocationIdList  (const vector<UI32> &vecLocationIdList)
{
    m_vecDestinationLocationId = vecLocationIdList;
}

vector<UI32> FileDeleteRequestMessage::getDestinationLocationIdList  ()
{
    return (m_vecDestinationLocationId);
}

UI32 FileDeleteRequestMessage::getLocationCountToReceiveNextMessage() const
{
    return(m_vecLocationToReceiveNextMessage.size());
}

vector<UI32> FileDeleteRequestMessage::getDestinationLocationIdListForNextMessage() 
{
    return (m_vecLocationToReceiveNextMessage);
}

void FileDeleteRequestMessage::removeLocationForSubsequentSends(const UI32 &nLocationId)
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

void FileDeleteRequestMessage::setFileTransferFlag(const UI32 &fileTransferFlag)
{
    m_fileTransferFlag  =  fileTransferFlag;
}

UI32 FileDeleteRequestMessage::getFileTransferFlag() const
{
    return (m_fileTransferFlag);
}

vector<ResourceId> FileDeleteRequestMessage::getResponseCodeList ()
{
    return m_ResponseCodeList;     
}

void FileDeleteRequestMessage::setResponseCodeList(vector<ResourceId>  &responseStatus)
{
    m_ResponseCodeList = responseStatus;
}

FileDeleteFileMessage::FileDeleteFileMessage ()
    : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCDELETEFILE ),
      m_destinationFileName     (""),
      m_fileTransferFlag        (0)    
{
}

FileDeleteFileMessage::FileDeleteFileMessage (const string &DestinationFileName)
    : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCDELETEFILE),
      m_destinationFileName     (DestinationFileName),
      m_fileTransferFlag        (0)
{
}

FileDeleteFileMessage::FileDeleteFileMessage (const string &DestinationFileName, const UI32 &fileTransferFlag) 
    : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCDELETEFILE),
      m_destinationFileName     (DestinationFileName),
      m_fileTransferFlag        (fileTransferFlag)
{
}

FileDeleteFileMessage::~FileDeleteFileMessage ()
{
}

void FileDeleteFileMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeString(&m_destinationFileName,"DestinationFileName"));
    addSerializableAttribute (new AttributeUI32(&m_fileTransferFlag,"FileTransferFlag"));    
}

void FileDeleteFileMessage::setDestinationFileName(const string &DestinationFileName)
{
    m_destinationFileName  =  DestinationFileName;
}

string FileDeleteFileMessage::getDestinationFileName() const
{
    return (m_destinationFileName);
}

void FileDeleteFileMessage::setFileTransferFlag(const UI32 &fileTransferFlag)
{
    m_fileTransferFlag  =  fileTransferFlag;
}

UI32 FileDeleteFileMessage::getFileTransferFlag() const
{
    return (m_fileTransferFlag);
}

FileDeleteFileToHaPeerMessage::FileDeleteFileToHaPeerMessage ()
    : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCDELETEFILETOHAPEER ),
      m_destinationFileName     (""),
      m_fileTransferFlag        (0)  
{
}

FileDeleteFileToHaPeerMessage::FileDeleteFileToHaPeerMessage (const string &DestinationFileName)
    : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCDELETEFILETOHAPEER ),
      m_destinationFileName      (DestinationFileName)
{
}

FileDeleteFileToHaPeerMessage::FileDeleteFileToHaPeerMessage (const string &DestinationFileName, const UI32 &fileTransferFlag)
    : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCDELETEFILETOHAPEER ),
      m_destinationFileName     (DestinationFileName),
      m_fileTransferFlag        (fileTransferFlag)
{
}

FileDeleteFileToHaPeerMessage::~FileDeleteFileToHaPeerMessage()
{
}

void FileDeleteFileToHaPeerMessage::setupAttributesForSerialization()
{   
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeString(&m_destinationFileName,"DestinationFileName"));
    addSerializableAttribute (new AttributeUI32(&m_fileTransferFlag,"FileTransferFlag"));
}

void FileDeleteFileToHaPeerMessage::setDestinationFileName(const string &DestinationFileName)
{   
    m_destinationFileName  =  DestinationFileName;
}

string FileDeleteFileToHaPeerMessage::getDestinationFileName() const
{   
    return (m_destinationFileName);
}

void FileDeleteFileToHaPeerMessage::setFileTransferFlag(const UI32 &fileTransferFlag)
{
    m_fileTransferFlag  =  fileTransferFlag;
}   

UI32 FileDeleteFileToHaPeerMessage::getFileTransferFlag() const
{
    return (m_fileTransferFlag);
}

}
