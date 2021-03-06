// Copyright 2003-2004 The Apache Software Foundation.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * This file was auto-generated by the Axis C++ Web Service Generator (WSDL2Ws)
 * This file contains definitions of the web service
 */

#include "SimpleTypeInnerUnboundedInOutputWS.hpp"


SimpleTypeInnerUnboundedInOutputWS::SimpleTypeInnerUnboundedInOutputWS()
{
}

SimpleTypeInnerUnboundedInOutputWS::~SimpleTypeInnerUnboundedInOutputWS()
{
}

/* This function is called by the AxisEngine when something went wrong
 with the current web service request processing. Appropriate actions should
 be taken here.*/
void SimpleTypeInnerUnboundedInOutputWS::onFault()
{
}

Type1* SimpleTypeInnerUnboundedInOutputWS::getInput(xsd__int * Value0)  
{
	Type1 *retValue=new Type1();
	_Type1_ident_Array ident;
	_Type1_ident * array=new _Type1_ident[*Value0];
	char *buff=new char[4];
	for(int i=0;i<*Value0;i++){
		array[i]=new char[4];
		sprintf(buff,"%d",i);
	    strcpy(array[i],buff);
	}
	ident.set(array,*Value0);
	retValue->setident(&ident);
	return retValue;
}
