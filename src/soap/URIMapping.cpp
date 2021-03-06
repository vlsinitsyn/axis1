/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */


// !!! This include file must be first thing in file !!!
#include "../platforms/PlatformAutoSense.hpp"

#include "URIMapping.h"
#include "../common/AxisUtils.h"

#include "../common/AxisTrace.h"

#define __TRC(X) AxisUtils::Convert(X)

AXIS_CPP_NAMESPACE_START

using namespace std;
map<AxisXMLString, URITYPE> URIMapping::m_sURIMap;
volatile bool URIMapping::m_bInit = false;

URIMapping::URIMapping()
{

}

URIMapping::~URIMapping()
{

}

void URIMapping::initialize()
{
    if (!m_bInit)
    {
        m_sURIMap[std::string("http://www.w3.org/2001/XMLSchema")] = URI_XSD;
        m_sURIMap[std::string("http://www.w3.org/2001/XMLSchema-instance")] = 
            URI_XSI;

        m_sURIMap[std::string("http://www.w3.org/2001/06/soap-encoding")] = URI_ENC;
        m_sURIMap[std::string("http://schemas.xmlsoap.org/soap/encoding/")] = URI_ENC;
        m_sURIMap[std::string("http://schemas.xmlsoap.org/soap/envelope/")] = 
            URI_ENVELOPE;

        m_sURIMap[std::string("http://www.w3.org/1999/XMLSchema")] = URI_XSD;
        m_sURIMap[std::string("http://www.w3.org/1999/XMLSchema-instance")] = 
            URI_XSI;

        m_bInit = true;
    }
}

void URIMapping::uninitialize()
{
    if (m_bInit)
    {
        m_sURIMap.clear();
        m_bInit = false;
    }
}
URITYPE URIMapping::getURI(const AxisXMLCh* uri)
{
    if (m_sURIMap.find(std::string(uri)) != m_sURIMap.end())
    {
        return m_sURIMap[std::string(uri)];
    }
    return URI_UNKNOWN;
}

AXIS_CPP_NAMESPACE_END
