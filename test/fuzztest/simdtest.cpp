// Tencent is pleased to support the open source community by making RapidJSON available.
// 
// Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip. All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed 
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
// CONDITIONS OF ANY KIND, either express or implied. See the License for the 
// specific language governing permissions and limitations under the License.

// Since Travis CI installs old Valgrind 3.7.0, which fails with some SSE4.2
// The unit tests prefix with SIMD should be skipped by Valgrind test

// __SSE2__ and __SSE4_2__ are recognized by gcc, clang, and the Intel compiler.
// We use -march=native with gmake to enable -msse2 and -msse4.2, if supported.
#if defined(__SSE4_2__)
#  define RAPIDJSON_SSE42
#elif defined(__SSE2__)
#  define RAPIDJSON_SSE2
#elif defined(__ARM_NEON)
#  define RAPIDJSON_NEON
#endif

#define RAPIDJSON_NAMESPACE rapidjson_simd

#include <fuzz_test.h>

#include "rapidjson/document.h"

#ifdef __GNUC__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(effc++)
#endif

using namespace rapidjson_simd;

FUZZ_TEST(SIMDDocument, Parse, const uint8_t * Data, size_t Size)
{
    std::string data{(const char *) Data, Size};
    Document doc;
    doc.Parse(data);
}

#ifdef __GNUC__
RAPIDJSON_DIAG_POP
#endif
