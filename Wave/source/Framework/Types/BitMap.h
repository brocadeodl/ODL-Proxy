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

 /**************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef BITMAP_H
#define BITMAP_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class BitMap
{
    private :
        void           construct            (const UI32 &numberOfBits);
        void           destroy              ();
        void           resizewithoutcopy    (const UI32 &numberOfBits);
        void           copy                 (const BitMap &bitMap);
                                            
    protected :                             
    public :                                
                       BitMap               ();                                    // Empty Constructor.  Will initialize to 256 Bits by defualt.  All bits will be reset.
                       BitMap               (const UI32 &numberOfBits);            // Constructor to initialize with specified number of bits.  All bits will be reset.
                       BitMap               (const BitMap &bitMap);                // Copy Constructor
                      ~BitMap               ();                                    // Destructor
                                            
        void           setAllValues         (const UI64 &val);                     // set all the private variables based on the 64 bit value provided
        void           resize               (const UI32 &numberOfBits);            // Resize the BitMap to the desired size based on the specified number of bits.
                                                                                   // If the new number of bits is less than the old number of bits, the bits in
                                                                                   // most significant positions will be truncated. and previous values for
                                                                                   // the remaining bits will be retained.
                                                                                   // If the new number of bits is greater than the old number of bits, most significant
                                                                                   // bits will be added and the new bits will be set to 0.
                                            
        string         toString             () const;                              // Converts to string in Hexadecimal format.
        void           fromString           (const string &bitMapInStringFormat, const UI32 &noOfBits = 0);   // Loads bit map from a given Hexadecimal string.  NO 0x prefix must be specified.
                                                                                   // Will automatically resize to meet the new size requirement based on the string specified.
                                                                      
        void           getPlainString       (string &valueString) const;
        ResourceId     loadFromPlainString  (const string &pBoolUCInPlainStringFormat, const UI32 &noOfBits = 0);
                      
        UI8            getValue             (const UI32 &index);                    // Get the value of the Bit at the specified index (0 .. numberOfBits - 1).
        UI64           getElemValue         () const;                               // Get the element value if there is only 1 elem
        void           setValue             (const UI32 &index, const UI8 &Value);  // Set the value of the Bit at the specified index (0 .. numberOfBits - 1) to the specified values (1/0).
        void           set                  (const UI32 &index);                    // Set the value of the Bit at the specified index (0 .. numberOfBits - 1) to 1.
        void           set                  ();                                     // Set all bits to 1.
        void           reset                (const UI32 &index);                    // Reset the value of the Bit at the specified index (0 .. numberOfBits - 1) to 0.
        void           reset                ();                                     // Reset all bits to 0.
                      
        UI32           getNumberOfBits      () const;
                      
        UI8            operator [] (const UI32 &index);                    // Index operator, returns the bit value at the index specified.  Returned value is NOT an lvalue.
        bool           operator == (const BitMap &bitMap);                 // Returns true if the given BitMap is same as this BitMap, including size.
        bool           operator != (const BitMap &bitMap);                 // Returns true if the given BitMap is NOT same as this BitMap, including size.
        BitMap        &operator =  (const BitMap &bitMap);                 // Assigns the given BitMap to this BitMap.  Automatically resizes this bit map based on the given bit map.

    // Now the data members

    private :
        UI64 *m_pBitBlocks;
        UI32  m_numberOfBitBlocks;
        UI32  m_numberOfBits;

    protected :
    public :
};

}

#endif // BITMAP_H
