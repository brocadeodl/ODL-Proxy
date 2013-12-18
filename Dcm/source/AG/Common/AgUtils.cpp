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
#include "PortCfg/Local/PortCfgPortCfgPortTranslateMessage.h"
#include "PortCfg/Local/PortCfgFCPortInterfaceInfoMessage.h"


namespace DcmNs
{

    bool AgUtils::isAgModeEnabled(void)
    {
        char    agFname[PATH_MAX];
        int     agMode = 0;

        snprintf(agFname, PATH_MAX-1, "%s.%d", FOS_AG_FNAME, MYSWITCH);

        if (access(agFname, F_OK) == 0) {
                agMode = 1;
        }

        return (agMode);
    }

    vector<string> AgUtils::bitmap_to_vectorstring(const BitMap_t &bitmap, agInterfaceType interfaceType) {
        vector<string> return_vector;
        int localmappedid = AgUtils::getLocalMappedId();
        string string_var;

        for(int i =0;i<MAX_AG_PORT;i++)
        {
            if ((BM_isBitNSet(i, bitmap))) {
                AgUtils::getInterfaceIdFromSwitchPort(localmappedid, interfaceType, i, string_var);

                return_vector.push_back(string_var);            
            }
        }

        return return_vector;

    }


/* 
   `in_string` should be a vector of string interfaces(three tuple format). 
   This converts the three tuple to switch port
   and sets the bitmap accordingly. The user should also assign memory for 
   BitMap_t string_bit else this will return false.
*/
    bool AgUtils::string_to_bitmap(vector<string> in_string, BitMap_t string_bit, agInterfaceType interfaceType) {
        int port;    
        vector<string>::iterator iter;
        string temp;

        if(string_bit) {
            for (iter = in_string.begin(); iter < in_string.end(); iter++) {
                temp = *iter;
                if((AgUtils::getSwitchPortFromInterfaceId(temp, interfaceType, port))!=-1) {
                    trace (TRACE_LEVEL_DEBUG, string("AgUtils::string_to_bitmap : Converted from ")+temp+string(" to ")+port);
                    if(port <= MAX_AG_PORT) {
                        BM_SetBitN(port, string_bit);
                    }
                } else {
                    trace (TRACE_LEVEL_DEBUG, string("AgUtils::string_to_bitmap : Conversion is -1 in string_to_bitmap!!"));
                }
            }
        } else {
            return false;
        }

        return true;
    }

   vector<string> AgUtils::add_fports_to_nport(vector<string> in_string, const string &nport_string) {
        BitMap_t string_bit = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        int fport;
        vector<string> fports;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
        vector<string>::iterator iter;
        //bool val;
        string temp;
        LocationId loc_id;
    
       /* The reasons behind the logic of converting from vector of strings to bitmap
          and back to vector of strings is to eliminate dupicates from input, also keep 
          the input sorted, to make the process of getting the existing ports from the MO 
          and add the new ports to the existing ones easier, and finally give back a 
          vector of strings to store in the MO.*/

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeString (nport_string, "agNPortNb"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx); 
        if (NULL != pResults && pResults->size()) {             
            pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject *>((*pResults)[0]);
            fports = pAgNPortMapLocalManagedObject->getNFPortTopo();
            //val = string_to_bitmap(fports, string_bit, AG_INTERFACE_TYPE_FCOE);
        }

        for (iter = in_string.begin(); iter < in_string.end(); iter++) {
            temp = *iter;
            if((AgUtils::getSwitchPortFromInterfaceId(temp, AG_INTERFACE_TYPE_FCOE, fport)) != -1) {
                if(fport<=MAX_AG_PORT) {
                    BM_SetBitN(fport, string_bit);
                }
            } else {
                trace (TRACE_LEVEL_ERROR, string("AgUtils::add_fports_to_nport : Conversion is -1 in add_ports!!"));
            }
        }

        in_string = bitmap_to_vectorstring(string_bit, AG_INTERFACE_TYPE_FCOE);
        trace (TRACE_LEVEL_DEBUG, string("AgUtils::add_fports_to_nport : In add_fports_to_nport : "));
        for (iter = in_string.begin(); iter < in_string.end(); iter++) {
             trace (TRACE_LEVEL_DEBUG, string(",") + *iter);
        }

        if(pResults) {
            delete(pResults);
        }

        if(pAgNPortMapLocalManagedObject) {
            delete(pAgNPortMapLocalManagedObject);
        }

        if(pSyncQueryCtx) {
            delete(pSyncQueryCtx);
        }

        if(string_bit) {
            free(string_bit);
            string_bit = NULL;
        }

        return in_string;
    }


    vector<string> AgUtils::remove_fports_from_nport(vector<string> in_string, const string &nport_string) {
        BitMap_t string_bit = (BitMap_t)BM_Calloc(1, MAX_AG_PORT);
        int fport;
        vector<string> fports;
        vector<WaveManagedObject *> *pResults = NULL;
        WaveManagedObjectSynchronousQueryContext *pSyncQueryCtx = NULL;
        AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject = NULL;
        vector<string>::iterator iter;
        //bool val;
        string temp;
        LocationId loc_id;

       /* The reasons behind the logic of converting from vector of strings to bitmap
          and back to vector of strings is to eliminate dupicates from input, also keep 
          the input sorted, to make the process of getting the existing ports from the MO 
          and deleting the new ports from the existing ones easier, and finally give back a 
          vector of strings to store in the MO.*/

        pSyncQueryCtx = new WaveManagedObjectSynchronousQueryContext(AgNPortMapLocalManagedObject::getClassName());
        pSyncQueryCtx->addAndAttribute (new AttributeString (nport_string, "agNPortNb"));

        loc_id = FrameworkToolKit::getThisLocationId();
        pSyncQueryCtx->setLocationIdFilter(loc_id);
        pResults = querySynchronously (pSyncQueryCtx);
        if (NULL != pResults && pResults->size()) {
            pAgNPortMapLocalManagedObject = dynamic_cast<AgNPortMapLocalManagedObject* >((*pResults)[0]);
            fports = pAgNPortMapLocalManagedObject->getNFPortTopo();
            //val = string_to_bitmap(fports, string_bit, AG_INTERFACE_TYPE_FCOE);
        }

        for (iter = in_string.begin(); iter < in_string.end(); iter++) {
            temp = *iter;
            if((AgUtils::getSwitchPortFromInterfaceId(temp, AG_INTERFACE_TYPE_FCOE, fport)) != -1) {
                if(fport <= MAX_AG_PORT) {
                    BM_ClrBitN(fport, string_bit);
                }
            } else {
                trace (TRACE_LEVEL_ERROR, string("AgUtils::remove_fports_from_nport : Conversion returned -1 in remove_fports!!"));
            }
        }

        in_string = bitmap_to_vectorstring(string_bit, AG_INTERFACE_TYPE_FCOE);
        trace (TRACE_LEVEL_DEBUG, string("AgUtils::remove_fports_from_nport : In remove_fports_from_nport : "));
        for (iter = in_string.begin(); iter < in_string.end(); iter++) {
             trace (TRACE_LEVEL_DEBUG, string(",") + *iter);
        }

        if(pResults) {
            delete(pResults);
        }

        if(pAgNPortMapLocalManagedObject) {
            delete(pAgNPortMapLocalManagedObject);
        }

        if(pSyncQueryCtx) {
            delete(pSyncQueryCtx);
        }

        if(string_bit) {
            free(string_bit);
            string_bit = NULL;
        }

        return in_string;
    }

    /* At certain stages of start-up, the API DcmToolKit::getLocalMappedId does not return
     * the correct rbridge-id value. Hence, we have a wrapper API to handle such a case,
     * and read the rbridge-id value directly from the Config file
     */
    int AgUtils::getLocalMappedId(void)
    {
        int mappedId = DcmToolKit::getLocalMappedId();
        trace (TRACE_LEVEL_DEBUG, string ("AgUtils::getLocalMappedId: getLocalMappedId switch id = ") + mappedId);

        /* Check whether the API DcmToolKit::getLocalMappedId returned a good value */
        if (mappedId == 0) {
            configInit ();
            if (configGet("fabric.domain", ResourceInt, &mappedId) != 0) {
                trace (TRACE_LEVEL_ERROR, "AgUtils::getLocalMappedId: configGet of fabric.domain key failed.");
                /* Local Switch as Default */
                mappedId = 1;
            }
            trace (TRACE_LEVEL_DEBUG, string ("AgUtils::getLocalMappedId: configGet of fabric.domain key value = ") + mappedId);
        }

        return (mappedId);
    }


    /*
     * translateInterfacePort
     *
     * Translates Interface to Switchport number and vice versa
     * Interfaces are of format 1/0/1. Point to note is that both TE and FC interfaces numbering
     * start from 1/0/1, 1/0/2 so on. Thus, the interface id is not unique. So, while translating
     * interface to a switch port, the  API setType("Fi") is used to indicate its a FC interface.
     * Whereas, switch port numbers are the FOS style port numbers which start from 0 and count
     * upto number of user ports supported. The switch port# are unique.
     *
     */

    int AgUtils::translateInterfacePort(string &ifname, agInterfaceType interfaceType, int &switchPort, int &mappedId)
    {
        int ret = -1, locationId;
        PortCfgPortCfgPortTranslateMessage *pPortCfgMsg = NULL;
        ResourceId status;
        string intfType;

        pPortCfgMsg = new PortCfgPortCfgPortTranslateMessage(ifname, switchPort, mappedId);
        if (pPortCfgMsg == NULL)
        {
            trace (TRACE_LEVEL_DEBUG, "AgUtils::translateInterfacePort pPortCfgMsg is NULL");
            return (-1);
        }

        /* If we are trying to translate interface to switch port, then we need to update the Type field
         * to differentiate b/w FC and TE interfaces
         */
        switch(interfaceType)
        {
            case AG_INTERFACE_TYPE_FI:
                intfType.assign("Fi");
                break;

            case AG_INTERFACE_TYPE_TE:
                intfType.assign("Te");
                break;

            case AG_INTERFACE_TYPE_FCOE:
                intfType.assign("Fcoe");
                break;

            default: // Error scenario
                trace(TRACE_LEVEL_DEBUG, string("AgUtils::translateInterfacePort unknown interface type:") + interfaceType);
                delete pPortCfgMsg;
                return (-1);
        }

        /* Update the request only if we are translating from interface to switchport */
        if (!ifname.empty())
        {
            pPortCfgMsg->setType(intfType);
        }

        if ((locationId = DcmToolKit::getLocationIdFromMappedId (mappedId)) == -1)
        {
            trace(TRACE_LEVEL_DEBUG, string("AgUtils::translateInterfacePort locationId is -1 for mappedId: ") + mappedId);
            delete pPortCfgMsg;
            return (-1);
        }
        status = sendSynchronously (pPortCfgMsg, locationId);

        if (status == WAVE_MESSAGE_SUCCESS)
        {
            ResourceId completionStatus = pPortCfgMsg->getCompletionStatus();
            if (completionStatus == WAVE_MESSAGE_SUCCESS)
            {
                /* PortCfgPortCfgPortTranslateMessage is capable of translating Interface to switchPort
                 * and viceversa. Thus at the end of translation, both Id and switchPort should be updated
                 * with valid values. Else its a translation error. Also, in case of switchport to interface
                 * translation, ensure that the returned interface is of the requested type
                 */
                if ((pPortCfgMsg->getSwitchPort() != -1) &&
                    (!pPortCfgMsg->getId().empty()) &&
                    (strcasecmp(pPortCfgMsg->getType().c_str(), intfType.c_str()) == 0))
                {
                    trace(TRACE_LEVEL_DEBUG, string("AgUtils::translateInterfacePort success ifname: ") + ifname);
                    trace(TRACE_LEVEL_DEBUG, string("AgUtils::translateInterfacePort success switchPort: ") + pPortCfgMsg->getSwitchPort());
                    switchPort = pPortCfgMsg->getSwitchPort();
                    ifname = pPortCfgMsg->getId();
                    ret = 0; 
                }
                else
                {
                    trace(TRACE_LEVEL_DEBUG, "AgUtils::translateInterfacePort port translate failed! :: " + pPortCfgMsg->getId());
                }
            }
            else
            {
                trace(TRACE_LEVEL_DEBUG, string("AgUtils::translateInterfacePort completion status error: ") + completionStatus);
            }
        }
        else
        {
            trace(TRACE_LEVEL_DEBUG, string("AgUtils::translateInterfacePort sendSynchronously failed: ") + status);
        }

        delete pPortCfgMsg;
        return ret;
    }


    bool AgUtils::isInterfaceStringValid(string &ifname)
    {
        bool valid = false;
        //string  delimiter = "/";

        // Check if VCS is enabled, AG should be enabled only after VCS enabled
        if (NsmUtils::isValidThreeTuple(ifname) && (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED))
        {
            /* Find the slot number from a 3-tuple value 1/0/1, where 0 represents slot number.
             * Usually AG boxes have slot number as zero. Plus, in hercules branch,
             * the portCfg plugin uses slot number 256 to identify ISLs. Thus, we want to ensure
             * that we consider interfaces with slot number 0 as valid ones and discard interfaces
             * with other slot numbers.
             */
            /* Fcoe ports have slot field value 1, thus slot number checking is commented for now.
            size_t first_pos = ifname.find_first_of(delimiter, 0);
            if (first_pos != string::npos)
            {
                 size_t second_pos = ifname.find_first_of(delimiter, (first_pos + 1));
                 if (second_pos != string::npos)
                 {
                     string slotStr = ifname.substr(first_pos + 1, second_pos - first_pos);
                     int slotnum = atoi(slotStr.c_str());
                     if (slotnum == 0) {
                         valid = true;
                     }
                 }
            }*/
            valid = true;
        }

        return valid;
    }

    int AgUtils::getMappedIdFromFcoeIfName(string ifname)
    {
        string  delimiter = "/";
        int mappedId = -1;

        /* Fcoe interface-id are of format vcs-id/rbridge-id/port_num
         * Fetch the rbridge-id from such an interface-id
         */
        size_t first_pos = ifname.find_first_of(delimiter, 0);
        if (first_pos != string::npos)
        {
             size_t second_pos = ifname.find_first_of(delimiter, (first_pos + 1));
             if (second_pos != string::npos)
             {
                 string mappedIdStr = ifname.substr(first_pos + 1, second_pos - first_pos);
                 mappedId = atoi(mappedIdStr.c_str());
             }
        }
        trace(TRACE_LEVEL_DEBUG, string("AgUtils::getMappedIdFromFcoeIfName mappedId for IF: ") + ifname + string(" is:") + mappedId);

        return mappedId;
    }

    int AgUtils::getPortNumFromFcoeIfName(string ifname)
    {
        string  delimiter = "/";
        int portNum = -1;

        /* Fcoe interface-id are of format vcs-id/rbridge-id/port_num
         * Fetch the rbridge-id from such an interface-id
         */
        size_t first_pos = ifname.find_first_of(delimiter, 0);
        if (first_pos != string::npos)
        {
             size_t second_pos = ifname.find_first_of(delimiter, (first_pos + 1));
             if (second_pos != string::npos)
             {
                 string portNumStr = ifname.substr(second_pos + 1, ifname.size());
                 portNum = atoi(portNumStr.c_str());
             }
        }
        trace(TRACE_LEVEL_DEBUG, string("AgUtils::getPortNumFromFcoeIfName port_num for IF: ") + ifname + string(" is:") + portNum);

        return portNum;
    }


    int AgUtils::getSwitchPortFromInterfaceId(string &ifname, agInterfaceType interfaceType, int &switchPort)
    {
        int ret = -1, mappedId = -1;

        trace(TRACE_LEVEL_DEBUG, string("AgUtils::getSwitchPortFromInterfaceId Entering.."));
        if (isInterfaceStringValid(ifname)) 
        {
            switch(interfaceType)
            {
                // TE & FI interface-id are of format rbridge-id/slot/port_num
                case AG_INTERFACE_TYPE_FI:
                case AG_INTERFACE_TYPE_TE:
                    mappedId = NsmUtils::getMappedIdFromIfName(ifname);
                    break;

                // Fcoe interface-id are of format vcs-id/rbridge-id/port_num
                case AG_INTERFACE_TYPE_FCOE:
                    mappedId = AgUtils::getMappedIdFromFcoeIfName(ifname);
                    break;

                default:
                    mappedId = -1;
            }

            if(mappedId == -1)
            {
                trace(TRACE_LEVEL_DEBUG, string("AgUtils::getSwitchPortFromInterfaceId mappedId is -1 for IF: ") + ifname);
                return (-1);
            }

            // Set switchPort to -1, to indicate that switchPort needs to be updated
            switchPort = -1;

            ret = translateInterfacePort(ifname, interfaceType, switchPort, mappedId);

        }

        return ret;
    }


    int AgUtils::getInterfaceIdFromSwitchPort(int &mappedId, agInterfaceType interfaceType, int &switchPort, string &ifname)
    {
        int ret = -1;

        // Check if VCS is enabled, AG should be enabled only after VCS enabled
        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED)
        {
            // Clear the ifname, to indicate that ifname needs to be updated
            ifname.clear();

            if ((ret = translateInterfacePort(ifname, interfaceType, switchPort, mappedId)) == 0)
            {
                // Check the ifname, if it is valid from AG configuration point of view
                if (!isInterfaceStringValid(ifname))
                    ret = -1;

            }
        }

        return ret;
    }


    bool AgUtils::isFCInterfaceValid(string &ifname)
    {
        bool valid = false;
        int switchPort, mappedId;

        if (isInterfaceStringValid(ifname))
        {
            mappedId = NsmUtils::getMappedIdFromIfName(ifname);
            if(mappedId == -1)
            {
                trace(TRACE_LEVEL_DEBUG, string("AgUtils::isFCInterfaceValid mappedId is -1 for IF: ") + ifname);
                return false;
            }

            // Set switchPort to -1, to indicate that switchPort needs to be updated
            switchPort = -1;
            if(translateInterfacePort(ifname, AG_INTERFACE_TYPE_FI, switchPort, mappedId) != -1)
                valid = true;
        }
        return valid;
    }

    bool AgUtils::isFcoeInterfaceValid(string &ifname)
    {
        bool valid = false;
        int switchPort, mappedId;

        if (isInterfaceStringValid(ifname))
        {
            mappedId = AgUtils::getMappedIdFromFcoeIfName(ifname);
            if(mappedId == -1)
            {
                trace(TRACE_LEVEL_DEBUG, string("AgUtils::isFcoeInterfaceValid mappedId is -1 for IF: ") + ifname);
                return false;
            }

            // Set switchPort to -1, to indicate that switchPort needs to be updated
            switchPort = -1;
            if(translateInterfacePort(ifname, AG_INTERFACE_TYPE_FCOE, switchPort, mappedId) != -1)
                valid = true;
        }
        return valid;
    }


    bool AgUtils::isInterfaceNportLocked(string &ifname)
    {
        int mappedId, switchPort;
        bool nportLock = false;
         
        if (isInterfaceStringValid(ifname))
        {
            mappedId = NsmUtils::getMappedIdFromIfName(ifname);
            if(mappedId == -1)
            {
                trace(TRACE_LEVEL_DEBUG, string("AgUtils::isInterfaceNportLocked mappedId is -1 for IF: ") + ifname);
                return false;
            }

            // Set switchPort to -1, to indicate that switchPort needs to be updated
            switchPort = -1;
            if(translateInterfacePort(ifname, AG_INTERFACE_TYPE_FI, switchPort, mappedId) != -1)
            {
                // Get FC port properties
                PortCfgFCPortInterfaceInfoMessage  *pPortCfgFCMsg = NULL;
                ResourceId status;
                string intfType;
                int locationId = -1;

                pPortCfgFCMsg = new PortCfgFCPortInterfaceInfoMessage();
                if (pPortCfgFCMsg == NULL)
                {
                    trace (TRACE_LEVEL_ERROR, "AgUtils::isInterfaceNportLocked pPortCfgFCMsg is NULL");
                    return (false);
                }
                pPortCfgFCMsg->setPortId(ifname);
                pPortCfgFCMsg->setSwitchPort(switchPort);

                if ((locationId = DcmToolKit::getLocationIdFromMappedId (mappedId)) == -1)
                {
                    trace(TRACE_LEVEL_DEBUG, string("AgUtils::isInterfaceNportLocked locationId is -1 for mappedId: ") + mappedId);
                    delete pPortCfgFCMsg;
                    return (false);
                }
                status = sendSynchronously (pPortCfgFCMsg, locationId);

                if (status == WAVE_MESSAGE_SUCCESS)
                {
                    ResourceId completionStatus = pPortCfgFCMsg->getCompletionStatus();
                    if (completionStatus == WAVE_MESSAGE_SUCCESS)
                    {
                        trace(TRACE_LEVEL_INFO, string("AgUtils::isInterfaceNportLocked IF: ") + ifname
                                       + string(" portType: ") + pPortCfgFCMsg->getPortType());
                        if (strcasecmp(pPortCfgFCMsg->getPortType().c_str(), "N-Port") == 0)
                        {
                            nportLock = true;
                        }
                    }
                }

                delete pPortCfgFCMsg;
            }
        }

        return nportLock;
    }

    int  AgUtils::stringToWwn(const char *s, wwn_t *w)
    {
        unsigned int p[8];
        int i, n = 1;
        const char *tmp;

       if ((s == NULL) || (w == NULL))
            return(-1);

        tmp = s;
        while (*tmp != '\0') {
            if (!isdigit(*tmp)) {
                if ((*tmp == ':') ||
                    // ((*tmp >= 'A') && (*tmp <= 'F')) || Not allowing upper case in WWN names 
                    ((*tmp >= 'a') && (*tmp <= 'f'))) {
                    // Do nothing, proper values
                } else {
                    return (-1);
                }
             }
             tmp++;
         }

        if (sscanf(s, "%x:%x:%x:%x:%x:%x:%x:%x%n",
            p, p+1, p+2, p+3, p+4, p+5, p+6, p+7, &n) != 8) {
            return (-1);
        }

        for (i = 0; i < 8; i++) {
            if (p[i] < 0 || p[i] > 0xff) {
                return (-1);
            }
            if (w)
                w->chars[i] = p[i];
        }

        return (0);
    }


    void AgUtils::removeCaseString(string str, vector<string> &str_vec)
    {
        vector<string>::iterator iter;

        for (iter = str_vec.begin(); iter != str_vec.end(); iter++) {
            if (strcasecmp((*iter).c_str(), str.c_str()) == 0) {
                trace (TRACE_LEVEL_DEBUG, string("AgUtils::removeCaseString : string to erase1!\n") + str);
                iter = str_vec.erase(iter);
                if (iter == str_vec.end())
                    break;
            }
        }
    }

    bool AgUtils::wwnPresent (string wwn, vector<string> wwns)
    {
        vector<string>::iterator iter;

        trace(TRACE_LEVEL_INFO, string("To check wwn : ")+wwn);

        if (wwn.size() == 0)
            return false;

        for (iter = wwns.begin(); iter != wwns.end(); iter++) {
            trace(TRACE_LEVEL_INFO, string("List value : ") + *iter);
            if (strcasecmp((*iter).c_str(), wwn.c_str()) == 0)
                return true;
        }

        return false;
    }


}
