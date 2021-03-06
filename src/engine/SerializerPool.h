/* -*- C++ -*- */
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
 *
 */

#if !defined(AXIS_SERIALIZERPOOL_H__OF_AXIS_INCLUDED_)
#define AXIS_SERIALIZERPOOL_H__OF_AXIS_INCLUDED_

#include "SharedObject.h"
#ifdef USER_SERIALIZER
#include <axis/IWrapperSoapSerializer.hpp>
extern void* GetUserSerializer();
#else
#include "../soap/SoapSerializer.h"
#endif 
#include <axis/GDefine.hpp>
#include <list>

AXIS_CPP_NAMESPACE_START

using namespace std;
/*
 *   @class SerializerPool
 *   @brief interface for the SerializerPool class.
 *
 *
 *   @author Susantha Kumara (skumara@virtusa.com)
 */

class SerializerPool:protected SharedObject
{
    public:
        SerializerPool ();
        virtual ~ SerializerPool ();
    private:
        list <IWrapperSoapSerializer*>m_SZList;
    public:
        int getInstance (IWrapperSoapSerializer** ppSZ);
        int putInstance (IWrapperSoapSerializer* pSZ);
};

AXIS_CPP_NAMESPACE_END

#endif 
