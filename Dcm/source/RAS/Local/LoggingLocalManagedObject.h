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
 *   Author : chenj                                                     *
 **************************************************************************/

#ifndef LOGGINGLOCALMANAGEDOBJECT_H
#define LOGGINGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "RASTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class RASLocalObjectManager;

    class LoggingLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
	    virtual void loadOperationalData(WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext);

        public:
            LoggingLocalManagedObject (RASLocalObjectManager *pRASLocalObjectManager);
            LoggingLocalManagedObject (RASLocalObjectManager *pRASLocalObjectManager,const string &raslog);
            virtual    ~LoggingLocalManagedObject ();
            static string getClassName();
	    void setSwitchId(const string &switchId);
            string getSwitchId()  const;
            void setRaslog(const string &raslog);
            string getRaslog()  const;
	    void setAutoSupport(bool support);
            bool getAutoSupport()  const;
	    void setChassisName(const string &name);
            string getChassisName()  const;
	    void setHostName(const string &name);
            string getHostName()  const;
// Now the data members

        private:
	
        protected:
        public:
	    string  m_switchId;
            string  m_raslog;
	    bool    m_isAutoSupport;
	    string  m_chassisName;
	    string  m_hostName;
    };
}
#endif   //LOGGINGLOCALMANAGEDOBJECT_H