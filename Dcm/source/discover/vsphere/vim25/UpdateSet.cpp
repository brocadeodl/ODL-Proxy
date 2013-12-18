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
 * UpdateSet.cpp
 *
 *  Created on: Apr 9, 2012
 *      Author: rmadapur
 */

#include "UpdateSet.h"

namespace DcmNs {

    UpdateSet::UpdateSet() {
        filterSet = new vector<PropertyFilterUpdate *>();

    }

    UpdateSet::~UpdateSet() {
        ListUtil<PropertyFilterUpdate>::clearListofPointers(filterSet);

    }
    void UpdateSet::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "version", this->version);
            XMLUtil::extractBoolean(childNode, "truncated", this->truncated);

            ListUtil<PropertyFilterUpdate>::extractObjectIntoList(childNode,
                    "filterSet", this->filterSet);
            childNode = childNode->next;
        }
    }

    PropertyFilterUpdate::PropertyFilterUpdate() {
        objectSet = new vector<ObjectUpdate *>();

    }

    PropertyFilterUpdate::~PropertyFilterUpdate() {
        ListUtil<ObjectUpdate>::clearListofPointers(objectSet);
        // delete (objectSet);

    }
    void PropertyFilterUpdate::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            WSObject::extractObject(childNode, "filter", &this->filter);

            ListUtil<ObjectUpdate>::extractObjectIntoList(childNode,
                    "objectSet", this->objectSet);

            childNode = childNode->next;
        }
    }

    ObjectUpdate::ObjectUpdate() {
        changeSet = new vector<PropertyChange *>();
    }

    ObjectUpdate::~ObjectUpdate() {
        ListUtil<PropertyChange>::clearListofPointers(changeSet);
        // delete (changeSet);

    }
    void ObjectUpdate::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            WSObject::extractObject(childNode, "kind", &this->kind);
            WSObject::extractObject(childNode, "obj", &this->obj);
            ListUtil<PropertyChange>::extractObjectIntoList(childNode,
                    "changeSet", this->changeSet);

            childNode = childNode->next;
        }
    }

    ObjectUpdateKind::ObjectUpdateKind() {

    }

    ObjectUpdateKind::~ObjectUpdateKind() {

    }
    void ObjectUpdateKind::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "val", this->val);

            childNode = childNode->next;
        }
    }

    PropertyChange::PropertyChange() {

    }

    PropertyChange::~PropertyChange() {
        if (valType.compare("ArrayOfEvent") == 0) {
           //Since Events are put in the queue and handled by timerExpiry.
           //Memory should not be de-allocated for Events. Just clear the list
           // ListUtil<Event>::clearListofPointers((vector<Event*> *) val);
            vector<Event *> * evtList = (vector<Event *> *)val;
            if(evtList!=NULL){
			   evtList->clear();
               delete (evtList);
            }
        }

    }
    void PropertyChange::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "name", this->name);
            WSObject::extractObject(childNode, "op", &this->op);

            if (xmlStrEqual(childNode->name, BAD_CAST "val")) {
                xmlAttrPtr attribute = childNode->properties;
                char *type = (char *) xmlNodeListGetString(childNode->doc,
                        attribute->children, 1);
                this->valType = type;

                xmlFree(type);

                //If type is Event
                if (valType.compare("ArrayOfEvent") == 0) {
                    XMLUtil::extractList<Event>(childNode, "Event", &this->val);

                }else if(valType.find("Event")!=std::string::npos){
                    //Event  Object
                    XMLUtil::extractSingleElementToList<Event>(childNode, "Event", &this->val);
                   /* Event * event = new Event();
                    vector<Event *> *  evList = new vector<Event *>();
                    evList->push_back(event);
                    event->deSerialize(childNode);
                    this->val = evList;*/

                }

            }

            childNode = childNode->next;
        }
    }
    PropertyChangeOp::PropertyChangeOp() {

    }

    PropertyChangeOp::~PropertyChangeOp() {

    }
    void PropertyChangeOp::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            XMLUtil::extractString(childNode, "val", this->val);
            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
