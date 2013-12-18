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

#include "Framework/ObjectModel/WaveClientSessionContext.h"

namespace WaveNs
{

WaveClientSessionContext::WaveClientSessionContext ()
    : m_waveClientOriginatingLocationId (0),
      m_waveNativeClientId              (0),
      m_waveUserClientId                (0)
{
}

WaveClientSessionContext::WaveClientSessionContext (const LocationId &waveClientOriginatingLocationId, const UI32 &waveNativeClientId, const UI32 &waveUserClientId)
    : m_waveClientOriginatingLocationId (waveClientOriginatingLocationId),
      m_waveNativeClientId              (waveNativeClientId),
      m_waveUserClientId                (waveUserClientId)
{
}

WaveClientSessionContext::~WaveClientSessionContext ()
{
}

void WaveClientSessionContext::setWaveClientOriginatingLocationId (const LocationId &waveClientOriginatingLocationId)
{
    m_waveClientOriginatingLocationId = waveClientOriginatingLocationId;
}

LocationId WaveClientSessionContext::getWaveClientOriginatingLocationId () const
{
    return (m_waveClientOriginatingLocationId);
}

void WaveClientSessionContext::setWaveNativeClientId (const UI32 &waveNativeClientId)
{
    m_waveNativeClientId = waveNativeClientId;
}

UI32 WaveClientSessionContext::getWaveNativeClientId () const
{
    return (m_waveNativeClientId);
}

void WaveClientSessionContext::setWaveUserClientId (const UI32 &waveUserClientId)
{
    m_waveUserClientId = waveUserClientId;
}

UI32 WaveClientSessionContext::getWaveUserClientId () const
{
    return (m_waveUserClientId);
}

}
