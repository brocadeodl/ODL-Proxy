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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Modeling/YANG/Parser/YinParser.h"
#include "Framework/Utils/TraceUtils.h"
#include "Modeling/YANG/ObjectModel/YangFactory/YangElementFactory.h"
#include "Modeling/YANG/ObjectModel/YangModule.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <ctype.h>

#include <vector>
#include <algorithm>

using namespace XERCES_CPP_NAMESPACE;
using namespace std;

namespace WaveNs
{

YinParser::YinParser ()
{
}

YinParser::~YinParser ()
{
}

YangElement *YinParser::parseFile (const string &yinFileName)
{
    XMLPlatformUtils::Initialize ();

    YangElementFactory::initialize ();

    XercesDOMParser    yinFileParser;
    XMLCh             *pYinFileName      = XMLString::transcode (yinFileName.c_str ());

    yinFileParser.setValidationScheme           (XercesDOMParser::Val_Auto);
    yinFileParser.setDoNamespaces               (false);
    yinFileParser.setDoSchema                   (false);
    yinFileParser.setCreateEntityReferenceNodes (false);

    LocalFileInputSource yinfileInputSource (pYinFileName);

    yinFileParser.parse (yinfileInputSource);

    DOMDocument *pDocument  = yinFileParser.getDocument ();

    if (NULL == pDocument)
    {
        return (NULL);
    }

    YangModule *pYangModule = NULL;

    DOMElement *pDomElement = pDocument->getDocumentElement ();

    if (NULL != pDomElement)
    {
        pYangModule = new YangModule ();

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->setFileName (yinFileName);

        pYangModule->loadFromDomElement (pDomElement);
    }

    //module.printYin ();

    XMLString::release (&pYinFileName);

    //XMLPlatformUtils::Terminate ();

    return (pYangModule);
}

vector<YangElement *> YinParser::parseFiles (const vector<string> &yinFileNames)
{
    UI32                   i                = 0;
    UI32                   numberOfYinFiles = yinFileNames.size ();
    vector<YangElement *>  yangElements;
    YangElement           *pYangElement     = NULL;

    for (i = 0; i < numberOfYinFiles; i++)
    {
        cout << "Parsing " << yinFileNames[i] << " ... ";

        pYangElement = YinParser::parseFile (yinFileNames[i]);

        yangElements.push_back (pYangElement);

        cout << "Done." << endl;
        fflush (stdout);
    }

    return (yangElements);
}

vector<YangElement *> YinParser::parseDiretory (const string &directoryPath, vector<string> &yinFileNames)
{
    vector<string> filePaths;

    FrameworkToolKit::getAllFilesInTheDirectory (directoryPath, filePaths, ".xml");

    yinFileNames = filePaths;

    return (parseFiles (filePaths));
}

vector<YangElement *>  YinParser::parseDirectoriesAndFiles (const vector<string> &directoryPaths, const vector<string> &yinFileNames, vector<string> &outputYinFileNames)
{
    vector<string> filePaths;
    vector<string> filePathsPerDirectory;
    UI32           numberOfDirectories    = 0;
    UI32           i                      = 0;

    numberOfDirectories = directoryPaths.size ();

    for (i = 0; i < numberOfDirectories; i++)
    {
        filePathsPerDirectory.clear ();

        FrameworkToolKit::getAllFilesInTheDirectory (directoryPaths[i], filePathsPerDirectory, ".xml");

        filePaths.insert (filePaths.end (), filePathsPerDirectory.begin (), filePathsPerDirectory.end ());
    }

    filePaths.insert (filePaths.end (), yinFileNames.begin (), yinFileNames.end ());

    std::sort (filePaths.begin (), filePaths.end ());

    outputYinFileNames = filePaths;

    return (parseFiles (filePaths));
}

ResourceId YinParser::debugParse (UI32 argc, vector<string> argv)
{
    string fileName = argv[1];

    trace (TRACE_LEVEL_INFO, "YinParser::debugParse : Parsing Yin file : " + fileName);

    parseFile (fileName);

    return (0);
}

YangElement *YinParser::parseYangUserInterface (const string &serializedYin)
{
    XMLPlatformUtils::Initialize ();

    YangElementFactory::initialize ();

    static const char * const pId = "user-interface";

    XercesDOMParser serializedYinParser;

    serializedYinParser.setValidationScheme           (XercesDOMParser::Val_Auto);
    serializedYinParser.setDoNamespaces               (false);
    serializedYinParser.setDoSchema                   (false);
    serializedYinParser.setCreateEntityReferenceNodes (false);

    MemBufInputSource *pInputSource = new MemBufInputSource ((const XMLByte *) serializedYin.c_str (), strlen (serializedYin.c_str ()), pId, false);

    prismAssert (NULL != pInputSource, __FILE__, __LINE__);

    serializedYinParser.parse (*pInputSource);

    DOMDocument *pDocument  = serializedYinParser.getDocument ();

    if (NULL == pDocument)
    {
        return (NULL);
    }

    YangUserInterface *pYangUserInterface = NULL;

    DOMElement *pDomElement = pDocument->getDocumentElement ();

    if (NULL != pDomElement)
    {
        pYangUserInterface = new YangUserInterface ();

        prismAssert (NULL != pYangUserInterface, __FILE__, __LINE__);

        pYangUserInterface->loadFromDomElement (pDomElement);
    }

    //XMLPlatformUtils::Terminate ();

    return (pYangUserInterface);
}


}
