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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#ifndef WAVEMANAGEDVIEW_H
#define WAVEMANAGEDVIEW_H

#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

class WaveManagedView : virtual public WaveManagedObject
{
    private :
    protected :
        void setupAttributesForPersistence  ();
        void setupAttributesForCreate       ();

    public :
                           WaveManagedView            (WaveObjectManager *pWaveObjectManager);
        virtual           ~WaveManagedView            ();

        static  string      getClassName                ();
        virtual string      getSqlForCreateView         ();
        //virtual void        getSetContextForCreateView  (WaveManagedObjectSynchronousQueryContextForSetOperation* &pViewContext);
        //virtual void        getSetContextForCreateView  (WaveManagedObjectSynchronousQueryContextForSetOperation *pViewContext);
        virtual void        getSetContextForCreateView  (WaveManagedObjectSynchronousQueryContextForSetOperation &viewContext);


    // Now the data members
    private :

    protected:
    public :
};

}

#endif // WAVEMANAGEDVIEW_H
