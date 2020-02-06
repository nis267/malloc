/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   calloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/04 15:53:02 by dewalter     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/04 17:49:54 by dewalter    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void    *calloc(size_t count, size_t size)
{
    void *addr;

    if ((addr = malloc(count * size)))
    {
        addr = ft_memset(addr, 0, count * size);
        // ft_putstr("calloc exit\n");
        return (addr);
    }
    else
    {
        // ft_putstr("calloc exit null\n");
        return (NULL);
    }
}
