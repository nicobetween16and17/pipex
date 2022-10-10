#ifndef PIPEX_H
# define PIPEX_H
# include "Libft/libft.h"
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
#include <stdio.h>
typedef struct s_pipex
{
	pid_t	pid;
	int		status;
	int		nb_cmd;
	int		pipe[2];
	char	*path;
	char	**option;
	char	*cmd;
	int		files[3];
	char	**env;
	char	**raw_args;
	int		crt_cmd;
}	t_pipex;

#endif