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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Subramanian                                            *
 ***************************************************************************/

#include "Shell/WaveCliTraceShell.h"
#include "Shell/ShellTrace.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/Integer.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

namespace WaveNs
{

WaveCliTraceShell::WaveCliTraceShell (WaveClientSynchronousConnection &connection)
    : WaveCliShell ("trace", connection)
{

   addCommandfunction ("help1", reinterpret_cast<WaveShellCommandFunction> (&WaveCliTraceShell::traceHelp), "List all services and their trace levels. Valid only on switch.",            reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliTraceShell::traceHelpHelp));
   addCommandfunction ("list", reinterpret_cast<WaveShellCommandFunction> (&WaveCliTraceShell::traceList), "List all services and their trace levels.",            reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliTraceShell::traceListHelp));
   addCommandfunction ("setlevel", reinterpret_cast<WaveShellCommandFunction> (&WaveCliTraceShell::traceSetLevel), "Set service trace level.",                     reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliTraceShell::traceSetLevelHelp));
   addCommandfunction ("setall", reinterpret_cast<WaveShellCommandFunction> (&WaveCliTraceShell::traceSetAll), "Set the trace level for all service.",             reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliTraceShell::traceSetAllHelp));
   addCommandfunction ("setdefault", reinterpret_cast<WaveShellCommandFunction> (&WaveCliTraceShell::traceSetDefault), "Default level set for all service.",       reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliTraceShell::traceSetDefaultHelp));
   addCommandfunction ("legend",  reinterpret_cast<WaveShellCommandFunction> (&WaveCliTraceShell::traceLegend), "Displays Trace Legend information.",              reinterpret_cast<WaveShellCommandHelpFunction> (&WaveCliTraceShell::traceLegendHelp));

}

WaveCliTraceShell::~WaveCliTraceShell ()
{
}

void WaveCliTraceShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Trace shell provides basic control to set the trave level");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        for services in Wave based application.  For more information on");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        controlling trace level of services please type help in trace shell");
}

ResourceId WaveCliTraceShell::traceHelp (const vector<string> &arguments)
{
    tracePrintf (TRACE_LEVEL_SUCCESS, true, true, " tracelevel help");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        provides help for tracelevel setting. It lists all the commands for tracelevel and their usages.");
    tracePrintf (TRACE_LEVEL_SUCCESS, true, true, " tracelevel list legend");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        lists down all possible tracelevels in descending order with topmost being least critical.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        These can be used to set the required tracelevel. The tracelevel in the list also enables all the tracelevels below it.");
    tracePrintf (TRACE_LEVEL_SUCCESS, true, true, " tracelevel list service <service_id>");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        lists the current trace level for a service with index \"service_id\".");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Please note, This index number is different from the Wave Service Id.");
    tracePrintf (TRACE_LEVEL_SUCCESS, true, true, " tracelevel list all");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        lists all registered services, their ids(indexes) and current tracelevels.");
    tracePrintf (TRACE_LEVEL_SUCCESS, true, true, " tracelevel set service <service_id> <trace>");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        sets tracelevel \"trace\" for a service with index \"service_id\".");
    tracePrintf (TRACE_LEVEL_SUCCESS, true, true, " tracelevel set all <trace>");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        sets all registered services to tracelevel \"trace\".");

    return (WAVE_MESSAGE_SUCCESS);
}

void WaveCliTraceShell::traceHelpHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : help");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Lists all the commands for the tracelevel and their usages.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    None.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Lists all the commands for the tracelevel and their usages.");
}

ResourceId WaveCliTraceShell::traceList (const vector<string> &arguments)
{
    vector<TraceClientId>                           traceClientIDs;
    vector<TraceLevel>                              traceLevels;
    vector<string>                                  serviceNames;
    string                                          serviceName;
    string                                          traceLevelName;
    WaveClientSynchronousConnection                 connection      = getConnection ();
    ResourceId                                      status;

    status = connection.getListOfTraceServices (traceClientIDs, traceLevels, serviceNames);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        //In case of a single service trace level
        UI8          argList              = arguments.size ();
        bool         isSingleService      = false;
        if ( 0 != argList ) 
        {
            if ( 1 < argList )
            {
                tracePrintf (TRACE_LEVEL_INFO, true, true, "Extra arguments entered...");
                return (WAVE_MESSAGE_ERROR);
            }
            isSingleService = true;
        }
        
        if (isSingleService)
        {
            UI32        serviceIndex           = strtoul (arguments[1].c_str (), NULL, 10);

            // check if given service id is valid i.e. it belongs to one of the registered service.
            if ((traceClientIDs.size () <= serviceIndex) || (0 > serviceIndex))
            {
                tracePrintf (TRACE_LEVEL_ERROR, true, true, "No service available with index \"%s\"", arguments[1].c_str ());
                return (WAVE_MESSAGE_ERROR);
            }

            tracePrintf (TRACE_LEVEL_INFO, true, true, "Service Index - Service Name                          :  Current Trace Level");
            tracePrintf (TRACE_LEVEL_INFO, true, true, "_______________________________________________________________________________________________");

            traceLevelName = FrameworkToolKit::localize (traceLevels[serviceIndex]);

            tracePrintf (TRACE_LEVEL_INFO, true, true, "%03u - %-48s:  %s", serviceIndex, serviceNames[serviceIndex].c_str (), traceLevelName.c_str ());    
            return (status); 
        }

        //In case of listing all the services

        UI32                                            nServices       = traceClientIDs.size ();
        UI32                                            index           = 0;
 
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Total Number Of Test Services : %d", nServices);
        tracePrintf (TRACE_LEVEL_INFO, true, true, "");
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Service Index - Service Name                          :  Current Trace Level");
        tracePrintf (TRACE_LEVEL_INFO, true, true, "_______________________________________________________________________________________________");

        for (index = 0; index < nServices; index++)
        {
            traceLevelName = FrameworkToolKit::localize (traceLevels[index]);

            tracePrintf (TRACE_LEVEL_INFO, true, true, "%03u - %-48s:  %s", index, serviceNames[index].c_str (), traceLevelName.c_str ());
        }

        tracePrintf (TRACE_LEVEL_INFO, true, true, "_______________________________________________________________________________________________");
    }
    else
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Failed to get trace list information");
    }

    return (status);
}

void WaveCliTraceShell::traceListHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : list");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Lists the trace services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Listing is done in the order that the trace services are registered with WAVE Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    None.  All arguments are ignored.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following fields are listed:");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Index        - A monotonically increasing number to identify a trace service in the CLI.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                       This index number is different from the Wave Service Id.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Service Name - The Name of the Wave trace Service.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Trace Level  - String describing the trace level set in each service");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

ResourceId WaveCliTraceShell::traceSetLevel (const vector<string> &arguments)
{
    vector<TraceClientId>                           traceClientIDs;
    vector<TraceLevel>                              traceLevels;
    vector<string>                                  serviceNames;
    WaveClientSynchronousConnection                 connection          = getConnection ();
    ResourceId                                      status;
    UI32                                            numberOfArguments   = arguments.size ();

    if(numberOfArguments < 2)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Not enough arguments specified");
        return (WAVE_MESSAGE_ERROR);
    }
    else if(numberOfArguments > 2)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "More than required number of arguments specified");
        return (WAVE_MESSAGE_ERROR);
    }

    status = connection.getListOfTraceServices (traceClientIDs, traceLevels, serviceNames);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        UI32        index           = strtoul (arguments[0].c_str (), NULL, 10); 
        string      trace           = arguments[1].c_str ();
        
        TraceLevel  traceLevel      = WaveCliTraceShell::getTraceLevelResourceId (trace.c_str());
        
        // check if given tracelevel is valid.
        if (TRACE_LEVEL_UNKNOWN == traceLevel)
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, true, "%s is an unknown tracelevel.", trace.c_str());
            return (WAVE_MESSAGE_ERROR);
        }
        // check if given service id is valid i.e. it belongs to one of the registered service.
        if ((traceClientIDs.size() <= index) || (0 > index))
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, true, "No service available with index \"%s\"", arguments[0].c_str ());
            return (WAVE_MESSAGE_ERROR);
        }

        string      serviceName     = serviceNames[index];;
        string      traceLevelName  = FrameworkToolKit::localize (traceLevel);

        status = connection.setTraceLevelForAService(traceClientIDs[index], traceLevel);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            tracePrintf (TRACE_LEVEL_INFO, true, true, "Trace level for \"%s\" service has been set to \"%s\"",
                  serviceName.c_str (), traceLevelName.c_str ());
            return (status);
        }
        else
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, true, "Could not set the Trace level for \"%s\" service to \"%s\": Message sending failed.",
                  serviceName.c_str (), traceLevelName.c_str ());
            return (status);
        }

    }
    else
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Failed to get trace list information");
    }

    return (status);
}

void WaveCliTraceShell::traceSetLevelHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setlevel <index> <traceLevel>");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Sets the trace level for the various Wave based application services.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    index        - The index of the service for which the trace level has to be modified.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "                   This value can be obtained via list command.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    traceLevel - The trace level to be set for the service.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Sets the trace level for a service and reports success/failure of the operation.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");

}


ResourceId WaveCliTraceShell::traceSetAll (const vector<string> &arguments)
{
    vector<TraceClientId>                           traceClientIDs;
    vector<TraceLevel>                              traceLevels;
    vector<string>                                  serviceNames;
    WaveClientSynchronousConnection                 connection          = getConnection ();
    ResourceId                                      status;
    UI32                                            numberOfArguments   = arguments.size ();

    if(numberOfArguments < 1)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Not enough arguments specified");
        return (WAVE_MESSAGE_ERROR);
    }
    else if(numberOfArguments > 1)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "More than required number of arguments specified");
        return (WAVE_MESSAGE_ERROR);;
    }

    string           trace           = arguments[0];
    TraceLevel       traceLevel      = WaveCliTraceShell::getTraceLevelResourceId (trace.c_str());

    if (TRACE_LEVEL_UNKNOWN == traceLevel)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, " %s is an unknown tracelevel.", trace.c_str());
        return (WAVE_MESSAGE_ERROR);
    }

    string           traceLevelName  = FrameworkToolKit::localize (traceLevel);


    status = connection.setTraceLevelForAllServices (traceLevel);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Trace levels for all the services have been set to \"%s\"",
                            traceLevelName.c_str ());
    }
    else
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Falied to set the Trace level to \"%s\" for atleast one service: Message sending failed.",
                            traceLevelName.c_str ());
    }

    return (status);
}

void WaveCliTraceShell::traceSetAllHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setall <traceLevelId>");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Sets the trace level for all the Wave based application services.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    traceLevelId - The trace level ID to be set for the services.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Sets the trace level for a services and reports success/failure for each of this operation.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");

}

ResourceId WaveCliTraceShell::traceSetDefault (const vector<string> &arguments)
{
    trace (TRACE_LEVEL_INFO, "Not supported yet ");
    
    return (WAVE_MESSAGE_SUCCESS);
}

void WaveCliTraceShell::traceSetDefaultHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setdefault");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Sets the trace level for the various Wave based application services.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "Not supported yet");
}

ResourceId WaveCliTraceShell::traceLegend (const vector<string> &arguments)
{
    vector<string> setLevelArguments;
    
    (WaveUserInterfaceObjectManager::getInstance ())->setTraceLevel (TRACE_LEVEL_UNKNOWN);

//    tracePrintf (TRACE_LEVEL_UNKNOWN, true, true, "%s : This is a message traced at TRACE_LEVEL_UNKNOWN", (Integer (TRACE_LEVEL_UNKNOWN)).toString ().c_str());
    tracePrintf (TRACE_LEVEL_DEVEL,   true, true, "DEVEL : This captures all messages with tracelevel TRACE_LEVEL_DEVEL and below" );  
    tracePrintf (TRACE_LEVEL_DEBUG,   true, true, "DEBUG : This captures all messages with tracelevel TRACE_LEVEL_DEBUG and below.");
    tracePrintf (TRACE_LEVEL_INFO,    true, true, "INFO  : This captures all messages with tracelevel TRACE_LEVEL_INFO and below." );
    tracePrintf (TRACE_LEVEL_WARN,    true, true, "WARN  : This captures all messages with tracelevel TRACE_LEVEL_WARN and below." ); 
    tracePrintf (TRACE_LEVEL_ERROR,   true, true, "ERROR : This captures all messages with tracelevel TRACE_LEVEL_ERROR and below.");
    tracePrintf (TRACE_LEVEL_FATAL,   true, true, "FATAL : This captures all messages with tracelevel TRACE_LEVEL_FATAL.");

    (WaveUserInterfaceObjectManager::getInstance ())->setTraceLevel (TRACE_LEVEL_INFO);

    return (WAVE_MESSAGE_SUCCESS);
}

void WaveCliTraceShell::traceLegendHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : legend");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Shows the legend of the various trace levels and their outputs.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Displays the legend of the various trace levels and their outputs.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

TraceLevel WaveCliTraceShell::getTraceLevelResourceId (const char* trace)
{
    TraceLevel tracelevel = TRACE_LEVEL_UNKNOWN;
    if ((!strcmp (trace, "DEVEL")) || (!strcmp (trace, "TRACE_LEVEL_DEVEL")) || (!strcmp (trace, "devel")) || (!strcmp (trace, "trace_level_devel")) )
    {
        tracelevel = TRACE_LEVEL_DEVEL;
    } 
    else if ((!strcmp (trace, "DEBUG")) || (!strcmp (trace, "TRACE_LEVEL_DEBUG")) || (!strcmp (trace, "debug")) || (!strcmp (trace, "trace_level_debug")))
    {
        tracelevel = TRACE_LEVEL_DEBUG;   
    }                  
    else if ((!strcmp (trace, "INFO")) || (!strcmp (trace, "TRACE_LEVEL_INFO")) || (!strcmp (trace, "info")) || (!strcmp (trace, "trace_level_info")))
    {
        tracelevel = TRACE_LEVEL_INFO;
    }
    else if ((!strcmp (trace, "WARN")) || (!strcmp (trace, "TRACE_LEVEL_WARN")) || (!strcmp (trace, "warn")) || (!strcmp (trace, "trace_level_warn")))
    {
        tracelevel = TRACE_LEVEL_WARN;
    }
    else if ((!strcmp (trace, "ERROR")) || (!strcmp (trace, "TRACE_LEVEL_ERROR")) || (!strcmp (trace, "error")) || (!strcmp (trace, "trace_level_error")))
    {
        tracelevel = TRACE_LEVEL_ERROR;
    }
    else if ((!strcmp (trace, "FATAL")) || (!strcmp (trace, "TRACE_LEVEL_FATAL")) || (!strcmp (trace, "fatal")) || (!strcmp (trace, "trace_level_fatal")))
    {
        tracelevel = TRACE_LEVEL_FATAL;
    }
    else
    {
        tracelevel = TRACE_LEVEL_UNKNOWN;
    }

    return (tracelevel);    
}
}
