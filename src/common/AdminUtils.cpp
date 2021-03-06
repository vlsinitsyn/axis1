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
#include "../platforms/PlatformAutoSense.hpp"

#include <axis/Axis.hpp>

#include "AdminUtils.h"
#include "../wsdd/WSDDDeployment.h"

#include "AxisTrace.h"

extern AXIS_CPP_NAMESPACE_PREFIX WSDDDeployment* g_pWSDDDeployment;

AXIS_CPP_NAMESPACE_START

AdminUtils::AdminUtils()
{

}

AdminUtils::~AdminUtils()
{

}

int AdminUtils::updateWSDD(AxisChar* wsdd)
{
    return g_pWSDDDeployment->updateWSDD(wsdd);    
}



void AdminUtils::stopAxis()
{
    Axis::stopAxis();
}

AXIS_CPP_NAMESPACE_END
