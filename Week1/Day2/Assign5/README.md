# Assignment 5 Response

The precision of a floating point number is directly tied to how many mantissa bits   you have, and what powers of 2 your number falls between, a formula for calculatin  g this would P<sub>2</sub>-P<sub>1</sub>/2<sup>M</sup> where P<sub>1</sub> is smaller power of 2 your number's between, P<sub>2</sub> is the bigger power of 2, and M is the amount of mantissa bits you have available. So in the case of 3.14, which is between 2<sup>1</sup> (2) and 2<sup>2</sup> (4) and with a 32bit float 23 bits are reserved to the mantissa.
Plugging all that in gets you 4-2/2<sup>23</sup> or 2/8388608, which gets you a precision of around `0.00000023841857910156`.

Since a float is comprised of both the number before the decimal point and after the decimal point, you'll end up having a struggle of precision as your number get bigger, since the bigger your number is, the more bits it takes up in the binary representation of your number, and the less bits you have available for after the decimal point.

I have written a small program that adds 2^I (where I is the iteration) to a floating point value, you can see as you continue to add to the value, you lose numbers after the decimal point, until you're eventually out of a decimal point entirely!
