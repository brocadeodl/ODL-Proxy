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
 *   Author : Amulya Makam                                                 *
 **************************************************************************/

#ifndef NSMLOCALSHOWECMPMESSAGE_H
#define NSMLOCALSHOWECMPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmLocalShowEcmpMessage : public PrismMessage
	{
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			NsmLocalShowEcmpMessage ();
			NsmLocalShowEcmpMessage (const UI32 &cmdCode,const UI32 &rbridgeId,const bool &isGwRequest);
			virtual    ~NsmLocalShowEcmpMessage ();
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode()  const;

			void setRbridgeId(const UI32 &rbridgeId);
			UI32 getRbridgeId()  const;

			void setIsGwRequest(const bool &isGwRequest);
			bool getIsGwRequest() const;

			void getOutputBufferRef (UI32   bufferNum,UI32 &size, void *&pBuffer);
			void copyAllBuffers (const PrismMessage &prismMessage);

			// Now the data members

		private:
		protected:
		public:
			UI32  m_cmdCode;
			UI32  m_rbridgeId;
			bool  m_isGwRequest;
	};
}
#endif

