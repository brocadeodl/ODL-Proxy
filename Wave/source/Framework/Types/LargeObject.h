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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Raymond Lai		                                           *
 ***************************************************************************/

#ifndef LARGEOBJECT_H
#define LARGEOBJECT_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class LargeObject
{
    private:
              void          construct			(const UI32 &numberOfBytes);
              void          destroy				();
			  void			resizewithoutcopy	(const UI32 &numberOfBytes);
              void          copy				(const LargeObject &largeObject);

    protected:
    public:
                            LargeObject			();
                            LargeObject			(const UI32 &numberOfBytes);
                            LargeObject			(const LargeObject &largeObject);
							~LargeObject		();

              void          resize				(const UI32 &numberOfBytes);
              string        toString			() const;
              void          fromString			(const string &largeObjectInStringFormat, const UI32 &noOfBytes = 0);
			  UI32			getNumberOfBytes	() const;
			  UI8			operator []			(const UI32 &index);
              bool          operator ==			(const LargeObject &largeObject) const;
              bool          operator !=			(const LargeObject &largeObject) const;
              LargeObject  	&operator =			(const LargeObject &largeObject);


            // now the data members
    private:
              UI8			*m_pByteBlocks;
              UI32        	m_numberOfBytes;

    protected:
    public:
};

}

#endif    // LARGEOBJECT_H
