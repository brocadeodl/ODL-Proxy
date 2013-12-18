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


/**************************************************************************
*   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
*   All rights reserved.                                                  *
*   Author : Mandar Datar                                                 *
***************************************************************************/

#ifndef VCSFABRICLOCALMANAGEMENTINTERFACEMESSAGES
#define VCSFABRICLOCALMANAGEMENTINTERFACEMESSAGES

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ManagementInterface/ManagementInterfaceServiceIndependentMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage : public ManagementInterfaceMessage
{
    private :
        void                        setupAttributesForSerialization                     ();

    protected :
    public :
                                    VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage  ();
        virtual                    ~VcsFabricGetCurrentRbridgeIdsAndLocationIdsMessage  ();

        const vector<UI32> &        getRbridgeIds                                       () const;
        void                        setRbridgeIds                                       (const vector<UI32> &rbridgeIds);

        const vector<LocationId> &  getLocationIds                                      () const;
        void                        setLocationIds                                      (const vector<LocationId> &locationIds);


    // Now the data members

    private :
        vector<UI32>                m_rbridgeIds;
        vector<LocationId>          m_locationIds;

    protected :
    public :
};

class VcsWyserEaRateLimitingDelayMessage : public ManagementInterfaceMessage
{
    private :
        void                        setupAttributesForSerialization                     ();

    protected :
    public :
                                    VcsWyserEaRateLimitingDelayMessage                    (const UI32 &operationId);
        virtual                    ~VcsWyserEaRateLimitingDelayMessage                    ();

        const string &              getWyserUserInfoContext                             () const;
        void                        setWyserUserInfoContext                             (const string &wyserUserSessionContext);

        const UI32 &                getDelayInMicroseconds                              () const;
        void                        setDelayInMicroseconds                              (const UI32 &delayInMicroseconds);


    // Now the data members

    private :
        string                      m_wyserUserSessionContext;
        UI32                        m_delayInMicroseconds;

    protected :
    public :

};

class VcsSetWyserEaRateLimitingDelayMessage : public VcsWyserEaRateLimitingDelayMessage
{   
    public :
                                    VcsSetWyserEaRateLimitingDelayMessage                 ();
        virtual                    ~VcsSetWyserEaRateLimitingDelayMessage                 ();
};

class VcsGetWyserEaRateLimitingDelayMessage : public VcsWyserEaRateLimitingDelayMessage
{
    public :
                                    VcsGetWyserEaRateLimitingDelayMessage                 ();
        virtual                    ~VcsGetWyserEaRateLimitingDelayMessage                 ();
};

class VcsSetWyserEaConfigFilterServiceIndependentMessage : public ManagementInterfaceServiceIndependentMessage
{
    private :
        void                        setupAttributesForSerialization                     ();

    protected :
    public :
                                    VcsSetWyserEaConfigFilterServiceIndependentMessage    ();
                                    VcsSetWyserEaConfigFilterServiceIndependentMessage    (const ResourceId &sessionConfigFilterContextType, const SI32 &userSessionId);
        virtual                    ~VcsSetWyserEaConfigFilterServiceIndependentMessage    ();

        const SI32                 &getUserSessionId                                    () const;
        void                        setUserSessionId                                    (const SI32 &userSessionId);

        const ResourceId           &getSessionConfigFilterContextType                   () const;
        void                        setSessionConfigFilterContextType                   (const ResourceId &sessionConfigFilterContextType);
        
    // Now the data members

    private :
        SI32                        m_userSessionId;
        ResourceId                  m_sessionConfigFilterContextType;

    protected :
    public :
};

class VcsRemoveWyserEaConfigFilterServiceIndependentMessage : public ManagementInterfaceServiceIndependentMessage
{
    private :
        void                        setupAttributesForSerialization                     ();

    protected :
    public :
                                    VcsRemoveWyserEaConfigFilterServiceIndependentMessage ();
                                    VcsRemoveWyserEaConfigFilterServiceIndependentMessage (const SI32 &userSessionId);
        virtual                    ~VcsRemoveWyserEaConfigFilterServiceIndependentMessage ();

        const SI32                 &getUserSessionId                                    () const;
        void                        setUserSessionId                                    (const SI32 &userSessionId);

    // Now the data members

    private :
        SI32                        m_userSessionId;

    protected :
    public :
};

}

#endif // VCSFABRICLOCALMANAGEMENTINTERFACEMESSAGES

