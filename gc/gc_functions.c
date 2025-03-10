#include "garbage_collector.h"
#include "../libft/libft.h"

void gc_free(void)
{
	int len;
	int count;
	t_gc_list	*gc;
	t_gc_node	*tmp;
	t_gc_node	*current;

	gc = create_gc();
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
	gc = NULL;
}
