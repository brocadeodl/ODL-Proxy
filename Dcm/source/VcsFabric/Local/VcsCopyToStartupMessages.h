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
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef VCSCOPYTOSTARTUPMESSAGES_H
#define VCSCOPYTOSTARTUPMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsCopyRunningToStartupMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsCopyRunningToStartupMessage ();
    virtual        ~VcsCopyRunningToStartupMessage ();
	private:

    // Now the data members

    private :
    protected :
    public :
};

class VcsCopyDefaultToStartupMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsCopyDefaultToStartupMessage ();
    virtual        ~VcsCopyDefaultToStartupMessage ();
	private:

    // Now the data members

    private :
    protected :
    public :
};

class VcsCopyToStartupMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsCopyToStartupMessage ();
                    VcsCopyToStartupMessage (const string &path);
    virtual        ~VcsCopyToStartupMessage ();
            void    setPath (const string &path);
            string  getPath () const;
	private:

    // Now the data members

    private :
        string m_path;
    protected :
    public :
};

class VcsCopyToStartupAllowedMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsCopyToStartupAllowedMessage ();
    virtual        ~VcsCopyToStartupAllowedMessage ();
            void    setIsAllowed (const bool isAllowed);
            bool    getIsAllowed () const;
	private:

    // Now the data members

    private :
        bool    m_isAllowed;
    protected :
    public :
};

}

#endif // VCSCOPYTOSTARTUPMESSAGES_H

