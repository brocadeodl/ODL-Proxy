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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : apurva                                                     *
 **************************************************************************/


#ifndef COMPANYSERVICELOCALGROUPCODE_H
#define COMPANYSERVICELOCALGROUPCODE_H




typedef enum
{
    COMPANYSERVICE_GROUP_CODE_DEFAULT                           = 0,
    COMPANYSERVICE_GROUP_CODE_LOCALS                            = 50000,
    COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEELOCATION         = 50001,
    COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEEGROUP            = 50002,
    COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEESTOCKOPTION      = 50003,
    COMPANYSERVICE_GROUP_CODE_EMPLOYEE_EMPLOYEENAME             = 50004,
    COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPEMPLOYEE           = 50005,
    COMPANYSERVICE_GROUP_CODE_GROUPBASE_GROUPLOCATION           = 50006,
    COMPANYSERVICE_GROUP_CODE_GROUP_SUBGROUP                    = 50007,
    COMPANYSERVICE_GROUP_CODE_GROUP_GROUPNAME                   = 50008,
    COMPANYSERVICE_GROUP_CODE_GROUP_GROUPMANAGER                = 50009,
    COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPNAME             = 50010,
    COMPANYSERVICE_GROUP_CODE_SUBGROUP_SUBGROUPMANAGER          = 50011,
    COMPANYSERVICE_GROUP_CODE_STOCKOPTION_ISSTOCKOPTION         = 50012,
    COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKSHARE            = 50013,
    COMPANYSERVICE_GROUP_CODE_GROUPMANAGER_GROUPMANAGERNAME     = 50014,
    COMPANYSERVICE_GROUP_CODE_SUBGROUPMANAGER_SUBGROUPMANAGERNAME= 50015,
    COMPANYSERVICE_GROUP_CODE_STOCKOPTION_STOCKOPTIONPRESENT    =50016
}COMPANYSERVICE_GROUP_CODE_TYPES;

#endif

