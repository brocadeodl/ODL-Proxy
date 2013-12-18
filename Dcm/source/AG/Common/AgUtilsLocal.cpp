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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Ramachandran Anantharaman                                    *
 **************************************************************************/
#include <algorithm>
#include <vector>
#include <sstream>

#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "AG/Local/AgLocalObjectManager.h"
#include "AG/Local/AgLocalConfigMessage.h"
#include "AG/Local/AgLocalManagedObject.h"
#include "AG/Local/AgNPortMapLocalManagedObject.h"
#include "AG/Local/AgPortGroupLocalManagedObject.h"
#include "AG/Local/AgLocalTypes.h"
#include "ClientInterface/AG/AgClientEnableMessage.h"
#include "AG/Common/AgUtils.h"
#include "AG/Common/AgUtilsLocal.h"
#include "PortCfg/Local/PortCfgPortCfgPortTranslateMessage.h"
#include "agd/public.h"



namespace DcmNs
{

    ResourceId AgUtilsLocal::validate_pgrename(UI32 pgid, string name) {
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        UI32 pgid_temp;
        int i;
        LocationId loc_id;

        //Validate PGID
        BM_Clear(bitmap, MAX_AG_PORT);

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            for(i = 0; i < (int)pResults->size(); i++) {
                pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[i]);
                pgid_temp = pAgPortGroupLocalManagedObject->getPgId();
                if(pgid_temp <= 8) {
                    BM_SetBitN(pgid_temp, bitmap);
                }
            }
        }
        if (!(BM_isBitNSet(pgid, bitmap))) {
            if (pResults) {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                delete pResults;
            }
            if(bitmap) {
                free(bitmap);
            }
            if(pSyncQueryCtx) {
                delete pSyncQueryCtx;
            }

            return AG_INVALID_PGID;
        }

        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            delete pResults;
        }
        if(bitmap) {
            free(bitmap);
        }
        if(pSyncQueryCtx) {
            delete pSyncQueryCtx;
        }
        return AG_VALIDATION_SUCCESS;
    }

    ResourceId AgUtilsLocal::validate_pgremove(UI32 pgid) {
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        UI32 pgid_temp;
        int i;
        LocationId loc_id;

        //Validate PGID
        if(pgid == 0){
            return AG_INVALID_PGID;
        }

        BM_Clear(bitmap, MAX_AG_PORT);

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            for(i = 0; i < (int)pResults->size(); i++) {
                pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[i]);
                pgid_temp = pAgPortGroupLocalManagedObject->getPgId();
                if(pgid_temp <= 8) {
                    BM_SetBitN(pgid_temp, bitmap);
                }
            }
        }
        if (!(BM_isBitNSet(pgid, bitmap))) {
            if (pResults) {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                delete pResults;
            }
            if(bitmap) {
                free(bitmap);
            }
            if(pSyncQueryCtx) {
                delete pSyncQueryCtx;
            }

            return AG_INVALID_PGID;
        }

        if (pResults) {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            delete pResults;
        }
        if(bitmap) {
            free(bitmap);
        }
        if(pSyncQueryCtx) {
            delete pSyncQueryCtx;
        }
        return AG_VALIDATION_SUCCESS;
    }

    ResourceId AgUtilsLocal::validate_pgdelmodes(UI32 pgid, vector<string> Modes) {
        vector<string>::iterator iter;
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        string mode;
        vector<string> existing_modes;
        LocationId loc_id;

        //validate the pgmodes
        //The only valid mode that can be deleted in LB mode
        for(iter = Modes.begin(); iter != Modes.end(); iter++) {
            mode = *iter;
            if((strcmp(mode.c_str(), AG_PGFLG_LB_NAME) != 0)) {
                if(bitmap)
                    free(bitmap);

                return AG_INVALID_PGMODES;
            }
        }

        BM_Clear(bitmap, MAX_AG_PORT);
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid, "agPgId"));
        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);
            existing_modes = pAgPortGroupLocalManagedObject->getPgFlags();
            for(iter = existing_modes.begin(); iter != existing_modes.end(); iter++) {
                mode = *iter;
                if(strcmp(mode.c_str(), AG_PGFLG_LB_NAME) == 0)  
                    BM_SetBitN(0, bitmap);
            }
        } else {
           if(bitmap)  
                 free(bitmap);

           if(pSyncQueryCtx)
               delete pSyncQueryCtx;

            return AG_INVALID_PGID;
        }

        for(iter = Modes.begin(); iter != Modes.end(); iter++) {
            mode = *iter;
            if(strcmp(mode.c_str(), AG_PGFLG_LB_NAME) == 0) {
                if (!(BM_isBitNSet(0, bitmap))) {
                    if(bitmap)  
                          free(bitmap);

                    if(pSyncQueryCtx)
                        delete pSyncQueryCtx;

                    return AG_MODE_NOT_SET;
                }
            }
        }

        if(bitmap)  
            free(bitmap);

        if(pSyncQueryCtx)
            delete pSyncQueryCtx;

        return AG_VALIDATION_SUCCESS;
    }

    ResourceId AgUtilsLocal::validate_pgaddmodes(UI32 pgid, vector<string> Modes) {
        vector<string>::iterator iter;
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        string mode;
        vector<string> existing_modes;
        LocationId loc_id;

        //validate the pgmodes
        for(iter = Modes.begin(); iter != Modes.end(); iter++) {
            mode = *iter;
            if((strcmp(mode.c_str(), AG_PGFLG_LB_NAME) != 0)) { 
               if(bitmap)
                   free(bitmap);

                return AG_INVALID_PGMODES;
            }
        }

        BM_Clear(bitmap, MAX_AG_PORT);
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid, "agPgId"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);
            existing_modes = pAgPortGroupLocalManagedObject->getPgFlags();
            for(iter = existing_modes.begin(); iter != existing_modes.end(); iter++) {
                mode = *iter;
                if(strcmp(mode.c_str(), AG_PGFLG_LB_NAME) == 0)  
                    BM_SetBitN(0, bitmap);
            }
        } else {
            if(bitmap)
                  free(bitmap);

              if(pSyncQueryCtx)
                  delete pSyncQueryCtx;

            return AG_INVALID_PGID;
        }

        for(iter = Modes.begin(); iter != Modes.end(); iter++) {
            mode = *iter;
            if(strcmp(mode.c_str(), AG_PGFLG_LB_NAME) == 0) {
                if ((BM_isBitNSet(0, bitmap))) {
                    if(bitmap)  
                          free(bitmap);

                    if(pSyncQueryCtx)
                        delete pSyncQueryCtx;

                    return AG_MODE_ALREADY_SET;
                }
            }
        }

        if(bitmap)  
              free(bitmap);

        if(pSyncQueryCtx)
              delete pSyncQueryCtx;

        return AG_VALIDATION_SUCCESS;
    }

    ResourceId AgUtilsLocal::validate_pgdel(UI32 pgid, vector<string> nports_given) {
        vector<string>::iterator iter;
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        vector<string> nports_temp;
        string mode;
        int i, portinteger, pgid_temp, ret;
        LocationId loc_id;

        // verify if the pgid exists
        if((pgid) <= 0 || (pgid > 8)) {
            return AG_INVALID_PGID;
        }

        BM_Clear(bitmap, MAX_AG_PORT);

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            for(i = 0; i < (int)pResults->size(); i++) {
                pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[i]);
                pgid_temp = pAgPortGroupLocalManagedObject->getPgId();
                if(pgid_temp <= 8) {
                    BM_SetBitN(pgid_temp, bitmap);
                }
            }
        }
        if (!(BM_isBitNSet(pgid, bitmap))) {
            return AG_INVALID_PGID;
        }

        //verify if the nports given belong to pgx
        //verify if they are nports first
        for(iter = nports_given.begin(); iter != nports_given.end(); iter++) {
            if(!(AgUtils::isInterfaceNportLocked(*iter))) {
                trace (TRACE_LEVEL_ERROR, "AgUtils::validate_pgdel : Check N port value !!\n");
                return AG_INVALID_NPORT;
            }
        }

        BM_Clear(bitmap, MAX_AG_PORT);

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid, "agPgId"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);
            nports_temp = pAgPortGroupLocalManagedObject->getPgNPortTopo();
            for(iter = nports_temp.begin(); iter != nports_temp.end(); iter++) {
                ret = AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger);
                if(ret != -1) {
                   trace (TRACE_LEVEL_DEBUG, string("Going to set value!") + portinteger);
                   if(portinteger < MAX_AG_PORT) {
                       BM_SetBitN(portinteger, bitmap);
                   } else {
                       trace (TRACE_LEVEL_ERROR, string("port integer problem.. wrong value!") + portinteger);
                   }
                 }
             }
        }

        for(iter = nports_given.begin(); iter != nports_given.end(); iter++) {
            ret = AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger);
            if(ret != -1) {
               if(portinteger < MAX_AG_PORT) {
                   if (!(BM_isBitNSet(portinteger, bitmap))) {
                       return AG_INVALID_NPORT;
                   }
               }
            }
        }

        return AG_VALIDATION_SUCCESS;
    }

    ResourceId AgUtilsLocal::validate_pgadd(UI32 pgid, vector<string> nports_given) {
        vector<string>::iterator iter;
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        UI32 pgid_0;
        vector<string> nports_temp;
        string mode;
        int i, portinteger, pgid_temp, ret;
        LocationId loc_id;

        // verify if the pgid exists

        BM_Clear(bitmap, MAX_AG_PORT);

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            for(i = 0; i < (int)pResults->size(); i++) {
                pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[i]);
                pgid_temp = pAgPortGroupLocalManagedObject->getPgId();
                BM_SetBitN(pgid_temp, bitmap);
            }
        }
        if (!(BM_isBitNSet(pgid, bitmap))) {
            return AG_INVALID_PGID;
        }

        //verify if the nports given belong to pg0(not belonging to any PG)
        //verify if they are nports first
        for(iter = nports_given.begin(); iter != nports_given.end(); iter++) {
            if(!(AgUtils::isInterfaceNportLocked(*iter))) {
                trace (TRACE_LEVEL_ERROR, "AgUtils::validate_pgadd : Check N port value !!\n");
                return AG_INVALID_NPORT;
            }
        }

        BM_Clear(bitmap, MAX_AG_PORT);
        pgid_0 = 0;

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeUI32 (pgid_0, "agPgId"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[0]);
            nports_temp = pAgPortGroupLocalManagedObject->getPgNPortTopo();
            for(iter = nports_temp.begin(); iter != nports_temp.end(); iter++) {
                ret = AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger);
                if(ret != -1) {
                   trace (TRACE_LEVEL_DEBUG, string("Going to set value!") + portinteger);
                   if(portinteger < MAX_AG_PORT) {
                       BM_SetBitN(portinteger, bitmap);
                   } else {
                       trace (TRACE_LEVEL_ERROR, string("port integer problem.. wrong value!") + portinteger);
                   }
                 }
             }
        }

        for(iter = nports_given.begin(); iter != nports_given.end(); iter++) {
            ret = AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FI, portinteger);
            if(ret != -1) {
               if(portinteger < MAX_AG_PORT) {
                   if (!(BM_isBitNSet(portinteger, bitmap))) {
                       return AG_NPORT_ALREADY_IN_PG;
                   }
               }
            }
        }

        return AG_VALIDATION_SUCCESS;
    }

    ResourceId AgUtilsLocal::validate_pgcreate(UI32 pgid) {


        vector<string>::iterator iter;
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgPortGroupLocalManagedObject *pAgPortGroupLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        UI32 pgid_temp;
        vector<string> nports_temp;
        string mode;
        int i;
        LocationId loc_id;
        int no_nports;
        BM_Clear(bitmap, MAX_AG_PORT);


        /* The allowed number of PGs are max N port - 1, hence this check */
        no_nports = AgUtilsLocal::getNumNports();
        if((int)(pgid) < 1 || ((int)pgid > (no_nports - 1))) {
            return AG_INVALID_PGID_CREATE_NUMBER;
        }

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgPortGroupLocalManagedObject::getClassName());
        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            for(i = 0; i < (int)pResults->size(); i++) {
                pAgPortGroupLocalManagedObject = dynamic_cast<AgPortGroupLocalManagedObject *>((*pResults)[i]);
                pgid_temp = pAgPortGroupLocalManagedObject->getPgId();
                    BM_SetBitN(pgid_temp, bitmap);
            }
        }

        if ((BM_isBitNSet(pgid, bitmap))) {
            return AG_PGID_IN_USE;
        }

        if(pSyncQueryCtx)
            delete pSyncQueryCtx;


        return AG_VALIDATION_SUCCESS;
    }
             


   ResourceId AgUtilsLocal::validate_policy(vector<string> policies, int flag) {
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgLocalManagedObject *pAgLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        vector<string> curr_policy ;
        vector<string>::iterator iter;
        vector<string>::iterator iter_policy;
        string check_policy;
        ResourceId ret = AG_VALIDATION_SUCCESS;
        LocationId loc_id;
        string policy;

        for(iter_policy = policies.begin(); iter_policy != policies.end(); iter_policy++) {
		policy = *iter_policy;
		WaveNs::trace (TRACE_LEVEL_DEBUG, string("AgUtils::validate_policy : Policy = ")+policy + "  " + flag);

		if((strcasecmp(policy.c_str(), "pg") != 0)) { 
		      return(AG_INVALID_POLICY);
		}

		if(strcasecmp(policy.c_str(), "pg") == 0) {
		      return(AG_POLICY_CANT_CHANGE);
		}
		pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgLocalManagedObject::getClassName());

		loc_id = FrameworkToolKit::getThisLocationId();
		pSyncQueryCtx->setLocationIdFilter(loc_id);
		pResults = querySynchronously (pSyncQueryCtx);

		if (NULL != pResults && pResults->size()) {
			trace (TRACE_LEVEL_DEBUG, "AgAgUtils::validate_policy : Got the MO!\n");
			pAgLocalManagedObject = dynamic_cast<AgLocalManagedObject *>((*pResults)[0]);
			curr_policy = pAgLocalManagedObject->getPolicy();

			iter = find(curr_policy.begin(), curr_policy.end(), policy);
			if(iter != curr_policy.end()) {
			      if (flag) {
				 ret = AG_POLICY_NOCHANGE;
			      }
			} else {
			      if (!flag) {
				 ret = AG_POLICY_NOCHANGE;
			      }
			}
		}

		if (pResults) {
		    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		    delete pResults;
		}

		if(pSyncQueryCtx)
		    delete pSyncQueryCtx;
        }

        return ret; 
    }


    ResourceId AgUtilsLocal::validate_mapset(string nPort, vector<string> fPorts) 
    {
        string nport_interface_converted;
        int nport_cf;
        vector<string> map_fports;
        vector<string>::iterator iter, iter1;
        string fport_temp;
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        int fportinteger, mid = AgUtils::getLocalMappedId();
        LocationId loc_id;

        //Validations
        trace (TRACE_LEVEL_INFO, string("AgUtilsLocal::validate_mapset : N port value = ")+nPort);
        for(iter = fPorts.begin(); iter != fPorts.end(); iter++) {
            trace (TRACE_LEVEL_INFO,string( "AgUtilsLocal::validate_mapset : Value of F port : ")+*iter);
        }

        //make sure the nport is valid
        if(!(AgUtils::isInterfaceNportLocked(nPort))) {
            trace (TRACE_LEVEL_ERROR, "Validation error : Check N port value !!\n");
            return AG_INVALID_NPORT;
        }

        //make sure the fports list is valid
        for(iter = fPorts.begin(); iter != fPorts.end(); iter++) {
            if(!AgUtils::isFcoeInterfaceValid(*iter)) {
                trace (TRACE_LEVEL_ERROR, "AgUtilsLocal::validate_mapset::Check F port values !\n");
                return AG_INVALID_FPORT;
            }
        }

        // make sure none  of the fports given are already mapped
        for(nport_cf = 0;nport_cf < MAX_AG_PORT; nport_cf ++) {
            trace (TRACE_LEVEL_INFO, string("AgUtilsLocalLocal::validate_mapset : Retrieving Fports for N port value = ")+nport_cf);

            AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FI, nport_cf, nport_interface_converted);

            trace (TRACE_LEVEL_INFO, string("AgUtilsLocal::validate_mapset : Retrieving Fports for N port value = ")+nport_interface_converted);

            if(AgUtils::isInterfaceNportLocked(nport_interface_converted)) {
                if(nport_interface_converted != nPort) {

                    trace (TRACE_LEVEL_INFO, string("AgUtilsLocal::validate_mapset : Not equal, so for N port  = ")+nport_interface_converted);

                    pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());
                    pSyncQueryCtx->addAndAttribute (new AttributeString (nport_interface_converted, "agNPortNb"));
 
                    loc_id = FrameworkToolKit::getThisLocationId();
                    pSyncQueryCtx->setLocationIdFilter(loc_id);
                    pResults = querySynchronously (pSyncQueryCtx);

                    if (NULL != pResults && pResults->size()) {
                        trace (TRACE_LEVEL_INFO, string("AgUtilsLocal::validate_mapset : Not NULL = ")+nport_interface_converted);

                        pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
                        map_fports = pAgNPortMapLocalManagedObject->getNFPortTopo();

                        for(iter = map_fports.begin(); iter != map_fports.end(); iter++) {
                            trace (TRACE_LEVEL_INFO, string("AgUtilsLocal::validate_mapset : Fports are ")+*iter);

                            AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FCOE, fportinteger);

                            if(fportinteger <= MAX_AG_PORT) {
                                BM_SetBitN(fportinteger, bitmap);
                            } else {
                                trace (TRACE_LEVEL_ERROR,string( "AgUtilsLocal::validate_mapset::Problem Port =")+fportinteger);
                            }
                        }
                    }
                }
            }
        }

        for(iter = fPorts.begin(); iter != fPorts.end(); iter++) {
            AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FCOE, fportinteger);
            if(fportinteger <= MAX_AG_PORT) {
                if ((BM_isBitNSet(fportinteger, bitmap))) {
                    trace (TRACE_LEVEL_ERROR, string("AgUtilsLocal::validate_mapset : F Port already mapped : ") + *iter);
                    return AG_FPORT_ALREADY_MAPPED;
                }
            }
        }

       if(pSyncQueryCtx)
           delete pSyncQueryCtx;

       return AG_VALIDATION_SUCCESS;
    }


    ResourceId AgUtilsLocal::validate_staticmapadd(string nPort, vector<string> fPorts) {
        string nport_interface_converted;
        int nport_cf;
        vector<string> map_fports;
        vector<string>::iterator iter, iter1;
        string fport_temp;
        BitMap_t bitmap = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        int fportinteger, mid = AgUtils::getLocalMappedId();
        LocationId loc_id;

        //Validations
        trace (TRACE_LEVEL_DEBUG, string("AgUtils::validate_staticmapadd : N port value = ")+nPort);
        for(iter = fPorts.begin(); iter != fPorts.end(); iter++) {
            trace (TRACE_LEVEL_DEBUG,string( "AgUtils::validate_staticmapadd : Value of F port : ")+*iter);
        }

        //make sure the nport is valid
        if(!(AgUtils::isInterfaceNportLocked(nPort))) {
            trace (TRACE_LEVEL_ERROR, "Validation error : Check N port value !!\n");
            return AG_INVALID_NPORT;
        }

        //make sure the fports list is valid
        for(iter = fPorts.begin(); iter != fPorts.end(); iter++) {
            if(!AgUtils::isFcoeInterfaceValid(*iter)) {
                trace (TRACE_LEVEL_ERROR, "AgUtils::validate_staticmapadd::Check F port values !\n");
                return AG_INVALID_FPORT;
            }
        }

        //check if the Fport is statically mapped to any other N port
        for(nport_cf = 0;nport_cf < MAX_AG_PORT; nport_cf ++) {
            trace (TRACE_LEVEL_DEBUG, string("AgUtils::validate_staticmapadd : Retrieving Fports for N port value = ")+nport_cf);

            AgUtils::getInterfaceIdFromSwitchPort(mid, AG_INTERFACE_TYPE_FI, nport_cf, nport_interface_converted);

            trace (TRACE_LEVEL_DEBUG, string("AgUtils::validate_staticmapadd : Retrieving Fports for N port value = ")+nport_interface_converted);

            if(AgUtils::isInterfaceNportLocked(nport_interface_converted)) {
                if(nport_interface_converted != nPort) {

                    trace (TRACE_LEVEL_DEBUG, string("AgUtils::validate_staticmapadd : Not equal, so for N port  = ")+nport_interface_converted);

                    pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());
                    pSyncQueryCtx->addAndAttribute (new AttributeString (nport_interface_converted, "agNPortNb"));

                    loc_id = FrameworkToolKit::getThisLocationId();
                    pSyncQueryCtx->setLocationIdFilter(loc_id);
                    pResults = querySynchronously (pSyncQueryCtx);

                    if (NULL != pResults && pResults->size()) {
                        trace (TRACE_LEVEL_DEBUG, string("AgUtils::validate_staticmapadd : Not NULL = ")+nport_interface_converted);

                        pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
                        map_fports = pAgNPortMapLocalManagedObject->getStaticNFPortTopo();

                        for(iter = map_fports.begin(); iter != map_fports.end(); iter++) {
                            trace (TRACE_LEVEL_DEBUG, string("AgUtils::validate_staticmapadd : Fports are ")+*iter);

                            AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FCOE, fportinteger);

                            if(fportinteger <= MAX_AG_PORT) {
                                BM_SetBitN(fportinteger, bitmap);
                            } else {
                                trace (TRACE_LEVEL_ERROR,string( "AgUtils::validate_staticmapadd::Problem Port =")+fportinteger);
                            }
                        }
                    }
                }
            }
        }

        for(iter = fPorts.begin(); iter != fPorts.end(); iter++) {
            AgUtils::getSwitchPortFromInterfaceId(*iter, AG_INTERFACE_TYPE_FCOE, fportinteger);
            if(fportinteger <= MAX_AG_PORT) {
                if ((BM_isBitNSet(fportinteger, bitmap))) {
                    trace (TRACE_LEVEL_ERROR, string("AgUtils::validate_staticmapadd : F Port already statically mapped : ") + *iter);
                    if(pSyncQueryCtx)
                        delete pSyncQueryCtx;

                    return AG_FPORT_ALREADY_MAPPED;
                }
            }
        }

       if(pSyncQueryCtx)
           delete pSyncQueryCtx;

        return AG_VALIDATION_SUCCESS;
    }


    ResourceId AgUtilsLocal::validate_staticmapdel(string nPort, vector<string> fPorts) {
        string nport_interface_converted;
        vector<string> map_fports;
        vector<string>::iterator iter, iter1;
        string fport_temp, delete_port;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
        vector<WaveManagedObject *> *pResults = NULL;
        LocationId loc_id;

        //Validations
        trace (TRACE_LEVEL_DEBUG, string("AgUtils::validate_staticmapadd : N port value = ")+nPort);
        for(iter = fPorts.begin(); iter != fPorts.end(); iter++) {
            trace (TRACE_LEVEL_DEBUG,string( "AgUtils::validate_staticmapadd : Value of F port : ")+*iter);
        }

        //make sure the nport is valid
        if(!(AgUtils::isInterfaceNportLocked(nPort))) {
            trace (TRACE_LEVEL_ERROR, "Validation error : Check N port value !!\n");
            return AG_INVALID_NPORT;
        }

        //make sure the fports list is valid
        for(iter = fPorts.begin(); iter != fPorts.end(); iter++) {
            if(!AgUtils::isFcoeInterfaceValid(*iter)) {
                trace (TRACE_LEVEL_ERROR, "AgUtils::validate_staticmapadd::Check F port values !\n");
                return AG_INVALID_FPORT;
            }
        }

        //check if the Fports are statically mapped to the given N port
        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeString (nPort, "agNPortNb"));
        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);

        pResults = querySynchronously (pSyncQueryCtx);

        if (NULL != pResults && pResults->size()) {
            trace (TRACE_LEVEL_DEBUG, string("AgUtils::validate_staticmapadd : Not NULL = ")+nport_interface_converted);

            pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
            map_fports = pAgNPortMapLocalManagedObject->getStaticNFPortTopo();

            for(iter = fPorts.begin(); iter != fPorts.end(); iter++) {
                delete_port = *iter;
                if(find(map_fports.begin(), map_fports.end(), delete_port) == map_fports.end()) {
                    //which means the port is not found in the mapping
                    if(pSyncQueryCtx)
                        delete pSyncQueryCtx;

                    return AG_FPORT_NOT_MAPPED;
                }
            }
        }


       if(pSyncQueryCtx)
           delete pSyncQueryCtx;

        return AG_VALIDATION_SUCCESS;
    }

    int AgUtilsLocal::getNumNports(void) {
        int i = 0, count = 0;

        for(i = 0; i < MAX_PHY_PORT; i++) {
            if(portNportEnabled(i))
                count++;
        }

        trace (TRACE_LEVEL_INFO, string("AgUtilsLocal::getNumNports : num N ports = ")+count);

        return count;
    }


}

