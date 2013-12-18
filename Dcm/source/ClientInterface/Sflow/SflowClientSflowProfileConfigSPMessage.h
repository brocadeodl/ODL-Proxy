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
 * SflowClientSflowProfileConfigSPMessage.h
 *
 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Sep 17, 2012
 *      Author: sunniram
 */

#ifndef SFLOWCLIENTSFLOWPROFILECONFIGSPMESSAGE_H_
#define SFLOWCLIENTSFLOWPROFILECONFIGSPMESSAGE_H_

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs {

	class SflowClientSflowProfileConfigSPMessage: public DcmManagementInterfaceMessage {
	private:
	protected:
		virtual void setupAttributesForSerialization();
	public:
		SflowClientSflowProfileConfigSPMessage();
		SflowClientSflowProfileConfigSPMessage(const string &profileName,
				const UI32 &sampleRate, const UI32 &opCode);
		virtual ~SflowClientSflowProfileConfigSPMessage();
		void setProfileName(const string &profileName);
		string getProfileName() const;
		void setSamplingRate(const UI32 &sampleRate);
		UI32 getSamplingRate() const;
		void setOpCode(const UI32 &opCode);
		UI32 getOpCode() const;
		void dumpMessageToBackend();
		virtual const void *getCStructureForInputs();
		virtual void loadOutputsFromCStructure(const void *pOutputCStructure);

	private:
	protected:
	public:
		string m_name;
		UI32 m_samplingRate;
		UI32 m_opCode;
	};
}

#endif /* SFLOWCLIENTSFLOWPROFILECONFIGSPMESSAGE_H_ */
