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
 *   Author : mmohan                                                       *
 **************************************************************************/

#ifndef DISCOVERMANAGEMENTINTERFACEMESSAGES_H
#define DISCOVERMANAGEMENTINTERFACEMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Attributes/AttributeStringVectorVector.h"
#include "discover/User/discoverDcmDef.h"

using namespace WaveNs;

namespace DcmNs {

    class DiscoverInternalMessage: public ManagementInterfaceMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            DiscoverInternalMessage();
            DiscoverInternalMessage(const UI32 &cmdcode,vector<vector<string> >  &m_respList);
            virtual ~DiscoverInternalMessage();
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode() const;
        public:
            UI32 m_cmdcode;
            vector<vector<string> > respList;
    };

    // ************* DiscoverDiscoverMessage ************
    class DiscoverDiscoverMessage: public ManagementInterfaceMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            DiscoverDiscoverMessage();
            DiscoverDiscoverMessage(const UI32 &cmdcode, const UI32 &msgType,
                    const string &vcenter);
            virtual ~DiscoverDiscoverMessage();
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode() const;
            void setMsgType(const UI32 &msgType);
            UI32 getMsgType() const;
            void setVcenter(const string &vcenter);
            string getVcenter() const;
            void setUrl(const string &url);
            string getUrl() const;
            void setUsername(const string &username);
            string getUsername() const;
            void setPassword(const string &password);
            string getPassword() const;
            void setIgnoreValue(const UI32 &ignorevalue);
            UI32 getIgnoreValue()  const;
            void setAlways(const bool &always);
            bool getAlways()  const;
            void setGlobalScriptNames(const string &globalScriptNames);
            string getGlobalScriptNames() const;
            void setLocalScriptNames(const string &localScriptNames);
            string getLocalScriptNames() const;
            void setLocationIds(const UI32 &locationIds);
            vector<UI32> getLocationIds() const;
            void setExitStatus(const UI32 &exitStatus);
            UI32 getExitStatus() const;
            void setState(const UI32 &state);
            UI32 getState() const;
            void setArg1(const string &arg1);
            string getArg1() const;
            void setArg2(const string &arg2);
            string getArg2() const;
            void setArg3(const string &arg3);
            string getArg3() const;
            void setArg4(const string &arg4);
            string getArg4() const;

            // Now the data members
        private:
        protected:
        public:
            UI32 m_cmdcode;
            UI32 m_msgType;
            string m_vcenter;
            string m_url;
            string m_username;
            string m_password;
            UI32    m_ignore_value;
            bool    m_always;
            string m_globalScriptNames;
            string m_localScriptNames;
            UI32 m_exitStatus;
            vector<UI32> m_locationIds;
            UI32 m_state;
            string m_arg1;
            string m_arg2;
            string m_arg3;
            string m_arg4;
    };

	class DiscoverUpdateMessage: public ManagementInterfaceMessage {
            private:
            protected:
                virtual void setupAttributesForSerialization();
            public:
                DiscoverUpdateMessage();
                DiscoverUpdateMessage(const UI32 &cmdcode, const string &vcenter, const UI32 &state,const UI32 &exitStatus );
                virtual ~DiscoverUpdateMessage();
				void setVcenter(const string &vcenter);
				string getVcenter() const;
				void setState(const UI32 &state);
				UI32 getState() const;
				void setExitStatus(const UI32 &exitStatus);
				UI32 getExitStatus() const;
				void setCmdcode(const UI32 &cmdcode);
				UI32 getCmdcode() const;

            private:
            protected:
            public:
                string m_vcenter;
                UI32 m_state;
                UI32 m_exitStatus;
                UI32 m_cmdcode;

        };


    class DiscoverDistributeMessage: public ManagementInterfaceMessage {
            private:
            protected:
                virtual void setupAttributesForSerialization();
            public:
                DiscoverDistributeMessage();
                DiscoverDistributeMessage(const UI32 &cmdcode, const UI32 &msgType,
                        const string &vcenter);
                virtual ~DiscoverDistributeMessage();
                void setCmdcode(const UI32 &cmdcode);
                UI32 getCmdcode() const;
                void setMsgType(const UI32 &msgType);
                UI32 getMsgType() const;
                void setVcenter(const string &vcenter);
                string getVcenter() const;

                void setLocationIds(vector<UI32> &locationIds);
                vector<UI32> getLocationIds() const;
                void setDataList(vector<string> &m_datalist);
                vector<string> getDataList() const;


                // Now the data members
            private:
            protected:
            public:
                UI32 m_cmdcode;
                UI32 m_msgType;
                string m_vcenter;
                vector<UI32> m_locationIds;
                vector <string> m_datalist;
                UI32 m_state;

        };
    class DistributeDBMessage: public ManagementInterfaceMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            DistributeDBMessage();
            DistributeDBMessage(const string &dbfile);
            virtual ~DistributeDBMessage();
            void setDBfile(const string &dbfile);
            string getDBfile() const;
            void setDestDBfile(const string &dbdestfile);
            string getDestDBfile() const;
            void setLocationIds(vector<UI32> &locationIds);
            vector<UI32> getLocationIds() const;

            // Now the data members
        private:
        protected:
        public:
            string m_dbfile;
            string m_dest_dbfile;
            vector<UI32> m_locationIds;
            UI32 m_state;

    };


    class VNMDPMessage: public ManagementInterfaceMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            VNMDPMessage();
            VNMDPMessage(const UI32 &cmdCode,const string &ifName,const string &hostMor,const string &portId, const string &vcenter);
            virtual ~VNMDPMessage();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setHostMor(const string &hostMor);
            string getHostMor()  const;
            void setPortId(const string &portId);
            string getPortId()  const;
            void setVcenter(const string &vcenter);
            string getVcenter()  const;

        public:
            UI32  m_cmdCode;
            string  m_ifName;
            string  m_hostMor;
            string  m_portId;
            string  m_vcenter;

    };



}
#endif  // DISCOVERMANAGEMENTINTERFACEMESSAGES_H
