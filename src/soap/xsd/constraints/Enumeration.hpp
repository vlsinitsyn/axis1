/*
 *   Copyright 2003-2004 The Apache Software Foundation.
// (c) Copyright IBM Corp. 2004, 2005 All Rights Reserved
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
  * @file Enumeration.hpp
  */
  
#if !defined(_ENUMERATION_HPP____OF_AXIS_INCLUDED_)
#define _ENUMERATION_HPP____OF_AXIS_INCLUDED_

#include "IConstrainingFacet.hpp"
#include "../../AxisSoapException.h"

AXIS_CPP_NAMESPACE_START

/**
 *   @class Enumeration
 *
 *   @author Adrian Dick (adrian.dick@uk.ibm.com)
 *
 */
class Enumeration : public IConstrainingFacet {

public:

    Enumeration();
    
    Enumeration(AxisChar** enumeration, int numberOfValues);
    
    ~Enumeration();

    void validateEnumeration(const AxisChar* value) throw (AxisSoapException);
    
private:
    AxisChar** m_Enumeration;
    int m_NumberOfValues;

};

AXIS_CPP_NAMESPACE_END

#endif
