#
#	Copyright (c) 2014 Colorado State University
#
#     Permission is hereby granted, free of charge, to any person
#     obtaining a copy of this software and associated documentation
#     files (the "Software"), to deal in the Software without
#     restriction, including without limitation the rights to use,
#     copy, modify, merge, publish, distribute, sublicense, and/or
#     sell copies of the Software, and to permit persons to whom
#     the Software is furnished to do so, subject to the following
#     conditions:
#
#     The above copyright notice and this permission notice shall be
#     included in all copies or substantial portions of the Software.
#
#     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
#     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#     HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#     WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#     FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#     OTHER DEALINGS IN THE SOFTWARE.
#
#

PROG = mksim

CXX = g++
EXEC = simsearch
CXXFLAGS = -g -O2 -Wfatal-errors -Wshadow -Wall -std=c++11 -lstdc++ -L./HElib-2015-06-17/src/fhe.a -L./libtomcrypt/libtomcrypt.a -I./HElib-2015-06-17/src -I./ntl-9.2.0/include -I./libtomcrypt/src/headers -L/usr/local/lib -I/usr/include/boost
LDFLAGS = -lboost_system -lboost_filesystem -lboost_serialization -pthread -lgmp -lm ./HElib-2015-06-17/src/fhe.a ./libtomcrypt/libtomcrypt.a ./ntl-9.2.0/src/ntl.a

all:
	$(MAKE) -C src/ntl-9.2.0/src
	$(MAKE) -C src/HElib-2015-06-17/src
	$(MAKE) -C src/libtomcrypt
	$(MAKE) -C src

clean:
	$(MAKE) clobber -C src/ntl-9.2.0/src
	$(MAKE) clean -C src/HElib-2015-06-17/src
	$(MAKE) clean -C src/libtomcrypt
	$(MAKE) clean -C src
