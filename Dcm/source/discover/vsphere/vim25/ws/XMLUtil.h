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
 * XMLUtil.h
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#ifndef XMLUTIL_H_
#define XMLUTIL_H_
#include <string>
#include <string.h>
#include "discover/vsphere/vim25/WSObject.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <vector>
#include "discover/vsphere/vim25/ListUtil.h"

#define XML_EXTRACTSTRING(X,Y,Z) if(XMLUtil::extractString(X, Y, Z)) {X=X->next;continue;}

#define XML_EXTRACTBOOLEAN(X,Y,Z) if(XMLUtil::extractBoolean(X, Y, Z)) {X=X->next;continue;}


using namespace std;
namespace DcmNs {

    class XMLUtil {

        public:
            XMLUtil();
            virtual ~XMLUtil();
            static bool extractString(xmlNodePtr & childNode, string key,
                    string &val);
            static bool extractString(xmlNodePtr & childNode, string &val);
            static bool extractStringIntoList(xmlNodePtr & node, string key,
                    vector<string*> *list);
            static bool extractBoolean(xmlNodePtr & node, string key,
                    bool &val);
            static bool extractAttributeString(xmlAttrPtr & node, string key,
                    string &val);

            static string beginTag(string tagName, string type);
            static string endTag(string tagName);
            static string serializeString(string tagName, string val);
            static string serializeType(string tagName, string val,string type);
            static string serializeBoolean(string tagName, bool val);

            template<class T>
            static void extractList(xmlNodePtr childNode, string key,
                    void **obj) {
                vector<T *> *_objList = new vector<T*>();
                if (childNode != NULL)
                    ListUtil<T>::populateList(childNode, key,
                            _objList);
                *obj = _objList;
            }

            template<class T>
            static void extractSingleElementToList(xmlNodePtr childNode, string key,
                                void **obj) {
                vector<T *> *_objList = new vector<T*>();
                if (childNode != NULL){
                    T * object = new T();
                    object->deSerialize(childNode);
                    _objList->push_back(object);
               }

                *obj = _objList;
                        }
            template<class T>
            static void extractObject(xmlNodePtr childNode, void **obj) {
                T *_obj = new T();
                if (childNode != NULL)
                    _obj->deSerialize(childNode);
                *obj = _obj;
            }

    };

} /* namespace DcmNs */
#endif /* XMLUTIL_H_ */
