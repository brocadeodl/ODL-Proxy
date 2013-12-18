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
 *   All rights reserved.
 *   Himanshu Varshney 
 ***************************************************************************/

#ifndef WAVEMANAGEDOBJECTASYNCHRONOUSCREATECONTEXT_H
#define WAVEMANAGEDOBJECTASYNCHRONOUSCREATECONTEXT_H

#include "Framework/Utils/PrismAsynchronousContext.h"

namespace WaveNs
{

class WaveManagedObjectAsynchronousCreateContext : public PrismAsynchronousContext
{
    private :
    protected :
    public :

                            WaveManagedObjectAsynchronousCreateContext  (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
        virtual            ~WaveManagedObjectAsynchronousCreateContext  ();

        vector<string>      getAttributeNames                           () const;
        void                setAttributeNames                           (const vector<string> &attributeNames);

        vector<string>      getAttributeValues                          () const;
        void                setAttributeValues                          (const vector<string> &attributeValues);
        
        string              getManagedObjectClassNameNeedToBeCreated    () const;
        void                setManagedObjectClassNameNeedToBeCreated    (const string &managedObjectClassName);

        ObjectId            getObjectId                                 () const;
        void                setObjectId                                 (const ObjectId &objectId);

        string              getParentAttributeName                      () const;
        void                setParentAttributeName                      (const string &attributeName);

    // Now the data members
    private :
        string              m_managedObjectClassName;
        ObjectId            m_objectId;
        vector<string>      m_attributeNames;
        vector<string>      m_attributeValues;
        string              m_parentAttributeName;

    protected :
    public :
};

}

#endif //WAVEMANAGEDOBJECTASYNCHRONOUSCREATECONTEXT_H
