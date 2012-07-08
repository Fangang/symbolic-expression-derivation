#include <stdio.h> 
#include <stdlib.h> 

//包含表达式的节点 
typedef struct expr
{
    char op;
    char var1;
    char var2; 
    struct expr *expr1;
    struct expr *expr2; 
} Expr;

//表达式节点初始化 
int initNode(Expr *expr,char op,char var1,char var2,Expr *expr1,Expr *expr2)
{
    expr->op = op;
    expr->var1 = var1;
    expr->var2 = var2;
    expr->expr1 = expr1;
    expr->expr2 = expr2; 
} 
 
int issum(Expr const *expr) 
{
    if((expr->op)=='+') return 1;
    else return 0; 
} 
int ischar(char var) 
{
    if(var>='a' && var <='z') return 1;
    else 
        return 0; 
} 
int equal(char expvar,char var) 
{
    if(expvar==var) return 1; 
    else return 0; 
}

int isproduct(Expr const *expr) 
{
    if((expr->op)=='*') return 1;
    else return 0; 
} 

//表达式求导 
int deriv(Expr *expr,char var)
{
    if (issum(expr))
    {
        if(expr->expr1 != NULL)  //递归调用部分 
            deriv(expr->expr1,var);
        if(expr->expr2 != NULL)
            deriv(expr->expr2,var);    
            
        if(ischar(expr->var1) && equal(expr->var1,var)) expr->var1 = '1';
        else expr->var1 = '0';
        
        if(ischar(expr->var2) && equal(expr->var2,var)) expr->var2 = '1';
        else expr->var2 = '0';
        
        return 0; 
    }
    else if(isproduct(expr))
    {  
        expr->op = '+'; //first make_sum
        
        if(expr->expr1 != NULL)//两层嵌套判断 ，注意动态扩展 
        { 
            if(expr->expr1->expr1 != NULL)
                deriv(expr->expr1->expr1,var);
            if(expr->expr1->expr2 != NULL)
                deriv(expr->expr1->expr2,var);
        }
        if(expr->expr2 != NULL)
        { 
            if(expr->expr2->expr1 != NULL)
                deriv(expr->expr2->expr1,var);
            if(expr->expr2->expr2 != NULL)
                deriv(expr->expr2->expr2,var);
        }   
            
        if(expr->expr1 != NULL && expr->expr2 == NULL)
        {
            //扩展expr->expr2 
            expr->expr2 = (Expr *)malloc(sizeof(Expr));
            expr->expr2->op = '*';
            expr->expr2->var1 = '1';
            expr->expr2->var2 = expr->var2;
            expr->expr2->expr1 = NULL;
            expr->expr2->expr2 = NULL; 
            expr->var2 = '\0'; 
            
            //备份expr->expr1 
            Expr *temp = (Expr *)malloc(sizeof(Expr));
            initNode(temp,expr->expr1->op,expr->expr1->var1,expr->expr1->var2,
                     expr->expr1->expr1,expr->expr1->expr2);
            //备份expr->expr2 
            Expr *tempp = (Expr *)malloc(sizeof(Expr));
            initNode(tempp,expr->expr2->op,expr->expr2->var1,expr->expr2->var2,
                     expr->expr2->expr1,expr->expr2->expr2);
            
            //对表达式1求导数 
            deriv(expr->expr1,var);
            //备份求导后的表达式1,用于扩展表达式1 
            Expr *temppp = (Expr *)malloc(sizeof(Expr));
            initNode(temppp,expr->expr1->op,expr->expr1->var1,expr->expr1->var2,
                     expr->expr1->expr1,expr->expr1->expr2);
            expr->expr1->op = '*'; 
            expr->expr1->var1 = '\0';
            expr->expr1->var2 = '\0'; 
            expr->expr1->expr1 = temppp;  //expr->expr1导数 NULL
            expr->expr1->expr2 = tempp;  //expr->expr2 NULL
            
            
            deriv(expr->expr2,var);
            //备份求导后的表达式2，用于动态扩展表达式 
            Expr *tempppp = (Expr *)malloc(sizeof(Expr));
            initNode(tempppp,expr->expr2->op,expr->expr2->var1,expr->expr2->var2,
                     expr->expr2->expr1,expr->expr2->expr2);
            expr->expr2->op = '*';
            expr->expr2->var1 = '\0';
            expr->expr2->var2 = '\0';    
            expr->expr2->expr1 = tempppp; //expr->expr2导数 NULL 
            expr->expr2->expr2 = temp; //NULL.It's all right. expr->expr1 
            
            return 0; 
        } 
        else if(expr->expr1 ==NULL && expr->expr2 != NULL)
        {
            //扩展expr->expr1 
            expr->expr1 = (Expr *)malloc(sizeof(Expr));
            expr->expr1->op = '*';
            expr->expr1->var1 = '1';
            expr->expr1->var2 = expr->var1;
            expr->expr1->expr1 = NULL;
            expr->expr1->expr2 = NULL;           
            expr->var1 = '\0';
            
            // 备份expr->expr1 
            Expr *temp = (Expr *)malloc(sizeof(Expr));
            initNode(temp,expr->expr1->op,expr->expr1->var1,expr->expr1->var2,
                     expr->expr1->expr1,expr->expr1->expr2);
            //备份expr->expr2 
            Expr *tempp = (Expr *)malloc(sizeof(Expr));
            initNode(tempp,expr->expr2->op,expr->expr2->var1,expr->expr2->var2,
                     expr->expr2->expr1,expr->expr2->expr2);

            //对表达式1求导数 
            deriv(expr->expr1,var);
            //备份求导后的表达式1,用于扩展表达式1 
            Expr *temppp = (Expr *)malloc(sizeof(Expr));
            initNode(temppp,expr->expr1->op,expr->expr1->var1,expr->expr1->var2,
                     expr->expr1->expr1,expr->expr1->expr2);
            expr->expr1->op = '*'; 
            expr->expr1->var1 = '\0';
            expr->expr1->var2 = '\0'; 
            expr->expr1->expr1 = temppp;
            expr->expr1->expr2 = tempp; 
            
            //对表达式2求导 
            deriv(expr->expr2,var);
            Expr *tempppp = (Expr *)malloc(sizeof(Expr));
            initNode(tempppp,expr->expr2->op,expr->expr2->var1,expr->expr2->var2,
                     expr->expr2->expr1,expr->expr2->expr2);
            expr->expr2->op = '*';
            expr->expr2->var1 = '\0';
            expr->expr2->var2 = '\0';    
            expr->expr2->expr1 = tempppp; 
            expr->expr2->expr2 = temp; //NULL.It's all right. 

            return 0; 
        }
        else if (expr->expr1 == NULL && expr->expr2 == NULL)
        {
            char temp = expr->var1; 
            if(ischar(expr->var1) && equal(expr->var1,var)) expr->var1 = expr->var2;
            else expr->var1 = '0';
        
            if(ischar(expr->var2) && equal(expr->var2,var)) expr->var2 = temp;
            else expr->var2 = '0';
        
            return 0; 
        }
        else  //两个均是表达式的情形 
        {
            //备份expr->expr1 
            Expr *temp = (Expr *)malloc(sizeof(Expr));
            initNode(temp,expr->expr1->op,expr->expr1->var1,expr->expr1->var2,
                     expr->expr1->expr1,expr->expr1->expr2);
                     
            //备份expr->expr2 
            Expr *tempp = (Expr *)malloc(sizeof(Expr));
            initNode(tempp,expr->expr2->op,expr->expr2->var1,expr->expr2->var2,
                     expr->expr2->expr1,expr->expr2->expr2);
        
            //对表达式1求导数 
            deriv(expr->expr1,var);
            //备份求导后的表达式1,用于扩展表达式1 
            Expr *temppp = (Expr *)malloc(sizeof(Expr));
            initNode(temppp,expr->expr1->op,expr->expr1->var1,expr->expr1->var2,
                     expr->expr1->expr1,expr->expr1->expr2);
            expr->expr1->op = '*'; 
            expr->expr1->var1 = '\0';
            expr->expr1->var2 = '\0'; 
            expr->expr1->expr1 = temppp;
            expr->expr1->expr2 = tempp; 
            
            //对表达式2求导 
            deriv(expr->expr2,var);
            Expr *tempppp = (Expr *)malloc(sizeof(Expr));
            initNode(tempppp,expr->expr2->op,expr->expr2->var1,expr->expr2->var2,
                     expr->expr2->expr1,expr->expr2->expr2);
            expr->expr2->op = '*';
            expr->expr2->var1 = '\0';
            expr->expr2->var2 = '\0';    
            expr->expr2->expr1 = tempppp;
            free(expr->expr2->expr2); 
            expr->expr2->expr2 = temp; //NULL.It's all right. 
            
            return 0; 
            
        } 
    } 
    else
    {
        printf("Error!");
        exit(1); 
    } 
}
int printExpr(Expr *expr) 
{
    printf("%c %c %c\n",expr->var1,expr->op,expr->var2) ;
    return 0; 
}
//打印整个表达式 
int printExprTree(Expr *expr)
{
    if(expr->expr1 != NULL)
        printExprTree(expr->expr1);
        
    printExpr(expr); 
    
    if(expr->expr2 != NULL)
        printExprTree(expr->expr2);
}
int Free(Expr *expr)
{
    if(expr->expr1 != NULL)
        Free(expr->expr1); 
    
    if(expr->expr2 != NULL)
        Free(expr->expr2);
        
    free(expr);
} 
int main(int argc,char *argv[]) 
{
    Expr *expr1=(Expr *)malloc(sizeof(Expr));
    Expr *expr2=(Expr *)malloc(sizeof(Expr));
    Expr *expr3=(Expr *)malloc(sizeof(Expr));
    Expr *expr4=(Expr *)malloc(sizeof(Expr));
    Expr *expr5=(Expr *)malloc(sizeof(Expr));
     
    initNode(expr1,'+','\0','\0',expr2,expr3); 
    initNode(expr2,'*','x','y',NULL,NULL);
    initNode(expr3,'*','\0','\0',expr5,expr4); 
    initNode(expr4,'+','x','x',NULL,NULL);
    initNode(expr5,'*','1','x',NULL,NULL);
   
    printExprTree(expr1);
    printf("\n==================\n"); 
    deriv(expr1,'x'); 
    printExprTree(expr1);
    
    Free(expr1);//终于知道树形结构的重要性了吧！哈哈 
    
    system("pause");
    return 0; 
} 
