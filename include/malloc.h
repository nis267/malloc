/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/15 14:19:59 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 13:39:53 by dewalter    ###    #+. /#+    ###.fr     */
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

typedef struct s_ma_chunk
{
    size_t              size;
    bool                free;
    struct s_ma_chunk   *next;
    struct s_ma_chunk   *prev;
}              t_ma_chunk;

typedef struct      s_region
{
	struct s_region	*head;
	struct s_region	*next;
	size_t		    remaining;
	size_t	        space;
	// unsigned	    max_chunk_size;
}                   t_region;

typedef struct s_ma_heap
{
    t_region   *tiny;
    t_region   *small;
    t_region   *large;
}               t_ma_heap;

t_ma_heap g_heap;

#endif