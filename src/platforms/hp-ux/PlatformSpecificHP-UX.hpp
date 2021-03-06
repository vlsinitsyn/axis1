/*
 *   Copyright 2004-2004 The Apache Software Foundation.
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


#if !defined(_PLATFORM_SPECIFIC_HPUX_HPP)
#define _PLATFORM_SPECIFIC_HPUX_HPP

#include <string>      

#define DIR_SEPARATOR   '/'
#define DIR_SEPARATOR_S "/"

// =============================================================
// Default paths to shared library/DLLs and files
// =============================================================
#define PLATFORM_DEFAULT_DEPLOY_PATH ""

#define PLATFORM_XMLPARSER_PATH      "libaxis_xmlparser.so"
#define PLATFORM_TRANSPORTHTTP_PATH  "libhttp_transport.so"
#define PLATFORM_CHANNEL_PATH        "libhttp_channel.so"
#define PLATFORM_SSLCHANNEL_PATH     "Unknown"

#define PLATFORM_LOG_PATH            "/usr/local/axiscpp_deploy/log/AxisLog"
#define PLATFORM_CLIENTLOG_PATH      "/usr/local/axiscpp_deploy/log/AxisClientLog"
#define PLATFORM_CONFIG_PATH         "/etc/axiscpp.conf"
#define PLATFORM_SECUREINFO			 ""

// =============================================================
// Library loading and procedure resolution
// =============================================================
#ifdef USE_LTDL
 #include <ltdl.h>
 #define DLHandler lt_dlhandle

 #define PLATFORM_LOADLIBINIT       lt_dlinit
 #define PLATFORM_LOADLIB(_lib)     lt_dlopen(_lib)
 #define PLATFORM_UNLOADLIB         lt_dlclose
 #define PLATFORM_GETPROCADDR       lt_dlsym
 #define PLATFORM_LOADLIBEXIT       lt_dlexit
 #define PLATFORM_LOADLIB_ERROR     string(lt_dlerror())
#else
 #include <dlfcn.h>
 #define DLHandler void*

 #define PLATFORM_LOADLIBINIT()
 #define PLATFORM_LOADLIB(_lib)     dlopen(_lib, RTLD_LAZY)

 #define PLATFORM_UNLOADLIB         dlclose
 #define PLATFORM_GETPROCADDR       dlsym
 #define PLATFORM_LOADLIBEXIT()
 #define PLATFORM_LOADLIB_ERROR     string(dlerror())

#endif

// =============================================================
// National Language Support
// =============================================================

// STRTOASC is to translate single byte 'native' character representation to ASCII
// ASCTOSTR is to translate single byte ascii representation to 'native' character
// CANNOT be used with constants
#define PLATFORM_STRTOASC( x ) ( x )
#define PLATFORM_ASCTOSTR( x ) ( x )

#define PLATFORM_DOUBLE_QUOTE_S                   "\""
#define PLATFORM_DOUBLE_QUOTE_C                   '\"'

// Some C functions are locale-sensitive. On IBM i strtod() is locale-sensitive,
// such that if running in french locale the strtod() function expects
// the decimal point to be a comma.  If the same holds true with
// other platforms then you will need to set this define appropriately.
#define PLATFORM_PROCESS_DECIMAL_POINT_C          '.'

#define PLATFORM_XML_ENTITY_REFERENCE_CHARS_S     "<>&\"\'"

// Case-insensitive string compares
#include <strings.h>    // for strcasecmp()

#define PLATFORM_STRCASECMP strcasecmp

// =============================================================
// Miscellaneous
// =============================================================
#include <sys/time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>      // for nonblocking

#define PLATFORM_SET_SOCKET_BLOCKING(x)           \
{                                                 \
    int fcntlArg = fcntl(x, F_GETFL, NULL);       \
    fcntlArg &= (~O_NONBLOCK);                    \
    fcntl(x, F_SETFL, fcntlArg);                  \
}

#define PLATFORM_SET_SOCKET_NONBLOCKING(x)        \
{                                                 \
    int fcntlArg = fcntl(x, F_GETFL, NULL);       \
    fcntlArg |= O_NONBLOCK;                       \
    fcntl(x, F_SETFL, fcntlArg);                  \
}

#define PLATFORM_SLEEP(x) sleep(x)

/**
 * Get the last error code from the system.
 * Please ensure that this is a thread safe implementation
 * and that it returns a long
 * @return long the lsat error message for this thread
 */
#define GETLASTERROR errno

/**
 * From the last error number get a sensible std::string representing it
 * @param errorNumber the error Number you are trying to get a message for
 * @return the error message. 
 */
#define PLATFORM_GET_ERROR_MESSAGE(errorNumber) string(strerror(errorNumber))

/**
 * Platform specific method to obtain current thread ID
 */
#define PLATFORM_GET_THREAD_ID pthread_self()

/**
 * Platform specific method to obtain current time in milli seconds
 */
#define PLATFORM_GET_TIME_IN_MILLIS ftime
#define PLATFORM_TIMEB timeb

/**
 * type to be used for 64bit integers
 */
#define LONGLONG long long
#define LONGLONGVALUE(value) value##LL
#define UNSIGNED_LONGLONGVALUE(value) value##ULL

/**
 * Format string to be used in printf for 64bit integers
 */
#define PRINTF_LONGLONG_FORMAT_SPECIFIER "%lld"
#define PRINTF_LONGLONG_FORMAT_SPECIFIER_CHARS "lld"
#define PRINTF_UNSIGNED_LONGLONG_FORMAT_SPECIFIER "%llu"
#define PRINTF_UNSIGNED_LONGLONG_FORMAT_SPECIFIER_CHARS "llu"
#define PRINTF_LONGLONG_LOG_FORMAT_SPECIFIER "%.8llu"

/**
 * File modes
 */
#define TRACE_FILE_MODE1   "a"
#define TRACE_FILE_MODE2   "a"

#endif


