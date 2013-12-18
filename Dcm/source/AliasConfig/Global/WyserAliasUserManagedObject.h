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

#ifndef WYSEREAALIASUSERMANAGEDOBJECT_H
#define WYSEREAALIASUSERMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "AliasConfig/Global/WyserCliCommandAliasManagedObject.h"
 
using namespace WaveNs;
 
namespace DcmNs
{

class WyserAliasUserManagedObject : public DcmManagedObject
{
    private :
                virtual     void        setupAttributesForPersistence       ();
                virtual     void        setupAttributesForCreate            ();

    protected :
    public :         
                                        WyserAliasUserManagedObject         (WaveObjectManager *pWaveObjectManager);
               virtual                  ~WyserAliasUserManagedObject        ();
                  
               static       string      getClassName                        ();
          
                            string      getusername                         () const;
                            void        setusername                         (string  username);
                            void        setuseralias                        (const vector<WaveManagedObjectPointer<WyserCliCommandAliasManagedObject> > &useralias);
                            vector<WaveManagedObjectPointer<WyserCliCommandAliasManagedObject> > getuseralias()  const;
                     

     private :
     protected :
     public :               
             string     m_username;
             vector<WaveManagedObjectPointer<WyserCliCommandAliasManagedObject> > m_useralias;
             string     m_description;
             bool       m_completeonspace;
             bool       m_ignoreleadingspace;
             UI64       m_idletimeout;
             bool       m_paginate;
             UI64       m_history;
             bool       m_autowizard;
             bool       m_showdefaults;
             UI64       m_displaylevel;
             string     m_prompt1;
             string     m_prompt2;
             
};    
}


#endif
