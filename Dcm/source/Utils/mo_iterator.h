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

/* **
 * mo_iterator.h
 *
 * May 22, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef MO_ITERATOR_H_
#define MO_ITERATOR_H_

#include <vector>
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Attributes/Attributes.h"
#include "DceContext.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs {

class mo_criteria
{
public:
    mo_criteria(const string &moClassName)
            : _qc(moClassName) { }

    ~mo_criteria() { }

public:

    inline mo_criteria& schema(const string& schemaName) {
        _qc.setSchemaToQuery(schemaName);
        return *this;
    }

    inline mo_criteria& select(const string &attr) {
        _qc.addSelectField(attr);
        return *this;
    }

    inline mo_criteria& select(vector<string> &attrs) {
        _qc.addSelectFields(attrs);
        return *this;
    }

    inline mo_criteria& orderBy(const string &attr, bool ascending = true) {
        _qc.addOrderField(attr, ascending);
        return *this;
    }


    inline mo_criteria& And(const string &attr, const string &value,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        return And(new AttributeString(value, attr), condOper);
    }

    inline mo_criteria& And(const string &attr, UI32 value,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        return And(new AttributeUI32(value, attr), condOper);
    }

    inline mo_criteria& And(const string &attr, const ObjectId &oid,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        return And(new AttributeObjectId(oid, attr), condOper);
    }

    inline mo_criteria& And(const string &attr, bool b,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        return And(new AttributeBool(b, attr), condOper);
    }

    inline mo_criteria& And(Attribute *attr,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        _qc.addAndAttribute(attr, condOper);
        return *this;
    }

    inline mo_criteria& Or(const string &attr, const string &value,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        return Or(new AttributeString(value, attr), condOper);
    }

    inline mo_criteria& Or(const string &attr, UI32 value,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        return Or(new AttributeUI32(value, attr), condOper);
    }

    inline mo_criteria& Or(const string &attr, const ObjectId &oid,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        return Or(new AttributeObjectId(oid, attr), condOper);
    }

    inline mo_criteria& Or(const string &attr, bool b,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        return Or(new AttributeBool(b, attr), condOper);
    }

    inline mo_criteria& Or(Attribute *attr,
            AttributeConditionOperator condOper = WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL) {
        _qc.addOrAttribute(attr, condOper);
        return *this;
    }

    inline mo_criteria& location(LocationId loc) {
        _qc.setLocationIdFilter(loc);
        return *this;
    }

    /* **
     * Sets the slot filter for query context. Not all MOs have slots.
     */
    inline mo_criteria& slot(const UI32 &slotNumber) {
        _qc.setSlotFilter(slotNumber);
        return *this;
    }

    inline mo_criteria& limit(UI32 n) {
        _qc.addLimitForQueryResults(n);
        return *this;
    }


    inline WaveManagedObjectSynchronousQueryContext* getQueryContext() {
        return &_qc;
    }

    inline vector<WaveManagedObject*> *query() {
        return WaveObjectManagerToolKit::querySynchronously(&_qc);
    }

    inline UI32 queryCount() {
        UI32 n = 0;
        WaveObjectManagerToolKit::querySynchronouslyForCount(&_qc, n);
        return n;
    }

    inline bool queryExists() {
        return queryCount() != 0;
    }

    inline ResourceId queryExists (bool &o_exists) {
        UI32 n = 0;
        ResourceId status = WaveObjectManagerToolKit::querySynchronouslyForCount(&_qc, n);
        if (status == WAVE_MESSAGE_SUCCESS) o_exists = (n != 0);
        return status;
    }

private:
    // hide assignment...
    mo_criteria& operator = (const mo_criteria& other);
    mo_criteria(const mo_criteria& other);

private:
    WaveManagedObjectSynchronousQueryContext _qc;

};


template <typename T>
class mo_iterator
{
public:

    mo_iterator (DceContext *ctx = NULL)
        : _criteria ( T::getClassName() )
        , _mo_list ( NULL ), _iter ( 0 )
        , _dce_context ( ctx )
    {
    }

    /* **
     * Make sure the MO is subclass of T.
     */
    mo_iterator (const string& moClassName, DceContext *ctx = NULL)
        : _criteria ( moClassName )
        , _mo_list ( NULL ), _iter ( 0 )
        , _dce_context ( ctx )
    {
    }

    ~mo_iterator() {
        if (_mo_list) {
            if (_dce_context) {
                _dce_context->addManagedObjectsForGarbageCollection(*_mo_list);
                delete _mo_list;
            } else {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(_mo_list);
            }
        }
    }

    bool hasNext() {
        if (!_mo_list) {
            _mo_list = _criteria.query();
            if (!_mo_list) _mo_list = new vector<WaveManagedObject*>();
            _iter = 0;
        }
        return _iter < _mo_list->size();
    }

    inline T* next() {
        return hasNext() ? dynamic_cast<T*>(_mo_list->at(_iter++)) : NULL;
    }

    inline mo_criteria& criteria() {
        return _criteria;
    }

private:
    mo_criteria _criteria;
    vector<WaveManagedObject*> *_mo_list;
    unsigned int _iter;
    unsigned int _size;

    DceContext *_dce_context;

};

}

#endif /* MO_ITERATOR_H_ */
