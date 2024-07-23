#include "../includes/minishell.h"

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

// t_lexeme get_lexem(char *content)
// {
//     if (content)
// }

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

//CHECK ""
void handle_quotes(t_token **tokens, char *line, int *iter)
{
    int i;
    int j;
    int flag;
    char *holder;
    char quote_type;

    i = iter;
    flag = 0;
    while (line[i])
    {
        if (line[i] == QUOTE || line[i] == DQUOTE)
        {
            j = i;
            quote_type = line[i];
            i++;
            while (line[i] && line[i] != quote_type)
                i++;  
            if (line[i] && line[i] == quote_type)
            {
                flag = 1;
                if (!is_space(line[i + 1]))
                {
                    flag = 2;
                    while (line[i] && !is_space(line[i]))
                        i++;
                }
            }
            if (flag == 1)
            {
                holder = ft_substr(line, j + 1, i - j - 1);
                ft_lstadd_token_back(tokens, ft_lstnew_token(holder));
                *iter = i + 1; 
                return ;
            }
            else if (flag == 2)
            {
                holder = ft_substr(line, j + 1, i - j - 1);
                // remove quote from the string
                holder = remove_quote(holder, quote_type);
                ft_lstadd_token_back(tokens, ft_lstnew_token(holder));
                *iter = i + 1; 
                return ;
            }
            else
            {
                printf("error\n");
                exit(1); // no closing quote
            }
        }
        i++;
    }
}

t_token *tokenizer(char *line)
{
    t_token *tokens;
    char *holder;
    int quote_flag;
    char quote_type;
    int i;
    int j;
    
    i = 0;
    quote_flag = 0;
    tokens = NULL;
    while (line[i])
    {
        if (line[i] == QUOTE || line[i] == DQUOTE)
        {
            printf("found");
            // quote_flag = 1;
            // quote_type = line[i];
            if (i == 0 || is_space(line[i - 1]))
            {
                handle_quotes(&tokens, line, &i);
                quote_flag = 0;
            }
        }
        // if (line[i] && quote_flag == 1)
        // {
        //     printf("handling quotes\n");
        //     j = i;
        //     while(line[j] && quote_flag == 1)
        //     {
        //         if (line[i] == quote_type)
        //         {
        //             quote_flag = 0;
        //             break;
        //         }
        //         j++;
        //     }
        //     if (quote_flag == 1)
        //         exit(1); // quote error
        //     holder = ft_substr(line, i, j - i);
        //     if (ft_strchr(holder, DQUOTE))
        //         holder = remove_quote(holder, DQUOTE);
        //     else if (ft_strchr(holder, QUOTE))
        //         holder = remove_quote(holder, QUOTE);
        //     ft_lstadd_token_back(&tokens, ft_lstnew_token(holder));
        //     i = j;
        // }
        if (line[i] && !is_space(line[i]))
        {
            j = i;
            while (line[j])
            {
                // printf("%c\n", line[j]);

                if (quote_flag == 0 && (line[j] == QUOTE || line[j] == DQUOTE))
                {
                    quote_flag = 1;
                    quote_type = line[j];
                    j++;
                }

                if (is_space(line[j]) && quote_flag == 0)
                {
                    printf("space\n");
                    break;
                }


                if (quote_flag == 1 && line[j] == quote_type)
                {
                    quote_flag = 0;
                    if (line[j + 1] && is_space(line[j + 1]))
                    {
                        // printf("========\n");
                        // printf("%c\n", line[j + 1]);
                        printf("break\n");
                        j++;
                        break;
                    }
                }

                // if (quote_flag == 1 && line[j + 1] && line[j + 1] == quote_type && (line[j + 2] || is_space(line[j + 2])))
                // {
                //     j++;
                //     printf("quote\n");
                //     quote_flag = 0;
                //     break;
                // }
                j++;
            }
            // exit(1);
            // while(line[j] && !is_space(line[j]))
            // {
            //     j++;
            // }
            if (quote_flag == 1)
                exit(1); // quote close
            holder = ft_substr(line, i, j - i);
            if (ft_strchr(holder, DQUOTE))
                holder = remove_quote(holder, DQUOTE);
            else if (ft_strchr(holder, QUOTE))
                holder = remove_quote(holder, QUOTE);
            ft_lstadd_token_back(&tokens, ft_lstnew_token(holder));
            i = j;
        }
        i++;
    }
    return (tokens);
}

// void check_quote(char *line)
// {
//     // parse quotes
// }

// char *quotes_handling(char *line)
// {
//     char *new;
//     int i;

//     i = 0;
//     while (line[i])
//     {
//         if (line[i] == QUOTE || line[i] == DQUOTE)
//         {
//             if (line[i] == DQUOTE)
//             {

//             }
//             else if (line[i] == QUOTE)
//             {

//             }
//         }
//         i++;
//     }
//     return new;
// }

// void initial_parsing(char *line)
// {
//     char *new;
//     int i;


//     i = 0;
//     while (line[i])
//     {
//         if (line[i] == QUOTE || line[i] == DQUOTE)
//             new = quotes_handling(line);
//         i++;
//     }
// }

int main (int ac, char **av)
{
    char *line;
    t_token *tokens;

    (void)ac;
    (void)av;
    while (1)
    {
        line = readline("-> ");
        // initial_parsing(line);
        tokens = tokenizer(line);
        print_lst(tokens);
        // printf("%s\n", line);
        exit(1);
        free(line);
    }   
    return (0);
}
