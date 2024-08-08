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
    ARG,
    PIPE,
    F,
    CMD,
    I_REDIRECTION,
    O_REDIRECTION,
    HERDOC,
    APPEND,
    AND,
    OR
} t_lexeme;

typedef enum 
{
    PIPELINE,
    COMMAND,
    REDIRECTION,
    ARGUMENTS
} t_node_type;

typedef struct s_token
{
    t_lexeme lexem;
    char *content;
    struct s_token *next;
} t_token;

typedef struct s_argument
{
    char *content;
    struct s_argument *next;
} t_argument;

// ast struct
typedef struct s_ast_node
{
    t_node_type type;
    union 
    {
        t_argument *arg_list;
        struct 
        {
            struct s_ast_node *left;
            struct s_ast_node *right;
        } childs;
    } data;
} t_ast_node;


#endif