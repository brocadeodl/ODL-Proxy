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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.              *
 *   All rights reserved.                                                 *
 *   Author : Aashish Akhouri                                             *
 ***************************************************************************/

#ifndef WAVEMANAGEDOBJECTTOOLKIT_H
#define WAVEMANAGEDOBJECTTOOLKIT_H

#include <vector>

using namespace std;
namespace WaveNs
{
class WaveManagedObject;
class Attribute;

//Class:WaveManagedObjectToolKit
//Description: This class will contain methods to process
// do miscellaneous operations on Managed objects
class WaveManagedObjectToolKit
{

    public :
        //Releases memory from the pointers in the vector as well as 
        // as the pointer to the vector as well
        static void releaseMemoryOfWaveMOVector     (vector<WaveManagedObject *>* & pResults);
        static void releaseMemoryOfAttributeVector  (vector<Attribute *> &attrVector );


    protected :
    private :
        //This class cannot be instantiated
        //and hence not destroyed
         WaveManagedObjectToolKit();
        ~WaveManagedObjectToolKit();

    private :
    protected :
    public :

};

}

#endif // WAVEMANAGEDOBJECTTOOLKIT_H
