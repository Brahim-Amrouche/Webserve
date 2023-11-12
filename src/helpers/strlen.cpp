/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strlen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 09:31:38 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/12 09:33:43 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

size_t ft_strlen(const char *str)
{
    if (!str)
        return 0;
    size_t i = 0;
    while (str[i])
        ++i;
    return i;
}