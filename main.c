#include "pipex.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Libft/libft.h"

int get_cmd_len(char *cmd)
{
	int	i;

	i = 0;
	while (cmd && cmd[i] && cmd[i] != 32)
		i++;
	return (i);
}

char *get_path(char *cmd)
{
	int		i;
	char	**paths;
	char	*cmd_path;
	char	*res;

	i = 0;
	cmd_path = malloc(sizeof(char) * (get_cmd_len(cmd) + 2));
	cmd_path[i] = '/';
	while (cmd[i] && cmd[i] != 32 && ++i)
		cmd_path[i] = cmd[i - 1];
	cmd_path[++i] = 0;
	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	res = ft_strjoin(paths[i], cmd_path);
	while (access(res, X_OK) && paths[++i])
	{
		free(res);
		res = ft_strjoin(paths[i], cmd_path);
	}
	//free(cmd_path);
	return (res);
}

int main(int ac, char **av, char **env)
{
	pid_t	pid;
	int		status;
	int		edge[2];
	char	*path;
	char	**option;
	int		files[2];
	pid_t	pids[2];

	files[0] = open(av[1], O_RDONLY);
	files[1] = open(av[4], O_CREAT | O_RDWR, 0666);
	if (ac != 5)
		return (-1);

	pipe(edge);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		usleep(1000);
		pids[0] = getpid();
		path = get_path(av[2]);
		option = ft_split(av[2], ' ');
		printf("path is %s cmd is %s\n", path, option[0]);
		dup2(files[0], 0);
		dup2(edge[1], 1);
		close(edge[0]);
		close(edge[1]);
		execve(path, option, env);
	}
	else if (pid > 0)
	{
		pid = fork();
		if (pid == -1)
			return (1);
		else if (pid == 0)
		{
			//free(path);
			usleep(1000);
			option = ft_split(av[3], ' ');
			path = get_path(av[3]);
			printf("path is %s cmd is %s\n", path, option[0]);
			pids[1] = getpid();
			dup2(edge[0], 0);
			dup2(files[1], 1);
			close(edge[0]);
			close(edge[1]);

			execve(path, option, env);
		}
		else if (pid > 0)
		{
			close(edge[0]);
			close(edge[1]);
			close(files[0]);
			close(files[1]);
			wait(&status);
			wait(&status);
		}
	}
	return (0);
}