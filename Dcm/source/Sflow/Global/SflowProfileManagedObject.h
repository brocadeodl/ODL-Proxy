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

/*
 * SflowProfileManagedObject.h
 *
 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Sep 17, 2012
 *      Author: sunniram
 */

#ifndef SFLOWPROFILEMANAGEDOBJECT_H_
#define SFLOWPROFILEMANAGEDOBJECT_H_


#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs {


class SflowGlobalObjectManager;
class SflowProfileManagedObject : virtual public DcmManagedObject
{
    private:
    protected:
        virtual void setupAttributesForPersistence();
        virtual void setupAttributesForCreate();
        virtual void setupKeys();
    public:
        SflowProfileManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager);
        SflowProfileManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager,const string &name,const UI32 &samplingRate);
        virtual    ~SflowProfileManagedObject ();
        static string getClassName();

        void setProfileName(const string &name);
        string getProfileName()  const;
        void setSamplingRate(const UI32 &samplingRate);
        UI32 getSamplingRate()  const;

    private:
    protected:
    public:
        string  m_name;
        UI32  m_samplingRate;

};

}

#endif /* SFLOWPROFILEMANAGEDOBJECT_H_ */
