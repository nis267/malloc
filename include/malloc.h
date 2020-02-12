/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/15 14:19:59 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/10 11:58:05 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include "libft.h"

#include <fcntl.h> // don't forget to delete


# define TINY 				1
# define TINY_BYTE_MIN		1
# define TINY_BYTE_MAX		2048
# define TINY_PAGE_NUMBER	64
# define TINY_MMAP_SIZE		TINY_PAGE_NUMBER * getpagesize()

# define SMALL 				2
# define SMALL_BYTE_MIN		2049
# define SMALL_BYTE_MAX     65536
# define SMALL_PAGE_NUMBER 	2048
# define SMALL_MMAP_SIZE	SMALL_PAGE_NUMBER * getpagesize()

# define LARGE 				4
# define LARGE_BYTE_MIN 	65537
# define ALIGNMENT          16

# define NB_TYPE_REGION 3

typedef struct s_ma_chunk
{
    size_t              size;
    bool                free;
    struct s_ma_chunk   *next;
    struct s_ma_chunk   *prev;
}              t_ma_chunk;

typedef struct      s_region
{
	struct s_region	*next;
	struct s_region	*prev;
	size_t		    remaining;
    size_t          size;
    // size_t             type;
}                   t_region;

t_region *g_region;
int g_fd;

void    *malloc(size_t size);
void    free(void *ptr);
void    *realloc(void *ptr, size_t size);
void    *calloc(size_t count, size_t size);

t_ma_chunk  *search_pointer(t_region *region, void *ptr);
size_t      get_region_chunk_type(size_t chunk_size);
void        split_chunk(t_ma_chunk *chunk, size_t chunk_size);
size_t      get_region_type(size_t region_size);
void        show_alloc_mem();

#endif