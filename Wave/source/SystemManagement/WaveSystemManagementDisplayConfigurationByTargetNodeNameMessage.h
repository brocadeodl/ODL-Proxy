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

#ifndef WAVESYSTEMMANAGEMENTDISPLAYCONFIGURATIONBYTARGETNODENAMEMESSAGE_H
#define WAVESYSTEMMANAGEMENTDISPLAYCONFIGURATIONBYTARGETNODENAMEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class WaveSystemManagementDisplayConfigurationByTargetNodeNameMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        WaveSystemManagementDisplayConfigurationByTargetNodeNameMessage ();
                        WaveSystemManagementDisplayConfigurationByTargetNodeNameMessage (const string &targetNodeName);
                        WaveSystemManagementDisplayConfigurationByTargetNodeNameMessage (const string &targetNodeName, const string &schemaName);
        virtual        ~WaveSystemManagementDisplayConfigurationByTargetNodeNameMessage ();

                string  getTargetNodeName                                               () const;
                void    setTargetNodeName                                               (const string &targetNodeName);

                string  getSchemaName                                                   () const;
                void    setSchemaName                                                   (const string &schemaName);

    // Now the Data Members

    private :
        string m_targetNodeName;
        string m_schemaName;

    protected :
    public :
};

}

#endif // WAVESYSTEMMANAGEMENTDISPLAYCONFIGURATIONBYTARGETNODENAMEMESSAGE_H