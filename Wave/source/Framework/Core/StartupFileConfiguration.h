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

#ifndef STARTUPFILECONFIGURATION_H
#define STARTUPFILECONFIGURATION_H

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/Types.h"
#include <vector>
#include <string>

namespace WaveNs
{

class StartupFileConfiguration : virtual public SerializableObject
{
    private :
    protected :
        virtual void setupAttributesForSerialization                        ();
        virtual void setupAttributesForSerializationInAttributeOrderFormat  ();

    public :
                            StartupFileConfiguration    ();
                           ~StartupFileConfiguration    ();

        UI32                getVersion                 () const;
        void                setVersion                 (const UI32 &version);
        UI32                getStartupFileType         () const;
        void                setStartupFileType         (const UI32 &startupFileType);
        string              getStartupFileName         () const;
        void                setStartupFileName         (const string &startupFileName);
        string              getStartupFile             () const;
        void                setStartupFile             (const string &startupFile);

        void                display                    () const;

    // now the data members

    private :
        UI32        m_version;
        UI32        m_startupFileType;
        string      m_startupFileName;
        string      m_startupFile;

    protected :
    public :

    friend class PrismFrameworkObjectManagerHaSyncWorker;
};

}

#endif // STARTUPFILECONFIGURATION_H
