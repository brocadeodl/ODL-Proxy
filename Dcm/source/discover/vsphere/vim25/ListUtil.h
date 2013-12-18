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
 * ListDelete.h
 *
 *  Created on: Mar 29, 2012
 *      Author: rmadapur
 */

#ifndef LISTDELETE_H_
#define LISTDELETE_H_
#include <vector>
#include <algorithm>
#include <libxml/tree.h>
//#include "vim25/HostProxySwitch.h"
using namespace std;
namespace DcmNs {
    template<class T>
    class ListUtil {
        public:
            void operator()(const T* it) const {
                delete it;
            }
            void static clearListofPointers(vector<T *> *list) {
                std::for_each(list->begin(), list->end(), ListUtil<T>());
                delete (list);
            }
            void static clearPointersFromList(vector<T *> &list) {
                std::for_each(list.begin(), list.end(), ListUtil<T>());

            }

            /**
             * Creates a vector of pointers of Type T
             * Populate the vector with elements of type T
             *
             */
            int static populateList(xmlNodePtr node, string key,
                    vector<T *> *list) {

                if (node != NULL) {
                    xmlNodePtr childNode = node->children;
                    while (childNode != NULL) {
                        extractObjectIntoList(childNode, key, list);
                        childNode = childNode->next;
                    }
                }

                return 0;
            }
            /**
             * Creates an object of Type T and populates the list
             * This function does not create the List
             */
            bool static extractObjectIntoList(xmlNodePtr & node, string key,
                    vector<T*> *list) {

                if (xmlStrEqual(node->name, BAD_CAST key.c_str())) {

                    T *obj = new T();
                    obj->deSerialize(node);
                    list->push_back(obj);
                    return true;

                }
                return false;

            }
    };

} /* namespace DcmNs */
#endif /* LISTDELETE_H_ */
