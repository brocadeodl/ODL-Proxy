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
 *   Author : mmohan                                                     *
 **************************************************************************/

#ifndef CDPCLIENTCONFSETMESSAGE_H
#define CDPCLIENTCONFSETMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

		class CdpClientConfSetMessage : public DcmManagementInterfaceMessage
		{
				private:
				protected:
						virtual void setupAttributesForSerialization();
				public:
						CdpClientConfSetMessage ();
						CdpClientConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const bool &protoEnable);
						virtual    ~CdpClientConfSetMessage ();
						void setMgmtFlags(const UI32 &mgmtFlags);
						UI32 getMgmtFlags()  const;
						void setCmdcode(const UI32 &cmdcode);
						UI32 getCmdcode()  const;
						void setMsgType(const UI32 &msgType);
						UI32 getMsgType()  const;
						void setProtoEnable(const bool &protoEnable);
						bool getProtoEnable()  const;
            
						virtual const void *getCStructureForInputs ();
                        virtual void loadOutputsFromCStructure (const void 
													*pOutputCStructure);

// Now the data members

				private:
				protected:
				public:
						UI32  m_mgmtFlags;
						UI32  m_cmdcode;
						UI32  m_msgType;
						bool  m_protoEnable;
		};
}
#endif //CDPCLIENTCONFSETMESSAGE_H

