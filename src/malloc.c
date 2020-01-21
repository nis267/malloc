#include "../include/malloc.h"
#include <stdio.h>

t_ma_heap g_heap = {NULL, NULL, NULL};

size_t      get_chunk_size(size_t size)
{
    size = size + sizeof(t_ma_chunk);
    while ((size % 16) != 0)
    {
        size = size + 1;
    }
    return (size);
}

int         get_region_type(size_t chunk_size)
{
    if (chunk_size <= TINY_BYTE_MAX)
    {
        return (TINY);
    }
    else if (chunk_size <= SMALL_BYTE_MAX)
    {
        return (SMALL);
    }
    else
    {
        return (LARGE);
    }
}

// void        add_new_region_node()
// {

// }

void        add_new_region(int region_type, void *region, size_t chunk_size)
{
    void *first;

    if (region_type == TINY)
    {
        if (g_heap.tiny == NULL)
        {
            g_heap.tiny = region;
            g_heap.tiny->remaining = TINY_MMAP_SIZE - sizeof(t_region);
        }
        else
        {
            while (g_heap.tiny && g_heap.tiny->next)
            {
                g_heap.tiny = g_heap.tiny->next;
            }
        }
        // printf("pointer tiny: %p\n", g_heap.tiny);
    }
    // else if (region_type == SMALL)
    // {

    // }
    // else
}

int         get_mmap_region(size_t chunk_size, int region_type)
{
    // printf("t_region: %lu\n", sizeof(t_region));
    size_t  mmap_region_size;
    void    *region;

    if (region_type == TINY)
        mmap_region_size = TINY_MMAP_SIZE;
    else if (region_type == SMALL)
        mmap_region_size = SMALL_MMAP_SIZE;
    else
        mmap_region_size = chunk_size;
    if ((region_type == TINY && (g_heap.tiny == NULL || g_heap.tiny->remaining < chunk_size))
    || (region_type == SMALL && (g_heap.small == NULL || g_heap.small->remaining < chunk_size))
    || region_type == LARGE)
    {
        if ((region = mmap(0, mmap_region_size, PROT_READ |
        PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1 , 0)) == MAP_FAILED)
            return (1);
        add_new_region(region_type, region, chunk_size);
    }
    return (0);
}

void        add_new_chunk_node()
{

}

void        *add_new_chunk(size_t chunk_size, int region_type)
{
    t_ma_chunk  *temp;
    void        *addr_data;
    printf("s_region: %lu\n", sizeof(t_region));
    if (region_type == TINY)
    {
        // if ()
        temp = (t_ma_chunk*)(g_heap.tiny + 1);
        if (temp->next == NULL)
        {
            addr_data = (void*)(temp + 1);
            temp->size = chunk_size - sizeof(t_ma_chunk);
            printf("temp size: %d\n", temp->size);
            printf("chunk size: %zu\n", chunk_size);
            printf("bool free: %d\n", temp->free);
            printf("addr temp next: %p\n", temp->next);
            printf("addr data: %p\n", addr_data);
        }
        printf("chunk: %p\n", temp->next);
    }
    else if (region_type == SMALL)
    {

    }
    else
    {

    }
    return (addr_data);
}

void        *ft_malloc(size_t size)
{
    void *addr;
    size_t chunk_size;
    int region_type;

    addr = NULL;
    if ((long long int)size < 0)
        return (NULL);
    chunk_size = get_chunk_size(size);
    region_type = get_region_type(chunk_size);
    if (get_mmap_region(chunk_size, region_type) == 1)
        return (NULL);
    add_new_chunk(chunk_size, region_type);
    return addr;
}

int main(int ac, char **av)
{
    if (ac == 2 && ((av[1][0] == '-' && av[1][1] >= 48 && av[1][1] <= 57) || (av[1][0] >= 48 && av[1][0] <= 57)))
    {
        printf("ft_malloc: %p\n", ft_malloc(atoll(av[1])));
        printf("malloc: %p\n", malloc(atoll(av[1])));
    }
    return (0);
}