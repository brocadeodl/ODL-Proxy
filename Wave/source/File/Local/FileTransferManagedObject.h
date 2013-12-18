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


#ifndef FILESVCFTTABLE_H
#define FILESVCFTTABLE_H

#include <map>
#include "Framework/ObjectModel/WaveLocalObjectManager.h"

using namespace WaveNs;

namespace WaveNs
{

    class CFileFragment {
          public:

               CFileFragment(UI32 FragmentSize, unsigned char* fragment_block, UI32 FragmentIndex, bool bLastFragment=false);
               ~CFileFragment();
               UI32 GetFragmentSize();
               UI32 GetFragmentIndex();
               unsigned char *GetFragmentBlock();
               bool IsLastFileFragment();  
               void SetIsLastFragment(bool bVal);

          private:
               UI32  m_nFragmentSize;
               UI32  m_nFragmentIndex;
               unsigned char *m_pFragmentBlock;               
               bool  m_bLastFragment; 
     };
     
     
     class CFile {
     
          public:
               CFile(const string &sFilename , bool bAllowUpdates);
               
               ~CFile();
               void                  UnmapTempFileAndClose(void);
               ResourceId            Init(void);
               unsigned char        *GetFragment(unsigned int fragment_idx);
               ResourceId            IsLastFileFragment(unsigned int fragment_idx, bool &bLastFragment);
               ResourceId            GetFragment(const unsigned int fragment_idx, unsigned char* &pBuf, unsigned int &fragmentsize);
               ResourceId            PutFragment(const unsigned int fragment_idx, const unsigned char* &pBuf, const unsigned int fragmentsize);
               UI32                  GetFragmentCount(void);
               ResourceId            ResizeMemoryMappedFile(unsigned int nNewFileSize , unsigned int nOldFilesize);              
               string                GetFileName();
               void                  FragmentFile(void);
               
     
          private:
               string         m_Filename;
               int            m_fd_handle;
               unsigned char *m_filebuffer;      
               UI32           m_filesize;
               UI32           m_fragmentsize;
               bool           m_AllowUpdates;
               bool           m_bFileMapped;

               vector<CFileFragment  *>  m_fragmentlist;
     
     };    
     
     class CTransferContext  {

          public:
               CTransferContext(const string &srcFilename, const string &destFilename, const LocationId &srcLocationId, const LocationId &dstLocationId, const string &stmpFilename, const UI32 &nFilesize, CFile *pFileObj);
               ~CTransferContext();
               
               private:
                    string              m_sSrcFilename;
                    string              m_sDestFilename;
                    LocationId          m_nSrcLocationdId;
                    vector<LocationId>  m_vecDstLocationId;
                    LocationId          m_nDstLocationdId;
                    UI32                m_nFileSize;
                    UI32                m_nExpectedFragmentIndex;
                    string              m_stmpDstFilename;
                    CFile              *m_pfileObj;
                    map < UI32, ResourceId >      m_ResponsecodeList;           // List of Response Status code (forward call status)
                    map < UI32, FrameworkStatus > m_FrameworkStatusList;        // List of FrameStatus code.  (status in the callback context)                    
                    // Store file transfer flag value.
                    // This flag tells if file needs to be sent to Ha Peer (after receiving last fragment)
                    UI32                          m_fileTransferFlag;
                    
                    
               public:
                    string         getSrcFilename();
                    string         getDestFilename();
                    string         getTmpDestFilename();
                    LocationId     getSrcLocationId();
                    vector<LocationId>  getDstLocationdIdList();
                    LocationId     getDstLocationId();
                    UI32            getFileSize();
                    UI32            getExpectedFragmentIndexAtReceiver();
                    void            incrementExpectedFragmentIndexAtReceiver();
                    int             getDestFileHandle();
                    CFile          *getFileObject(void);
                    
                    FrameworkStatus getFrameworkStatusCode(const UI32 &nLocationId);
                    ResourceId      getResponseStatusCode(const UI32 &nLocationId);                    
                    void            setFrameworkStatusCode(const UI32 &nLocationId, const FrameworkStatus  &frameworkStatus);
                    void            setResponseStatusCode(const UI32 &nLocationId, const ResourceId  &responseStatus);

                    void            setFileTransferFlag         (const UI32 &fileTransferFlag);
                    UI32            getFileTransferFlag         () const;
                    
                    
               
     };

     // Only one instance within the LocalObjectManager.    
     class CFileTransferManagedObject {
          public:
               CFileTransferManagedObject();
               ~CFileTransferManagedObject();

               ResourceId          AddToOutgoingQueueInFTTable(FileTransferHandshakeMessage  *pFTHandShakeMessage, UI32 &nDestinationLocationId);
               ResourceId          AddToInComingQueueInFTTable(FileTransferHandshakeMessage  *pFTHandShakeMessage);
               void                RemoveFromFTTable(const string &fthandle); 
               void                RemoveFromFTTable(const string &fthandle, const UI32 &nLocationId);
               ResourceId          SaveFragmentToTempFile(const string fthandle, const UI32 nFragmentIndex, const unsigned char *pBuffer, const UI32 nBufferSize, bool bLastFragment=false);               
               bool                IsInComingTransferList(const string &sFilename);               // Return true if sFilename is not in the Incoming list, false otherwise.
               bool                IsInOutGoingTransferList(const string &sFilename);             // Return true if sFilename is not in the outgoing list, false otherwise.

               ResourceId          getAllTransfersFromLocation(const LocationId &LocationId, vector<string> &vecFTHandle); // Retrieve all the FT Handles where the source LocationId is passed in as argument.

               string              GenerateFTHandle(const LocationId srcLocationId, const string &srcFileName);
               void                setResponseStatus(const string &fthandle, const ResourceId &resStatus, const UI32 &nLocationId);         // Set the response code.
               void                setFrameworkStatus(const string &fthandle, const FrameworkStatus &fwStatus, const UI32 &nLocationId);    // Set the framework return code.
               FrameworkStatus     getFrameworkStatus(const string &fthandle, const UI32 &nLocationId);
               ResourceId          getResponseStatus(const string &fthandle, const UI32 &nLocationId);
               CTransferContext   *GetContextObject(const string &fthandle);
               void DumpInfo();

               bool                doesFileNeedToBeSentToHaPeer         (const string &fthandle);
               void                getFileDetailsForIncomingFile        (const string &fthandle, string &fileName, UI32 &fileSize);
               
               // cleanup.
               void                RemoveFromIncomingList(const string &fthandle);
               void                RemoveFromContextList(const string &fthandle);
               
          private:

               ResourceId          AddToIncomingList(const string &fthandle, const string &sFilename);
               ResourceId          AddToOutgoingList(const string &fthandle, const string &sFilename);
               ResourceId          AddToContextList(const string &fthandle, CTransferContext   *pContextObj);
               ResourceId          AddToActiveRecepientList(FileTransferHandshakeMessage  *pFTHandShakeMessage, UI32 &nDestinationLocationId);

               // cleanup 
               void                RemoveFromOutgoingList(const string &fthandle);

               ResourceId          CreateTempFile(const string &stmpfilename, UI32 &nFileSize, CFile* &fileObj);  // create dummyfile as staging area.
               string              CreateTempFileName(const string &sFilename);

          private:

               PrismMutex     m_FTTAccessLock;
               std::map<string,  CTransferContext *>   m_TransferContextList;        // Map of FTHandle and Context structure.
               std::map <string, string>               m_IncomingActiveList;         // List to lookup incoming files to this node. map of IncomingFilename and FTHandle.
               std::map <string, string>               m_OutgoingActiveList;         // List to lookup the outgoing files from this node.  Map of outgoing Filename and FTHandle.
               std::map <string, vector<UI32> >        m_ActiveReceiptList;          // List of Nodes receiving a file as part of a given handle.
               UI32                                    m_nFTIndex;

     };

}
#endif     //FILESVCFTTABLE_H
