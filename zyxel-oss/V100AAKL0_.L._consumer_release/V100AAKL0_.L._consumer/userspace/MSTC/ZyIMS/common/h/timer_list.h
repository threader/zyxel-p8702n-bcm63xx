#ifndef _LIST_H_
#define _LIST_H_

#ifdef ENABLE_MPATROL
#include <mpatrol.h>
#endif

/**
 * @file timer_list.h
 * @brief timer list Routines
 *
 * This is a very simple implementation of a linked list.
 * <BR>There is not much to say about it... Except that it
 * could be a lot improved. Sadly, it would be difficult
 * to improve it without breaking the compatibility with
 * older version!
 */

/**
 * @defgroup timer_LIST timer list Handling
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DOXYGEN
/**
 * Structure for referencing a node in a timer_list_t element.
 * @var __node_t
 */
  typedef struct __node __node_t;

/**
 * Structure for referencing a node in a timer_list_t element.
 * @struct __node
 */
  struct __node
  {
    void *next;			/**< next __node_t containing element */
    void *element;              /**< element in Current node */
  };
#endif

/**
 * Structure for referencing a list of elements.
 * @var timer_list_t
 */
  typedef struct timer_list timer_list_t;

/* added by bennewit@cs.tu-berlin.de */
  typedef struct {
    __node_t * actual;
    __node_t ** prev;
    timer_list_t * li;
    int pos;
  } timer_list_iterator_t;

/**
 * Structure for referencing a list of elements.
 * @struct timer_list
 */
  struct timer_list
  {

    int nb_elt;         /**< Number of element in the list */
    __node_t *node;     /**< Next node containing element  */

  };

/* added by bennewit@cs.tu-berlin.de */
#define timer_list_iterator_has_elem( it ) ( 0 != (it).actual && (it).pos < (it).li->nb_elt )

/**
 * Initialise a timer_list_t element.
 * NOTE: this element MUST be previously allocated with
 * timer_malloc(). The timer_free() call on the list is
 * still automatically done by timer_list_free(). This
 * also means you can't use a static timer_list_t variable
 * if you want to use timer_list_free().
 * @param li The element to initialise.
 */
  int timer_list_init (timer_list_t * li);
/**
 * Free a list of element.
 * Each element will be free with the method given as the second parameter.
 * @param li The element to work on.
 * @param free_func The method that is able to release one element of the list.
 */
  void timer_list_special_free (timer_list_t * li, void *(*free_func) (void *));
/**
 * Free a list of element where elements are pointer to 'char'.
 * @param li The element to work on.
 */
  void timer_list_ofchar_free (timer_list_t * li);
/**
 * Get the size of a list of element.
 * @param li The element to work on.
 */
  int timer_list_size (const timer_list_t * li);
/**
 * Check if the end of list is detected .
 * @param li The element to work on.
 * @param pos The index of the possible element.
 */
  int timer_list_eol (const timer_list_t * li, int pos);
/**
 * Add an element in a list.
 * @param li The element to work on.
 * @param element The pointer on the element to add.
 * @param pos the index of the element to add. (or -1 to append the element at the end)
 */
  int timer_list_add (timer_list_t * li, void *element, int pos);
/**
 * Get an element from a list.
 * @param li The element to work on.
 * @param pos the index of the element to get.
 */
  void *timer_list_get (const timer_list_t * li, int pos);
/**
 * Remove an element from a list.
 * @param li The element to work on.
 * @param pos the index of the element to remove.
 */
  int timer_list_remove (timer_list_t * li, int pos);

/* added by bennewit@cs.tu-berlin.de */
  void * timer_list_get_first( timer_list_t * li, timer_list_iterator_t * it );
/* added by bennewit@cs.tu-berlin.de */
  void * timer_list_get_next( timer_list_iterator_t * it );
/* added by bennewit@cs.tu-berlin.de */
  void * timer_list_iterator_remove( timer_list_iterator_t * it );


void *timer_malloc (int size);
void timer_free (void *ptr);


#ifdef __cplusplus
}
#endif


/** @} */

#endif

