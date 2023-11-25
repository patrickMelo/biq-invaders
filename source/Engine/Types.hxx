/*
 * Source/Engine/Types.hxx
 *
 * This file is part of the Biq Invaders game source code.
 * Copyright 2023 Patrick Melo <patrick@patrickmelo.com.br>
 */

#ifndef BIQ_TYPES_HXX
#define BIQ_TYPES_HXX

// C/C++

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <atomic>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// We need at least a C++11 compiler.

#if ( __cplusplus < 201103L )
	#error "The compiler does not support the C++11 standard."
#endif

// Architectures

#if defined(i386) || defined(__i386) || defined(__i386__)
		#define ArchX86
		#define ArchName "x86"
	#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
			#define ArchX64
			#define ArchName "x86_64"
		#elif defined(__arm__)
				#define ArchARM
				#define ArchName "ARM"
			#else
				#error "Unsupported architecture."
			#endif

// Operating Systems

#if defined(__linux__) || defined(__linux) || defined(linux)
		#define LinuxOS
		#define OSName "Linux"
    #elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            #define WindowsOS
            #define OSName "Windows"
        #else
            #error "Unsupported operating system."
        #endif

// Platform Specific Definitions

#ifdef WindowsOS
	#ifdef _WIN32_WINNT
		#undef _WIN32_WINNT
	#endif

	#ifdef NTDDI_VERSION
		#undef NTDDI_VERSION
	#endif

	#define _WIN32_WINNT _WIN32_WINNT_WIN7
	#define NTDDI_VERSION NTDDI_WIN7

	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	extern "C" {
		#include <windows.h>
		#include <windowsx.h>
	}

	#undef INFO
	#undef WARNING
	#undef ERROR
	#undef DEBUG
	#undef TEXT
#endif

// Casts

#define UINT(value) static_cast<uint>(value)
#define U8(value) static_cast<u8>(value)
#define U16(value) static_cast<u16>(value)
#define U32(value) static_cast<u32>(value)
#define U64(value) static_cast<u64>(value)
#define I8(value) static_cast<i8>(value)
#define I16(value) static_cast<i16>(value)
#define I32(value) static_cast<i32>(value)
#define I64(value) static_cast<i64>(value)
#define F32(value) static_cast<f32>(value)
#define F64(value) static_cast<f64>(value)
#define DATA(value) reinterpret_cast<data>(value)

namespace Biq {

// Integer Types

typedef unsigned int	uint;
typedef uint8_t			u8;
typedef uint16_t		u16;
typedef uint32_t		u32;
typedef uint64_t		u64;

typedef int8_t	i8;
typedef int16_t	i16;
typedef int32_t	i32;
typedef int64_t	i64;

typedef	float	f32;
typedef double	f64;

// String Types

typedef unsigned char	uchar;
typedef char *			cstring;
typedef char const *	charconst;
typedef std::string		string;

// Pointer Types

typedef u8 *		data;
typedef void *		pointer;
typedef uintptr_t	intpointer;

// Compound Types

struct Vector2D {
	float x;
	float y;
};

struct Image {
    int width;
    int height;
    void* data;
};

struct GameInformation {
    cstring	name;
    uint    targetWidth;
    uint    targetHeight;
    uint    targetFPS;
	uint	maxWorldLayers;
};

} // namespace Biq

#endif // BIQ_TYPES_HXX