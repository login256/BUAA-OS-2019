#ifndef _SYS_QUEUE_H_
#define _SYS_QUEUE_H_

/*
 * This file defines three types of data structures: lists, tail queues,
 * and circular queues.
 *
 * A list is headed by a single forward pointer(or an array of forward
 * pointers for a hash table header). The elements are doubly linked
 * so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before
 * or after an existing element or at the head of the list. A list
 * may only be traversed in the forward direction.
 *
 * A tail queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or
 * after an existing element, at the head of the list, or at the end of
 * the list. A tail queue may only be traversed in the forward direction.
 *
 * A circle queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the list.
 * A circle queue may be traversed in either direction, but has a more
 * complex end of list detection.
 *
 * For details on the use of these macros, see the queue(3) manual page.
 */

/*
 * List declarations.
 */

/*
 * A list is headed by a structure defined by the LIST_HEAD macro.  This structure con‐
 * tains a single pointer to the first element on the list.  The elements are doubly
 * linked so that an arbitrary element can be removed without traversing the list.  New
 * elements can be added to the list after an existing element or at the head of the list.
 * A LIST_HEAD structure is declared as follows:
 *
 *       LIST_HEAD(HEADNAME, TYPE) head;
 *
 * where HEADNAME is the name of the structure to be defined, and TYPE is the type of the
 * elements to be linked into the list.
 */
#define LIST_HEAD(name, type)                                           \
        struct name {                                                           \
                struct type *lh_first;  /* first element */                     \
        }

/*
 * Set a list head variable to LIST_HEAD_INITIALIZER(head)
 * to reset it to the empty list.
 */
#define LIST_HEAD_INITIALIZER(head)                                     \
        { NULL }

/*
 * Use this inside a structure "LIST_ENTRY(type) field" to use
 * x as the list piece.
 *
 * The le_prev points at the pointer to the structure containing
 * this very LIST_ENTRY, so that if we want to remove this list entry,
 * we can do *le_prev = le_next to update the structure pointing at us.
 */
#define LIST_ENTRY(type)                                                \
        struct {                                                                \
                struct type *le_next;   /* next element */                      \
                struct type **le_prev;  /* address of previous next element */  \
        }

/*
 * List functions.
 */

/*
 * Detect the list named "head" is empty.
 */
#define LIST_EMPTY(head)        ((head)->lh_first == NULL)

/*
 * Return the first element in the list named "head".
 */
#define LIST_FIRST(head)        ((head)->lh_first)

/*
 * Iterate over the elements in the list named "head".
 * During the loop, assign the list elements to the variable "var"
 * and use the LIST_ENTRY structure member "field" as the link field.
 */
#define LIST_FOREACH(var, head, field)                                  \
        for ((var) = LIST_FIRST((head));                                \
                 (var);                                                 \
                 (var) = LIST_NEXT((var), field))

/*
 * Reset the list named "head" to the empty list.
 */
#define LIST_INIT(head) do {                                            \
                LIST_FIRST((head)) = NULL;                                      \
        } while (0)

/*
 * Insert the element "elm" *after* the element "listelm" which is
 * already in the list.  The "field" name is the link element
 * as above.
 */


#define LIST_INSERT_AFTER(listelm, elm, field)
        // Note: assign a to b <==> a = b
        //Step 1, assign elm.next to listelem.next.
        //Step 2: Judge whether listelm.next is NULL, if not, then assign listelm.pre to a proper value.
        //step 3: Assign listelm.next to a proper value.
        //step 4: Assign elm.pre to a proper value.


/*
 * Insert the element "elm" *before* the element "listelm" which is
 * already in the list.  The "field" name is the link element
 * as above.
 */
#define LIST_INSERT_BEFORE(listelm, elm, field) do {                    \
                (elm)->field.le_prev = (listelm)->field.le_prev;                \
                LIST_NEXT((elm), field) = (listelm);                            \
                *(listelm)->field.le_prev = (elm);                              \
                (listelm)->field.le_prev = &LIST_NEXT((elm), field);            \
        } while (0)

/*
 * Insert the element "elm" at the head of the list named "head".
 * The "field" name is the link element as above.
 */
#define LIST_INSERT_HEAD(head, elm, field) do {                         \
                if ((LIST_NEXT((elm), field) = LIST_FIRST((head))) != NULL)     \
                        LIST_FIRST((head))->field.le_prev = &LIST_NEXT((elm), field);\
                LIST_FIRST((head)) = (elm);                                     \
                (elm)->field.le_prev = &LIST_FIRST((head));                     \
        } while (0)

/*
 * Insert the element "elm" at the tail of the list named "head".
 * The "field" name is the link element as above. You can refer to LIST_INSERT_HEAD.
 * Note: this function has big differences with LIST_INSERT_HEAD !
 */
#define LIST_INSERT_TAIL(head, elm, field)
/* finish your code here. */


#define LIST_NEXT(elm, field)   ((elm)->field.le_next)

/*
 * Remove the element "elm" from the list.
 * The "field" name is the link element as above.
 */
#define LIST_REMOVE(elm, field) do {                                    \
                if (LIST_NEXT((elm), field) != NULL)                            \
                        LIST_NEXT((elm), field)->field.le_prev =                \
                                        (elm)->field.le_prev;                           \
                *(elm)->field.le_prev = LIST_NEXT((elm), field);                \
        } while (0)

/*
 * Tail queue definitions.
 */
#define TAILQ_HEAD(name, type)                                          \
        struct name {                                                           \
                struct type *tqh_first; /* first element */                     \
                struct type **tqh_last; /* addr of last next element */         \
        }

#define TAILQ_ENTRY(type)                                               \
        struct {                                                                \
                struct type *tqe_next;  /* next element */                      \
                struct type **tqe_prev; /* address of previous next element */  \
        }

#endif  /* !_SYS_QUEUE_H_ */

