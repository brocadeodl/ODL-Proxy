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
 *   Author : Mandar Datar                                                 *
 *   Note   : Copied from Framework/Types/UI32UC.h                         *
 ***************************************************************************/

#ifndef UI64UC_H
#define UI64UC_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class UI64UC
{
    private:
    protected:
    public:
                            UI64UC                     ();
                            UI64UC                     (const UI64 &ui64);
                            UI64UC                     (const UI64 &ui64, const bool &isUserConfigured);
                            UI64UC                     (const UI64UC &ui64UC);
                                                       
                void        setUI64Value               (const UI64 &ui64);
                UI64        getUI64Value               () const;
                                                       
                void        setIsUserConfigured        (const bool &isUserConfigured);
                bool        getIsUserConfigured        () const;
                            
                string      toString                   ();
                void        fromString                 (const string &ui64UCInStringFormat);
                void        getPlainString             (string &valueString) const;
                ResourceId  loadFromPlainString        (const string &pUI64UCInPlainStringFormat);
                            
                bool        operator == (const UI64UC &ui64UC)  const ;
                bool        operator != (const UI64UC &ui64UC)  const ;
                UI64UC     &operator  = (const UI64UC &ui64UC);


        // Now the data members

    private:
                UI64    m_pUI64;
                bool    m_pIsUserConfigured;
};

}

#endif //UI64UC_H
