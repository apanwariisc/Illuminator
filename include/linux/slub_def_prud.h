#ifndef _LINUX_SLUB_DEF_P_H
#define _LINUX_SLUB_DEF_P_H

struct gp_cache_data{
	void **freelist;
	unsigned long def_count;
	unsigned long gp_seq; /* Required to know the safe free time of objects */
	void *last;		/* Pointer to last object */
};

enum gp_cache_index {
	C_NEXT,
	C_WAIT };

#endif /* _LINUX_SLUB_DEF_P_H */
