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

#ifndef YANGLEAFLIST_H
#define YANGLEAFLIST_H

#include "Modeling/YANG/ObjectModel/YangDataElement.h"

namespace WaveNs
{

class YangLeafList : public YangDataElement
{
    private :
    protected :
    public :
                            YangLeafList    ();
        virtual            ~YangLeafList    ();

        static  string      getYangName     ();

        static YangElement *createInstance  ();

                void        displayConfigurtionForLeaf (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject);

        virtual void        applyFormattingToConfigString       (string &configString) const;

    // Now the data members
    private :
    protected :
    public :
};

}

#endif // YANGLEAFLIST_H
