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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVECLIENTSESSIONCONTEXT
#define WAVECLIENTSESSIONCONTEXT

#include "Framework/Types/Types.h"

namespace WaveNs
{

class WaveClientSessionContext
{
    private :
    protected :
    public :
                    WaveClientSessionContext           ();
                    WaveClientSessionContext           (const LocationId &waveClientOriginatingLocationId, const UI32 &waveNativeClientId, const UI32 &waveUserClientId);
                   ~WaveClientSessionContext           ();

        void        setWaveClientOriginatingLocationId (const LocationId &waveClientOriginatingLocationId);
        LocationId  getWaveClientOriginatingLocationId () const;
        void        setWaveNativeClientId              (const UI32 &waveNativeClientId);
        UI32        getWaveNativeClientId              () const;
        void        setWaveUserClientId                (const UI32 &waveUserClientId);
        UI32        getWaveUserClientId                () const;

    // Now the data members

    private :
        LocationId m_waveClientOriginatingLocationId;
        UI32       m_waveNativeClientId;
        UI32       m_waveUserClientId;

    protected :
    public :
};

}

#endif // WAVECLIENTSESSIONCONTEXT

