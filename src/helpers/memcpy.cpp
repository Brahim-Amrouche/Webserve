/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 16:11:04 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/05 16:12:09 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*c_dst;
	unsigned char	*c_src;
	size_t	i;

	if (!dst && !src && n)
		return (dst);
	c_dst = (unsigned char *)dst;
	c_src = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		c_dst[i] = c_src[i];
		i++;
	}
	return (dst);
}