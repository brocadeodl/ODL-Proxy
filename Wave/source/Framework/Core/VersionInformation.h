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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef VERSIONINFORMATION_H
#define VERSIONINFORMATION_H

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/Types.h"
#include <string>

namespace WaveNs
{

class VersionInformation : virtual public SerializableObject
{
    private :
    protected :
        virtual void setupAttributesForSerialization ();

    public :
                            VersionInformation       ();
                           ~VersionInformation       ();

        string              getFirmwareVersion         () const;
        void                setFirmwareVersion         (const string &firmwareVersion);
        UI32                getHaVersion               () const;
        void                setHaVersion               (const UI32 &haVersion);

        void                display                    () const;

    // now the data members

    private :
        string      m_firmwareVersion;
        UI32        m_haVersion;

    protected :
    public :

    friend class PrismFrameworkObjectManagerHaSyncWorker;
};

}

#endif // VERSIONINFORMATION_H
