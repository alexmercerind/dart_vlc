/*
 * Copyright (c) 2020, the Dart project authors.  Please see the AUTHORS file
 * for details. All rights reserved. Use of this source code is governed by a
 * BSD-style license that can be found in the LICENSE file.
 */

#ifndef API_DARTMANAGER_H_
#define API_DARTMANAGER_H_

#include "dart_api_dl.h"
#include "dart_version.h"
#include "internal/dart_api_dl_impl.h"

#include <string.h>

#define DART_API_DL_DEFINITIONS(name, R, A) name##_Type name##_DL = NULL;
DART_API_ALL_DL_SYMBOLS(DART_API_DL_DEFINITIONS)

#undef DART_API_DL_DEFINITIONS

typedef void* DartApiEntry_function;

DartApiEntry_function FindFunctionPointer(const DartApiEntry* entries,
                                          const char* name) {
  while (entries->name != NULL) {
    if (strcmp(entries->name, name) == 0) return (void*)entries->function;
    entries++;
  }
  return NULL;
}

intptr_t Dart_InitializeApiDL(void* data) {
  DartApi* dart_api_data = (DartApi*)data;
  if (dart_api_data->major != DART_API_DL_MAJOR_VERSION) {
    return -1;
  }
  const DartApiEntry* dart_api_function_pointers = dart_api_data->functions;
#define DART_API_DL_INIT(name, R, A) \
  name##_DL =                        \
      (name##_Type)(FindFunctionPointer(dart_api_function_pointers, #name));
  DART_API_ALL_DL_SYMBOLS(DART_API_DL_INIT)
#undef DART_API_DL_INIT
  return 0;
}

#endif
