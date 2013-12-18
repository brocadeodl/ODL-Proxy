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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/OsLayer/PrismOsLayer.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Core/Wave.h"
#include "Framework/Utils/TraceUtils.h"
#include "arpa/inet.h"
#include <net/if.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <assert.h>
#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>

using namespace std;

namespace WaveNs
{

unsigned int prismSleep (unsigned int numberOfSecondsToSleep)
{
    return (sleep (numberOfSecondsToSleep));
}

unsigned int prismUSleep (unsigned int numberOfMicroSecondsToSleep)
{
    return (usleep (numberOfMicroSecondsToSleep));
}

void prismSysLog (const char * const pStringToLog)
{
    syslog (LOG_USER | LOG_ERR, "%s", pStringToLog);
}

void initializePrismSocketLayer ()
{
}

void getIpAddressesForThisHost (vector<string> &ipAddressesForThisHost)
{
    static unsigned int maximumNumberOfInterfaces = 4096;
           unsigned int numberOfInterfaces        = 0;
           int          socketFileDescriptor;
    struct ifreq       *ifr;
    struct ifconf       ifc;
    struct sockaddr_in  sa;
           int          sizeOfIfreq               = sizeof (struct ifreq);
           unsigned int j                         = 0;
           string       publicIpAddress;

    socketFileDescriptor = ::socket (AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (-1 == socketFileDescriptor)
    {
        assert (0);
        return;
    }

    ifc.ifc_len = maximumNumberOfInterfaces * sizeOfIfreq;
    ifc.ifc_req = (struct ifreq *) malloc (maximumNumberOfInterfaces * sizeOfIfreq);

    assert (NULL != ifc.ifc_req);

    ioctl (socketFileDescriptor, SIOCGIFCONF, &ifc);
    numberOfInterfaces = ifc.ifc_len / sizeOfIfreq;

    ifr = ifc.ifc_req;

    for (j = 0; j < numberOfInterfaces; j++, ifr++)
    {
        //printf ("1 - Interface Name : %20s\n", ifr->ifr_name);

        if (ifr->ifr_addr.sa_data == (ifr+1)->ifr_addr.sa_data)
        {
            continue;  // duplicate, skip it
        }

        if (ioctl (socketFileDescriptor, SIOCGIFFLAGS, ifr))
        {
            continue;  // failed to get flags, skip it
        }

        //printf ("2 - Interface Name : %20s, FLAGS = 0x%X\n", ifr->ifr_name, ifr->ifr_flags);

        if ((ifr->ifr_flags & IFF_LOOPBACK) || (0 == (ifr->ifr_flags & IFF_UP)) || (0 == (ifr->ifr_flags & IFF_RUNNING)))
        {
            continue;  // We ignore loop back interface and the interfaces that are not up and that have no link present.
        }

        if (ioctl (socketFileDescriptor, SIOCGIFADDR, ifr))
        {
            continue;  // failed to get interface address, skip it
        }

        char *pTemp = &(ifr->ifr_addr.sa_data[sizeof sa.sin_port]);

        publicIpAddress = inet_ntoa (*((struct in_addr *) pTemp));
        ipAddressesForThisHost.push_back (publicIpAddress);
    }

    close (socketFileDescriptor);
    free (ifc.ifc_req);

    return;
}

string getIpAddressForInterface (const string &interfaceName)
{
           int          socketFileDescriptor;
    struct ifreq        ifr;
    struct sockaddr_in  sa;
           string       publicIpAddress;

    socketFileDescriptor = ::socket (AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (-1 == socketFileDescriptor)
    {
        return (publicIpAddress);
    }

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ-1);

    if ((ioctl (socketFileDescriptor, SIOCGIFFLAGS, &ifr) == 0) && (0 == (ifr.ifr_flags & IFF_LOOPBACK)) && (ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING))
    {
        if (ioctl (socketFileDescriptor, SIOCGIFADDR, &ifr) == 0)
        {
            char *pTemp = &(ifr.ifr_addr.sa_data[sizeof sa.sin_port]);

            publicIpAddress = inet_ntoa(*((struct in_addr *) pTemp));
        }
    }
    close (socketFileDescriptor);

    return publicIpAddress;
}

string getStateForInterface (const string &interfaceName)
{
    static unsigned int maximumNumberOfInterfaces = 8192;
           unsigned int numberOfInterfaces        = 0;
           int          socketFileDescriptor;
    struct ifreq       *ifr;
    struct ifconf       ifc;
    struct sockaddr_in  sa;
           int          sizeOfIfreq               = sizeof (struct ifreq);
           unsigned int j                         = 0;
           string       result;
           char         *pTemp;

    socketFileDescriptor = ::socket (AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (-1 == socketFileDescriptor)
    {
        result = "Failed to get the stream socket for interface query";
        return (result);
    }

    ifc.ifc_len = maximumNumberOfInterfaces * sizeOfIfreq;
    ifc.ifc_req = (struct ifreq *) malloc (maximumNumberOfInterfaces * sizeOfIfreq);

    if (NULL == ifc.ifc_req)
    {
       result = "failed to allocate memory to retrieve host interface list";
       close (socketFileDescriptor);
       return (result);
    }

    if (ioctl (socketFileDescriptor, SIOCGIFCONF, &ifc))
    {
       result = "ioctl on SIOCGIFCONF failed: errno = " + string(strerror(errno));
       close (socketFileDescriptor);
       free (ifc.ifc_req);

       return (result);
    }

    numberOfInterfaces = ifc.ifc_len / sizeOfIfreq;
    if (numberOfInterfaces == 0)
    {
       result = "no valid interface entry found by SIOCGIFCONF";
       close (socketFileDescriptor);
       free (ifc.ifc_req);

       return (result);
    }

    ifr = ifc.ifc_req;
    for (j = 0; j < numberOfInterfaces; j++, ifr++)
    {
        if (0 == (strcmp (interfaceName.c_str (), ifr->ifr_name)))
        {
            if (ioctl (socketFileDescriptor, SIOCGIFFLAGS, ifr))
            {
                result = "failed to get flag on the interface" + string(strerror(errno));
            }
            else if (0 == (ifr->ifr_flags & IFF_UP))
            {
                result = "interface is not in IFF_UP state";
            }
            else if (0 == (ifr->ifr_flags & IFF_RUNNING))
            {
                result = "interface is not in IFF_RUNNING state";
            }
            else if (ifr->ifr_flags & IFF_LOOPBACK)
            {
                result = "interface is in IFF_LOOPBACK state";
            }
            else if (ioctl (socketFileDescriptor, SIOCGIFADDR, ifr))
            {
                result = "failed to get ipaddress on the interface" + string(strerror(errno));
            }
            else 
            {
                pTemp = &(ifr->ifr_addr.sa_data[sizeof sa.sin_port]);
                result = inet_ntoa(*((struct in_addr *) pTemp)); 
            }
            break;
        }
    }
   
    if (j == numberOfInterfaces)
    {
        if (numberOfInterfaces == maximumNumberOfInterfaces)
        {
             result = "Interface not found in list - number of interfaces exceeded max 8192";
        }
        else if (interfaceName.size() >= IFNAMSIZ) 
        {
             result = "interface name equal or exceed the IFNAMSIZ";
        }
        else
        {
             result = "failed to find the interface in the host's list";
        }
    }
 
    close (socketFileDescriptor);
    free (ifc.ifc_req);

    return (result);
}

int prismCloseSocket (int fd)
{
    return (close (fd));
}

void prismBlockSignals ()
{
    // The following code massks the SIGALRM and SIGPIPE signals for the entire process.
    // This happens jut before the first ptheread gets created in the system and it only happens
    // once.  We make sure that this by using the signalMaskIndicator statuc variable.
    // On linux the oly way to make the sigwait to reliably work is to block the corresponding
    // sgnals for the entire process before any thread is crated in the system and them make a
    // particular thread wait for the signal using the sigwait call.  Refer to the "Threads and Signal handling"
    // section of the GNU libc manual located at the following URL:
    // http://www.gnu.org/software/libc/manual/html_mono/libc.html#Threads%20and%20Signal%20Handling
    // This works on GNU/Linux as well as Net BSD systems.

    // Make sure that the thread blocks the SIGALRM and SIGPIPE.
    // Only a special thread inside the framework timer service will wait for SIGALRM.

    sigset_t signalsToBlock;

    sigemptyset (&signalsToBlock);
    sigaddset (&signalsToBlock, SIGALRM);
    sigaddset (&signalsToBlock, SIGPIPE);

    int result = sigprocmask (SIG_BLOCK, &signalsToBlock, NULL);

    assert (0 == result);

    if (0 != result)
    {
        cerr << "Could not block Alarm Signal for this thread." << endl;
        exit (-1);
    }
}

void sigIntHandler (int signal)
{
    system ("stty cooked echo");

    cerr << endl;
    cerr << "Program received signal SIGINT, Exiting." << endl;

    Wave::waveExit ();
}

void sigSegvHandler (int signal)
{
    system ("stty cooked echo");

    cerr << endl;
    cerr << "Program received signal SIGSEGV, Exiting." << endl;

    prismAssert (0, __FILE__, __LINE__);

    Wave::waveExit ();
}

void prismInstallSigIntHandler ()
{
    struct sigaction signalAction;

    sigemptyset (&(signalAction.sa_mask));
    signalAction.sa_flags = 0;
    signalAction.sa_handler = sigIntHandler;

    sigaction (SIGINT, &signalAction, NULL);

    sigemptyset (&(signalAction.sa_mask));
    signalAction.sa_flags = 0;
    signalAction.sa_handler = sigSegvHandler;

    sigaction (SIGSEGV, &signalAction, NULL);
}

char *prismGetCwd (char *buf, size_t size)
{
    return (getcwd (buf, size));
}

void prismCreateWaitableTimer ()
{
}

void prismWaitForAlarmSignal ()
{
    int             sig;
    sigset_t        signalsToWaitFor;

    sigemptyset (&signalsToWaitFor);
    sigaddset (&signalsToWaitFor, SIGALRM);

    sigwait (&signalsToWaitFor, &sig);
}

int prismSetITimer (struct itimerval *pItimerValue)
{
    return (setitimer (ITIMER_REAL, pItimerValue, NULL));
}

void prismSetConsoleTextColor (TraceLevel requestedTraceLevel)
{
    switch (requestedTraceLevel)
    {
        case TRACE_LEVEL_UNKNOWN:
            cout << "\033[34m";
            break;
        case TRACE_LEVEL_DEVEL:
            cout << "\033[36m";
            break;
        case TRACE_LEVEL_DEBUG:
            cout << "\033[35m";
            break;
        case TRACE_LEVEL_INFO:
            break;
        case TRACE_LEVEL_PERF_START:
            break; 
        case TRACE_LEVEL_PERF_END:
            break; 
        case TRACE_LEVEL_SUCCESS:
            cout << "\033[32m";
            break;
        case TRACE_LEVEL_WARN:
            cout << "\033[33m";
            break;
        case TRACE_LEVEL_ERROR:
            cout << "\033[31m";
            break;
        case TRACE_LEVEL_FATAL:
            cout << "\033[37;41;1m";
            break;
    }
}

void prismResetConsoleTextColor ()
{
    cout << "\033[0m";
}

string getIPv6AddressForInterface (const string &interfaceName)
{
    string          IPv6Address = "";
    struct ifaddrs *ifaddr      = NULL;

    int    getifaddrsReturnValue = getifaddrs (&ifaddr);

    if ((0 != getifaddrsReturnValue) || (NULL == ifaddr))
    {
        // Error.
        return IPv6Address;
    }

    struct ifaddrs *ifa = NULL;

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        // pointer check

        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

        // match address family

        int addressFamily = ifa->ifa_addr->sa_family;

        if (addressFamily != AF_INET6)
        {
            continue;
        }

        struct sockaddr_in6 *pSockaddrIn6 = (struct sockaddr_in6 *) ifa->ifa_addr;

        // 1. skip link-local addresses.

        int isLinkLocal  = IN6_IS_ADDR_LINKLOCAL(&(pSockaddrIn6->sin6_addr));

        if (0 != isLinkLocal)
        {
            continue;
        }

        // 2. skip site-local addresses. ????

        int isSiteLocal = IN6_IS_ADDR_SITELOCAL(&(pSockaddrIn6->sin6_addr));

        if (0 != isSiteLocal)
        {
            continue;
        }

        // pointer check

        if (NULL == ifa->ifa_name)
        {
            continue;
        }

        // continue if interface name does not match requested interface name

        string nameOfInterface = ifa->ifa_name;

        if (0 != interfaceName.compare (nameOfInterface))
        {
            continue;
        }

        // continue if getnameinfo failes

        char IPv6AddressBuffer[NI_MAXHOST] = {'\0'};

        int  status = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6), IPv6AddressBuffer, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if (status != 0)
        {
            continue;
        }

        IPv6Address = IPv6AddressBuffer;

        break;
    }

    freeifaddrs(ifaddr);

    return IPv6Address;
}

}

