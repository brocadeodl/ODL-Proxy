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

/*
 * SflowProfileShowMessage.h
 *
 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Sep 17, 2012
 *      Author: sunniram
 */
#ifndef SFLOWPROFILESHOWMESSAGE_H
#define SFLOWPROFILESHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs {

	class SflowProfileShowMessage: public DcmManagementInterfaceMessage {
		private:
		protected:
			virtual void setupAttributesForSerialization();
		public:
			SflowProfileShowMessage();
			SflowProfileShowMessage(const string &profileName, const UI32 &samplingRate,
					const UI32 &cmdCode);
			virtual ~SflowProfileShowMessage();
			void setProfileName(const string &name);
			string getProfileName() const;
			void setCmdCode(const UI32 &cmdCode);
			UI32 getCmdCode() const;
			void setSamplingRate(const UI32 &samplingRate);
			UI32 getSamplingRate() const;

			virtual const void *getCStructureForInputs();
			virtual void loadOutputsFromCStructure(const void *pOutputCStructure);
			void getOutputBufferRef(UI32 bufferNum, UI32 &size, void *&pBuffer);

		private:
		protected:
		public:
			string m_profileName;
			UI32 m_samplingRate;
			UI32 m_cmdCode;
	};
}
#endif  //SFLOWPROFILESHOWMESSAGE_H
