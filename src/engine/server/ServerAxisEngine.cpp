/* -*- C++ -*- */
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
#include "../../platforms/PlatformAutoSense.hpp"

#include "../../soap/SoapBody.h"
#include "../../soap/SoapMethod.h"
#include "ServerAxisEngine.h"
#include <stdio.h>
#include "../../common/AxisUtils.h"
#include "../../wsdd/WSDDDeployment.h"
#include "../HandlerPool.h"
#include "../../common/AxisTrace.h"

extern AXIS_CPP_NAMESPACE_PREFIX HandlerPool *g_pHandlerPool;
extern AXIS_CPP_NAMESPACE_PREFIX WSDDDeployment *g_pWSDDDeployment;

AXIS_CPP_NAMESPACE_START
ServerAxisEngine::
ServerAxisEngine ()
{
	logEntryEngine("ServerAxisEngine::ServerAxisEngine")

    m_pWebService = NULL;
	
	logExit()
}

ServerAxisEngine::
~ServerAxisEngine ()
{
	logEntryEngine("ServerAxisEngine::~ServerAxisEngine")
	
	logExit()
}

int ServerAxisEngine::
process (SOAPTransport * pStream)
{
	logEntryEngine("ServerAxisEngine::process")

    int Status = 0;
    int nSoapVersion;
    try
    {
        if (!pStream)
        {
        	logThrowException("AxisConfigException - SERVER_CONFIG_TRANSPORT_CONF_FAILED")

            throw AxisConfigException (SERVER_CONFIG_TRANSPORT_CONF_FAILED);
        }
        string sSessionId = pStream->getSessionId ();
    
        // After this point we should return AXIS_SUCCESS. Otherwise the transport 
        // layer may not send the response back (either soap fault or result).
        do
        {
            /* populate MessageData with transport information */
            m_pMsgData->m_Protocol = pStream->getProtocol ();
    
            if (AXIS_SUCCESS != m_pDZ->setInputStream (pStream))
            {
                nSoapVersion = m_pDZ->getVersion ();
                nSoapVersion = (nSoapVersion == VERSION_LAST) ? SOAP_VER_1_2 : nSoapVersion;
                m_pSZ->setSoapVersion ((SOAP_VERSION) nSoapVersion);
                
            	logThrowException("AxisSoapException - CLIENT_SOAP_SOAP_CONTENT_ERROR")

                throw AxisSoapException (CLIENT_SOAP_SOAP_CONTENT_ERROR);
            }
    
            const char *cService = pStream->getTransportProperty (SERVICE_URI);
            if (!cService)
            {
            	logThrowException("AxisSoapException - CLIENT_SOAP_SOAP_CONTENT_ERROR")

                throw AxisSoapException (CLIENT_SOAP_SOAP_CONTENT_ERROR);
            }
            AxisString service = (cService == NULL) ? "" : cService;
    
            if (service.empty ())
            {
                nSoapVersion = m_pMsgData->m_pDZ->getVersion ();
                nSoapVersion = (nSoapVersion == VERSION_LAST) ? SOAP_VER_1_2 : nSoapVersion;
                m_pSZ->setSoapVersion ((SOAP_VERSION) nSoapVersion);
                
            	logThrowException("AxisSoapException - CLIENT_SOAP_SOAP_ACTION_EMTPY")

                throw AxisSoapException (CLIENT_SOAP_SOAP_ACTION_EMTPY);
            }
            
            // if there are quotes remove them. 
            if (service.find ('\"') != string::npos)
            {
                service = service.substr (1, service.length () - 2);
            }
    
            // get service description object from the WSDD Deployment object 
            m_pService = g_pWSDDDeployment->getService (service.c_str ());
            if (!m_pService)
            {
                nSoapVersion = m_pMsgData->m_pDZ->getVersion ();
                nSoapVersion = (nSoapVersion == VERSION_LAST) ? SOAP_VER_1_2 : nSoapVersion;
                m_pSZ->setSoapVersion ((SOAP_VERSION) nSoapVersion);
                
            	logThrowExceptionWithData("AxisWsddException - CLIENT_WSDD_SERVICE_NOT_FOUND", service.c_str())

                throw AxisWsddException (CLIENT_WSDD_SERVICE_NOT_FOUND, service.c_str());
            }
    
            m_CurrentProviderType = m_pService->getProvider ();
            m_pSZ->setCurrentProviderType (m_CurrentProviderType);
            m_pDZ->setCurrentProviderType (m_CurrentProviderType);
            switch (m_CurrentProviderType)
            {
                case C_RPC_PROVIDER:
                case CPP_RPC_PROVIDER:
                    m_pSZ->setStyle (RPC_ENCODED);
                    m_pDZ->setStyle (RPC_ENCODED);
                    break;
                case C_DOC_PROVIDER:
                case CPP_DOC_PROVIDER:
                    m_pSZ->setStyle (DOC_LITERAL);
                    m_pDZ->setStyle (DOC_LITERAL);
                    break;
                case COM_PROVIDER:
                // TODO: ??
                break;
                default:;
                // TODO: ??
            }
    
            /* Check for stream version in the request and decide whether we support
             * it or not. If we do not support send a soapfault with version 
             * mismatch. 
             */
            nSoapVersion = m_pDZ->getVersion ();
            if (m_pDZ->getStatus () != AXIS_SUCCESS)
            {
            	logThrowException("AxisSoapException - CLIENT_SOAP_MESSAGE_INCOMPLETE")

                throw AxisSoapException (CLIENT_SOAP_MESSAGE_INCOMPLETE);
            }
    
            if (nSoapVersion == VERSION_LAST)    // version not supported 
            {
                m_pSZ->setSoapVersion (SOAP_VER_1_2);
                
            	logThrowException("AxisSoapException - SOAP_VERSION_MISMATCH")

                throw AxisSoapException (SOAP_VERSION_MISMATCH);
            }
    
            /* Set Soap version in the Serializer and the envelope */
            if (AXIS_SUCCESS != m_pSZ->setSoapVersion ((SOAP_VERSION) nSoapVersion))
            {
            	logThrowException("AxisSoapException - CLIENT_SOAP_SOAP_CONTENT_ERROR")

                throw AxisSoapException (CLIENT_SOAP_SOAP_CONTENT_ERROR);
            }
    
            /* Get the operation name from transport information Ex: from 
             * SOAPAction header 
             */
            if (AXIS_SUCCESS != m_pDZ->getHeader ())
            {
            	logThrowException("AxisSoapException - CLIENT_SOAP_SOAP_CONTENT_ERROR")

                throw AxisSoapException (CLIENT_SOAP_SOAP_CONTENT_ERROR);
            }
            if (AXIS_SUCCESS != m_pDZ->getBody ())
            {
            	logThrowException("AxisSoapException - CLIENT_SOAP_SOAP_CONTENT_ERROR")

                throw AxisSoapException (CLIENT_SOAP_SOAP_CONTENT_ERROR);
            }
    
            AxisString sOperation = m_pDZ->getMethodNameToInvoke ();
    
            if (sOperation.empty ())
            {
            	logThrowException("AxisSoapException - CLIENT_SOAP_NO_SOAP_METHOD")

                throw AxisSoapException (CLIENT_SOAP_NO_SOAP_METHOD);
            }
            /* remove any quotes in the operation name */
            if (sOperation.rfind ('\"') != string::npos)
            {
                sOperation = sOperation.substr (0, sOperation.length () - 1);
            }
    
            AxisString operationToInvoke = sOperation;
    
            m_pMsgData->setOperationName (operationToInvoke.c_str ());
    
            if (m_pService->isAllowedMethod (operationToInvoke.c_str ()))
            {
                /* load actual web service handler */
                if (AXIS_SUCCESS != g_pHandlerPool->getWebService (&m_pWebService, sSessionId, m_pService))
                {
                	logThrowException("AxisEngineException - SERVER_ENGINE_COULD_NOT_LOAD_SRV")

                    throw AxisEngineException(SERVER_ENGINE_COULD_NOT_LOAD_SRV);
                }
        
                /* Check whether the provider type in the wsdd matchs the service's 
                 * binding style 
                 */
                AXIS_BINDING_STYLE nBindingStyle = RPC_ENCODED;
                if (0 != m_pWebService->_functions)
                    /* C service */
                {
                    nBindingStyle = m_pWebService->_functions->getBindingStyle (m_pWebService->_object);
                }
                else if (0 != m_pWebService->_object)
                {
                    nBindingStyle = ((WrapperClassHandler *)m_pWebService->_object)->getBindingStyle ();
                }
        
                if (m_pSZ->getStyle () != nBindingStyle)
                {
                	logThrowException("AxisSoapException - CLIENT_SOAP_SOAP_CONTENT_ERROR")

                    throw AxisSoapException (CLIENT_SOAP_SOAP_CONTENT_ERROR);
                }
            }
            else
            {
            	logThrowException("AxisWsddException - CLIENT_WSDD_METHOD_NOT_ALLOWED")

                throw AxisWsddException (CLIENT_WSDD_METHOD_NOT_ALLOWED);
            }
    
            // Get Global and Transport Handlers
            if (AXIS_SUCCESS != (Status = initializeHandlers (sSessionId,pStream->getProtocol())))
            {
            	logThrowException("AxisEngineException - SERVER_ENGINE_COULD_NOT_LOAD_HDL")

                throw AxisEngineException (SERVER_ENGINE_COULD_NOT_LOAD_HDL);
            }
            
            // Get Service specific Handlers from the pool if configured any
            if (AXIS_SUCCESS != (Status = g_pHandlerPool->getRequestFlowHandlerChain (&m_pSReqFChain,sSessionId,m_pService)))
            {
            	logThrowException("AxisEngineException - SERVER_ENGINE_COULD_NOT_LOAD_HDL")

                throw AxisEngineException (SERVER_ENGINE_COULD_NOT_LOAD_HDL);
            }
    
            if (AXIS_SUCCESS != (Status = g_pHandlerPool->getResponseFlowHandlerChain (&m_pSResFChain,sSessionId,m_pService)))
            {
            	logThrowException("AxisEngineException - SERVER_ENGINE_COULD_NOT_LOAD_HDL")

                throw AxisEngineException (SERVER_ENGINE_COULD_NOT_LOAD_HDL);
            }
    
            // Invoke all handlers 
            Status = invoke (m_pMsgData);
        }
        while (0);
    
        if (AXIS_SUCCESS != m_pDZ->flushInputStream ())
        {
        }
        
        // Get any header blocks unprocessed (left) in the Deserializer and add them
        // to the Serializer. They may be headers targetted to next soap processors.
        HeaderBlock *pHderBlk = NULL;
        while (true)
        {
            pHderBlk = m_pDZ->getHeaderBlock ();
            if (pHderBlk)
                m_pSZ->addHeaderBlock (pHderBlk);
            else
                break;
        }
        m_pSZ->setOutputStream (pStream);
    
        // Pool back the handlers and services
        releaseHandlers (pStream);
        
        //todo
        /* An exception derived from exception which is not handled will be 
         * handled here. You can call a method in AxisModule which may unload 
         * the ServerAxisEngine from the webserver and report the error. You can
         * also write this in a logfile specific to axis.
         */
    }
    catch (AxisException & e)
    {
        // An exception which is not handled will be handled here.
        int iExceptionCode = e.getExceptionCode ();
        if (AXISC_SERVICE_THROWN_EXCEPTION == iExceptionCode)
        {
            // Writes the SoapMessage in which soap body contains only Fault 
            m_pSZ->setOutputStream (pStream);
            releaseHandlers (pStream);
            return AXIS_SUCCESS;    
        }
        else
        {        
           /* An exception has occurred inside Axis C++ engine.(not in a
            * webservice or handler). Later we handle this by coming back in here
            * and serialising the fault out
            */
            throw e;
        }
    }
    catch (exception& e)
    {
        e = e;
        logExitWithReturnCode(SERVER_UNKNOWN_ERROR)

        return SERVER_UNKNOWN_ERROR;
    }
    catch (...)
    {
        logExitWithReturnCode(SERVER_UNKNOWN_ERROR)

        return SERVER_UNKNOWN_ERROR;
    }
    
    logExitWithReturnCode(AXIS_SUCCESS)
    
    return AXIS_SUCCESS;
}

int ServerAxisEngine::
invoke (MessageData * pMsg)
{
	logEntryEngine("ServerAxisEngine::invoke")
	
    enum AE_LEVEL { AE_START = 1, AE_TRH, AE_GLH, AE_SERH, AE_SERV };
    int Status = AXIS_FAIL;
    int level = AE_START;
    do
    {
        // Invoke transport request handlers
        if (m_pTReqFChain)
            if (AXIS_SUCCESS != (Status = m_pTReqFChain->invoke (pMsg)))
            {
            	logThrowException("AxisEngineException - SERVER_ENGINE_HANDLER_FAILED")

                throw AxisEngineException (SERVER_ENGINE_HANDLER_FAILED);
            }
        
        level++;        // AE_TRH
        // Invoke global request handlers
        if (m_pGReqFChain)
            if (AXIS_SUCCESS != (Status = m_pGReqFChain->invoke (pMsg)))
            {
            	logThrowException("AxisEngineException - SERVER_ENGINE_HANDLER_FAILED")

                throw AxisEngineException (SERVER_ENGINE_HANDLER_FAILED);
            }

        level++;        // AE_GLH
        // Invoke service specific request handlers
        if (m_pSReqFChain)
            if (AXIS_SUCCESS != (Status = m_pSReqFChain->invoke (pMsg)))
            {
            	logThrowException("AxisEngineException - SERVER_ENGINE_HANDLER_FAILED")

                throw AxisEngineException (SERVER_ENGINE_HANDLER_FAILED);
            }
        
        level++;        // AE_SERH
        
        /*
         * Before processing the soap body check whether there is any header 
         * blocks with mustUnderstand attribute left unprocessed in the 
         * Deserializer. If so return a soap fault.
         */
        if (m_pDZ->isAnyMustUnderstandHeadersLeft ())
        {
        	logThrowException("AxisSoapException - SOAP_MUST_UNDERSTAND")

            throw AxisSoapException (SOAP_MUST_UNDERSTAND);
        }
        
        // Call actual web service handler
        if (m_pWebService)
        {
            if (0 != m_pWebService->_functions)
            /* C web service */
            {
                // Disable C support
                //IMessageData_C cMC = { 0, 0 };
                //cMC._object = pMsg;
                //cMC._functions = &IMessageData::ms_VFtable;
                //Status = m_pWebService->_functions->invoke (m_pWebService->
                //    _object, &cMC);
            }
            else if (0 != m_pWebService->_object)
                Status = ((WrapperClassHandler *) m_pWebService->_object)->invoke (pMsg);
            else
                Status = AXIS_FAIL;
            
            if (AXIS_SUCCESS != Status)
            {
            	logThrowException("AxisEngineException - SERVER_ENGINE_WEBSERVICE_FAILED")

                throw AxisEngineException (SERVER_ENGINE_WEBSERVICE_FAILED);
            }
        }
        level++;        // AE_SERV
    }
    while (0);

    pMsg->setPastPivotState (true);

    /*
     * The case clauses in this switch statement have no breaks.
     * Hence, if Everything up to web service invocation was successful
     * then all response handlers are invoked. If there was a failure
     * at some point the response handlers from that point onwards
     * are invoked.
     */
    switch (level)
    {
        case AE_SERV:        // Everything Success.        
            Status = AXIS_SUCCESS;
            // no break;
        case AE_SERH:        // Actual web service handler has failed
            // Invoke web service specific response handlers
            if (m_pSResFChain)
                m_pSResFChain->invoke (pMsg);
            // no break;
        case AE_GLH:        // web service specific handlers have failed
            // invoke global response handlers
            if (m_pGResFChain)
                m_pGResFChain->invoke (pMsg);
            // no break;
        case AE_TRH:        // Global handlers have failed
            if (m_pTResFChain)
                m_pTResFChain->invoke (pMsg);
            // no break;
        case AE_START:;        // Transport handlers have failed
    };
    
	logExitWithReturnCode(Status)

    return Status;
}

void ServerAxisEngine::
onFault (MessageData * pMsg)
{
	logEntryEngine("ServerAxisEngine::onFault")
	
	logExit()
}

int ServerAxisEngine::
setFaultOutputStream (int iFaultCode, SOAPTransport * pStream)
{
	logEntryEngine("ServerAxisEngine::setFaultOutputStream")
	
    string sMessage = AxisException::getMessageForExceptionCode(iFaultCode);
    SoapFault *pObjSoapFault = SoapFault::getSoapFault (iFaultCode);
    pObjSoapFault->setFaultDetail (sMessage.c_str ());
    m_pSZ->setSoapFault (pObjSoapFault);
    m_pSZ->setOutputStream (pStream);
    releaseHandlers (pStream);
    
	logExitWithReturnCode(AXIS_SUCCESS)

    return AXIS_SUCCESS;
}

int ServerAxisEngine::
setFaultOutputStream (AxisException iFault, SOAPTransport * pStream)
{
	logEntryEngine("ServerAxisEngine::setFaultOutputStream")
	
    SoapFault *pObjSoapFault = SoapFault::getSoapFault (iFault.getExceptionCode());
    pObjSoapFault->setFaultDetail (iFault.what());
    m_pSZ->setSoapFault (pObjSoapFault);
    m_pSZ->setOutputStream (pStream);
    releaseHandlers (pStream);
    
	logExitWithReturnCode(AXIS_SUCCESS)

    return AXIS_SUCCESS;
}

int ServerAxisEngine::
releaseHandlers (SOAPTransport * pStream)
{
	logEntryEngine("ServerAxisEngine::releaseHandlers")
	
    string sSessionId = pStream->getSessionId ();
    // Pool back the Service specific handlers
    if (m_pSReqFChain)
        g_pHandlerPool->poolHandlerChain (m_pSReqFChain, sSessionId);
        
    if (m_pSResFChain)
        g_pHandlerPool->poolHandlerChain (m_pSResFChain, sSessionId);
        
    /* Pool back the Global and Transport handlers
     * UnInitializeHandlers(sSessionId, stream->trtype);
     * Pool back the webservice
     */
    if (m_pWebService)
        g_pHandlerPool->poolWebService (sSessionId, m_pWebService, m_pService);

	logExitWithReturnCode(AXIS_SUCCESS)

    return AXIS_SUCCESS;
}

AXIS_CPP_NAMESPACE_END
