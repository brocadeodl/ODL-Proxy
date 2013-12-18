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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Interface/ValInterfaceLocalObjectManager.h"
#include "Interface/InterfaceLocalManagedObject.h"
#include "Interface/Composition/InterfaceCompositionLocalManagedObject.h"
#include "Interface/Association/InterfaceAssociationLocalManagedObject.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionLocalManagedObject.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionChildLocalManagedObject.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionGChildLocalManagedObject.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionSkippedGChildLocalManagedObject.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Interface/ValInterfaceLocalObjectManagerConfigureInterfaceMessage.h"
#include "Interface/ValInterfaceLocalObjectManagerGetInterfaceMessage.h"
#include "Interface/Composition/ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage.h"
#include "Interface/Association/ValInterfaceConfigureVlanMessage.h"
#include "Interface/InterfacePriorityLocalManagedObject.h"
#include "Interface/InterfacePriorityGetInterfacePriorityMessage.h"
#include "Interface/InterfacePriorityConfigureInterfacePriorityMessage.h"
#include "Interface/InterfacePriorityTypes.h"
#include "Interface/InterfaceDataTypeLocalManagedObject.h"
#include "Interface/InterfaceDataTypeGetInterfaceDataTypeMessage.h"
#include "Interface/InterfaceDataTypeConfigureInterfaceDataTypeMessage.h"
#include "Interface/InterfaceDatatypeTypes.h"
#include "Interface/InterfaceClassLocalManagedObject.h"
#include "Interface/InterfaceNodeSpecificLocalManagedObject.h"
#include "Interface/InterfaceClassGetInterfaceClassMessage.h"
#include "Interface/InterfaceClassConfigureInterfaceClassMessage.h"
#include "Interface/InterfaceNodeSpecificConfigureMessage.h"
#include "Interface/InterfaceClassTypes.h"
#include "Interface/InterfaceNodeSpecificTypes.h"
#include "Interface/ContainerInList/InterfaceContainerInListLocalManagedObject.h"
#include "Interface/ContainerInList/InterfaceContainerInListGetMessage.h"
#include "Interface/ContainerInList/InterfaceContainerInListConfigureMessage.h"
#include "Interface/ContainerInList/InterfaceContainerInListTypes.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "DcmShell/DcmShell.h"
#include <Interface/ValInterfaceGlobalObjectManager.h>
#include "Interface/InterfaceInstallDemoObjectMessage.h"
#include "Interface/WyserEaAsyncDemoMessages.h"
#include "DcmCore/DcmToolKit.h"



namespace DcmNs
{

ValInterfaceLocalObjectManager::ValInterfaceLocalObjectManager ()
    : WaveLocalObjectManager (getClassName ())
{
    DcmShell::addToWaveShell ();
    associateWithVirtualWaveObjectManager   (ValInterfaceGlobalObjectManager::getInstance ());
    InterfaceLocalManagedObject             interface                               (this);
    InterfacePriorityLocalManagedObject     interfacePriorityLocalManagedObject     (this);
    InterfaceClassLocalManagedObject        interfaceClassLocalManagedObject        (this);
    InterfaceNodeSpecificLocalManagedObject interfaceNodeSpecificLocalManagedObject (this);
    InterfaceDataTypeLocalManagedObject     interfaceDataTypeLocalManagedObject     (this);

    interface.setupOrm ();
    addManagedClass (InterfaceLocalManagedObject::getClassName ());

    interfaceClassLocalManagedObject.setupOrm ();
    addManagedClass (InterfaceClassLocalManagedObject::getClassName ());
    
    interfaceNodeSpecificLocalManagedObject.setupOrm ();
    addManagedClass (InterfaceNodeSpecificLocalManagedObject::getClassName ());

    interfacePriorityLocalManagedObject.setupOrm ();
    addManagedClass (InterfacePriorityLocalManagedObject::getClassName ());

    interfaceDataTypeLocalManagedObject.setupOrm ();
    addManagedClass (InterfaceDataTypeLocalManagedObject::getClassName ());

    ///////////////////////////////
    // For Composition sample
    ///////////////////////////////
    InterfaceCompositionLocalManagedObject interfaceComposition (this);

    interfaceComposition.setupOrm ();
    addManagedClass (InterfaceCompositionLocalManagedObject::getClassName ());

    InterfaceCompositionChildLocalManagedObject interfaceCompositionChild (this);

    interfaceCompositionChild.setupOrm ();
    addManagedClass (InterfaceCompositionChildLocalManagedObject::getClassName ());

    ///////////////////////////////
    // For ContainerInList sample
    ///////////////////////////////
    InterfaceContainerInListLocalManagedObject interfaceContainerInList (this);

    interfaceContainerInList.setupOrm ();
    addManagedClass (InterfaceContainerInListLocalManagedObject::getClassName ());

    ///////////////////////////////
    // For Association sample
    ///////////////////////////////
    DcmInterfaceAssociationLocalManagedObject interfaceAssociation (this);

    interfaceAssociation.setupOrm ();
    addManagedClass (DcmInterfaceAssociationLocalManagedObject::getClassName ());

    InterfaceSampleVlanLocalManagedObject interfaceAssociationChild (this);

    interfaceAssociationChild.setupOrm ();
    addManagedClass (InterfaceSampleVlanLocalManagedObject::getClassName ());

    ///////////////////////////////
    // For NDeepComposition sample
    ///////////////////////////////
    InterfaceNDeepCompositionLocalManagedObject interfaceNDeepComposition (this);

    interfaceNDeepComposition.setupOrm ();
    addManagedClass (InterfaceNDeepCompositionLocalManagedObject::getClassName ());

    InterfaceNDeepCompositionChildLocalManagedObject interfaceNDeepCompositionChild (this);

    interfaceNDeepCompositionChild.setupOrm ();
    addManagedClass (InterfaceNDeepCompositionChildLocalManagedObject::getClassName ());

    InterfaceNDeepCompositionGChildLocalManagedObject interfaceNDeepCompositionGChild (this);

    interfaceNDeepCompositionGChild.setupOrm ();
    addManagedClass (InterfaceNDeepCompositionGChildLocalManagedObject::getClassName ());

    InterfaceNDeepCompSkippedGChildLocalManagedObject interfaceNDeepCompSkippedGChild (this);

    interfaceNDeepCompSkippedGChild.setupOrm ();
    addManagedClass (InterfaceNDeepCompSkippedGChildLocalManagedObject::getClassName ());

    addOperationMap (VAL_INTERFACE_CONFIGURE_INTERFACE, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::configureInterfaceMessageHandler));
    addOperationMap (VAL_INTERFACE_GET_INTERFACE,       reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::getInterfaceMessageHandler));
    addOperationMap (VAL_INTERFACE_CONFIGURE_INTERFACE_CHILDREN, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::configureInterfaceChildrenMessageHandler));
    addOperationMap (INTERFACEPRIORITYGETINTERFACEPRIORITY, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InterfacePriorityGetInterfacePriorityMessageHandler));
    addOperationMap (INTERFACEPRIORITYCONFIGUREINTERFACEPRIORITY, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InterfacePriorityConfigureInterfacePriorityMessageHandler));
    addOperationMap (INTERFACECLASSGETINTERFACECLASS, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InterfaceClassGetInterfaceClassMessageHandler));
    addOperationMap (INTERFACECLASSCONFIGUREINTERFACECLASS, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InterfaceClassConfigureInterfaceClassMessageHandler));
    addOperationMap (INTERFACENODESPECIFICCONFIGUREINTERFACE, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InterfaceNodeSpecificConfigureMessageHandler));
    addOperationMap (INTERFACEDATATYPEGETINTERFACEDATATYPE, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InterfaceDataTypeGetInterfaceDataTypeMessageHandler));
    addOperationMap (INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler));
    addOperationMap (VAL_INTERFACE_INSTALL_DEMO_OBJECTS, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InstallDemoObjectMessageHandler));
	// For ContainerInList
    addOperationMap (INTERFACECONTAINERINLISTGET, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InterfaceContainerInListGetMessageHandler));
    addOperationMap (INTERFACECONTAINERINLISTCONFIGURE, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHandler));
    addOperationMap (VAL_INTERFACE_CONFIGURE_ASSOC_VLAN, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::configureInterfaceAssociationVlanMessageHandler));

    addOperationMap (VAL_INTERFACE_WYSEREA_ASYNC_DEMO_MESSAGE, reinterpret_cast<PrismMessageHandler> (&ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageHandler));
}

ValInterfaceLocalObjectManager::~ValInterfaceLocalObjectManager ()
{
}

ValInterfaceLocalObjectManager *ValInterfaceLocalObjectManager:: getInstance ()
{
    static ValInterfaceLocalObjectManager *pValInterfaceLocalObjectManager = new ValInterfaceLocalObjectManager ();

    WaveNs::prismAssert (NULL != pValInterfaceLocalObjectManager, __FILE__, __LINE__);

    return (pValInterfaceLocalObjectManager);
}

PrismServiceId ValInterfaceLocalObjectManager:: getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

WaveManagedObject *ValInterfaceLocalObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((InterfaceLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceLocalManagedObject (this);
    }
    else if ((InterfaceCompositionLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceCompositionLocalManagedObject (this);
    }
    else if ((InterfaceCompositionChildLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceCompositionChildLocalManagedObject (this);
    }
    else if ((InterfaceNDeepCompositionLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceNDeepCompositionLocalManagedObject (this);
    }
    else if ((InterfaceNDeepCompositionChildLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceNDeepCompositionChildLocalManagedObject (this);
    }
    else if ((InterfaceNDeepCompositionGChildLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceNDeepCompositionGChildLocalManagedObject (this);
    }
    else if ((InterfaceNDeepCompSkippedGChildLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceNDeepCompSkippedGChildLocalManagedObject (this);
    }
    else if ((DcmInterfaceAssociationLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new DcmInterfaceAssociationLocalManagedObject (this);
    }
    else if ((InterfaceSampleVlanLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceSampleVlanLocalManagedObject (this);
    }
	else if ((InterfacePriorityLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfacePriorityLocalManagedObject (this);
    }
	else if ((InterfaceClassLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceClassLocalManagedObject (this);
    }
	else if ((InterfaceNodeSpecificLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceNodeSpecificLocalManagedObject (this);
    }
    else if ((InterfaceDataTypeLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new InterfaceDataTypeLocalManagedObject (this);
    }
    else if ((InterfaceContainerInListLocalManagedObject::getClassName ()) == managedClassName)
    {
		// For ContainerInList
        pWaveManagedObject = new InterfaceContainerInListLocalManagedObject (this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }

    return (pWaveManagedObject);
}

PrismMessage *ValInterfaceLocalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

	trace (TRACE_LEVEL_INFO, string("ValInterfaceLocalObjectManager::createMessageInstance: Entered with opcode: ") + operationCode);
    switch (operationCode)
    {
        case VAL_INTERFACE_CONFIGURE_INTERFACE :
            pPrismMessage = new ValInterfaceLocalObjectManagerConfigureInterfaceMessage ();
            break;

        case VAL_INTERFACE_GET_INTERFACE :
            pPrismMessage = new ValInterfaceLocalObjectManagerGetInterfaceMessage ();
            break;

        case VAL_INTERFACE_CONFIGURE_INTERFACE_CHILDREN :
            pPrismMessage = new ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage ();
            break;

        case INTERFACEPRIORITYGETINTERFACEPRIORITY:
            pPrismMessage = new InterfacePriorityGetInterfacePriorityMessage ();
            break;

        case INTERFACEPRIORITYCONFIGUREINTERFACEPRIORITY:
            pPrismMessage = new InterfacePriorityConfigureInterfacePriorityMessage ();
            break;

        case INTERFACEDATATYPEGETINTERFACEDATATYPE:
            pPrismMessage = new InterfaceDataTypeGetInterfaceDataTypeMessage ();

        case INTERFACEDATATYPECONFIGUREINTERFACEDATATYPE:
            pPrismMessage = new InterfaceDataTypeConfigureInterfaceDataTypeMessage ();
            break;

        case INTERFACECLASSGETINTERFACECLASS:
            pPrismMessage = new InterfaceClassGetInterfaceClassMessage ();
            break;

        case INTERFACECLASSCONFIGUREINTERFACECLASS:
            pPrismMessage = new InterfaceClassConfigureInterfaceClassMessage ();
            break;
        
        case INTERFACENODESPECIFICCONFIGUREINTERFACE:
            pPrismMessage = new InterfaceNodeSpecificConfigureMessage ();
            break;

		case INTERFACECONTAINERINLISTGET:
            pPrismMessage = new InterfaceContainerInListGetMessage ();
			break;

		case INTERFACECONTAINERINLISTCONFIGURE:
            pPrismMessage = new InterfaceContainerInListConfigureMessage ();
			break;

        case VAL_INTERFACE_CONFIGURE_ASSOC_VLAN :
            pPrismMessage = new ValInterfaceConfigureVlanMessage ();
            break;

        case VAL_INTERFACE_INSTALL_DEMO_OBJECTS :
            pPrismMessage = new InterfaceInstallDemoObjectMessage ();
            break;

        case VAL_INTERFACE_WYSEREA_ASYNC_DEMO_MESSAGE:
            pPrismMessage = new WyserEaAsyncDemoMessage ();
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}


void ValInterfaceLocalObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&ValInterfaceLocalObjectManager::bootCompleteForThisLocationEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void ValInterfaceLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId                                           status                     = FRAMEWORK_ERROR;
    WaveBootReason                                       bootReason                 = pWaveAsynchronousContextForBootPhases->getBootReason ();
    
    startTransaction ();
    
    InterfaceNodeSpecificLocalManagedObject *pNodeSpecificInterface = new InterfaceNodeSpecificLocalManagedObject (this);
    InterfacePriorityLocalManagedObject *pInterfacePriority = NULL;
    InterfaceDataTypeLocalManagedObject *pInterfaceDataType = NULL;

	pNodeSpecificInterface->setNsVlan (10);
	pNodeSpecificInterface->setNsEthernet (100);
    
    if ((WAVE_BOOT_FIRST_TIME_BOOT == bootReason) || (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason))
    {
        // Create the interfacepriority managed object
        pInterfacePriority = new InterfacePriorityLocalManagedObject (this);
        pInterfacePriority->setVlanPriority (10);
        pInterfacePriority->setEthernetPriority (100);

        // Create the child interfaceclass MO
        InterfaceClassLocalManagedObject *pInterfaceClass = new InterfaceClassLocalManagedObject (this);
        pInterfaceClass->setVlanClass (10);
        pInterfaceClass->setEthernetClass (100);

        // Set the Owner of interfaceclass to be interfacepriority
        pInterfaceClass->setOwnerManagedObjectId (pInterfacePriority->getObjectId());
        // Add the interfaceclass onto the vector of interfacepriority children
        pInterfacePriority->setChild (WaveManagedObjectPointer<InterfaceClassLocalManagedObject> (pInterfaceClass));

        // for datatype singleton also, we need to fill in the values just once for x nodes in cluster
        pInterfaceDataType = new InterfaceDataTypeLocalManagedObject (this);
        BitMap bitMap;
        bitMap.setAllValues(8);
        pInterfaceDataType->setBitMap(bitMap);

        Date date;
        date.fromString("2011-12-24");
        pInterfaceDataType->setDate(date);

        Time time;
        time.fromString("18:49:22.000228");
        pInterfaceDataType->setTime(time);

        DateTime datetime;
        datetime.fromString("2010-05-10 18:29:22");
        pInterfaceDataType->setDateTime(datetime);

        Decimal64 decimal64;
        decimal64.fromString("93523.22");
        pInterfaceDataType->setDecimal64(decimal64);

        // set the union type being used & the value
        pInterfaceDataType->setUtypeSi32(22);
        pInterfaceDataType->setUtype("utypeSi32");

        IpV4AddressNetworkMask ipV4NetworkMask;
        ipV4NetworkMask.fromString ("255.255.0.0/16");
        pInterfaceDataType->setIpV4AddressNetworkMask (ipV4NetworkMask);

        IpV6AddressNetworkMask ipV6NetworkMask;
        ipV6NetworkMask.fromString("2001:DB8::1428:57A8/125");
        pInterfaceDataType->setIpV6AddressNetworkMask (ipV6NetworkMask);

        SnmpObjectId  snmpObjectId;
        snmpObjectId.fromString("1.2.123.33.4.23");
        pInterfaceDataType->setSnmpObjectId(snmpObjectId);

        StringUC stringUC("defaultstring", true);
        pInterfaceDataType->setStringUC(stringUC);

        BoolUC boolUC;
        boolUC.setBoolValue (true);
        boolUC.setIsUserConfigured (false);
        pInterfaceDataType->setBoolUC (boolUC);    

        UI32UC ui32UC (10, true);
        pInterfaceDataType->setUI32UC(ui32UC);
    
        VirgoElement virgoElement = DCM;
        EnumUC enumUC (virgoElement, true);
        pInterfaceDataType->setEnumUC(enumUC);
   
        UI32Range    rangeIntegers;
        string       rangeIntegerString = "1,2-5,9,10-13";

        if (true == rangeIntegers.isAValidString (rangeIntegerString))
        {
            rangeIntegers.fromString (rangeIntegerString);
            pInterfaceDataType->setUI32Range (rangeIntegers);
        }
        rangeIntegerString = "0";

        if (true == rangeIntegers.isAValidString (rangeIntegerString))
        {
            rangeIntegers.fromString (rangeIntegerString);
            pInterfaceDataType->setUI32Range2 (rangeIntegers);
        }

        MACRange    rangeMac;
        string      rangeMacString = "0000.0000.0000";

        if (true == rangeMac.isAValidString (rangeMacString))
        {
            rangeMac.fromString (rangeMacString);
            pInterfaceDataType->setMacRange(rangeMac);
        }

        UI8 UI8Default = 0;
        pInterfaceDataType->setUI8Default (UI8Default);

        UI16 UI16Default = 0;
        pInterfaceDataType->setUI16Default (UI16Default);

        UI64 UI64Default = 0;
        pInterfaceDataType->setUI64Default (UI64Default);

        IpV4Address ipV4Address ("100.0.0.1");
        IpV4AddressUC ipV4AddressUC (ipV4Address, true);
        pInterfaceDataType->setIpV4AddressUC (ipV4AddressUC);

        vector<UI32> ui32Vector;
        ui32Vector.push_back (1);
        ui32Vector.push_back (2);
        ui32Vector.push_back (3);
        UI32VectorUC ui32VectorUC (ui32Vector, true);
        pInterfaceDataType->setUI32VectorUC (ui32VectorUC);

        vector<string> stringVector;
        stringVector.push_back ("default string");
        StringVectorUC stringVectorUC (stringVector, true);
        pInterfaceDataType->setStringVectorUC (stringVectorUC);

        IpV4Address hostIpV4Address ("100.0.0.1");
        HostUC hostUC (hostIpV4Address, true);
        pInterfaceDataType->setHostUC (hostUC);

        UI64UC ui64UC (640, true);
        pInterfaceDataType->setUI64UC(ui64UC);

        Uuid   uuid;
        pInterfaceDataType->setUuid(uuid);
    }

    status = commitTransaction ();


    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::Install: Installed node specific interface.");

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::Install: Failed to install node specific interface.");
        //prismAssert (false, __FILE__, __LINE__);
    }

    delete (pNodeSpecificInterface);
    delete pInterfacePriority;
    delete pInterfaceDataType;

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void ValInterfaceLocalObjectManager::InstallDemoObjectMessageHandler (InterfaceInstallDemoObjectMessage *message)
{
    // WARNING                WARNING                WARNING
    // This is demo code - must not be viewed as production code.
    // This portion of the code will be thrown away in favor of real implementation
    // So do not worry about hard coded stuff here.
    trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::install: Installing 512 interfaces by default for demo");

    UI32                                                 i                          = 0;
    SI32                                                 j                          = 0;
    ResourceId                                           status                     = FRAMEWORK_ERROR;
    LocationId                                           thisLocationId             = FrameworkToolKit::getThisLocationId ();
    UI32                                                 x                          = (thisLocationId/256)*512*5;
    vector<InterfaceLocalManagedObject *>                interfaces;
    vector<InterfaceCompositionLocalManagedObject *>     interfacesComposition;
    vector<InterfaceNDeepCompositionLocalManagedObject *>     interfacesNDeepComposition;
    vector<InterfaceCompositionLocalManagedObject *>     interfacesNDeepGChildComposition;
    vector<InterfaceContainerInListLocalManagedObject *> interfaceContainerInList;
    vector<DcmInterfaceAssociationLocalManagedObject *>  interfacesAssociation;
    vector<InterfaceSampleVlanLocalManagedObject *>      interfaceSampleVlanVector;
	vector<SI32>                                         si32Vector;
    vector<IpV4Address>                                  ipV4Vector;
    vector<IpV6Address>                                  ipV6Vector;


    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, InterfaceLocalManagedObject::getClassName ());

    if (pWaveManagedObjects->size () > 0)
    {
        trace (TRACE_LEVEL_WARN, "ValInterfaceLocalObjectManager::InstallDemoObjectMessageHandler Demo Objects are Already Installed");

        for (UI32 k = 0; k < pWaveManagedObjects->size (); k++)
        {
            delete (*pWaveManagedObjects)[k];
        }

        delete pWaveManagedObjects;

        message->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (message);
        return;
    }
    else
    {
        delete pWaveManagedObjects;
    }

	for (j=0; j < 20; j++)
		si32Vector.push_back (j*10);

    for (i=0; i < 4; i++)
    {
        IpV4Address ip_interface (string ("") + i + string (".") + i + string (".") + i + string (".") + i);
        IpV6Address ip6_interface ("fe80::21e:bff:febf:47d4");
        ipV4Vector.push_back (ip_interface);
        ipV6Vector.push_back (ip6_interface);
    }

    startTransaction ();

    SI32 intToStoreInInterfaceTuple = 0;
    intToStoreInInterfaceTuple = DcmToolKit::getLocalMappedId ();
    intToStoreInInterfaceTuple = (intToStoreInInterfaceTuple << 16);

    for (i = 0; i < 512; i++)
    {
        InterfaceLocalManagedObject *pInterface            = new InterfaceLocalManagedObject (this);
        char       formattedCounter[16];

        snprintf (formattedCounter, 16, "%03u", i);

		IpV4Address ip_interface (string ("") + (i % 256) + string (".") + (i % 256) + string (".") + (i % 256) + string (".") + (i % 256));
		IpV4Address ip_interface_def (string ("255.255.255.255") ); 
		IpV6Address ip6_interface ("fe80::21e:bff:febf:47d4");
        IpVxAddress ipVx_interface ("fe80::21e:bff:febf:47d4");
		MacAddress  macAddr ("00:1E:0B:BF:47:D4");
		MacAddress2  macAddr2 ("001E.0BBF.47D4");
        
		WorldWideName wwn ("20:00:00:e0:69:41:54:02");
		Protocol protocol = UDP;
		ErrorLevel	errorLevel	= DCM_DEBUG;
        pInterface->setString ("d-e-f-a-u-l-t-s-t-r-i-n-g");
        pInterface->setIpAddress  (ip_interface);
        pInterface->setIpAddressDef  (ip_interface_def);
        pInterface->setMtu8       (50);
        pInterface->setMtu16      (750);
        pInterface->setMtu32      (NSM_MTU_DEF_VAL);
        pInterface->setMtu32Def   (NSM_MTU_DEF_VAL);  //this will set the default value.
        pInterface->setMtu32Vector(si32Vector);
        pInterface->setMtu8u      (250);
        pInterface->setMtu16u     (1000);
        pInterface->setMtu32u     (NSM_MTU_DEF_VAL);
		pInterface->setMtu32udef  (NSM_MTU_DEF_VAL);
        pInterface->setMtu64u     (100);
        pInterface->setIsEthernet (true);
        pInterface->setIp6Address     (ip6_interface);
        pInterface->setIpVxAddress    (ipVx_interface);
        pInterface->setMacAddress     (macAddr);
        pInterface->setMacAddress2     (macAddr2);
        pInterface->setWorldWideName  (wwn);
	    pInterface->setEnum           (protocol);
	    // pInterface->setErrorLevelEnum (DCM_ERRORLEVEL_MAX);
		pInterface->setErrorLevelEnum (errorLevel);
        pInterface->setIpV4Vector (ipV4Vector);
        pInterface->setIpV6Vector (ipV6Vector);
        pInterface->setInterfaceTuple   (intToStoreInInterfaceTuple + i);

		if (i > 256)
		{
            pInterface->setShutdown (false);
		}
		else
        {
            pInterface->setShutdown (true);
		}

        pInterface->setName       (string ("") + DcmToolKit::getLocalMappedId () + string ("/0/") + string (formattedCounter));
        interfaces.push_back (pInterface);

        ///////////////////////////////////////////////////////
        /// For Composition
        /////////////////////////////////////////////////////////

        InterfaceCompositionLocalManagedObject *pInterfaceComposition            = new InterfaceCompositionLocalManagedObject (this);
        pInterfaceComposition->setName       (string ("") + DcmToolKit::getLocalMappedId () + string ("/0/") + string (formattedCounter));

        vector<WaveManagedObjectPointer<InterfaceCompositionChildLocalManagedObject> > tempVector;
        tempVector = pInterfaceComposition->getChildren ();

        if (0 == i%3)
        {
            SI32 id = 1;
            InterfaceCompositionChildLocalManagedObject *childMO = new InterfaceCompositionChildLocalManagedObject (this, id, false);
            tempVector.push_back (WaveManagedObjectPointer<InterfaceCompositionChildLocalManagedObject> (childMO));

        }

        if (0 == i%2)
        {
            SI32 id2 = 2;
            InterfaceCompositionChildLocalManagedObject *childMO2 = new InterfaceCompositionChildLocalManagedObject (this, id2, true);
            tempVector.push_back (WaveManagedObjectPointer<InterfaceCompositionChildLocalManagedObject> (childMO2));
        }

        pInterfaceComposition->setChildren (tempVector);
        interfacesComposition.push_back (pInterfaceComposition);

        ///////////////////////////////////////////////////////
        /// For ContainerInList
        /////////////////////////////////////////////////////////
        InterfaceContainerInListLocalManagedObject *pInterfaceContainerInList = new InterfaceContainerInListLocalManagedObject (this);

        pInterfaceContainerInList->setName 	(string ("") + DcmToolKit::getLocalMappedId () + string ("/0/") + string (formattedCounter));
        pInterfaceContainerInList->setIp 	(ip_interface);
        pInterfaceContainerInList->setWwn 	(wwn);
        pInterfaceContainerInList->setMtu32 (2500);
        pInterfaceContainerInList->setConfigTypeChoice (NO_CHOICE);
        pInterfaceContainerInList->setFcFabricId (0);
        pInterfaceContainerInList->setEthIpAddress (0);
        interfaceContainerInList.push_back  (pInterfaceContainerInList);

        ///////////////////////////////////////////////////////
        /// For Association
        /////////////////////////////////////////////////////////
        DcmInterfaceAssociationLocalManagedObject *pInterfaceAssociation            = new DcmInterfaceAssociationLocalManagedObject (this);
        pInterfaceAssociation->setName       (string ("") + DcmToolKit::getLocalMappedId () + string ("/0/") + string (formattedCounter));

        InterfaceSampleVlanLocalManagedObject *associationChildMO = new InterfaceSampleVlanLocalManagedObject (this, x);
        pInterfaceAssociation->setVlanId (associationChildMO->getObjectId ());

        interfaceSampleVlanVector.push_back (associationChildMO);

        if (0 == i%2)
        {
            pInterfaceAssociation->setObjectId (ObjectId::NullObjectId);
        }
        else
        {
            pInterfaceAssociation->setObjectId (associationChildMO->getObjectId ());
        }

        x++;

        if (0 == i%2)
        {
            vector<ObjectId> assocVector;
            for (j=0; j<4; j++)
            {
                InterfaceSampleVlanLocalManagedObject *assocVectorChildMO = new InterfaceSampleVlanLocalManagedObject (this, x);
                assocVector.push_back (assocVectorChildMO->getObjectId ());

                interfaceSampleVlanVector.push_back (assocVectorChildMO);

                x++;
            }
            pInterfaceAssociation->setVlanIdVector (assocVector);
            pInterfaceAssociation->setVlanIdVector2 (assocVector);
         }

        interfacesAssociation.push_back (pInterfaceAssociation);

        ///////////////////////////////////////////////////////
        /// For NDeep Composition
        /////////////////////////////////////////////////////////

        InterfaceNDeepCompositionLocalManagedObject *pInterfaceNDeepComposition            = new InterfaceNDeepCompositionLocalManagedObject (this);

        pInterfaceNDeepComposition->setName       (string ("") + DcmToolKit::getLocalMappedId () + string ("/0/") + string (formattedCounter));
        vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > tempNDeepVector;

        tempNDeepVector = pInterfaceNDeepComposition->getNDeepChildren ();

        //if (0 == i%3)
        if (3 == i)
        {
            SI32 id = 3;
            InterfaceNDeepCompositionChildLocalManagedObject *childMO = new InterfaceNDeepCompositionChildLocalManagedObject (this, id, false);

            ////////////////////////////////////////////////////////////////////
            // Set the grand children
            ////////////////////////////////////////////////////////////////////
            vector<WaveManagedObjectPointer<InterfaceNDeepCompositionGChildLocalManagedObject> > tempNDeepGVector;

            InterfaceNDeepCompSkippedGChildLocalManagedObject *sGChildMO3 = new InterfaceNDeepCompSkippedGChildLocalManagedObject (this, id, false, (SI32 &)id);
            tempNDeepGVector = sGChildMO3->getNDeepGChildren ();
            InterfaceNDeepCompositionGChildLocalManagedObject *gChildMO = new InterfaceNDeepCompositionGChildLocalManagedObject (this, (SI32 &)i, false, (SI32 &)i);
            tempNDeepGVector.push_back (WaveManagedObjectPointer<InterfaceNDeepCompositionGChildLocalManagedObject> (gChildMO));

            //InterfaceNDeepCompositionGChildLocalManagedObject *gChildMO2 = new InterfaceNDeepCompositionGChildLocalManagedObject (this, id, false, (SI32 &)id);

            sGChildMO3->setNDeepGChildren (tempNDeepGVector);
            childMO->setNDeepGChild (sGChildMO3);
            
            trace (TRACE_LEVEL_INFO, string("ValInterfaceLocalObjectManager::InstallDemoObjectMessageHandler: setting owner objId to " + (pInterfaceNDeepComposition->getObjectId ()).toString () + " for MO objid " + (childMO->getObjectId ()).toString ()));
            childMO->setOwnerManagedObjectId (pInterfaceNDeepComposition->getObjectId ());
            tempNDeepVector.push_back (WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> (childMO));
        }

        //if (0 == i%2)
        if (2 == i)
        {
            SI32 id2 = 2;
            //SI32 idx = 4;
            InterfaceNDeepCompositionChildLocalManagedObject *childMO2 = new InterfaceNDeepCompositionChildLocalManagedObject (this, id2, true);

            ////////////////////////////////////////////////////////////////////
            // Set the grand children
            ////////////////////////////////////////////////////////////////////
            vector<WaveManagedObjectPointer<InterfaceNDeepCompositionGChildLocalManagedObject> > tempNDeepGVector;


            InterfaceNDeepCompSkippedGChildLocalManagedObject *sGChildMO2 = new InterfaceNDeepCompSkippedGChildLocalManagedObject (this, id2, false, (SI32 &)id2);
            tempNDeepGVector = sGChildMO2->getNDeepGChildren ();
            //InterfaceNDeepCompositionGChildLocalManagedObject *gChildMO = new InterfaceNDeepCompositionGChildLocalManagedObject (this, (SI32 &)i, false, (SI32 &)i);
            //tempNDeepGVector.push_back (WaveManagedObjectPointer<InterfaceNDeepCompositionGChildLocalManagedObject> (gChildMO));

            ///
            //InterfaceNDeepCompositionGChildLocalManagedObject *gChildMO2 = new InterfaceNDeepCompositionGChildLocalManagedObject (this, idx, false, (SI32 &)i);
            //tempNDeepGVector.push_back (WaveManagedObjectPointer<InterfaceNDeepCompositionGChildLocalManagedObject> (gChildMO2));

            //InterfaceNDeepCompositionGChildLocalManagedObject *gChildMO3 = new InterfaceNDeepCompositionGChildLocalManagedObject (this, idx, false, (SI32 &)id2);
            ///


            sGChildMO2->setNDeepGChildren (tempNDeepGVector);
            childMO2->setNDeepGChild (sGChildMO2);

            trace (TRACE_LEVEL_INFO, string("ValInterfaceLocalObjectManager::InstallDemoObjectMessageHandler: setting owner objId to " + (pInterfaceNDeepComposition->getObjectId ()).toString () + " for MO objid " + (childMO2->getObjectId ()).toString ()));
            childMO2->setOwnerManagedObjectId (pInterfaceNDeepComposition->getObjectId ());
            tempNDeepVector.push_back (WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> (childMO2));
        }

        pInterfaceNDeepComposition->setNDeepChildren (tempNDeepVector);
        interfacesNDeepComposition.push_back (pInterfaceNDeepComposition);

    } // for loop instantiating 512 MOs

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::InstallDemoObjectMessageHandler Installed 512 interfaces by default for demo");

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InstallDemoObjectMessageHandler: Failed to install 512 interfaces by default for demo");
        prismAssert (false, __FILE__, __LINE__);
    }

    for (i = 0; i < 512; i++)
    {
        delete (interfaces[i]);
        delete (interfacesComposition[i]);
        delete (interfaceContainerInList[i]);
        delete (interfacesAssociation[i]);
        delete (interfacesNDeepComposition[i]);
    }

    UI32 numberSampleVlans = interfaceSampleVlanVector.size ();

    for (i = 0; i < numberSampleVlans; i++)
    {
        delete (interfaceSampleVlanVector[i]);
    }

    interfaces.clear ();
    interfacesComposition.clear ();
    interfaceContainerInList.clear ();
    interfacesAssociation.clear ();
    interfacesNDeepComposition.clear ();
    interfaceSampleVlanVector.clear ();

    message->setCompletionStatus (status);

    reply (message);
}

void ValInterfaceLocalObjectManager::postboot (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase)
{
    trace (TRACE_LEVEL_INFO, string("ValInterfaceLocalObjectManager::postboot Entered with pass:") + pWaveAsynchronousContextForPostbootPhase->getPassNum());

    pWaveAsynchronousContextForPostbootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForPostbootPhase->callback ();
}

void ValInterfaceLocalObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));
}

void ValInterfaceLocalObjectManager::configureInterfaceMessageHandler (ValInterfaceLocalObjectManagerConfigureInterfaceMessage *pValInterfaceLocalObjectManagerConfigureInterfaceMessage)
{
    ObjectId                     interfaceObjectId   = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getInterfaceObjectId ();
    string                       demoString          = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getString            ();
    IpV4Address                  ipAddress           = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getIpAddress         ();
    IpV4Address                  ipAddressdef        = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getIpAddressDef      ();
    SI8                          mtu8                = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu8              ();
    SI16                         mtu16               = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu16             ();
    SI32                         mtu32               = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu32             ();
    SI32                         mtu32def            = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu32Def          ();
    vector<SI32>                 mtu32Vector         = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu32Vector       ();
    UI8                          mtu8u               = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu8u             ();
    UI16                         mtu16u              = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu16u            ();
    UI32                         mtu32u              = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu32u            ();
    UI32                         mtu32udef           = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu32udef         ();
    UI64                         mtu64u              = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMtu64u            ();
    bool                         isEthernet          = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getIsEthernet        ();
    IpV6Address                  ip6Address          = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getIp6Address        ();
    IpVxAddress                  ipVxAddress         = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getIpVxAddress       ();
    MacAddress                   macAddress          = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMacAddress        ();
    MacAddress2                  macAddress2         = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getMacAddress2       ();
    WorldWideName                wwn                 = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getWorldWideName     ();
    Protocol                     protocol            = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getEnum              ();
    ErrorLevel                   errorLevel          = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getErrorLevelEnum    ();
    vector<IpV4Address>          ipV4Vector          = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getIpV4Vector        ();
    vector<IpV6Address>          ipV6Vector          = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getIpV6Vector        ();
    SI32                         interfaceTuple      = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getInterfaceTuple    ();

    InterfaceLocalManagedObject *pInterface          = NULL;
    WaveManagedObject           *pWaveManagedObject  = NULL;
    ResourceId                   status              = WAVE_MESSAGE_SUCCESS;

    pWaveManagedObject = queryManagedObject (interfaceObjectId);

    if (NULL == pWaveManagedObject)
    {
        trace (TRACE_LEVEL_ERROR, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : No Interface Found with the given ObjectId.");

        pValInterfaceLocalObjectManagerConfigureInterfaceMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pValInterfaceLocalObjectManagerConfigureInterfaceMessage);

        return;
    }

    pInterface = dynamic_cast<InterfaceLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pInterface, __FILE__, __LINE__);

    startTransaction ();

    updateWaveManagedObject (pInterface);

    if ("" != demoString)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting String to " + demoString);
        pInterface->setString (demoString);
    }

    if ("" != ipAddress.toString())
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting IPv4 Address to " + ipAddress.toString());

        pInterface->setIpAddress (ipAddress);
    }

    if ("" != ipAddressdef.toString())
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting IPv4 Address default to " + ipAddressdef.toString());

        pInterface->setIpAddressDef (ipAddressdef);
    }

    if ("" != ip6Address.toString())
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting IPv6 Address to " + ip6Address.toString());

        pInterface->setIp6Address (ip6Address);
    }

    if ("" != ipVxAddress.toString())
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting IPVX Address to " + ipVxAddress.toString());

        pInterface->setIpVxAddress (ipVxAddress);
    }


    if ("00:00:00:00:00:00" != macAddress.toString())
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting Mac Address to " + macAddress.toString());

        pInterface->setMacAddress (macAddress);
    }

    if ("0000.0000.0000" != macAddress2.toString())
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting Mac Address 2 to " + macAddress2.toString());

        pInterface->setMacAddress2 (macAddress2);
    }

    if ("00:00:00:00:00:00:00:00" != wwn.toString())
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting World Wide Name to " + wwn.toString());

        pInterface->setWorldWideName (wwn);
    }

    if (0 != mtu8)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU8 to ") + mtu8);

        pInterface->setMtu8 (mtu8);
    }

    if (0 != mtu16)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU16 to ") + mtu16);

        pInterface->setMtu16 (mtu16);
    }
    if (0 != mtu32)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU32 to ") + mtu32);

        pInterface->setMtu32 (mtu32);
    }

    if (0 != mtu32def)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU32Def to ") + mtu32def);

        pInterface->setMtu32Def (mtu32def);
    }

    if (0 != mtu32Vector.size())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU32Vector with size ") + mtu32Vector.size());

        pInterface->setMtu32Vector (mtu32Vector);
    }
    if (0 != mtu8u)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU8u to ") + mtu8u);

        pInterface->setMtu8u (mtu8u);
    }
    if (0 != mtu16u)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU16u to ") + mtu16u);

        pInterface->setMtu16u (mtu16u);
    }
    if (0 != mtu32u)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU32u to ") + mtu32u);

        pInterface->setMtu32u (mtu32u);
    }
    if (0 != mtu32udef)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU32udef to ") + mtu32udef);

        pInterface->setMtu32udef (mtu32udef);
    }
    if (0 != mtu64u)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting MTU64u to ") + mtu64u);

        pInterface->setMtu64u (mtu64u);
    }

    if (pInterface->getIsEthernet() != isEthernet)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting isEthernet to ") + isEthernet);

        pInterface->setIsEthernet (isEthernet);
    }

    if (protocol != PROTO_MAX)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting enumtype to ") + (UI32) protocol);
        pInterface->setEnum (protocol);
    }

    if (errorLevel != DCM_ERRORLEVEL_MAX)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting enumtype2 to ") + (UI32) errorLevel);
        pInterface->setErrorLevelEnum (errorLevel);
    }

    if (0 != ipV4Vector.size())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting ip4Vector with size ") + ipV4Vector.size());
        pInterface->setIpV4Vector (ipV4Vector);
    }

    if (0 != ipV6Vector.size())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting ip6Vector with size ") + ipV6Vector.size());
        pInterface->setIpV6Vector (ipV6Vector);
    }

    if (0 != interfaceTuple)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Setting interfaceTuple to ") + interfaceTuple);

        pInterface->setInterfaceTuple (interfaceTuple);
    }

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Successfully Cluster-Wide committed the Interface Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::configureInterfaceMessageHandler : Commiting the Interface configuration failed.  Status : " + FrameworkToolKit::localize (status));
    }

    delete pInterface;


    WaveClientSessionContext waveClientSessionContext = pValInterfaceLocalObjectManagerConfigureInterfaceMessage->getWaveClientSessionContext ();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        printfToWaveClientSession (waveClientSessionContext, "Configuring DCM Interface Succeeded.\n");
        printfToWaveClientSessionOver (waveClientSessionContext);
    }
    else
    {
        printfToWaveClientSession (waveClientSessionContext, "Configuring DCM Interface Failed.  Status : %s\n", (FrameworkToolKit::localize (status)).c_str ());
        printfToWaveClientSessionOver (waveClientSessionContext);
    }

    pValInterfaceLocalObjectManagerConfigureInterfaceMessage->setCompletionStatus (status);
    reply (pValInterfaceLocalObjectManagerConfigureInterfaceMessage);
}

void ValInterfaceLocalObjectManager::getInterfaceMessageHandler (ValInterfaceLocalObjectManagerGetInterfaceMessage *pValInterfaceLocalObjectManagerGetInterfaceMessage)
{
    string                       interfaceName = pValInterfaceLocalObjectManagerGetInterfaceMessage->getName ();
    vector<WaveManagedObject *> *pInterfaces   = NULL;
    InterfaceLocalManagedObject *pInterface    = NULL;

    pInterfaces = querySynchronouslyByName (InterfaceLocalManagedObject::getClassName (), interfaceName);

    if (NULL != pInterfaces)
    {
        UI32 numberOfInterfaces = pInterfaces->size ();

        prismAssert (numberOfInterfaces <= 1, __FILE__, __LINE__);

        if (1 == numberOfInterfaces)
        {
            pInterface = dynamic_cast<InterfaceLocalManagedObject *> ((*pInterfaces)[0]);
        }

        pInterfaces->clear ();

        delete pInterfaces;
    }

    if (NULL == pInterface)
    {
        trace (TRACE_LEVEL_ERROR, "ValInterfaceLocalObjectManager::getInterfaceMessageHandler : No Interface Found with the given Name : " + interfaceName);

        pValInterfaceLocalObjectManagerGetInterfaceMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pValInterfaceLocalObjectManagerGetInterfaceMessage);

        return;
    }

    pValInterfaceLocalObjectManagerGetInterfaceMessage->setString     (pInterface->getString     ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setIpAddress  (pInterface->getIpAddress  ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setIpAddressDef  (pInterface->getIpAddressDef ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMtu8   	  (pInterface->getMtu8       ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMtu16	  (pInterface->getMtu16      ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMtu32	  (pInterface->getMtu32      ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMtu32Def	  (pInterface->getMtu32Def   ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMtu8u	  (pInterface->getMtu8u      ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMtu16u	  (pInterface->getMtu16u     ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMtu32u	  (pInterface->getMtu32u     ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMtu32udef  (pInterface->getMtu32udef  ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMtu64u	  (pInterface->getMtu64u     ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setIsEthernet (pInterface->getIsEthernet ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setShutdown   (pInterface->getShutdown   ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setIpV4Vector (pInterface->getIpV4Vector ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setIpV6Vector (pInterface->getIpV6Vector ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setIpVxAddress (pInterface->getIpVxAddress ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setInterfaceTuple (pInterface->getInterfaceTuple ());
    pValInterfaceLocalObjectManagerGetInterfaceMessage->setMacAddress2 (pInterface->getMacAddress2 ());

    delete pInterface;

    pValInterfaceLocalObjectManagerGetInterfaceMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pValInterfaceLocalObjectManagerGetInterfaceMessage);
}

void ValInterfaceLocalObjectManager::configureInterfaceChildrenMessageHandler (ValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage *pValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage)
{
    ObjectId                     interfaceObjectId   = pValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage->getInterfaceChildObjectId ();
    bool                         isAllowed          = pValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage->getIsAllowed        ();

    InterfaceCompositionChildLocalManagedObject *pInterface          = NULL;
    WaveManagedObject           *pWaveManagedObject  = NULL;
    ResourceId                   status              = WAVE_MESSAGE_SUCCESS;

    pWaveManagedObject = queryManagedObject (interfaceObjectId);

    if (NULL == pWaveManagedObject)
    {
        trace (TRACE_LEVEL_ERROR, "ValInterfaceLocalObjectManager::configureInterfaceChildrenMessageHandler : No Interface Found with the given ObjectId.");

        pValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage);

        return;
    }

    pInterface = dynamic_cast<InterfaceCompositionChildLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pInterface, __FILE__, __LINE__);

    startTransaction ();

    updateWaveManagedObject (pInterface);

    if (pInterface->getIsAllowed() != isAllowed)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::configureInterfaceChildrenMessageHandler : Setting isAllowed to ") + isAllowed);

        pInterface->setIsAllowed (isAllowed);
    }

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceChildrenMessageHandler : Successfully Cluster-Wide committed the Interface Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::configureInterfaceChildrenMessageHandler : Commiting the Interface configuration failed.  Status : " + FrameworkToolKit::localize (status));
    }

    delete pInterface;

    pValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage->setCompletionStatus (status);
    reply (pValInterfaceLocalObjectManagerConfigureInterfaceChildrenMessage);
}

void ValInterfaceLocalObjectManager::configureInterfaceAssociationVlanMessageHandler (ValInterfaceConfigureVlanMessage *pValInterfaceConfigureVlanMessage)
{
    ObjectId                     interfaceObjectId   = pValInterfaceConfigureVlanMessage->getInterfaceChildObjectId ();
    UI32                         id          = pValInterfaceConfigureVlanMessage->getId        ();
    vector<UI32>                 vlanIdList2 = pValInterfaceConfigureVlanMessage->getVlanIdVector2 ();

    DcmInterfaceAssociationLocalManagedObject *pInterface          = NULL;
    WaveManagedObject           *pWaveManagedObject  = NULL;
    ResourceId                   status              = WAVE_MESSAGE_SUCCESS;

    pWaveManagedObject = queryManagedObject (interfaceObjectId);

    if (NULL == pWaveManagedObject)
    {
        trace (TRACE_LEVEL_ERROR, "ValInterfaceLocalObjectManager::configureInterfaceAssociationVlanMessageHandler : No Interface Found with the given ObjectId.");

        pValInterfaceConfigureVlanMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pValInterfaceConfigureVlanMessage);

        return;
    }

    pInterface = dynamic_cast<DcmInterfaceAssociationLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pInterface, __FILE__, __LINE__);

    startTransaction ();

    updateWaveManagedObject (pInterface);

    InterfaceSampleVlanLocalManagedObject *associationChildMO = new InterfaceSampleVlanLocalManagedObject (this, id);
    pInterface->setVlanId (associationChildMO->getObjectId ());

    if ( vlanIdList2.size () != 0 )
    {
        ObjectId            objectIdElem;
        vector<ObjectId>    objectIdVector2;
        UI32                i = 0;

        for ( i = 0; i < vlanIdList2.size (); i++)
        {
            objectIdElem.setClassId (vlanIdList2[i]);
            objectIdVector2.push_back(objectIdElem);
        }

        pInterface->setVlanIdVector2 (objectIdVector2);
    }

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configureInterfaceChildrenMessageHandler : Successfully Cluster-Wide committed the Interface Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::configureInterfaceChildrenMessageHandler : Commiting the Interface configuration failed.  Status : " + FrameworkToolKit::localize (status));
    }

    delete pInterface;

    pValInterfaceConfigureVlanMessage->setCompletionStatus (status);
    reply (pValInterfaceConfigureVlanMessage);
}

void  ValInterfaceLocalObjectManager::InterfacePriorityConfigureInterfacePriorityMessageHandler(InterfacePriorityConfigureInterfacePriorityMessage *pInterfacePriorityConfigureInterfacePriorityMessage)
{
    SI32                                 vlanPriority        = pInterfacePriorityConfigureInterfacePriorityMessage->getVlanPriority     ();
    SI32                                 ethernetPriority    = pInterfacePriorityConfigureInterfacePriorityMessage->getEthernetPriority ();
    InterfacePriorityLocalManagedObject *pInterface          = NULL;
    WaveManagedObject                   *pWaveManagedObject  = NULL;
    ResourceId                           status              = WAVE_MESSAGE_SUCCESS;

    vector<WaveManagedObject *> *pResults = querySynchronously (InterfacePriorityLocalManagedObject::getClassName());

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_DEBUG, "ValInterfaceLocalObjectManager::InterfacePriorityConfigureInterfacePriorityMessageHandler : Cannot find Managed object");

            pInterfacePriorityConfigureInterfacePriorityMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
            reply (pInterfacePriorityConfigureInterfacePriorityMessage);
        }
        else if (1 == numberOfResults)
        {
            pWaveManagedObject = (*pResults)[0];
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "ValInterfaceLocalObjectManager::InterfacePriorityConfigureInterfacePriorityMessageHandler : More than one managed object found for Singleton Managed Object");
        }

        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfacePriorityConfigureInterfacePriorityMessageHandler: System failure");
        prismAssert (false, __FILE__, __LINE__);
    }

    pInterface = dynamic_cast<InterfacePriorityLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pInterface, __FILE__, __LINE__);

    startTransaction ();

    updateWaveManagedObject (pInterface);

    if (0 != vlanPriority)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfacePriorityConfigureInterfacePriorityMessageHandler: Setting vlanPriority to ") + vlanPriority);

        pInterface->setVlanPriority (vlanPriority);

    }
    if (0 != ethernetPriority)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfacePriorityConfigureInterfacePriorityMessageHandler: Setting ethernetPriority to ") + ethernetPriority);

        pInterface->setEthernetPriority (ethernetPriority);
    }

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::InterfacePriorityConfigureInterfacePriorityMessageHandler : Successfully Cluster-Wide committed the Interface Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfacePriorityConfigureInterfacePriorityMessageHandler : Commiting the Interface configuration failed.  Status : " + FrameworkToolKit::localize (status));
    }

    delete pInterface;

    pInterfacePriorityConfigureInterfacePriorityMessage->setCompletionStatus (status);
    reply (pInterfacePriorityConfigureInterfacePriorityMessage);
}

void  ValInterfaceLocalObjectManager::InterfacePriorityGetInterfacePriorityMessageHandler(InterfacePriorityGetInterfacePriorityMessage *pInterfacePriorityGetInterfacePriorityMessage)
{
}

void  ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler(InterfaceDataTypeConfigureInterfaceDataTypeMessage *pInterfaceDataTypeConfigureInterfaceDataTypeMessage)
{
    BitMap      bitMap  = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getBitMap ();
    Date        date    = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getDate ();
    Time        time  = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getTime ();
    DateTime    datetime  = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getDateTime ();
    Decimal64   decimal64  = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getDecimal64 ();
    // get both the union types
    WaveUnion   utype = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUtype ();
    SI32        utypeSi32  = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUtypeSi32 ();
    string      utypeString  = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUtypeString ();
    IpV4AddressNetworkMask ipV4NetworkMask = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getIpV4AddressNetworkMask ();
    IpV6AddressNetworkMask ipV6NetworkMask = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getIpV6AddressNetworkMask ();
    SnmpObjectId           snmpObjectId    = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getSnmpObjectId ();
    UI32Range              rangeIntegers   = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUI32Range ();
    UI32Range              rangeIntegers2  = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUI32Range2 ();
    MACRange               rangeMac        = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getMacRange ();
    UI8                    UI8Default      = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUI8Default ();
    UI16                   UI16Default     = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUI16Default ();
    UI64                   UI64Default     = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUI64Default ();
    StringUC               stringUC        = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getStringUC ();
    UI32UC                 ui32UC          = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUI32UC ();
    BoolUC                 boolUC          = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getBoolUC ();
    EnumUC                 enumUC          = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getEnumUC ();
    IpV4AddressUC          ipV4AddressUC   = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getIpV4AddressUC ();
    UI32VectorUC           ui32VectorUC    = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUI32VectorUC ();
    StringVectorUC         stringVectorUC  = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getStringVectorUC ();
    HostUC                 hostUC          = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getHostUC ();
    UI64UC                 ui64UC          = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUI64UC ();
    Uuid                   uuid            = pInterfaceDataTypeConfigureInterfaceDataTypeMessage->getUuid();

    InterfaceDataTypeLocalManagedObject *pInterfaceDataType  = NULL;
    WaveManagedObject                   *pWaveManagedObject  = NULL;
    ResourceId                           status              = WAVE_MESSAGE_SUCCESS;

    vector<WaveManagedObject *> *pResults = querySynchronously (InterfaceDataTypeLocalManagedObject::getClassName());

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_DEBUG, "ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler : Cannot find Managed object");
            pInterfaceDataTypeConfigureInterfaceDataTypeMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
            reply (pInterfaceDataTypeConfigureInterfaceDataTypeMessage);
        }
        else if (1 == numberOfResults)
        {
            pWaveManagedObject = (*pResults)[0];
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler : More than one managed object found for Singleton Managed Object");
        }

        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: System failure");
        prismAssert (false, __FILE__, __LINE__);
    }

    pInterfaceDataType = dynamic_cast<InterfaceDataTypeLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pInterfaceDataType, __FILE__, __LINE__);

    startTransaction ();

    updateWaveManagedObject (pInterfaceDataType);

    //trace(TRACE_LEVEL_INFO, string("fields are, bitmap:") + bitMap.toString() + string(" date:") + date.toString() + string(" time:") + time.toString() + string(" datetime:") + datetime.toString() + string(" decimal64:") + decimal64.toString());
    if ("" != bitMap.toString())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting bitMap to ") + bitMap.toString());
        pInterfaceDataType->setBitMap (bitMap);
    }
    if ("0000-00-00" != date.toString())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting date to ") + date.toString());
        pInterfaceDataType->setDate (date);
    }
    if ("00:00:00.000000" != time.toString())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting time to ") + time.toString());
        pInterfaceDataType->setTime (time);
    }
    if ("0000-00-00 00:00:00" != datetime.toString())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting datetime to ") + datetime.toString());
        pInterfaceDataType->setDateTime (datetime);
    }
    if ("0.00" != decimal64.toString())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting decimal64 to ") + decimal64.toString());
        pInterfaceDataType->setDecimal64 (decimal64.toString());
    }
    // set union fields
    if ("" != utype)
    {
        if (("utypeSi32" == utype) && (0 != utypeSi32))
        {
            trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting utypeSi32 to ") + utypeSi32);
            pInterfaceDataType->setUtypeSi32 (utypeSi32);
        }
        if (("utypeString" == utype) && ("" != utypeString))
        {
            trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting utypeString to ") + utypeString);
            pInterfaceDataType->setUtypeString (utypeString);
        }
    }

    if ("0.0.0.0/0" != ipV4NetworkMask.toString ())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting IPV4NetworkMask to ") + ipV4NetworkMask.toString ());
        pInterfaceDataType->setIpV4AddressNetworkMask (ipV4NetworkMask);
    }

    if ("0:0:0:0:0:0:0:0/0" != ipV6NetworkMask.toString ())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting IPV6NetworkMask to ") + ipV6NetworkMask.toString ());
        pInterfaceDataType->setIpV6AddressNetworkMask (ipV6NetworkMask);
    }

    if ("0" != snmpObjectId.toString ())     {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting SnmpObjectId to ") + snmpObjectId.toString ());
        pInterfaceDataType->setSnmpObjectId (snmpObjectId);
    }

    if ("0" != rangeIntegers.toString ())   {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting UI32Range to ") + rangeIntegers.toString ());
        pInterfaceDataType->setUI32Range (rangeIntegers);
    }

    if ("0" != rangeIntegers2.toString ())   {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting UI32Range2 to ") + rangeIntegers2.toString ());
        pInterfaceDataType->setUI32Range2 (rangeIntegers2);
    }

    if ("0000.0000.0000" != rangeMac.toString ())   {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting MACRange to ") + rangeMac.toString ());
        pInterfaceDataType->setMacRange (rangeMac);
    }

    if (0 != UI8Default)   {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting UI8Default to ") + UI8Default);
        pInterfaceDataType->setUI8Default (UI8Default);
    }

    if (0 != UI16Default)   {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting UI16Default to ") + UI16Default);
        pInterfaceDataType->setUI16Default (UI16Default);
    }

    if (0 != UI64Default)   {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting UI64Default to ") + UI64Default);
        pInterfaceDataType->setUI64Default (UI64Default);
    }

    if ("" != stringUC.getStringValue ()) 
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting StringUC to ") + stringUC.getStringValue ());
        pInterfaceDataType->setStringUC (stringUC);
    }

    if (0 != ui32UC.getUI32Value ()) 
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting UI32UC to ") + ui32UC.getUI32Value ());
        pInterfaceDataType->setUI32UC (ui32UC);
    }

    if (false != boolUC.getIsUserConfigured ())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting BoolUC to ") + boolUC.getBoolValue ());
        pInterfaceDataType->setBoolUC (boolUC);
    }

    if (999 != enumUC.getUI32Value ())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting EnumUC to ") + enumUC.getUI32Value ());
        pInterfaceDataType->setEnumUC (enumUC);
    }

    if ("0.0.0.0" != ipV4AddressUC.getIpV4AddressValue ().toString ())
    {
         trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting IpV4AddressUC to ") + ipV4AddressUC.getIpV4AddressValue ().toString ());
         pInterfaceDataType->setIpV4AddressUC (ipV4AddressUC);
    }

    if (0 != (stringVectorUC.getStringVector ()).size ())
    {
        string vectorAsString = "";
        stringVectorUC.toString (vectorAsString);
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting StringVectorUC to ") + vectorAsString);
        pInterfaceDataType->setStringVectorUC (stringVectorUC);
    }

    if (0 != (ui32VectorUC.getUI32Vector ()).size ()) 
    {
        string vectorAsString = "";
        ui32VectorUC.toString (vectorAsString);
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting UI32VectorUC to ") + vectorAsString); 
        pInterfaceDataType->setUI32VectorUC (ui32VectorUC);
    }

    if (WAVE_UNKNOWN_HOSTTYPE != hostUC.getHostType ()) 
    {
        if ((WAVE_DOMAINNAME == hostUC.getHostType ()) && ("" != hostUC.getHostValue ())) 
        {
            trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting HostUC to ") + hostUC.getHostValue ());
            pInterfaceDataType->setHostUC (hostUC);
        }
        else if ((WAVE_IPV4ADDRESS == hostUC.getHostType ()) && 
            ("0.0.0.0" != hostUC.getHostValue ())) 
        {
            trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting HostUC to ") + hostUC.getHostValue ());
            pInterfaceDataType->setHostUC (hostUC);
        }
        else if ((WAVE_IPV6ADDRESS == hostUC.getHostType ()) && 
            ("0:0:0:0:0:0:0:0" != hostUC.getHostValue ()))
        {
            trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting HostUC to ") + hostUC.getHostValue ());
            pInterfaceDataType->setHostUC (hostUC);
        }
    }

    if (0 != ui64UC.getUI64Value ())
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting UI64UC to ") + ui64UC.getUI64Value ());
        pInterfaceDataType->setUI64UC (ui64UC);
    }

    if (36 == uuid.getUuidString().length())
    {
    	trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler: Setting Uuid to ") + uuid.getUuidString ());
    	pInterfaceDataType->setUuid (uuid);
    }

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler : Successfully Cluster-Wide committed the DataType Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfaceDataTypeConfigureInterfaceDataTypeMessageHandler : Commiting the DataType configuration failed.  Status : " + FrameworkToolKit::localize (status));
    }

    delete pInterfaceDataType;

    pInterfaceDataTypeConfigureInterfaceDataTypeMessage->setCompletionStatus (status);
    reply (pInterfaceDataTypeConfigureInterfaceDataTypeMessage);
}

void  ValInterfaceLocalObjectManager::InterfaceDataTypeGetInterfaceDataTypeMessageHandler(InterfaceDataTypeGetInterfaceDataTypeMessage *pInterfaceDataTypeGetInterfaceDataTypeMessage)
{
    WaveManagedObject                   *pWaveManagedObject   = NULL;
    InterfaceDataTypeLocalManagedObject *pInterfaceDataType   = NULL;

    vector<WaveManagedObject *> *pResults = querySynchronously (InterfaceDataTypeLocalManagedObject::getClassName());


    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_DEBUG, "ValInterfaceLocalObjectManager::InterfaceDataTypeGetInterfaceDataTypeMessageHandler : Cannot find Managed object");
            pInterfaceDataTypeGetInterfaceDataTypeMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
            reply (pInterfaceDataTypeGetInterfaceDataTypeMessage);
        }
        else if (1 == numberOfResults)
        {
            pWaveManagedObject = (*pResults)[0];
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "ValInterfaceLocalObjectManager::InterfaceDataTypeGetInterfaceDataTypeMessageHandler : More than one managed object found         for Singleton Managed Object");
        }
       pResults->clear ();
       delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfaceDataTypeGetInterfaceDataTypeMessageHandler: System failure");
        prismAssert (false, __FILE__, __LINE__);
    }

    pInterfaceDataType = dynamic_cast<InterfaceDataTypeLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pInterfaceDataType, __FILE__, __LINE__);

    pInterfaceDataTypeGetInterfaceDataTypeMessage->setBitMap  (pInterfaceDataType->getBitMap ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setDate  (pInterfaceDataType->getDate ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setTime  (pInterfaceDataType->getTime ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setDateTime  (pInterfaceDataType->getDateTime ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setDecimal64  (pInterfaceDataType->getDecimal64 ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUtype  (pInterfaceDataType->getUtype ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUtype  (pInterfaceDataType->getUtype ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setIpV4AddressNetworkMask (pInterfaceDataType->getIpV4AddressNetworkMask ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setIpV6AddressNetworkMask (pInterfaceDataType->getIpV6AddressNetworkMask ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setSnmpObjectId           (pInterfaceDataType->getSnmpObjectId ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUI32Range (pInterfaceDataType->getUI32Range ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUI32Range2 (pInterfaceDataType->getUI32Range2 ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setMacRange (pInterfaceDataType->getMacRange ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUI8Default (pInterfaceDataType->getUI8Default ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUI16Default (pInterfaceDataType->getUI16Default ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUI64Default (pInterfaceDataType->getUI64Default ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setStringUC (pInterfaceDataType->getStringUC ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUI32UC (pInterfaceDataType->getUI32UC ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setBoolUC (pInterfaceDataType->getBoolUC ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setEnumUC (pInterfaceDataType->getEnumUC ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setIpV4AddressUC (pInterfaceDataType->getIpV4AddressUC ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setStringVectorUC (pInterfaceDataType->getStringVectorUC ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUI32VectorUC (pInterfaceDataType->getUI32VectorUC ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setHostUC (pInterfaceDataType->getHostUC ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUI64UC (pInterfaceDataType->getUI64UC ());
    pInterfaceDataTypeGetInterfaceDataTypeMessage->setUuid (pInterfaceDataType->getUuid ());

    delete pInterfaceDataType;

    pInterfaceDataTypeGetInterfaceDataTypeMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pInterfaceDataTypeGetInterfaceDataTypeMessage);

}
void  ValInterfaceLocalObjectManager::InterfaceClassConfigureInterfaceClassMessageHandler(InterfaceClassConfigureInterfaceClassMessage *pInterfaceClassConfigureInterfaceClassMessage)
{
    SI32                                 vlanClass        = pInterfaceClassConfigureInterfaceClassMessage->getVlanClass     ();
    SI32                                 ethernetClass    = pInterfaceClassConfigureInterfaceClassMessage->getEthernetClass ();
    InterfaceClassLocalManagedObject *pInterface          = NULL;
    WaveManagedObject                   *pWaveManagedObject  = NULL;
    ResourceId                           status              = WAVE_MESSAGE_SUCCESS;

    vector<WaveManagedObject *> *pResults = querySynchronously (InterfaceClassLocalManagedObject::getClassName());

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_DEBUG, "ValInterfaceLocalObjectManager::InterfaceClassConfigureInterfaceClassMessageHandler : Cannot find Managed object");

            pInterfaceClassConfigureInterfaceClassMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
            reply (pInterfaceClassConfigureInterfaceClassMessage);
        }
        else if (1 == numberOfResults)
        {
            pWaveManagedObject = (*pResults)[0];
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "ValInterfaceLocalObjectManager::InterfaceClassConfigureInterfaceClassMessageHandler : More than one managed object found for Singleton Managed Object");
        }

        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfaceClassConfigureInterfaceClassMessageHandler: System failure");
        prismAssert (false, __FILE__, __LINE__);
    }

    pInterface = dynamic_cast<InterfaceClassLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pInterface, __FILE__, __LINE__);

    startTransaction ();

    updateWaveManagedObject (pInterface);

    if (0 != vlanClass)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceClassConfigureInterfaceClassMessageHandler: Setting vlanClass to ") + vlanClass);

        pInterface->setVlanClass (vlanClass);

    }
    if (0 != ethernetClass)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceClassConfigureInterfaceClassMessageHandler: Setting ethernetClass to ") + ethernetClass);

        pInterface->setEthernetClass (ethernetClass);
    }

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::InterfaceClassConfigureInterfaceClassMessageHandler : Successfully Cluster-Wide committed the Interface Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfaceClassConfigureInterfaceClassMessageHandler : Commiting the Interface configuration failed.  Status : " + FrameworkToolKit::localize (status));
    }

    delete pInterface;

    pInterfaceClassConfigureInterfaceClassMessage->setCompletionStatus (status);
    reply (pInterfaceClassConfigureInterfaceClassMessage);
}

void ValInterfaceLocalObjectManager::InterfaceClassGetInterfaceClassMessageHandler(InterfaceClassGetInterfaceClassMessage *pInterfaceClassGetInterfaceClassMessage)
{
}

// For ContainerInList
void ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHandler(InterfaceContainerInListConfigureMessage *pInterfaceContainerInListConfigureMessage)
{
    ObjectId     	interfaceObjectId   = pInterfaceContainerInListConfigureMessage->getInterfaceObjectId ();
    IpV4Address  	ipAddress           = pInterfaceContainerInListConfigureMessage->getIp				  ();
    WorldWideName   wwn                 = pInterfaceContainerInListConfigureMessage->getWwn     		  ();
    SI32         	mtu32               = pInterfaceContainerInListConfigureMessage->getMtu32             ();
    UI32         	configTypeChoice    = pInterfaceContainerInListConfigureMessage->getConfigTypeChoice  ();
    SI32         	fcFabricId          = pInterfaceContainerInListConfigureMessage->getFcFabricId        ();
    SI32         	ethIpAddress        = pInterfaceContainerInListConfigureMessage->getEthIpAddress      ();

    InterfaceContainerInListLocalManagedObject 	*pInterface          = NULL;
    WaveManagedObject           				*pWaveManagedObject  = NULL;
    ResourceId                   				status               = WAVE_MESSAGE_SUCCESS;

    pWaveManagedObject = queryManagedObject (interfaceObjectId);

    if (NULL == pWaveManagedObject)
    {
        trace (TRACE_LEVEL_ERROR, "ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHandler : No Interface Found with the given ObjectId.");

        pInterfaceContainerInListConfigureMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
        reply (pInterfaceContainerInListConfigureMessage);

        return;
    }

    pInterface = dynamic_cast<InterfaceContainerInListLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pInterface, __FILE__, __LINE__);

    startTransaction ();

    updateWaveManagedObject (pInterface);

    if ("" != ipAddress.toString())
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHandler : Setting IPv4 Address to " + ipAddress.toString());

        pInterface->setIp (ipAddress);
    }

    if ("00:00:00:00:00:00:00:00" != wwn.toString())
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHandler : Setting World Wide Name to " + wwn.toString());

        pInterface->setWwn (wwn);
    }

    if (0 != mtu32)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHandler : Setting MTU32 to ") + mtu32);

        pInterface->setMtu32 (mtu32);
    }

    if ((0 != fcFabricId) && (FIBRECHANNEL_TYPE == configTypeChoice))
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHandler : Setting Fabric ID to ") + fcFabricId);

        pInterface->setFcFabricId (fcFabricId);
        pInterface->setConfigTypeChoice (FIBRECHANNEL_TYPE);
    }

    if ((0 != ethIpAddress) && (ETHERNET_TYPE == configTypeChoice))
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHandler : Setting Ethernet IP Address to ") + ethIpAddress);

        pInterface->setEthIpAddress (ethIpAddress);
        pInterface->setConfigTypeChoice (ETHERNET_TYPE);
    }

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHa : Successfully Cluster-Wide committed the Interface Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfaceContainerInListConfigureMessageHa : Commiting the Interface configuration failed.  Status : " + FrameworkToolKit::localize (status));
    }

    delete pInterface;

    pInterfaceContainerInListConfigureMessage->setCompletionStatus (status);
    reply (pInterfaceContainerInListConfigureMessage);
}

void  ValInterfaceLocalObjectManager::InterfaceContainerInListGetMessageHandler(InterfaceContainerInListGetMessage *pInterfaceClassGetInterfaceClassMessage)
{
}

void  ValInterfaceLocalObjectManager::InterfaceNodeSpecificConfigureMessageHandler(InterfaceNodeSpecificConfigureMessage *pInterfaceNodeSpecificConfigureMessage)
{
    SI32                                 nsVlan = pInterfaceNodeSpecificConfigureMessage->getNsVlan     ();
    SI32                                 nsEthernet = pInterfaceNodeSpecificConfigureMessage->getNsEthernet ();
    InterfaceNodeSpecificLocalManagedObject *pInterface          = NULL;
    WaveManagedObject                   *pWaveManagedObject  = NULL;
    ResourceId                           status              = WAVE_MESSAGE_SUCCESS;

    //vector<WaveManagedObject *> *pResults = querySynchronously (InterfaceNodeSpecificLocalManagedObject::getClassName());

    LocationId locationId = DcmToolKit::getLocationIdFromMappedId (DcmToolKit::getLocalMappedId ());

    vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId (locationId, InterfaceNodeSpecificLocalManagedObject::getClassName());

	if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_DEBUG, "ValInterfaceLocalObjectManager::InterfaceNodeSpecificConfigureMessageHandler : Cannot find Managed object");

			pInterfaceNodeSpecificConfigureMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
            reply (pInterfaceNodeSpecificConfigureMessage);
        }
        else if (1 == numberOfResults)
        {
            pWaveManagedObject = (*pResults)[0];
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::InterfaceNodeSpecificConfigureMessageHandler : More than one managed object found for Singleton Managed Object");
        
            // Since there are multiple Managed Objects we need to query them based on current location id
	        pInterfaceNodeSpecificConfigureMessage->setCompletionStatus (status);
            reply (pInterfaceNodeSpecificConfigureMessage);
        }

        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfaceNodeSpecificConfigureMessageHandler: System failure");
        prismAssert (false, __FILE__, __LINE__);
    }

    pInterface = dynamic_cast<InterfaceNodeSpecificLocalManagedObject *> (pWaveManagedObject);

    prismAssert (NULL != pInterface, __FILE__, __LINE__);

    startTransaction ();

    updateWaveManagedObject (pInterface);

    if (0 != nsVlan)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceNodeSpecificConfigureMessageHandler: Setting nsVlan to ") + nsVlan);

        pInterface->setNsVlan (nsVlan);

    }
    if (0 != nsEthernet)
    {
        trace (TRACE_LEVEL_INFO, string ("ValInterfaceLocalObjectManager::InterfaceNodeSpecificConfigureMessageHandler: Setting nsEthernet to ") + nsEthernet);

        pInterface->setNsEthernet (nsEthernet);
    }

    status = commitTransaction ();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::InterfaceNodeSpecificConfigureMessageHandler : Successfully Cluster-Wide committed the Interface Configuration");
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "ValInterfaceLocalObjectManager::InterfaceNodeSpecificConfigureMessageHandler : Commiting the Interface configuration failed.  Status : " + FrameworkToolKit::localize (status));
    }

    delete pInterface;

	pInterfaceNodeSpecificConfigureMessage->setCompletionStatus (status);
    reply (pInterfaceNodeSpecificConfigureMessage);
}


void ValInterfaceLocalObjectManager::configReplayEnd (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd)
{
    trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::configReplayEnd : IN INTERFACE LOCAL OBJECT MANAGER ");

    pWaveAsynchronousContextForConfigReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForConfigReplayEnd->callback ();
}

void ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageHandler (WyserEaAsyncDemoMessage *pWyserEaAsyncDemoMessage)
{
    if (WYSEREA_ASYNC_DEMO_MESSAGE_RECALL_TEST == pWyserEaAsyncDemoMessage->m_commandCode)
    {
        prismUSleep (1000 * (pWyserEaAsyncDemoMessage->m_sleepDuration));

        reply (pWyserEaAsyncDemoMessage);

        return;
    }

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageMultipleReplyStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ValInterfaceLocalObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&ValInterfaceLocalObjectManager::prismLinearSequencerFailedStep)
    };

    WyserEaAsyncDemoLinearSequencerContext *pWyserEaAsyncDemoLinearSequencerContext = new WyserEaAsyncDemoLinearSequencerContext (reinterpret_cast<PrismMessage *> (pWyserEaAsyncDemoMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pWyserEaAsyncDemoLinearSequencerContext->m_numberOfExtraRepliesRemaining   = pWyserEaAsyncDemoMessage->m_numberOfExtraReplies;
    pWyserEaAsyncDemoLinearSequencerContext->m_delayBetweenReplies             = pWyserEaAsyncDemoMessage->m_delayBetweenReplies;

    pWyserEaAsyncDemoLinearSequencerContext->start ();
}

void ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageMultipleReplyStep (WyserEaAsyncDemoLinearSequencerContext *pWyserEaAsyncDemoLinearSequencerContext)
{
    ResourceId stepStatus = WAVE_MESSAGE_SUCCESS;

    WyserEaAsyncDemoMessage *pWyserEaAsyncDemoMessage = dynamic_cast<WyserEaAsyncDemoMessage *> (pWyserEaAsyncDemoLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pWyserEaAsyncDemoMessage, __FILE__, __LINE__);

    if (0 == pWyserEaAsyncDemoLinearSequencerContext->m_numberOfExtraRepliesRemaining)
    {
        pWyserEaAsyncDemoMessage->setIsLastReply (true);

        pWyserEaAsyncDemoMessage->m_demoString = string ("LAST REPLY.\n");

        // success step will do the final reply.
    }
    else if (0 == pWyserEaAsyncDemoLinearSequencerContext->m_delayBetweenReplies)
    {
        for (UI32 i = 0; i < pWyserEaAsyncDemoLinearSequencerContext->m_numberOfExtraRepliesRemaining; i++)
        {
            pWyserEaAsyncDemoMessage->setIsLastReply (false);

            pWyserEaAsyncDemoMessage->m_demoString = string ("REPLY NUMBER ") + (i + 1) + string ("\n");

            reply (pWyserEaAsyncDemoMessage);
        }

        pWyserEaAsyncDemoMessage->setIsLastReply (true);

        pWyserEaAsyncDemoMessage->m_demoString = string ("LAST REPLY.\n");

        // success step will do the final reply.
    }
    else
    {
        // i.e. timer is required, and nonzero extra replies are required.

        pWyserEaAsyncDemoMessage->setIsLastReply (false);

        pWyserEaAsyncDemoMessage->m_demoString = string ("FIRST REPLY\n");

        reply (pWyserEaAsyncDemoMessage);

        trace (TRACE_LEVEL_INFO, "ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageMultipleReplyStep : First Reply done.");

        pWyserEaAsyncDemoLinearSequencerContext->m_numberOfExtraRepliesRemaining--;

        TimerHandle timerHandle; // Can be ignored. (Only needed for cancel.)

        ResourceId status = startTimer (timerHandle, pWyserEaAsyncDemoLinearSequencerContext->m_delayBetweenReplies, reinterpret_cast<PrismTimerExpirationHandler> (&ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageTimerCallback), reinterpret_cast<void *> (pWyserEaAsyncDemoLinearSequencerContext));

        if (FRAMEWORK_SUCCESS != status)
        {
            tracePrintf (TRACE_LEVEL_ERROR, "ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageMultipleReplyStep : startTimer returned [%s]", (FrameworkToolKit::localizeToSourceCodeEnum (status)).c_str ());

            pWyserEaAsyncDemoMessage->setIsLastReply (true);

            stepStatus = WAVE_MESSAGE_ERROR;
        }
        else
        {
            ++(*pWyserEaAsyncDemoLinearSequencerContext);
        }
    }

    pWyserEaAsyncDemoLinearSequencerContext->executeNextStep (stepStatus);
}

void ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageTimerCallback (TimerHandle timerHandle, void *pContext)
{
    ResourceId stepStatus = WAVE_MESSAGE_SUCCESS;

    WyserEaAsyncDemoLinearSequencerContext *pWyserEaAsyncDemoLinearSequencerContext = reinterpret_cast<WyserEaAsyncDemoLinearSequencerContext *> (pContext);

    prismAssert (NULL != pWyserEaAsyncDemoLinearSequencerContext, __FILE__, __LINE__);

    --(*pWyserEaAsyncDemoLinearSequencerContext);

    WyserEaAsyncDemoMessage *pWyserEaAsyncDemoMessage = dynamic_cast<WyserEaAsyncDemoMessage *> (pWyserEaAsyncDemoLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pWyserEaAsyncDemoMessage, __FILE__, __LINE__);

    if (0 != pWyserEaAsyncDemoLinearSequencerContext->m_numberOfExtraRepliesRemaining)
    {
        pWyserEaAsyncDemoMessage->setIsLastReply (false);

        pWyserEaAsyncDemoMessage->m_demoString = string ("REPLIES REMAINING : ") + (pWyserEaAsyncDemoLinearSequencerContext->m_numberOfExtraRepliesRemaining) + string ("\n");

        reply (pWyserEaAsyncDemoMessage);

        pWyserEaAsyncDemoLinearSequencerContext->m_numberOfExtraRepliesRemaining--;

        TimerHandle newTimerHandle;

        ResourceId status = startTimer (newTimerHandle, pWyserEaAsyncDemoLinearSequencerContext->m_delayBetweenReplies, reinterpret_cast<PrismTimerExpirationHandler> (&ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageTimerCallback), pWyserEaAsyncDemoLinearSequencerContext);

        if (FRAMEWORK_SUCCESS != status)
        {
            tracePrintf (TRACE_LEVEL_ERROR, "ValInterfaceLocalObjectManager::WyserEaAsyncDemoMessageTimerCallback : startTimer returned [%s]", (FrameworkToolKit::localizeToSourceCodeEnum (status)).c_str ());

            pWyserEaAsyncDemoMessage->setIsLastReply (true);

            stepStatus = WAVE_MESSAGE_ERROR;
        }
        else
        {
            ++(*pWyserEaAsyncDemoLinearSequencerContext);
        }
    }
    else
    {
        pWyserEaAsyncDemoMessage->setIsLastReply (true);

        pWyserEaAsyncDemoMessage->m_demoString = string ("LAST REPLY\n");
    }

    pWyserEaAsyncDemoLinearSequencerContext->executeNextStep (stepStatus);
}

}
