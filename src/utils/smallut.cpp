/* Copyright (C) 2006-2019 J.F.Dockes
 *
 * All rights reserved. 
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


#include "smallut.h"

#include <ctype.h>

using namespace std;

void stringtolower(string& io)
{
    string::iterator it = io.begin();
    string::iterator ite = io.end();
    while (it != ite) {
        *it = ::tolower(*it);
        it++;
    }
}
string stringtolower(const string& i)
{
    string o = i;
    stringtolower(o);
    return o;
}

//  s1 is already lowercase
int stringlowercmp(const string& s1, const string& s2)
{
    string::const_iterator it1 = s1.begin();
    string::const_iterator it2 = s2.begin();
    string::size_type size1 = s1.length(), size2 = s2.length();
    char c2;

    if (size1 < size2) {
        while (it1 != s1.end()) {
            c2 = ::tolower(*it2);
            if (*it1 != c2) {
                return *it1 > c2 ? 1 : -1;
            }
            ++it1;
            ++it2;
        }
        return size1 == size2 ? 0 : -1;
    }

    while (it2 != s2.end()) {
        c2 = ::tolower(*it2);
        if (*it1 != c2) {
            return *it1 > c2 ? 1 : -1;
        }
        ++it1;
        ++it2;
    }

    return size1 == size2 ? 0 : 1;
}

void trimstring(string& s, const char *ws)
{
    rtrimstring(s, ws);
    ltrimstring(s, ws);
}

void rtrimstring(string& s, const char *ws)
{
    string::size_type pos = s.find_last_not_of(ws);
    if (pos == string::npos) {
        s.clear();
    } else if (pos != s.length() - 1) {
        s.replace(pos + 1, string::npos, string());
    }
}

void ltrimstring(string& s, const char *ws)
{
    string::size_type pos = s.find_first_not_of(ws);
    if (pos == string::npos) {
        s.clear();
        return;
    }
    s.replace(0, pos, string());
}

size_t upnp_strlcpy(char *dst, const char *src, size_t dsize)
{
	if (nullptr == dst || 0 == dsize)
		return strlen(src) + 1;

	// Copy until either output full or end of src. Final zero not copied
	size_t cnt = dsize;
	while (*src && cnt > 0) {
		*dst++ = *src++;
		cnt--;
	}

	if (cnt == 0) {
		// Stopped because output full. dst now points beyond the
		// buffer, set the final zero before it, and count how many
		// more bytes we would need.
		dst[-1] = 0;
		while (*src++) {
			dsize++;
		}
	} else {
		// Stopped because end of input, set the final zero.
		dst[0] = 0;
	}
	return dsize - cnt + 1;
}

string xmlQuote(const string& in)
{
    string out;
    for (char i : in) {
        switch (i) {
        case '"':
            out += "&quot;";
            break;
        case '&':
            out += "&amp;";
            break;
        case '<':
            out += "&lt;";
            break;
        case '>':
            out += "&gt;";
            break;
        case '\'':
            out += "&apos;";
            break;
        default:
            out += i;
        }
    }
    return out;
}

int dom_cmp_name(const std::string& domname, const std::string& ref)
{
	std::string::size_type colon = domname.find(':');
	return colon == std::string::npos ?
		domname.compare(ref) : domname.compare(colon+1, std::string::npos, ref);
}
