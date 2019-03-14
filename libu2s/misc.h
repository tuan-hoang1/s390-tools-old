/*
 * Misc - Local helper functions
 *
 * Copyright IBM Corp. 2016
 */

#ifndef MISC_H
#define MISC_H

#define MIN(x, y)		\
({				\
	__typeof__(x) _x = (x);	\
	__typeof__(y) _y = (y);	\
				\
	_x < _y ? _x : _y;	\
})

size_t misc_strlcpy(char *, const char *, size_t);

#endif /* MISC_H */
