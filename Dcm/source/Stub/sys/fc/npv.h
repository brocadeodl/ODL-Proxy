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
 *
 *    Copyright (c) 2002-2005 Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Description:
 *      N_Port Virtualization data structures and definitions
 *      for user-space and kernel.
 *
 */

#ifndef __SYS_NPV_H__
#define	__SYS_NPV_H__



/*
 * Valid range of values for the least significant byte in a virtual N_Port
 */
#define NPV_MIN_IX	1
#define NPV_MAX_IX	255

#define	NPV_SHARED_PT_MAX_ALPA	127	/* max number of alpas for shared area ports */

/* max number of alpas for ports with 10 bit area */
#define	NPV_TEN_BIT_AREA_PT_MAX_ALPA	63

/*
 * Defect 54925:  The default index defined here limits the number
 * of devices per F-port to 127 to suppress a backward compatibility
 * issue in NS with pre-v5.0 releases.  Now, a NPIV port can be user
 * configured to accept up to a maximum of 256 devices per F-port.
 * For this all other switches in the fabric running 2.6, 3.2 and
 * 4.4 releases must be upgraded to NPIV compatible patch release
 * versions.
 *
 * NOS 2.0.0  doesn't care about backward compatibility so change to 255.
 */

#define NPV_DEF_IX	255


/* Default values for NPIV config parameters */
#define NPV_PER_PORT_DEFAULT	16


#define NPV_BITS_IN_ENTRY	32
#define NPV_ENTRY_MASK		31
#define NPV_ENTRY_SHIFT		5

#define NPV_WORD_INDEX(ix) ((ix) >> NPV_ENTRY_SHIFT)
#define NPV_BIT_INDEX(ix) ((ix) & NPV_ENTRY_MASK)

#define NPV_ENTRY(bm, ix) (((u_int *)(bm))[NPV_WORD_INDEX(ix)])
#define NPV_BIT(ix) (1 << NPV_BIT_INDEX(ix))

#define NPV_LEFT_BITS_MASK(ix) (~(NPV_BIT(ix)) + 1)

#define NPV_IS_INDEX_VALID(ix) (NPV_MIN_IX <= (ix) && (ix) <= NPV_MAX_IX)

/* Number of words in an NPV bitmap */
#define NPV_NWORDS	((NPV_MAX_IX / NPV_BITS_IN_ENTRY) + \
			 ((NPV_MAX_IX % NPV_BITS_IN_ENTRY) ? 1 : 0))

typedef u_int	npvmap_t[NPV_NWORDS];

/*
 * Return the index value for bit 'b' in word 'w' of NPV bitmap 'bm'
 */
#define fcNPVIndex(w, b)	((w) * NPV_BITS_IN_ENTRY + (b))

/*
 * Is the bit for index 'ix' in NPV bitmap 'bm' set?
 */
#define NPV_IS_BIT_SET(bm, ix) \
	(NPV_IS_INDEX_VALID(ix) && ((NPV_ENTRY(bm, ix) & NPV_BIT(ix)) != 0))

/*
 * Is the bit for index 'ix' in NPV bitmap 'bm' cleared?
 */
#define NPV_IS_BIT_CLEAR(bm, ix) \
	(NPV_IS_INDEX_VALID(ix) && ((NPV_ENTRY(bm, ix) & NPV_BIT(ix)) == 0))

/*
 * Set the bit for index 'ix' in NPV bitmap 'bm'
 */
#define NPV_SET_BIT(bm, ix) \
	(NPV_IS_INDEX_VALID(ix) ? (NPV_ENTRY(bm, ix) |= NPV_BIT(ix)) : 0)

/*
 * Clear the bit for index 'ix' in NPV bitmap 'bm'
 */
#define NPV_CLEAR_BIT(bm, ix) \
	(NPV_IS_INDEX_VALID(ix) ? (NPV_ENTRY(bm, ix) &= ~NPV_BIT(ix)) : 0)

/*
 * Clear the whole NPV bitmap 'bm'
 */
#define NPV_CLEAR_BITMAP(bm)	memset((bm), 0, sizeof(npvmap_t))


#if defined(__KERNEL__)

/*
 * Return the next available index in NPV bitmap 'bm'
 *
 * This function scans for the next available bit position
 * by searching for a cleared bit in the bitmap.  The tricky
 * part in this function is code that skips a word in the
 * bitmap when the rest of the word has all bits set.  It
 * computes the bit mask of all the bits to the left of the
 * bit being tested including that bit.  The mask is computed
 * by the macro NPV_LEFT_BITS_MASK using two's compliment of
 * the bit mask of the indexed bit.
 *
 * The second parameter is the base index which is the
 * F-port's ALPA.  All index after that should be assigned
 * to the virtual devices.
 *
 * The third parameter to this function determines the
 * max count of device index starting from the base index
 */
static inline int
npiv_get_next_index(u_int *bm, int base_ix, int max_ix)
{
	int		ix, start_ix;
	int		ret_ix = -1; /* return -1 if no useable index found */
	u_int	left_bits_mask;

	if (!(NPV_IS_INDEX_VALID(max_ix))) {
		max_ix = NPV_DEF_IX;
	}

    start_ix = base_ix + 1;

	/*
	 * The following adjustment often happens to shared ports
	 * in positron whose alpa starts at 0x81.
	 * Since the base_ix is 0x80 and the max number of device is 127,
	 * the max_ix needs to be adjusted internally in this function
	 */
    if ((base_ix + max_ix) > NPV_MAX_IX) {
		max_ix = NPV_MAX_IX;
	} else {
		max_ix = base_ix + max_ix;
	}

	if (!(NPV_IS_INDEX_VALID(base_ix + 1))) {
		return (-1);
	}

	for (ix = start_ix; ix <= max_ix; ix++) {
		left_bits_mask = NPV_LEFT_BITS_MASK(ix);

		if ((NPV_ENTRY((bm), ix) & left_bits_mask) ==
			left_bits_mask) {

			(ix) += (NPV_BITS_IN_ENTRY - NPV_BIT_INDEX(ix) - 1);
			continue;
		}

		if (NPV_IS_BIT_CLEAR(bm, ix)) {
			ret_ix = ix;
			break;
		}
	}

	if ( (ret_ix == -1) && (base_ix == 0x13) ) {
		/* For AL_PA 0x13 base, we have to wrap back to index 0
		   if we haven't found a useable index yet */
		for (ix = 0; ix < base_ix; ix++) {
			left_bits_mask = NPV_LEFT_BITS_MASK(ix);

			if ((NPV_ENTRY((bm), ix) & left_bits_mask) ==
				left_bits_mask) {

				(ix) += (NPV_BITS_IN_ENTRY - NPV_BIT_INDEX(ix) - 1);
				continue;
			}

			if (NPV_IS_BIT_CLEAR(bm, ix)) {
				ret_ix = ix;
				break;
			}
		}
	}

	return (ret_ix);
}


#endif	/* (__KERNEL__) */


#endif /* __SYS_NPV_H__ */
