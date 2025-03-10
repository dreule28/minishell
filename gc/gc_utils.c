#include "garbage_collector.h"
#include "../libft/libft.h"

t_gc_list *create_gc(void)
{
	static t_gc_list gc = {0};

	return (&gc);
}