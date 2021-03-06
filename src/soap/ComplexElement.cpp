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
#include "../platforms/PlatformAutoSense.hpp"

#include <axis/GDefine.hpp>

#include "Attribute.h"
#include "ComplexElement.h"
#include "SoapSerializer.h"

#include "../common/AxisTrace.h"

AXIS_CPP_NAMESPACE_START

ComplexElement::
ComplexElement()
:m_pParent(NULL)
{
    m_pachPrefix = NULL;
    m_pachLocalName = NULL;
    m_pachURI = NULL;

    m_iNodeType= ELEMENT_NODE;
    iNoOfChildren = 0;
}

ComplexElement::
ComplexElement(AxisChar *pachLocalName, AxisChar *pachPrefix, AxisChar *pachUri)
: m_pParent(NULL)
{
    m_pachLocalName = new AxisChar[strlen(pachLocalName)+1];
    strcpy(m_pachLocalName, pachLocalName);
    m_pachPrefix = new AxisChar[strlen(pachPrefix)+1];
    strcpy(m_pachPrefix, pachPrefix);
    m_pachURI = new AxisChar[strlen(pachUri)+1];
    strcpy(m_pachURI, pachUri);

    m_iNodeType= ELEMENT_NODE;
    iNoOfChildren = 0;
}

ComplexElement::
ComplexElement(const ComplexElement& rCopy)
:BasicNode(rCopy), m_pachPrefix(NULL), m_pachLocalName(NULL), m_pachURI(NULL), m_pParent(NULL)
{
    this->iNoOfChildren = rCopy.iNoOfChildren;
    
    list<BasicNode*>::const_iterator itCurrBasicNode= rCopy.m_children.begin();

    while(itCurrBasicNode != rCopy.m_children.end())
    {
        this->m_children.push_back((*itCurrBasicNode)->clone());    
        itCurrBasicNode++;        
    } 
    
    if(rCopy.m_pachPrefix)
    {
        this->m_pachPrefix = new char[strlen(rCopy.m_pachPrefix)+1];
        strcpy(this->m_pachPrefix,rCopy.m_pachPrefix);
    }

    if(rCopy.m_pachLocalName)
    {
        this->m_pachLocalName = new char[strlen(rCopy.m_pachLocalName)+1];
        strcpy(this->m_pachLocalName,rCopy.m_pachLocalName);
    }

    if(rCopy.m_pachURI)
    {
        this->m_pachURI = new char[strlen(rCopy.m_pachURI)+1];
        strcpy(this->m_pachURI,rCopy.m_pachURI);
    }

    this->m_iNodeType= rCopy.m_iNodeType;
    this->iNoOfChildren = rCopy.iNoOfChildren;
}

BasicNode* ComplexElement::
clone()
{
    return new ComplexElement(*this);
}


ComplexElement::
~ComplexElement()
{
    delete [] m_pachPrefix;
    m_pachPrefix = NULL;

    delete [] m_pachLocalName;
    m_pachLocalName = NULL;

    delete [] m_pachURI;
    m_pachURI = NULL;

    m_pParent = NULL;

    /* Clear the Attributes */
    list<Attribute*>::iterator itCurrAttribute= m_attributes.begin();
    while(itCurrAttribute != m_attributes.end())
    {
        delete (*itCurrAttribute);
        itCurrAttribute++;
    }
    m_attributes.clear();

    /* Clear the children */
    list<BasicNode*>::iterator itCurrChild= m_children.begin();
    while(itCurrChild != m_children.end())
    {        
        delete (*itCurrChild);
        itCurrChild++;        
    }
    m_children.clear();
}

IAttribute* ComplexElement::
createAttribute(const AxisChar *localname,  const AxisChar *prefix, const AxisChar *value)
{
    Attribute* pAttribute = new Attribute(m_attributes, localname, prefix, value);
    
    m_attributes.push_back(pAttribute);
    return (IAttribute*)pAttribute;
}

IAttribute* ComplexElement::
createAttribute(const AxisChar *localname, const AxisChar *prefix, const AxisChar *uri, const AxisChar *value)
{
    Attribute* pAttribute = new Attribute(m_attributes, localname, prefix, uri, value);
    m_attributes.push_back(pAttribute);
   
    return (IAttribute*)pAttribute;
}

IAttribute* ComplexElement::
createAttribute(const AxisChar *localname, const AxisChar *value)
{
    Attribute* pAttribute = new Attribute(m_attributes, localname, "", value);
    m_attributes.push_back(pAttribute);
                                                                                                                                                                            
    return (IAttribute*)pAttribute;
}


int ComplexElement::
setPrefix(const AxisChar* pachPrefix)
{
    if (NULL==pachPrefix)
        return AXIS_FAIL;

    if (NULL != m_pachPrefix) 
        delete [] m_pachPrefix;
    m_pachPrefix = new AxisChar[strlen(pachPrefix)+1];
    strcpy(m_pachPrefix, pachPrefix);
    return AXIS_SUCCESS;
}

int ComplexElement::
setLocalName(const AxisChar* pachLocalName)
{
    if (NULL==pachLocalName)
        return AXIS_FAIL;

    if (NULL != m_pachLocalName) 
        delete [] m_pachLocalName;
    m_pachLocalName = new AxisChar[strlen(pachLocalName)+1];
    strcpy(m_pachLocalName, pachLocalName);
    return AXIS_SUCCESS;
}

int ComplexElement::
addChild(BasicNode *pBasicNode)
{
    if (pBasicNode)
    {
        if (pBasicNode->getNodeType() == ELEMENT_NODE)
        {
            ComplexElement *pComplexElement = static_cast<ComplexElement*>(pBasicNode);
            pComplexElement->setParent(this);
        }
        m_children.push_back(pBasicNode);
        iNoOfChildren++;
        return AXIS_SUCCESS;
    }
    else
    {
        return AXIS_FAIL;
    }
}

int ComplexElement::
serialize(SoapSerializer& pSZ)
{
    int iStatus= AXIS_SUCCESS;
    do
    {
        if(isSerializable()) 
        {    
            pSZ.serialize("<", NULL);    
            if((NULL!=m_pachPrefix) && (strlen(m_pachPrefix)!=0))
            {                
                pSZ.serialize(m_pachPrefix, ":", NULL);
            }
            pSZ.serialize(m_pachLocalName, NULL);
            if ((NULL!=m_pachPrefix) && (strlen(m_pachPrefix)!=0) && 
                (NULL!=m_pachURI) && (strlen(m_pachURI)!=0))
            {
                pSZ.serialize(" xmlns:", m_pachPrefix, "=", PLATFORM_DOUBLE_QUOTE_S, m_pachURI, PLATFORM_DOUBLE_QUOTE_S, NULL);
            }
            pSZ.serialize(">", NULL);
            iStatus= serializeChildren(pSZ);
            if(iStatus==AXIS_FAIL)
            {
                break;
            }
            pSZ.serialize("</", NULL);
            if((NULL!=m_pachPrefix) && (strlen(m_pachPrefix)!=0))
            {
                pSZ.serialize(m_pachPrefix, ":", NULL);
            }
            pSZ.serialize(m_pachLocalName, ">", NULL);
            iStatus= AXIS_SUCCESS;
        } 
        else
        {
            iStatus= AXIS_FAIL;
        }
    } while(0);    
    return iStatus;
}

int ComplexElement::
serialize(SoapSerializer& pSZ, list<AxisChar*>& lstTmpNameSpaceStack)
{
    int iStatus= AXIS_SUCCESS;
    do
    {
        if(isSerializable()) 
        {    
            bool blnIsNewNamespace = false;

            // Check if we have no namespace prefix, but do have a namespace URI
            if ( (m_pachPrefix == NULL || strlen(m_pachPrefix) == 0)
                && (m_pachURI != NULL && strlen(m_pachURI) != 0))
            {
                // Is this a child node?
                if (m_pParent != NULL)
                {
                    // Does the parent uses the same namespace URI?
                    if ( m_pParent->getURI() != NULL 
                        && strlen(m_pParent->getURI()) != 0
                        && strcmp(m_pachURI, m_pParent->getURI()) == 0)
                    {
                        // Does the parent have a namespace prefix?
                        if (m_pParent->getPrefix() != NULL && strlen(m_pParent->getPrefix()) != 0)
                        {
                            setPrefix(m_pParent->getPrefix());
                            setURI("");
                        }
                    }
                }
            }
            
            pSZ.serialize("<", NULL);    
            if( (m_pachPrefix != NULL) && (strlen(m_pachPrefix) != 0))
            {                
                pSZ.serialize(m_pachPrefix, ":", NULL);
                blnIsNewNamespace = true;
            }
            else if ( (m_pachURI != NULL) && (strlen(m_pachURI) != 0))
            {
                const AxisChar* pachTmp = pSZ.getNamespacePrefix(m_pachURI,
                    blnIsNewNamespace);

                if (NULL!=m_pachPrefix)
                    delete [] m_pachPrefix;
                m_pachPrefix = new AxisChar[strlen(pachTmp)+1];
                strcpy(m_pachPrefix , pachTmp);

                if (blnIsNewNamespace)
                    lstTmpNameSpaceStack.push_back(m_pachURI);

                pSZ.serialize(m_pachPrefix, ":", NULL);
            }

            pSZ.serialize(m_pachLocalName, NULL);

            if(((m_pachPrefix != NULL) && (strlen(m_pachPrefix) != 0)) && 
                    ( (m_pachURI != NULL) && (strlen(m_pachURI) != 0)) && 
                    (blnIsNewNamespace))
            {
                pSZ.serialize(" xmlns:", m_pachPrefix, "=", PLATFORM_DOUBLE_QUOTE_S, m_pachURI, PLATFORM_DOUBLE_QUOTE_S, NULL);
            }

            iStatus= attrSerialize(pSZ, lstTmpNameSpaceStack);
            if(iStatus==AXIS_FAIL)
               break;

            iStatus= serializeNamespaceDecl(pSZ);
            if(iStatus==AXIS_FAIL)
                break;

            pSZ.serialize(">", NULL);

            iStatus= serializeChildren(pSZ, lstTmpNameSpaceStack);
            if(iStatus==AXIS_FAIL)
                break;

            pSZ.serialize("</", NULL);
            if((m_pachPrefix!=NULL) && (strlen(m_pachPrefix) != 0))
                pSZ.serialize(m_pachPrefix, ":", NULL);

            pSZ.serialize(m_pachLocalName, ">", NULL);
            iStatus= AXIS_SUCCESS;
        } 
        else
            iStatus= AXIS_FAIL;
    } while(0);    

    return iStatus;
}

int ComplexElement::
attrSerialize(SoapSerializer& pSZ, list<AxisChar*>& lstTmpNameSpaceStack)
{
    int iStatus= AXIS_SUCCESS;
    
    list<Attribute*>::iterator itCurrAttribute= m_attributes.begin();

    while(itCurrAttribute != m_attributes.end())
    {
        iStatus= (*itCurrAttribute)->serialize(pSZ, lstTmpNameSpaceStack);
        if(iStatus==AXIS_FAIL)
            break;
        itCurrAttribute++;
    }

    return iStatus;
}

int ComplexElement::
serializeNamespaceDecl(SoapSerializer &pSZ)
{
    list<Attribute*>::iterator itCurrNamespaceDecl= m_namespaceDecls.begin();

    while(itCurrNamespaceDecl != m_namespaceDecls.end())
    {
        (*itCurrNamespaceDecl)->serialize(pSZ);
        itCurrNamespaceDecl++;
    }

    return AXIS_SUCCESS;
}

bool ComplexElement::
isSerializable()
{
    bool bStatus= true;

    do
    {
        if((NULL==m_pachLocalName) || (strlen(m_pachLocalName)==0))
        {
            bStatus= false;
            break;
        }
    }while(0);

    return bStatus;
}

int ComplexElement::
setURI(const AxisChar* pachURI)
{
    if (NULL!=m_pachURI)
        delete [] m_pachURI;
    m_pachURI = new AxisChar[strlen(pachURI)+1];
    strcpy(m_pachURI, pachURI);
    return AXIS_SUCCESS;
}

void ComplexElement::
setParent(ComplexElement *parent)
{
    m_pParent = parent;
}

int ComplexElement::
serializeChildren(SoapSerializer& pSZ)
{
    list<BasicNode*>::iterator itCurrBasicNode= m_children.begin();

    while(itCurrBasicNode != m_children.end())
    {
        (*itCurrBasicNode)->serialize(pSZ);
        
        itCurrBasicNode++;        
    }    

    return AXIS_SUCCESS;
}

int ComplexElement::
serializeChildren(SoapSerializer& pSZ, list<AxisChar*>& lstTmpNameSpaceStack)
{
    list<BasicNode*>::iterator itCurrBasicNode= m_children.begin();

    while(itCurrBasicNode != m_children.end())
    {
        if ((*itCurrBasicNode)->getNodeType() == ELEMENT_NODE)
            (*itCurrBasicNode)->serialize(pSZ, lstTmpNameSpaceStack);    
        else
        {
            /* for CHARACTER_NODE */
            (*itCurrBasicNode)->serialize(pSZ);
        }
        itCurrBasicNode++;        
    }    

    return AXIS_SUCCESS;
}

NODE_TYPE ComplexElement::
getNodeType() const
{
    return m_iNodeType;
}

BasicNode* ComplexElement::
getFirstChild()
{
    list<BasicNode*>::const_iterator itCurrChild= m_children.begin();

    if (itCurrChild != m_children.end())
        return (*itCurrChild);

    return NULL;
}

BasicNode* ComplexElement::
getLastChild()
{
    list<BasicNode*>::reverse_iterator ritCurrChild= m_children.rbegin();

    if (ritCurrChild != m_children.rend())
        return (*ritCurrChild);

    return NULL;
}

BasicNode* ComplexElement::
getChild(int iChildPosition)
{
    if (iChildPosition > iNoOfChildren)
        return NULL;

    list<BasicNode*>::const_iterator itCurrChild= m_children.begin();

    // The following is done since the previous line iterates one step forward
    iChildPosition--;

    // Takes the iterator to the requested position
    for (int i=0; i<iChildPosition; i++)
    {
        itCurrChild++;
    }

    if (itCurrChild != m_children.end())
        return *itCurrChild;

    return NULL;
}

int ComplexElement::
getNoOfChildren()
{
    return iNoOfChildren;
}

const AxisChar* ComplexElement::
getLocalName()
{
    return m_pachLocalName; 
}

const AxisChar* ComplexElement::
getURI()
{
    if ((m_pachURI == NULL || strlen(m_pachURI) == 0) && m_pParent != NULL)
        return m_pParent->getURI();

    return m_pachURI;
}

const AxisChar* ComplexElement::
getPrefix() {
    return m_pachPrefix;
}

IAttribute* ComplexElement::
getFirstAttribute()
{
    m_viCurrentAttribute = m_attributes.begin();
    if ( m_viCurrentAttribute == m_attributes.end())
        return NULL;
    else
        return ((IAttribute*)*m_viCurrentAttribute);                                                                                                                                                                           
}
                                                                                                                                                                            
IAttribute* ComplexElement::
getLastAttribute()
{
    list <Attribute*>::reverse_iterator iAttributeReverseIte = m_attributes.rbegin();
    if (iAttributeReverseIte == m_attributes.rend())
    {
         m_viCurrentAttribute = m_attributes.end();
         return NULL;
    }
    else
    {
        m_viCurrentAttribute = m_attributes.end();
        return ((IAttribute*)*iAttributeReverseIte);
    }
}
                                                                                                                                                                            
IAttribute* ComplexElement::
getNextAttribute()
{
    //already at the end?
    if (m_viCurrentAttribute == m_attributes.end())
        return NULL;
                                                                                                                                                                            
    m_viCurrentAttribute++;
    if ( m_viCurrentAttribute == m_attributes.end())
        return NULL;
    else
        return ((IAttribute*)*m_viCurrentAttribute);
}

IAttribute* ComplexElement::
getCurrentAttribute()
{
    //already at the end?
    if ( m_viCurrentAttribute == m_attributes.end())
        return NULL;
    else
        return ((IAttribute*)*m_viCurrentAttribute);
}

IAttribute* ComplexElement::
getAttribute(AxisChar* pachPrefix, AxisChar* pachURI, AxisChar* pachLocalname)
{
    // Create local enumeration for valid parameters.
    typedef enum
    {
        eParamHasURI = 1,
        eParamHasPrefix = 2,
        eParamHasLocalName = 4
    } EPARAMS_USED;

    // Keep a list of valid parameters.
    int    iValidParams = 0;

    // For each parameter, test its validity by checking that the pointer is
    // not null and then (and only then) that the string pointed to by the
    // non-null pointer is not empty (string length is greater than zero). Iff
    // the test remains valid, add the enumerated equivalent of the parameter
    // to the list of valid parameters.
    if( (pachURI != NULL) && (strlen( pachURI) > 0))
        iValidParams += eParamHasURI;

    if( (pachPrefix != NULL) && (strlen( pachPrefix) > 0))
        iValidParams += eParamHasPrefix;

    if( (pachLocalname != NULL) && (strlen( pachLocalname) > 0))
        iValidParams += eParamHasLocalName;

    // Get the pointer to the start of the attribute list.
    list<Attribute*>::iterator it_m_attributes = m_attributes.begin();

    // Depending on the list of valid parameters, choose which case is
    // appropriate and then perform the search using those valid parameters.
    // NB: Currently, the first match is returned in all cases.
    switch( iValidParams)
    {
        // Parameter list contains a valid URI
        case eParamHasURI:
        {
            while( it_m_attributes != m_attributes.end())
            {
                if( !strcmp( (*it_m_attributes)->getURI(), pachURI))
                    return (IAttribute*) (*it_m_attributes);
                else
                    it_m_attributes++;
            }
            break;
        }

        // Parameter list contains a valid Prefix
        case eParamHasPrefix:
        {
            while( it_m_attributes != m_attributes.end())
            {
                if( !strcmp( (*it_m_attributes)->getPrefix(), pachPrefix))
                    return (IAttribute*) (*it_m_attributes);
                else
                    it_m_attributes++;
            }
            break;
        }

        // Parameter list contains a valid URI and Prefix
        case eParamHasURI + eParamHasPrefix:
        {
            while( it_m_attributes != m_attributes.end())
            {
                if (!strcmp( (*it_m_attributes)->getURI(), pachURI) &&
                    !strcmp( (*it_m_attributes)->getPrefix(), pachPrefix))
                    return (IAttribute*) (*it_m_attributes);
                else
                    it_m_attributes++;
            }
            break;
        }

        // Parameter list contains a valid LocalName
        case eParamHasLocalName:
        {
            while( it_m_attributes != m_attributes.end())
            {
                if( !strcmp( (*it_m_attributes)->getLocalName(), pachLocalname))
                    return (IAttribute*) (*it_m_attributes);
                else
                    it_m_attributes++;
            }
            break;
        }

        // Parameter list contains a valid LocalName and URI
        case eParamHasLocalName + eParamHasURI:
        {
            while( it_m_attributes != m_attributes.end())
            {
                if( !strcmp( (*it_m_attributes)->getLocalName(), pachLocalname) &&
                    !strcmp( (*it_m_attributes)->getURI(), pachURI))
                    return (IAttribute*) (*it_m_attributes);
                else
                    it_m_attributes++;
            }
            break;
        }

        // Parameter list contains a valid LocalName and Prefix
        case eParamHasLocalName + eParamHasPrefix:
        {
            while( it_m_attributes != m_attributes.end())
            {
                if( !strcmp( (*it_m_attributes)->getLocalName(), pachLocalname) &&
                    !strcmp( (*it_m_attributes)->getPrefix(), pachPrefix))
                    return (IAttribute*) (*it_m_attributes);
                else
                    it_m_attributes++;
            }
            break;
        }

        // Parameter list contains a valid LocalName, URI and Prefix
        case eParamHasLocalName + eParamHasPrefix + eParamHasURI:
        {
            while( it_m_attributes != m_attributes.end())
            {
                if( !strcmp( (*it_m_attributes)->getLocalName(), pachLocalname) &&
                    !strcmp( (*it_m_attributes)->getURI(), pachURI) &&
                    !strcmp( (*it_m_attributes)->getPrefix(), pachPrefix))
                    return (IAttribute*) (*it_m_attributes);
                else
                    it_m_attributes++;
            }
            break;
        }

        // Parameter list contains no valid parameters
        default:
        {
            break;
        }
    }

    return NULL;
}

AXIS_CPP_NAMESPACE_END
