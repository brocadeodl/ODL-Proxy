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
 *   Copyright (C) 2012-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#ifndef PERSISTENCELOCALOBJECTMANAGERPUSHCONFIGTOKERNELMESSAGE_H
#define PERSISTENCELOCALOBJECTMANAGERPUSHCONFIGTOKERNELMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include <string>

using namespace std;

namespace WaveNs
{

class PersistenceLocalObjectManagerPushConfigToKernelMessaage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        PersistenceLocalObjectManagerPushConfigToKernelMessaage (const string &globalConfigs);
                        PersistenceLocalObjectManagerPushConfigToKernelMessaage ();
        virtual        ~PersistenceLocalObjectManagerPushConfigToKernelMessaage ();

                void    setGlobalConfigs                                        (const string &globalConfigs);
                string  getGlobalConfigs                                        () const;

    // Now the data members

    private :
        string m_globalConfigs;

    protected :
    public :
};

}

#endif // PERSISTENCELOCALOBJECTMANAGERPUSHCONFIGTOKERNELMESSAGE_H
