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

int is_schar (t_lexeme lexem)
{
    if (lexem == O_REDIRECTION || lexem == I_REDIRECTION 
        || lexem == HERDOC || lexem == APPEND)
        return 1;
    else if (lexem == PIPE)
        return 2;
    return 0;
}

t_argument *args_table(t_token *tokens, t_node_type type)
{
    t_argument *args;
    int flag;

    flag = 0;
    args = NULL;
    if (tokens != NULL && is_schar(tokens->lexem) == 1)
        flag = 1;
    while (tokens != NULL && tokens->lexem != PIPE)
    {
        if (type == ARGUMENTS)
        {
            if (is_schar(tokens->lexem) != 0 && flag != 1)
                return args;
            ft_argsadd_back(&args, ft_argsnew(ft_strdup(tokens->content)));
        }
        flag = 0;
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

// ls > ls > ls

t_ast_node *get_redirection_tree(t_node_type type, t_token *tokens)
{
    t_ast_node *root;

    root = NULL;
    if (tokens == NULL || tokens->lexem == PIPE)
        return root;
    root = alloc_ast_node(REDIRECTION, NULL);
    root->data.childs.right = alloc_ast_node(ARGUMENTS, args_table(tokens, ARGUMENTS));
    if (tokens != NULL)
        tokens = tokens->next;
    while(tokens != NULL && is_schar(tokens->lexem) == 0)
        tokens = tokens->next;
    root->data.childs.left = get_redirection_tree(type, tokens);
    return root;
}


t_ast_node *new_ast_node(t_node_type type, t_token *tokens)
{
    t_ast_node *node;
    t_token *tmp;
    // int red_flag;

    tmp = NULL;
    node = alloc_ast_node(type, NULL);
    if (type == COMMAND && tokens != NULL)
    {
        node->data.childs.right = alloc_ast_node(ARGUMENTS, args_table(tokens, ARGUMENTS));
        if (redirection_found(tokens))
        {

            // node->data.childs.left = alloc_ast_node(REDIRECTION, args_table(tokens, REDIRECTION));
            while(tokens != NULL && is_schar(tokens->lexem) == 0)
                tokens = tokens->next;
            node->data.childs.left = get_redirection_tree(REDIRECTION, tokens);
            
            // while (tokens != NULL && tokens->lexem != PIPE)
            // {
            //     tmp = tokens;
            //     red_flag = 0;
            //     while (tmp != NULL && red_flag == 0 && is_schar(tmp->lexem) == 0)
            //     {
                    
            //         tmp = tmp->next;
            //     }
            //     tokens = tokens->next;
            // }
        }
    }
    // else if (type = REDIRECTION && tokens != NULL)
    // {
    //     node->data.childs.right = alloc_ast_node(ARGUMENTS, args_table(tokens, ARGUMENTS));        
    // }
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
                }
                else 
                {
                    root->data.childs.left = tmp;
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


void print_args(t_argument *args)
{
    while (args != NULL)
    {
        printf("[ %s ] -> ", args->content);
        args = args->next;
    }
    printf("[ null ]");
}

/*===================================================================================*/

void print_space(int space)
{
    int i;

    i = -1;
    while (++i < space)
        printf("  ");
}

void print_ast_tree(t_ast_node *ast_tree, int space)
{
    if (ast_tree == NULL)
        return ;
    print_space(space);
    printf("--> %s ", types[ast_tree->type]);
    if (ast_tree->type == ARGUMENTS)
        print_args(ast_tree->data.arg_list);
    printf("\n");
    if (ast_tree->type == COMMAND || ast_tree->type == PIPELINE || ast_tree->type == REDIRECTION)
    {
        if (ast_tree->data.childs.right != NULL)
        {
            print_ast_tree(ast_tree->data.childs.right, space + 1);
        }
        else
        {
            print_space(space + 1);
            printf("\n");
        }

        if (ast_tree->data.childs.left != NULL)
        {
            print_ast_tree(ast_tree->data.childs.left, space + 1);
        }
        else
        {
            print_space(space + 1);
            printf("\n");
        }
    }
}


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
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup(">"), O_REDIRECTION));
    ft_lstadd_token_back(&tokens, ft_lstnew_token(ft_strdup("file2"), F));

    ast_tree = generate_ast_tree(tokens);    
    print_ast_tree(ast_tree, 0);
}