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

#include "Framework/Types/Uri.h"
#include "Framework/Utils/Integer.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

Uri::Uri ()
    : m_port           (0),
      m_isHierarchical (false),
      m_isOpaque       (false),
      m_isAbsolute     (false),
      m_isValid        (false)
{
}

Uri::Uri (const string &uriString)
{
    fromString (uriString);
}

Uri::Uri (const string &scheme, const string &schemeSpecificPart, const string &fragment)
    : m_scheme             (scheme),
      m_schemeSpecificPart (schemeSpecificPart),
      m_port               (0),
      m_fragment           (fragment),
      m_isHierarchical     (false),
      m_isOpaque           (false),
      m_isAbsolute         (false),
      m_isValid            (false)

{
    // Compute other flags based on the information computed above

    computeFlags ();

    // Further process the scheme specific part.

    parseSchemeSpecificPart ();
}

Uri::Uri (const string &scheme, const string userInformation, const string &host, const SI32 &port, const string &path, const string &query, const string &fragment)
    : m_scheme          (scheme),
      m_userInformation (userInformation),
      m_host            (host),
      m_port            (port),
      m_path            (path),
      m_query           (query),
      m_fragment        (fragment),
      m_isHierarchical  (true),
      m_isOpaque        (false),
      m_isAbsolute      (false),
      m_isValid         (false)

{
}

Uri::Uri (const string &scheme, const string &host, const string &path, const string &fragment)
    : m_scheme          (scheme),
      m_host            (host),
      m_port            (0),
      m_path            (path),
      m_fragment        (fragment),
      m_isHierarchical  (true),
      m_isOpaque        (false),
      m_isAbsolute      (false),
      m_isValid         (false)

{
}

Uri::Uri (const string &scheme, const string &authority, const string &path, const string &query, const string &fragment)
    : m_scheme         (scheme),
      m_authority      (authority),
      m_port           (0),
      m_path           (path),
      m_query          (query),
      m_fragment       (fragment),
      m_isHierarchical (true),
      m_isOpaque       (false),
      m_isAbsolute     (false),
      m_isValid        (false)
{
    parseAuthority ();
}

Uri::Uri (const Uri &thatUri)
{
    m_scheme             = thatUri.m_scheme;
    m_schemeSpecificPart = thatUri.m_schemeSpecificPart;
    m_authority          = thatUri.m_authority;
    m_userInformation    = thatUri.m_userInformation;
    m_host               = thatUri.m_host;
    m_port               = thatUri.m_port;
    m_path               = thatUri.m_path;
    m_query              = thatUri.m_query;
    m_fragment           = thatUri.m_fragment;
    m_isHierarchical     = thatUri.m_isHierarchical;
    m_isOpaque           = thatUri.m_isOpaque;
    m_isAbsolute         = thatUri.m_isAbsolute;
    m_isValid            = thatUri.m_isValid;
}

Uri::~Uri ()
{
}

Uri &Uri::operator = (const Uri &thatUri)
{
    m_scheme             = thatUri.m_scheme;
    m_schemeSpecificPart = thatUri.m_schemeSpecificPart;
    m_authority          = thatUri.m_authority;
    m_userInformation    = thatUri.m_userInformation;
    m_host               = thatUri.m_host;
    m_port               = thatUri.m_port;
    m_path               = thatUri.m_path;
    m_query              = thatUri.m_query;
    m_fragment           = thatUri.m_fragment;
    m_isHierarchical     = thatUri.m_isHierarchical;
    m_isOpaque           = thatUri.m_isOpaque;
    m_isAbsolute         = thatUri.m_isAbsolute;
    m_isValid            = thatUri.m_isValid;

    return (*this);
}

bool Uri::operator == (const Uri &thatUri)
{
    if (m_scheme != thatUri.m_scheme)
    {
        return (false);
    }

    if (m_schemeSpecificPart != thatUri.m_schemeSpecificPart)
    {
        return (false);
    }

    if (m_authority != thatUri.m_authority)
    {
        return (false);
    }

    if (m_userInformation != thatUri.m_userInformation)
    {
        return (false);
    }

    if (m_host != thatUri.m_host)
    {
        return (false);
    }

    if (m_port != thatUri.m_port)
    {
        return (false);
    }

    if (m_path != thatUri.m_path)
    {
        return (false);
    }

    if (m_query != thatUri.m_query)
    {
        return (false);
    }

    if (m_fragment != thatUri.m_fragment)
    {
        return (false);
    }

    if (m_isHierarchical != thatUri.m_isHierarchical)
    {
        return (false);
    }

    if (m_isOpaque != thatUri.m_isOpaque)
    {
        return (false);
    }

    if (m_isAbsolute != thatUri.m_isAbsolute)
    {
        return (false);
    }

    if (m_isValid != thatUri.m_isValid)
    {
        return (false);
    }

    return (true);
}

void Uri::fromString (const string &uriString)
{
    invalidate ();

    string::size_type firstOccuranceOfSlash = uriString.find ('/');
    string::size_type firstOccuranceOfColon     = uriString.find (':');
    string::size_type firstOccuranceOfHash      = uriString.find ('#');

    if (string::npos != firstOccuranceOfHash)
    {
        if (string::npos != firstOccuranceOfColon)
        {
            if (firstOccuranceOfHash < firstOccuranceOfColon)
            {
                return;
            }
        }

        if (string::npos != firstOccuranceOfSlash)
        {
            if (firstOccuranceOfHash < firstOccuranceOfSlash)
            {
                return;
            }
        }
    }

    // compute m_scheme and m_schemeSpecificPart

    if (string::npos != firstOccuranceOfColon)
    {
        if (string::npos != firstOccuranceOfSlash)
        {
            if (firstOccuranceOfSlash > firstOccuranceOfColon)
            {
                m_scheme = uriString.substr (0, firstOccuranceOfColon);

                if (string::npos != firstOccuranceOfHash)
                {
                    m_schemeSpecificPart = uriString.substr (firstOccuranceOfColon + 1, firstOccuranceOfHash - firstOccuranceOfColon - 1);
                }
                else
                {
                    m_schemeSpecificPart = uriString.substr (firstOccuranceOfColon + 1);
                }
            }
            else
            {
                if (string::npos != firstOccuranceOfHash)
                {
                    m_schemeSpecificPart = uriString.substr (0, firstOccuranceOfHash);
                }
                else
                {
                    m_schemeSpecificPart = uriString.substr (0);
                }
            }
        }
        else
        {
            m_scheme = uriString.substr (0, firstOccuranceOfColon);

            if (string::npos != firstOccuranceOfHash)
            {
                m_schemeSpecificPart = uriString.substr (firstOccuranceOfColon + 1, firstOccuranceOfHash - firstOccuranceOfColon - 1);
            }
            else
            {
                m_schemeSpecificPart = uriString.substr (firstOccuranceOfColon + 1);
            }
        }
    }
    else
    {
        m_scheme = "";

        if (string::npos != firstOccuranceOfHash)
        {
            m_schemeSpecificPart = uriString.substr (0, firstOccuranceOfHash);
        }
        else
        {
            m_schemeSpecificPart = uriString.substr (0);
        }
    }

    // compute m_fragment

    if (string::npos != firstOccuranceOfHash)
    {
        m_fragment = uriString.substr (firstOccuranceOfHash + 1);
    }

    // Compute other flags based on the information computed above

    computeFlags ();

    // Further process the scheme specific part.

    parseSchemeSpecificPart ();
}

void Uri::computeFlags ()
{
    // Compute other flags based on the information computed above

    if ("" != m_schemeSpecificPart)
    {
        m_isValid = true;
    }
    else
    {
        invalidate ();
        return;
    }

    if ("" != m_scheme)
    {
        m_isAbsolute = true;
    }

    if (true == m_isAbsolute)
    {
        if ('/' == m_schemeSpecificPart[0])
        {
            m_isOpaque       = false;
            m_isHierarchical = true;
        }
        else
        {
            m_isOpaque = true;
        }
    }
    else
    {
        m_isHierarchical = true;
    }
}

void Uri::parseSchemeSpecificPart ()
{
    if (2 <= (m_schemeSpecificPart.size ()))
    {
        if (('/' == m_schemeSpecificPart[0]) && ('/' == m_schemeSpecificPart[1]))
        {
            string::size_type firstSlashPositionAfterInitialSlashes = m_schemeSpecificPart.find ('/', 2);

            if (string::npos != firstSlashPositionAfterInitialSlashes)
            {
                m_authority = m_schemeSpecificPart.substr (2, firstSlashPositionAfterInitialSlashes - 2);

                string::size_type firstQuestionMarkPositionAfterAuthority = m_schemeSpecificPart.find ('?', firstSlashPositionAfterInitialSlashes + 1);

                if (string::npos != firstQuestionMarkPositionAfterAuthority)
                {
                    m_path = m_schemeSpecificPart.substr (firstSlashPositionAfterInitialSlashes, firstQuestionMarkPositionAfterAuthority - firstSlashPositionAfterInitialSlashes);

                    m_query = m_schemeSpecificPart.substr (firstQuestionMarkPositionAfterAuthority  + 1);
                }
                else
                {
                    m_path = m_schemeSpecificPart.substr (firstSlashPositionAfterInitialSlashes);
                }
            }
            else
            {
                m_authority = m_schemeSpecificPart.substr (2);
            }
        }
    }

    // Further process authority

    parseAuthority ();
}

void Uri::parseAuthority ()
{
    if (0 == (m_authority.size ()))
    {
        return;

    }

    string::size_type firstOccuranceOfAt     = m_authority.find ('@');
    string::size_type startOfHostInAuthority = 0;

    if (string::npos != firstOccuranceOfAt)
    {
        m_userInformation      = m_authority.substr (0, firstOccuranceOfAt);
        startOfHostInAuthority = firstOccuranceOfAt + 1;
    }

    string::size_type firstOccuranceOfColonInAuthority = m_authority.find (':', startOfHostInAuthority);

    if (string::npos != firstOccuranceOfColonInAuthority)
    {
        m_host = m_authority.substr (startOfHostInAuthority, firstOccuranceOfColonInAuthority - startOfHostInAuthority);

        string portString = m_authority.substr (firstOccuranceOfColonInAuthority + 1);

        m_port = strtoul (portString.c_str (), NULL, 10);
    }
    else
    {
        m_host = m_authority.substr (startOfHostInAuthority);
    }
}

void Uri::toString (string &uriString) const
{

    if ("" != m_scheme)
    {
        uriString += m_scheme;

        if ("" != m_schemeSpecificPart)
        {
            uriString += ":" + m_schemeSpecificPart;
        }

        if ("" != m_fragment)
        {
            uriString += "#" + m_fragment;
        }
    }
    else
    {
        uriString += m_schemeSpecificPart;

        if ("" != m_fragment)
        {
            uriString += "#" + m_fragment;
        }
    }
}

void Uri::computeAuthority ()
{
    m_authority = "";

    if (false == (getIsHierarchical ()))
    {
        return;
    }

    if ("" != m_host)
    {
        if ("" != m_userInformation)
        {
            m_authority = m_userInformation + "@";
        }

        m_authority += m_host;

        if (0 < m_port)
        {
            m_authority += ":" + (Integer (m_port)).toString ();
        }
    }
}

void Uri::computeSchemeSpecificPart ()
{

    if (("" != m_authority) && ("" != m_authority) && ("" != m_path))
    {
        m_schemeSpecificPart = "";

        if (false == (getIsHierarchical ()))
        {
            return;
        }

        if ("" != m_authority)
        {
            m_schemeSpecificPart += "//" + m_authority;
        }

        m_schemeSpecificPart += m_path;

        if ("" != m_query)
        {
            m_schemeSpecificPart += "?" + m_query;
        }
    }
    else
    {
        // Nothing to do.  Leave the current m_scheSpecificPart as it is.
    }
}

void Uri::invalidate ()
{
    m_scheme             = "";
    m_schemeSpecificPart = "";
    m_authority          = "";
    m_userInformation    = "";
    m_host               = "";
    m_port               = 0;
    m_path               = "";
    m_query              = "";
    m_fragment           = "";
    m_isHierarchical     = false;
    m_isOpaque           = false;
    m_isAbsolute         = false;
    m_isValid            = false;
}

string Uri::getScheme () const
{
    return (m_scheme);
}

string Uri::getSchemeSpecificPart () const
{
    return (m_schemeSpecificPart);
}

string Uri::getAuthority () const
{
    return (m_authority);
}

string Uri::getUserInformation () const
{
    return (m_userInformation);
}

string Uri::getHost () const
{
    return (m_host);
}

SI32 Uri::getPort () const
{
    return (m_port);
}

string Uri::getPath () const
{
    return (m_path);
}

string Uri::getQuery () const
{
    return (m_query);
}

string Uri::getFragment () const
{
    return (m_fragment);
}

bool Uri::getIsHierarchical () const
{
    return (m_isHierarchical);
}

bool Uri::getIsOpaque () const
{
    return (m_isOpaque);
}

bool Uri::getIsAbsolute () const
{
    return (m_isAbsolute);
}

bool Uri::getIsValid () const
{
    return (m_isValid);
}

void Uri::print () const
{
    string uriString;

    toString (uriString);

    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_scheme             = %s", m_scheme.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_schemeSpecificPart = %s", m_schemeSpecificPart.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_authority          = %s", m_authority.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_userInformation    = %s", m_userInformation.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_host               = %s", m_host.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_port               = %d", m_port);
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_path               = %s", m_path.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_query              = %s", m_query.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_fragment           = %s", m_fragment.c_str ());
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_isHierarchical     = %s", m_isHierarchical ? "TRUE" : "FALSE");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_isOpaque           = %s", m_isOpaque       ? "TRUE" : "FALSE");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_isAbsolute         = %s", m_isAbsolute     ? "TRUE" : "FALSE");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "m_isValid            = %s", m_isValid        ? "TRUE" : "FALSE");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "URI                  = %s", uriString.c_str ());
}

}
