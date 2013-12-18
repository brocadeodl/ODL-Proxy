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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef CONFIGURATIONMANAGEDOBJECT_H
#define CONFIGURATIONMANAGEDOBJECT_H

#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Types/DateTime.h"

#include <vector>

using namespace WaveNs;

namespace WaveNs
{

class ConfigurationManagedObject : public WaveManagedObject
{
    private :
        virtual void             setupAttributesForPersistence ();
        virtual void             setupAttributesForCreate      ();

    protected :
    public :
                                 ConfigurationManagedObject   (WaveObjectManager *pWaveObjectManager);
        virtual                 ~ConfigurationManagedObject   ();

        static string            getClassName               ();

               UI64              getConfigurationNumber      () const;
               DateTime          getConfigurationTime       () const;
               DateTime          getLatestUpdateTime        () const;

               void              setConfigurationNumber     (const UI64 &configurationNumber);
               void              setConfigurationTime       (const DateTime &configurationTime);
               void              setLatestUpdateTime        (const DateTime &latestUpdateTime);
               void              resetConfigurationManagedObject ();
    // Now the data members

    private :
               UI64              m_configurationNumber;
               DateTime          m_configurationTime;
               DateTime          m_latestUpdateTime;

    protected :
    public :
};

class XPathStringManagedObject : public WaveManagedObject
{
    private :
        virtual void                setupAttributesForPersistence       ();
        virtual void                setupAttributesForCreate            ();
        virtual void                setupKeys                           ();

    protected :
    public :
                                    XPathStringManagedObject            (WaveObjectManager *pWaveObjectManager);
        virtual                    ~XPathStringManagedObject            ();

        static  string              getClassName                        ();

                string              getXPathString                      () const;
                void                setXPathString                      (const string &xPathString);

    // Now the  data members

    private :
                string              m_xPathString;

    protected :
    public :
};

}

#endif //CONFIGURATIONMANAGEDOBJECT_H

