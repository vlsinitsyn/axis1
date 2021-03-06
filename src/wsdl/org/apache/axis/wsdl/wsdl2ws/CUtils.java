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
 
/**
 * @author Srinath Perera(hemapani@openource.lk)
 * @author Susantha Kumara(susantha@opensource.lk, skumara@virtusa.com)
 * @author Sanjaya Singharage (sanjayas@opensource.lk, sanjayas@jkcsworld.com)
 */

package org.apache.axis.wsdl.wsdl2ws;

import org.apache.axis.wsdl.wsdl2ws.info.ParameterInfo;
import org.apache.axis.wsdl.wsdl2ws.info.Type;
import org.apache.axis.wsdl.wsdl2ws.info.TypeMap;
import org.apache.axis.wsdl.wsdl2ws.info.WebServiceContext;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Arrays;

import javax.xml.namespace.QName;

import java.lang.reflect.Array;
import java.io.BufferedWriter;
import java.io.IOException;

/**
 * Utility class.  The usage of a lot of the methods is dependent on the language.  So prior 
 * to doing any code generation, setLanguage() should be invoked.  Currently, only C and C++ 
 * languages are supported.
 *
 */
public class CUtils 
{
    public static final String WRAPPER_NAME_APPENDER = "Wrapper";
    public static final String CLASS_LOADER_APPENDER = "Service";
    public static final QName xsdAnyElementQName = new QName("http://ws.apache.org/axisc/types","AnyType");
    public static final QName xsdAnyTypeQName = new QName(WrapperConstants.SCHEMA_NAMESPACE,"anyType");
    public static final QName xsdSchemaQName  = new QName(WrapperConstants.SCHEMA_NAMESPACE,"schema");

    // File suffix for C++ Class files
    private static final String CPP_CLASS_SUFFIX = ".cpp";
    // File suffix for C++ Header files
    private static final String CPP_HEADER_SUFFIX = ".hpp";
    // File suffix for C Source files
    private static final String C_FILE_SUFFIX = ".c";
    // File suffix fr C Header files
    private static final String C_HEADER_SUFFIX = ".h";
    
    // Valid XML but invalid or reserved C/C++ characters 
    private static final char c_invalidCChars[] = {
        '/', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',',
        '-', '.', ':', ';', '<', '=', '>', '?',  '@', '\\','^', '`', '{',
        '|', '}', '~', '[', ']', '\u00A3'     
    };
    
    private static String c_fileExtension       = CPP_CLASS_SUFFIX;
    private static String c_headerFileExtension = CPP_HEADER_SUFFIX;

    
    // Contains all primitive types.
    private static HashSet c_primitiveTypes = null;
    
    // Contains all schema defined simple types.
    private static HashSet c_schemaDefinedSimpleTypes = new HashSet();
    
    // Returns the initialization value string for a simple type.
    private static Hashtable c_initValueForSimpleType = new Hashtable();
    
    // Maps a QName to a simple type (c++).
    private static Hashtable c_qnameToPrimitiveTypeMapperCPP = new Hashtable();
    
    // Maps a QName to a simple type (c).
    private static Hashtable c_qnameToPrimitiveTypeMapperC = new Hashtable();
    
    // Maps QName to simple type - uses one of the above maps depending on language.
    private static Hashtable c_qnameToPrimitiveTypeMapper = c_qnameToPrimitiveTypeMapperCPP;
    
    // Maps schema-defined QName to simple type 
    private static Hashtable c_schemaDefinedQNameToSimpleTypeMapper = new Hashtable();
    
    // Maps axis simple type to method suffix
    private static Hashtable c_simpleTypeToMethodSuffixMapper = new Hashtable();
    
    // Maps user-defined simple types to axis enumerator
    private static Hashtable c_simpleTypeToEnumMapper = new Hashtable();
    
    // Used to find out whether a simple type is a pointer type.
    private static HashSet c_pointerBasedTypes = null;
    
    // Used to determine if string type represents an array...previously we use to
    // do something like outParamTypeName.lastIndexOf ("_Array") all over the place
    // but this does not work if type defined in WSDL already has _Array in the name.
    private static HashSet c_arrayTypes = new HashSet();
    
    // Used to determine if QName maps to an array.
    private static Hashtable c_arrayTypeMapper = new Hashtable();
    
    // Used to determine if QName maps to an array.
    private static Hashtable c_arrayTypeMapper2 = new Hashtable();

    // Language
    private static String  c_language = WrapperConstants.LANGUAGE_CPP;
    
    private static String c_getAttributeAs = "getAttributeAs";
    private static String c_getElementAs   = "getElementAs";
    
    // following in support of generating unique names
    private static Hashtable c_uniqueNameMapper = new Hashtable();
    private static Vector c_uniqueNamesGenerated = new Vector();
    
    // list of c and cpp keywords
    private static HashSet c_cppkeywords = null;
    
    static{    
        String[] primitiveTypes = {
            // ====================
            // C++ primitive types
            // ====================
            "xsd__duration",        "xsd__dateTime",            "xsd__time",                    
            "xsd__date",            "xsd__gYearMonth",          "xsd__gYear",                    
            "xsd__gMonthDay",       "xsd__gDay",                "xsd__gMonth",                
            "xsd__string",          "xsd__normalizedString",    "xsd__token",                    
            "xsd__language",        "xsd__Name",                "xsd__NCName",                
            "xsd__ID",              "xsd__IDREF",               "xsd__IDREFS",                
            "xsd__ENTITY",          "xsd__ENTITIES",            "xsd__NMTOKEN",                
            "xsd__NMTOKENS",        "xsd__boolean",             "xsd__base64Binary",            
            "xsd__hexBinary",       "xsd__float",               "xsd__decimal",                
            "xsd__integer",         "xsd__nonPositiveInteger",  "xsd__negativeInteger",        
            "xsd__long",            "xsd__int",                 "xsd__short",                    
            "xsd__byte",            "xsd__nonNegativeInteger",  "xsd__unsignedLong",            
            "xsd__unsignedInt",     "xsd__unsignedShort",       "xsd__unsignedByte",            
            "xsd__positiveInteger", "xsd__double",              "xsd__anyURI",    
            "xsd__QName",           "xsd__NOTATION",
            
            "xsd__anyType", 
            
            // ====================
            // C primitive types
            // ====================
            "xsdc__duration",       "xsdc__dateTime",           "xsdc__time",               
            "xsdc__date",           "xsdc__gYearMonth",         "xsdc__gYear",                  
            "xsdc__gMonthDay",      "xsdc__gDay",               "xsdc__gMonth",                 
            "xsdc__string",         "xsdc__normalizedString",   "xsdc__token",                   
            "xsdc__language",       "xsdc__Name",               "xsdc__NCName",                   
            "xsdc__ID",             "xsdc__IDREF",              "xsdc__IDREFS",                   
            "xsdc__ENTITY",         "xsdc__ENTITIES",           "xsdc__NMTOKEN",               
            "xsdc__NMTOKENS",       "xsdc__boolean",            "xsdc__base64Binary",           
            "xsdc__hexBinary",      "xsdc__float",              "xsdc__decimal",               
            "xsdc__integer",        "xsdc__nonPositiveInteger", "xsdc__negativeInteger",       
            "xsdc__long",           "xsdc__int",                "xsdc__short",                   
            "xsdc__byte",           "xsdc__nonNegativeInteger", "xsdc__unsignedLong",           
            "xsdc__unsignedInt",    "xsdc__unsignedShort",      "xsdc__unsignedByte",           
            "xsdc__positiveInteger","xsdc__double",             "xsdc__anyURI",                  
            "xsdc__QName",          "xsdc__NOTATION",
            
            "xsdc__anyType" 
        };
        
        c_primitiveTypes = new HashSet(Arrays.asList(primitiveTypes));

        
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "duration"),                "xsd__duration");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "dateTime"),                "xsd__dateTime");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "time"),                    "xsd__time");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "date"),                    "xsd__date");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gYearMonth"),            "xsd__gYearMonth");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gYear"),                "xsd__gYear");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gMonthDay"),            "xsd__gMonthDay");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gDay"),                    "xsd__gDay");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gMonth"),                "xsd__gMonth");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "string"),                "xsd__string");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "normalizedString"),        "xsd__normalizedString");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "token"),                "xsd__token");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "language"),                "xsd__language");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "Name"),                    "xsd__Name");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "NCName"),                "xsd__NCName");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "ID"),                    "xsd__ID");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "IDREF"),                "xsd__IDREF");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "IDREFS"),                "xsd__IDREFS");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "ENTITY"),                "xsd__ENTITY");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "ENTITIES"),                "xsd__ENTITIES");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "NMTOKEN"),                "xsd__NMTOKEN");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "NMTOKENS"),                "xsd__NMTOKENS");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "boolean"),                "xsd__boolean");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "base64Binary"),            "xsd__base64Binary");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "hexBinary"),            "xsd__hexBinary");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "float"),                "xsd__float");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "decimal"),                "xsd__decimal");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "integer"),                "xsd__integer");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "nonPositiveInteger"),    "xsd__nonPositiveInteger");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "negativeInteger"),        "xsd__negativeInteger");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "long"),                    "xsd__long");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "int"),                    "xsd__int");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "short"),                "xsd__short");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "byte"),                    "xsd__byte");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "nonNegativeInteger"),    "xsd__nonNegativeInteger");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "unsignedLong"),            "xsd__unsignedLong");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "unsignedInt"),            "xsd__unsignedInt");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "unsignedShort"),        "xsd__unsignedShort");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "unsignedByte"),            "xsd__unsignedByte");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "positiveInteger"),        "xsd__positiveInteger");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "double"),                "xsd__double");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "QName"),                "xsd__QName");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "anyURI"),                "xsd__anyURI");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "NOTATION"),                "xsd__NOTATION");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "anyType"),                "xsd__anyType");
        
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "schema"),                 "xsd__anyType");
        
        // TODO revisit attachment support.
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.APACHE_XMLSOAP_NAMESPACE, "Image"),       "ISoapAttachment");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.APACHE_XMLSOAP_NAMESPACE, "PlainText"),  "ISoapAttachment");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.APACHE_XMLSOAP_NAMESPACE, "Multipart"),  "ISoapAttachment");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.APACHE_XMLSOAP_NAMESPACE, "Source"),     "ISoapAttachment");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.APACHE_XMLSOAP_NAMESPACE, "octet-stream"),   "ISoapAttachment");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.APACHE_XMLSOAP_NAMESPACE, "DataHandler"),    "ISoapAttachment");


        
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "duration"),            "xsdc__duration");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "dateTime"),            "xsdc__dateTime");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "time"),                "xsdc__time");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "date"),                "xsdc__date");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gYearMonth"),            "xsdc__gYearMonth");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gYear"),                "xsdc__gYear");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gMonthDay"),            "xsdc__gMonthDay");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gDay"),                "xsdc__gDay");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "gMonth"),                "xsdc__gMonth");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "string"),                "xsdc__string");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "normalizedString"),    "xsdc__normalizedString");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "token"),                "xsdc__token");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "language"),            "xsdc__language");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "Name"),                "xsdc__Name");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "NCName"),                "xsdc__NCName");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "ID"),                    "xsdc__ID");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "IDREF"),                "xsdc__IDREF");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "IDREFS"),                "xsdc__IDREFS");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "ENTITY"),                "xsdc__ENTITY");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "ENTITIES"),            "xsdc__ENTITIES");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "NMTOKEN"),            "xsdc__NMTOKEN");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "NMTOKENS"),            "xsdc__NMTOKENS");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "boolean"),            "xsdc__boolean");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "base64Binary"),        "xsdc__base64Binary");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "hexBinary"),            "xsdc__hexBinary");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "float"),                "xsdc__float");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "decimal"),            "xsdc__decimal");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "integer"),            "xsdc__integer");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "nonPositiveInteger"),    "xsdc__nonPositiveInteger");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "negativeInteger"),    "xsdc__negativeInteger");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "long"),                "xsdc__long");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "int"),                "xsdc__int");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "short"),                "xsdc__short");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "byte"),                "xsdc__byte");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "nonNegativeInteger"),    "xsdc__nonNegativeInteger");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "unsignedLong"),        "xsdc__unsignedLong");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "unsignedInt"),        "xsdc__unsignedInt");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "unsignedShort"),        "xsdc__unsignedShort");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "unsignedByte"),        "xsdc__unsignedByte");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "positiveInteger"),    "xsdc__positiveInteger");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "double"),                "xsdc__double");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "QName"),                "xsdc__QName");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "anyURI"),                "xsdc__anyURI");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "NOTATION"),            "xsdc__NOTATION");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "anyType"),                "xsdc__anyType");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SCHEMA_NAMESPACE, "schema"),                 "xsdc__anyType");


        /* TODO:
         *   Should be removed when the following issue will be fixed :
         *     -> http://marc.theaimsgroup.com/?t=107907748000002&r=1&w=2 
         */
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "int"),            "xsd__int");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "byte"),            "xsd__byte");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "float"),            "xsd__float");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "long"),            "xsd__long");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "integer"),        "xsd__integer");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "double"),            "xsd__double");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "char"),            "xsd__char");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "short"),            "xsd__short");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "string"),            "xsd__string");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "dateTime"),        "xsd__dateTime");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "date"),            "xsd__date");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "time"),            "xsd__time");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "duration"),        "xsd__duration");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "base64Binary"),    "xsd__base64Binary");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "hexBinary"),        "xsd__hexBinary");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "decimal"),        "xsd__decimal");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "boolean"),        "xsd__boolean");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "anyURI"),            "xsd__anyURI");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "unsignedByte"),    "xsd__unsignedByte");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "unsignedInt"),    "xsd__unsignedInt");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "unsignedLong"),    "xsd__unsignedLong");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "unsignedShort"),    "xsd__unsignedShort");
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "QName"),            "xsd__QName");        
//        c_qnameToSimpleTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "NCName"),            "xsd__NCName");                
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "NMTOKEN"),       "xsd__NMTOKEN");
        
        /* TODO:
         *  Another strange issue from Axis 1.1 runtime when base64binary is in input/output operations.
         */    
        c_qnameToPrimitiveTypeMapperCPP.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "base64"), "xsd__base64Binary");        
        
        /* TODO:
         *   Should be removed when the following issue will be fixed :
         *     -> http://marc.theaimsgroup.com/?t=107907748000002&r=1&w=2 
         */
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "int"),            "xsdc__int");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "byte"),            "xsdc__byte");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "float"),            "xsdc__float");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "long"),            "xsdc__long");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "integer"),        "xsdc__integer");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "double"),            "xsdc__double");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "char"),            "xsdc__char");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "short"),            "xsdc__short");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "string"),            "xsdc__string");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "dateTime"),        "xsdc__dateTime");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "date"),            "xsdc__date");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "time"),            "xsdc__time");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "duration"),        "xsdc__duration");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "base64Binary"),    "xsdc__base64Binary");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "hexBinary"),        "xsdc__hexBinary");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "decimal"),        "xsdc__decimal");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "boolean"),        "xsdc__boolean");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "anyURI"),            "xsdc__anyURI");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "unsignedByte"),    "xsdc__unsignedByte");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "unsignedInt"),    "xsdc__unsignedInt");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "unsignedLong"),    "xsdc__unsignedLong");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "unsignedShort"),    "xsdc__unsignedShort");
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "QName"),            "xsdc__QName");        
//        c_qnameToSimpleTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "NCName"),            "xsdc__NCName");        


        /* TODO:
         *  Another strange issue from Axis 1.1 runtime when base64binary is in input/output operations.
         */    
        c_qnameToPrimitiveTypeMapperC.put(new QName(WrapperConstants.SOAPENC_NAMESPACE, "base64"), "xsdc__base64Binary");        
        
        c_simpleTypeToMethodSuffixMapper.put("xsd__duration",                "Duration");
        c_simpleTypeToMethodSuffixMapper.put("xsd__dateTime",                "DateTime");
        c_simpleTypeToMethodSuffixMapper.put("xsd__time",                    "Time");
        c_simpleTypeToMethodSuffixMapper.put("xsd__date",                    "Date");
        c_simpleTypeToMethodSuffixMapper.put("xsd__gYearMonth",            "GYearMonth");
        c_simpleTypeToMethodSuffixMapper.put("xsd__gYear",                "GYear");
        c_simpleTypeToMethodSuffixMapper.put("xsd__gMonthDay",            "GMonthDay");
        c_simpleTypeToMethodSuffixMapper.put("xsd__gDay",                    "GDay");
        c_simpleTypeToMethodSuffixMapper.put("xsd__gMonth",                "GMonth");
        c_simpleTypeToMethodSuffixMapper.put("xsd__string",                "String");
        c_simpleTypeToMethodSuffixMapper.put("xsd__normalizedString",        "NormalizedString");
        c_simpleTypeToMethodSuffixMapper.put("xsd__token",                "Token");
        c_simpleTypeToMethodSuffixMapper.put("xsd__language",                "Language");
        c_simpleTypeToMethodSuffixMapper.put("xsd__Name",                    "Name");
        c_simpleTypeToMethodSuffixMapper.put("xsd__NCName",                "NCName");
        c_simpleTypeToMethodSuffixMapper.put("xsd__ID",                    "ID");
        c_simpleTypeToMethodSuffixMapper.put("xsd__IDREF",                "IDREF");
        c_simpleTypeToMethodSuffixMapper.put("xsd__IDREFS",                "IDREFS");
        c_simpleTypeToMethodSuffixMapper.put("xsd__ENTITY",                "ENTITY");
        c_simpleTypeToMethodSuffixMapper.put("xsd__ENTITIES",                "ENTITIES");
        c_simpleTypeToMethodSuffixMapper.put("xsd__NMTOKEN",                "NMTOKEN");
        c_simpleTypeToMethodSuffixMapper.put("xsd__NMTOKENS",                "NMTOKENS");
        c_simpleTypeToMethodSuffixMapper.put("xsd__boolean",                "Boolean");
        c_simpleTypeToMethodSuffixMapper.put("xsd__base64Binary",            "Base64Binary");
        c_simpleTypeToMethodSuffixMapper.put("xsd__hexBinary",            "HexBinary");
        c_simpleTypeToMethodSuffixMapper.put("xsd__float",                "Float");
        c_simpleTypeToMethodSuffixMapper.put("xsd__decimal",                "Decimal");
        c_simpleTypeToMethodSuffixMapper.put("xsd__integer",                "Integer");
        c_simpleTypeToMethodSuffixMapper.put("xsd__nonPositiveInteger",     "NonPositiveInteger");
        c_simpleTypeToMethodSuffixMapper.put("xsd__negativeInteger",        "NegativeInteger");
        c_simpleTypeToMethodSuffixMapper.put("xsd__long",                    "Long");
        c_simpleTypeToMethodSuffixMapper.put("xsd__int",                    "Int");
        c_simpleTypeToMethodSuffixMapper.put("xsd__short",                "Short");
        c_simpleTypeToMethodSuffixMapper.put("xsd__byte",                    "Byte");
        c_simpleTypeToMethodSuffixMapper.put("xsd__nonNegativeInteger",    "NonNegativeInteger");
        c_simpleTypeToMethodSuffixMapper.put("xsd__unsignedLong",            "UnsignedLong");
        c_simpleTypeToMethodSuffixMapper.put("xsd__unsignedInt",            "UnsignedInt");
        c_simpleTypeToMethodSuffixMapper.put("xsd__unsignedShort",        "UnsignedShort");
        c_simpleTypeToMethodSuffixMapper.put("xsd__unsignedByte",            "UnsignedByte");
        c_simpleTypeToMethodSuffixMapper.put("xsd__positiveInteger",        "PositiveInteger");
        c_simpleTypeToMethodSuffixMapper.put("xsd__double",                "Double");
        c_simpleTypeToMethodSuffixMapper.put("xsd__anyURI",                "AnyURI");
        c_simpleTypeToMethodSuffixMapper.put("xsd__QName",                "QName");
        c_simpleTypeToMethodSuffixMapper.put("xsd__NOTATION",                "NOTATION");
        c_simpleTypeToMethodSuffixMapper.put("xsd__anyType",                "AnyType");

        
        c_simpleTypeToMethodSuffixMapper.put("xsdc__duration",                "Duration");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__dateTime",                "DateTime");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__time",                    "Time");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__date",                    "Date");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__gYearMonth",            "GYearMonth");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__gYear",                    "GYear");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__gMonthDay",                "GMonthDay");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__gDay",                    "GDay");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__gMonth",                "GMonth");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__string",                "String");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__normalizedString",        "NormalizedString");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__token",                    "Token");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__language",                "Language");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__Name",                    "Name");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__NCName",                "NCName");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__ID",                    "ID");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__IDREF",                    "IDREF");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__IDREFS",                "IDREFS");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__ENTITY",                "ENTITY");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__ENTITIES",                "ENTITIES");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__NMTOKEN",                "NMTOKEN");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__NMTOKENS",                "NMTOKENS");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__boolean",                "Boolean");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__base64Binary",            "Base64Binary");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__hexBinary",                "HexBinary");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__float",                    "Float");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__decimal",                "Decimal");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__integer",                "Integer");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__nonPositiveInteger",     "NonPositiveInteger");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__negativeInteger",        "NegativeInteger");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__long",                    "Long");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__int",                    "Int");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__short",                    "Short");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__byte",                    "Byte");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__nonNegativeInteger",    "NonNegativeInteger");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__unsignedLong",            "UnsignedLong");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__unsignedInt",            "UnsignedInt");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__unsignedShort",            "UnsignedShort");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__unsignedByte",            "UnsignedByte");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__positiveInteger",        "PositiveInteger");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__double",                "Double");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__anyURI",                "AnyURI");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__QName",                    "QName");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__NOTATION",                "NOTATION");
        c_simpleTypeToMethodSuffixMapper.put("xsdc__anyType",                "AnyType");

        
        c_simpleTypeToEnumMapper.put("xsd__duration",            "XSD_DURATION");
        c_simpleTypeToEnumMapper.put("xsd__dateTime",            "XSD_DATETIME");
        c_simpleTypeToEnumMapper.put("xsd__time",                "XSD_TIME");
        c_simpleTypeToEnumMapper.put("xsd__date",                "XSD_DATE");
        c_simpleTypeToEnumMapper.put("xsd__gYearMonth",            "XSD_GYEARMONTH");
        c_simpleTypeToEnumMapper.put("xsd__gYear",                "XSD_GYEAR");
        c_simpleTypeToEnumMapper.put("xsd__gMonthDay",            "XSD_GMONTHDAY");
        c_simpleTypeToEnumMapper.put("xsd__gDay",                "XSD_GDAY");
        c_simpleTypeToEnumMapper.put("xsd__gMonth",                "XSD_GMONTH");
        c_simpleTypeToEnumMapper.put("xsd__string",                "XSD_STRING");
        c_simpleTypeToEnumMapper.put("xsd__normalizedString",    "XSD_NORMALIZEDSTRING");
        c_simpleTypeToEnumMapper.put("xsd__token",                "XSD_TOKEN");
        c_simpleTypeToEnumMapper.put("xsd__language",            "XSD_LANGUAGE");
        c_simpleTypeToEnumMapper.put("xsd__Name",                "XSD_NAME");
        c_simpleTypeToEnumMapper.put("xsd__NCName",                "XSD_NCNAME");
        c_simpleTypeToEnumMapper.put("xsd__ID",                    "XSD_ID");
        c_simpleTypeToEnumMapper.put("xsd__IDREF",                "XSD_IDREF");
        c_simpleTypeToEnumMapper.put("xsd__IDREFS",                "XSD_IDREFS");
        c_simpleTypeToEnumMapper.put("xsd__ENTITY",                "XSD_ENTITY");
        c_simpleTypeToEnumMapper.put("xsd__ENTITIES",            "XSD_ENTITIES");
        c_simpleTypeToEnumMapper.put("xsd__NMTOKEN",            "XSD_NMTOKEN");
        c_simpleTypeToEnumMapper.put("xsd__NMTOKENS",            "XSD_NMTOKENS");
        c_simpleTypeToEnumMapper.put("xsd__boolean",            "XSD_BOOLEAN");
        c_simpleTypeToEnumMapper.put("xsd__base64Binary",        "XSD_BASE64BINARY");
        c_simpleTypeToEnumMapper.put("xsd__hexBinary",            "XSD_HEXBINARY");
        c_simpleTypeToEnumMapper.put("xsd__float",                "XSD_FLOAT");
        c_simpleTypeToEnumMapper.put("xsd__decimal",            "XSD_DECIMAL");
        c_simpleTypeToEnumMapper.put("xsd__integer",            "XSD_INTEGER");
        c_simpleTypeToEnumMapper.put("xsd__nonPositiveInteger",    "XSD_NONPOSITIVEINTEGER");
        c_simpleTypeToEnumMapper.put("xsd__negativeInteger",    "XSD_NEGATIVEINTEGER");
        c_simpleTypeToEnumMapper.put("xsd__long",                "XSD_LONG");
        c_simpleTypeToEnumMapper.put("xsd__int",                "XSD_INT");
        c_simpleTypeToEnumMapper.put("xsd__short",                "XSD_SHORT");
        c_simpleTypeToEnumMapper.put("xsd__byte",                "XSD_BYTE");
        c_simpleTypeToEnumMapper.put("xsd__nonNegativeInteger",    "XSD_NONNEGATIVEINTEGER");
        c_simpleTypeToEnumMapper.put("xsd__unsignedLong",        "XSD_UNSIGNEDLONG");
        c_simpleTypeToEnumMapper.put("xsd__unsignedInt",        "XSD_UNSIGNEDINT");
        c_simpleTypeToEnumMapper.put("xsd__unsignedShort",        "XSD_UNSIGNEDSHORT");
        c_simpleTypeToEnumMapper.put("xsd__unsignedByte",        "XSD_UNSIGNEDBYTE");
        c_simpleTypeToEnumMapper.put("xsd__positiveInteger",        "XSD_POSITIVEINTEGER");
        c_simpleTypeToEnumMapper.put("xsd__double",                "XSD_DOUBLE");
        c_simpleTypeToEnumMapper.put("xsd__anyURI",                "XSD_ANYURI");
        c_simpleTypeToEnumMapper.put("xsd__QName",                "XSD_QNAME");
        c_simpleTypeToEnumMapper.put("xsd__NOTATION",            "XSD_NOTATION");
        c_simpleTypeToEnumMapper.put("xsd__anyType",             "XSD_ANYTYPE");


        c_simpleTypeToEnumMapper.put("xsdc__duration",            "XSDC_DURATION");
        c_simpleTypeToEnumMapper.put("xsdc__dateTime",            "XSDC_DATETIME");
        c_simpleTypeToEnumMapper.put("xsdc__time",                "XSDC_TIME");
        c_simpleTypeToEnumMapper.put("xsdc__date",                "XSDC_DATE");
        c_simpleTypeToEnumMapper.put("xsdc__gYearMonth",            "XSDC_GYEARMONTH");
        c_simpleTypeToEnumMapper.put("xsdc__gYear",                "XSDC_GYEAR");
        c_simpleTypeToEnumMapper.put("xsdc__gMonthDay",            "XSDC_GMONTHDAY");
        c_simpleTypeToEnumMapper.put("xsdc__gDay",                "XSDC_GDAY");
        c_simpleTypeToEnumMapper.put("xsdc__gMonth",                "XSDC_GMONTH");
        c_simpleTypeToEnumMapper.put("xsdc__string",                "XSDC_STRING");
        c_simpleTypeToEnumMapper.put("xsdc__normalizedString",    "XSDC_NORMALIZEDSTRING");
        c_simpleTypeToEnumMapper.put("xsdc__token",                "XSDC_TOKEN");
        c_simpleTypeToEnumMapper.put("xsdc__language",            "XSDC_LANGUAGE");
        c_simpleTypeToEnumMapper.put("xsdc__Name",                "XSDC_NAME");
        c_simpleTypeToEnumMapper.put("xsdc__NCName",                "XSDC_NCNAME");
        c_simpleTypeToEnumMapper.put("xsdc__ID",                    "XSDC_ID");
        c_simpleTypeToEnumMapper.put("xsdc__IDREF",                "XSDC_IDREF");
        c_simpleTypeToEnumMapper.put("xsdc__IDREFS",                "XSDC_IDREFS");
        c_simpleTypeToEnumMapper.put("xsdc__ENTITY",                "XSDC_ENTITY");
        c_simpleTypeToEnumMapper.put("xsdc__ENTITIES",            "XSDC_ENTITIES");
        c_simpleTypeToEnumMapper.put("xsdc__NMTOKEN",                "XSDC_NMTOKEN");
        c_simpleTypeToEnumMapper.put("xsdc__NMTOKENS",            "XSDC_NMTOKENS");
        c_simpleTypeToEnumMapper.put("xsdc__boolean",                "XSDC_BOOLEAN");
        c_simpleTypeToEnumMapper.put("xsdc__base64Binary",        "XSDC_BASE64BINARY");
        c_simpleTypeToEnumMapper.put("xsdc__hexBinary",            "XSDC_HEXBINARY");
        c_simpleTypeToEnumMapper.put("xsdc__float",                "XSDC_FLOAT");
        c_simpleTypeToEnumMapper.put("xsdc__decimal",                "XSDC_DECIMAL");
        c_simpleTypeToEnumMapper.put("xsdc__integer",                "XSDC_INTEGER");
        c_simpleTypeToEnumMapper.put("xsdc__nonPositiveInteger",    "XSDC_NONPOSITIVEINTEGER");
        c_simpleTypeToEnumMapper.put("xsdc__negativeInteger",        "XSDC_NEGATIVEINTEGER");
        c_simpleTypeToEnumMapper.put("xsdc__long",                "XSDC_LONG");
        c_simpleTypeToEnumMapper.put("xsdc__int",                    "XSDC_INT");
        c_simpleTypeToEnumMapper.put("xsdc__short",                "XSDC_SHORT");
        c_simpleTypeToEnumMapper.put("xsdc__byte",                "XSDC_BYTE");
        c_simpleTypeToEnumMapper.put("xsdc__nonNegativeInteger",    "XSDC_NONNEGATIVEINTEGER");
        c_simpleTypeToEnumMapper.put("xsdc__unsignedLong",        "XSDC_UNSIGNEDLONG");
        c_simpleTypeToEnumMapper.put("xsdc__unsignedInt",            "XSDC_UNSIGNEDINT");
        c_simpleTypeToEnumMapper.put("xsdc__unsignedShort",        "XSDC_UNSIGNEDSHORT");
        c_simpleTypeToEnumMapper.put("xsdc__unsignedByte",        "XSDC_UNSIGNEDBYTE");
        c_simpleTypeToEnumMapper.put("xsdc__positiveInteger",        "XSDC_POSITIVEINTEGER");
        c_simpleTypeToEnumMapper.put("xsdc__double",                "XSDC_DOUBLE");
        c_simpleTypeToEnumMapper.put("xsdc__anyURI",                "XSDC_ANYURI");
        c_simpleTypeToEnumMapper.put("xsdc__QName",                "XSDC_QNAME");
        c_simpleTypeToEnumMapper.put("xsdc__NOTATION",            "XSDC_NOTATION");
        c_simpleTypeToEnumMapper.put("xsdc__anyType",             "XSDC_ANYTYPE");



        c_initValueForSimpleType.put("xsd__duration",                "0");
        c_initValueForSimpleType.put("xsd__dateTime",                "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsd__time",                    "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsd__date",                    "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsd__gYearMonth",            "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsd__gYear",                    "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsd__gMonthDay",                "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsd__gDay",                    "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsd__gMonth",                "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsd__string",                "NULL");
        c_initValueForSimpleType.put("xsd__normalizedString",        "NULL");
        c_initValueForSimpleType.put("xsd__token",                    "NULL");
        c_initValueForSimpleType.put("xsd__language",                "NULL");
        c_initValueForSimpleType.put("xsd__Name",                    "NULL");
        c_initValueForSimpleType.put("xsd__NCName",                "NULL");
        c_initValueForSimpleType.put("xsd__ID",                    "NULL");
        c_initValueForSimpleType.put("xsd__IDREF",                    "NULL");
        c_initValueForSimpleType.put("xsd__IDREFS",                "NULL");
        c_initValueForSimpleType.put("xsd__ENTITY",                "NULL");
        c_initValueForSimpleType.put("xsd__ENTITIES",                "NULL");
        c_initValueForSimpleType.put("xsd__NMTOKEN",                "NULL");
        c_initValueForSimpleType.put("xsd__NMTOKENS",                "NULL");
        c_initValueForSimpleType.put("xsd__boolean",                "false_");
//        initValuesCpp.put("xsd__base64Binary",            ""); // This is a class, so doesn't need to be initialized.
//        initValuesCpp.put("xsd__hexBinary",                ""); // This is a class, so doesn't need to be initialized.
        c_initValueForSimpleType.put("xsd__float",                    "0.0");
        c_initValueForSimpleType.put("xsd__decimal",                "0.0");
        c_initValueForSimpleType.put("xsd__integer",                "0");
        c_initValueForSimpleType.put("xsd__nonPositiveInteger",    "0");
        c_initValueForSimpleType.put("xsd__negativeInteger",        "0");
        c_initValueForSimpleType.put("xsd__long",                    "0");
        c_initValueForSimpleType.put("xsd__int",                    "0");
        c_initValueForSimpleType.put("xsd__short",                    "0");
        c_initValueForSimpleType.put("xsd__byte",                    "0");
        c_initValueForSimpleType.put("xsd__nonNegativeInteger",    "0");
        c_initValueForSimpleType.put("xsd__unsignedByte",            "0");
        c_initValueForSimpleType.put("xsd__unsignedInt",            "0");
        c_initValueForSimpleType.put("xsd__unsignedLong",            "0");
        c_initValueForSimpleType.put("xsd__unsignedShort",            "0");
        c_initValueForSimpleType.put("xsd__positiveInteger",        "0");
        c_initValueForSimpleType.put("xsd__double",                "0.0");
        c_initValueForSimpleType.put("xsd__anyURI",                "NULL");
        c_initValueForSimpleType.put("xsd__QName",                    "NULL");
        c_initValueForSimpleType.put("xsd__NOTATION",                "NULL");
        c_initValueForSimpleType.put("xsd__anyType",                "NULL");

        
        c_initValueForSimpleType.put("xsdc__duration",                "0");
        c_initValueForSimpleType.put("xsdc__dateTime",                "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsdc__time",                    "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsdc__date",                    "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsdc__gYearMonth",                "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsdc__gYear",                    "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsdc__gMonthDay",                "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsdc__gDay",                    "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsdc__gMonth",                    "{0, 0, 0, 0, 0, 0, 0, 0, 0}");
        c_initValueForSimpleType.put("xsdc__string",                    "NULL");
        c_initValueForSimpleType.put("xsdc__normalizedString",        "NULL");
        c_initValueForSimpleType.put("xsdc__token",                    "NULL");
        c_initValueForSimpleType.put("xsdc__language",                "NULL");
        c_initValueForSimpleType.put("xsdc__Name",                    "NULL");
        c_initValueForSimpleType.put("xsdc__NCName",                    "NULL");
        c_initValueForSimpleType.put("xsdc__ID",                        "NULL");
        c_initValueForSimpleType.put("xsdc__IDREF",                    "NULL");
        c_initValueForSimpleType.put("xsdc__IDREFS",                    "NULL");
        c_initValueForSimpleType.put("xsdc__ENTITY",                    "NULL");
        c_initValueForSimpleType.put("xsdc__ENTITIES",                "NULL");
        c_initValueForSimpleType.put("xsdc__NMTOKEN",                "NULL");
        c_initValueForSimpleType.put("xsdc__NMTOKENS",                "NULL");
        c_initValueForSimpleType.put("xsdc__boolean",                "false_");
        c_initValueForSimpleType.put("xsdc__base64Binary",            "{NULL, 0}");
        c_initValueForSimpleType.put("xsdc__hexBinary",                "{NULL, 0}");
        c_initValueForSimpleType.put("xsdc__float",                    "0.0");
        c_initValueForSimpleType.put("xsdc__decimal",                "0.0");
        c_initValueForSimpleType.put("xsdc__integer",                "0");
        c_initValueForSimpleType.put("xsdc__nonPositiveInteger",        "0");
        c_initValueForSimpleType.put("xsdc__negativeInteger",        "0");
        c_initValueForSimpleType.put("xsdc__long",                    "0");
        c_initValueForSimpleType.put("xsdc__int",                    "0");
        c_initValueForSimpleType.put("xsdc__short",                    "0");
        c_initValueForSimpleType.put("xsdc__byte",                    "0");
        c_initValueForSimpleType.put("xsdc__nonNegativeInteger",        "0");
        c_initValueForSimpleType.put("xsdc__unsignedByte",            "0");
        c_initValueForSimpleType.put("xsdc__unsignedInt",            "0");
        c_initValueForSimpleType.put("xsdc__unsignedLong",            "0");
        c_initValueForSimpleType.put("xsdc__unsignedShort",            "0");
        c_initValueForSimpleType.put("xsdc__positiveInteger",        "0");
        c_initValueForSimpleType.put("xsdc__double",                    "0.0");
        c_initValueForSimpleType.put("xsdc__anyURI",                    "NULL");
        c_initValueForSimpleType.put("xsdc__QName",                    "NULL");
        c_initValueForSimpleType.put("xsdc__NOTATION",                "NULL");
        c_initValueForSimpleType.put("xsdc__anyType",                "NULL");

        
        String[] pointerTypes = {
        // C++ types
        "xsd__string",         "xsd__normalizedString",             "xsd__token",
        "xsd__language",       "xsd__Name",                         "xsd__NCName",
        "xsd__ID",             "xsd__IDREF",                        "xsd__IDREFS",
        "xsd__ENTITY",         "xsd__ENTITIES",                     "xsd__NMTOKEN",
        "xsd__NMTOKENS",       "xsd__anyURI",                       "xsd__QName",
        "xsd__NOTATION",       "xsd__anyType",   
        // C types
        "xsdc__string",        "xsdc__normalizedString",            "xsdc__token",
        "xsdc__language",      "xsdc__Name",                        "xsdc__NCName",
        "xsdc__ID",            "xsdc__IDREF",                       "xsdc__IDREFS",
        "xsdc__ENTITY",        "xsdc__ENTITIES",                    "xsdc__NMTOKEN",       
        "xsdc__NMTOKENS",      "xsdc__anyURI",                      "xsdc__QName",         
        "xsdc__NOTATION",      "xsdc__anyType"
        };
        c_pointerBasedTypes = new HashSet(Arrays.asList(pointerTypes));
    
        String[] words2 = {
                "and", "and_eq", "asm", "auto",
                "bitand", "bitor", "bool", "break",
                "case", "catch", "char", "class",  "compl", "const", "const_cast", "continue",
                "default", "delete", "do",  "double", "dynamic_cast",
                "else", "enum", "errno", "explicit", "export", "extern",
                "false", "float", "for", "friend",       
                "goto",
                "if", "inline", "int",
                "long",
                "mutable",
                "namespace", "new", "not", "not_eq",
                "operator", "or", "or_eq",
                "private", "protected", "public",
                "register", "reinterpret_cast", "return",
                "short", "signed", "sizeof", "static", "static_cast", "struct", "switch",
                "template", "this", "throw", "true", "try", "typedef", "typeid", "typename",
                "union", "unsigned", "using",
                "virtual", "void", "volatile",  
                "wchar_t", "while",
                "xor", "xor_eq",
                "string"
         };
        c_cppkeywords = new HashSet(Arrays.asList(words2));
    }
    
    
    /**
     * This method resolved clashes between types and the programming language keywords
     * by adding "_" to the front. This is a JAX_RPC recommendation of the situation.  
     * 
     * @param name
     * @return
     */
    public static String resolveWSDL2LanguageNameClashes(String name)
    {
       if (c_cppkeywords.contains(name))
           return "_" + name;

       return name;
    }
   
    /**
     * Some utility methods key off of the language that is being generated. 
     * 
     * @param language
     */
    public static void setLanguage(String language) throws Exception
    {
        // Only C and C++ are supported here.
        c_language = language.toLowerCase();
        if (WrapperConstants.LANGUAGE_C.equalsIgnoreCase(language))
        {
            c_qnameToPrimitiveTypeMapper = c_qnameToPrimitiveTypeMapperC;
            c_getAttributeAs = "GetAttributeAs";
            c_getElementAs   = "GetElementAs";
            
            c_fileExtension       = C_FILE_SUFFIX;
            c_headerFileExtension = C_HEADER_SUFFIX;

        }
        else if (WrapperConstants.LANGUAGE_CPP.equalsIgnoreCase(language))
        {
            c_qnameToPrimitiveTypeMapper = c_qnameToPrimitiveTypeMapperCPP;
            c_getAttributeAs = "getAttributeAs";
            c_getElementAs   = "getElementAs";
            
            c_fileExtension       = CPP_CLASS_SUFFIX;
            c_headerFileExtension = CPP_HEADER_SUFFIX;
        }
        else
            throw new WrapperFault("Unsupported language.");
    }
    
    /**
     * Get language to generate.
     * 
     * @return
     */
    public static String getLanguage()
    {
        return c_language;
    }
    
    /**
     * Add schema defined simple type to hash table. 
     * 
     * @param qname
     * @param type
     */
    public static void addSchemaDefinedSimpleType(QName qname, String type)
    {
        c_schemaDefinedQNameToSimpleTypeMapper.put(qname, type);
        c_schemaDefinedSimpleTypes.add(type);
    }
    
    public static boolean isSimpleType(QName name)
    {
        if (c_qnameToPrimitiveTypeMapper.containsKey(name))
            return true;
        else 
            return c_schemaDefinedQNameToSimpleTypeMapper.containsKey(name);
    }
    
    /**
     * Method to determine if name passed in is that of a simple type. 
     * 
     * @param name
     * @return
     */
    public static boolean isSimpleType(String name)
    {
        if (c_primitiveTypes.contains(name))
            return true;
        else
            return c_schemaDefinedSimpleTypes.contains(name);
    } 
    
    /**
     * Returns boolean indicating whether QName represents a primitive type.
     * Primitive types map to axis types such as xsd__xxxx or xsdc__xxxxx.
     * 
     * @param qname
     * @return
     */
    public static boolean isPrimitiveType(QName qname)
    {
        return c_qnameToPrimitiveTypeMapper.containsKey(qname);
    }
    
    /**
     * Returns boolean indicating whether name is a primitive type
     * such as xsd__xxxx or xsdc__xxxxx.
     * 
     * @param qname
     * @return
     */
    public static boolean isPrimitiveType(String name)
    {
        return c_primitiveTypes.contains(name);
    }
    
    /**
     * Method to determine if simple type is a pointer type. 
     * 
     * @param name
     * @return
     */
    public static boolean isPointerType(String name)
    {
        if (name != null)
            return c_pointerBasedTypes.contains(name);
        
        return false;
    }
    
    /**
     * Method to determine if simple type is a pointer type. 
     * 
     * @param name
     * @return
     */
    public static boolean isPointerType(QName name)
    {
        if (null != c_qnameToPrimitiveTypeMapper.get(name))
            return isPointerType((String)c_qnameToPrimitiveTypeMapper.get(name));
        else
            return isPointerType((String)c_schemaDefinedQNameToSimpleTypeMapper.get(name));
    }    
    
    /**
     * Adds a type to the collection of pointer types.
     * 
     * @param s
     */
    public static void addPointerType(String s)
    {
        c_pointerBasedTypes.add(s);
    }
  
    /**
     * Method to determine if QName represents an xsd:any element.
     * 
     * @param name
     * @return
     */
    public static boolean isAnyElement(QName name)
    {
            return name.equals(xsdAnyElementQName);
    }
    
    /**
     * Method to determine if QName represents a schema.
     * 
     * @param name
     * @return
     */
    public static boolean isSchemaReference(QName name)
    {
            return name.equals(xsdSchemaQName);
    }
    
    /**
     * Method to determine if QName represents an xsd:anyType.
     * 
     * @param name
     * @return
     */
    public static boolean isAnyType(QName name)
    {
        // Note we treat schema reference as xsd:anyType.
            return (name.equals(xsdAnyTypeQName) || isSchemaReference(name));
    }
    
    /**
     * Returns deserializer method name for element or attribute to be deserialized.
     * 
     * @param typeName String representing simple type.
     * @param isAttrib Whether the attribute is an element or not.
     * @return
     */
    public static String getDeserializerMethodName(String typeName, boolean isAttrib)
    {
        String methodname = (String)c_simpleTypeToMethodSuffixMapper.get(typeName);
        
        // for attributes that are not defined with a type, just treat as string. Here
        // is an example: 
        //   <s:attribute fixed="http://tempuri.org/DStruttura.xsd" name="namespace" />
        // The above translates to anyType, but since we do not have such a method
        // for attributes, just treat as a string.
        if (isAttrib && methodname.equals("AnyType"))
            methodname = "String";
        
        methodname = (isAttrib ? c_getAttributeAs : c_getElementAs) + methodname;
        return methodname;
    }
    
    /**
     * Returns the method suffix name for a type.
     * 
     * @param type
     * @return
     */
    public static String getDeserializerMethodSuffixForType(String type)
    {
        return (String)c_simpleTypeToMethodSuffixMapper.get(type);
    }
    
    /**
     * Adds a method suffix name for type.
     * 
     * @param type
     * @param suffix
     */
    public static void addDeserializerMethodSuffixForType(String type, String suffix)
    {
        c_simpleTypeToMethodSuffixMapper.put(type, suffix);
    }

    /**
     * Returns simple type that matches QName.
     * 
     * @param qname
     * @return
     */
    public static String getSimpleType(QName qname) 
    {
        Object val = c_qnameToPrimitiveTypeMapper.get(qname);
        if (val == null)
            val = c_schemaDefinedQNameToSimpleTypeMapper.get(qname);
        
        return (String) val;
    }

    /**
     * Returns primitive type that matches QName.
     * 
     * @param qname
     * @return
     */
    public static String getPrimitiveType(QName qname) 
    {
        return (String)c_qnameToPrimitiveTypeMapper.get(qname);
    }
    
    /**
     * Get initialization value string for a simple type. 
     * 
     * @param typeName string representing a simple type.
     * @return String containing the axis XSD define for the type.
     */
    public static String getInitValueForType(String typeName) 
    {
        return (String)c_initValueForSimpleType.get(typeName);
    }
    
    /**
     * Adds initialization value string for a simple type to the mapper.
     * 
     * @param typeName string representing a simple type.
     * @return String containing the axis XSD define for the type.
     */
    public static void addInitValueForType(String typeName, String initValue) 
    {
        c_initValueForSimpleType.put(typeName, initValue);
    }    
        
    /**
     * 
     * @param wname
     * @return
     */
    public static String getWebServiceNameFromWrapperName(String wname)
    {
        return wname.substring(0, wname.length()- CUtils.WRAPPER_NAME_APPENDER.length());
    }
    
    /**
     * Return the XSD axis type enumerator for a simple type.
     * 
     * @param stype String representing a simple type.
     * @return
     */
    public static String getXSDEnumeratorForType(String stype)
    {
        return (String)c_simpleTypeToEnumMapper.get(stype); 
    }
    
    /**
     * Adds XSD axis type enumerator for a simple type.
     * 
     * @param stype String representing a simple type.
     * @param xsdEnum String representing an XSD axis enumerator for type.
     */
    public static void addXSDEnumeratorForType(String stype, String xsdEnum)
    {
        c_simpleTypeToEnumMapper.put(stype, xsdEnum); 
    }
    
    /**
     * Adds a type to the collection of array types.
     * 
     * @param s
     */
    public static void addArrayType(QName qn, QName qn_array)
    {
        c_arrayTypes.add(qn_array.getLocalPart());
        c_arrayTypeMapper.put(qn, qn_array);
        c_arrayTypeMapper2.put(qn.getLocalPart(), qn_array.getLocalPart());
    }
    
    /**
     * Method to determine if type is an array type. 
     * 
     * @param name
     * @return
     */
    public static boolean isArrayType(String name)
    {
        if (name == null)
            return false;
        
        if ((name.startsWith("xsd__") || name.startsWith("xsdc__")) && name.endsWith("_Array"))
            return true;
        else
            return c_arrayTypes.contains(name);
    }
    
    /**
     * Method to determine if type is an array type. 
     * 
     * @param name
     * @return
     */
    public static QName getArrayQNameForType(QName qname)
    {   
        return (QName)c_arrayTypeMapper.get(qname);
    }
    
    /**
     * Method to get array name for base name. 
     * 
     * @param name
     * @return
     */
    public static String getArrayNameForType(String b)
    {   
        String arrayName = (String)c_arrayTypeMapper2.get(b);
        if (arrayName == null)
        {
            arrayName = b + "_Array";
            c_arrayTypes.add(arrayName);
        }
        return arrayName;
    }
    
    /**
     * Generates array name for complex type.
     * 
     * @param qname
     * @return
     */
    public static String getArrayNameForComplexType(QName qname)
    {
        if (isAnyElement(qname))
            return "AnyType";
        
        String arrayName = null;
        if (!c_qnameToPrimitiveTypeMapper.containsKey(qname) 
                && !c_schemaDefinedQNameToSimpleTypeMapper.containsKey(qname))
        {
            QName arrayQName = getArrayQNameForType(qname);
            if (arrayQName != null) 
                arrayName = arrayQName.getLocalPart();
            else
            {
                arrayName = qname.getLocalPart() + "_Array";
                c_arrayTypes.add(arrayName);
            }
            
            if (TypeMap.isAnonymousType(qname))
                arrayName = CUtils.sanitizeString(arrayName);
        }
        
        return arrayName;        
    }
    
    /**
     * Generates array name for simple types.
     * 
     * @param stype
     * @return
     */
    public static String getArrayNameforSimpleType(QName qn, String stype)
    {
        String arrayName = null;
        
        QName arrayQName = getArrayQNameForType(qn);
        if (arrayQName != null) 
            arrayName = arrayQName.getLocalPart();
        else
        {
            arrayName = stype + "_Array";
            c_arrayTypes.add(arrayName);
        }
       
        return arrayName;        
    }   
    
    /**
     * Sanitizes a string so that it can be used in generated code. 
     * 
     * @param name
     * @return
     */
    public static String sanitizeString( String name)
    {
        int i;
        String sanitisedName=name;
        
        // Anonymous names start with '>'. For example, '>Type'. However, if it was 
        // nested, then it would be something like '>>Type>Type2'. 
        // We should really be nice and get the name after last '>', but will wait and
        // simply remove starting '>'.
        for (i=0; i<name.length() && name.charAt(i) == TypeMap.ANON_TOKEN_CHAR; ++i);
        sanitisedName = name.substring(i);

        // Now replace invalid character with '_'
        for(i=0; i < Array.getLength(c_invalidCChars); i++)
            sanitisedName = sanitisedName.replace((char)c_invalidCChars[i], '_'); 
        
        // Now ensure that string does not conflict with language constructs
        sanitisedName = resolveWSDL2LanguageNameClashes(sanitisedName);
        
        return sanitisedName;
    }
    
    /**
     * Sanitizes a string so that it can be used in generated code. 
     * The '*' character is not touched.
     * 
     */
    public static String sanitizeStringWithSplats( String name)
    {
        if (name == null)
            return name;
        
        String sanitisedName=name.trim();
        int i = -1;
        if (sanitisedName.endsWith("*"))
            i = sanitisedName.lastIndexOf('*');
        String suffix = "";
        String prefix = sanitisedName;
        
        if (i != -1)
        {
            suffix = sanitisedName.substring(i);
            prefix = sanitisedName.substring(0, i).trim();
        }
        
        sanitisedName = sanitizeString(prefix) + suffix;
        return sanitisedName;
    }
    /**
     * NOT CURRENTLY USED.
     * This routine is used to basically handle anonymous type naming.  Anonymous types
     * have names such as '>type' and '>>type>type2', the latter being a nested type. 
     * When generating classes, we want to use the simplist name, which is the name after
     * the last '>' character. This routine ensure the uniqueness of the name returned by
     * keeping a hash table of mapped names and a vector of generated unique names.
     */
    public static String getUniqueName(String oldName)
    {    
        // Should never happen, but just in case.
        if (oldName == null)
            return oldName;
        
        // If name already in hash table, return the corresponding name
        String newName = (String)c_uniqueNameMapper.get(oldName);
        
        // If name was not in hash table, generate one, store in hash table.
        if (newName == null)
        {            
            newName = sanitizeString(oldName);
            
            // Ensure name does not conflict with language constructs
            newName = resolveWSDL2LanguageNameClashes(newName);
            
            // Ensure uniqueness
            int suffix = 2;            
            while (c_uniqueNamesGenerated.contains(newName))
                newName = newName + Integer.toString(suffix++);
            
            // Put newname in hash tables
            c_uniqueNameMapper.put(oldName, newName);
            c_uniqueNamesGenerated.add(newName);
        }

        return newName;
    }
    
    /**
     * This routine is used to determine if a string can be used as an identifier
     * in the C or C++ language. Currently used to determine if enumerator value can 
     * be used as part of an identifier. 
     */
    public static boolean isValidCIdentifier(String id, boolean checkForNumericFirstChar)
    {    
        if (id == null || id.equals(""))
            return false;
        
        if (checkForNumericFirstChar)
            if (id.charAt(0) >= '0' && id.charAt(0) <= '9')
                return false;
        
        // Check for invalid characters
        for(int i=0; i < Array.getLength(c_invalidCChars); i++)
            if (id.indexOf(c_invalidCChars[i]) != -1)
                return false;
        
        // Check for blanks
        if (id.indexOf(' ') != -1)
            return false;
        
        return true;
    }
    
    /**
     * Removes all characters that match the specified character from the beginning of string.
     * 
     * @param s
     * @param c
     * @return
     */
    public static String removeStartingCharFromString(String s, char c)
    {
        String sNew = s;
        
        if (s != null && s.length()>0)
        {
            int i = 0;
            for (i=0; i < s.length() && s.charAt(i) == c; ++i);
            sNew = s.substring(i);
        }
        
        return sNew;
    }
    
    /**
     * Prints a block comment. 
     * 
     * @param writer
     * @param s
     * @throws IOException
     */
    public static void printBlockComment(BufferedWriter writer, String s) throws IOException
    {
        writer.write("\n");
        writer.write("\t// ======================================================================\n");
        
        // TODO: divide string into multiple lines if greater then 80, sensitive
        //       to not break line in middle of word. for now all comments are one-liners.
        writer.write("\t// " + s + "\n");
        
        writer.write("\t// ======================================================================\n");  
        writer.write("\n");
    }
    
    /**
     * Prints a method comment. 
     * 
     * @param writer
     * @param s
     * @throws IOException
     */
    public static void printMethodComment(BufferedWriter writer, String s) throws IOException
    {
        writer.write("\n");
        writer.write("/**\n");
        writer.write(" ******************************************************************************\n");
        
        // TODO: divide string into multiple lines if greater then 80, sensitive
        //       to not break line in middle of word. for now all comments are one-liners.
        writer.write(" * " + s + "\n");

        writer.write(" ******************************************************************************\n");
        writer.write(" */\n");  
        writer.write("\n");
    }   
    
    /**
     * Prints a comment. 
     * 
     * @param writer
     * @param s
     * @throws IOException
     */
    public static void printComment(BufferedWriter writer, String s) throws IOException
    {
        writer.write("\n");
        
        // TODO: divide string into multiple lines if greater then 80, sensitive
        //       to not break line in middle of word. for now all comments are one-liners.
        writer.write("\t// " + s + "\n");
        
        writer.write("\n");
    }
    
    /**
     * extract the parameter name from the fully qualified name
     * @param fullyQualifiedName
     * @return only the class name 
     */
    public static String getNameFromFullyQualifiedName(String fullyQualifiedName)
    {
        int index = fullyQualifiedName.indexOf('.');
        int lastIndex = 0;
        while (index != -1)
        {
            lastIndex = index;
            index = fullyQualifiedName.indexOf('.', lastIndex + 1);
        }
        
        if (lastIndex == 0)
            return fullyQualifiedName;

        return fullyQualifiedName.substring(lastIndex + 1);
    }

    /**
     *  get class part of the class and if name happen to be a Simple type return 
     *  the Wrapper Class name(service.0 wrapper class name)
     *  
     */
    public static String getLanguageTypeName4Type(Type type) throws WrapperFault
    {
        if (type.isArray())
        {
            QName qname = getArrayType(type).getName();
            //this can never be a simple type
            return CUtils.getArrayNameForComplexType(qname);
        }
        else
            return type.getLanguageSpecificName();
    }

    /**
     * Capitalize the first Character of a given String. 
     * 
     * @param value String 
     * @return changed String 
     */
    public static String capitalizeFirstCharacter(String value)
    {
        char[] chars = value.toCharArray();
        chars[0] = Character.toUpperCase(value.charAt(0));
        return new String(chars);
    }

    /**
     * Lowercase the first character in a string.
     * 
     * @param value
     * @return
     */
    public static String firstCharacterToLowercase(String value)
    {
        char[] chars = value.toCharArray();
        chars[0] = Character.toLowerCase(value.charAt(0));
        return new String(chars);
    }
    
    public static String getClassNameFromParamInfoConsideringArrays(
        ParameterInfo param,
        WebServiceContext wscontext)
        throws WrapperFault
    {
        if (CUtils.isAnyElement(param.getType().getName()))
            return "AnyType*";
        
        Type type = wscontext.getTypemap().getType(param.getSchemaName());
        if (null != type && type.isSimpleType())
        {
            String name = param.getLangName();
            
            if( name.indexOf(">") != -1)
                name = CUtils.getSimpleType (param.getType().getBaseType());
            
            return name;
        }
        else if (!CUtils.isPrimitiveType(param.getSchemaName()))
        { 
            //arrays or complex types
            if (null != type && type.isArray())
            {
                QName qn = getArrayType(type).getName();

                String arrayName = CUtils.getArrayNameForComplexType(qn);
                if (null == arrayName) 
                    arrayName = CUtils.getArrayNameforSimpleType(qn, CUtils.getSimpleType(qn));

                return arrayName;
            }
            else
            {
                return param.getLangName() + "*";
                //All complex types will be pointers    
            }
        }
        else if (null != type && param.isArray())
        {
            /* This enables having simple type array declarations in the wrapping element
             * <s:element name="GetProjectNamesResponse">
                <s:complexType>
                    <s:sequence>
                        <s:element minOccurs="0" maxOccurs="unbounded" form="unqualified" name="return" type="s:string" />
                    </s:sequence>
                </s:complexType>
            </s:element>
             */
            return CUtils.getArrayNameforSimpleType(type.getName(), CUtils.getSimpleType(type.getName()));
        }
        else
            return param.getLangName();
    }

    /**
     * Returns the Type of elements in the array.
     * 
     * @param type
     * @return
     * @throws WrapperFault
     */
    public static Type getArrayType(Type type) throws WrapperFault
    {
        if (!type.isArray())
            return null;

        Iterator elements = type.getElementnames();
        if (elements.hasNext())
            return type.getElementForElementName((String) elements.next()).getType();
        
        throw new WrapperFault("Array type do not have any attibutes");
    }

    /**
     * Returns the implementation file extension for a file (i.e. ".c" or ".cpp").
     * 
     * @return the c_fileExtension
     */
    public static String getImplFileExtension()
    {
        return c_fileExtension;
    }

    /**
     * Returns header file extension for a file (i.e. ".h" or ".hpp").
     * 
     * @return the c_headerFileExtension
     */
    public static String getHeaderFileExtension()
    {
        return c_headerFileExtension;
    }
}

