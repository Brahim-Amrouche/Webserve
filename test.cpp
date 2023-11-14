/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 23:27:32 by maboulkh          #+#    #+#             */
/*   Updated: 2023/11/14 22:28:04 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <unistd.h>
// #include <sys/wait.h>
#include <map>

// using std::cout;
// using std::endl;
// using std::tmpfile;
// using std::string;
// using std::ofstream;
// using std::ifstream;
// using std::ios;

int main(int argc, char **argv, char **envp) {
    // ofstream out;
    // ifstream in("inFile");
    (void) argc;
    (void) argv;
    (void) envp;

    // out.open("outFile", ios::out | ios::trunc);
    // in.open("inFile", ios::in);
    FILE * file2 = std::freopen("inFile", "w+", stdin);
    FILE * file3 = std::freopen("outFile", "w+", stdout);
    if (!file2 || !file3)
        std::cout << "can t open file" << std::endl;
    std::remove("inFile");

    char buff[100];
    char buff2[100];

    for (int i = 0; i< 99; i++)
        buff[i] = 'a';
    buff[99] = 0;

    for (int i = 0; i < 10000000; i++)
        write(0, buff, 100);

    std::rewind(file2);
    for (int i = 0; i < 10000000;i++)
    {
        read(0, buff2, 100);
        write(1, buff2, 100);
    }
    std::cout << "hello2" << std::endl;
    std::cout << buff2 << std::endl;
    // FILE* file1 = tmpfile();

    // int fd = fileno(file1);
    // string buff;

    // buff = "hello world \n";
    // buff += "\n";
    // buff += "hello there\n";
    // buff += "jdsfkljdlsksd\n";
    // buff += "\n";

    // int fd = fileno(file1);
    // fd++;

    // char buff[100];

    // for (int i = 0; i< 99; i++) 
    //     buff[i] = 'a';
    // buff[99] = 0;
    // int fd[2];
    // pipe(fd);
    // int pid = fork();
    // if (!pid)
    // {
    //     close(fd[1]);
    //     read(fd[0], buff, 100);
    //     std::cout << buff << std::endl;
    //     std::cout << "done" << std::endl;
    //     return (0);
    // }
    // close(fd[0]);
    // std::cout << "parent begin " << std::endl;
    // int i = 0;
    // while (true) {
    //     std::cout << "parent at  " << i << std::endl;
    //     write(fd[1], buff, 100);
    //     i++;
    // }
    // std::cout << "parent end " << std::endl;
    // write(file1->_fileno, buff.c_str(), buff.size());
    // fseek(file1, 0, SEEK_SET);
    // int fd = fork();
    // if (!fd)
    // {
    //     // char buff[1000];
    //     // while (in.read(buff, 1000))
    //     // {
    //     //     if (in.eof())
    //     //         break;
    //     //     write(0, buff, 20);
    //     // }
    // in.copyfmt(std::cin); // copy everything except rdstate and rdbuf
    // in.clear(std::cin.rdstate()); // copy rdstate
    // in.basic_ios<char>::rdbuf(std::cin.rdbuf()); // share the buffer
    // // std::streambuf *originalCinBuffer = std::cin.rdbuf(in.rdbuf());
    // // std::streambuf *originalCoutBuffer = std::cout.rdbuf(out.rdbuf());
    // // std::freopen("inFile", "r", stdin);
    // dup2(file1->_fileno, 0);
    // char *arg[4];
    // arg[0] = (char *) "/usr/bin/grep";
    // arg[1] = (char *) "hello";
    // arg[2] = 0;

    // execve(arg[0], arg, envp);
    
    // }
    // waitpid(fd, NULL, 0);
    // cout << "done" << endl;
    
}
