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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef PIMINTFCFGLOCALMANAGEDOBJECT_H
#define PIMINTFCFGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeBoolUC.h"
#include "Framework/Attributes/AttributeStringUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class PimIntfCfgLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PimIntfCfgLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~PimIntfCfgLocalManagedObject ();
            static string getClassName();
            void setEnabled(const bool &enabled);
            bool getEnabled()  const;
            void setDrPriority(const UI32 &drPriority, bool userConfig = false);
            UI32 getDrPriority()  const;
			/* Changing the type as we dont accept acl name as of now */
#if 0
            void setMultBoundaryAcl(const string &multBoundaryAcl, bool userConfig = false);
            string getMultBoundaryAcl()  const;
#endif
			void setMultBoundaryAcl(const bool &multBoundaryAcl, bool userConfig = false);
			bool getMultBoundaryAcl()  const;

            void setPimBorder(const bool &pimBorder, bool userConfig = false);
            bool getPimBorder()  const;
            void setTtlThold(const UI32 &ttlThold, bool userConfig = false);
            UI32 getTtlThold()  const;

// Now the data members

        private:
        protected:
        public:
            bool  m_enabled;
            UI32UC  m_drPriority;
			/* Changing the type as we dont accept acl name as of now */
            //StringUC  m_multBoundaryAcl;
			BoolUC m_multBoundaryAcl;
            BoolUC  m_pimBorder;
            UI32UC m_ttlThold;
    };
}
#endif                                            //PIMINTFCFGLOCALMANAGEDOBJECT_H
