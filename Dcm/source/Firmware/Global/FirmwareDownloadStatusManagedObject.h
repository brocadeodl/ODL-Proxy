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
*	CPP file that has the basic class definition for storing the
*	firmware sanity results as MO.
*/

#ifndef FIRMWAREDOWNLOADSTATUSMANAGEDOBJECT_H
#define FIRMWAREDOWNLOADSTATUSMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{
	class FirmwareObjectManager;
	class FirmwareDownloadStatusManagedObject : virtual public DcmManagedObject
	{
		private:
		protected:
			virtual	void	setupAttributesForPersistence();
			virtual	void	setupAttributesForCreate();
		public:
			FirmwareDownloadStatusManagedObject(FirmwareObjectManager *pFirmwareObjectManager);
			virtual	~FirmwareDownloadStatusManagedObject();
			static	string getClassName();
			void	setRbridge_Id_Local(const UI32 &rbridge_id);
			UI32	getRbridge_Id_Local()  const;
			void	setStatus(const int &status);
			int		getStatus() const;
			void	setMesg(const string &mesg);
			string	getMesg() const;
			void	setRbridge_Id_Principal(const UI32 &rbridge_id);
			UI32	getRbridge_Id_Principal()  const;
			void	setTime(const string &time);
			string	getTime() const;

		private:
		protected:
		public:
			UI32	m_rbridge_id_local;
			int		m_status;
			string	m_mesg;
			UI32	m_rbridge_id_principal;
			string	m_time;
	};
}
#endif
