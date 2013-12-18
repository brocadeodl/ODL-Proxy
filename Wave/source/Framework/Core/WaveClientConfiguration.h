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
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#ifndef WAVECLIENTCONFIGURATION_H
#define WAVECLIENTCONFIGURATION_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class WaveClientConfiguration
{
    private :
    protected :
    public :
                WaveClientConfiguration   ();
               ~WaveClientConfiguration   ();

        string  getApplicationName        () const;
        void    setApplicationName        (const string &applicationName);

        bool    getIsADaemon              () const;
        void    setIsADaemon              (const bool &isADaemon);

        bool    getIsSysLogRequired       () const;
        void    setIsSysLogRequired       (const bool &isSysLogRequired);

        bool    getIsDatabaseEnabled      () const;
        void    setIsDatabaseEnabled      (const bool &isDatabaseenabled);

        SI32    getDatabasePort           () const;
        void    setDatabasePort           (const SI32 &databasePort);

        bool    getNeedShell              () const;
        void    setNeedShell              (const bool &needShell);

        string  getApplicationCompactName () const;

    // Now the data members

    private :
        string m_applicationName;
        bool   m_isADaemon;
        bool   m_isSysLogRequired;
        bool   m_isDatabaseEnabled;
        SI32   m_databasePort;
        bool   m_needShell;

    protected :
    public :
};

}

#endif //WAVECLIENTCONFIGURATION_H
