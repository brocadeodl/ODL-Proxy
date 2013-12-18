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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef CEEGLOBALCREATECEEREMAPMESSAGE_H
#define CEEGLOBALCREATECEEREMAPMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class CeeGlobalCreateCeeRemapMessage : public ManagementInterfaceMessage
    {
		private:
		protected:
            virtual void setupAttributesForSerialization();
        public:
            CeeGlobalCreateCeeRemapMessage ();
			CeeGlobalCreateCeeRemapMessage (const string &ceeMapName, const SI32 remapPriority, const bool remapFabricP);
			virtual		~CeeGlobalCreateCeeRemapMessage();
			
			void setCeeMapName(const string &ceeMapName);
			string	getCeeMapName()		const;
			void setRemapPriority(const SI32 remapPriority);
			SI32 getRemapPriority()		const;
			void setRemapFabricP(const bool remapFabricP);
			bool getRemapFabricP()		const;

// Now the data members

        private:
        protected:
        public:
			string	m_ceeMapName;
			SI32	m_remapPriority;
			bool	m_remapFabricP;
	};
}
#endif
