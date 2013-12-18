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

#ifndef SPANLOCALSHOWMESSAGE_H
#define SPANLOCALSHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SpanLocalShowMessage : public PrismMessage
    {
          private:
          protected:
	          virtual void setupAttributesForSerialization();
	      public:
			  SpanLocalShowMessage ();
			  SpanLocalShowMessage (const UI32 &session_id,const UI32 &messageType,const UI32 &cmdCode,bool &isGwRequest);
			  virtual    ~SpanLocalShowMessage();
			
			  void setSession_id(const UI32 &session_id);
			  UI32 getSession_id()  const;
			
			  void setMessageType(const UI32 &messageType);
			  UI32 getMessageType()  const;
	
			  void setCmdCode(const UI32 &cmdCode);
			  UI32 getCmdCode()  const;
							
			  void setIsGwRequest (bool &isGwRequest);
			  UI32 getIsGwRequest() const;
			  
			  void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);
			  void copyAllBuffers (const PrismMessage &prismMessage);
			  
			 
			  
// Now the data members
		  private:
		  protected:
		  public:
			  UI32  m_session_id;
		      UI32  m_messageType;
			  UI32  m_cmdCode;
			  UI32  m_isGwRequest;

	};
}
#endif                                            //SPANLOCALSHOWMESSAGE_H					  
																	  
