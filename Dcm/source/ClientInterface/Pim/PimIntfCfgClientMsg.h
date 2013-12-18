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
 *   Author : Rahul Jain                                                   *
 ***************************************************************************/

#ifndef _PIMINTFCFGCLIENTMSG_H_
#define _PIMINTFCFGCLIENTMSG_H_

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Nsm/Local/Pim/PimIntfCfgLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class PimIntfCfgClientMsg : public DcmManagementInterfaceMessage
{
    /* Function declarations */
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        PimIntfCfgClientMsg();
        virtual ~PimIntfCfgClientMsg();
        
        void setCmdCode(const UI8 &cmdCode);
        void setIntfType(const UI8 &intfType);
        void setIntfName(const string &intfName);
        void setEnable(const bool &enable);
        void setDrPriority(const UI32 &drPriority);
		/* commenting : Changing the type as we are not accepting acl name now */
#if 0
        void setMultBoundaryAcl(const string &multBoundaryAcl);
		string getMultBoundaryAcl()  const;
#endif
		void setMultBoundaryAcl(const bool &multBoundaryAcl);
		bool getMultBoundaryAcl()  const;

        void setPimBorder(const bool &pimBorder);
        void setTtlThold(const UI32 &ttlThold);
       
        UI8 getCmdCode()  const;
        UI8 getIntfType()  const;
        string getIntfName()  const;
        bool getEnable()  const;
        UI32 getDrPriority()  const;
        bool getPimBorder()  const;
        UI32 getTtlThold() const;
        
        virtual const void *getCStructureForInputs ();
        virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
        
    /* Data member declarations */
    private:
    protected:
    public:
        UI8 m_cmdCode;
        UI8 m_intfType;
        string m_intfName;
        bool m_enable;
        UI32 m_drPriority;
		/* commenting : Changing the type as we are not accepting acl name now*/
        //string m_multBoundaryAcl;
		bool m_multBoundaryAcl;
        bool m_pimBorder;
        UI32 m_ttlThold;
};
} /* namespace */
#endif /* _PIMPHYINTFCFGCLIENTMSG_H_ */
