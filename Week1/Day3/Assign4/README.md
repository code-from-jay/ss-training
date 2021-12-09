# Vector Performance

Using the `push_back()` method on a vector is more efficient than the `insert()` method because most of the time all it has to do is write a value to a spot in memory. Whereas with insert, it has to write the inserted value (or values) and also move all values that were after the position you supplied to it to make room and keep the continuous memory promise.

However since vectors have a capacity attached to them, both methods will need to reallocate their available memory if they go over their capacity, which means all the values need to be copied to the new memory chunk.

This means that for `push_back()`, it'll only ever reach it's worst case scenario if it has to reallocate, and any other time it's working in the best case scenario. Meaning it's O(1) at best case, and O(N) at worst case.

For `insert()` it's best case is if you use it like `push_back()` and insert an element at the end of the vector, and also if no allocation is required. Whereas the worst case would be if you were to insert in the middle of your vector OR if require reallocation, since this will force you to visit every single element of your vector.
