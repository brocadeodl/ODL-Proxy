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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PHYSICALINTERFACELOCALOBJECTMANAGER_H
#define PHYSICALINTERFACELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

class PhysicalInterfaceCreateMessage;
class PhysicalInterfaceUpdateMessage;

class PhysicalInterfaceLocalObjectManager : public WaveLocalObjectManager
{
    private :
                      PhysicalInterfaceLocalObjectManager   ();

         virtual      WaveManagedObject *createManagedObjectInstance             (const string &managedClassName);              
                      PrismMessage *createMessageInstance                 (const UI32 &operationCode);

        void          createPhysicalInterfaceMessageHandler (PhysicalInterfaceCreateMessage *pPhysicalInterfaceCreateMessage);
        void          updatePhysicalInterfaceMessageHandler (PhysicalInterfaceUpdateMessage *pPhysicalInterfaceUpdateMessage);

    protected :
    public :
        virtual                                      ~PhysicalInterfaceLocalObjectManager ();

        static   string                               getClassName                        ();
        static   PhysicalInterfaceLocalObjectManager *getInstance                         ();
        static   PrismServiceId                       getPrismServiceId                   ();


    // Now the data members

    private:
    protected:
    public :
};

inline string PhysicalInterfaceLocalObjectManager::getClassName ()
{
    return ("Phyiscal Interface Local Management");
}

}

#endif // PHYSICALINTERFACELOCALOBJECTMANAGER_H
