#ifndef ENDIANESS_H
#define ENDIANESS_H
/**
* LICENSE
* 
* BSD 3-Clause License

Copyright (c) 2023, Sonu Aryan

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifdef __GNUC__
    // code for GCC and related compilers
	#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    // code for big-endian systems
		#define MBIG_ENDIAN
	#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		// code for little-endian systems
		#define MLITTLE_ENDIAN
	#endif

#elif defined(_MSC_VER)
    // code for Microsoft Visual C++
#if defined(_M_IX86) || defined(_M_AMD64) || defined(_M_IA64) || defined(_M_ARM) || defined(_M_ARM_NT) || defined(_M_ARM64)
	#define MLITTLE_ENDIAN
#elif defined(_M_MRX000) || defined(_MIPS_)
	#ifdef _MIPSEL
		#define MLITTLE_ENDIAN
	#else
		#define MBIG_ENDIAN
	#endif

#elif defined(_M_PPC) || defined(_M_PPCBE)
	#define MBIG_ENDIAN
#endif


	
#elif defined(__clang__)
    // code for Clang
	// code for GCC and related compilers
	#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    // code for big-endian systems
		#define MBIG_ENDIAN
	#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		// code for little-endian systems
		#define MLITTLE_ENDIAN
	#endif
	
#elif defined(__INTEL_COMPILER)
    // code for Intel C++ Compiler
	#ifdef _BIG_ENDIAN
    // code for big-endian systems
		#define MBIG_ENDIAN
	#else
		// code for little-endian systems
		#define MLITTLE_ENDIAN
	#endif

#elif defined(__IBMC__) || defined(__IBMCPP__)
    // code for IBM XL C/C++ Compiler
	#ifdef _LITTLE_ENDIAN
    // code for little-endian systems
		#define MLITTLE_ENDIAN
	#else
		// code for big-endian systems
		#define MBIG_ENDIAN
	#endif

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
    #ifdef __LITTLE_ENDIAN
    // code for little-endian systems
		#define MLITTLE_ENDIAN
	#else
		// code for big-endian systems
		#define MBIG_ENDIAN
	#endif

#elif defined(__TINYC__)
    // code for Tiny C Compiler (TCC)
	#error "Not available yet"
#else
    #error Unsupported compiler
#endif

#endif