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

#ifndef UI32VECTORUC_H
#define UI32VECTORUC_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class UI32VectorUC
{
    private:
    protected:
    public:
                              UI32VectorUC        ();
                              UI32VectorUC        (const vector<UI32> &ui32Vector);
                              UI32VectorUC        (const vector<UI32> &ui32Vector, const bool &isUserConfigured);
                              UI32VectorUC        (const UI32VectorUC &ui32VectorUC);
                             ~UI32VectorUC        ();

                void          setUI32Vector       (const vector<UI32> &ui32Vector);
                vector<UI32>  getUI32Vector       () const;

                void          setIsUserConfigured (const bool &isUserConfigured);
                bool          getIsUserConfigured () const;
    
                void          toString            (string &valueString);
                void          fromString          (const string &ui32VectorUCInStringFormat);
            
                bool          operator ==         (const UI32VectorUC &ui32VectorUC) const ;
                bool          operator !=         (const UI32VectorUC &ui32VectorUC) const ;
                UI32VectorUC &operator  =         (const UI32VectorUC &ui32VectorUC);


        // Now the data members

    private:
                vector<UI32>  m_pUI32Vector;
                bool          m_pIsUserConfigured;
};

}
#endif //UI32VECTORUC_H
