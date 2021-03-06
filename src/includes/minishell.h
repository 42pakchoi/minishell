/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpak <cpak@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 13:26:10 by sarchoi           #+#    #+#             */
/*   Updated: 2022/05/11 17:00:36 by cpak             ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <termios.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include "../../libft/libft.h"

# define  FT_SUCCESS	1
# define  FT_ERROR		-1
# define  FT_TRUE		1
# define  FT_FALSE		0

# define  PROMPT_STRING		" $ "
# define  PROMPT_HEREDOC	"> "
# define  PROMPT_COLOR_PWD		"\001\e[01;32m\002"
# define  PROMPT_COLOR_PROMPT	"\001\e[01;35m\002"
# define  PROMPT_COLOR_RESET	"\001\e[0m\002"

typedef enum e_op
{
	PIPE =		1,
	INPUT =		2,
	INPUT_D =	3,
	OUTPUT =	4,
	OUTPUT_D =	5
}	t_op;

typedef struct s_cmd
{
	char			**strarr;
	t_op			op;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}					t_cmd;

typedef struct s_var
{
	char			*var;
	int				scope;
	struct s_var	*next;
}					t_var;

/*
** value for `scope` the member of structure `t_var`
*/
# define  SHELL_VAR	0
# define  ENV_VAR	1
# define  EXPORT_VAR	2

typedef struct s_history
{
	char			*prev_input;
	int				fd;
}					t_history;

typedef struct s_minishell
{
	int				is_main_process;
	t_history		history;
	char			*prompt_str;
	char			*prompt_input;
	t_cmd			*cmd;
	int				cmd_len;
	t_var			*env;
	int				exit_status;
	int				has_redir_error;
	int				has_token_error;
}					t_minishell;

t_minishell	g_mini;

void	free_global(void);
void	print_welcome(void);

/*
** env
*/
void	init_env(int argc, char **argv, char **envp);

/*
** signal
*/
void	sigint_heredoc_handler(int signo);
void	init_signal(int is_child_process);
void	eof_handler(void);

/*
** prompt
*/
int		deal_prompt(void);

/*
** cmd
*/
int		deal_command(void);
int		exec_multi_cmd(void);
void	write_prompt_to_heredoc(char *delimiter);
void	set_exit_status(int status);
int		exec_cmd_node(t_cmd *cmd, int has_std);

/*
** builtin
*/
void	ft_env(void);
void	ft_unset(char **cmds);
void	ft_export(char **cmds);
void	ft_exit(char **cmds);
void	ft_echo(char **cmds);
void	ft_cd(char **cmds);
void	ft_pwd(void);

/*
** parse
*/
int		is_quote(char c);
int		is_op(char *str, int *sep_num);
int		is_white_space(char *str, int *sep_num);

void	set_quotes_flag(char c, int *flag);
char	*change_str(char *str, char *str_tar, char *str_src, int idx);
int		parse_cmd(char ***strarr);
int		parse_str(char **str);
int		trans_all_env(char **str);
int		parse_prompt_input(char *str);
int		check_quote_closed(char *str, int *flag);

/*
** split
*/
int		get_len_to_next(char **str, int (*sep_func)(char *, int *));
char	**split_with_quote(char *str, int (*sep_func)(char *, int *));

/*
** history
*/
int		init_history(void);
int		save_history(void);

/*
** pipex
*/
int		set_fd_to_stdin(int fd);
int		set_pipein_to_stdout(int *fds);
int		set_pipeout_to_stdin(int *fds);
int		set_fileout_to_fd(char *path, int fd);
int		set_filein_to_fd(char *path, int fd);
void	read_fd(int fd);
int		print_file(char *path);

void	set_self_pipe(void);
void	keep_ori_std(void);
void	restore_ori_stdin(void);
void	restore_ori_stdout(void);

int		exec_assign(char **command, t_cmd *cmd);
int		exec_builtin(char **cmd, int n);
int		exec_execve(char **command);
int		exec_output_redir(char *command[], t_op op, int has_std);
int		exec_input_redir(char *command[], t_op op, int has_std);
int		exec_heredoc(void);

int		is_builtin(char **cmd);
int		is_assign_cmd(char **cmd);

/*
** util: cd
*/
int		valid_directory(char *path);
int		valid_permission(char *path);
int		has_directory(char *path);
void	set_pwd_env(void);

/*
** util: execve
*/
int		valid_executable(char *path);
int		valid_execute_permission(char *path);

/*
** util: var
*/
t_var	*find_var(char *name);
char	*find_var_value(char *name);
void	remove_var(char *name);
void	remove_var_list(void);
void	add_var(char *name_and_value, int scope);
void	update_var(char *name, char *new_value);
char	**find_all_env(void);
int		is_valid_var_name(const char *str);

/*
** util: export
*/
void	print_sorted_env(void);

/*
** util: print
*/
void	print_strerror(char *cmd_str);
void	print_error(char *cmd_str, char *msg);
void	print_error2(char *cmd_str, char *arg_str, char *msg);
void	print_cwd(void);

/*
** util: cmd
*/
int		add_cmd(char **strarr, t_op op);
void	remove_cmd_list(void);
t_cmd	*find_nth_cmd(int idx);
t_cmd	*find_last_cmd(void);
void	read_cmd_list(void);
void	read_arr(char **arr);
t_cmd	*find_cmd_has_heredoc(void);

/*
** util: exit
*/
void	exit_with_status(unsigned char status);

#endif
