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
 * Argument.cpp
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#include "Argument.h"

namespace DcmNs {

    Argument::Argument() {
        // TODO Auto-generated constructor stub

    }
    Argument::Argument(string _name, string _type, void *_value):
           name(_name),type(_type),val(_value)
        {

    }

    Argument::~Argument() {

    }

    string Argument::getName() const
    {
        return name;
    }

    string Argument::getType() const
    {
        return type;
    }

    void *Argument::getVal() const
    {
        return val;
    }

    void Argument::setName(string _name)
    {
        this->name = _name;
    }

    void Argument::setType(string _type)
    {
        this->type = _type;
    }

    void Argument::setVal(void *_val)
    {
        this->val = _val;}

} /* namespace DcmNs */
