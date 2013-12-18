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

#include "Framework/Core/FrameworkSequenceGeneratorFactory.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"
#include "Framework/Core/FrameworkSequenceGeneratorType0.h"
#include "Framework/Core/FrameworkSequenceGeneratorType1.h"
#include "Framework/Core/FrameworkSequenceGeneratorType2.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

static FrameworkSequenceGeneratorType0 *s_pType0 = NULL;
static FrameworkSequenceGeneratorType1 *s_pType1 = NULL;
static FrameworkSequenceGeneratorType2 *s_pType2 = NULL;

FrameworkSequenceGenerator *FrameworkSequenceGeneratorFactory::obtainFrameworkSequenceGenerator (FrameworkSequenceType frameworkSequenceType)
{
    FrameworkSequenceGenerator *pTemp = NULL;

    switch (frameworkSequenceType)
    {
        case FRAMEWORK_SEQUENCE_TYPE0 :
            if (NULL == s_pType0)
            {
                s_pType0 = new FrameworkSequenceGeneratorType0 ();

                prismAssert (NULL != s_pType0, __FILE__, __LINE__);
            }

            pTemp = s_pType0;
            break;

        case FRAMEWORK_SEQUENCE_TYPE1 :
            if (NULL == s_pType1)
            {
                s_pType1 = new FrameworkSequenceGeneratorType1 ();

                prismAssert (NULL != s_pType1, __FILE__, __LINE__);
            }

            pTemp = s_pType1;
            break;

        case FRAMEWORK_SEQUENCE_TYPE2 :
            if (NULL == s_pType2)
            {
                s_pType2 = new FrameworkSequenceGeneratorType2 ();

                prismAssert (NULL != s_pType2, __FILE__, __LINE__);
            }

            pTemp = s_pType2;
            break;

        default :
            prismAssert (false, __FILE__, __LINE__);
    };

    return (pTemp);
}

}
