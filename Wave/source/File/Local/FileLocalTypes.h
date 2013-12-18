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

#ifndef FILELOCALTYPESH_H
#define FILELOCALTYPESH_H

using namespace WaveNs;

namespace WaveNs
{

const unsigned int sFILESERVICEVERSION = 0x0100;    // version major = 01, minor = 00.

    typedef enum
    {
        FILESVCFILETRANSFERHANDSHAKE = 0 ,
        FILESVCPUSHFILE,
        FILESVCPUSHFILEFRAGMENT,
        FILESVCABORTFILETRANSFER,
        FILESVCPUSHFILETOHAPEER,        // internal
        FILESVCFILEPUSHFILETOHAPEER,     // (as api)
        FILESVCREQUESTDELETEFILE,
        FILESVCDELETEFILE,
        FILESVCDELETEFILETOHAPEER
    }FileLocalMessageTypes;

     typedef enum
     {
     // Const flags related to file transfer.
     FILE_OVERWRITE_DEST_FILE_IF_EXIST=0,    // DEFAULT
     FILE_FAIL_IF_DEST_FILE_EXIST=1,
     FILE_DEST_PATH_ABSOLUTE=2, // At destination use the filename as absolute path.
     FILE_DEST_PATH_IWD=4, 		// At destination, prepend the initial_working_directory to the filename.
     FILE_DEST_PATH_CWD     = 8,   	// At destination, prepend the current_working_directory to the filename.
     FILE_PUSH_TO_HA_PEER   = 16   	// At destination, send the file to Ha peer location. (i.e. send to standby)
     } FileTransferFlag;

     typedef enum
     {
        // const enums for FileSvc message buffer types. 
        FILESVC_FILE_FRAGMENT_BUFFER_TAG            = 0,
        FILESVC_COMPLETE_FILE_BUFFER_FOR_HA_PEER
     }BufferTagsForFrameworkFileSvcMessages;

}
#endif   //FILELOCALTYPESH_H
