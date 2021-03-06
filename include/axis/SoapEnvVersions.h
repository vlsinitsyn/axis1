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
 *
 *
 */
/**
 * @file SoapEnvVersions.hpp
 */
#ifndef SOAPENVVERSIONS_INCLUDED
#define SOAPENVVERSIONS_INCLUDED

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE   */
/* ----------------------------------------------------------------   */
/* THIS HEADER FILE PATTERNED AFTER CORRESPONDING hpp HEADER FILE.    */
/* CHANGES TO hpp HEADER FILE MAY NEED TO BE PROPAGATED HERE AND IN   */
/* THE IMPLEMEMTATION OF THE C APIS.                                  */
/* ----------------------------------------------------------------   */
/* NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE   */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#ifdef __cplusplus
 extern "C" {
#endif


#include <axis/GDefine.h>

/*
 * Here SKW stands for SoapKeyWord. This is a internal naming convension
 * for Axis C++.
 */
/**
 * @enum SOAP_WORDS
 * Enumeration of SOAP key words
 */
typedef enum SOAP_WORDSTag 
{
    /**
     * SOAP Envelope
     */
    SKW_ENVELOPE=0, /* this should always be 0 */
    
    /**
     * SOAP Header
     */
    SKW_HEADER,
    
    /**
     * SOAP Body
     */
    SKW_BODY,
    
    /**
     * SOAP mustUnderstand attribute
     */
    SKW_MUSTUNDERSTAND,
    
    /**
     * SOAP actor attribute
     */
    SKW_ACTOR,
    
    /**
     * SOAP encodingStyle attribute
     */
    SKW_ENCODING_STYLE,
    
    /**
     * SOAP Fault
     */
    SKW_FAULT,
    
    /**
     * SOAP Fault code
     */
    SKW_FAULT_CODE,
    
    /**
     * SOAP Fault string
     */
    SKW_FAULT_STRING,
    
    /**
     * SOAP Fault actor
     */
    SKW_FAULT_ACTOR,
    
    /**
     * SOAP Fault detail
     */
    SKW_DETAIL,
    
    /**
     * Multi-Ref
     */
    SKW_MULTIREF,
    
    /**
     * type attribute
     */
    SKW_TYPE,
    
    /**
     * arrayType attribute
     */
    SKW_ARRAYTYPE,
    
    /**
     * href attribute
     */
    SKW_HREF,
    
    /**
     * id attribute
     */
    SKW_ID,

    SOAP_WORDS_LAST /*this should be the number of entries in this enum */
} AXISC_SOAP_WORDS;

/**
 * @enum SOAP_VERSION
 * Enumeration to indicate SOAP version being used
 */
typedef enum SOAP_VERSIONTag 
{
    /**
     * SOAP 1.1
     */
    SOAP_VER_1_1=0,
    
    /**
     * SOAP 1.2
     */
    SOAP_VER_1_2,

    VERSION_LAST
} AXISC_SOAP_VERSION;



#ifdef __cplusplus
 }
#endif

#endif /* SOAPENVVERSIONS_INCLUDED */
