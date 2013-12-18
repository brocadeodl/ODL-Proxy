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
 *   Author : sshaik                                                     *
 **************************************************************************/

#ifndef DiagLocalPostManagedObject_H
#define DiagLocalPostManagedObject_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Diag/Local/DiagLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class DiagLocalObjectManager;
    class DiagLocalPostManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            DiagLocalPostManagedObject (DiagLocalObjectManager *pDiagObjectManager);
            DiagLocalPostManagedObject (DiagLocalObjectManager *pDiagObjectManager,const UI32 &rbridgeId,const bool &enabled);
            virtual    ~DiagLocalPostManagedObject ();
            static string getClassName();
			void setrbridgeId(const UI32 &rbridgeId);
            UI32 getrbridgeId()  const;
            void setDiagPost(const bool &enabled);
            bool getDiagPost()  const;

// Now the data members

        private:
        protected:
        public:
			UI32  m_rbridgeId;
            bool  m_enabled;
    };
}
#endif                                            //DiagLocalPostManagedObject_H
