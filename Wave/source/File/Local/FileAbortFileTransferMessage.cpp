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

#include "File/Local/FileLocalObjectManager.h"
#include "File/Local/FileAbortFileTransferMessage.h"
#include "File/Local/FileLocalTypes.h"

namespace WaveNs
{

    FileAbortFileTransferMessage::FileAbortFileTransferMessage ()
        : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCABORTFILETRANSFER)
    {
    }

    FileAbortFileTransferMessage::FileAbortFileTransferMessage (vector<LocationId> &vecLocationIdList)
        : PrismMessage (FileLocalObjectManager::getPrismServiceId (), FILESVCABORTFILETRANSFER),
       m_vecLocationId(vecLocationIdList)
    {
    }

    FileAbortFileTransferMessage::~FileAbortFileTransferMessage ()
    {
    }

    void  FileAbortFileTransferMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32Vector(&m_vecLocationId, "vecLocationId"));
    }

    vector<LocationId> FileAbortFileTransferMessage::getLocationdIdList() const
    {
        return  (m_vecLocationId);
    }

}
