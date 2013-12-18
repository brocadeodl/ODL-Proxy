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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *	
 *   All rights reserved.                                                  *
 *   Author : Ojasvita	                                                   *
 **************************************************************************/

#ifndef NSMSTORMCONTROLINTFCONFLOCALMANAGEDOBJECT_H
#define NSMSTORMCONTROLINTFCONFLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "vcs.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmLocalObjectManager;
    class NsmStormControlIntfConfLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            NsmStormControlIntfConfLocalManagedObject (WaveObjectManager *pNsmLocalObjectManager);
            NsmStormControlIntfConfLocalManagedObject (WaveObjectManager *pNsmLocalObjectManager, 
				const UI32 &protocolType, const UI32 &rateFormat, const UI64 &bps, const UI32 &percentage, const UI32 &action);
			
            virtual    ~NsmStormControlIntfConfLocalManagedObject ();
            static string getClassName();

            void    setProtocolType(const UI32 &protocolType);
            UI32    getProtocolType()  const;
			
			void 	setRateFormat(const UI32 &rateFormat);
			UI32	getRateFormat()	const;
			
            void    setBps(const UI64 &bps);
            UI64    getBps()  const;
			
            void    setPercentage(const UI32 &percentage);
            UI32    getPercentage()  const;
			
            void    setAction(const UI32 &action);
            UI32    getAction()  const;

			void setupKeys();
			
// Now the data members

        private:
        protected:
        public:

            UI32    m_protocolType;
			UI32	m_rateFormat;
            UI64    m_bps;
            UI32    m_percentage;
            UI32    m_action;
	};
}

#endif

