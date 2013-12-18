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


#ifndef COMPANYSERVICEGLOBALGROUPCODE_H
#define COMPANYSERVICEGLOBALGROUPCODE_H




typedef enum
{
    COMPANYSERVICE_GROUP_CODE_DEFAULT                           = 0,
    COMPANYSERVICE_GROUP_CODE_GLOBALS                           = 80000,
    COMPANYSERVICE_GROUP_CODE_CITY_CITYNAME                     = 80001,
    COMPANYSERVICE_GROUP_CODE_CITY_CITYCODE                     = 80002,
    COMPANYSERVICE_GROUP_CODE_CITY_POPULATION                   = 80003,
    COMPANYSERVICE_GROUP_CODE_CITY_ISMETRO                      = 80004,
    COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESS              = 80005,
    COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV4ADDRESSNETWORKMASK   = 80006,
    COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESS              = 80007,
    COMPANYSERVICE_GROUP_CODE_CITY_CITYIPV6ADDRESSNETWORKMASK   = 80008,
    COMPANYSERVICE_GROUP_CODE_CITY_CITYIPVXADDRESS              = 80009,
    COMPANYSERVICE_GROUP_CODE_COMPANY_COMPANYNAME               = 80010,
    COMPANYSERVICE_GROUP_CODE_COMPANY_EMPLOYEE                  = 80011,
    COMPANYSERVICE_GROUP_CODE_COMPANY_LOCATION                  = 80012,
    COMPANYSERVICE_GROUP_CODE_CHOICE_REGION                     = 80013,
    COMPANYSERVICE_GROUP_CODE_REGION_EMEA                       = 80014,
    COMPANYSERVICE_GROUP_CODE_REGION_ASIAPAC                    = 80015,
    COMPANYSERVICE_GROUP_CODE_REGION_AMERICAS                   = 80016,
    COMPANYSERVICE_GROUP_CODE_COMPANY_GROUP                     = 80017,
    COMPANYSERVICE_GROUP_CODE_COMPANY_STOCKEXCHANGE             = 80018,
    COMPANYSERVICE_GROUP_CODE_COMPANY_BOARDOFDIRECTOR           = 80019,
    COMPANYSERVICE_GROUP_CODE_STOCKEXCHANGE_STOCKEXCHANGENAME   = 80020,
    COMPANYSERVICE_GROUP_CODE_STOCKEXCHANGE_STOCKOPTION         = 80021,
    COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORNAME= 80022,
    COMPANYSERVICE_GROUP_CODE_BOARDOFDIRECTOR_BOARDOFDIRECTORLOCATION= 80023
}COMPANYSERVICE_GROUP_CODE_TYPES;

#endif

