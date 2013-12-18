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
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#ifndef WAVECONFIGMANAGEDOBJECT_H
#define WAVECONFIGMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Persistence/WaveConfigurationAttributes.h"

using namespace WaveNs;

namespace WaveNs
{

class WaveConfigManagedObject : virtual public WaveConfigurationAttributes, public WaveManagedObject
{
    private :
                void    setupAttributesForPersistence ();
                void    setupAttributesForCreate      ();

    protected :
    public :
                        WaveConfigManagedObject (WaveObjectManager *pWaveObjectManager);
                        WaveConfigManagedObject ();
        virtual        ~WaveConfigManagedObject ();

        static  string  getClassName        ();

        void            setConfigName           (const string &configName);
        string          getConfigName           () const;

        void            setConfigValue          (const string &configValue);
        string          getConfigValue          () const;

        void            setConfigAttributeType  (const ResourceId &configAttributeType);
        ResourceId      getConfigAttributeType  () const;

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // WAVECONFIGMANAGEDOBJECT_H
