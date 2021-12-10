# Increment Operators

The program will work because C++ will automatically split `x+++2` to `(x++) + 2`. The reason it does this, and not, say, `x + (++2)` is because the Increment (`++`) operator has higher precedence over the Addition (`+`) operator, and since the compiler reads the tokens left to right, it will properly split the operations.
