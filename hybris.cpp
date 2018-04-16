/*
 * Copyright (C) 2014-2015 Jolla Ltd.
 * Copyright (C) 2018 Matti Lehtimäki.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authored by: Mohammed Hassan <mohammed.hassan@jolla.com>
 * Authored by: Matti Lehtimäki <matti.lehtimaki@gmail.com>
 */

#include "droidgnss.h"
#include <dlfcn.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LIB_DROID_GNSS_PATH
#define LIB_DROID_GNSS_PATH "/usr/libexec/droid-hybris/system/lib/libdroidgnss.so"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void *android_dlopen(const char *name, int flags);
void *android_dlsym(void *handle, const char *name);

#ifdef __cplusplus
};
#endif

static void *__handle = NULL;

static inline void __load_library() {
  if (!__handle) {
    __handle = android_dlopen(LIB_DROID_GNSS_PATH, RTLD_NOW);
    if (!__handle) {
      // calling abort() is bad but it does not matter anyway as we will crash.
      abort();
    }
  }
}

static inline void *__resolve_sym(const char *sym)
{
  __load_library();

  void *ptr = android_dlsym(__handle, sym);
  assert(ptr != NULL);
  if (!ptr) {
    // calling abort() is bad but it does not matter anyway as we will crash.
    abort();
  }

  return ptr;
}

#define HYBRIS_WRAPPER_1_0(ret,sym)		    \
  ret sym() {					    \
    static ret (* _sym)() = NULL;		    \
    if (!_sym)					    \
      *(void**)&_sym = __resolve_sym(#sym);		    \
    return _sym();				    \
  }						    \

#define HYBRIS_WRAPPER_1_1(ret,arg0,sym)     \
  ret sym(arg0 _arg0) {			     \
    static ret (* _sym)(arg0) = NULL;	     \
    if (!_sym)				     \
      _sym = __resolve_sym(#sym);	     \
    return _sym(_arg0);			     \
  }					     \

#define HYBRIS_WRAPPER_1_2(ret,arg0,arg1,sym)			     \
  ret sym(arg0 _arg0, arg1 _arg1) {				     \
    static ret (* _sym)(arg0, arg1) = NULL;			     \
    if (!_sym)							     \
      _sym = __resolve_sym(#sym);				     \
    return _sym(_arg0,_arg1);					     \
  }								     \

#define HYBRIS_WRAPPER_1_3(ret,arg0,arg1,arg2,sym)		     \
  ret sym(arg0 _arg0, arg1 _arg1, arg2 _arg2) {			     \
    static ret (* _sym)(arg0, arg1, arg2) = NULL;		     \
    if (!_sym)							     \
      _sym = __resolve_sym(#sym);				     \
    return _sym(_arg0,_arg1, _arg2);				     \
  }								     \

#define HYBRIS_WRAPPER_1_4(ret,arg0,arg1,arg2,arg3,sym)		     \
  ret sym(arg0 _arg0, arg1 _arg1, arg2 _arg2, arg3 _arg3) {	     \
    static ret (* _sym)(arg0, arg1, arg2, arg3) = NULL;		     \
    if (!_sym)							     \
      _sym = __resolve_sym(#sym);				     \
    return _sym(_arg0,_arg1, _arg2, _arg3);			     \
  }								     \

#define HYBRIS_WRAPPER_1_6(ret,arg0,arg1,arg2,arg3,arg4,arg5,sym)	\
  ret sym(arg0 _arg0, arg1 _arg1, arg2 _arg2, arg3 _arg3, arg4 _arg4, arg5 _arg5) { \
    static ret (* _sym)(arg0, arg1, arg2, arg3, arg4, arg5) = NULL;	\
    if (!_sym)								\
      _sym = __resolve_sym(#sym);					\
    return _sym(_arg0,_arg1, _arg2, _arg3, _arg4, _arg5);		\
  }									\

#define HYBRIS_WRAPPER_1_7(ret,arg0,arg1,arg2,arg3,arg4,arg5,arg6,sym)	\
  ret sym(arg0 _arg0, arg1 _arg1, arg2 _arg2, arg3 _arg3, arg4 _arg4, arg5 _arg5, arg6 _arg6) { \
    static ret (* _sym)(arg0, arg1, arg2, arg3, arg4, arg5, arg6) = NULL; \
    if (!_sym)								\
      _sym = __resolve_sym(#sym);					\
    return _sym(_arg0,_arg1, _arg2, _arg3, _arg4, _arg5, _arg6);	\
  }									\

#define HYBRIS_WRAPPER_0_4(arg0,arg1,arg2,arg3,sym)		     \
  void sym(arg0 _arg0, arg1 _arg1, arg2 _arg2, arg3 _arg3) {	     \
    static void (* _sym)(arg0, arg1, arg2, arg3) = NULL;	     \
    if (!_sym)							     \
      _sym = __resolve_sym(#sym);				     \
    _sym(_arg0,_arg1, _arg2, _arg3);				     \
  }								     \

#define HYBRIS_WRAPPER_0_1(arg0,sym)				     \
  void sym(arg0 _arg0) {					     \
    static void (* _sym)(arg0) = NULL;				     \
    if (!_sym)							     \
      _sym = __resolve_sym(#sym);				     \
    _sym(_arg0);						     \
  }								     \

#define HYBRIS_WRAPPER_0_2(arg0,arg1,sym)				\
  void sym(arg0 _arg0, arg1 _arg1) {					\
    static void (* _sym)(arg0, arg1) = NULL;				\
    if (!_sym)								\
      _sym = __resolve_sym(#sym);					\
    _sym(_arg0, _arg1);							\
  }									\

#define HYBRIS_WRAPPER_0_0(sym)						\
  void sym() {								\
    static void (* _sym)() = NULL;					\
    if (!_sym)								\
      *(void**)&_sym = __resolve_sym(#sym);					\
    _sym();								\
  }									\

#define HYBRIS_WRAPPER_0_3(arg0,arg1,arg2,sym)			     \
  void sym(arg0 _arg0, arg1 _arg1, arg2 _arg2) {		     \
    static void (* _sym)(arg0, arg1, arg2) = NULL;		     \
    if (!_sym)							     \
      _sym = __resolve_sym(#sym);				     \
    _sym(_arg0,_arg1, _arg2);					     \
  }								     \

HYBRIS_WRAPPER_0_0(droid_gnss_init);
HYBRIS_WRAPPER_0_0(droid_gnss_deinit);
HYBRIS_WRAPPER_1_0(DroidGnss *,droid_gnss_get_hal);
