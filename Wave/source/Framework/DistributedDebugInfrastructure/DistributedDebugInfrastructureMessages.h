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


 /*@file DistributedDebugInfrastructureMessages.h 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * Description: This file contains the declaration of all the
 *              messages used by the distributed debug 
 *              infrastructure
 *              
 * 
 * Author :     Aashish Akhouri 
 * Date :       06/18/2011 
 */
#ifndef DISTRIBUTIONDEBUGINFRASTRUCTUREMESSAGES_H
#define DISTRIBUTIONDEBUGINFRASTRUCTUREMESSAGES_H


#include "ManagementInterface/ManagementInterfaceMessage.h"

#include <vector>

using namespace std;

namespace WaveNs
{

class PrismMessage;

class RunDebugScriptMessage: public ManagementInterfaceMessage
{

   public:
                 RunDebugScriptMessage ();
                 RunDebugScriptMessage (bool nosOrbash);
       virtual  ~RunDebugScriptMessage ();
             bool      isScriptNos     () const;
             void      setScriptString   (const string & inputScript);   
       const string &  getScriptString   () const;
             void                   setOutputStringVector           (const vector<string> & outputStringVector);   
       const vector<string> &       getOutputStringVector           () const;  
             void                   setLocationsToRunDebugScript    (const vector<LocationId> & locationsToRunDebugScript);
       const vector<LocationId> &   getLocationsToRunDebugScript    () const;

   private:

      void setupAttributesForSerialization ();
      bool                  m_isNos;
      string                m_scriptString;
      vector<LocationId>    m_locationsToRunDebugScript;
      vector<string>        m_outputStringVector;
};

class RunDebugScriptOnClusterMemberMessage : public PrismMessage
{
   public:
                 RunDebugScriptOnClusterMemberMessage ();
                 RunDebugScriptOnClusterMemberMessage (bool nosOrbash);
       virtual  ~RunDebugScriptOnClusterMemberMessage ();
       
              bool      isScriptNos     () const;
              void      setScriptString   (const string & inputScript);   
       const  string &  getScriptString   () const;
              void      setOutputString   (const string & inputScript);   
       const  string &  getOutputString   () const;             
              
   private:

              void setupAttributesForSerialization ();

      bool     m_isNos;
      string   m_scriptString;
      string   m_outputString;
};

}

#endif//DISTRIBUTIONDEBUGINFRASTRUCTUREMESSAGES_H
