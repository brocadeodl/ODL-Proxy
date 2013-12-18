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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMVERSION_H
#define PRISMVERSION_H

#include <string>
#include "Framework/Utils/StringUtils.h"

using namespace std;

namespace WaveNs
{

class PrismVersion
{
    private :
        PrismVersion ();

    protected :
    public:
                                  ~PrismVersion     ();

                     string        getMajor         () const;
                     string        getMinor         () const;
                     string        getPatch         () const;
                     string        getRevision      () const;
                     string        getBuildType     () const;
                     string        getBuildNumber   () const;
                     string        getDate          () const;
                     string        getUser          () const;
                     string        getHost          () const;
                     string        getDomain        () const;

        static const PrismVersion *getInstance      ();
        static       string        getVersionString ();

    // Now the data members

    private :
        string m_major;
        string m_minor;
        string m_patch;
        string m_revision;
        string m_buildType;
        string m_buildNumber;
        string m_date;
        string m_user;
        string m_host;
        string m_domain;

    protected:
    public:
};

}

#endif // PRISMVERSION_H