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
 * This file serves as a placeholder to satisfy compilation of the plugin infrastructure
 * outside of the "normal" make process.
 */

#ifndef __FKIO_GETID_H__
#define __FKIO_GETID_H__

#define switchSetPortTuple(fc_index, user_blade, user_port, phy_blade, phy_port, type, display_type, subport) \
			fsetPortTuple(FabOS_SW_FD, fc_index, user_blade, user_port, phy_blade, phy_port, type, display_type)
#define switchSetPortTupleRemote(fc_index, domain, user_blade, user_port, type, display_type, qsfp_idx) \
			fsetPortTupleRemote(FabOS_SW_FD, fc_index, domain, user_blade, user_port, type, display_type, qsfp_idx)

using namespace WaveNs;

namespace DcmNs
{

#ifdef __cplusplus
extern "C"
{
#endif

extern inline int
fsetPortTuple(int sw_handle, int fc_index, int user_blade, int user_port, int phy_blade, int phy_port, int type, int display_type)
{
	sw_handle = sw_handle;
	fc_index = fc_index;
	user_blade = user_blade;
	user_port = user_port;
	phy_blade = phy_blade;
	phy_port = phy_port;
	type = type;
	display_type = display_type;

	return (-1);
}

extern inline int
fsetPortTupleRemote(int sw_handle, int fc_index, int domain, int user_blade, int user_port, int type, int display_type, int qsfp_idx)
{
	sw_handle = sw_handle;
	fc_index = fc_index;
	domain = domain;
	user_blade = user_blade;
	user_port = user_port;
	type = type;
	display_type = display_type;
        qsfp_idx = qsfp_idx;

	return (-1);
}

#ifdef __cplusplus
}
#endif

}  // end namespace DcmNs

#endif // __FKIO_GETID_H__
