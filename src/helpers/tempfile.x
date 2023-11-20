/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tempfile.x                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 00:49:07 by bamrouch          #+#    #+#             */
/*   Updated: 2023/11/19 16:21:06 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// TempFile::TempFile(string& file, FILE* std) : fileName(file), std(std) {
//     string newFileName = file;
//     size_t dotPos = newFileName.find_last_of('.');
//     if (newFileName.size() == 0)
//         dotPos = 0;
//     else if (dotPos != string::npos)
//         dotPos = newFileName.size() - 1;
//     stringstream ss;
//     for (int i = 1; access(newFileName.c_str(), F_OK) == 0; i++)
//     {
//         ss << i;
//         newFileName = fileName;
//         newFileName.insert(dotPos, ss.str());
//     }
//     fileName = newFileName;
//     if (std == stdin)
//         fd = ::dup(STDIN_FILENO);
//     else if (std == stdout)
//         fd = ::dup(STDOUT_FILENO);
//     fs = freopen(fileName.c_str(), "w+", std);
//     if (!fs)
//         ;//throw
// }

// void TempFile::rewind() {
//     std::rewind(fs);
// }

// int TempFile::read(string &str) {
//     char buff[2048] = {0};
//     str = "";
//     int n = 0;
//     int m = 0;
//     while (true) {
//         m = std::fread(buff, 1, 2047, fs);
//         buff[m] = 0;
//         n += m;
//         if (!m)
//             break ;
//         str += buff;
//     }
//     return (n);
// }

// int TempFile::resetFd() {
//     if (std == stdin)
//         ::dup2(fd, STDIN_FILENO);
//     else if (std == stdout)
//         ::dup2(fd, STDOUT_FILENO);
//     fd = -1;
// }

// int TempFile::write(string &str) {
//     return (std::fwrite(str.c_str(), 1, str.size(), fs));
// }

// TempFile::~TempFile() {
//     if (fd != -1)
//         resetFd();
//     remove(fileName.c_str());
//     fclose(fs);
// }