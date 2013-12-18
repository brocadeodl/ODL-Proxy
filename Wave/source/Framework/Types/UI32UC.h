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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef UI32UC_H
#define UI32UC_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class UI32UC
{
    private:
    protected:
    public:
                            UI32UC                     ();
                            UI32UC                     (const UI32 &ui32);
                            UI32UC                     (const UI32 &ui32, const bool &isUserConfigured);
                            UI32UC                     (const UI32UC &ui32UC);
                                                       
                void        setUI32Value               (const UI32 &ui32);
                UI32        getUI32Value               () const;
                                                       
                void        setIsUserConfigured        (const bool &isUserConfigured);
                bool        getIsUserConfigured        () const;
                            
                string      toString                   ();
                void        fromString                 (const string &ui32UCInStringFormat);
                void        getPlainString             (string &valueString) const;
                ResourceId  loadFromPlainString        (const string &pUI32UCInPlainStringFormat);
                            
                bool        operator == (const UI32UC &ui32UC)  const ;
                bool        operator != (const UI32UC &ui32UC)  const ;
                UI32UC     &operator  = (const UI32UC &ui32UC);


        // Now the data members

    private:
                UI32    m_pUI32;
                bool    m_pIsUserConfigured;
};

typedef UI32UC  EnumUC;

}

#endif //UI32UC_H
