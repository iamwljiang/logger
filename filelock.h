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
/***************************************************************************
                          filelock.h  -  description
                             -------------------
    begin                : Fri Jul 20 2001
    copyright            : (C) 2001 by Mark
    email                : alben@yeah.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WIN32
#ifndef FILELOCK_H
#define FILELOCK_H

#include <unistd.h>
#include <fcntl.h>


class CFileLock
{
protected:
	int m_iFd;
	
protected:
	//return false when interupt by singal
	bool Fcntl(int iCmd, int iType, int iOffset, int iLen, int iWhence);
	
public:
	CFileLock(const char* sPath);
	virtual ~CFileLock();
	
	//return false when interupt by singal	
	inline bool ReadLock(int iOffset = 0, int iLen = 1, int iWhence = SEEK_SET)
			{ return Fcntl(F_SETLK, F_RDLCK, iOffset, iLen, iWhence); }
	inline bool WriteLock(int iOffset = 0, int iLen = 1, int iWhence = SEEK_SET)
			{ return Fcntl(F_SETLK, F_WRLCK, iOffset, iLen, iWhence); }	
	inline bool ReadLockW(int iOffset = 0, int iLen = 1, int iWhence = SEEK_SET)
			{ return Fcntl(F_SETLKW, F_RDLCK, iOffset, iLen, iWhence); }	
	inline bool WriteLockW(int iOffset = 0, int iLen = 1, int iWhence = SEEK_SET)
			{ return Fcntl(F_SETLKW, F_WRLCK, iOffset, iLen, iWhence); }
	inline bool Unlock(int iOffset = 0, int iLen = 1, int iWhence = SEEK_SET)
			{ return Fcntl(F_SETLK, F_UNLCK, iOffset, iLen, iWhence); }
};

#endif
#endif