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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef HTTPTOOLKIT_H
#define HTTPTOOLKIT_H

#include "Framework/Types/Types.h"

#include <string>

using namespace std;

namespace WaveNs
{

class HttpToolKit
{
    private :
    protected :
    public :
        static WaveHttpInterfaceMethod getHttpInterfaceMethod             (const string &httpRequest);
        static WaveHttpInterfaceMethod getHttpInterfaceMethodFromString   (const string &httpRequestMethodString);

        static WaveHttpContentType     getHttpContentTypeFromString       (const string &contentTypeString);

        static void                    addHeader                          (string &httpResponseString);
        static void                    addFooter                          (string &httpResponseString);
        static void                    getFrontPage                       (string &frontPageString);
        static void                    getNotImplementedErrorString       (string &notImplementedErrorString);
        static void                    getMethodNotAllowedErrorString     (string &methodNotAllowedErrorString);
        static void                    getNotFoundErrorString             (string &notFoundErrorString);
        static void                    getUnauthorizedString              (string &unAuthorizedString);
        static void                    getBadRequestString                (string &badRequestString, const string &reason = "Bad Request");
        static void                    getCreatedString                   (string &createdString, const string &createdUri);
        static void                    getCreatedStringForRestXml         (string &createdString, const string &createdUri, const string &xmlBody);
        static void                    getDeletedStringForRestXml         (string &deletedString, const string &xmlBody);

        static void                    getOkStringForGetForRestXml        (string &okString, const string &xmlBody);
        static void                    getNoContentStringForPutForRestXml (string &noContentString);

        static void                    decodeUrl                          (string &url);

        static void                    getWaveServerPagePrePortion        (string &waveServerPagePrePortion);
        static void                    getWaveServerPagePostPortion       (string &waveServerPagePostPortion);

    // Now the data members

    private :
    protected :
    public :
};
}

#endif // HTTPTOOLKIT_H
