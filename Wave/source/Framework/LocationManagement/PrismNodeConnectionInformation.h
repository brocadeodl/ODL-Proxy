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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMNODECONNECTIONINFORMATION_H
#define PRISMNODECONNECTIONINFORMATION_H

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/Types.h"

namespace WaveNs
{

class PrismNodeConnectionInformation : virtual public SerializableObject
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                        PrismNodeConnectionInformation ();
                        PrismNodeConnectionInformation (const string &thisLocationServerIpAddress, const UI32 &thisLocationServerPort, const LocationId &expectedLocationId = 0, const string &messageVersion = "");
        virtual        ~PrismNodeConnectionInformation ();

                string  getThisLocationServerIpAddress () const;
                void    setThisLocationServerIpAddress (const string &thisLocationServerIpAddress);
                UI32    getThisLocationServerPort      () const;
                void    setThisLocationServerPort      (const UI32 &thisLocationServerPort);
                LocationId  getExpectedLocationId      () const;
                void        setExpectedLocationId      (const LocationId &expectedLocationId);

                string      getMessageVersion          () const;   
                void        setMessageVersion          (const string &msgVersion); 

    // Now the data members

    private :
        string m_thisLocationServerIpAddress;
        UI32   m_thisLocationServerPort;
        UI32   m_expectedLocationId;
        string m_messageVersion;
    protected :
    public :
};


class PrismHaNodeConnectionInformation : virtual public SerializableObject
{
    private :
        virtual void setupAttributesForSerialization                        ();
        virtual void setupAttributesForSerializationInAttributeOrderFormat  ();
    protected :
    public :
                        PrismHaNodeConnectionInformation ();
                        PrismHaNodeConnectionInformation (const string &thisLocationServerIpAddress, const UI32 &thisLocationServerPort, const LocationId &expectedLocationId = 0, const string &messageVersion = "");
        virtual        ~PrismHaNodeConnectionInformation ();

                string  getThisLocationServerIpAddress () const;
                void    setThisLocationServerIpAddress (const string &thisLocationServerIpAddress);
                UI32    getThisLocationServerPort      () const;
                void    setThisLocationServerPort      (const UI32 &thisLocationServerPort);
                LocationId  getExpectedLocationId          () const;
                void        setExpectedLocationId          (const LocationId &expectedLocationId);
                string  getMessageVersion                  () const;
                void    setMessageVersion                  (const string &thisLocationServerIpAddress);
    // Now the data members

    private :
        string m_thisLocationServerIpAddress;
        UI32   m_thisLocationServerPort;
        UI32   m_expectedLocationId;
        string m_messageVersion;
    protected :
    public :
};


}

#endif // PRISMNODECONNECTIONINFORMATION_H


