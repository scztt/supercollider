/*
	SuperCollider real time audio synthesis system
    Copyright (c) 2002 James McCartney. All rights reserved.
	http://www.audiosynth.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef LANG_BYTECODEARRAY_H
#define LANG_BYTECODEARRAY_H

#include "InitAlloc.h"
#include "SC_Assert.h"
#include <deque>

template <class T>
class CompilePoolAllocator {
public:
  typedef T value_type;
  
  CompilePoolAllocator() {}
  
  template <class U>
  CompilePoolAllocator(const CompilePoolAllocator<U>& other) {}
  
  T* allocate(std::size_t num)
  {
    T* newObj = (T*)pyr_pool_compile->Alloc(sizeof(T) * num);
    MEMFAIL(newObj);
    return newObj;
  }
  
  void deallocate(T* p, std::size_t n)
  {
    if (SC_COND_ASSERT(p != NULL)) {
      pyr_pool_compile->Free(p);
    }
  }
};

// ByteCodes ownership:
//	ByteCodesBase is always accessed via a unique_ptr, ByteCodes. ByteCodes objects can have only one
//  owner at a time. std::move always signifies a transfer of ownership, and must be used for functions
//  which accept a ByteCodes as an argument. After an std::move(byteCodes), byteCodes is NULL and
//  is no longer accessible. ByteCodesRef represents a non-ownership-transferring argument - no std::move
//  is required in this case.
class ByteCodesBase;
typedef unsigned char																	Byte;
typedef std::unique_ptr<ByteCodesBase>									ByteCodes;
typedef const ByteCodes&																ByteCodesRef; // ownership retained by caller
typedef std::deque<Byte, CompilePoolAllocator<Byte> >	ByteArray;

class ByteCodesBase {
public:
	void		push_back(const Byte& byte);
	
					// append bytecodes onto the end of this collection
	void		push_back(ByteCodesRef inByteCodes);
	
	void		set_byte(size_t index, const Byte& byte);

	size_t	length() const;

	void		copy_to(Byte* byteArray) const;

private:
  ByteArray mByteCodes;
};

extern ByteCodes gCompilingByteCodes;
extern long totalByteCodes;

void initByteCodes();
void compileByte(long byte);
void compileAndFreeByteCodes(ByteCodes byteCodes);
void copyByteCodes(Byte *dest, ByteCodesRef byteCodes);
ByteCodes getByteCodes();
ByteCodes saveByteCodeArray();
void restoreByteCodeArray(ByteCodes byteCodes);
size_t byteCodeLength(ByteCodesRef byteCodes);
void compileByteCodes(ByteCodesRef byteCodes);
ByteCodes allocByteCodes();
void reallocByteCodes(ByteCodes byteCodes);
void freeByteCodes(ByteCodes byteCodes);
int compileOpcode(long opcode, long operand1);
void compileJump(long opcode, long jumplen);
int compileNumber(unsigned long value);
int compileNumber24(unsigned long value);

#endif
