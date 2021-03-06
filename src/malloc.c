/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/31 14:30:18 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/12 16:30:44 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

size_t get_chunk_size(size_t size)
{
    size = size + sizeof(t_ma_chunk);
    size = (size + 15) & ~15;
    return (size);
}

size_t get_region_chunk_type(size_t chunk_size)
{
    if (chunk_size <= TINY_BYTE_MAX)
        return (TINY);
    else if (chunk_size <= SMALL_BYTE_MAX)
        return (SMALL);
    else
        return (LARGE);
}

size_t get_region_type(size_t region_size)
{
    if (region_size <= (size_t)TINY_MMAP_SIZE)
        return (TINY);
    else if (region_size <= (size_t)SMALL_MMAP_SIZE)
        return (SMALL);
    else
        return (LARGE);
}

size_t get_mmap_size(size_t chunk_size, int region_type)
{
    size_t mmap_region_size;

    if (region_type == TINY)
        mmap_region_size = TINY_MMAP_SIZE;
    else if (region_type == SMALL)
        mmap_region_size = SMALL_MMAP_SIZE;
    else
        mmap_region_size = chunk_size + sizeof(t_region);
    return (mmap_region_size);
}

void *add_new_chunk_node(t_region *region, size_t chunk_size, int first_chunk)
{
    t_ma_chunk *chunk;
    t_ma_chunk *chunk_temp;

    chunk = (t_ma_chunk *)(region + 1);
    (region)->remaining = region->remaining - chunk_size;
    if (first_chunk)
    {
        ft_memset(chunk, 0, sizeof(t_ma_chunk));
        chunk->size = chunk_size;
        chunk->free = false;
        return ((void *)(size_t)(chunk + 1));
    }
    while (chunk->next)
        chunk = chunk->next;
    chunk_temp = (t_ma_chunk *)((size_t)(chunk) + chunk->size);
    chunk_temp = ft_memset(chunk_temp, 0, sizeof(t_ma_chunk));
    chunk_temp->prev = chunk;
    chunk_temp->size = chunk_size;
    chunk->next = chunk_temp;
    return ((void *)(size_t)(chunk_temp + 1));
}

void *add_new_region_node(t_region **region, t_region *new_region, size_t chunk_size)
{
    void *addr;
    t_region *first;

    first = *region ? *region : NULL;
    if (*region)
    {
        while ((*region) && (*region)->next)
            (*region) = (*region)->next;
        new_region->prev = (*region);
        (*region)->next = new_region;
    }
    *region = new_region;
    addr = add_new_chunk_node(*region, chunk_size, 1);
    *region = first ? first : *region;
    return (addr);
}

void init_new_region_node(t_region *new_region, size_t region_size)
{
    ft_memset(new_region, 0, sizeof(t_region));
    new_region->remaining = region_size - sizeof(t_region);
    new_region->size = region_size;
}

void split_chunk(t_ma_chunk *chunk, size_t chunk_size)
{
    t_ma_chunk *new_chunk;

    new_chunk = (t_ma_chunk *)((size_t)chunk + chunk_size);
    new_chunk->size = chunk->size - chunk_size;
    new_chunk->free = true;
    new_chunk->next = chunk->next;
    new_chunk->prev = chunk;
    chunk->next = new_chunk;
}

void *get_address(t_ma_chunk *smallest, t_region *region_temp,
                  size_t chunk_size)
{
    if (smallest)
    {
        smallest->free = false;
        return ((void *)(size_t)(smallest + 1));
    }
    else if (region_temp)
        return (add_new_chunk_node(region_temp, chunk_size, 0));
    return (NULL);
}

void *check_free_chunk(t_region *region, size_t region_type, size_t chunk_size)
{
    t_ma_chunk *chunk;
    t_ma_chunk *smallest;
    t_region *region_temp;

    region_temp = NULL;
    smallest = NULL;
    while (region)
    {
        if (get_region_type(region->size) == region_type)
        {
            chunk = (t_ma_chunk *)((region) + 1);
            while (chunk)
            {
                if (chunk->free == true && (chunk->size >= chunk_size) &&
                    (smallest == NULL ||
                     (smallest->size < chunk->size)))
                    smallest = chunk;
                chunk = chunk->next;
            }
            if (!smallest && region->remaining >= chunk_size && !region_temp)
                region_temp = region;
        }
        region = region->next;
    }
    return(get_address(smallest, region_temp, chunk_size));
}

void *get_free_chunk(size_t chunk_size, int region_type)
{
    t_region *region;
    void *addr_data;
    size_t mmap_region_size;

    if (region_type == LARGE || !(addr_data =
    check_free_chunk(g_region, region_type, chunk_size)))
    {
        mmap_region_size = get_mmap_size(chunk_size, region_type);
        if ((region = (t_region *)mmap(0, mmap_region_size,
        PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
            return (NULL);
        init_new_region_node(region, mmap_region_size);
        addr_data = add_new_region_node(&g_region, region, chunk_size);
    }
    return (addr_data);
}

int check_size_limit(size_t size)
{
    struct rlimit rlim;
    int resource;

    resource = RLIMIT_AS;
    getrlimit(resource, &rlim);
    if (size > rlim.rlim_cur)
        return (1);
    return (0);
}

void *malloc(size_t size)
{
    size_t chunk_size;
    size_t region_type;
    void *addr;

    addr = NULL;
    if (check_size_limit(size))
        return (NULL);
    chunk_size = get_chunk_size(size);
    region_type = get_region_chunk_type(chunk_size);
    addr = get_free_chunk(chunk_size, region_type);
    return (addr);
}