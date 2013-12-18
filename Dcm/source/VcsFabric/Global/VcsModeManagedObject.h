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

/**
 *   @file VcsModeManagedObject.h
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Description:    This file defines the header for the VcsModeManagedObject.
 *   Author:         Brian Adaniya
 *   Date:           12/13/2010
 */

#ifndef VCSMODEMANAGEDOBJECT_H
#define VCSMODEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsModeManagedObject : public DcmManagedObject
{
    private :
        virtual void    setupAttributesForPersistence   ();
        virtual void    setupAttributesForCreate        ();

    protected :
    public :
                        VcsModeManagedObject            (WaveObjectManager *pWaveObjectManager);
        virtual        ~VcsModeManagedObject            ();

        static  string  getClassName                    ();

                bool    getVcsMode                      () const;
                void    setVcsMode                      (bool vcsMode);

                bool    getVcsClusterMode               () const;
                void    setVcsClusterMode               (bool vcsClusterMode);

    // Now the data members
    private :
                bool    m_vcsMode;
                bool    m_vcsClusterMode;

                UI32    m_swbdNumber;

    protected :
    public :
};

}

#endif // VCSMODEMANAGEDOBJECT_H
