/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

/**
 * @file AxisEngine.h
 *
 *   @author Susantha Kumara (skumara@virtusa.com)
 *
 */

#if !defined(__AXISENGINE_OF_AXIS_INCLUDED__)
#define __AXISENGINE_OF_AXIS_INCLUDED__


#include "../soap/SoapSerializer.h"
#include "../soap/SoapDeSerializer.h"
#include "../common/MessageData.h"
#include "HandlerChain.h"

/*
 *   @class AxisEngine
 *   @brief
 *
 *   @author Susantha Kumara (skumara@virtusa.com)
 */

AXIS_CPP_NAMESPACE_START

class AxisEngine 
{
protected:    
    SoapSerializer* m_pSZ;
    SoapDeSerializer* m_pDZ;
    HandlerChain* m_pGReqFChain;
    HandlerChain* m_pGResFChain;
    HandlerChain* m_pTReqFChain;
    HandlerChain* m_pTResFChain;
    HandlerChain* m_pSReqFChain;
    HandlerChain* m_pSResFChain;
    MessageData* m_pMsgData;
    PROVIDERTYPE m_CurrentProviderType;
public:
    static int m_bServer;
public:
    AxisEngine();
    virtual ~AxisEngine();
    virtual int initialize();
    virtual void unInitialize();
    virtual int process(SOAPTransport* pSoap)=0;
protected:
    virtual int invoke(MessageData* pMsg)=0;
    virtual void onFault(MessageData* pMsg)=0;
    virtual int initializeHandlers(string& sSessionId, 
        AXIS_PROTOCOL_TYPE protocol);
};

AXIS_CPP_NAMESPACE_END

#endif

