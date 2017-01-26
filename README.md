# "wcs" Stream Meter

This small tool by [Arno Wagner](http://www.arnowagner.info/tools/index.html) can be put between commands and prints the number of bytes piped through it to STDOUT. The name is derived from "wc stream", although it does not count words or lines.
Very useful for things like progress reporting, and a lot easier to use than the similar functionality of "dd". 
Very simple compile instructions at the start of the source file.
Example output:

```sh
root@gate:~# cat /dev/zero | wcs > /dev/null
 read:   6.717 GB [   6717444096 B]    avg:   1.120 GB/sec [     6 sec]
```

## Building

Simply run `make install` to install to the default prefix (/usr/local).

You can also install to a custom prefix by executing
```sh
make install prefix=YOUR_CUSTOM_PREFIX
```
