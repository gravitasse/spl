/*****************************************************************************\
 *  Copyright (C) 2007-2010 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Brian Behlendorf <behlendorf1@llnl.gov>.
 *  UCRL-CODE-235197
 *
 *  This file is part of the SPL, Solaris Porting Layer.
 *  For details, see <http://github.com/behlendorf/spl/>.
 *
 *  The SPL is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  The SPL is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with the SPL.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#ifndef _SPL_SUNDDI_H
#define _SPL_SUNDDI_H

#include <sys/cred.h>
#include <sys/uio.h>
#include <sys/sunldi.h>
#include <sys/mutex.h>
#include <sys/u8_textprep.h>
#include <sys/vnode.h>
#include <spl-device.h>
#include <sys/file.h>
#include <libkern/libkern.h>

typedef int ddi_devid_t;

#define	DDI_DEV_T_NONE				((dev_t)-1)
#define	DDI_DEV_T_ANY				((dev_t)-2)
#define	DI_MAJOR_T_UNKNOWN			((major_t)0)

#define	DDI_PROP_DONTPASS			0x0001
#define	DDI_PROP_CANSLEEP			0x0002

#define	DDI_SUCCESS				0
#define	DDI_FAILURE				-1

#define	ddi_prop_lookup_string(x1,x2,x3,x4,x5)	(*x5 = NULL)
#define	ddi_prop_free(x)			(void)0
#define	ddi_root_node()				(void)0

//extern int ddi_strtoul(const char *, char **, int, unsigned long *);
//extern int ddi_strtol(const char *, char **, int, long *);
//extern int ddi_strtoull(const char *, char **, int, unsigned long long *);
//extern int ddi_strtoll(const char *, char **, int, long long *);

#define  xcopyin( src, dst, size, flags)  copyin ((src), (dst), (size))
#define  xcopyout(src, dst, size, flags)  copyout((src), (dst), (size))


static inline int
ddi_strtol(const char *str, char **nptr, int base, long *result)
{
    *result = strtol(str, nptr, base);
    if (*result == 0)
        return (EINVAL);
    else if (*result == LONG_MIN || *result == LONG_MAX)
        return (ERANGE);
    return (0);
}

static inline int
ddi_strtoul(const char *str, char **nptr, int base, unsigned long *result)
{
    *result = strtoul(str, nptr, base);
    if (*result == 0)
        return (EINVAL);
    else if (*result == ULONG_MAX)
        return (ERANGE);
    return (0);
}

static inline int
ddi_strtoull(const char *str, char **nptr, int base, unsigned long long *result)
 {
     *result = (unsigned long long)strtouq(str, nptr, base);
     if (*result == 0)
         return (EINVAL);
     else if (*result == ULLONG_MAX)
         return (ERANGE);
     return (0);
 }


#ifndef OTYPCNT
#define	OTYPCNT		5
#define	OTYP_BLK	0
#define	OTYP_MNT	1
#define	OTYP_CHR	2
#define	OTYP_SWP	3
#define	OTYP_LYR	4
#endif

#define	P2END(x, align)			(-(~(x) & -(align)))

#define ddi_name_to_major(name) devsw_name2blk(name, NULL, 0)

struct dev_info {
    dev_t dev;   // Major / Minor
    void *devc;
    void *devb;
};
typedef struct dev_info dev_info_t;


int	ddi_strtoul(const char *, char **, int, unsigned long *);
int	ddi_strtol(const char *, char **, int, long *);
int	ddi_soft_state_init(void **, size_t, size_t);
int	ddi_soft_state_zalloc(void *, int);
void	*ddi_get_soft_state(void *, int);
void	ddi_soft_state_free(void *, int);
void	ddi_soft_state_fini(void **);
int	ddi_create_minor_node(dev_info_t *, char *, int,
                              minor_t, char *, int);
void	ddi_remove_minor_node(dev_info_t *, char *);

int ddi_driver_major(dev_info_t *);

typedef void 	*ldi_ident_t;

#define	DDI_SUCCESS	0
#define	DDI_FAILURE	-1

#define	DDI_PSEUDO	""

#define	ddi_prop_update_int64(a, b, c, d)	DDI_SUCCESS
#define	ddi_prop_update_string(a, b, c, d)	DDI_SUCCESS

#define	bioerror(bp, er)	(buf_seterror((bp), (er)))
#define biodone(bp) buf_biodone(bp)

#define ddi_ffs ffs
static inline long ddi_fls(long mask) {      \
    /*Algorithm courtesy of Steve Chessin.*/ \
    while (mask) {                           \
		long nx;                             \
		if ((nx = (mask & (mask - 1))) == 0) \
			break;                           \
		mask = nx;                           \
	}                                        \
	return (ffs(mask));                      \
}

#define getminor(X) minor((X))



/*
 * This data structure is entirely private to the soft state allocator.
 */
struct i_ddi_soft_state {
	void		**array;	/* the array of pointers */
	kmutex_t	lock;	/* serialize access to this struct */
	size_t		size;	/* how many bytes per state struct */
	size_t		n_items;	/* how many structs herein */
	struct i_ddi_soft_state *next;	/* 'dirty' elements */
};

#define	MIN_N_ITEMS	8	/* 8 void *'s == 32 bytes */



#endif /* SPL_SUNDDI_H */
