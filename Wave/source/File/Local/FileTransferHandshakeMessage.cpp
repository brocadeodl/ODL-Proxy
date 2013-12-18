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
#include "File/Local/FileTransferHandshakeMessage.h"
#include "File/Local/FileLocalTypes.h"

namespace WaveNs
{

    FileTransferHandshakeMessage::FileTransferHandshakeMessage ()
        : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCFILETRANSFERHANDSHAKE),
        m_serviceVersion(0x0100),
        m_sourceLocationId(0),
        m_fileSize(0),
        m_fileTransferFlag(0),
        m_responseCode(0)
    {
    }

    FileTransferHandshakeMessage::FileTransferHandshakeMessage (const UI32 &ServiceVersion,const string &sourceFileName, const string &DestinationFileName,const UI32 &SourceLocationId,const UI32 &FileSize,const UI32 &fileTransferFlag)
        : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCFILETRANSFERHANDSHAKE),
        m_serviceVersion    (ServiceVersion),
        m_sourceFileName    (sourceFileName),        
        m_destinationFileName    (DestinationFileName),
        m_sourceLocationId    (SourceLocationId),
        m_fileSize    (FileSize),
        m_fileTransferFlag    (fileTransferFlag),
        m_transferHandle    (""),
        m_responseCode(0)
    {
    }

    FileTransferHandshakeMessage::~FileTransferHandshakeMessage ()
    {
    }

    void  FileTransferHandshakeMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_serviceVersion,"ServiceVersion"));
        addSerializableAttribute (new AttributeString(&m_sourceFileName,"SourceFileName"));        
        addSerializableAttribute (new AttributeString(&m_destinationFileName,"DestinationFileName"));
        addSerializableAttribute (new AttributeUI32(&m_sourceLocationId,"SourceLocationId"));
        addSerializableAttribute (new AttributeUI32(&m_fileSize,"FileSize"));
        addSerializableAttribute (new AttributeUI32(&m_fileTransferFlag,"FileTransferFlag"));
        addSerializableAttribute (new AttributeString(&m_transferHandle,"TransferHandle"));
        addSerializableAttribute (new AttributeUI32(&m_responseCode,"ResponseCode"));
    }

    void  FileTransferHandshakeMessage::setServiceVersion(const UI32 &ServiceVersion)
    {
        m_serviceVersion  =  ServiceVersion;
    }

    UI32  FileTransferHandshakeMessage::getServiceVersion() const
    {
        return (m_serviceVersion);
    }

  void  FileTransferHandshakeMessage::setSourceFileName(const string &sourceFileName)
    {
        m_sourceFileName  =  sourceFileName;
    }

    string  FileTransferHandshakeMessage::getSourceFileName() const
    {
        return (m_sourceFileName);
    }

    void  FileTransferHandshakeMessage::setDestinationFileName(const string &DestinationFileName)
    {
        m_destinationFileName  =  DestinationFileName;
    }

    string  FileTransferHandshakeMessage::getDestinationFileName() const
    {
        return (m_destinationFileName);
    }

    void  FileTransferHandshakeMessage::setSourceLocationId(const UI32 &SourceLocationId)
    {
        m_sourceLocationId  =  SourceLocationId;
    }

    UI32  FileTransferHandshakeMessage::getSourceLocationId() const
    {
        return (m_sourceLocationId);
    }

    void  FileTransferHandshakeMessage::setFileSize(const UI32 &FileSize)
    {
        m_fileSize  =  FileSize;
    }

    UI32  FileTransferHandshakeMessage::getFileSize() const
    {
        return (m_fileSize);
    }

    void  FileTransferHandshakeMessage::setFileTransferFlag(const UI32 &fileTransferFlag)
    {
        m_fileTransferFlag  =  fileTransferFlag;
    }

    UI32  FileTransferHandshakeMessage::getFileTransferFlag() const
    {
        return (m_fileTransferFlag);
    }

    void  FileTransferHandshakeMessage::setTransferHandle(const string &TransferHandle)
    {
        m_transferHandle  =  TransferHandle;
    }

    string  FileTransferHandshakeMessage::getTransferHandle() const
    {
        return (m_transferHandle);
    }

}

