/*
 * fd_byte_buffer.h
 *
 *  Created on: 12 Apr 2021
 *      Author: billy
 */

#ifndef CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_BYTE_BUFFER_H_
#define CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_BYTE_BUFFER_H_


#include <stdarg.h>
#include <stddef.h>

#include "typedefs.h"

/**
 * A datatype to allow an automatically growing buffer for appending
 * data to.
 * @ingroup utility_group
 */
typedef struct ByteBuffer
{
	/** @privatesection */
	char *bb_data_p;
	size_t bb_size;
	size_t bb_current_index;
} ByteBuffer;


/** @publicsection */

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * Allocate a ByteBuffer.
 *
 * @param initial_size The initial size of the data buffer that this ByteBuffer will have.
 * @return The newly-allocated ByteBuffer or <code>NULL</code> on error.
 * @memberof ByteBuffer
 */
ByteBuffer *AllocateByteBuffer (size_t initial_size);


/**
 * Free a ByteBuffer.
 *
 * @param buffer_p The ByteBuffer to free.
 * @memberof ByteBuffer
 */
void FreeByteBuffer (ByteBuffer *buffer_p);


/**
 * Resize a ByteBuffer.
 * The content of the data buffer will be preserved.
 *
 * @param buffer_p The ByteBuffer to resize.
 * @param new_size The new size of the ByteBuffer's data.
 * @return <code>true</code> if the resize was successful <code>false</code>
 * upon failure. If the call failed, the contents of the data buffer are preserved.
 * @memberof ByteBuffer
 */
bool ResizeByteBuffer (ByteBuffer *buffer_p, size_t new_size);


/**
 * Increase the size of a ByteBuffer's data buffer.
 * The content of the data buffer will be preserved.
 *
 * @param buffer_p The ByteBuffer to resize.
 * @param increment The amount to increase the ByteBuffer's data buffer.
 * @return <code>true</code> if the resize was successful <code>false</code>
 * upon failure. If the call failed, the contents of the data buffer are preserved.
 * @memberof ByteBuffer
 */
bool ExtendByteBuffer (ByteBuffer *buffer_p, size_t increment);


/**
 * Append some data to a ByteBuffer's data buffer.
 *
 * @param buffer_p The ByteBuffer whose data buffer the new data will be appended to.
 * @param data_p The data to append.
 * @param data_length The length, in bytes, of the data to append.
 * @return <code>true</code> if the append was successful <code>false</code>
 * upon failure. If the call failed, the contents of the data buffer are preserved.
 * @memberof ByteBuffer
 */
bool AppendToByteBuffer (ByteBuffer *buffer_p, const void *data_p, const size_t data_length);


/**
 * Append a string to a ByteBuffer's data buffer.
 *
 * @param buffer_p The ByteBuffer whose data buffer the string will be appended to.
 * @param value_s The <code>NULL</code> terminated string to append.
 * @return <code>true</code> if the append was successful <code>false</code>
 * upon failure. If the call failed, the contents of the data buffer are preserved.
 * @memberof ByteBuffer
 */
bool AppendStringToByteBuffer (ByteBuffer *buffer_p, const char * const value_s);


/**
 * Append a varargs array of strings to a ByteBuffer's data buffer.
 *
 * @param buffer_p The ByteBuffer whose data buffer the string will be appended to.
 * @param value_s The varags-style array of <code>NULL</code> terminated strings to append. The final entry
 * in this varags-array must be a <code>NULL</code>.
 * @return <code>true</code> if the append was successful <code>false</code>
 * upon failure. If the call failed, the contents of the data buffer are preserved.
 * @see AppendVarArgsToByteBuffer
 * @memberof ByteBuffer
 */
bool AppendStringsToByteBuffer (ByteBuffer *buffer_p, const char *value_s, ...);


/**
 * Append a va_list of strings to a ByteBuffer's data buffer.
 *
 * @param buffer_p The ByteBuffer whose data buffer the string will be appended to.
 * @param value_s The varargs-style array of <code>NULL</code> terminated strings to append. The final entry
 * in this varargs-array must be a <code>NULL</code>.
 * @param args The varargs list of arguments used by value_s.
 * @return <code>true</code> if the append was successful <code>false</code>
 * upon failure. If the call failed, the contents of the data buffer are preserved.
 * @memberof ByteBuffer
 */
bool AppendVarArgsToByteBuffer (ByteBuffer *buffer_p, const char *value_s, va_list args);


/**
 * Clear any data stored in a ByteBuffer.
 *
 *
 * @param buffer_p The ByteBuffer who will have all of its data emptied.
 * @memberof ByteBuffer
 */
void ResetByteBuffer (ByteBuffer *buffer_p);


/**
 * Get the remaining space in a ByteBuffer's data buffer.
 *
 * @param buffer_p The ByteBuffer to get the remaining space for.
 * @return The space remaining by the ByteBuffer in bytes.
 * @memberof ByteBuffer
 */
size_t GetRemainingSpaceInByteBuffer (const ByteBuffer * const buffer_p);


/**
 * Get the currently used size of a ByteBuffer's data buffer.
 *
 * @param buffer_p The ByteBuffer to get the space used for.
 * @return The space used by the ByteBuffer in bytes.
 * @memberof ByteBuffer
 */
size_t GetByteBufferSize (const ByteBuffer * const buffer_p);


/**
 * Get the data stored in a ByteBuffer.
 *
 * @param buffer_p The ByteBuffer to get the data from.
 * @return The data as valid c-style string.
 * @memberof ByteBuffer
 */
const char *GetByteBufferData (const ByteBuffer * const buffer_p);



/**
 * Get the data stored in a ByteBuffer and free a ByteBuffer
 *
 * @param buffer_p The ByteBuffer to get the data from which will be deallocated
 * after this call.
 * @return The data as valid c-style string.
 * @memberof ByteBuffer
 */
char *DetachByteBufferData (ByteBuffer * const buffer_p);



/**
 * Remove data from the end of a byte buffer
 *
 * @param buffer_p The ByteBuffer to remove the data from
 * @param size The number of bytes to remove. If this is greater than the size of the
 * ByteBuffer, then the ByteBUffer will be reset.
 * @memberof ByteBuffer
 */
void RemoveFromByteBuffer (ByteBuffer *buffer_p, size_t size);


/**
 * Replace each instance of a character within a ByteBuffer with another
 *
 * @param buffer_p The ByteBuffer to replace the character in.
 * @param old_data The character to be replaced.
 * @param new_data The replacement character.
 * @memberof ByteBuffer
 * @see ReplaceChars
 */
void ReplaceCharsInByteBuffer (ByteBuffer *buffer_p, char old_data, char new_data);


#ifdef __cplusplus
}
#endif



#endif /* CLIENTS_FRICTIONLESS_DATA_INCLUDE_FD_BYTE_BUFFER_H_ */
