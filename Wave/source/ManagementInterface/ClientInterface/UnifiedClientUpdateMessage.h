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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 **************************************************************************/

#ifndef UNIFIEDCLIENTUPDATEMESSAGE_H 
#define UNIFIEDCLIENTUPDATEMESSAGE_H 

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectAsynchronousPluginContext.h"
#include "Framework/Types/WaveCValue.h"

namespace WaveNs
{

    class UnifiedClientUpdateMessage : public ManagementInterfaceMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
                    void getAttributeCValue (const ResourceId attributeType, const string &attributeValue);
        public:
            UnifiedClientUpdateMessage ();
            UnifiedClientUpdateMessage (const UI32 opCode);
            UnifiedClientUpdateMessage (const WaveManagedObjectAsynchronousPluginContext *pWaveManagedObjectAsynchronousPluginContext);
            UnifiedClientUpdateMessage (const UI32 opCode, const WaveManagedObjectAsynchronousPluginContext *pWaveManagedObjectAsynchronousPluginContext); 

            virtual    ~UnifiedClientUpdateMessage ();

            virtual     void  initializeFromContext( const WaveManagedObjectAsynchronousPluginContext *pWaveManagedObjectAsynchronousPluginContext );

            void            setManagedObjectName (const string &managedObjectName);
            string          getManagedObjectName ();

            void            setAttributeTypes (const vector<ResourceId> &attributeTypes);
            vector<ResourceId>    getAttributeTypes () const;
            
            void            setAttributeNames (const vector<string> &attributeNames);
            vector<string>  getAttributeNames () const;

            void            setAttributeValues (const vector<string> &attributeValues);
            vector<string>  getAttributeValues () const;
            
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

            void                getAttributeCValue (const ResourceId attributeType, const string &attributeValue, WaveCValue *pCValue);
            virtual       void  deleteCStructureForInputs (const void *pInputStruct);

            UI32            getGroupCode       () const;
            void            setGroupCode       (UI32 groupCode);

            void            setParentManagedObjectNames   ( const vector<string> &parentManagedObjectNames);
            void            getParentManagedObjectNames   ( vector<string> &parentManagedObjectNames) const;

            UI32            getWarningResourceId          () const;
            void            setWarningResourceId          (const UI32 warningResourceId);

            bool            getConfigReplayInProgressFlag () const;
            void            setConfigReplayInProgressFlag (const bool isConfigReplayInProgress);

            vector<string>  getAttributeNamesFromBackend  () const;
 
            vector<string>  getAttributeValuesFromBackend () const;


// Now the data members

        private:
        protected:
        public:
            UI32               m_groupCode;
            string             m_managedObjectName;
            vector<ResourceId> m_attributeTypes;
            vector<string>     m_attributeNames;   
            vector<string>     m_attributeValues;  
            vector<string>     m_parentManagedObjectNames;
            UI32               m_warningResourceId; 
            bool               m_isConfigReplayInProgress; 
            vector<string>     m_attributeNamesFromBackend;
            vector<string>     m_attributeValuesFromBackend;

    };
}
#endif                                            //UNIFIEDCLIENTUPDATEMESSAGE_H

