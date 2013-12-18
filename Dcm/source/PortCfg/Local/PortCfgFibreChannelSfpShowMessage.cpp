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

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgFibreChannelSfpShowMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgFibreChannelSfpShowMessage::PortCfgFibreChannelSfpShowMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFIBRECHANNELSFPSHOW)
    {
    }

    PortCfgFibreChannelSfpShowMessage::PortCfgFibreChannelSfpShowMessage (const UI32 &cmdCode,const vector<SI32> &switchPort,const vector<string> &portId,const vector<UI32> &rbridgeId,const vector<string> &identifier,const vector<string> &connector,const vector<string> &transceiver,const vector<string> &encoding,const vector<string> &baudRate,const vector<string> &length_9u_Km,const vector<string> &length_9u_m,const vector<string> &length_50u,const vector<string> &length_625u,const vector<string> &length_Cu,const vector<string> &vendor_Name,const vector<string> &vendor_OUI,const vector<string> &vendor_PN,const vector<string> &vendor_Rev,const vector<string> &wavelength,const vector<string> &options,const vector<string> &brMax,const vector<string> &brMin,const vector<string> &serialNo,const vector<string> &dateCode,const vector<string> &ddType,const vector<string> &enhOptions,const vector<string> &statusCtrl,const vector<string> &alarmFlags,const vector<string> &warnFlags,const vector<string> &temperature,const vector<string> &voltage,const vector<string> &current,const vector<string> &txPower,const vector<string> &rxPower,const vector<string> &errInfo)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFIBRECHANNELSFPSHOW),
        m_cmdCode    (cmdCode),
        m_switchPort    (switchPort),
        m_portId    (portId),
        m_rbridgeId    (rbridgeId),
        m_identifier    (identifier),
        m_connector    (connector),
        m_transceiver    (transceiver),
        m_encoding    (encoding),
        m_baudRate    (baudRate),
        m_length_9u_Km    (length_9u_Km),
        m_length_9u_m    (length_9u_m),
        m_length_50u    (length_50u),
        m_length_625u    (length_625u),
        m_length_Cu    (length_Cu),
        m_vendor_Name    (vendor_Name),
        m_vendor_OUI    (vendor_OUI),
        m_vendor_PN    (vendor_PN),
        m_vendor_Rev    (vendor_Rev),
        m_wavelength    (wavelength),
        m_options    (options),
        m_brMax    (brMax),
        m_brMin    (brMin),
        m_serialNo    (serialNo),
        m_dateCode    (dateCode),
        m_ddType    (ddType),
        m_enhOptions    (enhOptions),
        m_statusCtrl    (statusCtrl),
        m_alarmFlags    (alarmFlags),
        m_warnFlags    (warnFlags),
        m_temperature    (temperature),
        m_voltage    (voltage),
        m_current    (current),
        m_txPower    (txPower),
        m_rxPower    (rxPower),
        m_errInfo    (errInfo)
    {
    }

    PortCfgFibreChannelSfpShowMessage::~PortCfgFibreChannelSfpShowMessage ()
    {
    }

    void  PortCfgFibreChannelSfpShowMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32Vector(&m_switchPort,"switchPort"));
        addSerializableAttribute (new AttributeStringVector(&m_portId,"portId"));
        addSerializableAttribute (new AttributeUI32Vector(&m_rbridgeId,"rbridgeId"));
        addSerializableAttribute (new AttributeStringVector(&m_identifier,"identifier"));
        addSerializableAttribute (new AttributeStringVector(&m_connector,"connector"));
        addSerializableAttribute (new AttributeStringVector(&m_transceiver,"transceiver"));
        addSerializableAttribute (new AttributeStringVector(&m_encoding,"encoding"));
        addSerializableAttribute (new AttributeStringVector(&m_baudRate,"baudRate"));
        addSerializableAttribute (new AttributeStringVector(&m_length_9u_Km,"length_9u_Km"));
        addSerializableAttribute (new AttributeStringVector(&m_length_9u_m,"length_9u_m"));
        addSerializableAttribute (new AttributeStringVector(&m_length_50u,"length_50u"));
        addSerializableAttribute (new AttributeStringVector(&m_length_625u,"length_625u"));
        addSerializableAttribute (new AttributeStringVector(&m_length_Cu,"length_Cu"));
        addSerializableAttribute (new AttributeStringVector(&m_vendor_Name,"vendor_Name"));
        addSerializableAttribute (new AttributeStringVector(&m_vendor_OUI,"vendor_OUI"));
        addSerializableAttribute (new AttributeStringVector(&m_vendor_PN,"vendor_PN"));
        addSerializableAttribute (new AttributeStringVector(&m_vendor_Rev,"vendor_Rev"));
        addSerializableAttribute (new AttributeStringVector(&m_wavelength,"wavelength"));
        addSerializableAttribute (new AttributeStringVector(&m_options,"options"));
        addSerializableAttribute (new AttributeStringVector(&m_brMax,"brMax"));
        addSerializableAttribute (new AttributeStringVector(&m_brMin,"brMin"));
        addSerializableAttribute (new AttributeStringVector(&m_serialNo,"serialNo"));
        addSerializableAttribute (new AttributeStringVector(&m_dateCode,"dateCode"));
        addSerializableAttribute (new AttributeStringVector(&m_ddType,"ddType"));
        addSerializableAttribute (new AttributeStringVector(&m_enhOptions,"enhOptions"));
        addSerializableAttribute (new AttributeStringVector(&m_statusCtrl,"statusCtrl"));
        addSerializableAttribute (new AttributeStringVector(&m_alarmFlags,"alarmFlags"));
        addSerializableAttribute (new AttributeStringVector(&m_warnFlags,"warnFlags"));
        addSerializableAttribute (new AttributeStringVector(&m_temperature,"temperature"));
        addSerializableAttribute (new AttributeStringVector(&m_voltage,"voltage"));
        addSerializableAttribute (new AttributeStringVector(&m_current,"current"));
        addSerializableAttribute (new AttributeStringVector(&m_txPower,"txPower"));
        addSerializableAttribute (new AttributeStringVector(&m_rxPower,"rxPower"));
        addSerializableAttribute (new AttributeStringVector(&m_errInfo,"errInfo"));
    }

    void  PortCfgFibreChannelSfpShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PortCfgFibreChannelSfpShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  PortCfgFibreChannelSfpShowMessage::setSwitchPort(const vector<SI32> &switchPort)
    {
        m_switchPort  =  switchPort;
    }

    vector<SI32>  PortCfgFibreChannelSfpShowMessage::getSwitchPort() const
    {
        return (m_switchPort);
    }

    void  PortCfgFibreChannelSfpShowMessage::setPortId(const vector<string> &portId)
    {
        m_portId  =  portId;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getPortId() const
    {
        return (m_portId);
    }

    void  PortCfgFibreChannelSfpShowMessage::setRbridgeId(const vector<UI32> &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;
    }

    vector<UI32>  PortCfgFibreChannelSfpShowMessage::getRbridgeId() const
    {
        return (m_rbridgeId);
    }

    void  PortCfgFibreChannelSfpShowMessage::setIdentifier(const vector<string> &identifier)
    {
        m_identifier  =  identifier;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getIdentifier() const
    {
        return (m_identifier);
    }

    void  PortCfgFibreChannelSfpShowMessage::setConnector(const vector<string> &connector)
    {
        m_connector  =  connector;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getConnector() const
    {
        return (m_connector);
    }

    void  PortCfgFibreChannelSfpShowMessage::setTransceiver(const vector<string> &transceiver)
    {
        m_transceiver  =  transceiver;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getTransceiver() const
    {
        return (m_transceiver);
    }

    void  PortCfgFibreChannelSfpShowMessage::setEncoding(const vector<string> &encoding)
    {
        m_encoding  =  encoding;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getEncoding() const
    {
        return (m_encoding);
    }

    void  PortCfgFibreChannelSfpShowMessage::setBaudRate(const vector<string> &baudRate)
    {
        m_baudRate  =  baudRate;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getBaudRate() const
    {
        return (m_baudRate);
    }

    void  PortCfgFibreChannelSfpShowMessage::setLength_9u_Km(const vector<string> &length_9u_Km)
    {
        m_length_9u_Km  =  length_9u_Km;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getLength_9u_Km() const
    {
        return (m_length_9u_Km);
    }

    void  PortCfgFibreChannelSfpShowMessage::setLength_9u_m(const vector<string> &length_9u_m)
    {
        m_length_9u_m  =  length_9u_m;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getLength_9u_m() const
    {
        return (m_length_9u_m);
    }

    void  PortCfgFibreChannelSfpShowMessage::setLength_50u(const vector<string> &length_50u)
    {
        m_length_50u  =  length_50u;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getLength_50u() const
    {
        return (m_length_50u);
    }

    void  PortCfgFibreChannelSfpShowMessage::setLength_625u(const vector<string> &length_625u)
    {
        m_length_625u  =  length_625u;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getLength_625u() const
    {
        return (m_length_625u);
    }

    void  PortCfgFibreChannelSfpShowMessage::setLength_Cu(const vector<string> &length_Cu)
    {
        m_length_Cu  =  length_Cu;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getLength_Cu() const
    {
        return (m_length_Cu);
    }

    void  PortCfgFibreChannelSfpShowMessage::setVendor_Name(const vector<string> &vendor_Name)
    {
        m_vendor_Name  =  vendor_Name;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getVendor_Name() const
    {
        return (m_vendor_Name);
    }

    void  PortCfgFibreChannelSfpShowMessage::setVendor_OUI(const vector<string> &vendor_OUI)
    {
        m_vendor_OUI  =  vendor_OUI;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getVendor_OUI() const
    {
        return (m_vendor_OUI);
    }

    void  PortCfgFibreChannelSfpShowMessage::setVendor_PN(const vector<string> &vendor_PN)
    {
        m_vendor_PN  =  vendor_PN;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getVendor_PN() const
    {
        return (m_vendor_PN);
    }

    void  PortCfgFibreChannelSfpShowMessage::setVendor_Rev(const vector<string> &vendor_Rev)
    {
        m_vendor_Rev  =  vendor_Rev;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getVendor_Rev() const
    {
        return (m_vendor_Rev);
    }

    void  PortCfgFibreChannelSfpShowMessage::setWavelength(const vector<string> &wavelength)
    {
        m_wavelength  =  wavelength;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getWavelength() const
    {
        return (m_wavelength);
    }

    void  PortCfgFibreChannelSfpShowMessage::setOptions(const vector<string> &options)
    {
        m_options  =  options;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getOptions() const
    {
        return (m_options);
    }

    void  PortCfgFibreChannelSfpShowMessage::setBrMax(const vector<string> &brMax)
    {
        m_brMax  =  brMax;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getBrMax() const
    {
        return (m_brMax);
    }

    void  PortCfgFibreChannelSfpShowMessage::setBrMin(const vector<string> &brMin)
    {
        m_brMin  =  brMin;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getBrMin() const
    {
        return (m_brMin);
    }

    void  PortCfgFibreChannelSfpShowMessage::setSerialNo(const vector<string> &serialNo)
    {
        m_serialNo  =  serialNo;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getSerialNo() const
    {
        return (m_serialNo);
    }

    void  PortCfgFibreChannelSfpShowMessage::setDateCode(const vector<string> &dateCode)
    {
        m_dateCode  =  dateCode;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getDateCode() const
    {
        return (m_dateCode);
    }

    void  PortCfgFibreChannelSfpShowMessage::setDdType(const vector<string> &ddType)
    {
        m_ddType  =  ddType;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getDdType() const
    {
        return (m_ddType);
    }

    void  PortCfgFibreChannelSfpShowMessage::setEnhOptions(const vector<string> &enhOptions)
    {
        m_enhOptions  =  enhOptions;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getEnhOptions() const
    {
        return (m_enhOptions);
    }

    void  PortCfgFibreChannelSfpShowMessage::setStatusCtrl(const vector<string> &statusCtrl)
    {
        m_statusCtrl  =  statusCtrl;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getStatusCtrl() const
    {
        return (m_statusCtrl);
    }

    void  PortCfgFibreChannelSfpShowMessage::setAlarmFlags(const vector<string> &alarmFlags)
    {
        m_alarmFlags  =  alarmFlags;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getAlarmFlags() const
    {
        return (m_alarmFlags);
    }

    void  PortCfgFibreChannelSfpShowMessage::setWarnFlags(const vector<string> &warnFlags)
    {
        m_warnFlags  =  warnFlags;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getWarnFlags() const
    {
        return (m_warnFlags);
    }

    void  PortCfgFibreChannelSfpShowMessage::setTemperature(const vector<string> &temperature)
    {
        m_temperature  =  temperature;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getTemperature() const
    {
        return (m_temperature);
    }

    void  PortCfgFibreChannelSfpShowMessage::setVoltage(const vector<string> &voltage)
    {
        m_voltage  =  voltage;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getVoltage() const
    {
        return (m_voltage);
    }

    void  PortCfgFibreChannelSfpShowMessage::setCurrent(const vector<string> &current)
    {
        m_current  =  current;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getCurrent() const
    {
        return (m_current);
    }

    void  PortCfgFibreChannelSfpShowMessage::setTxPower(const vector<string> &txPower)
    {
        m_txPower  =  txPower;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getTxPower() const
    {
        return (m_txPower);
    }

    void  PortCfgFibreChannelSfpShowMessage::setRxPower(const vector<string> &rxPower)
    {
        m_rxPower  =  rxPower;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getRxPower() const
    {
        return (m_rxPower);
    }

    void  PortCfgFibreChannelSfpShowMessage::setErrInfo(const vector<string> &errInfo)
    {
        m_errInfo  =  errInfo;
    }

    vector<string>  PortCfgFibreChannelSfpShowMessage::getErrInfo() const
    {
        return (m_errInfo);
    }

}
