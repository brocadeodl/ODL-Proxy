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
 *   Author : nbellari													   *
 **************************************************************************/

#include "Qos/Global/DscpBitMap.h"
#include "Framework/Types/UI32Range.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/TraceUtils.h"

using namespace WaveNs;

namespace DcmNs {

	void DscpBitMap::andWith(const DscpBitMap &bitMap)
	{
		UI8 i;

		// Do not attempt to "and" if the number of bits mismatch.
		prismAssert (m_numberOfBits == bitMap.m_numberOfBits, __FILE__, __LINE__);
		
		for (i=0; i<m_numberOfBitBlocks; i++) {
			m_pBitBlocks[i] &= bitMap.m_pBitBlocks[i];
		}

	}

	void DscpBitMap::negateWith(const DscpBitMap &bitMap)
	{
		UI8 i;

		// Do not attempt to "and" if the number of bits mismatch.
		prismAssert (m_numberOfBits == bitMap.m_numberOfBits, __FILE__, __LINE__);
		
		for (i=0; i<m_numberOfBitBlocks; i++) {
			m_pBitBlocks[i] &= ~(bitMap.m_pBitBlocks[i]);
		}
	}

	void DscpBitMap::orWith(const DscpBitMap &bitMap)
	{
		UI8 i;

		// Do not attempt to "and" if the number of bits mismatch.
		prismAssert (m_numberOfBits == bitMap.m_numberOfBits, __FILE__, __LINE__);
		
		for (i=0; i<m_numberOfBitBlocks; i++) {
			m_pBitBlocks[i] |= bitMap.m_pBitBlocks[i];
		}
	}

	void DscpBitMap::construct (const UI32 &numberOfBits)
	{
		m_numberOfBits      = numberOfBits;    
		m_numberOfBitBlocks = ((m_numberOfBits + 63)/ 64) ;
		m_pBitBlocks        = new UI64[m_numberOfBitBlocks];
	}

	void DscpBitMap::destroy ()
	{
		if (NULL != m_pBitBlocks)
		{
			delete[] m_pBitBlocks;
		}

		m_numberOfBits      = 0;
		m_numberOfBitBlocks = 0;
	}

	void DscpBitMap::copy (const DscpBitMap &bitMap)
	{
		prismAssert (m_numberOfBits == bitMap.m_numberOfBits, __FILE__, __LINE__);

		memcpy(m_pBitBlocks, bitMap.m_pBitBlocks, (m_numberOfBitBlocks * 8)); 
	}

	DscpBitMap::DscpBitMap ()
	{
		construct (256);
	}

	DscpBitMap::DscpBitMap (const UI32 &numberOfBits)
	{
		construct (numberOfBits);
	}

	DscpBitMap::DscpBitMap (const DscpBitMap &bitMap)
	{
		construct (bitMap.m_numberOfBits);
		copy (bitMap);    
	}

	DscpBitMap::~DscpBitMap ()
	{
		destroy ();
	}

	UI8 DscpBitMap::getValue (const UI32 &index)
	{
		prismAssert (index < m_numberOfBits, __FILE__, __LINE__);
		
		UI8  userBitInBlock = 0;
		UI32 bitBlockNumber = index/64;         /* Block Number Starts From 0 */
		UI64 bitMask = 0;

		userBitInBlock = index % 64;
		bitMask = (UI64) 1 << userBitInBlock;
	 
		if (!(bitMask & m_pBitBlocks[bitBlockNumber]))
		{
			return ((UI8) 0);
		}
		else
		{
			return ((UI8) 1);
		}
	}

	void DscpBitMap::setValue (const UI32 &index, const UI8 &Value)
	{
		prismAssert (index < m_numberOfBits, __FILE__, __LINE__);

		UI8  userBitInBlock = 0;
		UI32 bitBlockNumber = index/64;
		UI64 bitMask = 0, bitMaskCompliment = 0;

		userBitInBlock = index % 64;
		if (1 == Value)
		{
			bitMask = (UI64) Value << userBitInBlock;
			m_pBitBlocks[bitBlockNumber] = m_pBitBlocks[bitBlockNumber] | bitMask;
		}
		else if (0 == Value)
		{
			bitMaskCompliment = (UI64) 1 << userBitInBlock;
			bitMask = ~bitMaskCompliment;
			m_pBitBlocks[bitBlockNumber] = m_pBitBlocks[bitBlockNumber] & bitMask;
		}
	}

	void DscpBitMap::setValuesFromVector(vector <UI32> &valVector)
	{
		vector<UI32>::iterator vIter;

		for (vIter = valVector.begin(); vIter != valVector.end(); vIter++)
		{
			/* set all the values in the bitmap */
			set(*vIter);
			//trace(TRACE_LEVEL_DEBUG, string("setting bit ") + *vIter + string(" in bitmap"));
		}

		//trace(TRACE_LEVEL_DEBUG, string("bitBlock, after setting bits in is ") + m_pBitBlocks[0]);
	}

	void DscpBitMap::getValuesInVector(vector <UI32> &outVector)
	{
		UI32 i,j;

		outVector.clear();

		//trace(TRACE_LEVEL_DEBUG, string("bitBlock, before pushing in is ") + m_pBitBlocks[0]);

		for (i=0, j=0; j<m_numberOfBitBlocks && i<m_numberOfBits; i++) {
			if (i && (i % (sizeof(m_pBitBlocks[j])*8) == 0)) {
				i = 0;
				j++;
			}

			if (m_pBitBlocks[j] & ((UI64)1 << i)) {
				//trace(TRACE_LEVEL_DEBUG, string("pushing ") + (i + 64*j) + string("into vector"));
				outVector.push_back((i + 64*j));
			}
		}
	}

	bool DscpBitMap::isBmapEmpty()
	{
		UI8 i;

		for (i=0; i<m_numberOfBitBlocks; i++) {
			if (m_pBitBlocks[i])
				return (false);
		}

		return (true);
	}

	void DscpBitMap::set (const UI32 &index)
	{
		prismAssert (index < m_numberOfBits, __FILE__, __LINE__);

		setValue (index, 1);
	}

	void DscpBitMap::set ()
	{
		UI64 extraBit = 0;

		memset(m_pBitBlocks, 0xff, (m_numberOfBitBlocks * 8));

		extraBit = (m_numberOfBitBlocks * 64) - m_numberOfBits;

		m_pBitBlocks[m_numberOfBitBlocks - 1] = m_pBitBlocks[m_numberOfBitBlocks - 1] >> extraBit;
	}

	void DscpBitMap::reset (const UI32 &index)
	{
		prismAssert (index < m_numberOfBits, __FILE__, __LINE__);

		setValue (index, 0);
	}

	void DscpBitMap::reset ()
	{
		memset(m_pBitBlocks, 0x0, (m_numberOfBitBlocks * 8));
	}

	UI32 DscpBitMap::getNumberOfBits () const
	{
		return (m_numberOfBits);
	}

	bool DscpBitMap::operator == (const DscpBitMap &bitMap)
	{
		UI32 i = 0;

		if (m_numberOfBits != bitMap.m_numberOfBits)
		{
			return (false);
		}
		else if (m_numberOfBitBlocks != bitMap.m_numberOfBitBlocks)
		{
			return (false);
		}
		else
		{
			for (i = 0; i < m_numberOfBitBlocks; i++)
			{
				if (m_pBitBlocks[i] != bitMap.m_pBitBlocks[i])
				{
					return (false);
				}
			}
		}

		return (true);
	}

	bool DscpBitMap::operator != (const DscpBitMap &bitMap)
	{
		return (! (operator == (bitMap)));
	}

	DscpBitMap &DscpBitMap::operator = (const DscpBitMap &bitMap)
	{
		destroy();
		construct(bitMap.m_numberOfBits);
		copy (bitMap);

		return (*this);
	}

}
