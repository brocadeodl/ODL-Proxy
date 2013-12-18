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

/* *************************************************************************
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 ***************************************************************************/

#ifndef FIRMWARETOOLKIT_H
#define FIRMWARETOOLKIT_H

#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveObjectManager.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{

#define	NA_STS_COMPLETED			0
#define	NA_STS_IN_PROGRESS			1
#define	NA_STS_DOWNLOADED			2
#define	NA_STS_FAILURE				3
#define	NA_STS_NOT_REQUIRED			4 /*Same firmware version*/
#define	NA_STS_DISABLED				6
#define	NA_STS_PRIN_VER_DFIFF		7 /*principle version is different*/
#define	NA_STS_SANITY_FAILED		8
#define NA_STS_START_SUCCESS		9

class FirmwareToolKit
{
	private:
	protected:
	public:
		//static ResourceId setNodeAutoUpgradeConfigData(NodeAutoUpgradeConfiguration	*pNodeAutoUpgradeConfigData);
		static void getNodeAutoUpgradeConfigData(NodeAutoUpgradeConfiguration
						*pNodeAutoUpgradeConfigData);
		static void checkPrincipalVersionVersusConfiguredVersion(NodeAutoUpgradeConfiguration
						*pNodeAutoUpgradeConfigData);
		static int getUpgradeStatus();
};

}

#endif // FIRMWARETOOLKIT_H
