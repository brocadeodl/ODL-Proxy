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
 * XMLUtil.cpp
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#include "XMLUtil.h"

namespace DcmNs {

    XMLUtil::XMLUtil() {
        // TODO Auto-generated constructor stub

    }

    XMLUtil::~XMLUtil() {
        // TODO Auto-generated destructor stub
    }

    bool XMLUtil::extractString(xmlNodePtr & node, string key, string &val) {

        if (xmlStrEqual(node->name, BAD_CAST key.c_str())) {

            char *value = (char *) xmlNodeListGetString(node->doc,
                    node->children, 1);
            if (value) {
                val = value;
               // printf("%s = %s\n", key.c_str(), val.c_str());
                xmlFree(value);
            }
            return true;

        }
        return false;

    }
    bool XMLUtil::extractString(xmlNodePtr & node, string &val) {

        char *value = (char *) xmlNodeListGetString(node->doc, node->children,
                1);
        if (value) {
            val = value;
            // printf("%s = %s\n", key.c_str(), val.c_str());
            xmlFree(value);
            return true;
        }

        return false;

    }
    bool XMLUtil::extractAttributeString(xmlAttrPtr & node, string key,
            string &val) {

        if (xmlStrEqual(node->name, BAD_CAST key.c_str())) {

            char *value = (char *) xmlNodeListGetString(node->doc,
                    node->children, 1);
            if (value) {
                val = value;
                // printf("%s = %s\n", key.c_str(), val.c_str());
                xmlFree(value);
            }
            return true;

        }
        return false;

    }
    bool XMLUtil::extractStringIntoList(xmlNodePtr & node, string key,
            vector<string*> *list) {
        xmlNodePtr listnode = node;

        if (xmlStrEqual(listnode->name, BAD_CAST key.c_str())) {

            char *value = (char *) xmlNodeListGetString(listnode->doc,
                    listnode->children, 1);
            if (value) {
                string *str1 = new string();
                str1->append(value);

                list->push_back(str1);
                // printf("Ritesh %s = %s\n", key.c_str(), value);
                xmlFree(value);
            }
            return true;

        }
        return false;

    }
    bool XMLUtil::extractBoolean(xmlNodePtr & node, string key, bool &val) {

        if (xmlStrEqual(node->name, BAD_CAST key.c_str())) {

            char *value = (char *) xmlNodeListGetString(node->doc,
                    node->children, 1);
            if (value) {
                val = (strcmp(value, "true") == 0) ? true : false;
                val = (strcmp(value, "TRUE") == 0) ? true : false;
                //printf("%s =%s\n", key.c_str(), value);
                xmlFree(value);
            }

            return true;
        }
        return false;
    }

    string XMLUtil::beginTag(string tagName, string type) {
        string sb;
        sb.append(
                "<" + tagName + " xmlns=\"urn:vim25\"  xsi:type=\"" + type
                        + "\">");
        return sb;
    }

    string XMLUtil::endTag(string tagName) {
        string sb;
        sb.append("</" + tagName + ">");
        return sb;
    }

    string XMLUtil::serializeString(string tagName, string val) {
        string sb;
        string type = "xsd:string";
        sb.append(XMLUtil::beginTag(tagName, type));
        sb.append(val);
        sb.append(XMLUtil::endTag(tagName));
        return sb;
    }
    string XMLUtil::serializeType(string tagName, string val,string  _type) {
            string sb;
            string type = "xsd:"+_type;
            sb.append(XMLUtil::beginTag(tagName, type));
            sb.append(val);
            sb.append(XMLUtil::endTag(tagName));
            return sb;
        }

    string XMLUtil::serializeBoolean(string tagName, bool val) {
        string sb;
        string type = "xsd:boolean";
        sb.append(XMLUtil::beginTag(tagName, type));
        sb.append(val == true ? "true" : "false");
        sb.append(XMLUtil::endTag(tagName));
        return sb;
    }

   /* template<class T>
    void XMLUtil::extractList(xmlNodePtr childNode, void **obj) {
        vector<T *> *_objList;
        ListUtil<T>::createList(childNode, &_objList);
        *obj = _objList;
    }
    template<class T>
    void XMLUtil::extractObject(xmlNodePtr childNode, void **obj) {
        T *_obj = new T();
        _obj->deSerialize(childNode);
        *obj = _obj;
    }*/

} /* namespace DcmNs */
