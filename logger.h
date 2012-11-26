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

#ifndef __LOGGER_H__
#define __LOGGER_H__
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include <stdarg.h>
#include<time.h>
#include "KmMutex.h"

namespace LOG_LEVEL{
const int DEBUG = 1;
const int INFO  = 2;
const int NOTICE= 4;
const int WARN  = 8;
const int ERR   = 16;
//const int ERROR = 16;
const int CRIT  = 32;
const int FATAL = 64;
const int EMERG = 128;
};

#if defined(WIN32) || defined(WIN64)
#define strcasecmp _stricmp
#pragma warning(disable:4996)
#endif

#if defined(WIN32) || defined(WIN64)
#include <io.h>
#define ACCESS(file) _access(file,0)
#else
#include <unistd.h>
#define ACCESS(file) access(file,F_OK)
#endif

const size_t MAX_PATH_LEN = 256;
const size_t MAX_BUFFER_SIZE = 8192;
const long MAX_LOG_SIZE = 1024*1024*10;

#ifndef gid_t
#define gid_t unsigned int
#endif

#ifndef uid_t
#define uid_t unsigned int
#endif

//defualt not used
#ifdef PROC_LOG_LOCK
#include "filelock.h"
#define   FILE_LOCK_FILEPATH "/dev/shm/log_file_lock.txt"
#endif


class Logger{
public:
	Logger();

	~Logger();

	//flush if set,flush every line when you write log to file
	//split have two way:
	//1.if you point daysplit,that per day have one log file
	//2.if you not point daysplit,that when logfile size greater max_size,we split file
	//log level ignoring case,can be "debug","info","warn","notice","error","fatal","crit","emerg"
	int 		init(const char* log_level,const char* log_name,
					const char *log_path="./",const char* mov_path="./",
					const char *uid_str=NULL, const char* gid_str =NULL,
					bool  flush=false,bool daysplit=false,long max_size=MAX_LOG_SIZE);

	int 		log(int log_level,const char*,...);
	
	int			error(const char*,...);

	int			info (const char*,...);

	int			debug(const char*,...);

	int			warn (const char*,...);

	int			notice(const char*,...);

	int			fatal(const char*,...);

	int			emerg(const char*,...);

	int			crit (const char*,...);

private://function
	void		op_error_of_what(const char*,...); 

	void		get_header_line(int log_level,char *line,int line_size);

	bool		open_log_file();	

private://data
	FILE*		log_file_ptr;

	char		header_line[MAX_PATH_LEN];
	
	char		buffer[MAX_BUFFER_SIZE];
		
	char		log_name[MAX_PATH_LEN];
	
	char		log_path[MAX_PATH_LEN];

	char		mov_path[MAX_PATH_LEN];

	KmMutex     log_mutex;
	
	bool 		flush;
	
	bool		daysplit;

	bool		inited;
	
	int			log_level;

	long		log_max_size;
	//get gid & uid use call process to get default value if uid and gid not set
	gid_t		gid;

	uid_t		uid;

#ifndef WIN32
#ifdef  PROC_LOG_LOCK
	CFileLock		*log_file_lock;
#endif
#endif

};
#endif
