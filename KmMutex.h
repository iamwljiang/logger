/*
 * Copyright (c) 2012, chlaws <iamwljiang at gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TEST_KMMUTEX_H_
#define _TEST_KMMUTEX_H_

#ifdef WIN32
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#endif

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#endif

class KmMutex {
  public:
    inline KmMutex() {
#ifdef WIN32
        InitializeCriticalSection(&_mutex);
#else
        pthread_mutex_init(&_mutex, NULL);
#endif
    }

    inline ~KmMutex() {
#ifdef WIN32
        DeleteCriticalSection(&_mutex);
#else
        pthread_mutex_destroy(&_mutex);
#endif
    }

    inline void lock() {
#ifdef WIN32
        EnterCriticalSection(&_mutex);
#else
        pthread_mutex_lock(&_mutex);
#endif
    }

    inline void unlock() {
#ifdef WIN32
        LeaveCriticalSection(&_mutex);
#else
        pthread_mutex_unlock(&_mutex);
#endif
    }

  private:

    KmMutex(const KmMutex &);

    const KmMutex & operator = (const KmMutex &);

  private:

#ifdef WIN32
    CRITICAL_SECTION    _mutex;
#else
    pthread_mutex_t     _mutex;
#endif
};


class KmScopedLock {
  public:
    inline KmScopedLock(KmMutex & mutex) : _mutex(mutex) {
        _mutex.lock();
    }

    inline ~KmScopedLock() {
        _mutex.unlock();
    }

  private:

    KmScopedLock(const KmScopedLock &);

    const KmScopedLock & operator = (const KmScopedLock &);

  private:

    KmMutex & _mutex;
};

#endif  // _TEST_KMMUTEX_H_
