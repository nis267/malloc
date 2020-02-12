/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   realloc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/31 14:30:24 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/09 12:19:34 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void *get_new_allocation(t_ma_chunk *chunk, /*size_t region_type,*/ size_t size)
{
    void *addr;

    addr = NULL;
    if (size <= (chunk->size - sizeof(t_ma_chunk)))
    {
        return (chunk + 1);
    }
    else
    {
        addr = malloc(size);
        ft_memcpy(addr, (chunk + 1), chunk->size);
        free(chunk + 1);
    }
    return (addr);
}

void *realloc(void *ptr, size_t size)
{
    t_ma_chunk *chunk;
    size_t region_type;
    void *addr;

    addr = NULL;
    region_type = get_region_type(size);
    if (ptr == NULL && size)
        addr = malloc(size);
    else
    {
        if ((chunk = search_pointer(g_region, ptr)))
            addr = get_new_allocation(chunk, size);
        else
            return (NULL);
    }
    return (addr);
}