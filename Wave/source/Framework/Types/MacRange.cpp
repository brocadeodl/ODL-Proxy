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
 *   Author : Anil Channaveerasetty                                        *
 ***************************************************************************/

#include "Framework/Types/SnmpObjectId.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Types/MacRange.h"
#include <cerrno>
#include <cctype>
#include <sstream>

namespace WaveNs
{

vector<string> MACRange::parsingMacRange (const string &rangeMacString) const
{
    UI32 i,j;
    string tempStringRange = rangeMacString;
    UI32 countFirstLevel = 0;
    UI32 countSecondLevel = 0;
    UI32 totalMacs = 0;
    vector<string> rangeOfMacs;
    vector<string> firstLevelToken;
    vector<string> secondLevelToken;
    UI32 startValue = 0;
    UI32 endValue   = 0;
    bool upperCase = false;
    string patternMac, nextMac; 
    char val[3];	

    tokenize (tempStringRange, firstLevelToken, ',');
    
    countFirstLevel = firstLevelToken.size ();

    for (i = 0; i < countFirstLevel; i++)
    {
        tokenize (firstLevelToken[i], secondLevelToken, '-');

        countSecondLevel = secondLevelToken.size ();

        prismAssert (2 >= countSecondLevel, __FILE__, __LINE__);

       
	    if ( 1 == countSecondLevel )
	    {
	         rangeOfMacs.push_back (secondLevelToken[0]);
	         totalMacs++;	
	    }
	    else
	    {
            // countSecondLevel is 2
            // Pattern is the first 12 char from token[0]

	        patternMac = (secondLevelToken[0].substr(0,12));
            startValue = strtoul((secondLevelToken[0].substr(12,2)).c_str(),NULL,16);
            endValue = strtoul(secondLevelToken[1].c_str(),NULL,16); 	 
            
            // Check for uppercase in 2 chars of token[1]

	        upperCase = isUpperCase(secondLevelToken[1].c_str());
	
	        rangeOfMacs.push_back (secondLevelToken[0]);
	        totalMacs++;

            for (j = startValue+1; j <= endValue; j++)
            {
	            nextMac = patternMac;	

                // If range is less than 15 then we need to append '0' as prefix

	            if( 15 >= j)
	            {
	                if(!upperCase)
                    {
	                sprintf( val,"%x",0);	   
                    }
	                else
                    {    
	                sprintf( val,"%X",0);
                    }
                  nextMac += val;
	            }

	            if(!upperCase)
                {
	                sprintf( val,"%x",j);	   
                }
	            else
                {
	                sprintf( val,"%X",j);
                }
                nextMac += val;
                rangeOfMacs.push_back (nextMac);
	        totalMacs++;
	        }
	     }
     }
    return (rangeOfMacs);
}


// Check fro uppercase in 2 chars of lastvalue

bool MACRange::isUpperCase (const char * lastValue) const
{
	UI8 i = 0;
	
	while ( lastValue[i] ) 
	{
        if ( isupper(lastValue[i]) )
        {            
    		return (true);	   
        }
    	    i++;
    }

	return (false);
}

void MACRange::copy (const MACRange &rangeMac)
{
    m_MacRange = rangeMac.m_MacRange;
}

void MACRange::copy (const string &rangeMacString)
{
    m_MacRange = rangeMacString;
}

bool MACRange::isAValidString (const string &rangeMacString)
{
    UI32 i;
    UI32 countFirstLevel = 0;
    UI32 countSecondLevel = 0;
    vector<string>  firstLevelToken;
    vector<string>  secondLevelToken;
    UI32 startValue = 0;
    UI32 endValue   = 0;
    SI32 valid = 0;
    string startValueString = "";
    string previousValueString = "";
    string endValueString = "";
    string maxValueString;

    tokenize (rangeMacString, firstLevelToken, ',');

    countFirstLevel = firstLevelToken.size ();

    for (i = 0; i < countFirstLevel; i++)
    {
        if (i > 0)
        {
	    previousValueString = endValueString;
        }

        tokenize (firstLevelToken[i], secondLevelToken, '-');

        countSecondLevel = secondLevelToken.size ();

        startValueString = secondLevelToken[0];

        // Get strtoul of last 2 token[0] chars which specify the start value of the range

	    startValue = getRangeValueFromString(secondLevelToken[0]);
	
	    if (errno == ERANGE)
        {
            trace (TRACE_LEVEL_DEBUG, "MACRange::isAValidString : Invalid Range String : " + rangeMacString);
            return (false);
        }


	    if (i > 0)
        {
            // check for startvalue is > previousvalue
            if (0 >= ( valid = startValueString.compare(previousValueString) ) )
            {
 	            trace (TRACE_LEVEL_DEBUG, "MACRange::isAValidString : Invalid Range String : " + rangeMacString);
                return (false);
            }
        }

        if ( 1 == countSecondLevel )
    	{
	        endValueString = startValueString;	
            continue ;	
	    }
	    else if ( 2 == countSecondLevel) 
	    {
       
             endValue = getRangeValueFromString(secondLevelToken[1]);
	         endValueString = 	(secondLevelToken[0].substr(0,12)) + secondLevelToken[1];

              if (errno == ERANGE)
              {
                trace (TRACE_LEVEL_DEBUG, "MACRange::isAValidString : Invalid Range String : " + rangeMacString);
                return (false);
              }
	          else if (endValue <= startValue)  // check for endvalue > startvalue in the range
              {
                 trace (TRACE_LEVEL_DEBUG, "MACRange isAValidString: Invalid Range String : " + rangeMacString);
                 return (false);
              }
        }
	    else         // if (2 < countSecondLevel)
        {
            trace (TRACE_LEVEL_DEBUG, "MACRange::isAValidString : Invalid Range String : " + rangeMacString);            
            return (false);
        }
    }

    return (true);
}


MACRange::MACRange ()
{
    string tempString = "";

    copy (tempString);
}

MACRange::MACRange (const string &rangeMacString)
{
    prismAssert (true == isAValidString (rangeMacString), __FILE__, __LINE__);

    copy (rangeMacString);
}

MACRange::MACRange (const MACRange &rangeMac)
{
    copy (rangeMac);
}

MACRange::MACRange (const string &startValue, const string &endValue)
{
    string tmpStr = startValue + "-" + endValue;

    copy (tmpStr);
}

string MACRange::toString () const
{
    return (m_MacRange);
}

void MACRange::fromString (const string &rangeMacString)
{
    prismAssert (true == isAValidString (rangeMacString), __FILE__, __LINE__);
    
    copy (rangeMacString);
}

ResourceId MACRange::loadFromPlainString (const string &rangeMacString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    if (true == isAValidString (rangeMacString))
    {
        copy (rangeMacString);
        status = WAVE_MESSAGE_SUCCESS;
    }    

    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("MACRange::loadFromPlainString : Improper format of MACRange in string"));
    }

    return status;
}

void MACRange::getMacRangeVector (vector <string> &rangeMacVector) const
{

    rangeMacVector.clear ();

    rangeMacVector = parsingMacRange (m_MacRange);
}

string MACRange::addOneElementToString(bool &insertComma, string elem)
{
    string          tmpStr;
    
    if (insertComma == 0)
    {
        tmpStr = elem;
        insertComma = 1;
    }
    else
    {
        tmpStr = "," + elem;
    }

    return (tmpStr);
}

string MACRange::addRangeToString(bool &insertComma, string firstElem, string lastElem)
{

    string          tmpStr;

    if (insertComma == 0)
    {
        tmpStr = firstElem + "-";
	    tmpStr += lastElem.substr(12,2);
        insertComma = 1;
    }
    else
    {
        tmpStr = ",";
        tmpStr += firstElem + "-";
	    tmpStr += lastElem.substr(12,2);
    }

    return (tmpStr);
}

UI32 MACRange::getRangeValueFromString (string rangeMacString)
{
      UI32 rangeVal = 0;
        
      errno = 0;
      if (14 == rangeMacString.size())
      {   
          rangeVal = strtoul((rangeMacString.substr(12,2)).c_str(),NULL,16);
      }
      else if (2 == rangeMacString.size())
      {
          rangeVal = strtoul((rangeMacString.substr(0,2)).c_str(),NULL,16);
      }   
      else
      { 
          trace(TRACE_LEVEL_ERROR, string("MACRange::getRangeValueFromString : Invalid Range detected" + rangeMacString) );
          errno = ERANGE;
      }
      return rangeVal;
}


string MACRange::getMacRangeStringFromVector (vector <string> &rangeMacVector)
{
    string          rangeString;
    string 	    firstElemString;	
    string 	    currElemString;	
    UI32            index = 0;
    UI32            firstElem;
    UI32            currElem;
    bool            insertComma = 0;

    if ( rangeMacVector.size() == 0 )
    {
        rangeString = "";
    }
    else if ( rangeMacVector.size() == 1 )
    {
		rangeString = rangeMacVector[0];
    }
    else
    {
	firstElemString = rangeMacVector[index];
    currElemString = rangeMacVector[index];

	
	firstElem = getRangeValueFromString(firstElemString);
	currElem = firstElem ;

	index ++;
        while ( index < rangeMacVector.size() )
        {
	    	
            if ( getRangeValueFromString(rangeMacVector[index]) == (currElem + 1))
            {
                // consecutive. proceed to next elem
                currElem = getRangeValueFromString(rangeMacVector[index]);
                currElemString = rangeMacVector[index];
                index ++;
                continue;
            }
            else
            {
                if (firstElem == currElem)
                {
                    // add a single number to string
                    rangeString += addOneElementToString(insertComma, firstElemString);
                }
                else
                {
                    // add the range to string
                    rangeString += addRangeToString(insertComma, firstElemString, currElemString);
                }

                firstElem = getRangeValueFromString(rangeMacVector[index]);
                firstElemString = rangeMacVector[index];
                currElem = firstElem;
                currElemString = rangeMacVector[index];
                index ++;
            }
        } // while loop
        if (firstElem == currElem)
        {
            // add a single number to string
            rangeString += addOneElementToString(insertComma, firstElemString);
        }
        else
        {
            // add the range to string
            rangeString += addRangeToString(insertComma, firstElemString, currElemString);
        }
    }

    return (rangeString);
}

bool MACRange::operator == (const MACRange &rangeMac) const
{
    if (m_MacRange != rangeMac.m_MacRange)
    {
        return (false);
    }

    return (true);
}

bool MACRange::operator != (const MACRange &rangeMac) const
{
    return (! (operator == (rangeMac)));
}

MACRange &MACRange::operator = (const MACRange &rangeMac)
{
    copy (rangeMac);

    return (*this);
}

}

