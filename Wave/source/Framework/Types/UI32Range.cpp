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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Types/SnmpObjectId.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Types/UI32Range.h"
#include <cerrno>
#include <sstream>
#include <algorithm>

using namespace std;

namespace WaveNs
{

vector<UI32> UI32Range::parsingRangeIntegers (const string &rangeIntegerString) const
{
    UI32            i = 0, j = 0;
    string          tempStringRange = rangeIntegerString;
    UI32            countFirstLevel = 0;
    UI32            countSecondLevel = 0;
    UI32            totalIntegers = 0;
    vector<UI32>    rangeOfIntegers;
    vector<string>  firstLevelToken;
    vector<string>  secondLevelToken;
    UI32            startValue = 0;
    UI32            endValue   = 0;

    tokenize (tempStringRange, firstLevelToken, ',');
    
    countFirstLevel = firstLevelToken.size ();

    for (i = 0; i < countFirstLevel; i++)
    {
        tokenize (firstLevelToken[i], secondLevelToken, '-');
        countSecondLevel = secondLevelToken.size ();
        prismAssert (2 >= countSecondLevel, __FILE__, __LINE__);
        startValue = strtoul (secondLevelToken[0].c_str (), NULL, 10);
        endValue   = strtoul (secondLevelToken[countSecondLevel - 1].c_str (), NULL, 10);

        for (j = startValue; j <= endValue; j++)
        {
            rangeOfIntegers.push_back (j);
            totalIntegers++;
        }
    
    }

    return (rangeOfIntegers);
}

map<UI32, bool> UI32Range::parsingRangeIntegersToMap (const string &rangeIntegerString) const 
{
    UI32            i = 0;
    string          tempStringRange = rangeIntegerString;
    UI32            countFirstLevel = 0;
    UI32            countSecondLevel = 0;
    map<UI32, bool> rangeMap;
    map<UI32, bool>::iterator end;
    map<UI32, bool>::iterator pos;
    map<UI32, bool>::iterator itr;
    vector<string>  firstLevelToken;
    vector<string>  secondLevelToken;
    UI64            startValue = 0;
    UI64            endValue   = 0;

    tokenize (tempStringRange, firstLevelToken, ',');

    countFirstLevel = firstLevelToken.size ();
    rangeMap.clear ();
    
    for (i = 0; i < countFirstLevel; i++)
    {
        tokenize (firstLevelToken[i], secondLevelToken, '-');
        countSecondLevel = secondLevelToken.size ();
        prismAssert (2 >= countSecondLevel, __FILE__, __LINE__);
    
        if (countSecondLevel == 2 || countSecondLevel == 1)
        {
            startValue = strtoul (secondLevelToken[0].c_str (), NULL, 10);
            endValue   = strtoul (secondLevelToken[countSecondLevel - 1].c_str (), NULL, 10);
        }
        
        /* swapping limits for reverse ranges */        
        if (startValue > endValue)
        {
            UI32 temp = startValue;
            startValue = endValue;
            endValue = temp;
        }
        
        /*Populating map with lower limit set as FALSE and upper limit set as TRUE */
            rangeMap[startValue] = false;
            rangeMap[endValue + 1] = true;
        

            if (startValue!= endValue)
            {
                rangeMap.erase (rangeMap.upper_bound (startValue), rangeMap.upper_bound (endValue));
            }

            mergeToPreviousMap (rangeMap, startValue);                                                                                                                                                          mergeToPreviousMap (rangeMap, endValue+1);  

            itr = rangeMap.upper_bound (endValue + 1);
            end = rangeMap.end ();

            if (end != itr)
            {
                mergeToPreviousMap (rangeMap, itr->first);   
            }
    }
    return (rangeMap);
}

void UI32Range::mergeToPreviousMap (map<UI32, bool> &rangeMap, const UI32 &limit)
{
    map<UI32, bool>::iterator begin   = rangeMap.begin ();
    map<UI32, bool>::iterator itr     = rangeMap.find (limit);
     
    if (begin == itr)   
    {
        return;
    }
        
    bool currentFlag = itr->second;

    itr--;
    
    /* overlapping ranges */
    if ((false == itr->second) && (false == currentFlag))
    {
        rangeMap.erase(limit);
    }
    else if ((true == itr->second) && (true == currentFlag))
    {
        rangeMap.erase (itr->first);
    }
    
}

void UI32Range::copy (const UI32Range &rangeIntegers)
{
    m_UI32RangeMap = rangeIntegers.m_UI32RangeMap;
}

void UI32Range::copy (const string &rangeIntegerString)
{
    m_UI32RangeMap = parsingRangeIntegersToMap(rangeIntegerString);
    
}

bool UI32Range::isAValidString (const string &rangeIntegerString)
{
    UI32            i = 0;
    string          tempStringRange = rangeIntegerString;
    UI32            countFirstLevel = 0;
    UI32            countSecondLevel = 0;
    vector<UI32>    rangeOfIntegers;
    vector<string>  firstLevelToken;
    vector<string>  secondLevelToken;
    UI64            startValue = 0;
    UI64            endValue   = 0;

    tokenize (rangeIntegerString, firstLevelToken, ',');

    countFirstLevel = firstLevelToken.size ();

    for (i = 0; i < countFirstLevel; i++)
    {
        tokenize (firstLevelToken[i], secondLevelToken, '-');

        countSecondLevel = secondLevelToken.size ();

        if (2 < countSecondLevel)
        {
            trace (TRACE_LEVEL_FATAL, "UI32Range isAValidString: Invalid Range String : " + rangeIntegerString);            
            return (false);
        }
        /* To check if the range lies within 0-(UINT_MAX-1) */
        char *firstInvalidCharacterPosition = NULL;
        char *secondInvalidCharacterPosition = NULL;
        startValue = strtoul (secondLevelToken[0].c_str (), &(firstInvalidCharacterPosition), 10);
        endValue   = strtoul (secondLevelToken[countSecondLevel - 1].c_str (), &(secondInvalidCharacterPosition), 10);
        
        // additional check to determine if any invalid characters where passed as part of the string 
        if( (endValue >= UINT_MAX || startValue >= UINT_MAX) || ( *firstInvalidCharacterPosition != '\0' || *secondInvalidCharacterPosition != '\0' ))
        {
            trace (TRACE_LEVEL_FATAL, "UI32Range Invalid Range String : " + rangeIntegerString);
            return (false);
        }

        errno = 0;

        if (errno == ERANGE)
        {
            trace (TRACE_LEVEL_FATAL, "UI32Range Invalid Range String : " + rangeIntegerString);
            return (false);
        }
    }

    return (true);
}

UI32Range::UI32Range ()
{
    string tempString = "";

    copy (tempString);
}

UI32Range::UI32Range (const string &rangeIntegerString)
{
    prismAssert (true == isAValidString (rangeIntegerString), __FILE__, __LINE__);
        
    copy (rangeIntegerString);
}

UI32Range::UI32Range (const UI32Range &rangeIntegers)
{
    copy (rangeIntegers);
}

UI32Range::UI32Range (const UI32 &startValue, const UI32 &endValue)
{
    char    tempBuffer[32] = {0};
    string  tempString;

    snprintf(tempBuffer, 32, "%u-%u", startValue, endValue);
    tempString = tempBuffer;

    copy (tempString);
}

UI32Range::UI32Range (const vector<UI32> &rangeIntegers)
{
    string tempString;

    tempString = getUI32RangeStringFromVector (rangeIntegers);

    prismAssert (true == isAValidString (tempString), __FILE__, __LINE__);

    copy (tempString);
}

string UI32Range::toString () const
{
   return ( getUI32RangeStringFromMap (m_UI32RangeMap));
}

void UI32Range::fromString (const string &rangeIntegerString)
{
    prismAssert (true == isAValidString (rangeIntegerString), __FILE__, __LINE__);
    
    copy (rangeIntegerString);
}

ResourceId UI32Range::loadFromPlainString (const string &rangeIntegerString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    if (true == isAValidString (rangeIntegerString))
    {
        copy (rangeIntegerString);
        status = WAVE_MESSAGE_SUCCESS;
    }

    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("UI32Range::loadFromPlainString : Improper format of UI32Range in string"));
    }

    return status;
}

void UI32Range::getUI32RangeVector (vector <UI32> &rangeIntegers) const
{

    rangeIntegers.clear ();

    rangeIntegers = parsingRangeIntegers (getUI32RangeStringFromMap (m_UI32RangeMap));
}

void UI32Range::getUI32RangeMap (map <UI32, bool> &rangeIntegers) const 
{
   rangeIntegers.clear ();
    rangeIntegers = parsingRangeIntegersToMap (getUI32RangeStringFromMap (m_UI32RangeMap));
}

string UI32Range::addOneElementToString (bool &insertComma, UI32 elem)
{
    string          tmpStr;
    stringstream    out;

    if (insertComma == 0)
    {
        out << elem;
        tmpStr = out.str();
        insertComma = 1;
    }
    else
    {
        out << elem;
        tmpStr = "," + out.str();
    }

    return (tmpStr);
}

string UI32Range::addRangeToString (bool &insertComma, UI32 firstElem, UI32 lastElem)
{
    stringstream    out;
    stringstream    out1;
    string          tmpStr;

    if (insertComma == 0)
    {
        out << firstElem;
        tmpStr = out.str() + "-";
        out1 << lastElem;
        tmpStr += out1.str();
        insertComma = 1;
    }
    else
    {
        tmpStr = ",";
        out << firstElem;
        tmpStr += out.str() + "-";
        out1 << lastElem;
        tmpStr += out1.str();
    }

    return (tmpStr);
}

string UI32Range::getUI32RangeStringFromVector (const vector <UI32> &rangeIntegers)
{
    string          rangeString;
    UI32            index = 0;
    UI32            firstElem = 0;
    UI32            currElem = 0;
    bool            insertComma = 0;

    if ( rangeIntegers.size() == 0)
    {
        rangeString = "";
    }
    else if ( rangeIntegers.size() == 1)
    {
    	rangeString = addOneElementToString(insertComma, rangeIntegers[0]);
    }
    else
    {
        firstElem = rangeIntegers[index];
        currElem  = rangeIntegers[index];
        index ++;
        
        while ( index < rangeIntegers.size())
        {
        
            if ( rangeIntegers[index] == (currElem + 1))
            {
                // consecutive. proceed to next elem
                currElem = rangeIntegers[index];
                index ++;
                continue;
            }

            else
            {
                if (firstElem == currElem)
                {
                    // add a single number to string
                    rangeString += addOneElementToString(insertComma, firstElem);
                }
                else
                {
                    // add the range to string
                    rangeString += addRangeToString(insertComma, firstElem, currElem);
                }

                firstElem = rangeIntegers[index];
                currElem  = rangeIntegers[index];
                index ++;
            }
        } // while loop
        
        if (firstElem == currElem)
        {
            // add a single number to string
            rangeString += addOneElementToString(insertComma, firstElem);
        }
        else
        {
            // add the range to string
            rangeString += addRangeToString(insertComma, firstElem, currElem);
        }
    }

    return (rangeString);
}

string UI32Range::getUI32RangeStringFromMap (const map<UI32, bool> &rangeMap)
{
    char    tempBuffer[32] = {0};
    string  tempString = "";
    UI32    lower = 0;
    UI32    upper = 0;
    bool    lower_flag;
    bool    upper_flag;

    map<UI32, bool>::const_iterator pos;
    
    for (pos = rangeMap.begin (); pos != rangeMap.end ();)
    {
        lower = pos->first;
        lower_flag = pos->second;
        if (++pos != rangeMap.end ())
        {
            upper = pos->first - 1;
        upper_flag = pos->second;
        
            if(false == lower_flag && true == upper_flag)
        {
            if (upper != lower)
            {
                snprintf(tempBuffer, 32, "%u-%u", lower, upper);
                tempString =tempString + tempBuffer + ',';
            }
            else
            {
                snprintf(tempBuffer, 32, "%u", lower);
                tempString =tempString + tempBuffer + ',';
            }
        }
    }
    }
    tempString = tempString.substr( 0,tempString.length() - 1);
    return (tempString);
}


bool UI32Range::operator == (const UI32Range &rangeIntegers) const
{
    if (m_UI32RangeMap != rangeIntegers.m_UI32RangeMap)
    {
        return (false);
    }

    return (true);
}

bool UI32Range::operator != (const UI32Range &rangeIntegers) const
{
    return (!(operator == (rangeIntegers)));
}

UI32Range &UI32Range::operator = (const UI32Range &rangeIntegers)
{
    copy (rangeIntegers);

    return (*this);
}

UI32Range UI32Range::operator + (const UI32Range &rangeIntegers)
{
    UI32Range           mergedRange;
    
    if (rangeIntegers.m_UI32RangeMap.empty())
    {
        mergedRange.m_UI32RangeMap = m_UI32RangeMap;
    }
    else
    {
        mergedRange.m_UI32RangeMap = mergeMap (m_UI32RangeMap, rangeIntegers.m_UI32RangeMap);
    }

    return mergedRange;
}

UI32Range UI32Range::operator + (const UI32 &rangeInteger)
{       
    UI32Range           mergedRange;
    map<UI32, bool>     firstMap = m_UI32RangeMap;
    map<UI32, bool>     secondMap;

    secondMap [rangeInteger]      = false;
    secondMap [rangeInteger+1]    = true;

    mergedRange.m_UI32RangeMap = mergeMap (firstMap, secondMap);

    return mergedRange;
}   

UI32Range UI32Range::operator - (const UI32Range &rangeIntegers)
{
    UI32Range           diffRange;
    map<UI32, bool>     secondMap;
    map<UI32, bool>     tempMap;

    /* removing a segment from a range means RANGE_1 = complement ((complement (RANGE_1)) + SEGMENT_1) */
    
    if (rangeIntegers.m_UI32RangeMap.empty())
    {
        diffRange.m_UI32RangeMap = m_UI32RangeMap;
    }
    else
    {
        tempMap = complement (m_UI32RangeMap);
    
        secondMap = mergeMap (tempMap, rangeIntegers.m_UI32RangeMap);

        diffRange.m_UI32RangeMap = complement (secondMap);
    }

    return diffRange;
}

UI32Range UI32Range::operator - (const UI32 &rangeInteger)
{
    UI32Range           diffRange;
    map<UI32, bool>     secondMap;
    map<UI32, bool>     tempMap;

    /* removing a segment from a range means RANGE_1 = complement ((complement (RANGE_1)) + SEGMENT_1) */
    tempMap = complement (m_UI32RangeMap);

    secondMap [rangeInteger]      = false;
    secondMap [rangeInteger+1]    = true;
    
    tempMap = mergeMap (tempMap,secondMap);
    
    diffRange.m_UI32RangeMap = complement (tempMap);

    return diffRange;
}

UI32Range UI32Range::operator += (const UI32Range &rangeIntegers)
{   
    (*this) = (*this) + rangeIntegers;
    return (*this);
}
    
UI32Range UI32Range::operator -= (const UI32Range &rangeIntegers)
{   
    (*this) = (*this) - rangeIntegers;
    return (*this);
}

UI32Range UI32Range::operator += (const UI32 &rangeInteger)
{
    (*this) = (*this) + rangeInteger;
    return (*this);
}

UI32Range UI32Range::operator -= (const UI32 &rangeInteger)
{
    (*this) = (*this) - rangeInteger;
    return (*this);
}

UI32Range UI32Range::intersection (const UI32Range &secondRangeIntegers)
{
    
    UI32Range intersectedRange = (*this) - secondRangeIntegers;
    intersectedRange = (*this) - intersectedRange;
    return (intersectedRange);
}

bool UI32Range::find (const UI32 &rangeInteger)
{
    map<UI32, bool>     resultMap;

    resultMap[rangeInteger]     = false;
    resultMap[rangeInteger + 1] = true; 

    resultMap = mergeMap (m_UI32RangeMap, resultMap);

    return (m_UI32RangeMap == resultMap);
}

UI32 UI32Range::getCount () const
{
    UI32            i = 0;
    UI32            count =0;
    UI32            countFirstLevel = 0;
    UI32            countSecondLevel = 0;
    UI32            startValue = 0;
    UI32            endValue   = 0;
    vector<string>  firstLevelToken;
    vector<string>  secondLevelToken;
    string          tempStringRange = getUI32RangeStringFromMap (m_UI32RangeMap);
    tokenize (tempStringRange, firstLevelToken, ',');
    
    countFirstLevel = firstLevelToken.size ();
    
    for (i = 0; i < countFirstLevel; i++)
    {
        tokenize (firstLevelToken[i], secondLevelToken, '-');
        countSecondLevel = secondLevelToken.size ();
        prismAssert (2 >= countSecondLevel, __FILE__, __LINE__);

        if (countSecondLevel == 2 || countSecondLevel == 1)
        {
            startValue = strtoul (secondLevelToken[0].c_str (), NULL, 10);
            endValue   = strtoul (secondLevelToken[countSecondLevel - 1].c_str (), NULL, 10);
            count = count + (endValue - startValue + 1);
        }    
    }
    
    return (count);
}

bool UI32Range::isEmpty () const
{
    return (m_UI32RangeMap.empty());
}

bool UI32Range::containsAll (const UI32Range &rangeIntegers)
{
    map<UI32, bool>     resultMap;

    /*A range containAll members of a segment if the merge (range+segment) is same as the range */
    
    resultMap = mergeMap (m_UI32RangeMap, rangeIntegers.m_UI32RangeMap);

    return (m_UI32RangeMap == resultMap);
}

bool UI32Range::containsAny (const UI32Range &rangeIntegers)
{
    UI32Range           mergedRange;
    map<UI32, bool>     resultMap;
    map<UI32, bool>     tempMap;

    /*A range containsAny one element of the segment if the merge (range + complement(segment)) is not equal to complement(segment) */
    
    tempMap =  complement (rangeIntegers.m_UI32RangeMap);

    resultMap = mergeMap (m_UI32RangeMap, tempMap);

    return (!(tempMap == resultMap));
}

map<UI32, bool> UI32Range::mergeMap (map<UI32, bool> range1, map<UI32, bool> range2)
{
    map<UI32, bool> rangeMap = range1;

    map<UI32, bool>::const_iterator pos;
    
    /*merging second map in first map */
    
    for (pos = range2.begin (); pos != range2.end (); pos++)
    {
        rangeMap [pos->first] = pos->second;
    }
    
    /*to remove overlapping ranges */

     for (pos = rangeMap.begin (); pos != rangeMap.end (); pos++)                                                                                                                                        {                                                                                                                                                                                                      mergeToPreviousMap (rangeMap, pos->first);                                                                         
     } 
     
    return(rangeMap);
}

map<UI32, bool> UI32Range::complement (map<UI32, bool> rangeMap)
{
    
    map<UI32, bool>::const_iterator pos;
    
    for (pos = rangeMap.begin (); pos != rangeMap.end (); pos++)
    {
        if(pos->second)
        {
            rangeMap[pos->first] = false;
        }
        else
        {
            rangeMap[pos->first] = true;
        }

    }
    // if the range is empty then it will be represented as lower limit (0) set as TRUE and upper limit(UINT_MAX -1) set as FALSE

    if( (rangeMap.find(0) == rangeMap.end()) && (rangeMap.find(UINT_MAX) == rangeMap.end()))
    {
        rangeMap[0] = false;
        rangeMap[UINT_MAX] = true;
    }
    else if ((rangeMap.find(0) == rangeMap.end()) && (rangeMap.find(UINT_MAX) != rangeMap.end()))
    {
        rangeMap[0] = false;
    }
    else if ((rangeMap.find(0) != rangeMap.end()) && (rangeMap.find(UINT_MAX) == rangeMap.end()))
    {
        rangeMap[UINT_MAX] = true;
    }
    return(rangeMap);
}
}
