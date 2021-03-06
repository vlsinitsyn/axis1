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

// !!! This include file must be first thing in file !!!
#include "../../platforms/PlatformAutoSense.hpp"

#include <axis/client/Call.hpp>
#include <axis/AxisException.hpp>
#include <axis/client/Stub.hpp>

#include "../AxisObjectConverter.hpp"
#include "StubC.h"

#include "../../common/AxisTrace.h"

#include <axis/Axis.h>
#include <axis/GDefine.h>
#include <axis/AxisUserAPI.h>
#include <axis/SoapEnvVersions.h>
#include <axis/TypeMapping.h>
#include <axis/WSDDDefines.h>
#include <axis/client/Call.h>

AXIS_CPP_NAMESPACE_USE

extern "C" {

static void processException(Call *c, AxisException& e)
{
    void *exceptionHandler = (void *)axiscAxisInvokeExceptionHandler;
    void *stubExceptionHandler;
    
    StubC *s = (StubC *)c->getCStub();
    if (s)
        if ((stubExceptionHandler = s->getCExceptionHandler()) != NULL)
            exceptionHandler = stubExceptionHandler;

    try
    {
        c->processSoapFault(&e, exceptionHandler);
    }
    catch ( AxisException& e  )
    {
        // already processing exception...ignore
    }

    if (s)
        s->doNotPerformClientRequest = true;
}


AXISC_STORAGE_CLASS_INFO 
AXISCHANDLE axiscCallCreate() 
{
    try
    {
        Call *c = new Call();
        return (AXISCHANDLE)c;
    }
    catch ( AxisException& e  )
    {
        axiscAxisInvokeExceptionHandler(e.getExceptionCode(), e.what(), NULL, NULL);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (AXISCHANDLE)NULL;
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallDestroy(AXISCHANDLE call) 
{
    if ((AXISCHANDLE)NULL == call)
        return;

    Call *c = (Call*)call;
    
    try
    {
        delete c;
    }
    catch ( AxisException& e  )
    {
        axiscAxisInvokeExceptionHandler(e.getExceptionCode(), e.what(), NULL, NULL);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallSetSOAPVersion(AXISCHANDLE call, 
                             AXISC_SOAP_VERSION version) 
{
    Call *c = (Call*)call;
    
    try
    {
        c->setSOAPVersion((SOAP_VERSION)version);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallSetTransportProperty(AXISCHANDLE call, 
                                  AXISC_TRANSPORT_INFORMATION_TYPE type, 
                                  const char * value) 
{   
    Call *c = (Call*)call;
    
    try
    {
        return c->setTransportProperty((AXIS_TRANSPORT_INFORMATION_TYPE)type, value);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO 
const char * axiscCallGetTransportProperty(AXISCHANDLE call, 
                                           const char *key, 
                                           AxiscBool response) 
{
    Call *c = (Call*)call;
    
    try
    {
        return c->getTransportProperty(key, (bool)(response !=0 ));
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (char *)NULL;
}

AXISC_STORAGE_CLASS_INFO
void axiscCloseTransportConnection(AXISCHANDLE call)
{
    Call *c = (Call*)call;
    
    try
    {
        c->closeTransportConnection();
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }    
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallSetHandlerProperty(AXISCHANDLE call, 
                                AxiscChar * name, 
                                void * value, 
                                int len) 
{
    Call *c = (Call*)call;
    
    try
    {
        return c->setHandlerProperty(name,value,len);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallSetProtocol(AXISCHANDLE call, 
                         AXISC_PROTOCOL_TYPE protocol) 
{
    Call *c = (Call*)call;
    
    try
    {
        return c->setProtocol((AXIS_PROTOCOL_TYPE)protocol);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return -1;
}

AXISC_STORAGE_CLASS_INFO 
AXISC_PROTOCOL_TYPE axiscCallGetProtocol(AXISCHANDLE call) 
{
    Call *c = (Call*)call;
    
    try
    {
        return (AXISC_PROTOCOL_TYPE)(c->getProtocol());
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (AXISC_PROTOCOL_TYPE)-1;
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallUnInitialize(AXISCHANDLE call) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s)
        s->doNotPerformClientRequest = false;
    
    try
    {
        return c->unInitialize();
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallInitialize(AXISCHANDLE call, 
                        AXISC_PROVIDERTYPE nStyle) 
{
    Call *c = (Call*)call;
    
    try
    {
        return c->initialize((PROVIDERTYPE)nStyle);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallInvoke(AXISCHANDLE call) 
{
    return axiscCallSendAndReceive(call);
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallSendAndReceive(AXISCHANDLE call) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
    	return -1;
    
    try
    {
        return c->sendAndReceive();
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallSend(AXISCHANDLE call) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
    	return -1;
    
    try
    {
        return c->send();
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallAddCmplxParameter(AXISCHANDLE call, 
                                void * pObject, 
                                void * pSZFunct, 
                                void * pDelFunct, 
                                const AxiscChar * pName, 
                                const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return;
    
    try
    {
        c->addCmplxParameter(pObject,pSZFunct,pDelFunct,pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallAddCmplxArrayParameter(AXISCHANDLE call, 
                                     Axisc_Array * pArray,
                                     void * pSZFunct, 
                                     void * pDelFunct, 
                                     const AxiscChar * pName, 
                                     const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return;
    
    try
    {
        // Need to convert C-style array to C++-style array before calling serialization method.
        Axis_Array objArray;
        if (pArray)
            objArray.set((void **)pArray->m_Array, pArray->m_Size, (XSDTYPE)pArray->m_Type);
    
        c->addCmplxArrayParameter(&objArray,pSZFunct,pDelFunct,pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallAddBasicArrayParameter(AXISCHANDLE call, 
                                     Axisc_Array * pArray, 
                                     AXISC_XSDTYPE nType, 
                                     const AxiscChar * pName) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return;
    
    try
    {
        // Need to convert C-style array to C++-style array before calling serialization method.
        Axis_Array objArray;
        if (pArray)
            objArray.set((void **)pArray->m_Array, pArray->m_Size, (XSDTYPE)pArray->m_Type);
    
        c->addBasicArrayParameter(&objArray, (XSDTYPE)nType, pName);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallAddParameter(AXISCHANDLE call, 
                           void * pValue, 
                           const char * pchName, 
                           AXISC_XSDTYPE nType) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return;
    
    try
    {
        c->addParameter(pValue,pchName,(XSDTYPE)nType);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO
AXISCHANDLE axiscCallCreateAttribute(AXISCHANDLE call, 
                                     const AxiscChar * pLocalname, 
	                                 const AxiscChar * pPrefix, 
	                                 const AxiscChar * pValue)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (AXISCHANDLE)NULL;
    
    try
    {
        return (AXISCHANDLE)(c->createAttribute(pLocalname, pPrefix, pValue));
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (AXISCHANDLE)NULL;
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallSetOperation(AXISCHANDLE call, 
                           const char * pchOperation, 
                           const char * pchNamespace) 
{
    Call *c = (Call*)call;
    
    try
    {
        c->setOperation(pchOperation,pchNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallSetOperationUnwrapped(AXISCHANDLE call, 
                                    const char * pchOperation, 
                                    const char * pchNamespace) 
{
    Call *c = (Call*)call;
    
    try
    {
        c->setOperation(pchOperation, pchNamespace, false);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallSetEndpointURI(AXISCHANDLE call, 
                            const char * pchEndpointURI) 
{
    Call *c = (Call*)call;
    
    try
    {
        return c->setEndpointURI(pchEndpointURI);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO 
AXISCHANDLE axiscCallCreateHeaderBlock(AXISCHANDLE call, 
                                       AxiscChar * pachLocalName, 
                                       AxiscChar * pachUri, 
                                       AxiscChar * pachPrefix) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (AXISCHANDLE)NULL;
    
    try
    {
        return (AXISCHANDLE)(c->createHeaderBlock(pachLocalName, pachUri, pachPrefix));
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (AXISCHANDLE)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__int * axiscCallGetElementAsInt(AXISCHANDLE call, 
                                     const AxiscChar * pName, 
                                     const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__int *)NULL;
    
    try
    {
        return c->getElementAsInt(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (xsdc__int *)NULL;
}

/*
AXISC_STORAGE_CLASS_INFO 
int axiscCallGetFaultDetail(AXISCHANDLE call, 
                            char * * ppcDetail) 
{
    
    
    Call *c = (Call*)call;
    
    try
    {
        return c->getFaultDetail(ppcDetail);
    }
    catch ( AxisException& e  )
    {
        
        processException(c, e);
    }
    catch ( ... )
    {
          
          
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
   
    return -1;
}
*/








AXISC_STORAGE_CLASS_INFO 
xsdc__boolean * axiscCallGetElementAsBoolean(AXISCHANDLE call, 
                                             const AxiscChar * pName, 
                                             const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__boolean *)NULL;
    
    try
    {
        return (xsdc__boolean* )(c->getElementAsBoolean(pName,pNamespace));
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__boolean *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__unsignedInt * axiscCallGetElementAsUnsignedInt(AXISCHANDLE call, 
                                                     const AxiscChar * pName, 
                                                     const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__unsignedInt *)NULL;
    
    try
    {
        return c->getElementAsUnsignedInt(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__unsignedInt *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__short * axiscCallGetElementAsShort(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__short *)NULL;
    
    try
    {
        return c->getElementAsShort(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__short *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__unsignedShort * axiscCallGetElementAsUnsignedShort(AXISCHANDLE call, 
                                                         const AxiscChar * pName, 
                                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__unsignedShort *)NULL;
    
    try
    {
        return c->getElementAsUnsignedShort(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__unsignedShort *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__byte * axiscCallGetElementAsByte(AXISCHANDLE call, 
                                       const AxiscChar * pName, 
                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__byte *)NULL;
    
    try
    {
        return c->getElementAsByte(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__byte *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__unsignedByte * axiscCallGetElementAsUnsignedByte(AXISCHANDLE call, 
                                                       const AxiscChar * pName, 
                                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__unsignedByte *)NULL;
    
    try
    {
        return c->getElementAsUnsignedByte(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (xsdc__unsignedByte *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__long * axiscCallGetElementAsLong(AXISCHANDLE call, 
                                       const AxiscChar * pName, 
                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__long *)NULL;
    
    try
    {
        return (xsd__long*)(c->getElementAsLong(pName,pNamespace));
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__long *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__integer * axiscCallGetElementAsInteger(AXISCHANDLE call, 
                                             const AxiscChar * pName, 
                                             const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__integer *)NULL;
    
    try
    {
        return c->getElementAsInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__integer *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__unsignedLong * axiscCallGetElementAsUnsignedLong(AXISCHANDLE call, 
                                                       const AxiscChar * pName, 
                                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__unsignedLong *)NULL;
    
    try
    {
        return c->getElementAsUnsignedLong(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__unsignedLong *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__float * axiscCallGetElementAsFloat(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__float *)NULL;
    
    try
    {
        return c->getElementAsFloat(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__float *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__double * axiscCallGetElementAsDouble(AXISCHANDLE call, 
                                           const AxiscChar * pName, 
                                           const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__double *)NULL;
    
    try
    {
        return c->getElementAsDouble(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__double *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__decimal * axiscCallGetElementAsDecimal(AXISCHANDLE call, 
                                             const AxiscChar * pName, 
                                             const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__decimal *)NULL;
    
    try
    {
        return c->getElementAsDecimal(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__decimal *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__string axiscCallGetElementAsString(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__string)NULL;
    
    try
    {
        return c->getElementAsString(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__string)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__anyURI axiscCallGetElementAsAnyURI(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__anyURI)NULL;
    
    try
    {
        return c->getElementAsAnyURI(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (xsdc__anyURI)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__QName axiscCallGetElementAsQName(AXISCHANDLE call, 
                                       const AxiscChar * pName, 
                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__QName)NULL;
    
    try
    {
        return c->getElementAsQName(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__QName)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__hexBinary * axiscCallGetElementAsHexBinary(AXISCHANDLE call, 
                                                 const AxiscChar * pName, 
                                                 const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__hexBinary *)NULL;
    
    try
    {
        xsd__hexBinary * pObjCpp = c->getElementAsHexBinary(pName, pNamespace);
        
        return AxisObjectConverter::cppHexBinaryToC(pObjCpp, NULL, true);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__hexBinary *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__base64Binary * axiscCallGetElementAsBase64Binary(AXISCHANDLE call, 
                                                       const AxiscChar * pName, 
                                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__base64Binary *)NULL;
    
    try
    {
        xsd__base64Binary * pObjCpp = c->getElementAsBase64Binary(pName, pNamespace);
        
        return AxisObjectConverter::cppBase64BinaryToC(pObjCpp, NULL, true);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__base64Binary *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__dateTime * axiscCallGetElementAsDateTime(AXISCHANDLE call, 
                                               const AxiscChar * pName, 
                                               const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__dateTime *)NULL;
    
    try
    {
        return c->getElementAsDateTime(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__dateTime *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__date * axiscCallGetElementAsDate(AXISCHANDLE call, 
                                       const AxiscChar * pName, 
                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__date *)NULL;
    
    try
    {
        return c->getElementAsDate(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__date *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__time * axiscCallGetElementAsTime(AXISCHANDLE call, 
                                       const AxiscChar * pName, 
                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__time *)NULL;
    
    try
    {
        return c->getElementAsTime(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__time *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__duration * axiscCallGetElementAsDuration(AXISCHANDLE call, 
                                               const AxiscChar * pName, 
                                               const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__duration *)NULL;
    
    try
    {
        return c->getElementAsDuration(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__duration *)NULL;
}



AXISC_STORAGE_CLASS_INFO
xsdc__gYearMonth * axiscCallGetElementAsGYearMonth(AXISCHANDLE call, 
                                                   const AxiscChar * pName, 
                                                   const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gYearMonth *)NULL;
    
    try
    {
        return c->getElementAsGYearMonth(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gYearMonth *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__gYear * axiscCallGetElementAsGYear(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gYear *)NULL;
    
    try
    {
        return c->getElementAsGYear(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gYear *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__gMonthDay * axiscCallGetElementAsGMonthDay(AXISCHANDLE call, 
                                                 const AxiscChar * pName, 
                                                 const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gMonthDay *)NULL;
    
    try
    {
        return c->getElementAsGMonthDay(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gMonthDay *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__gDay * axiscCallGetElementAsGDay(AXISCHANDLE call, 
                                       const AxiscChar * pName,
                                       const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gDay *)NULL;
    
    try
    {
        return c->getElementAsGDay(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gDay *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__gMonth * axiscCallGetElementAsGMonth(AXISCHANDLE call, 
                                           const AxiscChar * pName, 
                                           const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gMonth *)NULL;
    
    try
    {
        return c->getElementAsGMonth(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gMonth *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__nonPositiveInteger * axiscCallGetElementAsNonPositiveInteger(AXISCHANDLE call, 
                                                                   const AxiscChar * pName,
                                                                   const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__nonPositiveInteger *)NULL;
    
    try
    {
        return c->getElementAsNonPositiveInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__nonPositiveInteger *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__negativeInteger * axiscCallGetElementAsNegativeInteger(AXISCHANDLE call, 
                                                             const AxiscChar * pName,
                                                             const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__negativeInteger *)NULL;
    
    try
    {
        return c->getElementAsNegativeInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__negativeInteger *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__nonNegativeInteger * axiscCallGetElementAsNonNegativeInteger(AXISCHANDLE call, 
                                                                   const AxiscChar * pName,
                                                                   const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__nonNegativeInteger *)NULL;
    
    try
    {
        return c->getElementAsNonNegativeInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__nonNegativeInteger *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__positiveInteger * axiscCallGetElementAsPositiveInteger(AXISCHANDLE call, 
                                                             const AxiscChar * pName, 
                                                             const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__positiveInteger *)NULL;
    
    try
    {
        return c->getElementAsPositiveInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__positiveInteger *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__normalizedString axiscCallGetElementAsNormalizedString(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__normalizedString)NULL;
    
    try
    {
        return c->getElementAsNormalizedString(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__normalizedString)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__token axiscCallGetElementAsToken(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__token)NULL;
    
    try
    {
        return c->getElementAsToken(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__token)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__language axiscCallGetElementAsLanguage(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__language)NULL;
    
    try
    {
        return c->getElementAsLanguage(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__language)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__Name axiscCallGetElementAsName(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__Name)NULL;
    
    try
    {
        return c->getElementAsName(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__Name)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__NCName axiscCallGetElementAsNCName(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__NCName)NULL;
    
    try
    {
        return c->getElementAsNCName(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__NCName)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__ID axiscCallGetElementAsID(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__ID)NULL;
    
    try
    {
        return c->getElementAsID(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__ID)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__IDREF axiscCallGetElementAsIDREF(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__IDREF)NULL;
    
    try
    {
        return c->getElementAsIDREF(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__IDREF)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__IDREFS axiscCallGetElementAsIDREFS(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__IDREFS)NULL;
    
    try
    {
        return c->getElementAsIDREFS(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__IDREFS)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__ENTITY axiscCallGetElementAsENTITY(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__ENTITY)NULL;
    
    try
    {
        return c->getElementAsENTITY(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__ENTITY)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__ENTITIES axiscCallGetElementAsENTITIES(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__ENTITIES)NULL;
    
    try
    {
        return c->getElementAsENTITIES(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__ENTITIES)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__NMTOKEN axiscCallGetElementAsNMTOKEN(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__NMTOKEN)NULL;

    try
    {
        return c->getElementAsNMTOKEN(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__NMTOKEN)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__NMTOKENS axiscCallGetElementAsNMTOKENS(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__NMTOKENS)NULL;
    
    try
    {
        return c->getElementAsNMTOKENS(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__NMTOKENS)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__NOTATION axiscCallGetElementAsNOTATION(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__NOTATION)NULL;
    
    try
    {
        return c->getElementAsNOTATION(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__NOTATION)NULL;
}


AXISC_STORAGE_CLASS_INFO
xsdc__anyType axiscCallGetElementAsAnyType(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__anyType)NULL;
    
    try
    {
        return c->getElementAsAnyType(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__anyType)NULL;
}


AXISC_STORAGE_CLASS_INFO 
xsdc__int * axiscCallGetAttributeAsInt(AXISCHANDLE call, 
                                       const AxiscChar * pName,
                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__int *)NULL;
    
    try
    {
        return c->getAttributeAsInt(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__int *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__boolean * axiscCallGetAttributeAsBoolean(AXISCHANDLE call, 
                                               const AxiscChar * pName, const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__boolean *)NULL;
    
    try
    {
        return (xsdc__boolean* )(c->getAttributeAsBoolean(pName,pNamespace));
    }
    catch ( AxisException& e  )
    {
        
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__boolean *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__unsignedInt * axiscCallGetAttributeAsUnsignedInt(AXISCHANDLE call, 
                                                       const AxiscChar * pName, 
                                                       const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__unsignedInt *)NULL;
    
    try
    {
        return c->getAttributeAsUnsignedInt(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__unsignedInt *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__short * axiscCallGetAttributeAsShort(AXISCHANDLE call, 
                                           const AxiscChar * pName, 
                                           const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__short *)NULL;
    
    try
    {
        return c->getAttributeAsShort(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__short *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__unsignedShort * axiscCallGetAttributeAsUnsignedShort(AXISCHANDLE call, 
                                                           const AxiscChar * pName, 
                                                           const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__unsignedShort *)NULL;
    
    try
    {
        return c->getAttributeAsUnsignedShort(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__unsignedShort *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__byte * axiscCallGetAttributeAsByte(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__byte *)NULL;
    
    try
    {
        return c->getAttributeAsByte(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__byte *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__unsignedByte * axiscCallGetAttributeAsUnsignedByte(AXISCHANDLE call, 
                                                         const AxiscChar * pName, 
                                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__unsignedByte *)NULL;
    
    try
    {
        return c->getAttributeAsUnsignedByte(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__unsignedByte *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__long * axiscCallGetAttributeAsLong(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__long *)NULL;
    
    try
    {
        return c->getAttributeAsLong(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__long *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__integer * axiscCallGetAttributeAsInteger(AXISCHANDLE call, 
                                               const AxiscChar * pName, 
                                               const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__integer *)NULL;
    
    try
    {
        return c->getAttributeAsInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__integer *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__unsignedLong * axiscCallGetAttributeAsUnsignedLong(AXISCHANDLE call, 
                                                         const AxiscChar * pName, 
                                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__unsignedLong *)NULL;
    
    try
    {
        return c->getAttributeAsUnsignedLong(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__unsignedLong *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__float * axiscCallGetAttributeAsFloat(AXISCHANDLE call, 
                                           const AxiscChar * pName, 
                                           const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__float *)NULL;

    try
    {
        return c->getAttributeAsFloat(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__float *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__double * axiscCallGetAttributeAsDouble(AXISCHANDLE call, 
                                             const AxiscChar * pName, 
                                             const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__double *)NULL;

    try
    {
        return c->getAttributeAsDouble(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__double *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__decimal * axiscCallGetAttributeAsDecimal(AXISCHANDLE call, 
                                               const AxiscChar * pName, 
                                               const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__decimal *)NULL;

    try
    {
        return c->getAttributeAsDecimal(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__decimal *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__string axiscCallGetAttributeAsString(AXISCHANDLE call, 
                                           const AxiscChar * pName, 
                                           const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__string)NULL;

    try
    {
        return c->getAttributeAsString(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__string)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__anyURI axiscCallGetAttributeAsAnyURI(AXISCHANDLE call, 
                                           const AxiscChar * pName, 
                                           const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__anyURI)NULL;

    try
    {
        return c->getAttributeAsAnyURI(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__anyURI)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__QName axiscCallGetAttributeAsQName(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__QName)NULL;

    try
    {
        return c->getAttributeAsQName(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__QName)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__hexBinary * axiscCallGetAttributeAsHexBinary(AXISCHANDLE call, 
                                                   const AxiscChar * pName, 
                                                   const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__hexBinary *)NULL;

    try
    {
        xsd__hexBinary * pObjCpp = c->getAttributeAsHexBinary(pName, pNamespace);
        
        return AxisObjectConverter::cppHexBinaryToC(pObjCpp, NULL, true);  
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__hexBinary *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__base64Binary * axiscCallGetAttributeAsBase64Binary(AXISCHANDLE call, 
                                                         const AxiscChar * pName, 
                                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__base64Binary *)NULL;

    try
    {
        xsd__base64Binary * pObjCpp = c->getAttributeAsBase64Binary(pName, pNamespace);
        
        return AxisObjectConverter::cppBase64BinaryToC(pObjCpp, NULL, true);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__base64Binary *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__dateTime * axiscCallGetAttributeAsDateTime(AXISCHANDLE call, 
                                                 const AxiscChar * pName, 
                                                 const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__dateTime *)NULL;

    try
    {
        return c->getAttributeAsDateTime(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__dateTime *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__date * axiscCallGetAttributeAsDate(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__date *)NULL;

    try
    {
        return c->getAttributeAsDate(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__date *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__time * axiscCallGetAttributeAsTime(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__time *)NULL;

    try
    {
        return c->getAttributeAsTime(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    return (xsdc__time *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
xsdc__duration * axiscCallGetAttributeAsDuration(AXISCHANDLE call, 
                                                 const AxiscChar * pName, 
                                                 const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__duration *)NULL;

    try
    {
        return c->getAttributeAsDuration(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (xsdc__duration *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__gYearMonth * axiscCallGetAttributeAsGYearMonth(AXISCHANDLE call, 
                                                   const AxiscChar * pName, 
                                                   const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gYearMonth *)NULL;

    try
    {
        return c->getAttributeAsGYearMonth(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gYearMonth *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__gYear * axiscCallGetAttributeAsGYear(AXISCHANDLE call, 
                                         const AxiscChar * pName, 
                                         const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gYear *)NULL;

    try
    {
        return c->getAttributeAsGYear(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gYear *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__gMonthDay * axiscCallGetAttributeAsGMonthDay(AXISCHANDLE call, 
                                                 const AxiscChar * pName, 
                                                 const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gMonthDay *)NULL;

    try
    {
        return c->getAttributeAsGMonthDay(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gMonthDay *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__gDay * axiscCallGetAttributeAsGDay(AXISCHANDLE call, 
                                       const AxiscChar * pName,
                                       const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gDay *)NULL;

    try
    {
        return c->getAttributeAsGDay(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gDay *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__gMonth * axiscCallGetAttributeAsGMonth(AXISCHANDLE call, 
                                           const AxiscChar * pName, 
                                           const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__gMonth *)NULL;

    try
    {
        return c->getAttributeAsGMonth(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__gMonth *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__nonPositiveInteger * axiscCallGetAttributeAsNonPositiveInteger(AXISCHANDLE call, 
                                                                   const AxiscChar * pName,
                                                                   const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__nonPositiveInteger *)NULL;

    try
    {
        return c->getAttributeAsNonPositiveInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__nonPositiveInteger *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__negativeInteger * axiscCallGetAttributeAsNegativeInteger(AXISCHANDLE call, 
                                                             const AxiscChar * pName,
                                                             const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__negativeInteger *)NULL;

    try
    {
        return c->getAttributeAsNegativeInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__negativeInteger *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__nonNegativeInteger * axiscCallGetAttributeAsNonNegativeInteger(AXISCHANDLE call, 
                                                                   const AxiscChar * pName,
                                                                   const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__nonNegativeInteger *)NULL;

    try
    {
        return c->getAttributeAsNonNegativeInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__nonNegativeInteger *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__positiveInteger * axiscCallGetAttributeAsPositiveInteger(AXISCHANDLE call, 
                                                             const AxiscChar * pName, 
                                                             const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__positiveInteger *)NULL;

    try
    {
        return c->getAttributeAsPositiveInteger(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__positiveInteger *)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__normalizedString axiscCallGetAttributeAsNormalizedString(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__normalizedString)NULL;

    try
    {
        return c->getAttributeAsNormalizedString(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__normalizedString)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__token axiscCallGetAttributeAsToken(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__token)NULL;

    try
    {
        return c->getAttributeAsToken(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__token)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__language axiscCallGetAttributeAsLanguage(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__language)NULL;

    try
    {
        return c->getAttributeAsLanguage(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__language)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__Name axiscCallGetAttributeAsName(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__Name)NULL;

    try
    {
        return c->getAttributeAsName(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__Name)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__NCName axiscCallGetAttributeAsNCName(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__NCName)NULL;

    try
    {
        return c->getAttributeAsNCName(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__NCName)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__ID axiscCallGetAttributeAsID(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__ID)NULL;

    try
    {
        return c->getAttributeAsID(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__ID)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__IDREF axiscCallGetAttributeAsIDREF(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__IDREF)NULL;

    try
    {
        return c->getAttributeAsIDREF(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__IDREF)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__IDREFS axiscCallGetAttributeAsIDREFS(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__IDREFS)NULL;

    try
    {
        return c->getAttributeAsIDREFS(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__IDREFS)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__ENTITY axiscCallGetAttributeAsENTITY(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__ENTITY)NULL;

    try
    {
        return c->getAttributeAsENTITY(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__ENTITY)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__ENTITIES axiscCallGetAttributeAsENTITIES(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__ENTITIES)NULL;

    try
    {
        return c->getAttributeAsENTITIES(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__ENTITIES)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__NMTOKEN axiscCallGetAttributeAsNMTOKEN(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__NMTOKEN)NULL;

    try
    {
        return c->getAttributeAsNMTOKEN(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__NMTOKEN)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__NMTOKENS axiscCallGetAttributeAsNMTOKENS(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__NMTOKENS)NULL;

    try
    {
        return c->getAttributeAsNMTOKENS(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__NMTOKENS)NULL;
}

AXISC_STORAGE_CLASS_INFO
xsdc__NOTATION axiscCallGetAttributeAsNOTATION(AXISCHANDLE call, const AxiscChar * pName, 
	const AxiscChar * pNamespace)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__NOTATION)NULL;

    try
    {
        return c->getAttributeAsNOTATION(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return (xsdc__NOTATION)NULL;
}

AXISC_STORAGE_CLASS_INFO 
void * axiscCallGetCmplxObject(AXISCHANDLE call, 
                               void * pDZFunct, 
                               void * pCreFunct, 
                               void * pDelFunct, 
                               const AxiscChar * pName, 
                               const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return NULL;

    try
    {
        return c->getCmplxObject(pDZFunct,pCreFunct,pDelFunct,pName,pNamespace);    
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return NULL;
}

AXISC_STORAGE_CLASS_INFO 
Axisc_Array* axiscCallGetCmplxArray(AXISCHANDLE call, 
                                    Axisc_Array* pArray, 
                                    void * pDZFunct, 
                                    void * pCreFunct, 
                                    void * pDelFunct, 
                                    const AxiscChar * pName, 
                                    const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (Axisc_Array *)NULL;

    try
    {
    	Axis_Array ObjArray;
    	Axis_Array *pObjArray;
    	
    	if (pArray)
    		ObjArray.set(pArray->m_Array, pArray->m_Size, (XSDTYPE)pArray->m_Type);
        
    	pObjArray = c->getCmplxArray(&ObjArray, 
    	                             pDZFunct, pCreFunct,pDelFunct, pName, pNamespace); 
    	                             
    	return AxisObjectConverter::cppArrayToC(pObjArray, pArray, false);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (Axisc_Array *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
Axisc_Array* axiscCallGetBasicArray(AXISCHANDLE call, 
                                    AXISC_XSDTYPE nType, 
                                    const AxiscChar * pName, 
                                    const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (Axisc_Array *)NULL;

    try
    {
    	Axis_Array *pObjArray = c->getBasicArray((XSDTYPE)nType, pName, pNamespace);
    	
    	return AxisObjectConverter::cppArrayToC(pObjArray, NULL, true);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (Axisc_Array *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallCheckMessage(AXISCHANDLE call, 
                          const AxiscChar * pName, 
                          const AxiscChar * pNamespace) 
{
    return axiscCallValidateMessage(call, pName, pNamespace, 1);
}

AXISC_STORAGE_CLASS_INFO
int axiscCallValidateMessage( AXISCHANDLE call, 
		                      const AxiscChar * pName,
                              const AxiscChar * pNamespace,
                              AxiscBool consumeIt)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return -1;

    try
    {
        return c->validateMessage(pName,pNamespace, (bool)(consumeIt != 0));
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallGetChardataAs(AXISCHANDLE call,
		                    void** pValue, 
		                    AXISC_XSDTYPE type)
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
    {
        *pValue = NULL;
        return;
    }

    try
    {
        return c->getChardataAs( pValue,  (XSDTYPE)type);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }    
}

AXISC_STORAGE_CLASS_INFO 
void * axiscCallCheckFault(AXISCHANDLE call, 
                           const AxiscChar * pName, 
                           const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    
    try
    {
        return c->checkFault(pName,pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return NULL;
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallGetStatus(AXISCHANDLE call) 
{
    Call *c = (Call*)call;
    
    try
    {
        return c->getStatus();
    }
    catch ( AxisException& e  )
    {
        
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO
AXISCHANDLE axiscCallGetSOAPSerializer(AXISCHANDLE call)
{
    Call *c = (Call*)call;

    try
    {
        return (AXISCHANDLE)(c->getSOAPSerializer());
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }

    return NULL;
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallSetProxy(AXISCHANDLE call, 
                       const char * pcProxyHost, 
                       unsigned int uiProxyPort) 
{
    Call *c = (Call*)call;
    
    try
    {
        c->setProxy(pcProxyHost,uiProxyPort);    
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO 
AxiscAnyType * axiscCallGetAnyObject(AXISCHANDLE call) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (AxiscAnyType *)NULL;

    try
    {
        AnyType * pObjCpp = c->getAnyObject();
        
        return AxisObjectConverter::cppAnyTypeToC(pObjCpp, NULL, true);
    }
    catch ( AxisException& e  )
    {
        
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (AxiscAnyType *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
int axiscCallAddAnyObject(AXISCHANDLE call, 
                          AxiscAnyType * pAnyObject) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return -1;

    try
    {
        AnyType objAnyType;
        
        if (pAnyObject && pAnyObject->_size > 0)
        {
            objAnyType._size  = pAnyObject->_size;
            objAnyType._array = pAnyObject->_array;
        }
        
        int returnCode = c->addAnyObject(&objAnyType);
        
        // Reset object so it will not free memory.
        objAnyType._size  = 0;
        objAnyType._array = NULL;

        return returnCode;
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return -1;
}

AXISC_STORAGE_CLASS_INFO 
const AxiscChar * axiscCallGetNamespacePrefix(AXISCHANDLE call, 
                                              const AxiscChar * pNamespace) 
{
    Call *c = (Call*)call;
    
    try
    {
        return c->getNamespacePrefix(pNamespace);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    { 
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (AxiscChar *)NULL;
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallSetSOAPMethodAttribute(AXISCHANDLE call, 
                                 const AxiscChar * pLocalname, 
                                 const AxiscChar * pPrefix, 
                                 const AxiscChar * pUri, 
                                 const AxiscChar * pValue) 
{
    Call *c = (Call*)call;
    
    try
    {
        c->setSOAPMethodAttribute(pLocalname,pPrefix,pUri,pValue);
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallClearSOAPMethodAttributes(AXISCHANDLE call)
{
    Call *c = (Call*)call;
    
    try
    {
        c->clearSOAPMethodAttributes();
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
}


AXISC_STORAGE_CLASS_INFO 
xsdc__string axiscCallGetFaultAsXMLString(AXISCHANDLE call) 
{
    Call *c = (Call*)call;
    StubC *s = (StubC *)c->getCStub();
    if (s && s->doNotPerformClientRequest)
        return (xsdc__string)NULL;

    try
    {
        return c->getFaultAsXMLString();
    }
    catch ( AxisException& e  )
    {
        processException(c, e);
    }
    catch ( ... )
    {
        axiscAxisInvokeExceptionHandler(-1, "Unrecognized exception thrown.", NULL, NULL);
    }
    
    return (xsdc__string)NULL;
}

AXISC_STORAGE_CLASS_INFO 
void axiscCallSetCStub(AXISCHANDLE call, void *pStub) 
{ 
        Call *c = (Call*)call;
        c->setCStub(pStub);
}

AXISC_STORAGE_CLASS_INFO 
void *axiscCallGetCStub(AXISCHANDLE call) 
{ 
        Call *c = (Call*)call;
        return c->getCStub();
}

AXISC_STORAGE_CLASS_INFO
void axiscCallSetSoapFaultNamespace(AXISCHANDLE call, const char * pSoapFaultNamespace)
{
    Call *c = (Call*)call;
    c->setSoapFaultNamespace(pSoapFaultNamespace);
}


AXISC_STORAGE_CLASS_INFO
void axiscCallAddSoapFaultToList(AXISCHANDLE call, 
                                 const char * faultName, 
                                 void * createFp,
                                 void * deleteFp,
                                 void * deserializerFp)
{
    Call *c = (Call*)call;
    c->addSoapFaultToList(faultName, createFp, deleteFp, deserializerFp);
}





}
