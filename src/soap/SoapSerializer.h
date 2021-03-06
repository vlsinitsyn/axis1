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

#if !defined(_SOAPSERIALIZER_H____OF_AXIS_INCLUDED_)
#define _SOAPSERIALIZER_H____OF_AXIS_INCLUDED_

#include <axis/IHandlerSoapSerializer.hpp>
#include "../transport/SOAPTransport.h"

#include "../common/BasicTypeSerializer.h"
#include <axis/WSDDDefines.hpp>
#include <axis/IAttribute.hpp>
#include "SoapAttachment.hpp"
#include <stdarg.h>
#include <cctype>



AXIS_CPP_NAMESPACE_START

class SoapEnvelope;
class SoapHeader;
class SoapMethod;
class SoapBody;
class SoapFault;
class HeaderBlock;
class ArrayBean;
class Attribute;
class ISoapAttachment;
class SoapAttachmentReference;

/**
 *  @class SoapSerializer
 *  @brief interface for the SoapSerializer class.
 */

class SoapSerializer : public IHandlerSoapSerializer
{
private:
    map<AxisString, ISoapAttachment*> m_SoapAttachments;
    list<SoapAttachmentReference*> m_attachmentRefs;
    int m_nCounter;
    AxisChar m_Buf[BTS_BUFFSIZE];
    SoapEnvelope* m_pSoapEnvelope;        
    int m_iSoapVersion;
    AXIS_BINDING_STYLE m_nStyle;
    /* Overall status of Serializer. If anything goes wrong this is not AXIS_SUCCESS */
    int m_nStatus;
    /* Map that contains pairs of currently available namespaces and prefixes */ 
    map<AxisXMLString, AxisXMLString> m_NsStack;
    /* Provider type of current service that uses this Serializer object */
    PROVIDERTYPE m_ProviderType;
    /* the local namespace for this serializer */
    AxisChar* m_pNamespace;
    ContentIdSet *m_pContentIdSet;

    /**
     * Following is for C-binding support. The stub that the serializer object belongs to.
     */
    void * m_pStub;


public:
    /**
     * Set C-binding stub pointer associated with object.
     * This function was added in support of the c-Binding implementation.
     *
     * @param pStub - pointer to C binding stub object.
     */
    void setCStub(void *pStub) { m_pStub = pStub; }

    /**
     * Get C-binding stub pointer.
     * This function was added in support of the c-Binding implementation.
     *
     * @return Pointer to C binding stub object.
     */
    void *getCStub() { return m_pStub; }

    inline const AxisChar* AXISCALL getNamespace() const {return m_pNamespace;}; 
    inline void AXISCALL setNamespace(const AxisChar* pNamespace) {m_pNamespace = (const_cast<AxisChar*>(pNamespace));}; 

#ifdef UNIT_TESTING_ON
    int setOutputStreamForTesting(SOAPTransport* pStream);
#endif
    int AXISCALL createSoapMethod(const AxisChar* sLocalName, const AxisChar* sURI);
    int AXISCALL createSoapMethod(const AxisChar* sLocalName, const AxisChar* sURI, bool bIsWrapperStyle);

    int AXISCALL createSoapFault(const AxisChar* sLocalName, 
        const AxisChar* sURI, const AxisChar* sFaultCode,
    const AxisChar* sFaultString);

    IWrapperSoapSerializer& operator<<(const AxisChar* cSerialized);
    /**
     * Returns the corrosponding namespace prefix.
     * @param pNamespace The namespace.
     * @return The corrosponding prefix.
     */
    const AxisChar* AXISCALL getNamespacePrefix(const AxisChar* pNamespace);
    /**
     * Returns the corrosponding namespace prefix. This method is 
     * called when the caller also wants to know whether this is a new 
     * namespace or not as appose to its overloaded other member.
     * @param pNamespace The namespace.
     * @param blnIsNewPrefix The boolean which behaves as 
     * a out parameter to indicate whether this is a new namespace or not.
     * @return The corrosponding prefix.
     */
    const AxisChar* AXISCALL getNamespacePrefix(const AxisChar* pNamespace,  bool& blnIsNewPrefix);
    void AXISCALL removeNamespacePrefix(const AxisChar* pNamespace);
    int setSoapVersion(SOAP_VERSION);
    int init();
    int setOutputStream(SOAPTransport* pStream);
    void markEndOfStream();
    int setSoapMethod(SoapMethod* pSoapMethod);
    int setSoapFault(SoapFault* pSoapFault);
    int setSoapBody(SoapBody* pSoapBody);
    int setSoapEnvelope(SoapEnvelope* pSoapEnvelope);
    SoapSerializer();
    virtual ~SoapSerializer();

    /* for arrays of basic types */
    int AXISCALL addOutputBasicArrayParam(const Axis_Array* pArray, 
        XSDTYPE nType, const AxisChar* pName);

    /* for arrays of complex types */
    int AXISCALL addOutputCmplxArrayParam(const Axis_Array* pArray, 
        void* pSZFunct, void* pDelFunct, 
        const AxisChar* pName, const AxisChar* pNamespace);

    /* for complex types */
    int AXISCALL addOutputCmplxParam(void* pObject, void* pSZFunct, 
        void* pDelFunct, const AxisChar* pName, const AxisChar* pNamespace);

    int AXISCALL addFaultDetail(void* pObject, void* pSZFunct,
        void* pDelFunct, const AxisChar* pName, const AxisChar* pNamespace );

    int AXISCALL addFaultDetail( const AxisChar * pDetail);

    int AXISCALL serializeCmplxArray(const Axis_Array* pArray, void* pSZFunct,
        void* pDelFunct, const AxisChar* pName, 
        const AxisChar* pNamespace);

    int AXISCALL serializeBasicArray(const Axis_Array* pArray, XSDTYPE nType, 
        const AxisChar* pName);
    
    int AXISCALL serializeBasicArray(const Axis_Array* pArray, const AxisChar* pNamespace,
        XSDTYPE nType, const AxisChar* pName);


    /* following two functions are needed by serializer functions of complex 
     * types for RPC style web services 
     */
    void AXISCALL serializeStartElementOfType(const AxisChar* pName, 
        const AxisChar* pNamespace, const AxisChar* pPrefix);
    void AXISCALL serializeEndElementOfType(const AxisChar* pName);
    PROVIDERTYPE getCurrentProviderType() { return m_ProviderType;};
    void setCurrentProviderType(PROVIDERTYPE nType) { m_ProviderType = nType;};

private:
    ArrayBean* makeArrayBean(XSDTYPE nType, void** pArray);
    ArrayBean* makeArrayBean(void** pObject, void* pSZFunct, void* pDelFunct);
    int setSoapHeader(SoapHeader* pSoapHeader);

public: /* Basic Type Serializing methods */
    int removeSoapHeader();
    int setHeaderBlock(HeaderBlock* pHeaderBlock);
    IHeaderBlock* createHeaderBlock();
    
private:
    bool checkAttachmentAvailability();
    void serializeAttachments(SoapSerializer &pSZ);
    BasicTypeSerializer m_BTSZ;
    SOAPTransport* m_pOutputStream;

public:
    IHeaderBlock* getCurrentHeaderBlock();
    ISoapAttachment* createSoapAttachment();
    void addNamespaceToNamespaceList(const AxisChar *pachNamespaceURI, const AxisChar* pachPrefix);
    void addNamespaceToEnvelope(AxisChar *pachNamespaceURI, AxisChar* pachPrefix);
    void addAttachmentBody(const AxisChar* achId, xsd__base64Binary* pAttchBody);
    void addAttachmentHeader(const AxisChar* achId, const AxisChar* achHeaderName, const AxisChar* achHeaderValue);
    void addAttachment(const AxisChar* achId, ISoapAttachment* pAttach);
    void addAttachments(ISoapAttachment** pAttach, int iAttchArraySize);
    void setContentIdSet(ContentIdSet *pContentIdSet);
    void addAttachmentParameter(ISoapAttachment* att, const char* pName, IAttribute **attributes, int nAttributes);
    IHeaderBlock* getHeaderBlock(const AxisChar* pcName, const AxisChar* pcNamespace);
    IHeaderBlock* getFirstHeaderBlock();
    IHeaderBlock* getNextHeaderBlock();

    int setSOAPMethodAttribute(Attribute* pAttribute);
    void clearSOAPMethodAttributes();
    SoapMethod* getSOAPMethod();
    IHeaderBlock* createHeaderBlock(const AxisChar *pachLocalName,  const AxisChar *pachUri);
    int AXISCALL addHeaderBlock(IHeaderBlock* pBlk);
    int AXISCALL addOutputParam(const AxisChar* pchName, void* pValue, XSDTYPE type);
    int AXISCALL serializeAsElement(const AxisChar* pchName, 
                                    void* pValue,
                                    XSDTYPE type);
    int AXISCALL serializeAsElement(const AxisChar* pchName,
                                    const AxisChar* pNamespace,
                                    void* pValue,
                                    XSDTYPE type);
    int AXISCALL serializeAsElement(const AxisChar* pchName,
                                    IAnySimpleType* pSimpleType);
    int AXISCALL serializeAsElement(const AxisChar* pchName,
                                    const AxisChar* pNamespace,
                                    IAnySimpleType* pSimpleType);
    int AXISCALL serializeAsAttribute(const AxisChar* pName,
                                      const AxisChar* pNamespace,
                                      void* pValue,
                                      XSDTYPE type);
    int AXISCALL serializeAsAttribute(const AxisChar* pName,
                                      const AxisChar* pNamespace,
                                      IAnySimpleType* pSimpleType);
    void AXISCALL serializeVargs(const char * pFirst, va_list vList);
    void AXISCALL serialize(const char* pFirst, ...);
    void setStyle(AXIS_BINDING_STYLE nStyle) { m_nStyle = nStyle; m_BTSZ.setStyle(nStyle);};
    AXIS_BINDING_STYLE getStyle(){return m_nStyle;};
    int AXISCALL setBodyAsHexBinary(xsd__hexBinary body);
    int AXISCALL setBodyAsBase64Binary(xsd__base64Binary body);
    const AxisChar* AXISCALL getBodyAsString();
    int addOutputAnyObject(AnyType* pAnyObject);
    int serializeAnyObject(AnyType* pAnyObject);
    int serializeAsChardata(void* pValue, XSDTYPE type);
    AxisXMLString getNamespaceURL( string sNameSpace);
    int deleteHeaderBlock(const AxisChar* pName, const AxisChar* pNamespace);    

  /**
    * This method will clear up all the current out parameters in preparation of a
    * new method call
    */
    void reset();

    /**
     * Add namespace to SOAP header. 
     */
    void addNamespaceToSOAPHeader(const AxisChar *pachNamespaceURI, const AxisChar* pachPrefix);

    /**
     * Clear SOAP header namespaces.  
     */
    void clearSOAPHeaderNamespaces();

    /**
     * Add attribute to SOAP header.  
     */
    void addAttributeToSOAPHeader(const AxisChar *pLocalname, const AxisChar *pPrefix, const AxisChar *pValue);

    /**
     * Clear SOAP header attributes.  
     */
    void clearSOAPHeaderAttributes();

    /**
     * Add namespace to SOAP body.  
     */
    void addNamespaceToSOAPBody(const AxisChar *pachNamespaceURI, const AxisChar* pachPrefix);

    /**
     * Clear SOAP body namespaces.  
     */
    void clearSOAPBodyNamespaces();

    /**
     * Add attribute to SOAP body.  
     */
    void addAttributeToSOAPBody(const AxisChar *pLocalname, const AxisChar *pPrefix, const AxisChar *pValue);

    /**
     * Clear SOAP body attributes.  
     */
    void clearSOAPBodyAttributes();

};

AXIS_CPP_NAMESPACE_END

#endif 


