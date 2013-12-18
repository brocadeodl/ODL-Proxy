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
 *   Author : rhkumar                                                      *
 **************************************************************************/
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/AG/AgClientConfigAllMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "AG/Common/AgPluginTypes.h"

namespace DcmNs
{

    AgClientConfigAllMessage::AgClientConfigAllMessage ()
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_CONFIG_ALL)
    {
    }

    AgClientConfigAllMessage::~AgClientConfigAllMessage ()
    {
    }

    AgClientConfigAllMessage::AgClientConfigAllMessage (const vector<string> &policy, const bool &fportTrunkEnable, const UI32 &fnmtov, const UI32 &relcounter)
        : DcmManagementInterfaceMessage (AG_CLIENTMSG_CONFIG_ALL),
        m_policy       (policy),
        m_fportTrunkEnable  (fportTrunkEnable),
        m_fnmtov(fnmtov),
        m_relcountervalue(relcounter)
    {
        trace(TRACE_LEVEL_DEBUG, "AgClientConfigAllMessage::AgClientConfigAllMessage... constructor 2");
    }

    void  AgClientConfigAllMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeStringVector(&m_policy, "policy"));
        addSerializableAttribute (new AttributeBool(&m_fportTrunkEnable, "fportTrunkEnable"));
        addSerializableAttribute (new AttributeUI32(&m_fnmtov, "fnmtovalue"));
        addSerializableAttribute (new AttributeUI32(&m_relcountervalue, "reliabilityvalue"));

        addSerializableAttribute (new AttributeStringVector(&m_nPort, "nPort"));
        addSerializableAttribute (new AttributeUI32Vector(&m_offset, "offset"));
        addSerializableAttribute (new AttributeStringVector(&m_fPortTopo, "fPortTopo"));
        addSerializableAttribute (new AttributeBoolVector(&m_failover, "failover"));
        addSerializableAttribute (new AttributeBoolVector(&m_failback, "failback"));

        addSerializableAttribute (new AttributeUI32Vector(&m_pgId, "pgId"));
        addSerializableAttribute (new AttributeBoolVector(&m_pgEnable, "pgEnable"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pgFlags, "pgFlags"));
        addSerializableAttribute (new AttributeStringVector(&m_pgName, "pgName"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pgNPortOffset, "pgNPortOffset"));
        addSerializableAttribute (new AttributeStringVector(&m_pgNPortTopo, "pgNPortTopo"));
        addSerializableAttribute (new AttributeUI32Vector(&m_pgFPortOffset, "pgFPortOffset"));
        addSerializableAttribute (new AttributeStringVector(&m_pgFPortTopo, "pgFPortTopo"));
    }

    void AgClientConfigAllMessage::addNportMap(const string &nPort, const vector<string> &fPortTopo, const bool &failover, const bool &failback)
    {
        FILE *mapping;
        vector<string>::const_iterator iter;
        string temp;
        mapping = fopen("/var/log/client_message_mapping", "a");

        fprintf(mapping, "Adding N port map for nport : %s\n", nPort.c_str());
        m_nPort.push_back(nPort);

        fprintf(mapping, "    The F ports are :\n");
        for(iter = fPortTopo.begin();iter != fPortTopo.end(); iter++) {
            temp = *iter;
            fprintf(mapping, "\t%s", temp.c_str());
        }
        fprintf(mapping, "\n");
        m_fPortTopo.insert(m_fPortTopo.end(), fPortTopo.begin(), fPortTopo.end());

        // Offset indicates the number of Fports for a given Nport in the FportTopo vector
        // Thats the reason why offset is updated after updating the Fporttopo vector
        fprintf(mapping, "The offset is :%d\n", (int)m_fPortTopo.size());
        m_offset.push_back(m_fPortTopo.size());

        m_failover.push_back(failover);
        fprintf(mapping, "The failover is :%d\n", (int)failover);
        m_failback.push_back(failback);
        fprintf(mapping, "The failback is :%d\n", (int)failback);

        fclose(mapping);
    }

    void AgClientConfigAllMessage::addPortGroup(const UI32 &pgId, const bool &pgEnable, const UI32 &pgFlags, const string &pgName, 
              const vector<string> &pgNPortTopo, const vector<string> &pgFPortTopo)
    {
        FILE *pgs;
        vector<string>::const_iterator iter;
        pgs = fopen("/var/log/client_message_pgs", "a");
        string temp;

        fprintf(pgs, "The pgid is :%d\n", (int)pgId);
        m_pgId.push_back(pgId);

        fprintf(pgs, "The enable is :%d\n", (pgEnable)?1:0);
        m_pgEnable.push_back(pgEnable);

        fprintf(pgs, "The flags is :%lu\n", (unsigned long)pgFlags);
        m_pgFlags.push_back(pgFlags);

        fprintf(pgs, "The pgname is :%s\n", pgName.c_str());
        m_pgName.push_back(pgName);

        fprintf(pgs, "    The N ports are :\n");
        for(iter = pgNPortTopo.begin();iter != pgNPortTopo.end(); iter++) {
            temp = *iter;
            fprintf(pgs, "\t%s", temp.c_str());
        }
        m_pgNPortTopo.insert(m_pgNPortTopo.end(), pgNPortTopo.begin(), pgNPortTopo.end());
        fprintf(pgs, "\nThe numberof N ports are :%d\n", (int)m_pgNPortTopo.size());
        m_pgNPortOffset.push_back(m_pgNPortTopo.size());

        fprintf(pgs, "\n    The F ports are :\n");
        for(iter = pgFPortTopo.begin();iter != pgFPortTopo.end(); iter++) {
            temp = *iter;
            fprintf(pgs, "\t%s", temp.c_str());
        }
        m_pgFPortTopo.insert(m_pgFPortTopo.end(), pgFPortTopo.begin(), pgFPortTopo.end());
        fprintf(pgs, "\nThe numberof F ports are :%d\n", (int)m_pgFPortTopo.size());
        m_pgFPortOffset.push_back(m_pgFPortTopo.size());

        fclose(pgs);
    }

    void AgClientConfigAllMessage::updatePortBitmap(BitMap_t bm, vector<string> strVec, UI32 offsetBegin, UI32 offsetEnd)
    {
        UI32 index = offsetBegin;
        UI32 port;

        for (; index < offsetEnd; ++index)
        {
            port = atoi(strVec[index].c_str());
            BM_SetBitN(port, bm);
        }
    }

    const void * AgClientConfigAllMessage::getCStructureForInputs()
    {
        ag_config_all_msg_t *msg_p = NULL;
        UI32 nportsSize, pgSize, msgSize, nbNports, nbPgs, count;
        UI32 nPortRecSize, pgRecSize, portOffset, fportOffset;
        nportmap_rec_t *nPortPtr;
        pgmap_rec_t *pgPtr;
        string policy;
        vector<string>::iterator iter;
        //FILE *fd;// For debug, uncomment the 'fd' related code


        //fd = fopen("/var/log/ag_dcm.log", "a");
        // Find the total size of the message
        nbNports = m_nPort.size();
        nPortRecSize = sizeof(nportmap_rec_t) + SIZEOF_BM(MAX_AG_PORT);
        nportsSize = nPortRecSize * nbNports;
        nbPgs = m_pgId.size();
        pgRecSize = sizeof(pgmap_rec_t) + SIZEOF_BM(MAX_AG_PORT) + SIZEOF_BM(MAX_AG_PORT);
        pgSize = pgRecSize * nbPgs;
        msgSize = sizeof (ag_config_all_msg_t) + nportsSize + pgSize;
        //fprintf(fd, "Nports: %d Nportsize %d SIZEOF_BM(MAX_AG_PORT): %d\n", nbNports, nportsSize, SIZEOF_BM(MAX_AG_PORT));
        //fprintf(fd, "nbPgs: %d pgSize: %d msgSize: %d\n", nbPgs, pgSize, msgSize);

        //Allocate the entire message now
        msg_p = (ag_config_all_msg_t *) calloc (msgSize, 1);

        //Update the AG info
        msg_p->msg_size = msgSize;

        AG_POLICY_CLEAR_BITMAP(msg_p->ag_cfg.policy);
        msg_p->ag_cfg.policy = AG_POLICY_PG;
        for(iter = m_policy.begin(); iter < m_policy.end(); iter++) {
            policy = *iter;
            if (!strcasecmp(policy.c_str(), "ads"))
                msg_p->ag_cfg.policy |=  AG_POLICY_ADS;
        }
        msg_p->ag_cfg.fporttrunkenable = m_fportTrunkEnable;
        msg_p->ag_cfg.fnmtov = m_fnmtov;
        msg_p->ag_cfg.relcountervalue = m_relcountervalue;


        //Update NPortmap Records
        msg_p->num_nportmap_rec = nbNports;
        msg_p->nport_prop = (nportmap_rec_t *)((char *)msg_p + sizeof(ag_config_all_msg_t));
        fportOffset = 0;
        //fprintf(fd, "Portprop Starting address: %d\n", (int)msg_p->nport_prop);
        for (count = 0; count < nbNports; ++count)
        {
            nPortPtr = (nportmap_rec_t *)((char *)msg_p->nport_prop + (count * nPortRecSize));
            //fprintf(fd, "[%d] nport address: %d\n", count, (int)&nPortPtr->nport);
            nPortPtr->nport = atoi(m_nPort[count].c_str());
            nPortPtr->failover = m_failover[count];
            nPortPtr->failback = m_failback[count];
            // Initialize the fports pointer to the bitmap address
            nPortPtr->fports = (BitMap_t)((char *)nPortPtr + sizeof (nportmap_rec_t));
            //fprintf(fd, "Portprop fports address: %d fportOffset: %d\n", (int)nPortPtr->fports, fportOffset);
            updatePortBitmap(nPortPtr->fports, m_fPortTopo, fportOffset, m_offset[count]);
            fportOffset = m_offset[count];
        }

        msg_p->num_pg_rec = nbPgs;
        msg_p->pg_prop = (pgmap_rec_t *)((char *) msg_p->nport_prop + nportsSize);
        portOffset = 0;
        fportOffset = 0;
        //fprintf(fd, "PG prtop Starting address: %d\n", (int)msg_p->pg_prop);
        for (count = 0; count < nbPgs; ++count)
        {
            pgPtr = (pgmap_rec_t *)((char *)msg_p->pg_prop + (count * pgRecSize));
            //fprintf(fd, "[%d] PGID address: %d\n", count, (int)&pgPtr->pgid);
            pgPtr->pgid = m_pgId[count];
            pgPtr->pgenable = m_pgEnable[count];
            pgPtr->pgflags = m_pgFlags[count];

            m_pgName[count].copy(pgPtr->pgname, sizeof(pgPtr->pgname), 0);

            pgPtr->nports = (BitMap_t)((char *)pgPtr + sizeof (pgmap_rec_t));
            pgPtr->fports = (BitMap_t)((char *)pgPtr->nports + SIZEOF_BM(MAX_AG_PORT));
            //fprintf(fd, "nports address %d portOffset %d fports address: %d fportOffset: %d\n", 
                         //(int)pgPtr->nports, portOffset, (int)pgPtr->fports, fportOffset);
            updatePortBitmap(pgPtr->nports, m_pgNPortTopo, portOffset, m_pgNPortOffset[count]);
            portOffset = m_pgNPortOffset[count];

            updatePortBitmap(pgPtr->fports, m_pgFPortTopo, fportOffset, m_pgFPortOffset[count]);
            fportOffset = m_pgFPortOffset[count];
        }

//        fclose(fd);
        return msg_p;
    }

    void
    AgClientConfigAllMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        ag_config_msg_resp_t *resp_p = (ag_config_msg_resp_t *)pOutputCStructure;

        if (resp_p)
            addBuffer(resp_p->cmd_code, resp_p->size, (unsigned char *)resp_p, false);

        return;
    }



}
