#include <stddef.h>
#include "array.hpp"
#include "../math/common.hpp"

namespace sic
{



const unsigned short VECTOR_MAX_SIZE = 32;
const unsigned short VECTOR_BIT_CHUNK_SIZE = VECTOR_MAX_SIZE >> 2;

unsigned int get_bit_chunk(size_t chunk)
{
  // constexpr unsigned short bit_mask = power(2, VECTOR_BIT_CHUNK_SIZE - 1) - 1;
};



template<typename T>
class VectorLeaf {

  Array<T> data;
};

template<typename T>
class VectorBranch {

  Array<VectorLeaf<T>> data;
};

/**
 * @brief immutable optimized vector, copy on write & implemented as
 * a bitmapped vector trie.
 * @note only the leaf notes hold the actual data, otherwise it's pointers
 */
template<typename T>
class Vector {

  union {
    VectorLeaf<T> leaf;
    VectorBranch<T> branch;
  } data;

  const T& operator[](size_t index)
  {

  }

};

}
