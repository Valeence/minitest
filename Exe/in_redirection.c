#include "../Include/MiniShell.h"
#include "../Include/MiniShell.h"

int *creat_fd_out_in_in(t_data *data, t_Token *curr) {
    int i = 0;
    t_Token *tmp = curr;

    while (tmp->next) {
        if (search_redi(tmp)) {
            tmp->next->next->file = ft_split(tmp->next->next->Token_str, ' ');
            if (tmp->next->type == E_OUT) {
                data->output_fd[i] = open(tmp->next->next->file[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (data->output_fd[i] == -1) {
                    printf("bash: %s: Permission denied\n", tmp->next->next->file[0]);
                    gvalret = 1;
                    exit(1);
                }
                i++;
            }
        }
        tmp = tmp->next;
    }
    return data->output_fd;
}

bool shr_out_in_redi_2(t_Token *curr) {
    t_Token *shr_out = curr;
    int flag = 0;

    while (shr_out && shr_out->next) {
        if (shr_out->type == E_PIPE) flag++;
        if (shr_out->type == E_OUT || shr_out->type == E_APPEND) flag++;
        if (flag == 2) return true;
        shr_out = shr_out->next;
    }
    return false;
}

bool shr_out_in_redi_3(t_Token *curr) {
    t_Token *shr_out = curr;

    while (shr_out && shr_out->next) {
        if (shr_out->type == E_PIPE) return false;
        if (shr_out->type == E_OUT || shr_out->type == E_APPEND) return true;
        shr_out = shr_out->next;
    }
    return false;
}

void free_for_fail_exe2(t_data *data, t_Token *curr) {
    if (data->cmd[0] && data->cmd[0][0] != '\0') ft_free_tabtab(data->cmd);
    free(data->path_exe);
    free_token(curr);
}

void close_fd_in(t_data *data) {
    for (int i = 0; i < data->nb_file_in; i++) {
        close(data->in_fd[i]);
    }
    free(data->in_fd);
}

int *creat_fd_in(t_data *data, t_Token *curr) {
    int i = 0;
    t_Token *tmp = curr;

    data->in_fd = malloc(data->nb_file_in * sizeof(int));
    if (!data->in_fd) {
        perror("malloc");
        exit(1);
    }

    while (i < data->nb_file_in) {
        if (tmp->next->next->type == E_WORD) {
            tmp->next->file = ft_split(tmp->next->Token_str, ' ');
            data->in_fd[i] = open(tmp->next->file[0], O_RDONLY);
            if (data->in_fd[i] == -1) {
                printf("bash: %s : No such file or directory\n", tmp->next->file[0]);
                exit(126);
            }
            if (tmp->next->file[1] && tmp->next->file[1][0] != '\0') {
                data->cmd = copy_array_arrays(tmp->next->file);
            }
            i++;
        } else if (tmp->next->next->type == E_FILE) {
            tmp->next->next->file = ft_split(tmp->next->next->Token_str, ' ');
            data->in_fd[i] = open(tmp->next->next->file[0], O_RDONLY);
            if (data->in_fd[i] == -1) {
                printf("bash: %s : No such file or directory\n", tmp->next->next->file[0]);
                exit(126);
            }
            if (tmp->next->next->file[1] && tmp->next->next->file[1][0] != '\0') {
                data->cmd = copy_array_arrays(tmp->next->next->file);
            }
            i++;
        }
        tmp = tmp->next->next;
    }
    return data->in_fd;
}

pid_t fork_process_redi_in(t_data *data, t_Token *curr, t_Token *lst_tok) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // Child process
        if (data->redi_out_bf_pipe == 1 && !shr_out_in_redi_2(curr)) {
            close(data->pipe_fd[1]);
            dup2(data->pipe_fd[0], STDIN_FILENO);
            close(data->pipe_fd[0]);
        } else {
            data->in_fd = creat_fd_in(data, curr);
            if (shr_out_in_redi_2(curr)) {
                exit(0);
            } else if (shr_out_in_redi_3(curr)) {
                data->output_fd = creat_fd_out_in_in(data, curr);
                dup2(data->in_fd[data->nb_file_in - 1], STDIN_FILENO);
                dup2(data->output_fd[data->nb_file_out - 1], STDOUT_FILENO);
            } else {
                dup2(data->in_fd[data->nb_file_in - 1], STDIN_FILENO);
            }
        }
        if (!check_builtins(data, curr, data->envp)) {
            close_fd_in(data);
            free_token(curr);
            exit(0);
        }
        if (curr->type == E_IN && curr->next->type == E_FILE && curr->next->next->type == E_WORD) {
            data->path_exe = ft_get_reading(data, curr->next->next);
        } else if (curr->type == E_WORD) {
            data->path_exe = ft_get_reading(data, curr);
        }
        if (!data->path_exe) {
            ft_no_patch(data, lst_tok);
        }
        if (execve(data->path_exe, data->cmd, data->envp) == -1) {
            close_fd_in(data);
            free_for_fail_exe_exit(data, curr);
        }
        free_for_fail_exe2(data, curr);
        close_fd_in(data);
    }
    return pid;
}
