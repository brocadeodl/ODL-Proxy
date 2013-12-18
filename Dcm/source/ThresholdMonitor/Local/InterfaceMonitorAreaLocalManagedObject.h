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
 *	 Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Priya Ahuja											       *
 **************************************************************************/

#ifndef INTERFACEMONITORAREALOCALMANAGEDOBJECT_H
#define INTERFACEMONITORAREALOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/AttributeBitMap.h"
#include "Framework/Attributes/AttributeEnumUC.h"

using namespace WaveNs;

namespace DcmNs
{

	class ThresholdMonitorLocalObjectManager;
	class InterfaceMonitorAreaLocalManagedObject : virtual public DcmLocalManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
			virtual void setupKeys();
		public:
			InterfaceMonitorAreaLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager);
			InterfaceMonitorAreaLocalManagedObject
				(ThresholdMonitorLocalObjectManager
				 *pThresholdMonitorLocalObjectManager,const UI32
				 &interfacetypename, const UI32 &areaname,const SI32 &highthresh,const SI32 &lowthresh,const SI32 &buffer, const BitMap &abovehighthreshaction, const BitMap &abovelowthreshaction, const BitMap &belowhighthreshaction, const BitMap &belowlowthreshaction,const UI32 &timebase);
			virtual    ~InterfaceMonitorAreaLocalManagedObject();
			static string getClassName();
			void setInterfacetypeName(const UI32 &interfacetypename, bool userConfig);
			UI32 getInterfacetypeName()  const;
			void setAreaName(const UI32 &areaname,  bool userConfig);
			UI32 getAreaName()  const;
			void setHighThresh(const SI32 &highthresh);
			SI32 getHighThresh() const;  
			void setLowThresh(const SI32 &lowthresh);
			SI32 getLowThresh() const;
			void setBuffer(const SI32 &buffer);
			SI32 getBuffer() const;
			void setAboveHighThreshAction(const BitMap &abovehighthreshaction);
			BitMap getAboveHighThreshAction() const;
			void setAboveLowThreshAction(const BitMap &abovelowthreshaction);
			BitMap getAboveLowThreshAction() const;
			void setBelowHighThreshAction(const BitMap &belowhighthreshaction);
			BitMap getBelowHighThreshAction() const;
			void setBelowLowThreshAction(const BitMap &belowlowthreshaction);
			BitMap getBelowLowThreshAction() const;
			void setTimeBase(const UI32 &timebase);
			UI32 getTimeBase()  const;

			// Now the data members
		private:
		protected:
		public:
			EnumUC m_interfacetypename;
			EnumUC m_areaname;
			SI32 m_highthresh;
			SI32 m_lowthresh;
			SI32 m_buffer;
			BitMap m_abovehighthreshaction;
			BitMap m_abovelowthreshaction;
			BitMap m_belowhighthreshaction;
			BitMap m_belowlowthreshaction;
			UI32 m_timebase;
	};
}
#endif															//INTERFACEMONITORAREALOCALMANAGEDOBJECT_H
