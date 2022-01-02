## Calendar Assignment

The application is able to show accurate calendars since before the Epoch listed in the standard -- [January 1st, 1970](https://www.cplusplus.com/reference/ctime/time/) -- at least so long as it's built with clang, since `time`, and more specifically for our cases, `time_t` is implementation specific. So in the case of clang, the reason it's able to show accurate calendars since before the Epoch is due to having a *signed* integral value for `time_t`. This allows `mktime` to essentially calculate the days in reverse.

With that in mind, you could technically make a calendar without using any of the time methods provided by the C standard library. So long as have a 'reference day', you can work backwards, or forwards to see what weekday a day lands on, of course making sure to keep track of leap years.
