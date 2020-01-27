#ifndef _UPNP_UTIL_H_
#define _UPNP_UTIL_H_

/*******************************************************************************
 *
 * Copyright (c) 2000-2003 Intel Corporation
 * All rights reserved.
 * Copyright (c) 2012 France Telecom All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * - Neither name of Intel Corporation nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#include <stdlib.h>
#include <string>

extern size_t upnp_strlcpy(char *dst, const char *src, size_t dsize);
inline size_t upnp_strlcpy(char *dst, const std::string& src, size_t dsize) {
	return upnp_strlcpy(dst, src.c_str(), dsize);
}

/* Size of the errorBuffer variable, passed to the strerror_r() function */
#define ERROR_BUFFER_LEN (size_t)256

/* C specific */
/* VC needs these in C++ mode too (do other compilers?) */
#if !defined(__cplusplus) || defined(UPNP_USE_MSVCPP)
	#ifdef WIN32
		#ifndef S_ISREG
			#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
		#endif
		#ifndef S_ISDIR
			#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
		#endif
		#ifndef EADDRINUSE		/* VS2010 has this defined */
			#define EADDRINUSE		WSAEADDRINUSE
		#endif
#ifndef strcasecmp
		#define strcasecmp		stricmp
#endif
#ifndef strncasecmp
		#define strncasecmp		strnicmp
#endif
		#define sleep(a)		Sleep((a)*1000)
		#define usleep(a)		Sleep((a)/1000)
		#define strerror_r(a,b,c)	(strerror_s((b),(c),(a)))
	#else
	#endif /* !WIN32 */
#endif /* !defined(__cplusplus) || defined(UPNP_USE_MSVCPP) */

#if !defined(_WIN32)
#include <string.h>
inline char *_check_strerror_r(int, char *errbuf) {
	return errbuf;
}
inline char *_check_strerror_r(char *cp, char *) {
	return cp;
}
inline int posix_strerror_r(int err, char *buf, size_t len) {
	char *cp = _check_strerror_r(strerror_r(err, buf, len), buf);
	if (cp != buf) {
		upnp_strlcpy(buf, cp, len);
	}
	return 0;
}
#endif

#ifndef MAX
#define MAX(a, b)   (((a)>(b))? (a):(b))
#endif
#ifndef MIN
#define MIN(a, b)   (((a)<(b))? (a):(b))
#endif

#endif /* _UPNP_UTIL_H_ */
