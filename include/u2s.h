/*
 * File...........: u2s.h
 * Author(s)......: Gerhard Tonn <ton@de.ibm.com>
 *
 * Copyright IBM Corp. 2004,2007
 *
 * History of changes (starts July 2004)
 * 2004-07-02 initial
 */

#ifndef U2S_H
#define U2S_H

#define U2S_BUS_ID_SIZE    32

int u2s_getbusid(char *, char *);
int u2s_read_attribute(char *, char *, char *, size_t);
int u2s_get_host_access_count(char *);

#endif /* U2S_H */
