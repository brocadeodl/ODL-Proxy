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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef WAVEHANODE_H
#define WAVEHANODE_H

#include "Framework/ObjectModel/WaveLocalManagedObject.h"
#include "Cluster/Local/WaveHaNodeTypes.h"


namespace WaveNs
{

class WaveHaNode : public WaveLocalManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                            WaveHaNode      (WaveObjectManager *pWaveObjectManager);
                            WaveHaNode      (WaveObjectManager *pWaveObjectManager, const WaveHaNodeRole &role, const string &internalIpAddress, const string &seialNumber);
                            WaveHaNode      (WaveObjectManager *pWaveObjectManager, const WaveHaNodeRole &role);
                            WaveHaNode      (const WaveHaNode &waveNode);
        virtual            ~WaveHaNode      ();

        static  string      getClassName            ();

                WaveHaNodeRole  getHaRole           () const;
                void        setHaRole               (const WaveHaNodeRole &role);

                string      getInternalIpAddress    () const;
                void        setInternalIpAddress    (const string &ipAddress);

                string      getSerialNumber         () const;
                void        setSerialNumber         (const string &serialNumber);
    // now the data members

    private :
        WaveHaNodeRole      m_haRole;
        string              m_internalIpAddress;
        string              m_serialNumber;

    protected :
    public :
};

}

#endif // WAVEHANODE_H
