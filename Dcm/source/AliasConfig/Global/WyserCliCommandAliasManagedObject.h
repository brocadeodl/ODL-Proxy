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
*   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : cgangwar                                                     *
***************************************************************************/

#ifndef WYSEREACLICOMMANDALIASMANAGEDOBJECT_H
#define WYSEREACLICOMMANDALIASMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
 
using namespace WaveNs;

namespace DcmNs
{
     
class WyserCliCommandAliasManagedObject : public DcmManagedObject
{
       private :
           virtual   void       setupAttributesForPersistence           ();
           virtual   void       setupAttributesForCreate                ();
             
       protected :
       public :
                                WyserCliCommandAliasManagedObject       (WaveObjectManager *pWaveObjectManager);
            virtual             ~WyserCliCommandAliasManagedObject      ();
                     
            static  string      getClassName                            ();
                            			     
                    string      getaliasname                            () const;
                    void        setaliasname                            (string  aliasname);
        
                    string      getaliasexpansion                       () const;
                    void        setaliasexpansion                       (string aliasexpansion);
                            							     
     // Now the data members
       private :
                    string      m_aliasname;
                    string      m_aliasexpansion;
    
       protected :
       public :
};

class WyserEaCliCommandGlobalAliasManagedObject : public DcmManagedObject
{
       private :
           virtual   void       setupAttributesForPersistence           ();
           virtual   void       setupAttributesForCreate                ();
 
       protected :
       public :
                                WyserEaCliCommandGlobalAliasManagedObject       (WaveObjectManager *pWaveObjectManager);
            virtual             ~WyserEaCliCommandGlobalAliasManagedObject      ();
 
            static  string      getClassName                                  ();
  
                    string      getaliasname                                  () const;
                    void        setaliasname                                  (string  aliasname);
 
                    string      getaliasexpansion                             () const;
                    void        setaliasexpansion                             (string aliasexpansion);
 
     // Now the data members
       private :
                    string      m_aliasname;
                    string      m_aliasexpansion;
 
       protected :
       public :
};

}

#endif
