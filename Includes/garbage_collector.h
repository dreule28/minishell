#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

// Includes
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

// Structs
typedef struct s_gc_node
{
	void				*data;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_gc_list
{
	t_gc_node			*head;
	t_gc_node			*tail;
	size_t				size;
}						t_gc_list;

// Funcitons in ft_malloc.c
void					*ft_malloc(size_t count, size_t size);

// Functions in gc_untils.c
void					gc_add(void *ptr);
void					gc_free(void);
void					gc_init(void);
t_gc_list				*get_gc(void);
void					clean_up(void);

#endif