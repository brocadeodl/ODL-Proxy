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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVEMAINCONFIGURATION_H
#define WAVEMAINCONFIGURATION_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class WaveMainConfiguration
{
    private :
    protected :
    public :
                        WaveMainConfiguration                           ();
                       ~WaveMainConfiguration                           ();

        string          getApplicationName                              () const;
        void            setApplicationName                              (const string &applicationName);

        bool            getIsADaemon                                    () const;
        void            setIsADaemon                                    (const bool &isADaemon);

        bool            getIsSysLogRequired                             () const;
        void            setIsSysLogRequired                             (const bool &isSysLogRequired);

        bool            getIsDatabaseEnabled                            () const;
        void            setIsDatabaseEnabled                            (const bool &isDatabaseenabled);

        bool            getIsDatabaseLogEnabled                         () const;
        void            setIsDatabaseLogEnabled                         (const bool isDatabaseLogEnabled);

        SI32            getPort                                         () const;
        void            setPort                                         (const SI32 &port);

        SI32            getDatabasePort                                 () const;
        void            setDatabasePort                                 (const SI32 &databasePort);

        string          getDatabaseLogFileName                          () const;
        void            setDatabaseLogFileName                          (const string &databaseLogFileName);

        string          getDatabaseSharedBuffers                        () const;
        void            setDatabaseSharedBuffers                        (const string &databaseSharedBuffers);

        SI32            getManagementPort                               () const;
        void            setManagementPort                               (const SI32 &managementPort);

        string          getEthernetInterface                            () const;
        void            setEthernetInterface                            (const string &ethernetInterface);

        SI32            getManagementClientPort                         () const;
        void            setManagementClientPort                         (const SI32 &managementPort);

        string          getChangeDirectoryTo                            () const;
        void            setChangeDirectoryTo                            (const string &changeDirectoryTo);

        string          getApplicationCompactName                       () const;

        vector<string>  getYinPaths                                     () const;
        void            setYinPaths                                     (const vector<string> &yinPaths);

        string          getWyserTagsFilePath                            () const;
        void            setWyserTagsFilePath                            (const string &wyserTagsFilePath);

        string          getDatabaseEmptyType                            () const;
        void            setDatabaseEmptyType                            (const string &emptyDatabaseType);

        UI32            getDatabaseEmptyTypeAutoDetectionThresholdValue () const;
        void            setDatabaseEmptyTypeAutoDetectionThresholdValue (const UI32 &thresholdValue);
    // Now the data members

    private :
        string         m_applicationName;
        bool           m_isADaemon;
        bool           m_isSysLogRequired;
        bool           m_isDatabaseEnabled;
        bool           m_isDatabaseLogEnabled;
        SI32           m_port;
        SI32           m_databasePort;
        string         m_databaseLogFileName;
        string         m_databaseSharedBuffers;
        SI32           m_managementPort;
        string         m_ethernetInterface;
        SI32           m_managementClientPort;
        string         m_changeDirectoryTo;
        vector<string> m_yinPaths;
        string         m_wyserTagsFilePath;
        string         m_databaseEmptyType;
        UI32           m_databaseEmptyTypeAutoDetectionThresholdValue;
    protected :
    public :
};

}

#endif
