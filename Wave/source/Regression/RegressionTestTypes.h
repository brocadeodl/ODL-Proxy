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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef REGRESSIONTESTTYPES_H
#define REGRESSIONTESTTYPES_H

namespace WaveNs
{

typedef enum
{
    REGRESSION_GET_TEST_SERVICE_ENTRIES = 0,
    REGRESSION_SET_TEST_SERVICE_STATE,
    REGRESSION_START_REGRESSION,
    REGRESSION_PREPARE_TEST_FOR_A_SERVICE,
    REGRESSION_RUN_TEST_FOR_A_SERVICE
} RegresionMessageIds;

typedef enum
{
    REGRESSION_TEST_SUCCESS = 0, // FIXME : sagar : conver these enum values to Brocade Resources.
    REGRESSION_TEST_ERROR_INVALID_TEST_SERVICE_ID
} RegressionTestObjectManagerMessageStatus;

}

#endif // REGRESSIONTESTTYPES_H
