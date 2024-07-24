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
# define CPIPE 124
# define GREATER 62
# define LESS 60
# define DOLLAR_SIGN 63

typedef enum
{
    STRING,
    PIPE,
    I_REDIRECTION,
    O_REDIRECTION,
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