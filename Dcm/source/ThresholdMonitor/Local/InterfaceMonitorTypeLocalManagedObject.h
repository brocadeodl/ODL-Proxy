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

#ifndef INTERFACEMONITORTYPELOCALMANAGEDOBJECT_H
#define INTERFACEMONITORTYPELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeStringUC.h"
#include "ThresholdMonitor/Local/InterfaceMonitorAreaLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

	class ThresholdMonitorLocalObjectManager;
	class InterfaceMonitorTypeLocalManagedObject : virtual public DcmLocalManagedObject
	{
		private:
		protected:
			virtual void setupAttributesForPersistence();
			virtual void setupAttributesForCreate();
		public:
			InterfaceMonitorTypeLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager);
			InterfaceMonitorTypeLocalManagedObject
				(ThresholdMonitorLocalObjectManager
				 *pThresholdMonitorLocalObjectManager, const string &policyName, const vector<WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> > &areaThresholdAlertMap);
			virtual    ~InterfaceMonitorTypeLocalManagedObject();
			static string getClassName();
			void setpolicyName(const string &policyName, bool userConfig);
			string getpolicyName ()  const; 
			void setAreaThresholdAlertMap(const vector<WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> > &areaThresholdAlertMap);
			vector<WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> > getAreaThresholdAlertMap() const;	 
			void addAreaThreshAlertMap(InterfaceMonitorAreaLocalManagedObject *areaMap);

			// Now the data members
		private:
		protected:
		public:
			StringUC m_policyName;
			vector<WaveManagedObjectPointer<InterfaceMonitorAreaLocalManagedObject> > m_areaThresholdAlertMap; 
				
	};
}
#endif															//INTERFACEMONITORTYPELOCALMANAGEDOBJECT_H
