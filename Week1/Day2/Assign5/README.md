# Assignment 5 Response

You would measure a floating point number essentially by the amount of numbers following the decimal point. However, since a float is comprised of both the number before the decimal point and after the decimal point, you'll end up having a struggle of precision as your number get bigger, since the bigger your number is, the more bits it takes up in the binary representation of your number, and the less bits you have available for after the decimal point.

I have written a small program that adds 2^I (where I is the iteration) to a floating point value, you can see as you continue to add to the value, you lose numbers after the decimal point, until you're eventually out of a decimal point entirely!
