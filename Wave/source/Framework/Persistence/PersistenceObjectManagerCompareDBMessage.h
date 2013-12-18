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

/**************************************************************************************
 *  @file : PersistenceObjectManagerCompareDBMessage.h                                *
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.                          *
 *   All rights reserved.                                                             *
 *   Description : declares a message PersistenceObjectManagerCompareDBMessage used   *
                   for databases compare on each nodes in cluster with Primary.       *
 *   Author : Pritee Ghosh                                                            *
 *   Date   : 02/22/2011                                                              *
 **************************************************************************************/

#ifndef PERSISTENCEOBJECTMANAGERCOMPAREDBMESSAGE_H
#define PERSISTENCEOBJECTMANAGERCOMPAREDBMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"


using namespace std;

namespace WaveNs
{

class PersistenceObjectManagerCompareDBMessage : public ManagementInterfaceMessage
{
    private :
        void                    setupAttributesForSerialization             ();
    protected :
    public :
                                PersistenceObjectManagerCompareDBMessage    ();
        virtual                 ~PersistenceObjectManagerCompareDBMessage   ();

        void                    setMatchingDbLocations                      (const vector<UI32> &matchingLocations);
        vector<LocationId>      getMatchingDbLocations                      () const;

        void                    setNonMatchingDbLocations                   (const vector<UI32> &matchingLocations);
        vector<LocationId>      getNonMatchingDbLocations                   () const;

        void                    setFailedLocations                          (const vector<UI32> &failedLocations);
        vector<UI32>            getFailedLocations                          () const;

        vector<string>          getIpAndPortsForMatchingDbLocations         () const;
        void                    setIpAndPortsForMatchingDbLocations         (const vector<string> &ipAndPortsForMatchingDbLocations);

        vector<string>          getIpAndPortsForNonMatchingDbLocations      () const;
        void                    setIpAndPortsForNonMatchingDbLocations      (const vector<string> &ipAndPortsForNonMatchingDbLocations);

        vector<string>          getIpAndPortsForFailedLocations             () const;
        void                    setIpAndPortsForFailedLocations             (const vector<string> &ipAndPortsForFailedLocations);

    // Now the data members
    private :
        vector<LocationId>      m_matchingDbLocations;
        vector<LocationId>      m_nonMatchingDbLocations;
        vector<LocationId>      m_failedLocations;
        vector<string>          m_ipAndPortsForMatchingDbLocations;
        vector<string>          m_ipAndPortsForNonMatchingDbLocations;
        vector<string>          m_ipAndPortsForFailedLocations;

    protected :
    public :
};

}

#endif // PERSISTENCEOBJECTMANAGERCOMPAREDBMESSAGE_H
