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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nbellari                                                     *
 **************************************************************************/

#ifndef DSCPBITMAP_H
#define DSCPBITMAP_H

#include "Framework/Types/Types.h"

using namespace WaveNs;

namespace DcmNs {

class DscpBitMap
{
    private :
        void construct         (const UI32 &numberOfBits);
        void destroy           ();
        void copy              (const DscpBitMap &bitMap);

    protected :
    public :
		DscpBitMap      ();                                     // Empty Constructor.  Will initialize to 256 Bits by defualt.  All bits will be reset.
		DscpBitMap      (const UI32 &numberOfBits);             // Constructor to initialize with specified number of bits.  All bits will be reset.
		DscpBitMap      (const DscpBitMap &bitMap);                 // Copy Constructor
	   ~DscpBitMap      ();                                     // Destructor

        UI8     getValue    (const UI32 &index);                    // Get the value of the Bit at the specified index (0 .. numberOfBits - 1).
        void    setValue    (const UI32 &index, const UI8 &Value);  // Set the value of the Bit at the specified index (0 .. numberOfBits - 1) to the specified values (1/0).
        void    set         (const UI32 &index);                    // Set the value of the Bit at the specified index (0 .. numberOfBits - 1) to 1.
        void    set         ();                                     // Set all bits to 1.
        void    reset       (const UI32 &index);                    // Reset the value of the Bit at the specified index (0 .. numberOfBits - 1) to 0.
        void    reset       ();                                     // Reset all bits to 0.
		void	andWith(const DscpBitMap &bmap);				// Ands the contents of current bit map with the given one. a & b
		void	negateWith(const DscpBitMap &bmap);			// Negates the contents of current bitmap with the given one i.e a & ~b
		void	orWith(const DscpBitMap &bmap);				// Ors the contenshts of current bitmap with the given one i.e. a | b
		void	setValuesFromVector(vector <UI32> &valVector);
		void	getValuesInVector(vector <UI32> &outVector);
		bool	isBmapEmpty();
    
        UI32    getNumberOfBits () const;
        DscpBitMap &operator =  (const DscpBitMap &bitMap);                 // Assigns the given DscpBitMap to this DscpBitMap.  Automatically resizes this bit map based on the given bit map.
		bool operator == (const DscpBitMap &bitMap);
		bool operator != (const DscpBitMap &bitMap);

    // Now the data members

    private :
        UI64 *m_pBitBlocks;
        UI32  m_numberOfBitBlocks;
        UI32  m_numberOfBits;

    protected :
    public :
};

}
#endif // DSCPBITMAP_H
