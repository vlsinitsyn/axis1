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
 *
 */


#if !defined(AXIS_SESSIONSCOPEHANDLERPOOL_H__OF_AXIS_INCLUDED_)
#define AXIS_SESSIONSCOPEHANDLERPOOL_H__OF_AXIS_INCLUDED_

#define SESSIONLESSHANDLERS	"0aaaaa"

#include "SharedObject.h"
#include <axis/BasicHandler.hpp>

#include <map>
#include <list>
#include <string>

AXIS_CPP_NAMESPACE_START

using namespace std;
/*
 *   @class SessionScopeHandlerPool
 *   @brief interface for the SessionScopeHandlerPool class.
 *
 *
 *   @author Susantha Kumara (skumara@virtusa.com)
 */
class SessionScopeHandlerPool:protected SharedObject
{
    public:
        SessionScopeHandlerPool ();
        virtual ~ SessionScopeHandlerPool ();
    private:
        typedef map < string, list <BasicHandler*> >SessionHandlers;
        map <int, SessionHandlers*>m_Handlers;
    public:
        int getInstance (string &sSessionId, BasicHandler** pHandler, 
            int nLibId);
        int putInstance (string &sSessionId, BasicHandler* pHandler, 
            int nLibId);
        void endSession (string &sSessionId);
};

AXIS_CPP_NAMESPACE_END

#endif 
