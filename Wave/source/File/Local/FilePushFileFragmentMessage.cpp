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

/**********************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.     *
 *   All rights reserved.                                             *
 *   Author : jiyer                                                   *
 **********************************************************************/

#include "File/Local/FileLocalObjectManager.h"
#include "File/Local/FilePushFileFragmentMessage.h"
#include "File/Local/FileLocalTypes.h"

namespace WaveNs
{

    FilePushFileFragmentMessage::FilePushFileFragmentMessage ()
        : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCPUSHFILEFRAGMENT),
        m_sourceLocationId    (0),
        m_fileFragmentIndex   (0),
        m_fragmentSize        (0),
        m_lastFragment        (false)        
    {
    }

    FilePushFileFragmentMessage::FilePushFileFragmentMessage (const string &TransferHandle,const LocationId &sourceLocationId, const UI32 &FileFragmentIndex,const UI32 &FragmentSize, bool LastFragment)
        : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCPUSHFILEFRAGMENT),
        m_transferHandle      (TransferHandle),
        m_sourceLocationId    (sourceLocationId),
        m_fileFragmentIndex   (FileFragmentIndex),
        m_fragmentSize        (FragmentSize),
        m_lastFragment        (LastFragment)
    {
    }

    FilePushFileFragmentMessage::~FilePushFileFragmentMessage ()
    {
    }

    void  FilePushFileFragmentMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_transferHandle,"TransferHandle"));
        addSerializableAttribute (new AttributeUI32(&m_sourceLocationId,"SourceLocationId"));
        addSerializableAttribute (new AttributeUI32(&m_fileFragmentIndex,"FileFragmentIndex"));
        addSerializableAttribute (new AttributeUI32(&m_fragmentSize,"FragmentSize"));
        addSerializableAttribute (new AttributeBool(&m_lastFragment,"LastFragment"));
    }

    void  FilePushFileFragmentMessage::setTransferHandle(const string &TransferHandle)
    {
        m_transferHandle  =  TransferHandle;
    }

    string  FilePushFileFragmentMessage::getTransferHandle() const
    {
        return (m_transferHandle);
    }

    void  FilePushFileFragmentMessage::setFileFragmentIndex(const UI32 &FileFragmentIndex)
    {
        m_fileFragmentIndex  =  FileFragmentIndex;
    }

    UI32  FilePushFileFragmentMessage::getFileFragmentIndex() const
    {
        return (m_fileFragmentIndex);
    }

    void  FilePushFileFragmentMessage::setFragmentSize(const UI32 &FragmentSize)
    {
        m_fragmentSize  =  FragmentSize;
    }

    UI32  FilePushFileFragmentMessage::getFragmentSize() const
    {
        return (m_fragmentSize);
    }

    void FilePushFileFragmentMessage::setSourceLocationId(const LocationId &locationId)
    {
        m_sourceLocationId = locationId;
    }

    LocationId  FilePushFileFragmentMessage::getSourceLocationId() const
    {
        return m_sourceLocationId;
    }
    bool  FilePushFileFragmentMessage::IsLastFragment() const
    {
        return (m_lastFragment);
    }

}
