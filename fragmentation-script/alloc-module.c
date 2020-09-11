#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gfp.h>
#include <linux/mmzone.h>
#include <asm/pgtable.h>
#include <linux/page-flags.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#define NUM_THREADS		(1)

int iteration = 1;
module_param(iteration, int, 0);

int MB_PER_THREAD;
int ALLOC_PAGES_PER_THREAD;
int FREE_PAGES_PER_THREAD;

struct completion completed[NUM_THREADS];

void initialize_global_variables(void)
{
	if (iteration == 1) {
		MB_PER_THREAD = 1000;
		ALLOC_PAGES_PER_THREAD = MB_PER_THREAD * 256;
		FREE_PAGES_PER_THREAD = (ALLOC_PAGES_PER_THREAD * 95)/100;
	}
	else {
		MB_PER_THREAD = 300;
		ALLOC_PAGES_PER_THREAD = MB_PER_THREAD * 256;
		FREE_PAGES_PER_THREAD = (ALLOC_PAGES_PER_THREAD * 80)/100;
	}
}

static int kthread_alloc_pages(void *data)
{
	int itr, id = *(int *)data;
	struct page *page, **plist;
	unsigned int *flist;
	unsigned long pbnum;

	plist = kmalloc(sizeof(struct page *) * ALLOC_PAGES_PER_THREAD, GFP_KERNEL);
	flist = kzalloc(sizeof(unsigned int) * 10000, GFP_KERNEL);
	if (!plist || !flist)
		goto nomem;

	for (itr = 0; itr < ALLOC_PAGES_PER_THREAD; itr++) {
		page = alloc_pages(GFP_KERNEL, 0);
		if (!page)
			goto nomem;
		plist[itr] = page;
		pbnum = (int)(page_to_pfn(page) / pageblock_nr_pages);
		flist[pbnum]++;
	}

	for (itr = 0; itr < ALLOC_PAGES_PER_THREAD; itr++) {
		page = plist[itr];
		pbnum = (int)(page_to_pfn(page) / pageblock_nr_pages);
		if (flist[pbnum] > 2) {
			__free_pages(page, 0);
			flist[pbnum]--;
		}
	}

	kfree(plist);
	complete(&completed[id]);
	printk("Completed Successfully\n");
	return 0;

nomem:
	printk("Out of memory: %d\n", id);
	kfree(plist);
	complete(&completed[id]);
	return -ENOMEM;
}

static void start_alloc(void)
{
	int id;
	int arr[NUM_THREADS];
/*
	if (alloc_and_free_all())
		return;
*/
	initialize_global_variables();
	for (id = 0; id < NUM_THREADS; id++) {
		arr[id] = id;
		init_completion(&completed[id]);
		kthread_run(&kthread_alloc_pages, (void *)&arr[id], "NULL");
	}

	for (id = 0; id < NUM_THREADS; id++)
		wait_for_completion(&completed[id]);
}

static int __init init(void)
{
	start_alloc();
	printk("Main Thread Completed !!!\n");
	return 0;
}

static void __exit cleanup(void)
{
    printk(KERN_INFO "Existing Successfully !!!\n");
}

module_init(init);
module_exit(cleanup);
MODULE_LICENSE("GPLv2");
