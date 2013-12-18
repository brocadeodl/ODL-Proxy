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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brunda                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/ConfigContextMappingIntWorker.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetContextMappingIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigRemoveContextMappingIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddContextMappingIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "SnmpServer/Global/ContextMappingManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "snmp/snmplib.h"
#include "DcmResourceIds.h"

namespace DcmNs
{

    ConfigContextMappingIntWorker::ConfigContextMappingIntWorker ( SnmpServerIntLocalObjectManager *pSnmpServerIntLocalObjectManager)
        : WaveWorker  (pSnmpServerIntLocalObjectManager)
    {
        addOperationMap (SNMPSERVERINTSNMPCONFIGSETCONTEXTMAPPINGINT, reinterpret_cast<PrismMessageHandler> (&ConfigContextMappingIntWorker::SnmpServerIntSnmpConfigSetContextMappingIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGREMOVECONTEXTMAPPINGINT, reinterpret_cast<PrismMessageHandler> (&ConfigContextMappingIntWorker::SnmpServerIntSnmpConfigRemoveContextMappingIntMessageHandler));
        addOperationMap (SNMPSERVERINTSNMPCONFIGADDCONTEXTMAPPINGINT, reinterpret_cast<PrismMessageHandler> (&ConfigContextMappingIntWorker::SnmpServerIntSnmpConfigAddContextMappingIntMessageHandler));
    }

    ConfigContextMappingIntWorker::~ConfigContextMappingIntWorker ()
    {
    }

    PrismMessage  *ConfigContextMappingIntWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERINTSNMPCONFIGSETCONTEXTMAPPINGINT :
                pPrismMessage = new SnmpServerIntSnmpConfigSetContextMappingIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGREMOVECONTEXTMAPPINGINT :
                pPrismMessage = new SnmpServerIntSnmpConfigRemoveContextMappingIntMessage ();
                break;
            case SNMPSERVERINTSNMPCONFIGADDCONTEXTMAPPINGINT :
                pPrismMessage = new SnmpServerIntSnmpConfigAddContextMappingIntMessage ();
                break;
           default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigContextMappingIntWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

	/*
    ** This function is the local message handler to add new Context Mapping details .
    ** Called from: Global message handler of add ContextMapping.
    ** Inputs:      Context Name and optionally vrf-name 
    ** Functions:   Finds an empty entry in the SNMPd agtcfg structure, populates it with new values.
    ** Errors:      Return GET/SET errors if  backend failed. Only 256 context Mapping entries can be populated.
    ** On success:  Return WAVE_MESSAGE_SUCCESS.	
	*/

    void  ConfigContextMappingIntWorker::SnmpServerIntSnmpConfigAddContextMappingIntMessageHandler( SnmpServerIntSnmpConfigAddContextMappingIntMessage *pSnmpServerIntSnmpConfigAddContextMappingIntMessage)
    {
		trace (TRACE_LEVEL_DEBUG, "Local message handler Add Context Mapping: Coming here");

		if (pSnmpServerIntSnmpConfigAddContextMappingIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Add ContextMapping Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		agtcfg_t agtcfgin;
		int ret = -1, Sret = -1, found = -1, count = 0, i = 0;

		string contextName = pSnmpServerIntSnmpConfigAddContextMappingIntMessage->getContextName(); 
		string vrfName = pSnmpServerIntSnmpConfigAddContextMappingIntMessage->getVrfName();
        trace(TRACE_LEVEL_DEBUG,string("size of agtcfg_t and agtcfgin ")+sizeof(agtcfg_t) +sizeof(agtcfgin));

		ret = agtcfgXMGet(&agtcfgin);


		if (ret == -1) {
			trace (TRACE_LEVEL_ERROR,"Local message handler ADD: GET failed");
			pSnmpServerIntSnmpConfigAddContextMappingIntMessage->setCompletionStatus 
								(WRC_SNMP_GET_CONTEXT_MAPPING_FAILED);
			reply(pSnmpServerIntSnmpConfigAddContextMappingIntMessage);
			return;
		}

		for (i = 0; i < MAX_CONTEXT_MAPPING_ENTRIES; i++)
		{
			if (contextName.compare((char *)agtcfgin.agtContextMap[i].contextName) == 0) {
				found = 0;
				break;
			}
			
		}
		if (found != 0) {
	        for (count = 0; count < MAX_CONTEXT_MAPPING_ENTRIES; count++)
    	    {
            if(strlen(agtcfgin.agtContextMap[count].contextName)==0)
             {
                 trace (TRACE_LEVEL_DEBUG, string ("Add this contextMap") +contextName);
                 strncpy(agtcfgin.agtContextMap[count].contextName,contextName.c_str(),contextName.length());
                 agtcfgin.agtContextMap[count].contextName[contextName.length()] = '\0';
                 strncpy(agtcfgin.agtContextMap[count].vrfName,vrfName.c_str(),vrfName.length());
                 agtcfgin.agtContextMap[count].vrfName[vrfName.length()] = '\0';
                 found = 1;
                 break;  
             }
        	}
		}

		if (found == 1) {
			Sret = agtcfgXMSet(&agtcfgin);
			if (Sret == -1) {
				trace (TRACE_LEVEL_FATAL, "Local message handler ADD Context Mapping: SET failed");
				pSnmpServerIntSnmpConfigAddContextMappingIntMessage->setCompletionStatus 
								(WRC_SNMP_SET_CONTEXT_MAPPING_FAILED);
			} else {
				pSnmpServerIntSnmpConfigAddContextMappingIntMessage->setCompletionStatus 
										(WAVE_MESSAGE_SUCCESS);
			}
		} else if (found == -1) {
			trace (TRACE_LEVEL_FATAL, "Local message handler ADD ContextMapping: No of CONTEXT MApping Entries exceeded");
            pSnmpServerIntSnmpConfigAddContextMappingIntMessage->setCompletionStatus 
								(WRC_SNMP_NUM_CONTEXT_MAPPING_EXCEEDED_ERROR);
		} else {
			trace (TRACE_LEVEL_INFO, "Local message handler ADD ContextMapping: Data Already exists");
			pSnmpServerIntSnmpConfigAddContextMappingIntMessage->setCompletionStatus
                                        (WAVE_MESSAGE_SUCCESS);
		} 

		reply(pSnmpServerIntSnmpConfigAddContextMappingIntMessage);
    }

	/*
    ** This function is the local message handler to remove Context Mapping Details.
    ** Called from: Global message handler of remove context Mapping.
    ** Inputs:      Context Name and optionally vrf-name.
    ** Functions:   Finds the context MApping  inputted from SNMPd agtcfg structure, populates it with NULL.
    ** Errors:      Return GET/SET errors if failed..
    ** On success:  Return WAVE_MESSAGE_SUCCESS.	
	*/

    void  ConfigContextMappingIntWorker::SnmpServerIntSnmpConfigRemoveContextMappingIntMessageHandler( SnmpServerIntSnmpConfigRemoveContextMappingIntMessage *pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage)
	{
		trace (TRACE_LEVEL_DEBUG, "Local message handler Remove Context Mapping: Coming here");
		if (pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Del ContextMapping Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		string contextName = pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage->getContextName();
		string delFieldName = pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage->getDelFieldName();
		trace (TRACE_LEVEL_INFO, "Local message handler REMOVE: Coming here" +contextName + delFieldName);

		agtcfg_t agtcfgin;
		int ret = -1, Sret = -1, found = -1, count = 0;
		ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1) {
			trace (TRACE_LEVEL_ERROR, "Local message handler REMOVE: GET failed");
			pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage->setCompletionStatus 
									(WRC_SNMP_GET_CONTEXT_MAPPING_FAILED);
			reply (pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage);
			return;
		}

       for (count = 0; count < MAX_CONTEXT_MAPPING_ENTRIES; count++)
       {
            if(contextName.compare(agtcfgin.agtContextMap[count].contextName)==0) 
            {
                trace (TRACE_LEVEL_DEBUG, string ("Deleting  this") +agtcfgin.agtContextMap[count].contextName);
                if(delFieldName.length() == 0)
                {
                     agtcfgin.agtContextMap[count].contextName[0]='\0';
                     memset(agtcfgin.agtContextMap[count].contextName, 0x0, AGT_MAX_CONTEXT_SIZE);
                     agtcfgin.agtContextMap[count].vrfName[0]='\0';
                     memset(agtcfgin.agtContextMap[count].vrfName, 0x0, AGT_MAX_CONTEXT_SIZE);
                }
                else
                {
                      if(delFieldName.compare( "vrfName")==0)
                      {
                           agtcfgin.agtContextMap[count].vrfName[0]='\0';
                           memset(agtcfgin.agtContextMap[count].vrfName, 0x0, AGT_MAX_CONTEXT_SIZE);

                      } 
                }
                found =1;
                break;
            }
     
       }
		
		if (found == 1) {
			Sret = agtcfgXMSet(&agtcfgin);
			if (Sret == -1) {
				trace (TRACE_LEVEL_ERROR, "Local message handler REMOVE: SET failed");
				pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage->setCompletionStatus 
										(WRC_SNMP_SET_CONTEXT_MAPPING_FAILED);
			} else {
				pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage->setCompletionStatus 
											(WAVE_MESSAGE_SUCCESS);
			}
		} else {
			trace (TRACE_LEVEL_INFO, "Local message handler REMOVE: Data not exists in Backend");
			pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage->setCompletionStatus 
										(WAVE_MESSAGE_SUCCESS);
		} 
		reply (pSnmpServerIntSnmpConfigRemoveContextMappingIntMessage);
	}

	/*
    ** This function is the local message handler to change vrfName.
    ** Called from: Global message handler of set contextMapping .
    ** Inputs:      ContextName, optionally vrfName.
    ** Functions:   Finds the contextMapping inputted from SNMPd agtcfg structure, copies it to correct 
	**				position and NULLS the previous location.
    ** Errors:      Return GET/SET errors if failed. 
    ** On success:  Return WAVE_MESSAGE_SUCCESS.	
	*/

    void  ConfigContextMappingIntWorker::SnmpServerIntSnmpConfigSetContextMappingIntMessageHandler( SnmpServerIntSnmpConfigSetContextMappingIntMessage *pSnmpServerIntSnmpConfigSetContextMappingIntMessage)
	{
		trace (TRACE_LEVEL_ERROR, "Local Set message handler: Coming here");
		if (pSnmpServerIntSnmpConfigSetContextMappingIntMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Change ContextMapping Local:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
        }

		agtcfg_t    agtcfgin;
		string contextName = pSnmpServerIntSnmpConfigSetContextMappingIntMessage->getContextName();
		string vrfName     = pSnmpServerIntSnmpConfigSetContextMappingIntMessage->getVrfName();

		int ret = -1,  idx = -1, Sret = -1, count = 0;
        bool addSuccess = false;

		ret = agtcfgXMGet(&agtcfgin);

		if (ret == -1) {
			trace (TRACE_LEVEL_ERROR, "Local message handler SET: GET failed");
			pSnmpServerIntSnmpConfigSetContextMappingIntMessage->setCompletionStatus(
							WRC_SNMP_GET_CONTEXT_MAPPING_FAILED);
			reply (pSnmpServerIntSnmpConfigSetContextMappingIntMessage);
			return;
		}

		for (count = 0; count < MAX_CONTEXT_MAPPING_ENTRIES; count++) {
			if (contextName.compare(agtcfgin.agtContextMap[count].contextName) == 0) {
				idx = count;
				trace (TRACE_LEVEL_DEBUG, string ("Change this") 
						+agtcfgin.agtContextMap[count].contextName);

                if (vrfName.length() != 0) {
                    agtcfgin.agtContextMap[count].vrfName[0]='\0';
                    memset(agtcfgin.agtContextMap[count].vrfName, 0x0, AGT_MAX_CONTEXT_SIZE);
                    strncpy(agtcfgin.agtContextMap[count].vrfName, vrfName.c_str(), vrfName.length());
                    agtcfgin.agtContextMap[count].vrfName[vrfName.length()]='\0';
                    addSuccess = true;
                }
				break;
			}
		}


		if ((idx != -1) && (addSuccess == true)) {
			Sret = agtcfgXMSet(&agtcfgin);
			if (Sret == -1) {
				trace (TRACE_LEVEL_ERROR, "SNMP Local Set MessageHandler - failed");
				pSnmpServerIntSnmpConfigSetContextMappingIntMessage ->setCompletionStatus
									(WRC_SNMP_SET_CONTEXT_MAPPING_FAILED);
				reply (pSnmpServerIntSnmpConfigSetContextMappingIntMessage);
				return;
			}
		}

 		if (addSuccess != true) {
			pSnmpServerIntSnmpConfigSetContextMappingIntMessage ->setCompletionStatus 
							(WRC_SNMP_CONTEXT_MAPPING_FAILED);
		} else {
			pSnmpServerIntSnmpConfigSetContextMappingIntMessage ->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		}
		
		reply (pSnmpServerIntSnmpConfigSetContextMappingIntMessage);

	}

}
