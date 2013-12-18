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

/**
 *@file RelationReferenceCountPointer.h 
 * Copyright (C) 2011 Brocade Communications Systems,Inc.
 * All rights reserved. 
 * Description: This file declares the class that maintains the
 *              reference count of the relationships at the 'to'
 *              end of any relationship.
 *              
 *               
 * Author :     Aashish Akhouri 
 * Date :       11/04/2011 
 */


#ifndef RELATIONREFERENCECOUNTPOINTER_H
#define RELATIONREFERENCECOUNTPOINTER_H


#include "Framework/Types/Types.h"

namespace WaveNs
{

template<class T> class RelationReferenceCountPointer
{
 public:
  RelationReferenceCountPointer (const RelationReferenceCountPointer<T> & pRelationReferenceCountPointer);
  RelationReferenceCountPointer (T* pObjectPtr);
  ~RelationReferenceCountPointer ();
   T* get ();
  RelationReferenceCountPointer & operator=(RelationReferenceCountPointer & pRelationReferenceCountPointer);

  T*  operator->() const;
  T & operator*();
 private:
  T *mPtrToObject;

};




}


#endif
