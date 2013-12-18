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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef PERSISTENCEOBJECTMANAGERMESSAGES_H
#define PERSISTENCEOBJECTMANAGERMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class PersistenceObjectManagerAddXPathStringsMessage : public ManagementInterfaceMessage
{
    private:
                void                        setupAttributesForSerialization                             ();
    protected :
    public :
                                            PersistenceObjectManagerAddXPathStringsMessage              (const vector<string> &xPathStrings);
                                            PersistenceObjectManagerAddXPathStringsMessage              ();
        virtual                            ~PersistenceObjectManagerAddXPathStringsMessage              ();

                vector<string>              getXPathStrings                                             () const;
                void                        setXPathStrings                                             (const vector<string> &xPathStrings);

    // Now the data members

    private :
        vector<string>              m_xPathStrings;

    protected :
    public :
};

class PersistenceObjectManagerDeleteXPathStringsMessage : public ManagementInterfaceMessage
{
    private:
                void                        setupAttributesForSerialization                             ();
    protected :
    public :
                                            PersistenceObjectManagerDeleteXPathStringsMessage           (const vector<string> &xPathStrings);
                                            PersistenceObjectManagerDeleteXPathStringsMessage           ();
        virtual                            ~PersistenceObjectManagerDeleteXPathStringsMessage           ();

                vector<string>              getXPathStrings                                             () const;
                void                        setXPathStrings                                             (const vector<string> &xPathStrings);

    // Now the data members

    private :
        vector<string>              m_xPathStrings;

    protected :
    public :
};

class PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage : public ManagementInterfaceMessage
{
    private:
                void                        setupAttributesForSerialization                                        ();
    protected :
    public :
                                            PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage  (const vector<string> &xPathStrings);
                                            PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage  ();
        virtual                            ~PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage  ();

                vector<string>              getXPathStrings                                                        () const;
                void                        setXPathStrings                                                        (const vector<string> &xPathStrings);

                vector<UI64>                getLastUpdatedTimestamps                                               () const;
                void                        setLastUpdatedTimestamps                                               (const vector<UI64> &lastUpdatedTimestamps);

    // Now the data members

    private :
        vector<string>              m_xPathStrings;
        vector<UI64>                m_lastUpdatedTimestamps;

    protected :
    public :
};

class PersistenceObjectManagerResetXPathStringsTimestampsMessage : public ManagementInterfaceMessage
{
    private:
                void            setupAttributesForSerialization                              ();
    protected :
    public :
                                PersistenceObjectManagerResetXPathStringsTimestampsMessage   ();
        virtual                ~PersistenceObjectManagerResetXPathStringsTimestampsMessage   ();

    // Now the data members

    private :

    protected :
    public :
};

}

#endif // PERSISTENCEOBJECTMANAGERMESSAGES_H
