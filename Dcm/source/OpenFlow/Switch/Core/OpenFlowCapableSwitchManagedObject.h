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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef OPENFLOWCAPABLESWITCHMANAGEDOBJECT_H
#define OPENFLOWCAPABLESWITCHMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Types/Uri.h"

#include <string>

using namespace std;
using namespace DcmNs;

namespace OpenFlowNs
{

class OpenFlowCapableSwitchManagedObject : public DcmManagedObject
{
    private :
        void setupAttributesForPersistence ();
        void setupAttributesForCreate      ();

    protected :
    public :
                        OpenFlowCapableSwitchManagedObject (WaveObjectManager *pWaveObjectManager);
        virtual        ~OpenFlowCapableSwitchManagedObject ();

        static  string  getClassName                       ();

                string  getOpenFlowSwitchName              () const;
                void    setOpenFlowSwitchName              (const string &openFlowSwitchName);

                Uri     getOpenFlowSwitchUri               () const;
                void    setOpenFlowSwitchUri               (const Uri &openFlowSwitchUri);

    // Now the data members

    private :
        string m_openFlowSwitchName;
        Uri    m_openFlowSwitchUri;

    protected :
    public :
};

}

#endif // OPENFLOWCAPABLESWITCHMANAGEDOBJECT_H
