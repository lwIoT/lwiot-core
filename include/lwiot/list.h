/*
 * E/Stack list header
 *
 * Author: Michel Megens
 * Date: 28/11/2017
 * Email: dev@bietje.net
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>

#include <lwiot/compiler.h>

 /**
  * @brief List head structure.
  */
struct DLL_EXPORT list_head {
	struct list_head *next; //!< Next pointer.
	struct list_head *prev; //!< Previous pointer.
};

/**
 * @brief Static list initialiser.
 * @param name Name of the list which has to be initialised.
 */
#define STATIC_INIT_LIST_HEAD(name) { &(name), &(name) }

 /**
  * @brief Dynamic list initialiser.
  * @param list List which has to be initialised.
  */
static inline void list_head_init(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *lnew,
	struct list_head *prev,
	struct list_head *next)
{
	next->prev = lnew;
	lnew->next = next;
	lnew->prev = prev;
	prev->next = lnew;
}

static inline void __list_del(struct list_head *prev,
	struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * @brief Delete an entry from a linked list.
 * @param entry Entry which has to be deleted.
 */
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

/**
 * @brief Check if a list is empty.
 * @param head List head to check.
 * @return True or false based on whether the list is empty or not.
 * @retval true if the list is empty.
 * @retval false if the list is not empty.
 */
static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

/**
 * @brief Check if an entry is the last entry of a list.
 * @param list List entry to check.
 * @param head List head.
 * @return True or false based on whether the entry is the last entry or not.
 */
static inline int list_is_last(const struct list_head *list,
	const struct list_head *head)
{
	return list->next == head;
}

typedef int(*list_comparator_t)(struct list_head *lh1, struct list_head *lh2);

#define list_peak(h, type, e) \
		list_empty(h) ? NULL : list_first_entry(h,type,e)

/**
 * @brief Loop through a list.
 * @param pos Carriage pointer.
 * @param head List head.
 */
#define list_for_each(pos, head) \
		for(pos = (head)->next; pos != (head); pos = pos->next)

 /**
  * @brief Loop reversed through a list.
  * @param pos Carriage pointer.
  * @param head Head pointer.
  */
#define list_for_each_prev(pos, head) \
		for(pos = (head)->prev; pos != (head); pos = pos->prev)

  /**
   * @brief Loop safely through a list.
   * @param pos Carriage pointer.
   * @param n Temporary value.
   * @param head List head.
   *
   * Safe means that the current item (\p pos) can be deleted.
   */
#define list_for_each_safe(pos, n, head) \
		for(pos = (head)->next, n = pos->next; pos != (head); \
				pos = n, n = pos->next)

   /**
	* @brief Loop backwards, in a safe manner through a list.
	* @param pos Carriage pointer.
	* @param n Temporary value.
	* @param head List head.
	* @see list_for_each_safe
	*/
#define list_for_each_prev_safe(pos, n, head) \
		for(pos = (head)->prev, n = pos->prev; pos != (head); \
				pos = n, n = pos->prev)

	/**
	 * @brief Add a new list node.
	 * @param ___n Node to add.
	 * @param ___h List head.
	 */
#define list_add(___n, ___h) __list_add(___n, ___h, (___h)->next)

	 /**
	  * @brief Add a new entry to the tail.
	  * @param __n New node.
	  * @param __h List head.
	  */
#define list_add_tail(__n, __h) __list_add(__n, (__h)->prev, __h)

	  /**
	   * @brief Get the list container structure.
	   * @param ptr Entry pointer.
	   * @param type Container type.
	   * @param member Container member name.
	   */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

	   /**
		* @brief Get the first entry from a list.
		* @param ptr the list head to take the element from.
		* @param type the type of the struct this is embedded in.
		* @param member the name of the list_struct within the struct.
		*
		* Note, that list is expected to be not empty.
		*/
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)

#endif
