#include "pipex.h"

char *get_path(char *cmd) {
	int i;
	char **paths;
	char *cmd_path;
	char *res;

	cmd_path = ft_strjoin("/", cmd);
	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	res = ft_strjoin(paths[i], cmd_path);
	while (access(res, X_OK) && paths[++i]) {
		free(res);
		res = ft_strjoin(paths[i], cmd_path);
		free(paths[i])
	}
	i -= 1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	free(cmd_path);
	return (res);
}
int	get_nb_cmd(t_pipex pipex)
{
	int		i;
	char	**arg;

	i = 0;
	arg = pipex.raw_args + 1;
	while (arg[i])
		i++;
	return (i - 2);
}

void child(t_pipex pipex)
{
	pipex.pid = fork();
	if (pipex.pid == -1)
		return (1);
	if (!pipex.pid)
	{
		if (pipex.crt_cmd == 0)
		{
			dup2(pipex.files[0], 0);
			dup2(pipex.pipe[(2 * pipex.crt_cmd) + 1], 1);
		}
		else if (pipex.crt_cmd == pipex.nb_cmd)
		{
			dup2(pipex.pipe(2 * pipex.crt_cmd - 2), 0);
			dup2(pipex.pipe[(2 * pipex.crt_cmd) + 1], pipex.files[1]);
		}
		else
		{
			dup2(pipex.pipe(2 * pipex.crt_cmd - 2), 0);
			dup2(pipex.pipe[(2 * pipex.crt_cmd) + 1], 1);
		}
		pipex.option = ft_split(pipex.raw_args[2], ' ');
		pipex.path = get_path(pipex.option[0]);
		dup2(pipex.files[0], 0);
		dup2(pipex.pipe[1], 1);
		close(pipex.pipe[0]);
		close(pipex.pipe[1]);
		execve(pipex.path, pipex.option, pipex.env);
	}
}
int main(int ac, char **av, char **env)
{
	t_pipex	pipex;
	int	i;

	i = -1;
	if (ac < 3)
		return (-1);
	pipex.crt_cmd = 1;
	pipex.env = env;
	pipex.raw_args = av;
	pipex.files[0] = open(av[1], O_RDONLY);
	pipex.files[1] = open(av[4], O_CREAT | O_RDWR, 0666);
	pipex.nb_cmd = get_nb_cmd(pipex);
	pipex.crt_cmd = -1;
	pipe(pipex.pipe);
	while (++i < ac -3)
	{
		pipex.pid = fork();
		if (pipex.pid == -1)
			return (1);
		if (pipex.pid == 0)
		{
			usleep(1000);
			pipex.option = ft_split(pipex.raw_args[2], ' ');
			pipex.path = get_path(pipex.option[0]);
			dup2(pipex.files[0], 0);
			dup2(pipex.pipe[1], 1);
			close(pipex.pipe[0]);
			close(pipex.pipe[1]);
			execve(pipex.path, pipex.option, pipex.env);
		}
	}
	pipex.pid = fork();
	if (pipex.pid == -1)
		return (1);
	if (pipex.pid == 0)
	{
		usleep(1000);
		pipex.option = ft_split(pipex.raw_args[2], ' ');
		pipex.path = get_path(pipex.option[0]);
		dup2(pipex.files[0], 0);
		dup2(pipex.pipe[1], 1);
		close(pipex.pipe[0]);
		close(pipex.pipe[1]);
		execve(pipex.path, pipex.option, pipex.env);
	}
	else if (pipex.pid > 0)
	{
		pipex.pid = fork();
		if (pipex.pid == -1)
			return (1);
		else if (pipex.pid == 0)
		{
			usleep(1000);
			pipex.option = ft_split(pipex.raw_args[3], ' ');
			pipex.path = get_path(pipex.option[0]);
			dup2(pipex.pipe[0], 0);
			dup2(pipex.files[1], 1);
			close(pipex.pipe[0]);
			close(pipex.pipe[1]);
			execve(pipex.path, pipex.option, pipex.env);
		}
		else if (pipex.pid > 0)
		{
			close(pipex.pipe[0]);
			close(pipex.pipe[1]);
			close(pipex.files[0]);
			close(pipex.files[1]);
			wait(&pipex.status);
			wait(&pipex.status);
		}
	}
	return (0);
}