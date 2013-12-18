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
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#ifndef APPOBJECTMANAGER_H
#define APPOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "AppInterface.h"

namespace WaveNs
{
class AppDetail;
class AppInterfaceObjectManager;
class WaveAsynchronousContextForBootPhases;

class AppObjectManager : public WaveLocalObjectManagerForUserSpecificTasks
{
    private :
                      AppObjectManager                       (AppDetail *appDetail, UI32 applicationId, const UI32 &stackSize = 0);
        static  void  addToApplicationSpecificServicesVector (const PrismServiceId &prismServiceId);

    protected :
    public :
                                           ~AppObjectManager                         ();
               void                         boot                                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        static AppObjectManager            *createInstance                           (AppDetail *appDetail, UI32 applicationId);
        static SI32                         registerApp                              (string appName, applicationMainFunction applicationMain);
        static SI32                         registerApp                              (string appName, applicationMainFunction applicationMain, UI32 argc, char *argv[]);
        static void                         createAppObjectMangers                   ();
        static AppInterfaceObjectManager   *getAppInterfaceObjectManager             (UI32 appId);
        static AppInterfaceObjectManager   *getAppInterfaceObjectManagerForServiceId (UI32 appInterafceServiceId);
        static AppObjectManager            *getAppObjectManager                      (UI32 appId);

        static bool                         isAnApplicationSpecificService           (const PrismServiceId &prismServiceId);
        static bool                         validateAppId                            (SI32 appId);

    // Now the data members

    private :
            static  AppObjectManager         *m_pAppObjectManager;
                    applicationMainFunction   m_applicationMain;
                    UI32                      m_applicationId;
                    UI32                      m_argc;
                    char                    **m_argv;

    protected :
    public :
};

class AppDetail
{
    private :
    protected :
    public :
                                   AppDetail                    ();
                                   AppDetail                    (string appName, applicationMainFunction applicationMain);
                                   AppDetail                    (string appName, applicationMainFunction applicationMain, UI32 argc, char **argv);
                                  ~AppDetail                    ();
        void                       setAppName                   (string appName);
        string                     getAppName                   ();
        void                       setAppObjectManager          (AppObjectManager *appObjectManager);
        AppObjectManager          *getAppObjectManager          ();
        void                       setAppInterfaceObjectManager (AppInterfaceObjectManager *appInterfaceObjectManager);
        AppInterfaceObjectManager *getAppInterfaceObjectManager ();
        void                       setApplicationMain           (applicationMainFunction applicationMain);
        applicationMainFunction    getApplicationMain           ();
        void                       setArgc                      (UI32 argc);
        UI32                       getArgc                      ();
        void                       setArgv                      (char **argv);
        char                     **getArgv                      ();
        void                       setAppInterfaceServiceId     (UI32 appInterfaceServiceId);
        void                       setAppServiceId              (UI32 appServiceId);
        UI32                       getAppInterfaceServiceId     ();
        UI32                       getAppServiceId              ();
    // Now the data members

    private :
        string                      m_appName;
        AppInterfaceObjectManager  *m_appInterfaceObjectManager;
        applicationMainFunction     m_applicationMain;
        AppObjectManager           *m_appObjectManager;
        UI32                        m_argc;
        char                      **m_argv;
        UI32                        m_appInterfaceServiceId;
        UI32                        m_appServiceId;
    protected :
    public :
};


}

#endif // APPOBJECTMANAGER_H
