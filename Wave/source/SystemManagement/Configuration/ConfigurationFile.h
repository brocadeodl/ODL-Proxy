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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef CONFIGURATIONFILE_H
#define CONFIGURATIONFILE_H


#include "Framework/Types/Types.h"

#include <string>

using namespace std;

namespace WaveNs
{

class ConfigurationFile
{
    private :
    protected :
    public :
                        ConfigurationFile        ();
                        ConfigurationFile        (const string &configurationFilePath);
        virtual        ~ConfigurationFile        ();

        virtual void    load                     () = 0;
        virtual void    reload                   () = 0;
        virtual void    reload                   (const string &configurationFilePath) = 0;
        virtual bool    validate                 () const = 0;
        virtual void    print                    () const = 0;

                string  getConfigurationFilePath () const;
                void    setConfigurationFilePath (const string &configurationFilePath);

                bool    getIsAlreadyLoaded       () const;
                void    setIsAlreadyLoaded       (const bool &isAlreadyLoaded);

        // Now the data members

    private :
        string m_configurationFilePath;
        bool   m_isAlreadyLoaded;

    protected :
    public :
};

}

#endif // CONFIGURATIONFILE_H
