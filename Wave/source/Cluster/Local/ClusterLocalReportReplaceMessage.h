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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef CLUSTERLOCALREPORTREPLACEMESSAGE_H
#define CLUSTERLOCALREPORTREPLACEMESSAGE_H

#include <string>
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Messaging/Local/PrismMessage.h"
namespace WaveNs
{

class PrismMessage;

///Class:
///ClusterLocalReportReplaceMessage
/// 
///Description:
/// This Class represents the message that will be sent at the end of Phase2 
/// processing of an Replace Rejoin to the Local Cluster Service so that
/// it can update its node status
 
class ClusterLocalReportReplaceMessage: public  PrismMessage
{

    public :
        virtual        ~ClusterLocalReportReplaceMessage ();
                        ClusterLocalReportReplaceMessage ();
                        ClusterLocalReportReplaceMessage (const string & thisNodeIpAddress,const UI32 thisNodePort,const LocationId   thisNodeLocationId, const LocationId   replacedNodeLocationId);


                //Mutators
                void    setThisNodeIpAddress                         (const string &thisNodeName);
                void    setThisNodePort                              (const UI32 thisNodePort);
                void    setThisNodeLocationId                        (const LocationId thisLocationId);
                void    setReplacedNodeLocationId                    (const LocationId replacedLocationId);
       virtual  void    setupAttributesForSerialization              ();
 
                //Accessors
                string  const & getThisNodeIpAddress                 ()  const;
                UI32            getThisNodePort                      ()  const;
                LocationId      getThisNodeLocationId                ()  const;           
                LocationId      getReplacedNodeLocationId            ()  const;           

    protected :
    private :
    // Now the data members


    public:
    protected: 
    private :

        //This node info used in setting the fields in WaveNode
        string          m_thisNodeIpAddress;
        UI32            m_thisNodePort;
        UI32            m_thisNodeLocationId;
        UI32            m_replacedNodeLocationId;

};


}

#endif // CLUSTERLOCALREPORTREPLACEMESSAGE_H
