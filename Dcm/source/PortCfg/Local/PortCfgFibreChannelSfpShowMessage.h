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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#ifndef PORTCFGFIBRECHANNELSFPSHOWMESSAGE_H
#define PORTCFGFIBRECHANNELSFPSHOWMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgFibreChannelSfpShowMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgFibreChannelSfpShowMessage ();
            PortCfgFibreChannelSfpShowMessage (const UI32 &cmdCode,const vector<SI32> &switchPort,const vector<string> &portId,const vector<UI32> &rbridgeId,const vector<string> &identifier,const vector<string> &connector,const vector<string> &transceiver,const vector<string> &encoding,const vector<string> &baudRate,const vector<string> &length_9u_Km,const vector<string> &length_9u_m,const vector<string> &length_50u,const vector<string> &length_625u,const vector<string> &length_Cu,const vector<string> &vendor_Name,const vector<string> &vendor_OUI,const vector<string> &vendor_PN,const vector<string> &vendor_Rev,const vector<string> &wavelength,const vector<string> &options,const vector<string> &brMax,const vector<string> &brMin,const vector<string> &serialNo,const vector<string> &dateCode,const vector<string> &ddType,const vector<string> &enhOptions,const vector<string> &statusCtrl,const vector<string> &alarmFlags,const vector<string> &warnFlags,const vector<string> &temperature,const vector<string> &voltage,const vector<string> &current,const vector<string> &txPower,const vector<string> &rxPower,const vector<string> &errInfo);
            virtual    ~PortCfgFibreChannelSfpShowMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setSwitchPort(const vector<SI32> &switchPort);
            vector<SI32> getSwitchPort()  const;
            void setPortId(const vector<string> &portId);
            vector<string> getPortId()  const;
            void setRbridgeId(const vector<UI32> &rbridgeId);
            vector<UI32> getRbridgeId()  const;
            void setIdentifier(const vector<string> &identifier);
            vector<string> getIdentifier()  const;
            void setConnector(const vector<string> &connector);
            vector<string> getConnector()  const;
            void setTransceiver(const vector<string> &transceiver);
            vector<string> getTransceiver()  const;
            void setEncoding(const vector<string> &encoding);
            vector<string> getEncoding()  const;
            void setBaudRate(const vector<string> &baudRate);
            vector<string> getBaudRate()  const;
            void setLength_9u_Km(const vector<string> &length_9u_Km);
            vector<string> getLength_9u_Km()  const;
            void setLength_9u_m(const vector<string> &length_9u_m);
            vector<string> getLength_9u_m()  const;
            void setLength_50u(const vector<string> &length_50u);
            vector<string> getLength_50u()  const;
            void setLength_625u(const vector<string> &length_625u);
            vector<string> getLength_625u()  const;
            void setLength_Cu(const vector<string> &length_Cu);
            vector<string> getLength_Cu()  const;
            void setVendor_Name(const vector<string> &vendor_Name);
            vector<string> getVendor_Name()  const;
            void setVendor_OUI(const vector<string> &vendor_OUI);
            vector<string> getVendor_OUI()  const;
            void setVendor_PN(const vector<string> &vendor_PN);
            vector<string> getVendor_PN()  const;
            void setVendor_Rev(const vector<string> &vendor_Rev);
            vector<string> getVendor_Rev()  const;
            void setWavelength(const vector<string> &wavelength);
            vector<string> getWavelength()  const;
            void setOptions(const vector<string> &options);
            vector<string> getOptions()  const;
            void setBrMax(const vector<string> &brMax);
            vector<string> getBrMax()  const;
            void setBrMin(const vector<string> &brMin);
            vector<string> getBrMin()  const;
            void setSerialNo(const vector<string> &serialNo);
            vector<string> getSerialNo()  const;
            void setDateCode(const vector<string> &dateCode);
            vector<string> getDateCode()  const;
            void setDdType(const vector<string> &ddType);
            vector<string> getDdType()  const;
            void setEnhOptions(const vector<string> &enhOptions);
            vector<string> getEnhOptions()  const;
            void setStatusCtrl(const vector<string> &statusCtrl);
            vector<string> getStatusCtrl()  const;
            void setAlarmFlags(const vector<string> &alarmFlags);
            vector<string> getAlarmFlags()  const;
            void setWarnFlags(const vector<string> &warnFlags);
            vector<string> getWarnFlags()  const;
            void setTemperature(const vector<string> &temperature);
            vector<string> getTemperature()  const;
            void setVoltage(const vector<string> &voltage);
            vector<string> getVoltage()  const;
            void setCurrent(const vector<string> &current);
            vector<string> getCurrent()  const;
            void setTxPower(const vector<string> &txPower);
            vector<string> getTxPower()  const;
            void setRxPower(const vector<string> &rxPower);
            vector<string> getRxPower()  const;
            void setErrInfo(const vector<string> &errInfo);
            vector<string> getErrInfo()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_cmdCode;
            vector<SI32>  m_switchPort;
            vector<string>  m_portId;
            vector<UI32>  m_rbridgeId;
            vector<string>  m_identifier;
            vector<string>  m_connector;
            vector<string>  m_transceiver;
            vector<string>  m_encoding;
            vector<string>  m_baudRate;
            vector<string>  m_length_9u_Km;
            vector<string>  m_length_9u_m;
            vector<string>  m_length_50u;
            vector<string>  m_length_625u;
            vector<string>  m_length_Cu;
            vector<string>  m_vendor_Name;
            vector<string>  m_vendor_OUI;
            vector<string>  m_vendor_PN;
            vector<string>  m_vendor_Rev;
            vector<string>  m_wavelength;
            vector<string>  m_options;
            vector<string>  m_brMax;
            vector<string>  m_brMin;
            vector<string>  m_serialNo;
            vector<string>  m_dateCode;
            vector<string>  m_ddType;
            vector<string>  m_enhOptions;
            vector<string>  m_statusCtrl;
            vector<string>  m_alarmFlags;
            vector<string>  m_warnFlags;
            vector<string>  m_temperature;
            vector<string>  m_voltage;
            vector<string>  m_current;
            vector<string>  m_txPower;
            vector<string>  m_rxPower;
            vector<string>  m_errInfo;
    };
}
#endif                                            //PORTCFGFIBRECHANNELSFPSHOWMESSAGE_H
