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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/Wave.h"
#include "DcmCore/Dcm.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "DeviceService/Local/DeviceServiceLocalObjectManager.h"
#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Global/PortCfgGlobalObjectManager.h"
#include "FspfService/Global/FspfServiceGlobalObjectManager.h"
#include "FspfService/Local/FspfServiceLocalObjectManager.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/Test/DcmInterfaceTestObjectManager.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"

#include "PhysicalInterface/Local/PhysicalInterfaceLocalObjectManager.h"
#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/Test/VcsNodeLocalMessagingTestObjectManager.h"
#include "VcsFabric/Local/Test/VcsNodeLocalEventTestObjectManager.h"
#include "VcsFabric/Local/VcsNodeHaObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Local/QosLocalObjectManager.h"
#include "Policer/Global/PolicerGlobalObjectManager.h"
#include "Policer/Local/PolicerLocalObjectManager.h"
#include "Rmon/Global/RmonGlobalObjectManager.h"
#include "Rmon/Local/RmonLocalObjectManager.h"
#include "Sflow/Global/SflowGlobalObjectManager.h"
#include "Sflow/Local/SflowLocalObjectManager.h"
#include "RAS/Local/RASLocalObjectManager.h"
#include "RAS/Global/RASObjectManager.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "FabricService/Local/FabricServiceLocalObjectManager.h"
#include "LicenseService/Local/LicenseServiceLocalObjectManager.h"
#include "LicenseService/Global/LicenseServiceGlobalObjectManager.h"
#include "Firmware/Global/FirmwareObjectManager.h"
#include "Firmware/Global/FirmwareStatusObjectManager.h"
#include "Firmware/Local/FirmwareClusterLocalObjectManager.h"
#include "Firmware/Local/FirmwareOpLocalObjectManager.h"
#include "Firmware/Local/FirmwareShowLocalObjectManager.h"
#include "Firmware/Global/NodeAutoUpgradeObjectManager.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Local/LldpLocalObjectManager.h"
#include "Diag/Global/DiagObjectManager.h"
#include "Diag/Local/DiagTestsLocalObjectManager.h"
#include "Diag/Local/DiagShowLocalObjectManager.h"
#include "Diag/Local/DiagLocalObjectManager.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Local/APPMLocalObjectManager.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Global/FCAuthGlobalObjectManager.h"
#include "FCPathUtil/Local/FCPathUtilLocalObjectManager.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalObjectManager.h"
#include "VRRP/Global/VRRPGlobalObjectManager.h"
#include "VRRP/Local/VRRPLocalObjectManager.h"

#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Local/FcoeLocalObjectManager.h"

#include "AG/Local/AgLocalObjectManager.h"

#include "L2sys/Global/L2sysGlobalObjectManager.h"
#include "L2sys/Local/L2sysLocalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Local/NTPClockLocalObjectManager.h"
#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Global/IpadmGlobalObjectManager.h"
#include "Ipadm/Local/IpadmChassisObjectManager.h"
#include "Igmps/Global/IgmpsGlobalObjectManager.h"
#include "Igmps/Local/IgmpsLocalObjectManager.h"
#include "Arp/Local/ArpLocalObjectManager.h"
#include "ConfigFileMgmt/Local/ConfigFileMgmtLocalObjectManager.h"
#include "Span/Local/SpanLocalLocalObjectManager.h"
#include "Span/Global/SpanGlobalObjectManager.h"
#include "Interface/ValInterfaceGlobalObjectManager.h"

#include "Terminal/Global/TerminalTypes.h"
#include "Terminal/Global/TerminalObjectManager.h"

#include "SwitchConfig/Local/SwitchConfigLocalObjectManager.h"
#include "Em/Local/LinecardLocalObjectManager.h"
#include "TrillOam/Local/TrillOamLocalObjectManager.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"

#include "Beacon/Local/BeaconLocalObjectManager.h"
#include "Rtm/Local/RtmLocalObjectManager.h"
#include "Pim/Local/PimLocalObjectManager.h"
#include "Igmp/Local/IgmpLocalObjectManager.h"
#include "discover/Framework/discoverObjectManager.h"
#include "discover/Framework/discoverLocalObjectManager.h"
#include "vCenter/Framework/vCenterObjectManager.h"
#include "vCenter/Framework/vCenterLocalObjectManager.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Local/ZoneLocalObjectManager.h"
#include "DcmCore/DcmToolKit.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"

#include "Ha/Local/HaOpLocalObjectManager.h"
#include "Ha/Global/HaOpGlobalObjectManager.h"
#include "Ha/Global/HaShowGlobalObjectManager.h"

#include "RAS/Global/RASVCSObjectManager.h"
#include "RAS/Local/RASVCSLocalObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManager.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagementObjectManager.h"
#include "EmployeeManagementService/CompanyProfileService/GroupLocalObjectManager.h"
#include "EmployeeManagementService/CompanyLocationService/CityObjectManager.h"
#include "EmployeeManagementService/CompanyLocationService/CityLocalObjectManager.h"
#include "AliasConfig/Global/AliasConfigObjectManager.h"
#include "Vrf/Local/VrfLocalObjectManager.h"
#include "EmployeeManagementService/CompanyProfileService/CompanyManagementLocalObjectManager.h"

#include "discover/Framework/EventEngineObjectManager.h"
#include "discover/Framework/EventEngineLocalObjectManager.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "UidService/Global/UidServiceObjectManager.h"

#include "Udld/Global/UdldGlobalObjectManager.h"
#include "Udld/Local/UdldLocalObjectManager.h"
#include "L3Node/Global/L3NodeSpecificGlobalObjectManager.h"
#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService/Global/CompanyServiceObjectManager.h"

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Interface/OpenFlowSwitchTransportObjectManager.h"

using namespace NspNs;
using namespace OpenFlowNs;

namespace DcmNs
{

void Dcm::initialize ()
{
    // Setup DCM specific Most ORM Base Classes

    OrmRepository::addMostBaseClass (DcmManagedObject::getClassName ());
    OrmRepository::addMostBaseClass (DcmLocalManagedObject::getClassName ());

    // Setup the Wave User Client Print Function(s))

    // Setup the Wave User to get clients instances
    PrismFrameworkObjectManager::setGetInstancesFunction (&DcmToolKit::getClientInstances);

    // Setup the Wave User to get clients instances and sub instances
    PrismFrameworkObjectManager::setGetInstancesAndSubInstancesFunction (&DcmToolKit::getClientInstancesAndSubInstances);
 
    // Setup the Wave User to get this client slot instance
    PrismFrameworkObjectManager::setGetThisSlotInstanceFunction (&DcmToolKit::getThisSlotInstance);

    // Setup the Wave User to log raslog
    PrismFrameworkObjectManager::setRaslogAssertFunction (&DcmToolKit::raslogAssert);

    // Setup the Wave User to get the firmware version
    PrismFrameworkObjectManager::setGetFirmwareVersionFunction (&DcmToolKit::getFirmwareVersion);

    // Setup the Wave User to process the dcmConfigFssSyncData_t
    PrismFrameworkObjectManager::setHandleDcmFssConfigUpdateFunction (&DcmToolKit::handleDcmFssConfigUpdate);
    PrismFrameworkObjectManager::setSetVcsClusterConfigHandlerFunction (&VcsFabricLocalObjectManager::setVcsClusterConfigHandler);
    PrismFrameworkObjectManager::setGetVcsClusterConfigHandlerFunction (&VcsFabricLocalObjectManager::getVcsClusterConfigHandler);
    PrismFrameworkObjectManager::setNotifySyncUpdateFailureFunction (&VcsNodeFabosInterfaceObjectManager::dcmHaSyncUpdateFailure);
    PrismFrameworkObjectManager::setNotifyClusterReadyStateFunction (&VcsNodeFabosInterfaceObjectManager::dcmNotifyHasmClusterReadyState);

    // Setup the Wave Usert to create dummy slot
    PrismFrameworkObjectManager::setCreateDummySlotFunction (&DcmToolKit::createDummySlot);

    // Setup the Wave User to push Kernel Config
    PersistenceLocalObjectManager::setUpdateGlobalConfigInKernelHandlerFunction (&DcmToolKit::pushConfigToKernel);

    // Setup the Wave User Client path

    // register a function with persistence object manager, which tells whether Management Cluster is enabled.
    VcsFabricLocalObjectManager::registerClusterEnabledCheckFunctionWithPersistence ();
    // register a function to add a RAS log for "copy default to start" status. A DCM function will be called from wave.
    VcsFabricLocalObjectManager::registerAddRASLogFunctionWithPersistence ();
    // register a function with persistence OM, which checks the VcsMode and returns if it is enabled.
    VcsFabricLocalObjectManager::registerVCSEnableCheckFunctionWithWave   ();

    //register a function with WaveLayer to get LinecardReadyBitMap 
    VcsFabricLocalObjectManager::registerLinecardReadySlotIdBitMapFunction();

    registerServices (false);
}

void Dcm::registerServices (const bool &registerAllServicesInInactiveMode)
{
    ResourceId waveServiceMode = WAVE_SERVICE_ACTIVE;

    if (true == registerAllServicesInInactiveMode)
    {
        waveServiceMode = WAVE_SERVICE_INACTIVE;
    }

    // Now add the Dcm specific services to Framework.
    Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (VcsFabObjectManager::getInstance), true, waveServiceMode);
    Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (VcsFabricLocalObjectManager::getInstance), true, waveServiceMode);
    Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (VcsNodeFabosInterfaceObjectManager::getInstance), true, waveServiceMode);
    Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (VcsNodeHaObjectManager::getInstance), true, waveServiceMode);


    if ((DCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()) || (DCM_SMD == DcmToolKit::getDistributionMode ()))
    {
        // L3NodeSpecificGlobalOM should be kept at the beginning
        Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (ValInterfaceGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (ValInterfaceLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerNativeService (reinterpret_cast<NativePrismServiceInstantiator> (DcmInterfaceTestObjectManager::getInstance), true, waveServiceMode);
        
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (L3NodeSpecificGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SnmpServerObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SnmpServerIntLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (PhysicalInterfaceLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SsmLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SsmGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (AAAGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (AAALocalLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DeviceServiceLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (PortCfgLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (PortCfgGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FspfServiceGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FspfServiceLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (VcsNodeLocalMessagingTestObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (VcsNodeLocalEventTestObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FabricServiceLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NsmGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NsmLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (QosGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (QosLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (PolicerGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (PolicerLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (RmonGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (RmonLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SflowGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SflowLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (BgpLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SpanLocalLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SpanGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (MstpGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (MstpLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (UdldGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (UdldLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (L2sysGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (L2sysLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (TerminalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (LldpGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (LldpLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (RASLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (RASObjectManager::getInstance), true, waveServiceMode);
		Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SSObjectManager::getInstance), true, waveServiceMode);
		Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SSLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (RASVCSLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (RASVCSObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (HaOpLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (HaOpGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (HaShowGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DiagObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DiagTestsLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DiagShowLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DiagLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (APPMGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (APPMLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (IgmpsGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (IgmpsLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (ArpLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (OSPFLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DhcpRelLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (VRRPLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (VRRPGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (IpPolicyLocalObjectManager::getInstance), true, waveServiceMode);

        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (LicenseServiceLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (LicenseServiceGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (IpadmLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (IpadmGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (IpadmChassisObjectManager::getInstance), true, waveServiceMode);

        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (RtmLocalObjectManager::getInstance), true, waveServiceMode);

        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FcoeGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FcoeLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NTPClockGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NTPClockLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (ConfigFileMgmtLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SwitchConfigLocalObjectManager::getInstance), true, waveServiceMode);

        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (LinecardLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (BeaconLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (PimLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (IgmpLocalObjectManager::getInstance), true, waveServiceMode);

        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (RASLogLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FCAuthLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FCAuthGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FCPathUtilLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SystemMonitorGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (SystemMonitorLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (ThresholdMonitorLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (ThresholdMonitorGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (AliasConfigObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (VrfLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (AgLocalObjectManager::getInstance), true, waveServiceMode);


        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FirmwareObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FirmwareClusterLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FirmwareOpLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FirmwareShowLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NodeAutoUpgradeObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (FirmwareStatusObjectManager::getInstance), true, waveServiceMode);

        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (CompanyManagementObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (CompanyManagementLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (GroupLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (CityObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (CityLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (CompanyServiceLocalObjectManager::getInstance));
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (CompanyServiceObjectManager::getInstance));
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (OpenFlowSwitchTransportObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (OpenFlowSwitchObjectManager::getInstance),          true, waveServiceMode);
        
        if (DCM_ONLY == DcmToolKit::getDistributionMode ())
        {
            ////////////////////////////////////////////////////////////////
            // IMPORTANT - register CCMD plugins as inactive here too..
            // Failing to comply with this would crash FC to MC transition
            ////////////////////////////////////////////////////////////////

            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (EventEngineObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (EventEngineLocalObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (ZoneObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (vCenterObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (vCenterLocalObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DiscoverObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DiscoverLocalObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (TrillOamLocalObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NetworkConfigurationObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NetworkConfigurationLocalObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (UidServiceObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
            Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (ZoneLocalObjectManager::getInstance), true, WAVE_SERVICE_INACTIVE);
        }
    }
    
    if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (DCM_CCM == DcmToolKit::getDistributionMode ()) || (DCM_SMD == DcmToolKit::getDistributionMode ()))
    {
        //  Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (VirtualIpGlobalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (EventEngineObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (EventEngineLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (ZoneObjectManager::getInstance), true, waveServiceMode);

        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (vCenterObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (vCenterLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DiscoverObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (DiscoverLocalObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (TrillOamLocalObjectManager::getInstance), true, waveServiceMode);

        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NetworkConfigurationObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (NetworkConfigurationLocalObjectManager::getInstance), true, waveServiceMode);

        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (UidServiceObjectManager::getInstance), true, waveServiceMode);
        Wave::registerApplicationSpecificNativeServices (reinterpret_cast<NativePrismServiceInstantiator> (ZoneLocalObjectManager::getInstance), true, waveServiceMode);
    }

}

void Dcm::initializeForWsm ()
{
    // Setup DCM specific Most ORM Base Classes

    OrmRepository::addMostBaseClass (DcmManagedObject::getClassName ());
    OrmRepository::addMostBaseClass (DcmLocalManagedObject::getClassName ());

    // InitializeServices in inactive mode for WSM purposes

    registerServices (true);
}

}

