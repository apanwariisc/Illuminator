#undef TRACE_SYSTEM
#define TRACE_SYSTEM kmem

#if !defined(_TRACE_KMEM_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_KMEM_H

#include <linux/types.h>
#include <linux/tracepoint.h>
#include <trace/events/gfpflags.h>

DECLARE_EVENT_CLASS(kmem_alloc,

	TP_PROTO(unsigned long call_site,
		 const void *ptr,
		 size_t bytes_req,
		 size_t bytes_alloc,
		 gfp_t gfp_flags),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags),

	TP_STRUCT__entry(
		__field(	unsigned long,	call_site	)
		__field(	const void *,	ptr		)
		__field(	size_t,		bytes_req	)
		__field(	size_t,		bytes_alloc	)
		__field(	gfp_t,		gfp_flags	)
	),

	TP_fast_assign(
		__entry->call_site	= call_site;
		__entry->ptr		= ptr;
		__entry->bytes_req	= bytes_req;
		__entry->bytes_alloc	= bytes_alloc;
		__entry->gfp_flags	= gfp_flags;
	),

	TP_printk("call_site=%lx ptr=%p bytes_req=%zu bytes_alloc=%zu gfp_flags=%s",
		__entry->call_site,
		__entry->ptr,
		__entry->bytes_req,
		__entry->bytes_alloc,
		show_gfp_flags(__entry->gfp_flags))
);

DEFINE_EVENT(kmem_alloc, kmalloc,

	TP_PROTO(unsigned long call_site, const void *ptr,
		 size_t bytes_req, size_t bytes_alloc, gfp_t gfp_flags),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags)
);

DEFINE_EVENT(kmem_alloc, kmem_cache_alloc,

	TP_PROTO(unsigned long call_site, const void *ptr,
		 size_t bytes_req, size_t bytes_alloc, gfp_t gfp_flags),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags)
);

DECLARE_EVENT_CLASS(kmem_alloc_node,

	TP_PROTO(unsigned long call_site,
		 const void *ptr,
		 size_t bytes_req,
		 size_t bytes_alloc,
		 gfp_t gfp_flags,
		 int node),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags, node),

	TP_STRUCT__entry(
		__field(	unsigned long,	call_site	)
		__field(	const void *,	ptr		)
		__field(	size_t,		bytes_req	)
		__field(	size_t,		bytes_alloc	)
		__field(	gfp_t,		gfp_flags	)
		__field(	int,		node		)
	),

	TP_fast_assign(
		__entry->call_site	= call_site;
		__entry->ptr		= ptr;
		__entry->bytes_req	= bytes_req;
		__entry->bytes_alloc	= bytes_alloc;
		__entry->gfp_flags	= gfp_flags;
		__entry->node		= node;
	),

	TP_printk("call_site=%lx ptr=%p bytes_req=%zu bytes_alloc=%zu gfp_flags=%s node=%d",
		__entry->call_site,
		__entry->ptr,
		__entry->bytes_req,
		__entry->bytes_alloc,
		show_gfp_flags(__entry->gfp_flags),
		__entry->node)
);

DEFINE_EVENT(kmem_alloc_node, kmalloc_node,

	TP_PROTO(unsigned long call_site, const void *ptr,
		 size_t bytes_req, size_t bytes_alloc,
		 gfp_t gfp_flags, int node),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags, node)
);

DEFINE_EVENT(kmem_alloc_node, kmem_cache_alloc_node,

	TP_PROTO(unsigned long call_site, const void *ptr,
		 size_t bytes_req, size_t bytes_alloc,
		 gfp_t gfp_flags, int node),

	TP_ARGS(call_site, ptr, bytes_req, bytes_alloc, gfp_flags, node)
);

DECLARE_EVENT_CLASS(kmem_free,

	TP_PROTO(unsigned long call_site, const void *ptr),

	TP_ARGS(call_site, ptr),

	TP_STRUCT__entry(
		__field(	unsigned long,	call_site	)
		__field(	const void *,	ptr		)
	),

	TP_fast_assign(
		__entry->call_site	= call_site;
		__entry->ptr		= ptr;
	),

	TP_printk("call_site=%lx ptr=%p", __entry->call_site, __entry->ptr)
);

DEFINE_EVENT(kmem_free, kfree,

	TP_PROTO(unsigned long call_site, const void *ptr),

	TP_ARGS(call_site, ptr)
);

DEFINE_EVENT_CONDITION(kmem_free, kmem_cache_free,

	TP_PROTO(unsigned long call_site, const void *ptr),

	TP_ARGS(call_site, ptr),

	/*
	 * This trace can be potentially called from an offlined cpu.
	 * Since trace points use RCU and RCU should not be used from
	 * offline cpus, filter such calls out.
	 * While this trace can be called from a preemptable section,
	 * it has no impact on the condition since tasks can migrate
	 * only from online cpus to other online cpus. Thus its safe
	 * to use raw_smp_processor_id.
	 */
	TP_CONDITION(cpu_online(raw_smp_processor_id()))
);

TRACE_EVENT_CONDITION(mm_page_free,

	TP_PROTO(struct page *page, unsigned int order),

	TP_ARGS(page, order),


	/*
	 * This trace can be potentially called from an offlined cpu.
	 * Since trace points use RCU and RCU should not be used from
	 * offline cpus, filter such calls out.
	 * While this trace can be called from a preemptable section,
	 * it has no impact on the condition since tasks can migrate
	 * only from online cpus to other online cpus. Thus its safe
	 * to use raw_smp_processor_id.
	 */
	TP_CONDITION(cpu_online(raw_smp_processor_id())),

	TP_STRUCT__entry(
		__field(	unsigned long,	pfn		)
		__field(	unsigned int,	order		)
	),

	TP_fast_assign(
		__entry->pfn		= page_to_pfn(page);
		__entry->order		= order;
	),

	TP_printk("page=%p pfn=%lu order=%d",
			pfn_to_page(__entry->pfn),
			__entry->pfn,
			__entry->order)
);

TRACE_EVENT(mm_page_free_batched,

	TP_PROTO(struct page *page, int cold),

	TP_ARGS(page, cold),

	TP_STRUCT__entry(
		__field(	unsigned long,	pfn		)
		__field(	int,		cold		)
	),

	TP_fast_assign(
		__entry->pfn		= page_to_pfn(page);
		__entry->cold		= cold;
	),

	TP_printk("page=%p pfn=%lu order=0 cold=%d",
			pfn_to_page(__entry->pfn),
			__entry->pfn,
			__entry->cold)
);

TRACE_EVENT(mm_page_alloc,

	TP_PROTO(struct page *page, unsigned int order,
			gfp_t gfp_flags, int migratetype),

	TP_ARGS(page, order, gfp_flags, migratetype),

	TP_STRUCT__entry(
		__field(	unsigned long,	pfn		)
		__field(	unsigned int,	order		)
		__field(	gfp_t,		gfp_flags	)
		__field(	int,		migratetype	)
	),

	TP_fast_assign(
		__entry->pfn		= page ? page_to_pfn(page) : -1UL;
		__entry->order		= order;
		__entry->gfp_flags	= gfp_flags;
		__entry->migratetype	= migratetype;
	),

	TP_printk("page=%p pfn=%lu order=%d migratetype=%d gfp_flags=%s",
		__entry->pfn != -1UL ? pfn_to_page(__entry->pfn) : NULL,
		__entry->pfn != -1UL ? __entry->pfn : 0,
		__entry->order,
		__entry->migratetype,
		show_gfp_flags(__entry->gfp_flags))
);

DECLARE_EVENT_CLASS(mm_page,

	TP_PROTO(struct page *page, unsigned int order, int migratetype),

	TP_ARGS(page, order, migratetype),

	TP_STRUCT__entry(
		__field(	unsigned long,	pfn		)
		__field(	unsigned int,	order		)
		__field(	int,		migratetype	)
	),

	TP_fast_assign(
		__entry->pfn		= page ? page_to_pfn(page) : -1UL;
		__entry->order		= order;
		__entry->migratetype	= migratetype;
	),

	TP_printk("page=%p pfn=%lu order=%u migratetype=%d percpu_refill=%d",
		__entry->pfn != -1UL ? pfn_to_page(__entry->pfn) : NULL,
		__entry->pfn != -1UL ? __entry->pfn : 0,
		__entry->order,
		__entry->migratetype,
		__entry->order == 0)
);

DEFINE_EVENT(mm_page, mm_page_alloc_zone_locked,

	TP_PROTO(struct page *page, unsigned int order, int migratetype),

	TP_ARGS(page, order, migratetype)
);

TRACE_EVENT_CONDITION(mm_page_pcpu_drain,

	TP_PROTO(struct page *page, unsigned int order, int migratetype),

	TP_ARGS(page, order, migratetype),

	/*
	 * This trace can be potentially called from an offlined cpu.
	 * Since trace points use RCU and RCU should not be used from
	 * offline cpus, filter such calls out.
	 * While this trace can be called from a preemptable section,
	 * it has no impact on the condition since tasks can migrate
	 * only from online cpus to other online cpus. Thus its safe
	 * to use raw_smp_processor_id.
	 */
	TP_CONDITION(cpu_online(raw_smp_processor_id())),

	TP_STRUCT__entry(
		__field(	unsigned long,	pfn		)
		__field(	unsigned int,	order		)
		__field(	int,		migratetype	)
	),

	TP_fast_assign(
		__entry->pfn		= page ? page_to_pfn(page) : -1UL;
		__entry->order		= order;
		__entry->migratetype	= migratetype;
	),

	TP_printk("page=%p pfn=%lu order=%d migratetype=%d",
		pfn_to_page(__entry->pfn), __entry->pfn,
		__entry->order, __entry->migratetype)
);

TRACE_EVENT(mm_page_alloc_extfrag,

	TP_PROTO(struct page *page,
		int alloc_order, int fallback_order,
		int alloc_migratetype, int fallback_migratetype),

	TP_ARGS(page,
		alloc_order, fallback_order,
		alloc_migratetype, fallback_migratetype),

	TP_STRUCT__entry(
		__field(	unsigned long,	pfn			)
		__field(	int,		alloc_order		)
		__field(	int,		fallback_order		)
		__field(	int,		alloc_migratetype	)
		__field(	int,		fallback_migratetype	)
		__field(	int,		change_ownership	)
	),

	TP_fast_assign(
		__entry->pfn			= page_to_pfn(page);
		__entry->alloc_order		= alloc_order;
		__entry->fallback_order		= fallback_order;
		__entry->alloc_migratetype	= alloc_migratetype;
		__entry->fallback_migratetype	= fallback_migratetype;
		__entry->change_ownership	= (alloc_migratetype ==
					get_pageblock_migratetype(page));
	),

	TP_printk("page=%p pfn=%lu alloc_order=%d fallback_order=%d pageblock_order=%d alloc_migratetype=%d fallback_migratetype=%d fragmenting=%d change_ownership=%d",
		pfn_to_page(__entry->pfn),
		__entry->pfn,
		__entry->alloc_order,
		__entry->fallback_order,
		pageblock_order,
		__entry->alloc_migratetype,
		__entry->fallback_migratetype,
		__entry->fallback_order < pageblock_order,
		__entry->change_ownership)
);

TRACE_EVENT(def_alloc_free,

	TP_PROTO(unsigned long gp, int cpu, int total_objs, int alloc_count,
		const char *op, const char *slab),

	TP_ARGS(gp, cpu, total_objs, alloc_count, op, slab),

	TP_STRUCT__entry(
		__field(        unsigned long, gp              )
		__field(        int, cpu                )
		__field(        int, total_objs)
		__field(        int, alloc_count)
		__field(        const char *, op)
		__field(        const char *, slab)
	),

	TP_fast_assign(
		__entry->gp = gp;
		__entry->cpu = cpu;
		__entry->total_objs = total_objs;
		__entry->alloc_count = alloc_count;
		__entry->op = op;
		__entry->slab = slab;
	),

	TP_printk("gp=%ld cpu=%d total_objs=%d alloc_count=%d op=%s slab=%s",
		__entry->gp,
		__entry->cpu,
		__entry->total_objs,
		__entry->alloc_count,
		__entry->op,
		__entry->slab)
);

TRACE_EVENT(alloc_rate,

	TP_PROTO(unsigned long gp, int cpu, int p0, int p1, int p2, int alloc_count,
		int rate, int free_count, int f_rate, int total_objs, const char *slab),

	TP_ARGS(gp, cpu, p0, p1, p2, alloc_count, rate, free_count, f_rate,
			total_objs, slab),

	TP_STRUCT__entry(
		__field(        unsigned long, gp              )
		__field(        int, cpu                )
		__field(        int, p0)
		__field(        int, p1)
		__field(        int, p2)
		__field(        int, alloc_count)
		__field(        int, rate   )
		__field(        int, free_count)
		__field(        int, f_rate   )
		__field(        int, total_objs)
		__field(        const char *, slab)
	),

	TP_fast_assign(
		__entry->gp = gp;
		__entry->cpu = cpu;
		__entry->p0 = p0;
		__entry->p1 = p1;
		__entry->p2 = p2;
		__entry->alloc_count = alloc_count;
		__entry->rate = rate;
		__entry->free_count = free_count;
		__entry->f_rate = f_rate;
		__entry->total_objs = total_objs;
		__entry->slab = slab;
	),

	TP_printk("gp=%ld cpu=%d p0=%d p1=%d p2=%d alloc_count=%d rate=%d f_count=%d f_rate=%d total_objs=%d slab=%s",
		__entry->gp,
		__entry->cpu,
		__entry->p0,
		__entry->p1,
		__entry->p2,
		__entry->alloc_count,
		__entry->rate,
		__entry->free_count,
		__entry->f_rate,
		__entry->total_objs,
		__entry->slab)
);

TRACE_EVENT(new_slab,

	TP_PROTO(unsigned long gp, int cpu, unsigned int total_objs,
		unsigned long gp1, unsigned int def1,
		unsigned long gp2, unsigned int def2, const char *slab),

	TP_ARGS(gp, cpu, total_objs, gp1, def1, gp2, def2, slab),

	TP_STRUCT__entry(
		__field(        unsigned long, gp              )
		__field(        int, cpu                )
		__field(        unsigned int, total_objs   )
		__field(        unsigned long, gp1   )
		__field(        unsigned int, def1   )
		__field(        unsigned long, gp2   )
		__field(        unsigned int, def2   )
		__field(        const char *, slab)
	),

	TP_fast_assign(
		__entry->gp = gp;
		__entry->cpu = cpu;
		__entry->total_objs = total_objs;
		__entry->gp1 = gp1;
		__entry->def1 = def1;
		__entry->gp2 = gp2;
		__entry->def2 = def2;
		__entry->slab = slab;
	),

	TP_printk("gp=%ld cpu=%d total_objs=%d, gp1=%ld, def1=%d, gp2=%ld, def2=%d slab=%s",
		__entry->gp,
		__entry->cpu,
		__entry->total_objs,
		__entry->gp1,
		__entry->def1,
		__entry->gp2,
		__entry->def2,
		__entry->slab)
);

TRACE_EVENT(merge_page,

	TP_PROTO(unsigned long gp, int cpu, int was_frozen,
		void *prior, unsigned int inuse, const char *slab),

	TP_ARGS(gp, cpu, was_frozen, prior, inuse, slab),

	TP_STRUCT__entry(
		__field(        unsigned long, gp              )
		__field(        int, cpu                )
		__field(        int, was_frozen)
		__field(        void *, prior)
		__field(        unsigned int, inuse)
		__field(        const char *, slab)
	),

	TP_fast_assign(
		__entry->gp = gp;
		__entry->cpu = cpu;
		__entry->was_frozen = was_frozen;
		__entry->prior = prior;
		__entry->inuse= inuse;
		__entry->slab = slab;
	),

	TP_printk("gp=%ld cpu=%d was_frozen=%d, prior=0x%llx, inuse=%d, slab=%s",
		__entry->gp,
		__entry->cpu,
		__entry->was_frozen,
		__entry->prior,
		__entry->inuse,
		__entry->slab)
);

TRACE_EVENT(page_obj_stat,

	TP_PROTO(int node, unsigned int inuse, unsigned int def_w,
		unsigned int def_n, unsigned int objects, const char *slab),

	TP_ARGS(node, inuse, def_w, def_n, objects, slab),

	TP_STRUCT__entry(
		__field(        int, node)
		__field(        unsigned int, inuse)
		__field(        unsigned int, def_w   )
		__field(        unsigned int, def_n   )
		__field(        unsigned int, objects   )
		__field(        const char *, slab)
	),

	TP_fast_assign(
		__entry->node = node;
		__entry->inuse = inuse;
		__entry->def_w = def_w;
		__entry->def_n = def_n;
		__entry->objects = objects;
		__entry->slab = slab;
	),

	TP_printk("node=%d inuse=%d, def_w=%d, def_n=%d objects=%d, slab=%s",
		__entry->node,
		__entry->inuse,
		__entry->def_w,
		__entry->def_n,
		__entry->objects,
		__entry->slab)
);
#endif /* _TRACE_KMEM_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
