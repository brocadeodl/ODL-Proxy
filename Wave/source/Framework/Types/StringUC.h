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

#ifndef STRINGUC_H
#define STRINGUC_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class StringUC
{
    private:
    protected:
    public:
                               StringUC                     ();
                               StringUC                     (const string &pString);
                               StringUC                     (const string &pString, const bool &isUserConfigured);
                               StringUC                     (const StringUC &stringUC);
                                                            
                void           setStringValue               (const string &pString);
                string         getStringValue               () const;
                                                            
                void           setIsUserConfigured          (const bool &isUserConfigured);
                bool           getIsUserConfigured          () const;
                               
                string         toString                     ();
                void           fromString                   (const string &pStringUCInStringFormat);
                void           getPlainString               (string &valueString) const;
                ResourceId     loadFromPlainString          (const string &pStringUCInPlainStringFormat);
                               
                bool           operator ==                  (const StringUC &pStringUC)  const ;
                bool           operator !=                  (const StringUC &pStringUC)  const ;
                StringUC      &operator =                   (const StringUC &pStringUC);


        // Now the data members

    private:
                string  m_pString;
                bool    m_pIsUserConfigured;
};

}

#endif //StringUC_H
