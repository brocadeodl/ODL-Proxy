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
 *   Copyright (C) 2010-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "VcsFabric/Local/EarlyConfigurationReplayWorker.h"
#include "VcsFabric/Local/EarlyConfigReplayMessagingContext.h"
#include "VcsFabric/Local/VcsNodeSlotMessages.h"
#include "VcsFabric/Local/VcsNodeSlotManagementInterfaceMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/ConnectorConfigLocalMessage.h"
#include "Framework/Utils/ConfigFileManagementToolKit.h"

using namespace std;

namespace DcmNs
{

EarlyConfigurationReplayWorker::EarlyConfigurationReplayWorker (VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager)
    : WaveWorker (pVcsNodeFabosInterfaceObjectManager)
{
}

EarlyConfigurationReplayWorker::~EarlyConfigurationReplayWorker () {
}

EarlyConfigurationReplayWorker *EarlyConfigurationReplayWorker::getInstance ()
{
     static EarlyConfigurationReplayWorker *pEarlyConfigurationReplayWorker = (VcsNodeFabosInterfaceObjectManager::getInstance ())->m_earlyConfigurationReplayWorker;
     WaveNs::prismAssert (NULL != pEarlyConfigurationReplayWorker, __FILE__, __LINE__);
     return pEarlyConfigurationReplayWorker;
} 

string EarlyConfigurationReplayWorker::getClassName () 
{
     return ("EarlyConfigurationReplayWorker");
}

ResourceId EarlyConfigurationReplayWorker::extractStartupFile (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("EarlyConfigurationReplayWorker::extractStartupFile : Entering ..."));
 
 
    string filename = FrameworkToolKit::getStartupFileName ();
    WaveNs::trace (TRACE_LEVEL_INFO, string ("EarlyConfigurationReplayWorker::extractStartupFile : startup file : ") + filename);
 
    fstream startupFile;
    startupFile.open (filename.c_str (), ifstream::in);
 
    vector<string> lines;
    string line = "";
 
    if (!startupFile.is_open ())
    {
        WaveNs::trace (TRACE_LEVEL_WARN, string ("EarlyConfigurationReplayWorker::extractStartupFile : Failed to open: ") + filename);
        return WAVE_MESSAGE_ERROR;
    }
    else
    {
        if (pEarlyConfigReplayMessagingContext->getParsingIncompleteOnFwdlFlag ())
        {
            string cpBackup = "/bin/cp " + filename + " " + FrameworkToolKit::getStartupBackupFileName ();
 
            vector<string> output;
            SI32 ret = FrameworkToolKit::systemCommandOutput (cpBackup.c_str(), output);
 
            if (0 != ret)
            {
                WaveNs::tracePrintf (TRACE_LEVEL_WARN, true, false, "EarlyConfigurationReplayWorker::extractStartupFile: failed to backup the startup config file.");
 
                if (false == output.empty())
                {
                    WaveNs::tracePrintf (TRACE_LEVEL_WARN, true, false, "EarlyConfigurationReplayWorker::extractStartupFile: command [%s] failed with error : [%s]", cpBackup.c_str(), output[0].c_str());
                }
                //prismAssert (false, __FILE__, __LINE__);
                return WAVE_MESSAGE_ERROR;
            }
            else
            {
                WaveNs::tracePrintf (TRACE_LEVEL_INFO, true, false, "EarlyConfigurationReplayWorker::extractStartupFile: successfully took the backup : [%s]", cpBackup.c_str());
            }
        }
        
        while (startupFile.good ())
        {
            getline (startupFile, line);
            lines.push_back (line);
        }
        startupFile.close ();
        pEarlyConfigReplayMessagingContext->setConfigVectorPointer (lines);
        pEarlyConfigReplayMessagingContext->setConfigFileForParsing (FrameworkToolKit::getStartupBackupFileName ());
    }
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId EarlyConfigurationReplayWorker::extractConfigSnapshotFile (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("EarlyConfigurationReplayWorker::extractConfigSnapshotFile : Entering ..."));

    string configFilePath  = ConfigFileManagementToolKit::getConfigFileManagementScriptsDirectory ();
    string configFilename  = ConfigFileManagementToolKit::getConfigFileManagementLocalOnlyFilename();
    
    string filename = configFilePath + "/" + configFilename;
    WaveNs::trace (TRACE_LEVEL_INFO, string ("EarlyConfigurationReplayWorker::extractConfigSnapshotFile : config file : ") + filename);

    fstream configFile;
    configFile.open (filename.c_str (), ifstream::in);

    vector<string> lines;
    string line = "";

    if (!configFile.is_open ())
    {
        WaveNs::trace (TRACE_LEVEL_WARN, string ("EarlyConfigurationReplayWorker::extractConfigSnapshotFile : Failed to open: ") + filename);
        return WAVE_MESSAGE_ERROR;
    }
    else
    {
        while (configFile.good ())
        {
            getline (configFile, line);
            lines.push_back (line);
        }
        configFile.close ();
        pEarlyConfigReplayMessagingContext->setConfigVectorPointer (lines);
    }
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId EarlyConfigurationReplayWorker::writeLinecardConfigurationToDB (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("EarlyConfigurationReplayWorker::writeLinecardConfigurationToDB : Entering ..."));

    string line = "";
    string linecardStr = "linecard";
    string rbridgeIdStr = "rbridge-id";
    size_t pos;
    UI32 rbridgeId = DcmToolKit::getThisVcsNodeDomainId ();
    UI32 slotNumber = 0;

    vector<string> configs = pEarlyConfigReplayMessagingContext->getConfigVectorPointer ();
    UI32 vectorLen = configs.size (); 
    UI32 i = 0;

    while (i < vectorLen)
    {
        line = configs[i];

        if (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED)
        {
            pos = line.find (rbridgeIdStr);

            if (pos != string::npos)
            {
                string tempString = line;
                vector<string>  token;

                tokenize (tempString, token, ' ');

                UI32 numberOfToken = token.size ();

                prismAssert (2 <= numberOfToken, __FILE__, __LINE__);

                if (strcmp (rbridgeIdStr.c_str (), token[0].c_str ()) == 0)
                {
                    string temp = token[1];
                    rbridgeId = atoi (temp.c_str ());

                    WaveNs::trace (TRACE_LEVEL_INFO, string ("EarlyConfigurationReplayWorker::writeLinecardConfigurationToDB : rbridgeId = ") + rbridgeId);
                }
            }
        }

        pos = line.find (linecardStr);

        if (pos != string::npos)
        {
            string tempString = line;
            vector<string>  token;

            tokenize (tempString, token, ' ');

            UI32 numberOfToken = token.size ();

            prismAssert (3 <= numberOfToken, __FILE__, __LINE__);

            string slotId = token[1];
            string slotType = token[2];

            // Since the mode option is currently disabled, it is always default mode
            string mode = "default";
             
            WaveNs::trace (TRACE_LEVEL_INFO, string ("EarlyConfigurationReplayWorker::writeLinecardConfigurationToDB : slotId = ") + slotId + " slotType = " + slotType + " mode = " + mode);

            pos = slotId.find ('/');

            if (pos != string::npos)
            {
                string temp = slotId.substr (0, pos);
                UI32 tempRid = atoi (temp.c_str ());

                if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (rbridgeId != tempRid))
                {
                    WaveNs::trace (TRACE_LEVEL_ERROR, string ("EarlyConfigurationReplayWorker::writeLinecardConfigurationToDB : RbridgeId mismatch - from startup file = ") + tempRid + " compared to local rbridgeId = " + rbridgeId);
                }

                temp = slotId.substr (pos + 1);
                slotNumber = atoi (temp.c_str ());
            }
            else
            {
                slotNumber = atoi (slotId.c_str ());
            }

            VcsNodeSlotConfigMessage *pVcsNodeSlotConfigMessage = new VcsNodeSlotConfigMessage(slotNumber, rbridgeId, getHwIdFromLinecardType (slotType), getLinecardModeFromString (mode));

            ResourceId status = sendSynchronously (pVcsNodeSlotConfigMessage);

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_DEBUG, "EarlyConfigurationReplayWorker::writeLinecardConfigurationToDB : Success.");
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::writeLinecardConfigurationToDB : Failed.  Status : " + FrameworkToolKit::localize (status));
            }
        }
        i++;
    }

    return WAVE_MESSAGE_SUCCESS;
}

/*
 * This function will read the hardware connector configs from startup-file and save them in Db.
 * In Nos4.0.0 release, Sfp breakout configuration are static configs and should be in Db before HASM
 * reads the Db when switch boots up. During file replay, Db does not have sfp breakout configs. Thus
 * needs to put Sfp breakout configs in Db from startup-file, so that HASM can read from Db and program
 * breakout interfaces to ASIC.
 */
ResourceId EarlyConfigurationReplayWorker::writeHardwareConnectorConfigToDB (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{
    WaveNs::trace (TRACE_LEVEL_DEBUG, string ("EarlyConfigurationReplayWorker::writeHardwareConnectorConfigToDB : Entering ..."));

    string line = "";
    string connectorStr = "connector";
    string sfpBreakoutStr = "sfp breakout";
    string noStr = "no";

    size_t pos;
    UI32 connectorRbridgeId = 0;
    UI32 rbridgeId = DcmToolKit::getThisVcsNodeDomainId ();
    string IntfName;
    bool isBreakoutEnable = false;
    vector<string> breakoutConfigs;

    vector<string> configs = pEarlyConfigReplayMessagingContext->getConfigVectorPointer ();
    UI32 vectorLen = configs.size (); 

    for (UI32 i = 0; i < vectorLen; i++) {
        line = configs[i];
        // read connector <intfname> line
        pos = line.find (connectorStr);
        if (pos != string::npos) {
            string tempString = line;
            vector<string>  token;
            tokenize (tempString, token, ' ');
            UI32 numberOfToken = token.size ();
            if (numberOfToken < 2) {
                WaveNs::trace (TRACE_LEVEL_ERROR, "EarlyConfigurationReplayWorker::writeHardwareConnectorConfigToDB : Invalid Connector info.");
                continue;
            }
            if (connectorStr == token[0]) {
                IntfName = string(token[1]);
            }

            // Observed that last character of IntfName is carriage return (ascii 13), hence removing that
            UI32 intfNameSize = IntfName.length();
            if (! isdigit(IntfName[intfNameSize-1])) {
                IntfName = IntfName.substr (0, intfNameSize - 1);
            }
            // If connector rbridgeId does not matches with local node rbridgeId, skip that connector
            if ((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) && NsmUtils::isValidThreeTuple(IntfName)) {
                connectorRbridgeId = (UI32) NsmUtils::getMappedIdFromIfName (IntfName);
                if (connectorRbridgeId != rbridgeId) {
                    continue;
                }
            }

            token.clear();
            // Next line should be sfp breakout line
            if ((i + 1) < vectorLen) {
                line = configs[i + 1];
                pos = line.find (sfpBreakoutStr);
                if (pos != string::npos) {
                    tempString = line;
                    tokenize (tempString, token, ' ');
                    numberOfToken = token.size ();
                    if (numberOfToken == 2) {
                        isBreakoutEnable = true;
                    } else if ((numberOfToken == 3) && (noStr == token[0])) {
                        isBreakoutEnable = false;
                    }
                }
            }
            WaveNs::trace (TRACE_LEVEL_INFO, string ("EarlyConfigurationReplayWorker::writeHardwareConnectorConfigToDB : connector: ") + IntfName + string(" -- breakout enable: ") + string((isBreakoutEnable) ? "True" : "False"));

            // creating string like 111/4/49#1 or 111/4/40#0 where last digit 1 indicates breakout enable and 0 indicates breakout disable
            breakoutConfigs.push_back (IntfName + string("#") + string((isBreakoutEnable) ? "1" : "0"));
        }
    }
    //create message
    ConnectorConfigLocalMessage *pMsg = new ConnectorConfigLocalMessage();
    pMsg->setCmdCode(CONNECTOR_CONFIG_BULK_UPDATE);
    pMsg->setBulkConfigsVector (breakoutConfigs);
    ResourceId status = sendSynchronously (pMsg, DcmToolKit::getLocationIdFromMappedId(rbridgeId));

    if (WAVE_MESSAGE_SUCCESS == status) {
        WaveNs::trace (TRACE_LEVEL_DEBUG, "EarlyConfigurationReplayWorker::writeHardwareConnectorConfigToDB : Success.");
    } else {
        WaveNs::trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::writeHardwareConnectorConfigToDB : Failed.  Status : " + FrameworkToolKit::localize (status));
    }
    delete pMsg;
    pMsg = NULL;

    return WAVE_MESSAGE_SUCCESS;
}

/*
** This function will edit the startup config file and insert rbridge-id block around system-monitor configuration
** It is only applicable when FC is turned ON and firmware upgrade happens between 3.x to 4.x
 */

ResourceId EarlyConfigurationReplayWorker::editSystemMonitorConfig (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{
	trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSystemMonitorConfig Entering..");
	if (!pEarlyConfigReplayMessagingContext->getParsingIncompleteOnFwdlFlag ())
	{
		trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSystemMonitorConfig Do nothing");
		return (WAVE_MESSAGE_SUCCESS);
	}
	if (DcmToolKit::isVcsEnabled () != WAVE_PERSISTENCE_CLUSTER_ENABLED)
	{
		trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSystemMonitorConfig SA enabled");
		return (WAVE_MESSAGE_SUCCESS);
	}
	const char *currVer = DcmToolKit::getCurrentFirmwareVersion().c_str();
	const char *fromVer = DcmToolKit::getPreviousFirmwareVersion().c_str();
	int toMajor = 0, toMinor = 0;
	int fromMajor = 0, fromMinor = 0;
	sscanf (currVer, "%d.%d", &toMajor, &toMinor);
	sscanf (fromVer, "%d.%d", &fromMajor, &fromMinor);

	if (fromMajor >= 4)
	{
		trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSystemMonitorConfig not needed for this version");
		tracePrintf (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSystemMonitorConfig to major..%d",toMajor);
		tracePrintf (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSystemMonitorConfig from major..%d",fromMajor);
		return (WAVE_MESSAGE_SUCCESS);
	}
    vector<string> configs = pEarlyConfigReplayMessagingContext->getConfigVectorPointer ();
	string cFile = pEarlyConfigReplayMessagingContext->getConfigFileForParsing ();
    UI32 rbridgeId = DcmToolKit::getThisVcsNodeDomainId ();
	char rbstr[50];
	memset (rbstr, 0, sizeof (rbstr));
	sprintf (rbstr, "rbridge-id %d",rbridgeId);
	vector <string> new_configs;
	UI32 sz = configs.size();
	UI32 i = 0, sysmon_cnt = 0, new_sz = 0, flag = 0;
	size_t found;
	string line = "";
	/* Token is system-monitor<space>, removing the space will tokenize system-monitor-mail too */
	string sysmon_token = "system-monitor ";
	ofstream configFile;
	while (i < sz)
	{
		line = configs[i];
		found = line.find (sysmon_token);
		if (found != string::npos)
		{
			/* Found system-monitor */
			flag = 1;
		} else {
			/* Exiting system-monitor block. If flag is 2, insert ! */
			if (flag == 1) {
				flag = 2;
			}
		}

		if (flag == 1) {
			if (sysmon_cnt == 0)
			{
				if (i == 0) {
					new_configs.push_back (rbstr);
				}
				else if (configs[i-1].compare(0, 10, "rbridge-id") != 0) {
						new_configs.push_back (rbstr);
				}
			}
			if (line[0] != ' ') {
				line.insert (0, " ");
			}
			new_configs.push_back (line);
			sysmon_cnt++;
		}

		else if (flag == 2) {
			/* Insert ! only if system-monitor block has exited */
			if (sysmon_cnt != 0) 
			{
				if (i == sz - 1)
				{
					new_configs.push_back("!");
					new_configs.push_back("!");
					sysmon_cnt = 0;
				}
				else if (configs[i+1].compare("!") != 0)
				{
					new_configs.push_back("!");
					new_configs.push_back("!");
					sysmon_cnt = 0;
				}
			}
			new_configs.push_back(line);
			flag = 0;
		}
		else
			new_configs.push_back(line);

		i++;
	}
	i = 0;
	new_sz = new_configs.size();
	configFile.open (cFile.c_str());
	while (i < new_sz - 1)
	{
		configFile << new_configs[i];
		configFile << endl;
		i++;
	}
	configFile.close();
    pEarlyConfigReplayMessagingContext->setConfigVectorPointer (new_configs);

	return (WAVE_MESSAGE_SUCCESS);
}

/*
** This function will edit the startup config file and insert rbridge-id block around fcsp configuration
** It is only applicable when FC is turned ON and firmware upgrade happens between 3.x to 4.x
 */
                
ResourceId EarlyConfigurationReplayWorker::editFcspConfig (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{           
    trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editFcspConfig Entering..");
    if (!pEarlyConfigReplayMessagingContext->getParsingIncompleteOnFwdlFlag ())
    {       
        trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editFcspConfig Do nothing");
        return (WAVE_MESSAGE_SUCCESS);    }
    if (DcmToolKit::isVcsEnabled () != WAVE_PERSISTENCE_CLUSTER_ENABLED)
    {       
        trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editFcspConfig SA enabled");
        return (WAVE_MESSAGE_SUCCESS);
    }
    const char *currVer = DcmToolKit::getCurrentFirmwareVersion().c_str();
    const char *fromVer = DcmToolKit::getPreviousFirmwareVersion().c_str();
    int toMajor = 0, toMinor = 0;
    int fromMajor = 0, fromMinor = 0;
    sscanf (currVer, "%d.%d", &toMajor, &toMinor);
    sscanf (fromVer, "%d.%d", &fromMajor, &fromMinor);

    if (fromMajor >= 4)
    {
        trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editFcspConfig not needed for this version");
        tracePrintf (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editFcspConfig to major..%d",toMajor);
        tracePrintf (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editFcspConfig from major..%d",fromMajor);
        return (WAVE_MESSAGE_SUCCESS);
    }
    vector<string> configs = pEarlyConfigReplayMessagingContext->getConfigVectorPointer ();
    string cFile = pEarlyConfigReplayMessagingContext->getConfigFileForParsing ();
    UI32 rbridgeId = DcmToolKit::getThisVcsNodeDomainId ();
    char rbstr[50];
    memset (rbstr, 0, sizeof (rbstr));
    sprintf (rbstr, "rbridge-id %d",rbridgeId);
    vector <string> new_configs;
    UI32 sz = configs.size();
    UI32 i = 0, fcsp_cnt = 0, new_sz = 0, flag = 0;
    size_t found;
    string line = "";
    string fcsp_token = "fcsp ";
    ofstream configFile;
    while (i < sz)
    {
        line = configs[i];
        found = line.find (fcsp_token);
        if (found != string::npos)
        {
            /* Found fcsp */
            flag = 1;
        } else {
            /* Exiting fcsp block. If flag is 2, insert ! */
            if (flag == 1) {
                flag = 2;
            }
        }

        if (1 == flag) {
            if (0 == fcsp_cnt)
            {
                if (i == 0) {
					new_configs.push_back (rbstr);
				} else if (configs[i-1].compare(0, 10, "rbridge-id") != 0) {
                        new_configs.push_back (rbstr);
                }
            }
            if (line[0] != ' ') {
                line.insert (0, " ");
            }
            new_configs.push_back (line);
            fcsp_cnt++;
        }

        else if (2 == flag) {
            /* Insert ! only if fcsp block has exited */
            if (fcsp_cnt != 0)
            {
				if (i == sz - 1) {
					new_configs.push_back("!");
					new_configs.push_back("!");
					fcsp_cnt = 0;
				} else if (configs[i+1].compare("!") != 0) {
					//TODO: should we add two !s
                    new_configs.push_back("!");
                    new_configs.push_back("!");
					fcsp_cnt = 0;
                }
            }
            new_configs.push_back(line);
            flag = 0;
        }
        else
            new_configs.push_back(line);

        i++;
    }
    i = 0;
    new_sz = new_configs.size();
    configFile.open (cFile.c_str());
    while (i < new_sz - 1)
    {
        configFile << new_configs[i];
        configFile << endl;
        i++;
    }
    configFile.close();
    pEarlyConfigReplayMessagingContext->setConfigVectorPointer (new_configs);

    return (WAVE_MESSAGE_SUCCESS);
}

/*
** This function will edit the startup config file and insert rbridge-id block around secpolicy configuration
** It is only applicable when FC is turned ON and firmware upgrade happens between 3.x to 4.x
 */

ResourceId EarlyConfigurationReplayWorker::editSecpolicyConfig (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{
    trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSecpolicyConfig Entering..");
    if (!pEarlyConfigReplayMessagingContext->getParsingIncompleteOnFwdlFlag ())
    {
        trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSecpolicyConfig Do nothing");
        return (WAVE_MESSAGE_SUCCESS);
    }
    if (DcmToolKit::isVcsEnabled () != WAVE_PERSISTENCE_CLUSTER_ENABLED)
    {
        trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSecpolicyConfig SA enabled");
        return (WAVE_MESSAGE_SUCCESS);
    }
    const char *currVer = DcmToolKit::getCurrentFirmwareVersion().c_str();
    const char *fromVer = DcmToolKit::getPreviousFirmwareVersion().c_str();
    int toMajor = 0, toMinor = 0;
    int fromMajor = 0, fromMinor = 0;
    sscanf (currVer, "%d.%d", &toMajor, &toMinor);
    sscanf (fromVer, "%d.%d", &fromMajor, &fromMinor);

    if (fromMajor >= 4)
    {
        trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSecpolicyConfig not needed for this version");
        tracePrintf (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSecpolicyConfig to major..%d",toMajor);
        tracePrintf (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::editSecpolicyConfig from major..%d",fromMajor);
        return (WAVE_MESSAGE_SUCCESS);
    }
    vector<string> configs = pEarlyConfigReplayMessagingContext->getConfigVectorPointer ();
    string cFile = pEarlyConfigReplayMessagingContext->getConfigFileForParsing ();
    UI32 rbridgeId = DcmToolKit::getThisVcsNodeDomainId ();
    char rbstr[50];
    memset (rbstr, 0, sizeof (rbstr));
    sprintf (rbstr, "rbridge-id %d",rbridgeId);
    vector <string> new_configs;
    UI32 sz = configs.size();
    UI32 i = 0, secpolicy_cfg = 0, new_sz = 0, flag = 0;
    size_t secpolicy_found, memberentry_found, secpolicyDelim_found;
    string line = "";
    string secpolicy_token = "secpolicy ";
	string memberentry_token = "member-entry ";
    ofstream configFile;
    while (i < sz)
    {
        line = configs[i];
        secpolicy_found = line.find (secpolicy_token);
		memberentry_found = line.find (memberentry_token);
		secpolicyDelim_found = line.find ("!");
        if ((secpolicy_found != string::npos) || (memberentry_found != string::npos) ||
				((secpolicyDelim_found != string::npos) && (i > 0) && (configs[i-1].find (memberentry_token)!= string::npos)))
        {
            /* Found secpolicy or member entry */
            flag = 1;
        } else {
            /* Exiting secpolicy insert ! */
            if (1 == flag) {
                flag = 2;
            }
        }

        if (1 == flag) {
            if (0 == secpolicy_cfg)
            {
                if (i == 0) {
					new_configs.push_back (rbstr);
				} else if (configs[i-1].compare(0, 10, "rbridge-id") != 0) {
                        new_configs.push_back (rbstr);                   
                }
            }
            line.insert (0, " ");
            new_configs.push_back (line);
			secpolicy_cfg++;
        }

        else if (2 == flag) {
            /* Insert ! only if system-monitor block has exited */
            if (secpolicy_cfg != 0)
            {
				if (i == sz - 1) {
					new_configs.push_back("!");
                    new_configs.push_back("!");
                    secpolicy_cfg = 0;
					
				} else if (configs[i+1].compare("!") != 0) {
					//TODO: should we add two !s
                    new_configs.push_back("!");
                    new_configs.push_back("!");
					secpolicy_cfg = 0;
                }
            }
            new_configs.push_back(line);
            flag = 0;
        }
        else
            new_configs.push_back(line);

        i++;
    }
    i = 0;
    new_sz = new_configs.size();
    configFile.open (cFile.c_str());
    while (i < new_sz - 1)
    {
        configFile << new_configs[i];
        configFile << endl;
        i++;
    }
    configFile.close();
    pEarlyConfigReplayMessagingContext->setConfigVectorPointer (new_configs);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId EarlyConfigurationReplayWorker::parseStartupFileStep (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{
    tracePrintf (TRACE_LEVEL_DEBUG, "EarlyConfigurationReplayWorker::parseStartupFileStep: needParsingFlag        : %d", pEarlyConfigReplayMessagingContext->getParsingIncompleteOnFwdlFlag ());
    tracePrintf (TRACE_LEVEL_DEBUG, "EarlyConfigurationReplayWorker::parseStartupFileStep: configFileForParsing   : %s", (pEarlyConfigReplayMessagingContext->getConfigFileForParsing ()).c_str());
    tracePrintf (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::parseStartupFileStep: currentFirmwareVersion : %s", (DcmToolKit::getCurrentFirmwareVersion()).c_str());
    tracePrintf (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::parseStartupFileStep: olderFirmwareVersion   : %s", (DcmToolKit::getPreviousFirmwareVersion()).c_str());

    if (pEarlyConfigReplayMessagingContext->getParsingIncompleteOnFwdlFlag ())
    {
        // Plug-ins can do parsing of the startup config file here. Please use the file (pEarlyConfigReplayMessagingContext->getConfigFileForParsing ()) for parsing.
        // This file is a copy of startup config file. At the successful completion of the parsing and earlyConfigReplay, this file will be copied back to the startup config file.
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "EarlyConfigurationReplayWorker::parseStartupFileStep: parsing is not required.");
    }

    return WAVE_MESSAGE_SUCCESS;
}

void EarlyConfigurationReplayWorker::invalidStartupDBHandlerSuccessStep (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{
    // move startup config backup file as config file when parsing required is true.
    if (pEarlyConfigReplayMessagingContext->getParsingIncompleteOnFwdlFlag ())
    {
        string startupFile          = FrameworkToolKit::getStartupFileName ();
        string destStartupFile      = ""; 
        char   tmpDest[1024];
        int    retStatus            = -1;

        retStatus = readlink (startupFile.c_str(), tmpDest, (sizeof(tmpDest)-1));

        if (-1 != retStatus)
        {
            tmpDest[retStatus] ='\0';
            destStartupFile = string(tmpDest);
            WaveNs::tracePrintf (TRACE_LEVEL_WARN, true, false, "EarlyConfigurationReplayWorker::invalidStartupDBHandlerSuccessStep : destStartupFile : [%s]", destStartupFile.c_str());

            string         cmdString = "/bin/mv " + FrameworkToolKit::getStartupBackupFileName() + " " + destStartupFile;
            vector<string> output;

            SI32 status = FrameworkToolKit::systemCommandOutput(cmdString.c_str(), output);
            if (0 != status)
            {
                if (false == output.empty())
                {
                    WaveNs::tracePrintf (TRACE_LEVEL_WARN, true, false, "EarlyConfigurationReplayWorker::invalidStartupDBHandlerSuccessStep: copying parsed startup backup file to startup file failed. Error : %s", output[0].c_str());
                }
            }
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_WARN, "EarlyConfigurationReplayWorker::invalidStartupDBHandlerSuccessStep: failed to get the target startupFile.");
        }        
    }

    EarlyConfigurationReplayWorker::prismSynchronousLinearSequencerSucceededStep (pEarlyConfigReplayMessagingContext);
}

void EarlyConfigurationReplayWorker::invalidStartupDBHandlerFailureStep (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext)
{
    WaveNs::trace (TRACE_LEVEL_WARN, "EarlyConfigurationReplayWorker::invalidStartupDBFailureStep: entering..");

    if (pEarlyConfigReplayMessagingContext->getParsingIncompleteOnFwdlFlag ())
    {
        string         cmdString = "/bin/rm " + FrameworkToolKit::getStartupBackupFileName();
        vector<string> output;
        FrameworkToolKit::systemCommandOutput(cmdString.c_str(), output);
    }

    EarlyConfigurationReplayWorker::prismSynchronousLinearSequencerFailedStep (pEarlyConfigReplayMessagingContext);
}

void EarlyConfigurationReplayWorker::invalidStartupDBHandler ()
{
    trace (TRACE_LEVEL_DEBUG, string ("EarlyConfigurationReplayWorker::invalidStartupDBHandler : Entering ..."));

    /*
        NOTE: Any plugin related step added in this handler must also be added in the configSnapshotEarlyConfigReplayHandler.
    */

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::extractStartupFile),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::writeLinecardConfigurationToDB),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::writeHardwareConnectorConfigToDB),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::parseStartupFileStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::editSystemMonitorConfig),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::editFcspConfig),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::editSecpolicyConfig),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::invalidStartupDBHandlerSuccessStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::invalidStartupDBHandlerFailureStep)
    };

    EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext = new EarlyConfigReplayMessagingContext (NULL, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pEarlyConfigReplayMessagingContext, __FILE__, __LINE__);

    if (true == FrameworkToolKit::isStartupConfigBackupFileExist())
    {
        pEarlyConfigReplayMessagingContext->setParsingIncompleteOnFwdlFlag (true);
    }
    
    pEarlyConfigReplayMessagingContext->execute ();
}

void EarlyConfigurationReplayWorker::configSnapshotEarlyConfigReplayHandler ()
{
    trace (TRACE_LEVEL_DEBUG, string ("EarlyConfigurationReplayWorker::configSnapshotEarlyConfigReplayHandler : Entering ..."));
   
    /*
        NOTE: Any plugin related step added in this handler must also be added in the invalidStartupDBHandler.
    */
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::extractConfigSnapshotFile),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::writeLinecardConfigurationToDB),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::writeHardwareConnectorConfigToDB),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EarlyConfigurationReplayWorker::prismSynchronousLinearSequencerFailedStep)
    };  
    
    EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext = new EarlyConfigReplayMessagingContext (NULL, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));    prismAssert (NULL != pEarlyConfigReplayMessagingContext, __FILE__, __LINE__);
    
    pEarlyConfigReplayMessagingContext->execute ();
}

UI32 EarlyConfigurationReplayWorker::getHwIdFromLinecardType (const string linecardType)
{
    UI32 hwId = VCS_NODE_SLOT_LINECARD_TYPE_UNKNOWN;

    if (strcmp (linecardType.c_str (), "LC48x10G") == 0)
    {
        hwId = MERCURY_LC48X10G_BLADE_ID;
    }
    else if (strcmp (linecardType.c_str (), "LC12x40G") == 0)
    {
        hwId = MERCURY_LC12X40G_BLADE_ID;
    }
    else if (strcmp (linecardType.c_str (), "LC72x1G") == 0)
    {
        hwId = MERCURY_LC72X1G_BLADE_ID;
    }
    else if (strcmp (linecardType.c_str (), "LC48x1G") == 0)
    {
        hwId = MERCURY_LC48X1G_BLADE_ID;
    }

    return hwId;
}

UI32 EarlyConfigurationReplayWorker::getLinecardModeFromString (const string linecardMode)
{
    UI32 mode = 0;

    if (strcmp (linecardMode.c_str (), "default") == 0)
    {
        mode = VCS_NODE_SLOT_LINECARD_MODE_DEFAULT;
    }
    else if (strcmp (linecardMode.c_str (), "10g") == 0)
    {
        mode = VCS_NODE_SLOT_LINECARD_MODE_10G;
    }

    return mode;
}

}
