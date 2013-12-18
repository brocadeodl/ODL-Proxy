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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#ifndef IGMPSVLANINTFCONFMANAGEDOBJECT_H
#define IGMPSVLANINTFCONFMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class IgmpsVlanIntfConfManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            IgmpsVlanIntfConfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            IgmpsVlanIntfConfManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &lmqi,const UI32 &qmrt,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout, const UI32 &vlanId);
            virtual    ~IgmpsVlanIntfConfManagedObject ();
            static string getClassName();
            void setLmqi(const UI32 &lmqi);
            UI32 getLmqi()  const;
            void setQmrt(const UI32 &qmrt);
            UI32 getQmrt()  const;
            void setEnable(const bool &enable);
            bool getEnable()  const;
            void setFastLeave(const bool &fastLeave);
            bool getFastLeave()  const;
            void setQueryInterval(const UI32 &queryInterval);
            UI32 getQueryInterval()  const;
            void setQuerierEnable(const bool &querierEnable);
            bool getQuerierEnable()  const;
            void setMrouterTimeout(const UI32 &mrouterTimeout);
            UI32 getMrouterTimeout()  const;
            void setVlanId(const UI32 &vlanId);
			UI32 getVlanId()  const;

            virtual string                        customConfigurationDisplay                    ( WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext);

// Now the data members

        private:
        protected:
        public:
            UI32  m_lmqi;
            UI32  m_qmrt;
            bool  m_enable;
            bool  m_fastLeave;
            UI32  m_queryInterval;
            bool  m_querierEnable;
            UI32  m_mrouterTimeout;
            UI32  m_vlanId;
    };
}
#endif                                            //IGMPSVLANINTFCONFMANAGEDOBJECT_H
