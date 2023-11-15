/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TempFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 00:46:06 by elasce            #+#    #+#             */
/*   Updated: 2023/11/15 13:39:30 by elasce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstdio>

class TempFile {
    private:
    int fd;
    std::FILE* fs;
    std::FILE* std;
    std::string fileName;
    
    public:
    TempFile(std::string& file, FILE* std);
    ~TempFile();
    int write(std::string &str);
    int read(std::string &str);
    void rewind();
    // int dup();
    int resetFd();
};