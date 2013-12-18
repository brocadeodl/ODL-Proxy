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
 *@file RelationReferenceCount.cpp
 * Copyright (C) 2011 Brocade Communications Systems,Inc.
 * All rights reserved. 
 * Description: This file has the implementation for the class
 *              that maintains the reference count of the
 *              relationships at the 'to' end of any
 *              relationship.If the refrencecount becomes 1 from
 *              0, then a call is made to an appropriate virtual
 *              function. Similary if teh reference count
 *              becomes 0 from 1 , then a call is made to the
 *              corresponding virtual function.
 *              
 *               
 * Author :     Aashish Akhouri 
 * Date :       11/04/2011 
 */

#include "Framework/ObjectRelationalMapping/RelationReferenceCount.h"

namespace WaveNs
{


RelationReferenceCount::RelationReferenceCount ()
:mReferenceCount(0)
{
}

RelationReferenceCount::~RelationReferenceCount ()
{
}

void RelationReferenceCount::addReference ()
{
  ++mReferenceCount;
  if(1==mReferenceCount) 
  {
      this->processReferenceCreation ();
  }
}

void RelationReferenceCount::removeReference ()
{
  --mReferenceCount;
  if(0==mReferenceCount) 
  {
      this->processReferenceDeletion ();
  }

}

void RelationReferenceCount::processReferenceCreation ()
{
  return;
}

void RelationReferenceCount::processReferenceDeletion ()
{

}
unsigned int RelationReferenceCount::getReferenceCount ()
{
  return mReferenceCount;
}
  
}
