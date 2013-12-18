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
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#ifndef NODEMANAGEDOBJECT_H
#define NODEMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveWorker.h"

#include "Framework/ObjectModel/WaveManagedObject.h"
#include <string>
#include <vector>


namespace WaveNs
{

    typedef enum
    {
        NODE_CREATED,
        NODE_FAILED,
        NODE_COMMUNICATED
    } nodeStatusEnum;

class NodeManagedObject : public  WaveManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                           NodeManagedObject          (WaveObjectManager *pWaveObjectManager);
                           //NodeManagedObject          (WaveObjectManager *pWaveObjectManager, string nodeName);
                           NodeManagedObject          (WaveObjectManager *pWaveObjectManager, LocationId locationId, string nodeName, UI32 nodePort);
                           NodeManagedObject          (const NodeManagedObject &rhs);
                          ~NodeManagedObject          ();

        static string      getClassName               ();

        ResourceId         getNodeGenericStatus       ();
        void               setNodeGenericStatus       (ResourceId status);

        LocationId         getLocationId              () const;
        void               setLocationId              (const LocationId &locationId);

        string             getNodeName                ();
        void               setNodeName                (string nodeName);

        UI32               getNodePort                ();
        void               setNodePort                (UI32 nodePort);
        NodeManagedObject &operator =                 (const NodeManagedObject &rhs);
        bool               operator ==                (const NodeManagedObject &rhs);
        bool               operator !=                (const NodeManagedObject &rhs);

        // Now the data members

    private :
    protected :
    public :
        LocationId                m_locationId;
        string                    m_nodeName;
        UI32                      m_nodePort;

};

class Node
{
    private :
    protected :
    public :
                         Node          ();
                         Node          (string nodeName, UI32 nodePort);
                        ~Node          ();

        string           getNodeName   ();
        void             setNodeName   (string nodeName);

        UI32             getNodePort   ();
        void             setNodePort   (UI32 nodePort);
        Node            &operator =    (const Node &rhs);
        bool             operator ==   (const Node &rhs);
        bool             operator !=   (const Node &rhs);


        // Now the data members

    private :
    protected :
    public :
        string                    m_nodeName;
        UI32                      m_nodePort;

};

}
#endif //NodeManagedObject

