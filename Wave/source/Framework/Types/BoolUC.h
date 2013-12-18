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

#ifndef BOOLUC_H
#define BOOLUC_H

#include <string>
#include "Framework/Types/Types.h"

namespace WaveNs
{

class BoolUC
{
    private:
    protected:
    public:
                                      BoolUC                      ();
                                      BoolUC                      (const bool &pBool);
                                      BoolUC                      (const bool &pBool, const bool &isUserConfigured);
                                      BoolUC                      (const BoolUC &boolUC);
                                      
                  void                setBoolValue                (const bool &pBool);
                  bool                getBoolValue                () const;
                                      
                  void                setIsUserConfigured         (const bool &isUserConfigured);
                  bool                getIsUserConfigured         () const;
                                      
                  string              toString                    ();
                  void                fromString                  (const string &pBoolUCInStringFormat);
        
                  void                getPlainString              (string &valueString) const;
                  ResourceId          loadFromPlainString         (const string &pBoolUCInPlainStringFormat);
                                      
                  bool                operator == (const BoolUC &pBoolUC)  const ;
                  bool                operator != (const BoolUC &pBoolUC)  const ;
                  BoolUC               &operator  = (const BoolUC &pBoolUC);


        // Now the data members

    private:
                bool    m_pBool;
                bool    m_pIsUserConfigured;
};

}

#endif //BOOLUC_H
