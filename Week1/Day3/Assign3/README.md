# Array vs Vector

Arrays and Vectors have their own strengths and weaknesses.

An array is very useful for when you *know* how many elements you'll need to store, and that you won't need to add more.In this instance you won't need all of the extra features of std::vector, nor the overhead that comes along with it.

This is especially useful with small arrays, since the extra data from the vector could end up doubling your memory usage, as seen in the included program.


However, when you need to store an unknown amount of elements whose size can change at any time, vectors are the real winners, as the class's extra methods allow for safe and automatically managed reallocation of memory. They additionally deallocate automatically unlike if you were to try to create a dynamic array on the heap.

Additionally, vectors provide bounds checking, a feature not included with normal arrays, this will stop you from unintentionally writing values to memory not owned by your array, like you can also see in the included program.
