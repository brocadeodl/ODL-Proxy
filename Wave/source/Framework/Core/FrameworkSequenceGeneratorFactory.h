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

#ifndef FRAMEWORKSEQUENCEGENERATORFACTORY_H
#define FRAMEWORKSEQUENCEGENERATORFACTORY_H

namespace WaveNs
{

typedef enum
{
    FRAMEWORK_SEQUENCE_TYPE0 = 0,
    FRAMEWORK_SEQUENCE_TYPE1,
    FRAMEWORK_SEQUENCE_TYPE2
} FrameworkSequenceType;

class FrameworkSequenceGenerator;

class FrameworkSequenceGeneratorFactory
{
    private :
    protected :
    public :
        static FrameworkSequenceGenerator *obtainFrameworkSequenceGenerator (FrameworkSequenceType framewokSequenceType);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // FRAMEWORKSEQUENCEGENERATORFACTORY_H
