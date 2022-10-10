#include <stdio.h>
#include <unistd.h>
#include "Libft/libft.h"

int main(int ac, char **av, char **env)
{
	int		i;
	char	**paths;
	char	*cmd;
	char	*cmd_path;

	cmd_path = ft_strjoin("/", av[1]);
	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	cmd = ft_strjoin(paths[0], av[1]);
	while (access(cmd, X_OK) && paths[++i])
	{
		free(cmd);
		cmd = ft_strjoin(paths[i], cmd_path);
	}
	printf("res = %s\n", cmd);

}
/*
 * tant que i < nb_fils
 * -si nb fils == 0
 * --
 */