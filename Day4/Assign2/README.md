# Truth Table For Cast Operators

|           | bool  | char  | int   | float |
| --------- | ----- | ----- | ----- | ----- |
| **bool**  | int   | int   | int   | float |
| **char**  | int   | int   | int   | float |
| **int**   | int   | int   | int   | float |
| **float** | float | float | float | float |

As you can see, it'll implicitly cast the result to int, unless a float is involved, in which case it'll cast the result to a float as well.

To confirm this, you can compile the include program. It will fail during linking, and conviently with templates, it'll tell you what type was passed in, that way you can see what the compiler implicitly casted to.
