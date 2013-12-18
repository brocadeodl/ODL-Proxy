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

#include "HttpInterface/WaveServerPage.h"
#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"
#include "HttpInterface/WaveServerPageDirectory.h"
#include "HttpInterface/HttpToolKit.h"
#include "Framework/Utils/ServerStreamingSocket.h"

namespace WaveNs
{

WaveServerPage::WaveServerPage (HttpInterfaceReceiverObjectManager *pHttpInterfaceReceiverObjectManager, const string& path)
    : WaveWorker (pHttpInterfaceReceiverObjectManager),
      m_path     (path)
{
    WaveServerPageDirectory::registerServerPage (this);
}


WaveServerPage::~WaveServerPage ()
{
}

string WaveServerPage::getPageHeading () const
{
    return (m_pageHeading);
}

void WaveServerPage::setPageHeading (const string& pageHeading)
{
    m_pageHeading = pageHeading;
}

void WaveServerPage::get (const HttpRequest& httpRequest)
{
    string                 httpResponseString;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket      ();

    getWaveServerPagePrePortionForGet (httpResponseString);

    httpResponseString += "<H2 STYLE=\"COLOR:RGB(255, 0, 0);TEXT-ALIGN:CENTER;\">" + m_pageHeading + "</H2>\r\n";

    getWaveServerPagePostPortionForGet (httpResponseString);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponseString;
    }
}

void WaveServerPage::head (const HttpRequest& httpRequest)
{
}

void WaveServerPage::post (const HttpRequest& httpRequest)
{
    string                 httpResponseString;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket      ();

    getWaveServerPagePrePortionForPost (httpResponseString);

    httpResponseString += "<H2 STYLE=\"COLOR:RGB(255, 0, 0);TEXT-ALIGN:CENTER;\">" + m_pageHeading + "</H2>\r\n";

    getWaveServerPagePostPortionForPost (httpResponseString);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponseString;
    }
}

void WaveServerPage::put (const HttpRequest& httpRequest)
{
    string                 httpResponseString;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket      ();

    getWaveServerPagePrePortionForPut (httpResponseString);

    httpResponseString += "<H2 STYLE=\"COLOR:RGB(255, 0, 0);TEXT-ALIGN:CENTER;\">" + m_pageHeading + "</H2>\r\n";

    getWaveServerPagePostPortionForPut (httpResponseString);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponseString;
    }
}

void WaveServerPage::deleteMethod (const HttpRequest& httpRequest)
{
    string                 httpResponseString;
    ServerStreamingSocket *pServerStreamingSocket = httpRequest.getPServerStreamingSocket      ();

    getWaveServerPagePrePortionForDelete (httpResponseString);

    httpResponseString += "<H2 STYLE=\"COLOR:RGB(255, 0, 0);TEXT-ALIGN:CENTER;\">" + m_pageHeading + "</H2>\r\n";

    getWaveServerPagePostPortionForDelete (httpResponseString);

    if (NULL != pServerStreamingSocket)
    {
        (*pServerStreamingSocket) << httpResponseString;
    }
}

void WaveServerPage::options (const HttpRequest& httpRequest)
{
}

void WaveServerPage::traceX (const HttpRequest& httpRequest)
{
}

void WaveServerPage::connect (const HttpRequest& httpRequest)
{
}

void WaveServerPage::getWaveServerPagePrePortionForGet (string& waveServerPagePrePortionForGet)
{
    vector<string> optionsForGet;
    UI32           numberOfOptionsForGet;
    UI32           i                      = 0;

    getListOfOptionsForGet (optionsForGet);

    numberOfOptionsForGet = optionsForGet.size ();

    waveServerPagePrePortionForGet  = "HTTP/1.1 200 OK\r\n";
    waveServerPagePrePortionForGet += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    waveServerPagePrePortionForGet += "Content-Type: text/html\r\n";
    waveServerPagePrePortionForGet += "\r\n";
    waveServerPagePrePortionForGet += "<HTML>\r\n";

    waveServerPagePrePortionForGet += "<HEAD>\r\n";

    waveServerPagePrePortionForGet += "<STYLE>\r\n";

    waveServerPagePrePortionForGet += "A:VISITED\r\n";
    waveServerPagePrePortionForGet += "{\r\n";
    waveServerPagePrePortionForGet += "COLOR:RGB(255, 0 ,0);\r\n";
    waveServerPagePrePortionForGet += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortionForGet += "}\r\n";

    waveServerPagePrePortionForGet += "TABLE\r\n";
    waveServerPagePrePortionForGet += "{\r\n";
    waveServerPagePrePortionForGet += "BORDER-COLLAPSE:COLLAPSE;\r\n";
    waveServerPagePrePortionForGet += "LEFT-MARGIN:AUTO;\r\n";
    waveServerPagePrePortionForGet += "RIGHT-MARGIN:AUTO;\r\n";
    waveServerPagePrePortionForGet += "}\r\n";
    waveServerPagePrePortionForGet += "TABLE, TD, TH\r\n";
    waveServerPagePrePortionForGet += "{\r\n";
    waveServerPagePrePortionForGet += "BORDER:1PX SOLID RED;\r\n";
    waveServerPagePrePortionForGet += "}\r\n";

    waveServerPagePrePortionForGet += "A:LINK\r\n";
    waveServerPagePrePortionForGet += "{\r\n";
    waveServerPagePrePortionForGet += "COLOR:RGB(0, 0 ,255);\r\n";
    waveServerPagePrePortionForGet += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortionForGet += "}\r\n";

    waveServerPagePrePortionForGet += "A:HOVER\r\n";
    waveServerPagePrePortionForGet += "{\r\n";
    waveServerPagePrePortionForGet += "COLOR:RGB(0, 255 ,0)\r\n";
    waveServerPagePrePortionForGet += "}\r\n";

    waveServerPagePrePortionForGet += "A:ACTIVE\r\n";
    waveServerPagePrePortionForGet += "{\r\n";
    waveServerPagePrePortionForGet += "COLOR:RGB(255, 255 ,0)\r\n";
    waveServerPagePrePortionForGet += "}\r\n";

    waveServerPagePrePortionForGet += "</STYLE>\r\n";

    waveServerPagePrePortionForGet += "</HEAD>\r\n";

    waveServerPagePrePortionForGet += "<BODY LINK=\"COLOR:RGB(255,0,0)\" VLINK=\"COLOR:RGB(0, 255, 0)\" ALINK=\"COLOR:RGB(0, 0, 255)\">\r\n";
    waveServerPagePrePortionForGet += "<DIV id=\"EntirePage\" STYLE=\"BACKGROUND-COLOR:RGB(255, 255, 255)\">\r\n";

    waveServerPagePrePortionForGet += "<DIV id=\"Header\" STYLE=\"WIDTH:100%;HEIGHT:5%;BACKGROUND-COLOR:RGB(255, 255,255);COLOR:RGB(200, 200, 200);TEXT-ALIGN:CENTER;\">\r\n";
    waveServerPagePrePortionForGet += "<A>\r\n";
    waveServerPagePrePortionForGet += "<H1 STYLE=\"MARGIN-BOTTOM:0\">Wave World Wide Web Server (W4S)</H1>\r\n";
    waveServerPagePrePortionForGet += "</A>\r\n";
    waveServerPagePrePortionForGet += "</DIV>\r\n";

    waveServerPagePrePortionForGet += "<DIV id=\"HeaderOptions\" STYLE=\"WIDTH:100%;HEIGHT:3%;BACKGROUND-COLOR:RGB(0, 0,0);COLOR:RGB(255, 255, 255);TEXT-ALIGN:CENTER;FLOAT:LEFT\">\r\n";
    waveServerPagePrePortionForGet += "<A HREF=\"/\">Home</A>\r\n";
    waveServerPagePrePortionForGet += " | \r\n";
    waveServerPagePrePortionForGet += "<A HREF=\"/\">About</A>\r\n";
    waveServerPagePrePortionForGet += " | \r\n";
    waveServerPagePrePortionForGet += "<A HREF=\"/\">Contact Us</A>\r\n";
    waveServerPagePrePortionForGet += "</DIV>\r\n";

    waveServerPagePrePortionForGet += "<DIV id=\"MenuPane\" STYLE=\"WIDTH:20%;HEIGHT:92%;BACKGROUND-COLOR:RGB(202,200,200);COLOR:RGB(255,255,255);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";

    waveServerPagePrePortionForGet += "<HR/>\r\n";

    if (0 < numberOfOptionsForGet)
    {
        waveServerPagePrePortionForGet += "<UL>\r\n";

        for (i = 0; i < numberOfOptionsForGet; i++)
        {
            waveServerPagePrePortionForGet += "<LI><A HREF=\"" + m_path + "/" + optionsForGet[i] + "\">" + optionsForGet[i] + "</A></BR></LI>\r\n";
        }

        waveServerPagePrePortionForGet += "</UL>\r\n";
    }

    waveServerPagePrePortionForGet += "<HR/>\r\n";

    waveServerPagePrePortionForGet += "</DIV>\r\n";

    waveServerPagePrePortionForGet += "<DIV id=\"ContentPane\" STYLE=\"WIDTH:80%;HEIGHT:92%;BACKGROUND-COLOR:RGB(255, 255, 255);COLOR:RGB(0, 0, 0);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";
}

void WaveServerPage::getWaveServerPagePostPortionForGet (string& waveServerPagePostPortionForGet)
{
    waveServerPagePostPortionForGet += "</DIV>\r\n";

    waveServerPagePostPortionForGet += "</DIV>\r\n";
    waveServerPagePostPortionForGet += "</BODY>\r\n";
    waveServerPagePostPortionForGet += "</HTML>\r\n";
}

void WaveServerPage::getWaveServerPagePrePortionForPost (string &waveServerPagePrePortionForPost)
{
    vector<string> optionsForPost;
    UI32           numberOfOptionsForPost;
    UI32           i                      = 0;

    getListOfOptionsForPost (optionsForPost);

    numberOfOptionsForPost = optionsForPost.size ();

    waveServerPagePrePortionForPost  = "HTTP/1.1 200 OK\r\n";
    waveServerPagePrePortionForPost += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    waveServerPagePrePortionForPost += "Content-Type: text/html\r\n";
    waveServerPagePrePortionForPost += "\r\n";
    waveServerPagePrePortionForPost += "<HTML>\r\n";

    waveServerPagePrePortionForPost += "<HEAD>\r\n";

    waveServerPagePrePortionForPost += "<STYLE>\r\n";

    waveServerPagePrePortionForPost += "A:VISITED\r\n";
    waveServerPagePrePortionForPost += "{\r\n";
    waveServerPagePrePortionForPost += "COLOR:RGB(255, 0 ,0);\r\n";
    waveServerPagePrePortionForPost += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortionForPost += "}\r\n";

    waveServerPagePrePortionForPost += "TABLE\r\n";
    waveServerPagePrePortionForPost += "{\r\n";
    waveServerPagePrePortionForPost += "BORDER-COLLAPSE:COLLAPSE;\r\n";
    waveServerPagePrePortionForPost += "LEFT-MARGIN:AUTO;\r\n";
    waveServerPagePrePortionForPost += "RIGHT-MARGIN:AUTO;\r\n";
    waveServerPagePrePortionForPost += "}\r\n";
    waveServerPagePrePortionForPost += "TABLE, TD, TH\r\n";
    waveServerPagePrePortionForPost += "{\r\n";
    waveServerPagePrePortionForPost += "BORDER:1PX SOLID RED;\r\n";
    waveServerPagePrePortionForPost += "}\r\n";

    waveServerPagePrePortionForPost += "A:LINK\r\n";
    waveServerPagePrePortionForPost += "{\r\n";
    waveServerPagePrePortionForPost += "COLOR:RGB(0, 0 ,255);\r\n";
    waveServerPagePrePortionForPost += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortionForPost += "}\r\n";

    waveServerPagePrePortionForPost += "A:HOVER\r\n";
    waveServerPagePrePortionForPost += "{\r\n";
    waveServerPagePrePortionForPost += "COLOR:RGB(0, 255 ,0)\r\n";
    waveServerPagePrePortionForPost += "}\r\n";

    waveServerPagePrePortionForPost += "A:ACTIVE\r\n";
    waveServerPagePrePortionForPost += "{\r\n";
    waveServerPagePrePortionForPost += "COLOR:RGB(255, 255 ,0)\r\n";
    waveServerPagePrePortionForPost += "}\r\n";

    waveServerPagePrePortionForPost += "</STYLE>\r\n";

    waveServerPagePrePortionForPost += "</HEAD>\r\n";

    waveServerPagePrePortionForPost += "<BODY LINK=\"COLOR:RGB(255,0,0)\" VLINK=\"COLOR:RGB(0, 255, 0)\" ALINK=\"COLOR:RGB(0, 0, 255)\">\r\n";
    waveServerPagePrePortionForPost += "<DIV id=\"EntirePage\" STYLE=\"BACKGROUND-COLOR:RGB(255, 255, 255)\">\r\n";

    waveServerPagePrePortionForPost += "<DIV id=\"Header\" STYLE=\"WIDTH:100%;HEIGHT:5%;BACKGROUND-COLOR:RGB(255, 255,255);COLOR:RGB(200, 200, 200);TEXT-ALIGN:CENTER;\">\r\n";
    waveServerPagePrePortionForPost += "<A>\r\n";
    waveServerPagePrePortionForPost += "<H1 STYLE=\"MARGIN-BOTTOM:0\">Wave World Wide Web Server (W4S)</H1>\r\n";
    waveServerPagePrePortionForPost += "</A>\r\n";
    waveServerPagePrePortionForPost += "</DIV>\r\n";

    waveServerPagePrePortionForPost += "<DIV id=\"HeaderOptions\" STYLE=\"WIDTH:100%;HEIGHT:3%;BACKGROUND-COLOR:RGB(0, 0,0);COLOR:RGB(255, 255, 255);TEXT-ALIGN:CENTER;FLOAT:LEFT\">\r\n";
    waveServerPagePrePortionForPost += "<A HREF=\"/\">Home</A>\r\n";
    waveServerPagePrePortionForPost += " | \r\n";
    waveServerPagePrePortionForPost += "<A HREF=\"/\">About</A>\r\n";
    waveServerPagePrePortionForPost += " | \r\n";
    waveServerPagePrePortionForPost += "<A HREF=\"/\">Contact Us</A>\r\n";
    waveServerPagePrePortionForPost += "</DIV>\r\n";

    waveServerPagePrePortionForPost += "<DIV id=\"MenuPane\" STYLE=\"WIDTH:20%;HEIGHT:92%;BACKGROUND-COLOR:RGB(202,200,200);COLOR:RGB(255,255,255);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";

    waveServerPagePrePortionForPost += "<HR/>\r\n";

    if (0 < numberOfOptionsForPost)
    {
        waveServerPagePrePortionForPost += "<UL>\r\n";

        for (i = 0; i < numberOfOptionsForPost; i++)
        {
            waveServerPagePrePortionForPost += "<LI><A HREF=\"" + m_path + "/" + optionsForPost[i] + "\">" + optionsForPost[i] + "</A></BR></LI>\r\n";
        }

        waveServerPagePrePortionForPost += "</UL>\r\n";
    }

    waveServerPagePrePortionForPost += "<HR/>\r\n";

    waveServerPagePrePortionForPost += "</DIV>\r\n";

    waveServerPagePrePortionForPost += "<DIV id=\"ContentPane\" STYLE=\"WIDTH:80%;HEIGHT:92%;BACKGROUND-COLOR:RGB(255, 255, 255);COLOR:RGB(0, 0, 0);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";
}

void WaveServerPage::getWaveServerPagePostPortionForPost (string &waveServerPagePostPortionForPost)
{
    waveServerPagePostPortionForPost += "</DIV>\r\n";

    waveServerPagePostPortionForPost += "</DIV>\r\n";
    waveServerPagePostPortionForPost += "</BODY>\r\n";
    waveServerPagePostPortionForPost += "</HTML>\r\n";
}

void WaveServerPage::getWaveServerPagePrePortionForPut (string &waveServerPagePrePortionForPut)
{
    vector<string> optionsForPut;
    UI32           numberOfOptionsForPut;
    UI32           i                      = 0;

    getListOfOptionsForPut (optionsForPut);

    numberOfOptionsForPut = optionsForPut.size ();

    waveServerPagePrePortionForPut  = "HTTP/1.1 200 OK\r\n";
    waveServerPagePrePortionForPut += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    waveServerPagePrePortionForPut += "Content-Type: text/html\r\n";
    waveServerPagePrePortionForPut += "\r\n";
    waveServerPagePrePortionForPut += "<HTML>\r\n";

    waveServerPagePrePortionForPut += "<HEAD>\r\n";

    waveServerPagePrePortionForPut += "<STYLE>\r\n";

    waveServerPagePrePortionForPut += "A:VISITED\r\n";
    waveServerPagePrePortionForPut += "{\r\n";
    waveServerPagePrePortionForPut += "COLOR:RGB(255, 0 ,0);\r\n";
    waveServerPagePrePortionForPut += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortionForPut += "}\r\n";

    waveServerPagePrePortionForPut += "TABLE\r\n";
    waveServerPagePrePortionForPut += "{\r\n";
    waveServerPagePrePortionForPut += "BORDER-COLLAPSE:COLLAPSE;\r\n";
    waveServerPagePrePortionForPut += "LEFT-MARGIN:AUTO;\r\n";
    waveServerPagePrePortionForPut += "RIGHT-MARGIN:AUTO;\r\n";
    waveServerPagePrePortionForPut += "}\r\n";
    waveServerPagePrePortionForPut += "TABLE, TD, TH\r\n";
    waveServerPagePrePortionForPut += "{\r\n";
    waveServerPagePrePortionForPut += "BORDER:1PX SOLID RED;\r\n";
    waveServerPagePrePortionForPut += "}\r\n";

    waveServerPagePrePortionForPut += "A:LINK\r\n";
    waveServerPagePrePortionForPut += "{\r\n";
    waveServerPagePrePortionForPut += "COLOR:RGB(0, 0 ,255);\r\n";
    waveServerPagePrePortionForPut += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortionForPut += "}\r\n";

    waveServerPagePrePortionForPut += "A:HOVER\r\n";
    waveServerPagePrePortionForPut += "{\r\n";
    waveServerPagePrePortionForPut += "COLOR:RGB(0, 255 ,0)\r\n";
    waveServerPagePrePortionForPut += "}\r\n";

    waveServerPagePrePortionForPut += "A:ACTIVE\r\n";
    waveServerPagePrePortionForPut += "{\r\n";
    waveServerPagePrePortionForPut += "COLOR:RGB(255, 255 ,0)\r\n";
    waveServerPagePrePortionForPut += "}\r\n";

    waveServerPagePrePortionForPut += "</STYLE>\r\n";

    waveServerPagePrePortionForPut += "</HEAD>\r\n";

    waveServerPagePrePortionForPut += "<BODY LINK=\"COLOR:RGB(255,0,0)\" VLINK=\"COLOR:RGB(0, 255, 0)\" ALINK=\"COLOR:RGB(0, 0, 255)\">\r\n";
    waveServerPagePrePortionForPut += "<DIV id=\"EntirePage\" STYLE=\"BACKGROUND-COLOR:RGB(255, 255, 255)\">\r\n";

    waveServerPagePrePortionForPut += "<DIV id=\"Header\" STYLE=\"WIDTH:100%;HEIGHT:5%;BACKGROUND-COLOR:RGB(255, 255,255);COLOR:RGB(200, 200, 200);TEXT-ALIGN:CENTER;\">\r\n";
    waveServerPagePrePortionForPut += "<A>\r\n";
    waveServerPagePrePortionForPut += "<H1 STYLE=\"MARGIN-BOTTOM:0\">Wave World Wide Web Server (W4S)</H1>\r\n";
    waveServerPagePrePortionForPut += "</A>\r\n";
    waveServerPagePrePortionForPut += "</DIV>\r\n";

    waveServerPagePrePortionForPut += "<DIV id=\"HeaderOptions\" STYLE=\"WIDTH:100%;HEIGHT:3%;BACKGROUND-COLOR:RGB(0, 0,0);COLOR:RGB(255, 255, 255);TEXT-ALIGN:CENTER;FLOAT:LEFT\">\r\n";
    waveServerPagePrePortionForPut += "<A HREF=\"/\">Home</A>\r\n";
    waveServerPagePrePortionForPut += " | \r\n";
    waveServerPagePrePortionForPut += "<A HREF=\"/\">About</A>\r\n";
    waveServerPagePrePortionForPut += " | \r\n";
    waveServerPagePrePortionForPut += "<A HREF=\"/\">Contact Us</A>\r\n";
    waveServerPagePrePortionForPut += "</DIV>\r\n";

    waveServerPagePrePortionForPut += "<DIV id=\"MenuPane\" STYLE=\"WIDTH:20%;HEIGHT:92%;BACKGROUND-COLOR:RGB(202,200,200);COLOR:RGB(255,255,255);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";

    waveServerPagePrePortionForPut += "<HR/>\r\n";

    if (0 < numberOfOptionsForPut)
    {
        waveServerPagePrePortionForPut += "<UL>\r\n";

        for (i = 0; i < numberOfOptionsForPut; i++)
        {
            waveServerPagePrePortionForPut += "<LI><A HREF=\"" + m_path + "/" + optionsForPut[i] + "\">" + optionsForPut[i] + "</A></BR></LI>\r\n";
        }

        waveServerPagePrePortionForPut += "</UL>\r\n";
    }

    waveServerPagePrePortionForPut += "<HR/>\r\n";

    waveServerPagePrePortionForPut += "</DIV>\r\n";

    waveServerPagePrePortionForPut += "<DIV id=\"ContentPane\" STYLE=\"WIDTH:80%;HEIGHT:92%;BACKGROUND-COLOR:RGB(255, 255, 255);COLOR:RGB(0, 0, 0);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";
}

void WaveServerPage::getWaveServerPagePostPortionForPut (string &waveServerPagePostPortionForPut)
{
    waveServerPagePostPortionForPut += "</DIV>\r\n";

    waveServerPagePostPortionForPut += "</DIV>\r\n";
    waveServerPagePostPortionForPut += "</BODY>\r\n";
    waveServerPagePostPortionForPut += "</HTML>\r\n";
}

void WaveServerPage::getWaveServerPagePrePortionForDelete (string &waveServerPagePrePortionForDelete)
{
    vector<string> optionsForDelete;
    UI32           numberOfOptionsForDelete;
    UI32           i                      = 0;

    getListOfOptionsForDelete (optionsForDelete);

    numberOfOptionsForDelete = optionsForDelete.size ();

    waveServerPagePrePortionForDelete  = "HTTP/1.1 200 OK\r\n";
    waveServerPagePrePortionForDelete += "Server: Wave World Wide Web Server (W4S) v0.0.1\r\n";
    waveServerPagePrePortionForDelete += "Content-Type: text/html\r\n";
    waveServerPagePrePortionForDelete += "\r\n";
    waveServerPagePrePortionForDelete += "<HTML>\r\n";

    waveServerPagePrePortionForDelete += "<HEAD>\r\n";

    waveServerPagePrePortionForDelete += "<STYLE>\r\n";

    waveServerPagePrePortionForDelete += "A:VISITED\r\n";
    waveServerPagePrePortionForDelete += "{\r\n";
    waveServerPagePrePortionForDelete += "COLOR:RGB(255, 0 ,0);\r\n";
    waveServerPagePrePortionForDelete += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortionForDelete += "}\r\n";

    waveServerPagePrePortionForDelete += "TABLE\r\n";
    waveServerPagePrePortionForDelete += "{\r\n";
    waveServerPagePrePortionForDelete += "BORDER-COLLAPSE:COLLAPSE;\r\n";
    waveServerPagePrePortionForDelete += "LEFT-MARGIN:AUTO;\r\n";
    waveServerPagePrePortionForDelete += "RIGHT-MARGIN:AUTO;\r\n";
    waveServerPagePrePortionForDelete += "}\r\n";
    waveServerPagePrePortionForDelete += "TABLE, TD, TH\r\n";
    waveServerPagePrePortionForDelete += "{\r\n";
    waveServerPagePrePortionForDelete += "BORDER:1PX SOLID RED;\r\n";
    waveServerPagePrePortionForDelete += "}\r\n";

    waveServerPagePrePortionForDelete += "A:LINK\r\n";
    waveServerPagePrePortionForDelete += "{\r\n";
    waveServerPagePrePortionForDelete += "COLOR:RGB(0, 0 ,255);\r\n";
    waveServerPagePrePortionForDelete += "TEXT-DECORATION:NONE;\r\n";
    waveServerPagePrePortionForDelete += "}\r\n";

    waveServerPagePrePortionForDelete += "A:HOVER\r\n";
    waveServerPagePrePortionForDelete += "{\r\n";
    waveServerPagePrePortionForDelete += "COLOR:RGB(0, 255 ,0)\r\n";
    waveServerPagePrePortionForDelete += "}\r\n";

    waveServerPagePrePortionForDelete += "A:ACTIVE\r\n";
    waveServerPagePrePortionForDelete += "{\r\n";
    waveServerPagePrePortionForDelete += "COLOR:RGB(255, 255 ,0)\r\n";
    waveServerPagePrePortionForDelete += "}\r\n";

    waveServerPagePrePortionForDelete += "</STYLE>\r\n";

    waveServerPagePrePortionForDelete += "</HEAD>\r\n";

    waveServerPagePrePortionForDelete += "<BODY LINK=\"COLOR:RGB(255,0,0)\" VLINK=\"COLOR:RGB(0, 255, 0)\" ALINK=\"COLOR:RGB(0, 0, 255)\">\r\n";
    waveServerPagePrePortionForDelete += "<DIV id=\"EntirePage\" STYLE=\"BACKGROUND-COLOR:RGB(255, 255, 255)\">\r\n";

    waveServerPagePrePortionForDelete += "<DIV id=\"Header\" STYLE=\"WIDTH:100%;HEIGHT:5%;BACKGROUND-COLOR:RGB(255, 255,255);COLOR:RGB(200, 200, 200);TEXT-ALIGN:CENTER;\">\r\n";
    waveServerPagePrePortionForDelete += "<A>\r\n";
    waveServerPagePrePortionForDelete += "<H1 STYLE=\"MARGIN-BOTTOM:0\">Wave World Wide Web Server (W4S)</H1>\r\n";
    waveServerPagePrePortionForDelete += "</A>\r\n";
    waveServerPagePrePortionForDelete += "</DIV>\r\n";

    waveServerPagePrePortionForDelete += "<DIV id=\"HeaderOptions\" STYLE=\"WIDTH:100%;HEIGHT:3%;BACKGROUND-COLOR:RGB(0, 0,0);COLOR:RGB(255, 255, 255);TEXT-ALIGN:CENTER;FLOAT:LEFT\">\r\n";
    waveServerPagePrePortionForDelete += "<A HREF=\"/\">Home</A>\r\n";
    waveServerPagePrePortionForDelete += " | \r\n";
    waveServerPagePrePortionForDelete += "<A HREF=\"/\">About</A>\r\n";
    waveServerPagePrePortionForDelete += " | \r\n";
    waveServerPagePrePortionForDelete += "<A HREF=\"/\">Contact Us</A>\r\n";
    waveServerPagePrePortionForDelete += "</DIV>\r\n";

    waveServerPagePrePortionForDelete += "<DIV id=\"MenuPane\" STYLE=\"WIDTH:20%;HEIGHT:92%;BACKGROUND-COLOR:RGB(202,200,200);COLOR:RGB(255,255,255);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";

    waveServerPagePrePortionForDelete += "<HR/>\r\n";

    if (0 < numberOfOptionsForDelete)
    {
        waveServerPagePrePortionForDelete += "<UL>\r\n";

        for (i = 0; i < numberOfOptionsForDelete; i++)
        {
            waveServerPagePrePortionForDelete += "<LI><A HREF=\"" + m_path + "/" + optionsForDelete[i] + "\">" + optionsForDelete[i] + "</A></BR></LI>\r\n";
        }

        waveServerPagePrePortionForDelete += "</UL>\r\n";
    }

    waveServerPagePrePortionForDelete += "<HR/>\r\n";

    waveServerPagePrePortionForDelete += "</DIV>\r\n";

    waveServerPagePrePortionForDelete += "<DIV id=\"ContentPane\" STYLE=\"WIDTH:80%;HEIGHT:92%;BACKGROUND-COLOR:RGB(255, 255, 255);COLOR:RGB(0, 0, 0);TEXT-ALIGN:LEFT;FLOAT:LEFT\">\r\n";
}

void WaveServerPage::getWaveServerPagePostPortionForDelete (string &waveServerPagePostPortionForDelete)
{
    waveServerPagePostPortionForDelete += "</DIV>\r\n";

    waveServerPagePostPortionForDelete += "</DIV>\r\n";
    waveServerPagePostPortionForDelete += "</BODY>\r\n";
    waveServerPagePostPortionForDelete += "</HTML>\r\n";
}

void WaveServerPage::getListOfOptionsForGet (vector<string> &optionsForGet) const
{
}

void WaveServerPage::getListOfOptionsForPost (vector<string> &optionsForPost) const
{
}

void WaveServerPage::getListOfOptionsForPut (vector<string> &optionsForPost) const
{
}

void WaveServerPage::getListOfOptionsForDelete (vector<string> &optionsForDelete) const
{
}

string WaveServerPage::getPath () const
{
    return (m_path);
}

}
