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


#ifndef FILEPUSHFILEMESSAGE_H
#define FILEPUSHFILEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"


using namespace WaveNs;

namespace WaveNs
{

    class FilePushFileMessage : public PrismMessage
    {
        private:
                    void           InitInternalTables(vector<UI32> &DestinationLocationIdList);
        protected:

            virtual void           setupAttributesForSerialization();

        public:

            FilePushFileMessage                                  ();

            FilePushFileMessage                                  (const string &SourceFileName,const string &DestinationFileName,const UI32 &SourceLocationId,
                                                                  vector<UI32> &DestinationLocationId, UI32 &FileTransferFlag) ;
          
            FilePushFileMessage                                  (const string &SourceFileName,const string &DestinationFileName,const UI32 &SourceLocationId,
                                                                  vector<UI32> &DestinationLocationId);

            virtual               ~FilePushFileMessage           ();
            
            void                   setSourceFileName             (const string &SourceFileName);
            string                 getSourceFileName             () const;
            void                   setDestinationFileName        (const string &DestinationFileName);
            string                 getDestinationFileName        () const;
            void                   setSourceLocationId           (const UI32 &SourceLocationId);
            UI32                   getSourceLocationId           () const;
            
            void                   setDestinationLocationIdList  (const vector<UI32> &vecLocationIdList);
            vector<UI32>        getDestinationLocationIdList     ();
                        
            void                   setFileTransferFlag           (const UI32 &FileTransferFlag);
            UI32                   getFileTransferFlag           () const;
            string                 getTransferHandle             ();
            void                   setTransferHandle             (string &TransferHandle);

            vector<FrameworkStatus>      getFrameworkStatusList  ();

            vector<ResourceId>     getResponseCodeList           ();
            void                   setResponseCodeList           (vector<ResourceId>  &responseStatus);
            UI32                   getLocationCountToReceiveNextMessage         () const;
            vector<UI32>           getDestinationLocationIdListForNextMessage   ();

            void                   removeLocationForSubsequentSends(const UI32 &nLocationId);
            

     // Now the data members
     private:

          string         m_sourceFileName;
          string         m_destinationFileName;
          UI32           m_sourceLocationId;
          vector<UI32>   m_vecDestinationLocationId;             // User supplied list of Locations to which this File needs to be pushed.
          vector<UI32>   m_vecLocationToReceiveNextMessage;      // List of locations to which the next message is to be received. Initially both this and the above list are same.

          UI32      m_fileTransferFlag;
          string    m_transferHandle;
          
          vector<ResourceId>        m_ResponseCodeList;
          vector<FrameworkStatus>   m_FrameworkStatusList;

     protected:
     public:
            
    };

    class FilePushFileToHaPeerMessage : public PrismMessage
    {
        private:
        protected:
            virtual void            setupAttributesForSerialization ();

        public:
                                    FilePushFileToHaPeerMessage     ();
                                    FilePushFileToHaPeerMessage     (const string &sourceFileName, const string &destinationFileName);
            virtual                ~FilePushFileToHaPeerMessage     ();
            
            void                    setSourceFileName               (const string &sourceFileName);
            string                  getSourceFileName               () const;
            void                    setDestinationFileName          (const string &destinationFileName);
            string                  getDestinationFileName          () const;
            void                    setFileSize                     (const UI32 &fileSize);
            UI32                    getFileSize                     () const;


        // Now the data members

        private:
            string                  m_sourceFileName;
            string                  m_destinationFileName;
            UI32                    m_fileSize;                 // Internal only.

        protected:
        public:
            
    };

}
#endif // FILEPUSHFILEMESSAGE_H

