#include <linux/mm.h>
#include <linux/mmzone.h>
#include <linux/bootmem.h>
#include <linux/page_block.h>
#include <linux/memory.h>
#include <linux/vmalloc.h>
#include <linux/kmemleak.h>

unsigned long total_usage = 0;

void __meminit pgdat_page_block_init(struct pglist_data *pgdat)
{
	pgdat->node_page_block = NULL;
}

static int __init alloc_node_page_block(int nid)
{
	struct page_block *base;
	unsigned long table_size;
	unsigned long nr_pages;

	nr_pages = NODE_DATA(nid)->node_spanned_pages;
	if (!nr_pages)
		return 0;

	table_size = sizeof(struct page_block) * nr_pages;
	table_size = table_size/pageblock_nr_pages;

	base = memblock_virt_alloc_try_nid_nopanic(
		table_size, PAGE_SIZE, __pa(MAX_DMA_ADDRESS),
		BOOTMEM_ALLOC_ACCESSIBLE, nid);

	if (!base)
		return -ENOMEM;
	memset(base , 0, table_size);
	NODE_DATA(nid)->node_page_block = base;
	total_usage += table_size;
	return 0;
}

void __init page_block_init_flatmem(void)
{
	int nid, fail;

	for_each_online_node(nid) {
		fail = alloc_node_page_block(nid);
		if (fail)
			goto fail;
	}
	pr_info("allocated %ld bytes of page_block\n", total_usage);
	return;
fail:
	pr_crit("allocation of page_block failed.\n");
	panic("Out of memory");
}

struct page_block *lookup_page_block(struct page *page)
{
	unsigned long pfn = page_to_pfn(page) & ~(pageblock_nr_pages - 1);
	unsigned long offset;
	struct page_block *base;

	base = NODE_DATA(page_to_nid(page))->node_page_block;
	if (!base)
		return NULL;
	if (unlikely(pfn == 0))
		pfn = 1;
	offset = pfn-NODE_DATA(page_to_nid(page))->node_start_pfn;
	offset = offset/pageblock_nr_pages;
	return base+offset;
}
EXPORT_SYMBOL(lookup_page_block);

inline void update_pageblock_free(struct page_block *page_block, int nr_pages,
					int mtype, int operation)
{
	if (operation == OP_ALLOC) {
		page_block->free -= nr_pages;
		if (mtype < MIGRATE_MOVABLE)
			page_block->pages_kernel -= nr_pages;
	} else {
		page_block->free += nr_pages;
		if (mtype < MIGRATE_MOVABLE)
			page_block->pages_kernel += nr_pages;
	}
}

void update_page_block(struct page *page, int order, int mtype, int op)
{
	struct page_block *page_block;
	int nr_pages = 1 << order;
	unsigned long start = page_to_pfn(page);
	unsigned long end = start + nr_pages - 1;

	page_block = lookup_page_block(page);

	if (pfn_to_pb(start) == pfn_to_pb(end))
		update_pageblock_free(page_block, nr_pages, mtype, op);
	else {
		int first = pageblock_nr_pages - start % pageblock_nr_pages;

		update_pageblock_free(page_block, first, mtype, op);
		page_block++;
		update_pageblock_free(page_block, nr_pages-first, mtype, op);
	}
}

int pageblock_free_pages(struct page *page)
{
	struct page_block *page_block;

	page_block = lookup_page_block(page);
	return page_block->free;
}
