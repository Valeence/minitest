#include "../Include/MiniShell.h"

bool ft_isspace(char c) 
{
    if(c ==   32 || c ==   9)
		return (true);
	return false;
}

bool ft_is_del(char c) 
{
    if(c == '|' || c == '<' || c == '>')
		return (true);
	return false;
}

bool ft_is_qote(char c)
{
	if (c ==  '\'' || c ==  '"')
		return (true);
	return false;
}

int	skip_begin_spaces_prompt(const char *readline, int start)
{
	while (ft_isspace(readline[start]))
		start++;
	return (start);
}

int	extract_token(const char *readline, int start, char **token, t_data *data)
{
	int	end;

	end = start;
	while (readline[end])
	{	
		if(readline[end] == 32 || readline[end] == 9)
			end++;
		if (readline[end] == '\'')
            data->in_single_quote = !data->in_single_quote;
        else if (readline[end] == '"')
            data->in_double_quote = !data->in_double_quote;	
		if(ft_is_del(readline[end]) && (data->in_single_quote == 1 || data->in_double_quote == 1))
				end++;
		else if(ft_is_del(readline[end]) && (data->in_single_quote == 0 || data->in_double_quote == 0))
			break;
		end++;	
	}
	*token = ft_strndup(readline + start, end - start);
	return (end);
}