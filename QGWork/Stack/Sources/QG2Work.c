#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //调用isdigit函数，本来要用ASCLL表来判断数读取的，给AI看了，知道了这个函数的调用更方便，准确
#include <string.h>

typedef int SElemType;
typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef struct StackNode 
{
    SElemType data;
    struct StackNode *next;
} StackNode;

typedef struct LinkStack 
{
    StackNode *top;
    int count;
} LinkStack;

void StackInit(LinkStack *S) 
{
    S->top = NULL;
    S->count = 0;
}

Status Push(LinkStack *S, SElemType e) 
{
    StackNode *s = (StackNode*)malloc(sizeof(StackNode));
    if (!s) return ERROR;
    s->data = e;
    s->next = S->top;
    S->top = s;
    S->count++;
    return OK;
}

Status StackEmpty(LinkStack *S) 
{
    if(S->top==NULL)
    {
        return TRUE;
    }else{
        return ERROR;
    }
}

Status Pop(LinkStack *S, SElemType *e) 
{
    if (StackEmpty(S)) 
    {
        return ERROR;
    }
    StackNode *p = S->top;
    *e = p->data;
    S->top = p->next;
    free(p);
    S->count--;
    return OK;
}

Status ReadTop(LinkStack *S, SElemType *e) 
{
    if (StackEmpty(S)) 
    {
        return ERROR;
    }
    *e = S->top->data;
    return OK;
}

int Priority(int c)//定义优先级表，方便后面比较
 {
    switch (c) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/': 
            return 2;

        case '(': 
            return 0;
        default: 
            return -1;
    }
}

void Transfer(SElemType e) //想用这个打印字符的，但最后没用上，优化掉了
{
    putchar(e);
}

int main() 
{
    LinkStack S;
    StackInit(&S);
    char c;
    SElemType e;
    char String[1000] = {0}; // 存储后缀表达式

    printf("请输入中缀表达式（以#结束）:\n");
    c = getchar();

    // 中缀转后缀处理
    while (c != '#') {
        if (isdigit(c))
         { 
            // 处理多位数
            int num = 0;
            do {
                num = num * 10 + (c - '0');
                c = getchar();
            } while (isdigit(c));
            sprintf(String + strlen(String), "%d ", num); // 数字后加空格
        } else if (c == '(') {
            Push(&S, '(');
            c = getchar();
        } else if (c == ')') {
            // 进入循环，配对左括号
            while (ReadTop(&S, &e) && e != '(') {
                Pop(&S, &e);
                sprintf(String + strlen(String), "%c ", e);
            }
            Pop(&S, &e); // 弹出左括号
            c = getchar();
        } else {
            // 处理运算符优先级
            while (!StackEmpty(&S) && Priority(c) <= Priority(S.top->data)) {
                Pop(&S, &e);
                sprintf(String + strlen(String), "%c ", e);
            }
            Push(&S, c);
            c = getchar();
        }
    }

    // 弹出剩余运算符
    while (!StackEmpty(&S)) {
        Pop(&S, &e);
        sprintf(String + strlen(String), "%c ", e);
    }

    printf("生成的后缀表达式: %s\n", String);

    // 后缀表达式计算部分 
    LinkStack SaveStack;
    StackInit(&SaveStack);
    char *token = strtok(String, " "); // 分割表达式，这个函数是查来的分割，自己写的分割不行

    while (token != NULL) {
        if (isdigit(token[0])) { 
            // 数字直接入栈
            int num = atoi(token);//这个atoi函数也是查的，字符转整型，本来想用num='token'-48来转换，不好
            Push(&SaveStack, num);
        } else { 
            // 运算符处理
            int a, b, result;
            if (Pop(&SaveStack, &b) == ERROR || Pop(&SaveStack, &a) == ERROR) {
                printf("错误：表达式不合法，操作数不足！\n");
                return -1;
            }
            switch (token[0]) {
                case '+': 
                    result = a + b; break;
                case '-': 
                    result = a - b; break;
                case '*': 
                    result = a * b; break;
                case '/':
                    if (b == 0) {
                        printf("除数不能为零！\n");
                        return -1;
                    }
                    result = a / b; // 整数除法
                    break;
                default:
                    printf("运算符不正确 '%c'\n", token[0]);
                    return -1;
            }
            Push(&SaveStack, result); // 结果入栈
        }
        token = strtok(NULL, " "); // 继续分割
    }

    // 验证最终结果
    int finalResult;
    if (Pop(&SaveStack, &finalResult) && StackEmpty(&SaveStack)) {
        printf("最终结果: %d\n", finalResult);
    } else {
        printf("表达式不完整\n");
    }

    return 0;
}