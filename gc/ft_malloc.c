#include "../libft/libft.h"
#include "garbage_collector.h"

void	*ft_malloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(count, size);
	if (!ptr)
		return (NULL);
	gc_add(ptr);
	return (ptr);
}
/*
count is the number of elements to allocate
size is the size of each element (int or char or struct)
*/




// #include "garbage_collector.h"


// int	main(void)
// {
// 	int	*arr;

// 	gc_init(); //Muss nur einmaal aufgerufen werden
// 	arr = ft_malloc(sizeof(int), 10);
// 	if (!arr)
// 		return (1);
// 	arr[0] = 42;
// 	printf("First element: %d\n", arr[0]);
// 	// gc_free();
// 	// ft_bzero(get_gc(), sizeof(t_gc_list)); // Alternativ auch mit memset
	// clean_up();
// 	return (0);
// }



// int	main(void)
// {
// 	t_gc_list *gc;

// 	//get a static t_gc_list
// 	gc = get_gc();

// 	char *str_single_pointer;
// 	// char *str_single_pointer2;
// 	// char *str_single_pointer3;
// 	// char *str_single_pointer4;
// 	// char *str_single_pointer5;
// 	// char *str_single_pointer6;
// 	// char *str_single_pointer7;
// 	// int **int_double_ptr;
// 	// int *int_single_ptr;
// 	// char **char_double_ptr;

// 	//initialize the garbage collector
// 	gc_init();

// 	//print the size of the list
// 	printf("\ngc size: %ld\n\n", gc->size);

// 	// for an array the size will be the number of elements in
// 	// the array +1 because the first element is the size of the array
// 	// so for and array of 5 elements the size will be 6
// 	// this counts for both intergers and characters ararys

// 	// example of a single pointer
// 	// of char type
// 	str_single_pointer = ft_malloc(6, sizeof(char));
// 	// str_single_pointer2 = ft_malloc(6, sizeof(char));
// 	// str_single_pointer3 = ft_malloc(6, sizeof(char));
// 	// str_single_pointer4 = ft_malloc(6, sizeof(char));
// 	// str_single_pointer5 = ft_malloc(6, sizeof(char));
// 	// str_single_pointer6 = ft_malloc(6, sizeof(char));
// 	// str_single_pointer7 = ft_malloc(6, sizeof(char));
// 	ft_strlcpy(str_single_pointer, "hallo", 6);
// 	// ft_strlcpy(str_single_pointer2, "hallo", 6);
// 	// ft_strlcpy(str_single_pointer3, "hallo", 6);
// 	// ft_strlcpy(str_single_pointer4, "hallo", 6);
// 	// ft_strlcpy(str_single_pointer5, "hallo", 6);
// 	// ft_strlcpy(str_single_pointer6, "hallo", 6);
// 	// ft_strlcpy(str_single_pointer7, "hallo", 6);
// 	printf("str_single_pointer: %s\n", str_single_pointer);
// 	// printf("str_single_pointer2: %s\n", str_single_pointer2);
// 	// printf("str_single_pointer3: %s\n", str_single_pointer3);
// 	// printf("str_single_pointer4: %s\n", str_single_pointer4);
// 	// printf("str_single_pointer5: %s\n", str_single_pointer5);
// 	// printf("str_single_pointer6: %s\n", str_single_pointer6);
// 	// printf("str_single_pointer7: %s\n", str_single_pointer7);

// 	// print the size of the list
// 	printf("\ngc size: %ld\n\n", gc->size);

// 	//example of a double pointer
// 	//of char type
// 	// char_double_ptr = ft_malloc(2, sizeof(char *));
// 	// char_double_ptr[0] = ft_malloc(6, sizeof(char));
// 	// char_double_ptr[1] = ft_malloc(6, sizeof(char));
// 	// ft_strlcpy(char_double_ptr[0], "hallo", 6);
// 	// ft_strlcpy(char_double_ptr[1], "hallo", 6);
// 	// printf("char_double_ptr[0]: %s\n", char_double_ptr[0]);
// 	// printf("char_double_ptr[1]: %s\n", char_double_ptr[1]);

// 	//print the size of the list
// 	// printf("\ngc size: %ld\n\n", gc->size);


// 	//example of a single pointer
// 	//of int type
// 	// int_single_ptr = ft_malloc(1, sizeof(int));
// 	// *int_single_ptr = 42;
// 	// printf("int_single_ptr: %d\n", *int_single_ptr);


// 	//print the size of the list
// 	// printf("\ngc size: %ld\n\n", gc->size);


// 	//example of a double pointer
// 	//of int type
// 	// int_double_ptr = ft_malloc(2, sizeof(int *));
// 	// int_double_ptr[0] = ft_malloc(1, sizeof(int));
// 	// int_double_ptr[1] = ft_malloc(1, sizeof(int));
// 	// *int_double_ptr[0] = 42;
// 	// *int_double_ptr[1] = 21;
// 	// printf("int_double_ptr[0]: %d\n", *int_double_ptr[0]);
// 	// printf("int_double_ptr[1]: %d\n", *int_double_ptr[1]);

// 	//print the size of the list
// 	//printf("\ngc size: %ld\n\n", gc->size);

// 	//free all the allocated memory
// 	// gc_free();
// 	return (0);
// }
