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
 *   Author : yugant Chandrakar                                            *
 **************************************************************************/

#ifndef SFPGLOBALSHOWMESSAGE_H
#define SFPGLOBALSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
//#include "Ssm/Global/SsmGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{
	class SfpGlobalShowMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			SfpGlobalShowMessage ();
			SfpGlobalShowMessage (const UI32 &areaName, const string &ifName);
			virtual ~SfpGlobalShowMessage();
			void setAreaName (const UI32 &areaName);
			UI32 getAreaName () const;
			void setIfName (const string &ifName);
			string getIfName () const;
			void setLocationId (const UI32 &locationId);
			UI32 getLocationId () const;
			void setIsGwRequest (const bool &IsGwRequest);
			bool getIsGwRequest () const;

			void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
			void copyAllBuffers (const PrismMessage &prismMessage);

		private:
		protected:
		public:
			UI32 m_areaName;
			string m_ifName;
			UI32 m_locationId;
			bool m_IsGwRequest;
	};
}

#endif
