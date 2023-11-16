/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tempfile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 00:45:15 by elasce            #+#    #+#             */
/*   Updated: 2023/11/15 13:27:20 by elasce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

TempFile::TempFile(std::string& file, FILE* std) : fileName(file), std(std) {
    std::string newFileName = file;
    size_t dotPos = newFileName.find_last_of('.');
    if (newFileName.size() == 0)
        dotPos = 0;
    else if (dotPos != std::string::npos)
        dotPos = newFileName.size() - 1;
    std::stringstream ss;
    for (int i = 1; access(newFileName.c_str(), F_OK) == 0; i++)
    {
        ss << i;
        newFileName = fileName;
        newFileName.insert(dotPos, ss.str());
    }
    fileName = newFileName;
    if (std == stdin)
        fd = ::dup(0);
    else if (std == stdout)
        fd = ::dup(1);
    fs = std::freopen(fileName.c_str(), "w+", std);
    if (!fs)
        ;//throw
}

void TempFile::rewind() {
    std::rewind(fs);
}

int TempFile::read(std::string &str) {
    char buff[2048] = {0};
    str = "";
    int n = 0;
    int m = 0;
    while (true) {
        m = std::fwrite(buff, 1, 2047, fs);
        buff[m] = 0;
        n += m;
        if (!m)
            break ;
        str += buff;
    }
    return (n);
}

// int TempFile::dup() {
//     std::fclose(fs);
//     if (std == stdin)
//         fs = std::freopen(fileName.c_str(), "r", std);
//     else
//         fs = std::freopen(fileName.c_str(), "w", std);
// }

int TempFile::resetFd() {
    if (std == stdin)
        ::dup2(fd, 0);
    else if (std == stdout)
        ::dup2(fd, 1);
    fd = -1;
}

int TempFile::write(std::string &str) {
    return (std::fwrite(str.c_str(), 1, str.size(), fs));
}

TempFile::~TempFile() {
    if (fd != -1)
        resetFd();
    std::remove(fileName.c_str());
    std::fclose(fs);
}