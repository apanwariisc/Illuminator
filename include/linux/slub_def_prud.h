#ifndef _LINUX_SLUB_DEF_P_H
#define _LINUX_SLUB_DEF_P_H

struct gp_cache_data{
	void **freelist;
	unsigned long gp_seq; /* Required to know the safe free time of objects */
	void *last;		/* Pointer to last object */
	unsigned def_count;
};

enum gp_cache_index {
	C_WAIT,
	C_NEXT };

#endif /* _LINUX_SLUB_DEF_P_H */
