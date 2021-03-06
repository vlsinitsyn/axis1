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

 
/**
 * @author Srinath Perera(hemapani@openource.lk)
 */
package org.apache.axis.wsdl.wsdl2ws;

public interface WrapperConstants {
    public static final String SCHEMA_NAMESPACE = "http://www.w3.org/2001/XMLSchema";
	public static final String SOAPENC_NAMESPACE = "http://schemas.xmlsoap.org/soap/encoding/";
	public static final String APACHE_XMLSOAP_NAMESPACE = "http://xml.apache.org/xml-soap";

	//language
	public static final String LANGUAGE_CPP = "c++";
	public static final String LANGUAGE_C = "c";
	//service style 
    public static final String STYLE_RPC      = "rpc";
    public static final String STYLE_DOCUMENT = "document";
	
	public static final String TRANSPORT_HTTP = "http://schemas.xmlsoap.org/soap/http";
	
	public static final String DEFAULT_TARGET_ENDPOINT_URI = "http://127.0.0.1:8080/";
	//wrapper genarator type
	//cpp constants 
	public static final int GENERATOR_CLASSLOADER_CPP = 13 ;
	public static final int GENERATOR_WRAPPER_CPP = 14;
	public static final int GENERATOR_CLIENT_STUB_CPP = 15;
	public static final int GENERATOR_SERVICE_CPP = 17 ;
	public static final int GENERATOR_WRAPPER_HPP = 18;
	public static final int GENERATOR_CLIENT_STUB_HPP = 19;
	public static final int GENERATOR_PARAM_CPP_ALL = 20 ;
	public static final int GENERATOR_SERVICE_HPP = 21 ;
	public static final int GENERATOR_CLIENT_EXCEPTION = 50;
	public static final int GENERATOR_SERVER_EXCEPTION = 51;
	//c constants 
	public static final int GENERATOR_CLASSLOADER_C = 22 ;
	public static final int GENERATOR_WRAPPER_C = 23;
	public static final int GENERATOR_CLIENT_STUB_C = 24;
	public static final int GENERATOR_SERVICE_C = 25 ;
	public static final int GENERATOR_WRAPPER_H = 26;
	public static final int GENERATOR_CLIENT_STUB_H = 27;
	public static final int GENERATOR_PARAM_C_ALL = 28;

	//common constants
	public static final int GENERATOR_DEPLOYMENT = 40;
	public static final int GENERATOR_UNDEPLOYMENT = 41;
	

    //target engine
    public static final String SERVER = "server";
	public static final String CLIENT = "client";
	
	public static final String SOAPENC_ARRAY = "Array";
	
}
