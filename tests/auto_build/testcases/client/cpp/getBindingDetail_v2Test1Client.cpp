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

/*
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 */

#include <string>
#include <iostream>
#include <stdlib.h>
#include <axis/AxisException.hpp>
using namespace std;

#include "Inquire.hpp"

#define ARRAYSIZE 1 

char endpoint[256];

int
main(int argc, char *argv[])
{
    if( argc < 2 )
    {
        printf( "Usage: %s [endpoint] ", argv[0]);
        exit(0);
    }

    sprintf(endpoint, argv[1]); 
        char* cpname = "2.0";
        int outputSize=0;
    Inquire *ws = new Inquire(endpoint);   

                bool bSuccess = false;
                int     iRetryIterationCount = 3;

        do
        {
                try 
                {
                        char buffer[100];
                        // Storing Binding Key to Buffer 
                        sprintf(buffer, "585e9258-c7b7-4701-986e-1646f7166ed1");        
             
                         /* Case1 :
                            In this we pass one binding key and get back one binding Template and then displays the details*/
                        bindingKey_Array arrBindingKeys;
                        bindingKey * arrayBinding = new bindingKey[ARRAYSIZE];  
                        int i;
                        for (i = 0; i < ARRAYSIZE; i++)
                         {
                                 arrayBinding[i] = new char[100];
                                 strcpy(arrayBinding[i],buffer);
                         }
                        arrBindingKeys.set(arrayBinding,ARRAYSIZE);
                        cout << "invoking get_bindingDetail ...\n";                     
                        bindingTemplate_Array* bindingTemplates = ws->get_bindingDetail(&arrBindingKeys, cpname);
                        outputSize = 0;
                        bindingTemplate ** arrayOutBinding = bindingTemplates->get(outputSize);                 
                        cout << "bindingTemplate_Array size = " <<  outputSize << endl;
                        for( i = 0; i < outputSize; i++ ) 
                        {
                                cout << "Binding Template " <<  i + 1 << endl;
                                cout << "\t Binding Key Ref = " << arrayOutBinding[i]->bindingKey_Ref<<endl;
                                cout << "\t Service Key Ref = " << arrayOutBinding[i]->serviceKey_Ref<<endl;    
                                //Display Description
                                int outputSizeDesc=0;
                                description ** arrayDesc = arrayOutBinding[i]->description_Ref->get(outputSizeDesc);
                                cout << "\t Description ... " << endl;
                                for( int j = 0; j < outputSizeDesc; j++)
                                {
                                        cout << "\t\t Description " << j+1 << " = " << arrayDesc[j]->description_value<< endl;
                                }
                                /*if( arrayOutBinding[i]->accessPoint_Ref)
                                {
                                        cout << "\t Access Point " << endl;                                             
                                        cout << "\t\t Point = " << arrayOutBinding[i]->accessPoint_Ref->accessPoint_value << endl;
                                        cout <<  "\t\t URL Type = " << arrayOutBinding[i]->accessPoint_Ref->URLType_Ref << endl;                                        
                                }*/
                                if(arrayOutBinding[i]->tModelInstanceDetails_Ref){
                                                int tModelInstanceInfoSize=0;
                                                tModelInstanceInfo ** arrtModelInstInfo = arrayOutBinding[i]->tModelInstanceDetails_Ref->tModelInstanceInfo_Ref->get(tModelInstanceInfoSize);
                                                cout << "\ttModelInstanceInfo..." << endl;
                                                for(int k=0;k<tModelInstanceInfoSize;k++){
                                                        cout<<"\t\ttModelKey Value = " << arrtModelInstInfo[k]<< endl;
                                                }
                                }  
                        }


                        /* Case2:
                           In this we pass 2 bindingKeys and get back two business Services */
                        
                        arrayBinding = new bindingKey[2];        
                        for (i = 0; i < 2; i++)
                         {
                                 arrayBinding[i] = new char[100];                                
                         } 
                         strcpy(arrayBinding[0],buffer);
                         strcpy(arrayBinding[1],"2ebf3e2e-58c0-4022-83a6-eed8b1eea6e8");
                        arrBindingKeys.set(arrayBinding,2);
                        cout << "\n\ninvoking get_bindingDetail ...\n";                 
                        //Inquire *ws1 = new Inquire(endpoint); 
                        bindingTemplates = ws->get_bindingDetail(&arrBindingKeys, cpname);
                        outputSize = 0;
                        arrayOutBinding = bindingTemplates->get(outputSize);                    
                        cout << "bindingTemplate_Array size = " <<  outputSize << endl;
                        for( i = 0; i < outputSize; i++ ) 
                        {
                                cout << "Binding Template " <<  i + 1 << endl;
                                cout << "\t Binding Key Ref = " << arrayOutBinding[i]->bindingKey_Ref<<endl;
                                cout << "\t Service Key Ref = " << arrayOutBinding[i]->serviceKey_Ref<<endl;    
                                //Display Description
                                int outputSizeDesc=0;
                                description ** arrayDesc = arrayOutBinding[i]->description_Ref->get(outputSizeDesc);
                                cout << "\t Description ... " << endl;
                                for( int j = 0; j < outputSizeDesc; j++)
                                {
                                        cout << "\t\t Description " << j+1 << " = " << arrayDesc[j]->description_value<< endl;
                                }                               
                                
                                /*if( arrayOutBinding[i]->accessPoint_Ref)
                                {
                                        cout << "\t Access Point " << endl;                                             
                                        cout << "\t\t Point = " << arrayOutBinding[i]->accessPoint_Ref->accessPoint_value << endl;
                                        cout <<  "\t\t URL Type = " << arrayOutBinding[i]->accessPoint_Ref->URLType_Ref << endl;                                        
                                }*/
                                if(arrayOutBinding[i]->tModelInstanceDetails_Ref){
                                                int tModelInstanceInfoSize=0;
                                                tModelInstanceInfo ** arrtModelInstInfo = arrayOutBinding[i]->tModelInstanceDetails_Ref->tModelInstanceInfo_Ref->get(tModelInstanceInfoSize);
                                                cout << "\ttModelInstanceInfo..." << endl;
                                                for(int k=0;k<tModelInstanceInfoSize;k++){
                                                        cout<<"\t\ttModelKey Value = " << arrtModelInstInfo[k]<< endl;
                                                }
                                }  

                        }

                        /* Case 3:
                           In this we pass a non existing Binding Key */

                        
                        arrayBinding = new bindingKey[2];        
                        for (i = 0; i < 1; i++)
                         {
                                 arrayBinding[i] = new char[100];       
                                 strcpy(arrayBinding[0],"05e39405-6216-46bd-8f4c-dad67cda4c15");
                         } 
                         
                        
                        arrBindingKeys.set(arrayBinding,1);                     
                        //Inquire *ws2 = new Inquire(endpoint); 
                        cout << "\n\ninvoking get_bindingDetail ...\n";
                        bindingTemplates = ws->get_bindingDetail(&arrBindingKeys, cpname);
                        outputSize = 0;
                        arrayOutBinding = bindingTemplates->get(outputSize);                    
                        cout << "bindingTemplate_Array size = " << outputSize << endl;
                        for( i = 0; i < outputSize; i++ ) 
                        {
                                cout << "Binding Template " <<  i + 1 << endl;
                                cout << "\t Binding Key Ref = " << arrayOutBinding[i]->bindingKey_Ref<<endl;
                                cout << "\t Service Key Ref = " << arrayOutBinding[i]->serviceKey_Ref<<endl;    
                                //Display Description
                                int outputSizeDesc=0;
                                description ** arrayDesc = arrayOutBinding[i]->description_Ref->get(outputSizeDesc);
                                cout << "\t Description ... " << endl;
                                for( int j = 0; j < outputSizeDesc; j++)
                                {
                                        cout << "\t\t Description " << j+1 << " = " << arrayDesc[j]->description_value<< endl;
                                }                               
                                

                                /*if( arrayOutBinding[i]->accessPoint_Ref)
                                {
                                        cout << "\t Access Point " << endl;                                             
                                        cout << "\t\t Point = " << arrayOutBinding[i]->accessPoint_Ref->accessPoint_value << endl;
                                        cout <<  "\t\t URL Type = " << arrayOutBinding[i]->accessPoint_Ref->URLType_Ref << endl;                                        
                                }*/
                                
                                if(arrayOutBinding[i]->tModelInstanceDetails_Ref){
                                                int tModelInstanceInfoSize=0;
                                                tModelInstanceInfo ** arrtModelInstInfo = arrayOutBinding[i]->tModelInstanceDetails_Ref->tModelInstanceInfo_Ref->get(tModelInstanceInfoSize);
                                                cout << "\ttModelInstanceInfo..." << endl;
                                                for(int k=0;k<tModelInstanceInfoSize;k++){
                                                        cout<<"\t\ttModelKey Value = " << arrtModelInstInfo[k]<< endl;
                                                }
                                }  
                        }
                        bSuccess = true;
                } 
                catch(AxisException& e)
                {
                        bool bSilent = false;

                        if( e.getExceptionCode() == CLIENT_TRANSPORT_OPEN_CONNECTION_FAILED)
                        {
                                if( iRetryIterationCount > 0)
                                {
                                        bSilent = true;
                                }
                        }
                        else
                        {
                                iRetryIterationCount = 0;
                        }

            if( !bSilent)
                        {
        printf("Exception caught : %s\n", e.what());
                        }
    }
                iRetryIterationCount--;
                } while( iRetryIterationCount > 0 && !bSuccess);

    return 0;
}
