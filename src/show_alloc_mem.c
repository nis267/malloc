/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   show_alloc_mem.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/09 13:37:48 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/10 17:07:20 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

char *print_hexa_helper(char *dest, u_int64_t x)
{
    if (x >= 16)
        dest = print_hexa_helper(dest, (x / 16));
    *dest++ = "0123456789ABCDEF"[x & 15];
    return (dest);
}

void print_hex(u_int64_t x)
{
    char dest[16];

    *print_hexa_helper(dest, x) = '\0';
    ft_putstr("0x");
    ft_putstr(dest);
}

size_t print_chunk_info(t_region *region)
{
    t_ma_chunk *chunk;
    size_t         total;
    size_t          size;

    total = 0;
    chunk = (t_ma_chunk *)region + 1;
    while (chunk)
    {
        if (chunk->free == false)
        {
            print_hex((u_int64_t)(chunk ));
            ft_putstr(" - ");
            print_hex(((u_int64_t)chunk) + chunk->size);
            ft_putstr(" : ");
            size = (((u_int64_t)(chunk )) + chunk->size) - (u_int64_t)chunk;
            ft_putnbr(size);
            ft_putstr(" octets\n");
            total += size;
        }
        chunk = chunk->next;
    }
    return (total);
}

size_t print_region_info(t_region *region, char *type)
{
    ft_putstr(type);
    ft_putstr(" : ");
    print_hex((u_int64_t)region);
    write(1, "\n", 1);
    return (print_chunk_info(region));
}

size_t print_regions(t_region *region, size_t region_type)
{
    size_t total;

    total = 0;
    while (region)
    {
        if (get_region_type(region->size) == region_type)
        {
            if (region_type == TINY)
                total += print_region_info(region, "TINY");
            else if (region_type == SMALL)
                total += print_region_info(region, "SMALL");
            else if (region_type == LARGE)
                total += print_region_info(region, "LARGE");
        }
        region = region->next;
    }
    return (total);
}

void show_alloc_mem(void)
{
    short i;
    size_t region_type;
    size_t total;

    i = -1;
    region_type = 1;
    total = 0;
    while (++i < NB_TYPE_REGION)
    {
        total += print_regions(g_region, region_type);
        region_type = region_type + region_type;
    }
    ft_putstr("Total : ");
    ft_putnbr(total);
    ft_putchar('\n');
}