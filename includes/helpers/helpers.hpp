/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 19:03:15 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/16 20:02:29 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <cstddef>

// libft
void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);
size_t  ft_strlen(const char *str);
bool    string_is_path(const string path);
int     next_non_space(const string &line, int start);
int     next_space(const string &line, int start);
void    parse_host_string(const string str, string (&info)[2]);
string  parse_size_string(const string str);
bool    string_is_dir(const string str);
bool    parse_codes(const string s_code,t_code_type t_code);