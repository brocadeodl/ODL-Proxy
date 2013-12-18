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
 *   Copyright (C) 2011-2015 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Sudha Balodia          			                                       *
 **************************************************************************/

#ifndef QOSCOSTOTAILDROPMANAGEDOBJECT_H
#define QOSCOSTOTAILDROPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class QosCosToTailDropManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            QosCosToTailDropManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            QosCosToTailDropManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager, const UI32 &cos0_threshold, const
					UI32 &cos1_threshold, const UI32 &cos2_threshold, const UI32 &cos3_threshold, const UI32 &cos4_threshold,
					const UI32 &cos5_threshold, const UI32 &cos6_threshold, const UI32 &cos7_threshold);
            virtual    ~QosCosToTailDropManagedObject ();
            static string getClassName();
			UI32 getCos0Threshold() const;
			void setCos0Threshold(const UI32 &threshold);
			UI32 getCos1Threshold() const;
			void setCos1Threshold(const UI32 &threshold);
			UI32 getCos2Threshold() const;
			void setCos2Threshold(const UI32 &threshold);
			UI32 getCos3Threshold() const;
			void setCos3Threshold(const UI32 &threshold);
			UI32 getCos4Threshold() const;
			void setCos4Threshold(const UI32 &threshold);
			UI32 getCos5Threshold() const;
			void setCos5Threshold(const UI32 &threshold);
			UI32 getCos6Threshold() const;
			void setCos6Threshold(const UI32 &threshold);
			UI32 getCos7Threshold() const;
			void setCos7Threshold(const UI32 &threshold);

// Now the data members

        private:
        protected:
        public:
            UI32  m_cos0_threshold;
            UI32  m_cos1_threshold;
            UI32  m_cos2_threshold;
            UI32  m_cos3_threshold;
            UI32  m_cos4_threshold;
            UI32  m_cos5_threshold;
            UI32  m_cos6_threshold;
            UI32  m_cos7_threshold;
    };
}
#endif                                            //QOSCosToTailDropMANAGEDOBJECT_H
