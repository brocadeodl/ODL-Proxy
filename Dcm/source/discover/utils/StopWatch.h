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
 * StopWatch.h
 *
 *  Created on: Jun 15, 2012
 *      Author: rmadapur
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#define  TIME_FORMAT "%d.%06d"
#define  TIME_PARAM(ab) ab.tv_sec, ab.tv_usec

namespace DcmNs {
    struct stopWatch {
            timeval start;
            timeval stop;
    };

    class StopWatch {

        private:
            stopWatch timer;
            bool isTimerSet;
            bool isTimerStopped;
            const double microsecondsInSeconds;
            const double microsecondsInMilliseconds;
            const double millisecondsInSeconds;
        public:
            StopWatch();
            int start();
            int stop();
            // Returns the elapsed CPU time in milliseconds
            timeval getElapsedTime();


    };

} /* namespace DcmNs */
#endif /* STOPWATCH_H_ */
