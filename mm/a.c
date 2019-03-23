# 1 "pmap.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "pmap.c"
# 1 "../include/mmu.h" 1
# 132 "../include/mmu.h"
# 1 "../include/types.h" 1
# 11 "../include/types.h"
typedef unsigned __attribute__((__mode__(QI))) u_int8_t;
typedef int __attribute__((__mode__(HI))) int16_t;
typedef unsigned __attribute__((__mode__(HI))) u_int16_t;
typedef int __attribute__((__mode__(SI))) int32_t;
typedef unsigned __attribute__((__mode__(SI))) u_int32_t;
typedef int __attribute__((__mode__(DI))) int64_t;
typedef unsigned __attribute__((__mode__(DI))) u_int64_t;

typedef int32_t register_t;

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef u_int64_t u_quad_t;
typedef int64_t quad_t;
typedef quad_t *qaddr_t;

typedef u_int32_t size_t;
# 133 "../include/mmu.h" 2
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);

extern char bootstacktop[], bootstack[];

extern u_long npage;

typedef u_long Pde;
typedef u_long Pte;

extern volatile Pte *vpt[];
extern volatile Pde *vpd[];
# 174 "../include/mmu.h"
extern void tlb_out(u_int entryhi);
# 2 "pmap.c" 2
# 1 "../include/pmap.h" 1




# 1 "../include/queue.h" 1
# 6 "../include/pmap.h" 2
# 1 "../include/mmu.h" 1
# 7 "../include/pmap.h" 2
# 1 "../include/printf.h" 1
# 15 "../include/printf.h"
# 1 "/usr/lib/gcc/i686-linux-gnu/4.6/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/i686-linux-gnu/4.6/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 102 "/usr/lib/gcc/i686-linux-gnu/4.6/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 16 "../include/printf.h" 2
void printf(char *fmt, ...);

void _panic(const char *, int, const char *, ...)
__attribute__((noreturn));

void printcharc(char ch);
# 8 "../include/pmap.h" 2


struct Page_list { struct Page *lh_first; };
typedef struct { struct Page *le_next; struct Page **le_prev; } Page_LIST_entry_t;

struct Page {
 Page_LIST_entry_t pp_link;






 u_short pp_ref;
};

extern struct Page *pages;

static inline u_long
page2ppn(struct Page *pp)
{
 return pp - pages;
}



static inline u_long
page2pa(struct Page *pp)
{
 return page2ppn(pp) << 12;
}



static inline struct Page *
pa2page(u_long pa)
{
 if ((((u_long)(pa))>>12) >= npage) {
  _panic("../include/pmap.h", 46, "pa2page called with invalid pa: %x", pa);
 }

 return &pages[(((u_long)(pa))>>12)];
}



static inline u_long
page2kva(struct Page *pp)
{
 return ({ u_long ppn = (((u_long)(page2pa(pp)))>>12); if (ppn >= npage) _panic("../include/pmap.h", 57, "KADDR called with invalid pa %08lx", (u_long)page2pa(pp)); (page2pa(pp)) + 0x80000000; });
}



static inline u_long
va2pa(Pde *pgdir, u_long va)
{
 Pte *p;

 pgdir = &pgdir[((((u_long)(va))>>22) & 0x03FF)];

 if (!(*pgdir & 0x0200)) {
  return ~0;
 }

 p = (Pte *)({ u_long ppn = (((u_long)(((u_long)(*pgdir)&~0xFFF)))>>12); if (ppn >= npage) _panic("../include/pmap.h", 73, "KADDR called with invalid pa %08lx", (u_long)((u_long)(*pgdir)&~0xFFF)); (((u_long)(*pgdir)&~0xFFF)) + 0x80000000; });

 if (!(p[((((u_long)(va))>>12) & 0x03FF)]&0x0200)) {
  return ~0;
 }

 return ((u_long)(p[((((u_long)(va))>>12) & 0x03FF)])&~0xFFF);
}



void mips_detect_memory();

void mips_vm_init();

void mips_init();
void page_init(void);
void page_check();
void physical_memory_manage_check();
int page_alloc(struct Page **pp);
void page_free(struct Page *pp);
void page_decref(struct Page *pp);
int pgdir_walk(Pde *pgdir, u_long va, int create, Pte **ppte);
int page_insert(Pde *pgdir, struct Page *pp, u_long va, u_int perm);
struct Page *page_lookup(Pde *pgdir, u_long va, Pte **ppte);
void page_remove(Pde *pgdir, u_long va) ;
void tlb_invalidate(Pde *pgdir, u_long va);

void boot_map_segment(Pde *pgdir, u_long va, u_long size, u_long pa, int perm);

extern struct Page *pages;
# 3 "pmap.c" 2
# 1 "../include/printf.h" 1
# 4 "pmap.c" 2
# 1 "../include/env.h" 1







# 1 "../include/trap.h" 1
# 33 "../include/trap.h"
# 1 "../include/types.h" 1
# 34 "../include/trap.h" 2

struct Trapframe {

 unsigned long regs[32];


 unsigned long cp0_status;
 unsigned long hi;
 unsigned long lo;
 unsigned long cp0_badvaddr;
 unsigned long cp0_cause;
 unsigned long cp0_epc;
 unsigned long pc;
};
void *set_except_vector(int n, void *addr);
void trap_init();
# 9 "../include/env.h" 2
# 21 "../include/env.h"
struct Env {
 struct Trapframe env_tf;
 struct { struct Env *le_next; struct Env **le_prev; } env_link;
 u_int env_id;
 u_int env_parent_id;
 u_int env_status;
 Pde *env_pgdir;
 u_int env_cr3;


 u_int env_ipc_value;
 u_int env_ipc_from;
 u_int env_ipc_recving;
 u_int env_ipc_dstva;
 u_int env_ipc_perm;


 u_int env_pgfault_handler;
 u_int env_xstacktop;


 u_int env_runs;
};

struct Env_list { struct Env *lh_first; };
extern struct Env *envs;
extern struct Env *curenv;

void env_init(void);
int env_alloc(struct Env **e, u_int parent_id);
void env_free(struct Env *);
void env_create(u_char *binary, int size);
void env_destroy(struct Env *e);

int envid2env(u_int envid, struct Env **penv, int checkperm);
void env_run(struct Env *e);
# 5 "pmap.c" 2
# 1 "../include/error.h" 1
# 6 "pmap.c" 2



u_long maxpa;
u_long npage;
u_long basemem;
u_long extmem;

Pde *boot_pgdir;

struct Page *pages;
static u_long freemem;

static struct Page_list page_free_list;





void mips_detect_memory()
{





    printf("Physical memory: %dK available, ", (int)(maxpa / 1024));
    printf("base = %dK, extended = %dK\n", (int)(basemem / 1024),
           (int)(extmem / 1024));
}
# 44 "pmap.c"
static void *alloc(u_int n, u_int align, int clear)
{
    extern char end[];
    u_long alloced_mem;



    if (freemem == 0) {
        freemem = (u_long)end;
    }


    freemem = (((((u_long)(freemem))+(align)-1)) & ~((align)-1));


    alloced_mem = freemem;


    freemem = freemem + n;


    if (clear) {
        bzero((void *)alloced_mem, n);
    }


    if (({ u_long a = (u_long) (freemem); if (a < 0x80000000) _panic("pmap.c", 70, "PADDR called with invalid kva %08lx", a); a - 0x80000000; }) >= maxpa) {
        _panic("pmap.c", 71, "out of memorty\n");
        return (void *)-4;
    }


    return (void *)alloced_mem;
}






static Pte *boot_pgdir_walk(Pde *pgdir, u_long va, int create)
{

    Pde *pgdir_entryp;
    Pte *pgtable, *pgtable_entry;
# 103 "pmap.c"
}
# 113 "pmap.c"
void boot_map_segment(Pde *pgdir, u_long va, u_long size, u_long pa, int perm)
{
    int i, va_temp;
    Pte *pgtable_entry;
# 125 "pmap.c"
}






void mips_vm_init()
{
    extern char end[];
    extern int mCONTEXT;
    extern struct Env *envs;

    Pde *pgdir;
    u_int n;


    pgdir = alloc(4096, 4096, 1);
    printf("to memory %x for struct page directory.\n", freemem);
    mCONTEXT = (int)pgdir;

    boot_pgdir = pgdir;





    pages = (struct Page *)alloc(npage * sizeof(struct Page), 4096, 1);
    printf("to memory %x for struct Pages.\n", freemem);
    n = (((((u_long)(npage * sizeof(struct Page)))+(4096)-1)) & ~((4096)-1));
    boot_map_segment(pgdir, ((0x80000000 - (4*1024*1024)) - (4*1024*1024)), n, ({ u_long a = (u_long) (pages); if (a < 0x80000000) _panic("pmap.c", 155, "PADDR called with invalid kva %08lx", a); a - 0x80000000; }), 0x0400);



    envs = (struct Env *)alloc((1<<10) * sizeof(struct Env), 4096, 1);
    n = (((((u_long)((1<<10) * sizeof(struct Env)))+(4096)-1)) & ~((4096)-1));
    boot_map_segment(pgdir, (((0x80000000 - (4*1024*1024)) - (4*1024*1024)) - (4*1024*1024)), n, ({ u_long a = (u_long) (envs); if (a < 0x80000000) _panic("pmap.c", 161, "PADDR called with invalid kva %08lx", a); a - 0x80000000; }), 0x0400);

    printf("pmap.c:\t mips vm init success\n");
}







void
page_init(void)
{
# 187 "pmap.c"
}
# 203 "pmap.c"
int
page_alloc(struct Page **pp)
{
    struct Page *ppage_temp;
# 215 "pmap.c"
}





void
page_free(struct Page *pp)
{
# 232 "pmap.c"
    _panic("pmap.c", 232, "cgh:pp->pp_ref is less than zero\n");
}
# 251 "pmap.c"
int
pgdir_walk(Pde *pgdir, u_long va, int create, Pte **ppte)
{
    Pde *pgdir_entryp;
    Pte *pgtable;
    struct Page *ppage;
# 270 "pmap.c"
    return 0;
}
# 284 "pmap.c"
int
page_insert(Pde *pgdir, struct Page *pp, u_long va, u_int perm)
{
    u_int PERM;
    Pte *pgtable_entry;
    PERM = perm | 0x0200;


    pgdir_walk(pgdir, va, 0, &pgtable_entry);

    if (pgtable_entry != 0 && (*pgtable_entry & 0x0200) != 0) {
        if (pa2page(*pgtable_entry) != pp) {
            page_remove(pgdir, va);
        } else {
            tlb_invalidate(pgdir, va);
            *pgtable_entry = (page2pa(pp) | PERM);
            return 0;
        }
    }
# 315 "pmap.c"
    return 0;
}







struct Page *
page_lookup(Pde *pgdir, u_long va, Pte **ppte)
{
    struct Page *ppage;
    Pte *pte;


    pgdir_walk(pgdir, va, 0, &pte);


    if (pte == 0) {
        return 0;
    }
    if ((*pte & 0x0200) == 0) {
        return 0;
    }




    ppage = pa2page(*pte);
    if (ppte) {
        *ppte = pte;
    }

    return ppage;
}



void page_decref(struct Page *pp) {
    if(--pp->pp_ref == 0) {
        page_free(pp);
    }
}



void
page_remove(Pde *pgdir, u_long va)
{
    Pte *pagetable_entry;
    struct Page *ppage;


    ppage = page_lookup(pgdir, va, &pagetable_entry);

    if (ppage == 0) {
        return;
    }




    ppage->pp_ref--;
    if (ppage->pp_ref == 0) {
        page_free(ppage);
    }


    *pagetable_entry = 0;
    tlb_invalidate(pgdir, va);
    return;
}



void
tlb_invalidate(Pde *pgdir, u_long va)
{
    if (curenv) {
        tlb_out(((u_long)(va)&~0xFFF) | (((curenv->env_id)>> 11)<<6));
    } else {
        tlb_out(((u_long)(va)&~0xFFF));
    }
}

void
physical_memory_manage_check(void)
{
    struct Page *pp, *pp0, *pp1, *pp2;
    struct Page_list fl;
    int *temp;


    pp0 = pp1 = pp2 = 0;
    do { if (!(page_alloc(&pp0) == 0)) _panic("pmap.c", 410, "assertion failed: %s", "page_alloc(&pp0) == 0"); } while (0);
    do { if (!(page_alloc(&pp1) == 0)) _panic("pmap.c", 411, "assertion failed: %s", "page_alloc(&pp1) == 0"); } while (0);
    do { if (!(page_alloc(&pp2) == 0)) _panic("pmap.c", 412, "assertion failed: %s", "page_alloc(&pp2) == 0"); } while (0);

    do { if (!(pp0)) _panic("pmap.c", 414, "assertion failed: %s", "pp0"); } while (0);
    do { if (!(pp1 && pp1 != pp0)) _panic("pmap.c", 415, "assertion failed: %s", "pp1 && pp1 != pp0"); } while (0);
    do { if (!(pp2 && pp2 != pp1 && pp2 != pp0)) _panic("pmap.c", 416, "assertion failed: %s", "pp2 && pp2 != pp1 && pp2 != pp0"); } while (0);




    fl = page_free_list;

    do { (((&page_free_list))->lh_first) = ((void *) 0); } while (0);

    do { if (!(page_alloc(&pp) == -4)) _panic("pmap.c", 425, "assertion failed: %s", "page_alloc(&pp) == -E_NO_MEM"); } while (0);

    temp = (int*)page2kva(pp0);

    *temp = 1000;

    page_free(pp0);
    printf("The number in address temp is %d\n",*temp);


    do { if (!(page_alloc(&pp0) == 0)) _panic("pmap.c", 435, "assertion failed: %s", "page_alloc(&pp0) == 0"); } while (0);
    do { if (!(pp0)) _panic("pmap.c", 436, "assertion failed: %s", "pp0"); } while (0);


    do { if (!(temp == (int*)page2kva(pp0))) _panic("pmap.c", 439, "assertion failed: %s", "temp == (int*)page2kva(pp0)"); } while (0);

    do { if (!(*temp == 0)) _panic("pmap.c", 441, "assertion failed: %s", "*temp == 0"); } while (0);

    page_free_list = fl;
    page_free(pp0);
    page_free(pp1);
    page_free(pp2);
    struct Page_list test_free;
    struct Page *test_pages;
 test_pages= (struct Page *)alloc(10 * sizeof(struct Page), 4096, 1);
 do { (((&test_free))->lh_first) = ((void *) 0); } while (0);

 int i,j=0;
 struct Page *p, *q;

 for(i=0;i<10;i++) {
  test_pages[i].pp_ref=i;


  do { if ((((&test_free))->lh_first) != ((void *) 0)) { ((((&test_pages[i]), pp_link))->pp_link.le_next) = (((&test_free))->lh_first); while ((((((&test_pages[i]))->pp_link.le_next))->pp_link.le_next)) { (((&test_pages[i]))->pp_link.le_next) = (((((&test_pages[i]))->pp_link.le_next))->pp_link.le_next) ; } (((((&test_pages[i]))->pp_link.le_next))->pp_link.le_next) = (&test_pages[i]); (&test_pages[i])->pp_link.le_prev = &(((((&test_pages[i]))->pp_link.le_next))->pp_link.le_next); (((&test_pages[i]))->pp_link.le_next) = ((void *) 0); } else { (((&test_free))->lh_first) = (&test_pages[i]); (&test_pages[i])->pp_link.le_prev = &(((&test_free))->lh_first); } } while (0);


 }
 p = ((&test_free)->lh_first);
 int answer1[]={0,1,2,3,4,5,6,7,8,9};
 do { if (!(p!=((void *) 0))) _panic("pmap.c", 465, "assertion failed: %s", "p!=NULL"); } while (0);
 while(p!=((void *) 0))
 {

  do { if (!(p->pp_ref==answer1[j++])) _panic("pmap.c", 469, "assertion failed: %s", "p->pp_ref==answer1[j++]"); } while (0);

  p=((p)->pp_link.le_next);

 }

 int answer2[]={0,1,2,3,4,20,5,6,7,8,9};
 q=(struct Page *)alloc(sizeof(struct Page), 4096, 1);
 q->pp_ref = 20;


 do { (((q))->pp_link.le_next) = (((&test_pages[4]))->pp_link.le_next); if ((((&test_pages[4]))->pp_link.le_next)) { (((&test_pages[4]))->pp_link.le_next)->pp_link.le_prev = &(((q))->pp_link.le_next); } (((&test_pages[4]))->pp_link.le_next) = (q); (q)->pp_link.le_prev = &(((&test_pages[4]))->pp_link.le_next); }while (0);;

 p = ((&test_free)->lh_first);
 j=0;

 while(p!=((void *) 0)){

   do { if (!(p->pp_ref==answer2[j++])) _panic("pmap.c", 487, "assertion failed: %s", "p->pp_ref==answer2[j++]"); } while (0);
   p=((p)->pp_link.le_next);
 }



    printf("physical_memory_manage_check() succeeded\n");
}


void
page_check(void)
{
    struct Page *pp, *pp0, *pp1, *pp2;
    struct Page_list fl;


    pp0 = pp1 = pp2 = 0;
    do { if (!(page_alloc(&pp0) == 0)) _panic("pmap.c", 505, "assertion failed: %s", "page_alloc(&pp0) == 0"); } while (0);
    do { if (!(page_alloc(&pp1) == 0)) _panic("pmap.c", 506, "assertion failed: %s", "page_alloc(&pp1) == 0"); } while (0);
    do { if (!(page_alloc(&pp2) == 0)) _panic("pmap.c", 507, "assertion failed: %s", "page_alloc(&pp2) == 0"); } while (0);

    do { if (!(pp0)) _panic("pmap.c", 509, "assertion failed: %s", "pp0"); } while (0);
    do { if (!(pp1 && pp1 != pp0)) _panic("pmap.c", 510, "assertion failed: %s", "pp1 && pp1 != pp0"); } while (0);
    do { if (!(pp2 && pp2 != pp1 && pp2 != pp0)) _panic("pmap.c", 511, "assertion failed: %s", "pp2 && pp2 != pp1 && pp2 != pp0"); } while (0);


    fl = page_free_list;

    do { (((&page_free_list))->lh_first) = ((void *) 0); } while (0);


    do { if (!(page_alloc(&pp) == -4)) _panic("pmap.c", 519, "assertion failed: %s", "page_alloc(&pp) == -E_NO_MEM"); } while (0);


    do { if (!(page_insert(boot_pgdir, pp1, 0x0, 0) < 0)) _panic("pmap.c", 522, "assertion failed: %s", "page_insert(boot_pgdir, pp1, 0x0, 0) < 0"); } while (0);


    page_free(pp0);
    do { if (!(page_insert(boot_pgdir, pp1, 0x0, 0) == 0)) _panic("pmap.c", 526, "assertion failed: %s", "page_insert(boot_pgdir, pp1, 0x0, 0) == 0"); } while (0);
    do { if (!(((u_long)(boot_pgdir[0])&~0xFFF) == page2pa(pp0))) _panic("pmap.c", 527, "assertion failed: %s", "PTE_ADDR(boot_pgdir[0]) == page2pa(pp0)"); } while (0);

    printf("va2pa(boot_pgdir, 0x0) is %x\n",va2pa(boot_pgdir, 0x0));
    printf("page2pa(pp1) is %x\n",page2pa(pp1));

    do { if (!(va2pa(boot_pgdir, 0x0) == page2pa(pp1))) _panic("pmap.c", 532, "assertion failed: %s", "va2pa(boot_pgdir, 0x0) == page2pa(pp1)"); } while (0);
    do { if (!(pp1->pp_ref == 1)) _panic("pmap.c", 533, "assertion failed: %s", "pp1->pp_ref == 1"); } while (0);


    do { if (!(page_insert(boot_pgdir, pp2, 4096, 0) == 0)) _panic("pmap.c", 536, "assertion failed: %s", "page_insert(boot_pgdir, pp2, BY2PG, 0) == 0"); } while (0);
    do { if (!(va2pa(boot_pgdir, 4096) == page2pa(pp2))) _panic("pmap.c", 537, "assertion failed: %s", "va2pa(boot_pgdir, BY2PG) == page2pa(pp2)"); } while (0);
    do { if (!(pp2->pp_ref == 1)) _panic("pmap.c", 538, "assertion failed: %s", "pp2->pp_ref == 1"); } while (0);


    do { if (!(page_alloc(&pp) == -4)) _panic("pmap.c", 541, "assertion failed: %s", "page_alloc(&pp) == -E_NO_MEM"); } while (0);

    printf("start page_insert\n");

    do { if (!(page_insert(boot_pgdir, pp2, 4096, 0) == 0)) _panic("pmap.c", 545, "assertion failed: %s", "page_insert(boot_pgdir, pp2, BY2PG, 0) == 0"); } while (0);
    do { if (!(va2pa(boot_pgdir, 4096) == page2pa(pp2))) _panic("pmap.c", 546, "assertion failed: %s", "va2pa(boot_pgdir, BY2PG) == page2pa(pp2)"); } while (0);
    do { if (!(pp2->pp_ref == 1)) _panic("pmap.c", 547, "assertion failed: %s", "pp2->pp_ref == 1"); } while (0);



    do { if (!(page_alloc(&pp) == -4)) _panic("pmap.c", 551, "assertion failed: %s", "page_alloc(&pp) == -E_NO_MEM"); } while (0);


    do { if (!(page_insert(boot_pgdir, pp0, (4*1024*1024), 0) < 0)) _panic("pmap.c", 554, "assertion failed: %s", "page_insert(boot_pgdir, pp0, PDMAP, 0) < 0"); } while (0);


    do { if (!(page_insert(boot_pgdir, pp1, 4096, 0) == 0)) _panic("pmap.c", 557, "assertion failed: %s", "page_insert(boot_pgdir, pp1, BY2PG, 0) == 0"); } while (0);


    do { if (!(va2pa(boot_pgdir, 0x0) == page2pa(pp1))) _panic("pmap.c", 560, "assertion failed: %s", "va2pa(boot_pgdir, 0x0) == page2pa(pp1)"); } while (0);
    do { if (!(va2pa(boot_pgdir, 4096) == page2pa(pp1))) _panic("pmap.c", 561, "assertion failed: %s", "va2pa(boot_pgdir, BY2PG) == page2pa(pp1)"); } while (0);

    do { if (!(pp1->pp_ref == 2)) _panic("pmap.c", 563, "assertion failed: %s", "pp1->pp_ref == 2"); } while (0);
    printf("pp2->pp_ref %d\n",pp2->pp_ref);
    do { if (!(pp2->pp_ref == 0)) _panic("pmap.c", 565, "assertion failed: %s", "pp2->pp_ref == 0"); } while (0);
    printf("end page_insert\n");


    do { if (!(page_alloc(&pp) == 0 && pp == pp2)) _panic("pmap.c", 569, "assertion failed: %s", "page_alloc(&pp) == 0 && pp == pp2"); } while (0);


    page_remove(boot_pgdir, 0x0);
    do { if (!(va2pa(boot_pgdir, 0x0) == ~0)) _panic("pmap.c", 573, "assertion failed: %s", "va2pa(boot_pgdir, 0x0) == ~0"); } while (0);
    do { if (!(va2pa(boot_pgdir, 4096) == page2pa(pp1))) _panic("pmap.c", 574, "assertion failed: %s", "va2pa(boot_pgdir, BY2PG) == page2pa(pp1)"); } while (0);
    do { if (!(pp1->pp_ref == 1)) _panic("pmap.c", 575, "assertion failed: %s", "pp1->pp_ref == 1"); } while (0);
    do { if (!(pp2->pp_ref == 0)) _panic("pmap.c", 576, "assertion failed: %s", "pp2->pp_ref == 0"); } while (0);


    page_remove(boot_pgdir, 4096);
    do { if (!(va2pa(boot_pgdir, 0x0) == ~0)) _panic("pmap.c", 580, "assertion failed: %s", "va2pa(boot_pgdir, 0x0) == ~0"); } while (0);
    do { if (!(va2pa(boot_pgdir, 4096) == ~0)) _panic("pmap.c", 581, "assertion failed: %s", "va2pa(boot_pgdir, BY2PG) == ~0"); } while (0);
    do { if (!(pp1->pp_ref == 0)) _panic("pmap.c", 582, "assertion failed: %s", "pp1->pp_ref == 0"); } while (0);
    do { if (!(pp2->pp_ref == 0)) _panic("pmap.c", 583, "assertion failed: %s", "pp2->pp_ref == 0"); } while (0);


    do { if (!(page_alloc(&pp) == 0 && pp == pp1)) _panic("pmap.c", 586, "assertion failed: %s", "page_alloc(&pp) == 0 && pp == pp1"); } while (0);


    do { if (!(page_alloc(&pp) == -4)) _panic("pmap.c", 589, "assertion failed: %s", "page_alloc(&pp) == -E_NO_MEM"); } while (0);


    do { if (!(((u_long)(boot_pgdir[0])&~0xFFF) == page2pa(pp0))) _panic("pmap.c", 592, "assertion failed: %s", "PTE_ADDR(boot_pgdir[0]) == page2pa(pp0)"); } while (0);
    boot_pgdir[0] = 0;
    do { if (!(pp0->pp_ref == 1)) _panic("pmap.c", 594, "assertion failed: %s", "pp0->pp_ref == 1"); } while (0);
    pp0->pp_ref = 0;


    page_free_list = fl;


    page_free(pp0);
    page_free(pp1);
    page_free(pp2);

    printf("page_check() succeeded!\n");
}

void pageout(int va, int context)
{
    u_long r;
    struct Page *p = ((void *) 0);

    if (context < 0x80000000) {
        _panic("pmap.c", 614, "tlb refill and alloc error!");
    }

    if ((va > 0x7f400000) && (va < 0x7f800000)) {
        _panic("pmap.c", 618, ">>>>>>>>>>>>>>>>>>>>>>it's env's zone");
    }

    if (va < 0x10000) {
        _panic("pmap.c", 622, "^^^^^^TOO LOW^^^^^^^^^");
    }

    if ((r = page_alloc(&p)) < 0) {
        _panic("pmap.c", 626, "page alloc error!");
    }

    p->pp_ref++;

    page_insert((Pde *)context, p, (((u_long)(va)) & 0xFFFFF000 ), 0x0400);
    printf("pageout:\t@@@___0x%x___@@@  ins a page \n", va);
}
