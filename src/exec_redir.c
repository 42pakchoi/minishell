
#include "minishell.h"

int exec_output_redir(char *command[], t_op op)
{
    int     file_fd;
    int		fds[2];
    char    *line;

    file_fd = open(command[0], O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (file_fd == -1) 
    {
        if (op == OUTPUT)
            file_fd = open(command[0], O_WRONLY | O_TRUNC);
        else
            file_fd = open(command[0], O_WRONLY | O_APPEND);
    }
    pipe(fds);
    dup2(fds[1], STDOUT_FILENO);
    close(fds[1]);
    while (get_next_line(STDIN_FILENO, &line) > 0)
    {
        ft_putstr_fd(line, file_fd);
        ft_putstr_fd("\n", file_fd);
        printf("%s\n", line);
        free(line);
    }
    close(file_fd);
    dup2(fds[0], STDIN_FILENO);
    close(fds[0]);
    restore_ori_stdout();
    return (1);
}

int exec_input_redir(char *command[], t_op op)
{
    int		fds[2];

    pipe(fds);
    if (g_mini.cmd_idx == 1)
        close(STDIN_FILENO);
    dup2(fds[1], STDOUT_FILENO);
    read_fd(STDIN_FILENO);
    if (op == INPUT)
    {
        close(fds[1]);
        print_file(command[0]);
    }
    else
        exec_heredoc(command, fds[1]);
    dup2(fds[0], STDIN_FILENO);
    close(fds[0]);
    close(STDOUT_FILENO);
    restore_ori_stdout();
    return (1);
}