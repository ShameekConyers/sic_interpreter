#pragma once

#include "common.hpp"

#include <stdlib.h>

ssize_t grow_capacity(ssize_t capacity);

void* reallocate(void* ptr, size_t old_size, size_t new_size);

template<typename T>
T* grow_array(T* ptr, ssize_t old_count, ssize_t new_count);

template<typename T>
void free_array(T* ptr, ssize_t old_count);


// END DECLARATIONS

ssize_t grow_capacity(ssize_t capacity)
{
	return capacity < 8 ? 8 : (capacity) * 1.5;
}


void* reallocate(void* ptr, size_t old_size, size_t new_size)
{
	if (new_size == 0) {
		free(ptr);
		return nullptr;
	}

	void* result = realloc(ptr, new_size);
	if (result == 0) exit(1);
	return result;
}

template<typename T>
T* grow_array(T* ptr, ssize_t old_count, ssize_t new_count)
{
	return (T*)reallocate(ptr, sizeof(T) * old_count, sizeof(T) * new_count);
}

template<typename T>
void free_array(T* ptr, ssize_t old_count)
{
	reallocate(ptr, old_count, 0);
}
