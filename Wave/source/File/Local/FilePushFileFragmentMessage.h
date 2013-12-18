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

#ifndef FILEPUSHFILEFRAGMENTMESSAGE_H
#define FILEPUSHFILEFRAGMENTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace WaveNs
{

    class FilePushFileFragmentMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization    ();
        public:
                         FilePushFileFragmentMessage        ();
                         FilePushFileFragmentMessage        (const string &TransferHandle,const LocationId& locationId, const UI32 &FileFragmentIndex,const UI32 &FragmentSize, bool LastFragment = false);
            virtual     ~FilePushFileFragmentMessage        ();
            void         setTransferHandle                  (const string &TransferHandle);
            string       getTransferHandle                  ()  const;
            void         setFileFragmentIndex               (const UI32 &FileFragmentIndex);
            UI32         getFileFragmentIndex               ()  const;
            void         setFragmentSize                    (const UI32 &FragmentSize);
            UI32         getFragmentSize                    ()  const;
            bool         IsLastFragment                     ()  const;
            LocationId   getSourceLocationId                () const;
            void         setSourceLocationId                (const LocationId &locationid); 

        // Now the data members
        private:
        protected:
        public:
            string       m_transferHandle;
            LocationId   m_sourceLocationId;
            UI32         m_fileFragmentIndex;
            UI32         m_fragmentSize;
            bool         m_lastFragment;
    };
}
#endif     // FILEPUSHFILEFRAGMENTMESSAGE_H

