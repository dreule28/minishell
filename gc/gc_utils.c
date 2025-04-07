#include "../libft/libft.h"
#include "garbage_collector.h"

void	clean_up(void)
{
	gc_free();
	ft_bzero(get_gc(), sizeof(t_gc_list)); // Alternativ auch mit memset
}

t_gc_list	*get_gc(void)
{
	static t_gc_list	gc = {0};

	return (&gc);
}

void	gc_free(void)
{
	int			len;
	int			count;
	t_gc_list	*gc;
	t_gc_node	*tmp;
	t_gc_node	*current;

	gc = get_gc();
	if (!gc)
		return ;
	current = gc->head;
	len = gc->size;
	count = 0;
	while (current && count < len)
	{
		tmp = current->next;
		free(current->data);
		free(current);
		current = tmp;
		count++;
	}
	// gc->head = NULL;
	// gc->tail = NULL;
	// gc->size = 0;
	// gc = NULL;
}

void	gc_add(void *ptr)
{
	t_gc_list	*gc;
	t_gc_node	*new_node;

	if (!ptr)
		return ;
	gc = get_gc();
	if (!gc)
		return ;
	new_node = ft_calloc(1, sizeof(t_gc_node));
	if (!new_node)
		return ;
	new_node->data = ptr;
	if (gc->head == NULL)
	{
		new_node->next = NULL;
		gc->head = new_node;
		gc->tail = new_node;
	}
	else
	{
		new_node->next = gc->head;
		gc->head = new_node;
	}
	gc->size++;
}

void	gc_init(void)
{
	t_gc_list	*gc;

	gc = get_gc();
	if (!gc)
		return ;
	gc->head = NULL;
	gc->tail = NULL;
	gc->size = 0;
}

// void	free_gc(void)
// {
// 	int			len;
// 	int			count;
// 	t_gc_list	*gc;
// 	t_gc_node	*tmp;
// 	t_gc_node	*current;

// 	gc = get_gc();
// 	if (!gc)
// 		return ;
// 	current = gc->head;
// 	len = gc->size;
// 	count = 0;
// 	while (current && count < len)
// 	{
// 		tmp = current->next;
// 		free(current->data);
// 		free(current);
// 		current = tmp;
// 		count++;
// 	}
// 	gc->head = NULL;
// 	gc->tail = NULL;
// 	gc->size = 0;
// 	// gc = NULL;
// }

// void	gc_add(void *ptr)
// {
// 	t_gc_list	*gc;/* gc is a pointer to a t_gc_list */
// 	t_gc_node	*new_node; /* new_node is a pointer to a t_gc_node */

// 	if (!ptr)
// 		return ;
// 	gc = get_gc();	/* get_gc is a function that returns a static t_gc_list */
// 	if (!gc)
// 		return ;
/* ft_calloc sets the allocs the new node and sets its value to 0 */
// 	new_node = ft_calloc(1, sizeof(t_gc_node));
// 	if (!new_node)
// 		return ;
// 	new_node->data = ptr;
// if the head is NULL then assign the new node to the head and tail
// 	if (gc->head == NULL)
// 	{
// 		new_node->next = NULL;
// 		gc->head = new_node;
// 		gc->tail = new_node;
// 	}
// 	else /* adds a new node to the head of the list */
// 	{
// 		new_node->next = (gc->head);
// 		gc->head = new_node;
// 	}
// 	gc->size++; /*increment the size of the list*/
// }