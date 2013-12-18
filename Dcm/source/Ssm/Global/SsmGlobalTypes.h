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
 *   Author : pasu                                                     *
 **************************************************************************/

#ifndef SSMGLOBALTYPESH_H
#define SSMGLOBALTYPESH_H

#define SSM_PLUGIN_GLOBAL_VERSION  0

namespace DcmNs
{

    typedef enum
    {
        SSMGLOBALADDMACACLPOLICY = 0 ,
        SSMGLOBALREMMACACLPOLICY,
        SSMGLOBALASSOCRULETOPOLICY,
        SSMGLOBALGETMACACLPOLICY,
        SSMGLOBALASSOCPOLICYTOINTERFACE,
        SSMGLOBALRESEQMACACLPOLICY,
		/* IP ACL Policy Related */
        SSMGLOBALADDIPACLPOLICY,
        SSMGLOBALREMIPACLPOLICY,
        SSMGLOBALASSOCIPRULETOPOLICY,
		SSMGLOBALRESEQIPACLPOLICY,
		SSMGLOBALGETIPACLPOLICY,
		SSMGLOBALIP6ACLPOLICY,
		SSMGLOBALASSOCIP6RULETOPOLICY
    }SsmGlobal;

	typedef enum
	{
		IP6_ADD_OPER,
		IP6_DELETE_OPER
	} SsmGlobalMessageOperationType;

	typedef enum
	{
		STANDARD_IP6_ACL = 0,
		EXTENDED_IP6_ACL
	} Ip6AclType;

}
	
#endif                                            //SSMGLOBALTYPESH_H
