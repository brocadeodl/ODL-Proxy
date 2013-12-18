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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef STRINGVECTORUC_H
#define STRINGVECTORUC_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class StringVectorUC
{
    private:
    protected:
    public:
                                StringVectorUC      ();
                                StringVectorUC      (const vector<string> &stringVector);
                                StringVectorUC      (const vector<string> &stringVector, const bool &isUserConfigured);
                                StringVectorUC      (const StringVectorUC &stringVectorUC);
                               ~StringVectorUC      ();

                void            setStringVector     (const vector<string> &stringVector);
                vector<string>  getStringVector     () const;

                void            setIsUserConfigured (const bool &isUserConfigured);
                bool            getIsUserConfigured () const;
    
                void            toString            (string &valueString);
                void            fromString          (const string &stringVectorUCInStringFormat);
            
                bool            operator ==         (const StringVectorUC &stringVectorUC) const ;
                bool            operator !=         (const StringVectorUC &stringVectorUC) const ;
                StringVectorUC &operator  =         (const StringVectorUC &stringVectorUC);

                string          getPlainString      () const;


        // Now the data members

    private:
                vector<string>  m_pStringVector;
                bool            m_pIsUserConfigured;
};

}
#endif //STRINGVECTORUC_H
