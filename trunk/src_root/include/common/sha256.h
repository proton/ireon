/* Copyright (C) The Freenet Project (http://freenetproject.org/)

* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef _sha256_h
#define _sha256_h

#define word32 unsigned long
#define byte unsigned char

typedef struct {
      word32 H[ 8 ];
      word32 hbits, lbits;
      byte M[ 64 ];
      byte mlen;
} SHA256_ctx;

void SHA256_init ( SHA256_ctx* );
void SHA256_update( SHA256_ctx*, const byte *, word32 );
void SHA256_final ( SHA256_ctx* );
void SHA256_digest( SHA256_ctx*, byte* );

class SHA256 {

    SHA256_ctx ctx;
    byte digest[32];
 public:

    SHA256 () { init (); }

    enum { DIGEST_LEN = 32 };

    void init ( ) { SHA256_init (&ctx); }
    void reset () { SHA256_init (&ctx); }
    void write(const byte *a, unsigned int b) { SHA256_update (&ctx, a, b); }
    void final () { SHA256_final (&ctx); SHA256_digest (&ctx, digest); }
    byte *read () { return digest; }
};


#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif