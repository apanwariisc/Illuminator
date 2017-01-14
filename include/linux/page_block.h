#define OP_ALLOC	1
#define OP_FREE		2
#define pfn_to_pb(pfn)	(pfn/pageblock_nr_pages)

struct pglist_data;
struct page_block {
	int free;
	int pages_kernel;
};

extern void pgdat_page_block_init(struct pglist_data *pgdat);
extern void page_block_init_flatmem(void);
struct page_block *lookup_page_block(struct page *page);
void update_page_block(struct page *page, int order, int mtype, int op);
int pageblock_free_pages(struct page *page);
