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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : agidwani                                                     *
 **************************************************************************/

#ifndef IGMPINTFCONFMANAGEDOBJECT_H
#define IGMPINTFCONFMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeBoolUC.h"

using namespace WaveNs;

namespace DcmNs
{

class NsmLocalObjectManager;
class IgmpIntfConfManagedObject : virtual public DcmLocalManagedObject
{
    private:
    protected:
        virtual void setupAttributesForPersistence();
        virtual void setupAttributesForCreate();
    public:
        IgmpIntfConfManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
        IgmpIntfConfManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &lmqi,const UI32 &qmrt,const bool &immediateLeave,const UI32 &queryInterval);
        virtual    ~IgmpIntfConfManagedObject ();
        static string getClassName();
        void setLmqi(const UI32 &lmqi, bool usrCfg);
        UI32 getLmqi()  const;
        void setQmrt(const UI32 &qmrt, bool usrCfg);
        UI32 getQmrt()  const;
        void setImmediateLeave(const bool &immediateLeave, bool usrCfg);
        bool getImmediateLeave()  const;
        void setQueryInterval(const UI32 &queryInterval, bool usrCfg);
        UI32 getQueryInterval()  const;
        bool hasNonDefaultConfig() const;

// Now the data members

    private:
    protected:
    public:
        UI32UC  m_lmqi;
        UI32UC  m_qmrt;
        BoolUC  m_immediateLeave;
        UI32UC  m_queryInterval;
};
}
#endif                                            //IGMPINTFCONFMANAGEDOBJECT_H
