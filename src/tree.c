#include "../includes/minishell.h"


char *lexs[] = 
{
    "STRING",
    "ARG",
    "PIPE",
    "FILE",
    "CMD",
    "I_RED",
    "O_RED",
    "HERDOC",
    "APPEND",
    "AND",
    "OR"
};

char *types[] = 
{
    "PIPELINE",
    "COMMAND",
    "REDIRECTION",
    "ARGUMENTS"
};

void	ft_argsadd_back(t_argument **lst, t_argument *new)
{
	t_argument	*node;

	if (new != NULL)
	{
		if (*lst == NULL)
			*lst = new;
		else
		{
			node = *lst;
			while (node->next != NULL)
			{
				node = node->next;
			}
			node->next = new;
		}
	}
}

t_argument	*ft_argsnew(void *content)
{
	t_argument	*node;

	node = (t_argument *)malloc(sizeof(t_argument));
	if (node == NULL)
		return (0);
	node->content = content;
	node->next = NULL;
	return (node);
}

// ============================================================================ //

t_ast_node *alloc_ast_node(t_node_type type, t_argument *args)
{
    t_ast_node *new_node;

    new_node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (new_node == NULL)
        return NULL;
    new_node->type = type;
    if (args != NULL)
        new_node->data.arg_list = args;
    return new_node;
}

int redirection_found(t_token *tokens)
{
    while (tokens != NULL && tokens->lexem != PIPE)
    {
        if (tokens->lexem == I_REDIRECTION || tokens->lexem == O_REDIRECTION 
            || tokens->lexem == HERDOC || tokens->lexem == APPEND)
        {
            return 1;
        }
        tokens = tokens->next;
    }
    return 0;
}

int is_schar (t_token *token)
{
    if (token->lexem == O_REDIRECTION || token->lexem == I_REDIRECTION 
        || token->lexem == HERDOC || token->lexem == APPEND
        || token->lexem == PIPE)
    {
        return 1;
    }
    return 0;
}

t_argument *args_table(t_token *tokens, t_node_type type)
{
    t_argument *args;

    args = NULL;
    while (tokens != NULL && tokens->lexem != PIPE)
    {
        if (type == ARGUMENTS)
        {
            if (!is_schar(tokens))
                return args;
        }
        ft_argsadd_back(&args, ft_argsnew(ft_strdup(tokens->content)));
        tokens = tokens->next;
    }
    return args;
}

int pipe_exist(t_token *tokens)
{
    while (tokens != NULL)
    {
        if (tokens->lexem == PIPE)
            return 1;
        tokens = tokens->next;
    }
    return 0;
}

// t_ast_node *insert_right(t_ast_node *root, t_token *node){} 
// t_ast_node *insert_left(t_node_type *head, t_node_type *node){}

t_ast_node *new_ast_node(t_node_type type, t_token *tokens)
{
    t_ast_node *node;

    node = alloc_ast_node(type, NULL);
    if (type == COMMAND && tokens != NULL)
    {
        node->data.childs.right = alloc_ast_node(ARGUMENTS, args_table(tokens, ARGUMENTS));
        // insert_right(node, alloc_ast_node(ARGUMENTS, args_table(tokens, ARGUMENTS)));
        if (redirection_found(tokens))
        {
            node->data.childs.left = alloc_ast_node(REDIRECTION, args_table(tokens, REDIRECTION));
            // insert_left(node, alloc_ast_node(REDIRECTION, args_table(tokens, REDIRECTION)));
        }
    }
    return node;
}

t_ast_node *generate_ast_tree(t_token *tokens)
{
    t_ast_node *root;
    t_ast_node *tmp;
    t_token *head;

    head = tokens;
    root = NULL;
    tmp = NULL;
    while (head != NULL)
    {
        if (root == NULL)
            root = new_ast_node(COMMAND, head);
        else 
        {
            if (head->lexem == CMD)
            {
                tmp = new_ast_node(COMMAND, head);
                if (root->data.childs.right == NULL)
                {
                    root->data.childs.right = tmp;
                    // insert_right(root, tmp);
                }
                else 
                {
                    root->data.childs.left = tmp;
                    // insert_left(root, tmp);
                }
                tmp = NULL;
            }
            else if (head->lexem == PIPE)
            {

                tmp = new_ast_node(PIPELINE, NULL);
                {
                    tmp->data.childs.right = root;
                    root = tmp;
                    // root->data.childs.right = tmp;
                    // insert_right(tmp, root);
                }
                root = tmp;
                tmp = NULL;
            }
        }
        head = head->next;
    }
    return root;
}



//tokenizer function
/*===================================================================================*/

t_token	*ft_lstnew_token(char *content, t_lexeme lexem)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (token == NULL)
		return (0);
    token->lexem = lexem;
	token->content = content;
	token->next = NULL;
	return (token);
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

void print_lst(t_token *tokens)
{
    int i;

    i = 1;
    while (tokens != NULL)
    {
        printf("=> %s  : %s\n", tokens->content, lexs[tokens->lexem]);
        i++;
        tokens = tokens->next;
    }
}

/*===================================================================================*/


int main ()
{
    t_token *tokens = NULL;
    t_ast_node *ast_tree = NULL;

    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup("ls"), CMD)); 
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup("-al"), ARG));
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup("|"), PIPE));
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup("grep"), CMD));
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup(".."), ARG));
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup("|"), PIPE));
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup("wc"), CMD));
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup("-l"), ARG));
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup(">"), O_REDIRECTION));
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup("file"), F));

    // print_lst(tokens);

    ast_tree = generate_ast_tree(tokens);
    
    printf("%s\n", types[ast_tree->data.childs.left->type]);
}