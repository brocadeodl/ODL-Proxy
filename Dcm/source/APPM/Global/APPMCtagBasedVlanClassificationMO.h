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

/********************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.					*
 *   All rights reserved.														*
 *   Author : Shivanand															*
 ********************************************************************************/

#ifndef APPMCTAGBASEDVLANCLASSIFICATIONMO_H
#define APPMCTAGBASEDVLANCLASSIFICATIONMO_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs {

    class APPMVlanClassificationMO : virtual public DcmLocalManagedObject {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            APPMVlanClassificationMO (WaveObjectManager *pWaveObjectManager);
            virtual ~APPMVlanClassificationMO();
            static string getClassName();
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId() const;
            void setCtagId(const UI32 &ctagId);
            UI32 getCtagId() const;

        private:
        protected:
        public:
            UI32 m_vlanId;
            UI32 m_ctagId;
    };

    class APPMCtagVlanClassificationMO : public APPMVlanClassificationMO {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            APPMCtagVlanClassificationMO (WaveObjectManager *pWaveObjectManager);
            virtual ~APPMCtagVlanClassificationMO();
            static string getClassName();
    };

    class APPMCtagVlanClassificationRemoveMO : public APPMVlanClassificationMO {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            APPMCtagVlanClassificationRemoveMO (WaveObjectManager *pWaveObjectManager);
            virtual ~APPMCtagVlanClassificationRemoveMO();
            static string getClassName();
    };

}
#endif // APPMCTAGBASEDVLANCLASSIFICATIONMO_H


