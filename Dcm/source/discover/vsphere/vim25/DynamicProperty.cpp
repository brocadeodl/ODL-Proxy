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

/*
 * DynamicProperty.cpp
 *
 *  Created on: Mar 25, 2012
 *      Author: rmadapur
 */

#include "DynamicProperty.h"

namespace DcmNs {

    DynamicProperty::DynamicProperty() {
        // TODO Auto-generated constructor stub

    }

    DynamicProperty::~DynamicProperty() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Destruct DyanmicProperty\n");

        if (objType.compare("ArrayOfHostVirtualNic") == 0) {
            ListUtil<HostVirtualNic>::clearListofPointers(
                    (vector<HostVirtualNic*> *) obj);
        }
        else if (objType.compare("ArrayOfGuestNicInfo") == 0) {
            ListUtil<GuestNicInfo>::clearListofPointers(
                    (vector<GuestNicInfo*> *) obj);

        }
        else if (objType.compare("ArrayOfPhysicalNic") == 0) {
            ListUtil<PhysicalNic>::clearListofPointers(
                    (vector<PhysicalNic*> *) obj);

        }
        else if (objType.compare("ArrayOfHostPortGroup") == 0) {

            ListUtil<HostPortGroup>::clearListofPointers(
                    (vector<HostPortGroup*> *) obj);

        }
        else if (objType.compare("ArrayOfManagedObjectReference") == 0) {
            ListUtil<ManagedObjectReference>::clearListofPointers(
                    (vector<ManagedObjectReference*> *) obj);

        }
        else if (objType.compare("ArrayOfHostVirtualSwitch") == 0) {
            ListUtil<HostVirtualSwitch>::clearListofPointers(
                    (vector<HostVirtualSwitch*> *) obj);

        }
        else if (objType.compare("ArrayOfHostProxySwitch") == 0) {
            ListUtil<HostProxySwitch>::clearListofPointers(
                    (vector<HostProxySwitch*> *) obj);
        }
        else if (objType.compare("ArrayOfDistributedVirtualSwitchHostMember")
                == 0) {

            ListUtil<DistributedVirtualSwitchHostMember>::clearListofPointers(
                    (vector<DistributedVirtualSwitchHostMember*> *) obj);

        }
        else if (objType.compare("ArrayOfDVSNetworkResourcePool") == 0) {

                   ListUtil<DVSNetworkResourcePool>::clearListofPointers(
                           (vector<DVSNetworkResourcePool*> *) obj);

               }
        else if (objType.compare("ArrayOfTag") == 0) {

            ListUtil<Tag>::clearListofPointers((vector<Tag*> *) obj);
        }
        else if (objType.compare("xsd:string") == 0) {
            delete ((string *) obj);

        }
        else if (objType.compare("xsd:boolean") == 0) {

            delete ((string *) obj);

        }
        else if (objType.compare("VmwareDistributedVirtualSwitch") == 0) {

            delete ((ManagedObjectReference *) obj);

        }
        else if (objType.compare("Folder") == 0) {

            delete ((ManagedObjectReference *) obj);

        }
        else if (objType.compare("VMwareDVSPortSetting") == 0) {

            delete ((VMwareDVSPortSetting *) obj);

        }
        else if (objType.compare("VirtualMachinePowerState") == 0) {

            delete ((VirtualMachinePowerState *) obj);

        }
        else if (objType.compare("ArrayOfVirtualDevice") == 0) {

            ListUtil<VirtualDevice>::clearListofPointers(
                    (vector<VirtualDevice*> *) obj);

        }else if (objType.compare("TaskInfo") == 0) {

            delete ((TaskInfo *) obj);

        }
		else if(objType.compare("HostSystem") == 0) {
            delete ((ManagedObjectReference *) obj);
         }



    }
    DynamicProperty::DynamicProperty(const DynamicProperty &ref) {
        name = ref.name;
        objType = ref.objType;
        obj = ref.obj;
    }
    void DynamicProperty::operator=(const DynamicProperty &ref) {
        name = ref.name;
        objType = ref.objType;
        obj = ref.obj;
    }

    void DynamicProperty::deSerialize(xmlNodePtr dyPropNode) {
        xmlNodePtr childNode = dyPropNode->children;
        while (childNode != NULL) {
            //Extract Name
            XMLUtil::extractString(childNode, "name", this->name);
            //Extract Type;
            if (xmlStrEqual(childNode->name, BAD_CAST "val")) {
                xmlAttrPtr attribute = childNode->properties;
                char *type = (char *) xmlNodeListGetString(childNode->doc,
                        attribute->children, 1);

                this->objType = type;

                xmlFree(type);

                //If type is HostVirtualNic
                if (objType.compare("ArrayOfHostVirtualNic") == 0) {
                    XMLUtil::extractList<HostVirtualNic>(childNode,
                            "HostVirtualNic", &this->obj);

                }else if(objType.compare("ArrayOfGuestNicInfo") == 0) {
                    XMLUtil::extractList<GuestNicInfo>(childNode,
                                                "GuestNicInfo", &this->obj);

                }
                else if (objType.compare("ArrayOfPhysicalNic") == 0) {

                    XMLUtil::extractList<PhysicalNic>(childNode, "PhysicalNic",
                            &this->obj);

                }
                else if (objType.compare("ArrayOfHostPortGroup") == 0) {
                    XMLUtil::extractList<HostPortGroup>(childNode,
                            "HostPortGroup", &this->obj);

                }
                else if (objType.compare("ArrayOfManagedObjectReference")
                        == 0) {
                    XMLUtil::extractList<ManagedObjectReference>(childNode,
                            "ManagedObjectReference", &this->obj);

                }
                else if (objType.compare("ArrayOfHostVirtualSwitch") == 0) {
                    XMLUtil::extractList<HostVirtualSwitch>(childNode,
                            "HostVirtualSwitch", &this->obj);
                }
                else if (objType.compare("ArrayOfHostProxySwitch") == 0) {
                    XMLUtil::extractList<HostProxySwitch>(childNode,
                            "HostProxySwitch", &this->obj);

                }
                else if (objType.compare(
                        "ArrayOfDistributedVirtualSwitchHostMember") == 0) {
                    XMLUtil::extractList<DistributedVirtualSwitchHostMember>(
                            childNode, "DistributedVirtualSwitchHostMember",
                            &this->obj);

                }
                else if (objType.compare("ArrayOfDVSNetworkResourcePool")
                                        == 0) {
                                    XMLUtil::extractList<DVSNetworkResourcePool>(childNode,
                                            "DVSNetworkResourcePool", &this->obj);

                                }
                else if (objType.compare("ArrayOfTag") == 0) {
                    XMLUtil::extractList<Tag>(childNode, "Tag", &this->obj);
                }
                else if (objType.compare("xsd:string") == 0) {

                    string *str = new string();
                    XMLUtil::extractString(childNode, "val", *str);

                    this->obj = str;

                }
                else if (objType.compare("xsd:boolean") == 0) {

                    string *str = new string();
                    XMLUtil::extractString(childNode, "val", *str);

                    this->obj = str;

                }
                else if (objType.compare("VmwareDistributedVirtualSwitch")
                        == 0) {
                    XMLUtil::extractObject<ManagedObjectReference>(childNode,
                            &this->obj);

                }
                else if (objType.compare("VMwareDVSPortSetting") == 0) {
                    XMLUtil::extractObject<VMwareDVSPortSetting>(childNode,
                            &this->obj);

                }
                else if (objType.compare("VirtualMachinePowerState") == 0) {
                    XMLUtil::extractObject<VirtualMachinePowerState>(childNode,
                            &this->obj);

                }
                else if (objType.compare("ArrayOfVirtualDevice") == 0) {
                    XMLUtil::extractList<VirtualDevice>(childNode,
                            "VirtualDevice", &this->obj);

                }
                else if (objType.compare("Folder") == 0) {
                    XMLUtil::extractObject<ManagedObjectReference>(childNode,
                            &this->obj);

                }
                else if(objType.compare("TaskInfo") == 0) {
                    XMLUtil::extractObject<TaskInfo>(childNode,
                            &this->obj);

                }
				else if(objType.compare("HostSystem") == 0) {
                   XMLUtil::extractObject<ManagedObjectReference>(childNode,
                            &this->obj);
                }

            }

            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
