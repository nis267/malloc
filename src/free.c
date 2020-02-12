/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/31 14:06:03 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/12 16:31:15 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_ma_chunk *search_pointer(t_region *region, void *ptr)
{
    t_ma_chunk *chunk;
    void *data_ptr;

    while (region)
    {
        chunk = (t_ma_chunk *)(region + 1);
        while (chunk)
        {
            data_ptr = (chunk + 1);
            if (data_ptr == ptr)
                return (chunk);
            chunk = chunk->next;
        }
        region = region->next;
    }
    return (NULL);
}

t_region    *get_region(t_ma_chunk *chunk)
{
    t_region *region;

    while (chunk->prev)
        chunk = chunk->prev;
    region = (t_region*)((size_t)chunk - sizeof(t_region));
    return (region);
}

bool    check_region_free(t_region *region)
{
    t_ma_chunk *chunk;

    chunk = (t_ma_chunk*)(region + 1);
    while (chunk)
    {
        if (chunk->free == false)
            return (false);
        chunk = chunk->next;
    }
    return (true);
}

int     count_regions_all()
{
    size_t region_count;
    t_region *region;

    region_count = 0;
    region = g_region;
    while (region)
    {
        region_count++;   
        region = region->next;
    }
    return (region_count);
}

int     count_regions_type(size_t region_type)
{
    size_t region_count;
    t_region *region;

    region_count = 0;
    region = g_region;
    while (region)
    {
        if (get_region_type(region->size) == region_type)
            region_count++;   
        region = region->next;
    }
    return (region_count);
}

void    munmap_region(t_region *region)
{
    g_region = region;
    if (!region->prev && !region->next)
        region = NULL;
    else
    {
        if (region->prev)
            region->prev->next = region->next;
        else if (region->next)
        {
            region = region->next;
            region->prev = NULL;
        }
        if (region->next && region->next->prev)
            region->next->prev = region->prev;
        else if (region->prev)
        {
            region = region->prev;
            region->next = NULL;
        }
    }
    while (region && region->prev)
        region = region->prev;
    munmap(g_region, g_region->size);
    g_region = region;
}

void free_pointer(t_ma_chunk *chunk)
{
    size_t region_type;
    t_region *region;

    region_type = get_region_chunk_type(chunk->size);
    region = get_region(chunk);
    if (chunk->free == false)
    {
        chunk->free = true;
        if (check_region_free(region) == true && (get_region_type(region->size)
        == LARGE || count_regions_type(get_region_type(region->size)) > 1))
            munmap_region(region);
    }
}

void free(void *ptr)
{
    t_ma_chunk *chunk;

    if (ptr == NULL)
        return;
    if ((chunk = search_pointer(g_region, ptr)))
    {
        free_pointer(chunk);
        return;
    }
}