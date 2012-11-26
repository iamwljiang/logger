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
#include "logger.h"
#include <iostream>
#include <vector>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/syscall.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#endif
#define DEBUG_STR "this is test data use to test log DEBUG"
#define INFO_STR  "this is test data use to test log INFO"
#define NOTICE_STR "this is test data use to test log NOTICE"
#define WARN_STR  "this is test data use to test log WARN"
#define ERR_STR   "this is test data use to test log ERROR"
#define FATAL_STR "this is test data use to test log FATAL"
#define CRIT_STR  "this is test data use to test log CRIT"
#define EMERG_STR "this is test data use to test log EMERG"

void test_log(const char* level)
{
	Logger logger;
	logger.init(level,"run.log","./","./",NULL,NULL,false,false,1024*1024*5);
	logger.debug("%s" ,DEBUG_STR );
	logger.info("%s"  ,INFO_STR );
	logger.notice("%s" ,NOTICE_STR );
	logger.warn("%s"  ,WARN_STR);
	logger.error("%s" ,ERR_STR);
	logger.fatal("%s" ,FATAL_STR);
	logger.crit("%s"  ,CRIT_STR);
	logger.emerg("%s" ,EMERG_STR);
}
static Logger   multi_log;
void test_multi_log(const char* level)
{
	long thread_id = 0;
#ifdef WIN32 
	thread_id = GetCurrentThreadId();
#else
	thread_id = syscall(SYS_gettid);
#endif
	multi_log.debug("thread:%ld %s" ,thread_id,DEBUG_STR );
	multi_log.info("thread:%ld %s"  ,thread_id,INFO_STR );
	multi_log.notice("thread:%ld %s" ,thread_id,NOTICE_STR );
	multi_log.warn("thread:%ld %s"  ,thread_id,WARN_STR);
	multi_log.error("thread:%ld %s" ,thread_id,ERR_STR);
	multi_log.fatal("thread:%ld %s" ,thread_id,FATAL_STR);
	multi_log.crit("%thread:%ld s"  ,thread_id,CRIT_STR);
	multi_log.emerg("thread:%ld %s" ,thread_id,EMERG_STR);
}
void test_single_thread()
{
	long i = 0;
	for(; i < 10000; ++i){
		test_log("debug");
		test_log("info");
		test_log("notice");
		test_log("warn");
		test_log("err");
		test_log("error");
		test_log("fatal");
		test_log("crit");
		test_log("emerg");
	}
}
#ifdef WIN32
DWORD WINAPI thread_func(LPVOID pParameter)
#else
void* thread_func(void *data)
#endif
{
	long i = 0;
	for(; i < 5000; ++i){
		test_multi_log("debug");
		test_multi_log("info");
		test_multi_log("notice");
		test_multi_log("warn");
		test_multi_log("err");
		test_multi_log("error");
		test_multi_log("fatal");
		test_multi_log("crit");
		test_multi_log("emerg");
	}
#ifdef WIN32
	return 0;
#else
	return NULL;
#endif
}

void test_multi_thread(int n)
{
#ifdef WIN32
	std::vector<HANDLE> thread_pools;
#else
	std::vector<pthread_t> thread_pools;
#endif

	for(int i = 0; i < n; ++i){
#ifdef WIN32
		HANDLE handle = CreateThread(NULL,0,thread_func,NULL,0,NULL);
		thread_pools.push_back(handle);
#else
		pthread_t tid; 
		if(pthread_create(&tid,NULL,thread_func,NULL) == 0)
			thread_pools.push_back(tid);
#endif
	}

	for(size_t i = 0; i < thread_pools.size(); ++i){
#ifdef WIN32
		WaitForSingleObject(thread_pools.at(i),INFINITE);
		CloseHandle(thread_pools.at(i));
#else
		pthread_join(thread_pools.at(i),NULL);
		//int status;
	
		//waitpid(thread_pools.at(i),&status,WNOHANG);
#endif
	}
}
int main()
{	
	multi_log.init("info","multi_run.log","./","./",NULL,NULL,false,false,1024*1024*5);
	std::cout << "start test single thread\n";
	test_single_thread();
	std::cout << "test single thread end\n";
	std::cout << "start test multi thread\n";
	test_multi_thread(5);
	std::cout << "test multi thread end\n";
	return 0;
}
