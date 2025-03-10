#ifndef GARAGE_COLLECTOR_H
#define GARAGE_COLLECTOR_H


// Includes
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Structs

typedef struct s_gc_node
{
	void 				*data;
	struct s_gc_node 	*next;
} t_gc_node;


typedef struct s_gc_list
{
	t_gc_node *head;
	t_gc_node *tail;
	size_t 	size;
} t_gc_list;

// Funcitons in ft_malloc.c
void	*ft_malloc(size_t count, size_t size);

// Functions in gc_functions.c
void 		gc_add(void *ptr);
void 		gc_free(void);
void 		gc_init(void);



// Functions in gc_utils.c
t_gc_list 	*create_gc(void);

#endif