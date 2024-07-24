#include "../includes/minishell.h"

t_token	*ft_lstlast_token(t_token *lst)
{
	t_token	*node;

	node = NULL;
	while (lst)
	{
		if (lst->next == NULL)
			node = lst;
		lst = lst->next;
	}
	return (node);
}


void	ft_lstadd_token_back(t_token **lst, t_token *new)
{
	t_token	*token;

	if (new != NULL)
	{
		if (*lst == NULL)
			*lst = new;
		else
		{
			token = *lst;
			while (token->next != NULL)
				token = token->next;
			token->next = new;
		}
	}
}

t_lexeme set_lexem(char *content)
{
    
    // set token lexeme
    // check is special chat insid a string
    // remove double quote
}

t_token	*ft_lstnew_token(void *content)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (token == NULL)
		return (0);
    token->lexem = STRING;
	token->content = content;
	token->next = NULL;
	return (token);
}

void print_lst(t_token *tokens)
{
    int i;

    i = 1;
    while (tokens != NULL)
    {
        printf("token %d ==> %s|\n", i, tokens->content);
        i++;
        tokens = tokens->next;
    }
}

char *remove_quote(char *holder, char quote_type)
{
    char *new;
    int i;
    int j;

    i = 0;
    j = 0;
    if (holder == NULL)
        return NULL;
    new = malloc(sizeof(char) * ft_strlen(holder));
    while (holder[i])
    {
        if(holder[i] == quote_type)
            i++;
        new[j++] = holder[i++];
    }
    new[j] = '\0';
    free(holder);
    return (new);
}

void print_error(char *error, int exit_code)
{
    ft_putstr_fd(error, 2);
    exit(exit_code);
}

int check_for_echo(t_token *tokens)
{
    char *holder;
    int i;

    i = 0;
    holder = ft_strdup(ft_lstlast_token(tokens)->content);
    while (holder[i] && (holder[i] != DQUOTE && holder[i] != QUOTE))
        i++;
    holder = remove_quote(holder, holder[i]);
    holder = ft_strtrim(holder, " \t\v\n\r\f");
    if (!ft_strncmp("echo", holder, 4) && ft_strlen(holder) == 4)
    {
        free(holder);
        return 1;
    }
    free(holder);
    return 0;
}

t_token *tokenizer(char *line)
{
    t_token *tokens;
    char *holder;
    int quote_flag;
    char quote_type;
    int stop_flag;
    int i;
    int j;
    
    i = 0;
    stop_flag = 0;
    tokens = NULL;
    while (line[i])
    {
        if (tokens != NULL && check_for_echo(tokens))
        {
            stop_flag = 0;
            j = i;
            while (line[i] && stop_flag == 0)
            {
                if (quote_flag == 0  && (line[i] == QUOTE || line[i] == DQUOTE))
                {
                    quote_flag = 1;
                    quote_type = line[i++];
                }
                if (quote_flag == 1 && line[i] == quote_type)
                    quote_flag = 0;
                if (stop_flag == 0 && (line[i] == CPIPE || line[i] == GREATER
                    || line[i] == LESS))
                {
                    stop_flag = 1;
                    break;
                }
                i++;
            }
            if (quote_flag == 1)
                print_error("close quote\n", 1);
            if (stop_flag == 1)
                i--;
            holder = ft_substr(line, j, i - j);
            // if (ft_strchr(holder, DQUOTE))
            //     holder = remove_quote(holder, DQUOTE);
            // else if (ft_strchr(holder, QUOTE))
            //     holder = remove_quote(holder, QUOTE);
            ft_lstadd_token_back(&tokens, ft_lstnew_token(holder));
            
        }
        else if (line[i] && !is_space(line[i]))
        {
            j = i;
            while (line[j])
            {
                if (quote_flag == 0 && (line[j] == QUOTE || line[j] == DQUOTE))
                {
                    quote_flag = 1;
                    quote_type = line[j];
                    j++;
                }
                if (quote_flag == 1 && line[j] == quote_type)
                {
                    quote_flag = 0;
                    if (line[j + 1] && is_space(line[j + 1]))
                    {
                        j++;
                        break;
                    }
                }
                if (is_space(line[j]) && quote_flag == 0)
                    break;
                j++;
            }
            if (quote_flag == 1)
                print_error("close quote\n", 1); // quote close
            holder = ft_substr(line, i, j - i);
            // if (ft_strchr(holder, DQUOTE))
            //     holder = remove_quote(holder, DQUOTE);
            // else if (ft_strchr(holder, QUOTE))
            //     holder = remove_quote(holder, QUOTE);
            ft_lstadd_token_back(&tokens, ft_lstnew_token(holder));
            i = j;
        }
        i++;
    }
    return (tokens);
}

int count_repetition(char *line, char c, int itr)
{
    int i;
    int count;

    i = itr;
    count = 0;
    while (line[i])
    {
        if (line[i] == c)
            count++;
        else 
            break;
        i++;
    }
    return count;
}

int count_special_chars(char *line)
{
    int count;
    int flag;
    int i;

    i = 0;
    flag = 0;
    count = 0;
    while(line[i])
    {
        if (flag == 0 && (line[i] == DQUOTE ||  line[i] == QUOTE))
            flag = 1;
        else if (flag == 1 && (line[i] == DQUOTE ||  line[i] == QUOTE))
            flag = 0;

        if (flag == 0 && (line[i] == CPIPE || line[i] == GREATER
            || line[i] == LESS))
        {
            if (line[i] == GREATER)
            {
                if (count_repetition(line, line[i], i) > 2)
                    print_error("parse error\n", 1);
                else if (line[i + 1] && line[i + 1] == LESS)
                    print_error("parse error\n", 1);
                else if (count_repetition(line, line[i], i) == 2)
                    count++;
                else if (line[i - 1] != GREATER)
                    count++;
            }
            else if (line[i] == LESS)
            {
                if (count_repetition(line, line[i], i) > 2)
                    print_error("parse error\n", 1);
                else if (line[i + 1] && line[i + 1] == GREATER)
                    print_error("parse error\n", 1);
                else if (count_repetition(line, line[i], i) == 2)
                    count ++;
                else if (line[i - 1] != LESS)
                    count++;
            }
            else if (line[i] == CPIPE)
            {
                if (count_repetition(line, line[i], i) > 2)
                    print_error("parse error\n", 1);
                else if (count_repetition(line, line[i], i) == 2)
                    count ++;
                else if (line[i - 1] != CPIPE)
                    count++;
            }
        }
        i++;
    }
    return count;
}

char *modify_line(char *line)
{
    int i;
    int j;
    int flag;
    char *new_line;

    i = 0;
    j = 0;
    flag = 0;
    new_line = malloc(sizeof(char) * (ft_strlen(line) + count_special_chars(line) * 2 + 1));
    if (new_line == NULL)
        return NULL;
    while (line[i])
    {
        if (flag == 0 && (line[i] == DQUOTE ||  line[i] == QUOTE))
            flag = 1;
        else if (flag == 1 && (line[i] == DQUOTE ||  line[i] == QUOTE))
            flag = 0;
        if (flag == 0 && (line[i] == CPIPE || line[i] == GREATER
            || line[i] == LESS))
        {
            new_line[j++] = ' ';
            if (line[i + 1] && line[i + 1] != line[i] && line[i - 1] != line[i])
                new_line[j++] = line[i];
            else
            {
                new_line[j++] = line[i++];
                new_line[j++] = line[i];
            }
            new_line[j] = ' ';
        }
        else 
            new_line[j] = line[i];
        i++;
        j++;
    }
    new_line[j] = '\0';
    free(line);
    return new_line;
}

void initial_parsing(char *line)
{
    int i;

    i = 0;
    while (line[i] && is_space(line[i]))
        i++;
    if(line[i] && line[i] == CPIPE)
        print_error("parse error\n", 1);
    while(line[i++]);
    i -= 2;
    while (i >= 0 && is_space(line[i]))
        i--;
    if (line[i] == CPIPE || line[i] == GREATER || line[i] == LESS)
        print_error("parse error\n", 1);
}

int main (int ac, char **av)
{
    char *line;
    t_token *tokens;

    (void)ac;
    (void)av;
    while (1)
    {
        line = readline("-> ");
        initial_parsing(line);
        line = modify_line(line);
        tokens = tokenizer(line);
        print_lst(tokens);
        // printf("%s\n", line);
        // exit(1);
        free(line);
    }   
    return (0);
}
