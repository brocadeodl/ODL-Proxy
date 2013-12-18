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

#ifndef PERSISTENCEOBJECTMANAGERGETALLCLASSNAMESMESSAGE_H
#define PERSISTENCEOBJECTMANAGERGETALLCLASSNAMESMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

#include <string>
#include <vector>
#include <set>

using namespace std;

namespace WaveNs
{

class PersistenceObjectManagerGetAllClassNamesMessage : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization  ();
                void computeCpnfigurationSegmentNames ();

    protected :
    public :
                      PersistenceObjectManagerGetAllClassNamesMessage ();
        virtual      ~PersistenceObjectManagerGetAllClassNamesMessage ();

                void  getAllClassNames                                (vector<string> &allClassNames) const;
                void  getAllClassNames                                (set<string> &allClassNames) const;
                void  setAllClassNames                                (const vector<string> &allClassNames);
                void  setAllClassNames                                (const set<string> &allClassNames);

                void  getConfigurationSegmentNames                    (vector<string> &configurationSegmentNames) const;
                void  setConfigurationSegmentNames                    (const vector<string> &configurationSegmentNames);

                void  getClassNamesForConfigurationSegmentNames       (vector<string> &classNamesForConfigurationSegmentNames) const;
                void  setClassNamesForConfigurationSegmentNames       (const vector<string> &classNamesForConfigurationSegmentNames);

                void  computeConfigurationSegmentNames                ();

    // Now the data members

    private :
        vector<string> m_allClassNames;
        vector<string> m_configurationSegmentNames;
        vector<string> m_classNamesForconfigurationSegmentNames;

    protected :
    public :
};

}

#endif // PERSISTENCEOBJECTMANAGERGETALLCLASSNAMESMESSAGE_H
