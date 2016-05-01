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

#include <stdlib.h>
#include <string.h>
#include "SCBase.h"
#include "ByteCodeArray.h"
#include "Opcodes.h"

ByteCodes gCompilingByteCodes;
long totalByteCodes = 0;

void ByteCodesBase::push_back(const Byte& byte) {
	mByteCodes.push_back(byte);
}

void ByteCodesBase::push_back(ByteCodesRef inByteCodes) {
	for (auto& entry : inByteCodes->mByteCodes) {
		mByteCodes.push_back(entry);
	}
}

void ByteCodesBase::set_byte(size_t index, const Byte& byte) {
	if (SC_COND_ASSERT(index < mByteCodes.size())) {
		mByteCodes[index] = byte;
	}
}

size_t ByteCodesBase::length() const {
	return mByteCodes.size();
}

void ByteCodesBase::copy_to(Byte* byteArray) const {
	Byte* iterPtr = byteArray;
	for (auto& byte : mByteCodes) {
		*iterPtr = byte;
		iterPtr++;
	}
}

void initByteCodes()
{
	gCompilingByteCodes.reset(new ByteCodesBase);
}

int compileOpcode(long opcode, long operand1)
{
	int retc;
	if (operand1 <= 15) {
		compileByte((opcode<<4) | operand1);
		retc = 1;
	} else {
		compileByte(opcode);
		compileByte(operand1);
		if (opcode == opSendMsg || opcode == opSendSpecialMsg || opcode == opSendSuper) {
			// these expect numKeyArgsPushed to be passed.
			compileByte(0);
		}
		retc = 2;
	}
	return retc;
}

void compileJump(long opcode, long jumplen)
{
	compileByte((opSpecialOpcode<<4) | opcode);
	compileByte((jumplen >> 8) & 0xFF);
	compileByte(jumplen & 0xFF);
}

void compileByte(long byte)
{
	if (gCompilingByteCodes == NULL) {
		gCompilingByteCodes.reset(new ByteCodesBase);
	}
	
	totalByteCodes++;
	gCompilingByteCodes->push_back(byte);
}

int compileNumber(unsigned long value)
{
	compileByte((value >> 24) & 0xFF);
	compileByte((value >> 16) & 0xFF);
	compileByte((value >> 8) & 0xFF);
	compileByte(value & 0xFF);
	return 4;
}

int compileNumber24(unsigned long value)
{
	compileByte((value >> 16) & 0xFF);
	compileByte((value >> 8) & 0xFF);
	compileByte(value & 0xFF);
	return 4;
}

void compileAndFreeByteCodes(ByteCodes byteCodes)
{
	compileByteCodes(byteCodes);
	freeByteCodes(std::move(byteCodes));
}

void copyByteCodes(Byte *dest, ByteCodesRef byteCodes)
{
  byteCodes->copy_to(dest);
}

ByteCodes getByteCodes()
{
  ByteCodes	curByteCodes;

  curByteCodes = std::move(gCompilingByteCodes);
  SC_ASSERT(gCompilingByteCodes == NULL);

  return curByteCodes;
}

ByteCodes saveByteCodeArray()
{
	ByteCodes	curByteCodes;

	curByteCodes = std::move(gCompilingByteCodes);
	SC_ASSERT(gCompilingByteCodes == NULL);

	return curByteCodes;
}

void restoreByteCodeArray(ByteCodes byteCodes)
{
	SC_ASSERT(!gCompilingByteCodes);
	gCompilingByteCodes = std::move(byteCodes);
}

size_t byteCodeLength(ByteCodesRef byteCodes)
{
  if (!byteCodes) {
    return 0;
	} else {
		return byteCodes->length();
	}
}

/***********************************************************************
 *
 *	Internal routines.
 *
 ***********************************************************************/

void compileByteCodes(ByteCodesRef byteCodes)
{
  SC_ASSERT(byteCodes != NULL);

  totalByteCodes += byteCodes->length();
  gCompilingByteCodes->push_back(byteCodes);
}

ByteCodes allocByteCodes()
{
	ByteCodes	newByteCodes(new ByteCodesBase);
	return newByteCodes;
}

void freeByteCodes(ByteCodes byteCodes)
{
	SC_ASSERT(byteCodes != NULL);
	byteCodes.reset(); // memory is free'd here
}
