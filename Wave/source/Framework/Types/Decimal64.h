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
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#ifndef DECIMAL64_H
#define DECIMAL64_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class Decimal64
{

    private:
    protected:
    public:
                         Decimal64             ();
                         Decimal64             (const string &decimal64InStringFormat);
                         Decimal64             (const Decimal64 &decimal64);
                        ~Decimal64             ();
                                               
        string           toString              ()                                             const;
        void             fromString            (const string &decimal64InStringFormat);
        ResourceId       loadFromPlainString   (const string &decimal64InStringFormat);
                                               
        bool             operator ==           (const Decimal64 &decimal64)                   const;
        bool             operator !=           (const Decimal64 &decimal64)                   const;
        Decimal64       &operator =            (const Decimal64 &decimal64);
                                               
        SI64             getVal                ()                                             const;
        UI8              getFractionDigits     ()                                             const;   // indicates where to put '.' in val above
        void             setAllValues          (SI64 val, UI8 fractiondigits);

    // Now the data members
    private:
        SI64        m_val;
        UI8         m_fractionDigits;   // indicates where to put '.' in val above
                                        // e.g. 24957.54 implies val=2495754 & fraction=2
    protected:
    public:
};

}

#endif //DECIMAL64_H
