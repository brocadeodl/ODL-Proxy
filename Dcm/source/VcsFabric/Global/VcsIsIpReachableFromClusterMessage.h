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

/**************************************************************************
*   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : Sudhakar Jha                                               *
***************************************************************************/

#ifndef VCSISIPREACHABLEFROMCLUSTERMESSAGE_H
#define VCSISIPREACHABLEFROMCLUSTERMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

using namespace WaveNs;

namespace DcmNs
{

	class VcsIsIpReachableFromClusterMessage : public ManagementInterfaceMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			VcsIsIpReachableFromClusterMessage ();
			VcsIsIpReachableFromClusterMessage (const string &IpAddress);
			virtual	~VcsIsIpReachableFromClusterMessage ();
			string getIpAddress()  const;

			void setIsReachableFlag(const bool IsReachable);
			bool getIsReachableFlag() const;


			void addRbridgeIdOfConnectedNodes(const UI32 rbridgeID);
			vector<UI32> getPingResponseForNode() const;

// Now the data members

		private:
		protected:
		public:
			string  m_IpAddress;
			bool m_IsReachable;
			vector<UI32> m_rbridgeIdOfIpReachableNodes;
	};
}
#endif											//VCSISIPREACHABLEFROMCLUSTERMESSAGE_H
