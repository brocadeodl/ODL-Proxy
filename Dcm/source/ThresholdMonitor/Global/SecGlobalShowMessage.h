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
  *    Copyright (C) 2005-2011 Brocade Communications Systems, Inc.        *
  *   All rights reserved.                                                 *
  *   Author : Ayush Jaiswal	                                           *
  **************************************************************************/
#ifndef  SECGLOBALSHOWMESSAGE_H
#define  SECGLOBALSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{
	class SecGlobalShowMessage  : public PrismMessage
    {
        private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			SecGlobalShowMessage ();
			SecGlobalShowMessage  (const UI32 &areaName);
			virtual ~SecGlobalShowMessage ();
			void setAreaName(const UI32 &areaName);
			UI32 getAreaName() const;
			void setSendToAllNodes(const bool &sendToAllNodes);
			bool getSendToAllNodes() const;
			void setLocationId(const UI32 &locationId);
			UI32 getLocationId() const;
			void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
			void copyAllBuffers (const PrismMessage &prismMessage);
		private:
		protected:
		public:
			UI32 m_areaName;
			bool m_sendToAllNodes;
			UI32 m_locationId;
	};
}
#endif 													//SECGLOBALSHOWMESSAGE_H
	
