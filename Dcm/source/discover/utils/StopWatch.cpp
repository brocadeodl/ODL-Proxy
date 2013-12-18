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
 * StopWatch.cpp
 *
 *  Created on: Jun 15, 2012
 *      Author: rmadapur
 */

#include "discover/utils/StopWatch.h"

namespace DcmNs {

    StopWatch::StopWatch() :
            isTimerSet(false), isTimerStopped(false), microsecondsInSeconds(
                    1000000.0), microsecondsInMilliseconds(1000.0), millisecondsInSeconds(
                    1000.0) {
    }
    int StopWatch::start() {
        //rusage ru;
        int retVal = gettimeofday(&(timer.start), NULL);
        //int retVal = getrusage(RUSAGE_SELF, &ru);
        //timeradd(&(ru.ru_utime), &(ru.ru_stime), &(timer.start));
        isTimerSet = true;
        isTimerStopped = false;
        return retVal;
    }
    int StopWatch::stop() {
        //rusage ru;
        int retVal = gettimeofday(&(timer.stop), NULL);
        //int retVal = getrusage(RUSAGE_SELF, &ru);
        //timeradd(&(ru.ru_utime), &(ru.ru_stime), &(timer.stop));
        isTimerSet = true;
        isTimerStopped = true;
        return retVal;
    } //  // Returns the elapsed CPU time in milliseconds  //
    timeval StopWatch::getElapsedTime() {
        timeval res;
        if (isTimerSet && !isTimerStopped) {
            stop();
        }
        if (isTimerSet && isTimerStopped) {

            timersub(&(timer.stop), &(timer.start), &res);
            return res;
        }
        else // this means timer never properly started.
        {
            return res;
        }
    }

} /* namespace DcmNs */
