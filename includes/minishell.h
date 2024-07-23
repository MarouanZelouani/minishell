#ifndef MINISHELL_H
#define MINISHELL_H 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../lib/libft/libft.h"

# define QUOTE 39
# define DQUOTE 34

typedef enum
{
    STRING,
    PIPE,
    LESS,
    GREATER,
    AND,
    OR
} t_lexeme;

typedef struct s_token
{
    t_lexeme lexem;
    char *content;
    struct s_token *next;
} t_token;

#endif