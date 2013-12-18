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

#ifndef FILETRANSFERHANDSHAKEMESSAGE_H
#define FILETRANSFERHANDSHAKEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace WaveNs
{

    class FileTransferHandshakeMessage : public PrismMessage
    {
        private:
        protected:
            virtual void                          setupAttributesForSerialization         ();
        public:
                                                  FileTransferHandshakeMessage            ();
                                                  FileTransferHandshakeMessage            (const UI32 &ServiceVersion, const string &sourceFileName, const string &DestinationFileName, const UI32 &SourceLocationId, const UI32 &FileSize, const UI32 &FileTransferFlag);
            virtual                              ~FileTransferHandshakeMessage            ();

            void                                  setServiceVersion                       (const UI32 &ServiceVersion);
            UI32                                  getServiceVersion                       ()  const;
            void                                  setSourceFileName                       (const string &SourceFileName);
            string                                getSourceFileName                       ()  const;            
            void                                  setDestinationFileName                  (const string &DestinationFileName);
            string                                getDestinationFileName                  ()  const;
            void                                  setSourceLocationId                     (const UI32 &SourceLocationId);
            UI32                                  getSourceLocationId                     ()  const;
            void                                  setFileSize                             (const UI32 &FileSize);
            UI32                                  getFileSize                             ()  const;
            void                                  setFileTransferFlag                     (const UI32 &FileTransferFlag);
            UI32                                  getFileTransferFlag                     ()  const;
            void                                  setTransferHandle                       (const string &TransferHandle);
            string                                getTransferHandle                       ()  const;

        // Now the data members
        private:
            UI32                   m_serviceVersion;
            string                 m_sourceFileName;
            string                 m_destinationFileName;
            UI32                   m_sourceLocationId;
            UI32                   m_fileSize;
            UI32                   m_fileTransferFlag;
            string                 m_transferHandle;
            UI32                   m_responseCode;

        protected:
        public:
                    
    };
}
#endif  //   FILETRANSFERHANDSHAKEMESSAGE_H

