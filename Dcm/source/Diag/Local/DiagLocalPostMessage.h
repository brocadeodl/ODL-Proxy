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
 *   Author : Subhani Shaik                                                *
 **************************************************************************/

#ifndef DIAGLOCALPOSTMESSAGE_H
#define DIAGLOCALPOSTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include "Diag/Local/DiagLocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class DiagLocalPostMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	DiagLocalPostMessage ();
	DiagLocalPostMessage (const UI32 &rbridgeId,const bool &enablePost);
	virtual    ~DiagLocalPostMessage ();
	void setrbridgeId(const UI32 &rbridgeId);
    UI32 getrbridgeId()  const;
	void setDiagPost(const bool &enablePost);
	bool getDiagPost()  const;

  /* Data Members */
  private:
	UI32  m_rbridgeId;
	bool m_enable;
};

}
#endif                                            //DIAGLOCALPOSTMESSAGE_H
