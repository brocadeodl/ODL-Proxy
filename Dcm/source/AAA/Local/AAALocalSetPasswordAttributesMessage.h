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
 *   Author : ckunduru                                                     *
 **************************************************************************/

#ifndef AAALOCALSETPASSWORDATTRIBUTESMESSAGE_H
#define AAALOCALSETPASSWORDATTRIBUTESMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalSetPasswordAttributesMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAALocalSetPasswordAttributesMessage ();
	AAALocalSetPasswordAttributesMessage (const SI32 &minlength,const SI32 &maxretry,const SI32 &uppercase,const SI32 &lowercase,const SI32 &numerics,const SI32 &splchars, const SI32 &adminlockout);
	virtual    ~AAALocalSetPasswordAttributesMessage ();
	void setMinlength(const SI32 &minlength);
	SI32 getMinlength()  const;
	void setMaxretry(const SI32 &maxretry);
	SI32 getMaxretry()  const;
	void setUppercase(const SI32 &uppercase);
	SI32 getUppercase()  const;
	void setLowercase(const SI32 &lowercase);
	SI32 getLowercase()  const;
	void setNumerics(const SI32 &numerics);
	SI32 getNumerics()  const;
	void setSplchars(const SI32 &splchars);
	SI32 getSplchars()  const;
	void setAdminlockout(const SI32 &adminlockout);
	SI32 getAdminlockout() const;

  /* Data members */
  private:
	SI32  m_minlength;
	SI32  m_maxretry;
	SI32  m_uppercase;
	SI32  m_lowercase;
	SI32  m_numerics;
	SI32  m_splchars;
	SI32  m_adminlockout;
};

}
#endif                                            //AAALOCALSETPASSWORDATTRIBUTESMESSAGE_H
