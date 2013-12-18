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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVEBROKERBASEDMESSAGEPAYLOAD_H
#define WAVEBROKERBASEDMESSAGEPAYLOAD_H

#include "Framework/Types/Types.h"

#include <vector>
#include <string>

using namespace std;

namespace WaveNs
{

class WaveBrokerBasedMessagePayload
{
    private :
    protected :
    public :
                        WaveBrokerBasedMessagePayload  ();
                       ~WaveBrokerBasedMessagePayload  ();

        UI32           &getMessageBodySize             ();
        char          *&getMessageBody                 ();
        vector<UI32>   &getBufferTags                  ();
        vector<UI32>   &getBufferSizes                 ();
        vector<UI8 *>  &getBuffers                     ();

        void            prepareAndGetIovecToPostOnWire (struct iovec **pIoVector, UI32 &ioVectorCount);

    // Now the data members

    private :
        UI32          m_messageBodySize;
        char         *m_pMessageBody;
        vector<UI32>  m_bufferTags;
        vector<UI32>  m_bufferSizes;
        vector<UI8 *> m_buffers;
        UI32          m_numberOfBuffers;
        bool          m_isPreparedForPostingOnWire;

        struct iovec *m_pIoVector;
        UI32           m_ioVectorCount;

    protected :
    public :
};

}

#endif // WAVEBROKERBASEDMESSAGEPAYLOAD_H
