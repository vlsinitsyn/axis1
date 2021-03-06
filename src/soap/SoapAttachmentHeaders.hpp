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

/*
 * @author Rangika Mendis (rangika@opensource.lk)
 * @author Nithyakala Thangarajah (nithya@opensource.lk)
 * @author Roshan Weerasuriya (roshan@opensource.lk, roshan@jkcsworld.com)
 *
 */


#if !defined(AFX_SOAPATTACHMENTHEADERS_H__88A54506_DC18_41D4_B484_AB877ADC42D4__INCLUDED_)
#define AFX_SOAPATTACHMENTHEADERS_H__88A54506_DC18_41D4_B484_AB877ADC42D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include <axis/GDefine.hpp>

AXIS_CPP_NAMESPACE_START

using namespace std;

class SoapSerializer;

class SoapAttachmentHeaders  
{
private:    
    vector < pair < AxisString, AxisString > > m_AttachHeaders;

public:
    const char *getHeader(AxisString sName);
    void serialize(SoapSerializer& pSZ);
    void addHeader(AxisString name, AxisString value);
    SoapAttachmentHeaders();
    virtual ~SoapAttachmentHeaders();
};

AXIS_CPP_NAMESPACE_END

#endif // !defined(AFX_SOAPATTACHMENTHEADERS_H__88A54506_DC18_41D4_B484_AB877ADC42D4__INCLUDED_)

