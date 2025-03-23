#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //����isdigit����������Ҫ��ASCLL�����ж�����ȡ�ģ���AI���ˣ�֪������������ĵ��ø����㣬׼ȷ
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

int Priority(int c)//�������ȼ����������Ƚ�
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

void Transfer(SElemType e) //���������ӡ�ַ��ģ������û���ϣ��Ż�����
{
    putchar(e);
}

int main() 
{
    LinkStack S;
    StackInit(&S);
    char c;
    SElemType e;
    char String[1000] = {0}; // �洢��׺���ʽ

    printf("��������׺���ʽ����#������:\n");
    c = getchar();

    // ��׺ת��׺����
    while (c != '#') {
        if (isdigit(c))
         { 
            // �����λ��
            int num = 0;
            do {
                num = num * 10 + (c - '0');
                c = getchar();
            } while (isdigit(c));
            sprintf(String + strlen(String), "%d ", num); // ���ֺ�ӿո�
        } else if (c == '(') {
            Push(&S, '(');
            c = getchar();
        } else if (c == ')') {
            // ����ѭ�������������
            while (ReadTop(&S, &e) && e != '(') {
                Pop(&S, &e);
                sprintf(String + strlen(String), "%c ", e);
            }
            Pop(&S, &e); // ����������
            c = getchar();
        } else {
            // ������������ȼ�
            while (!StackEmpty(&S) && Priority(c) <= Priority(S.top->data)) {
                Pop(&S, &e);
                sprintf(String + strlen(String), "%c ", e);
            }
            Push(&S, c);
            c = getchar();
        }
    }

    // ����ʣ�������
    while (!StackEmpty(&S)) {
        Pop(&S, &e);
        sprintf(String + strlen(String), "%c ", e);
    }

    printf("���ɵĺ�׺���ʽ: %s\n", String);

    // ��׺���ʽ���㲿�� 
    LinkStack SaveStack;
    StackInit(&SaveStack);
    char *token = strtok(String, " "); // �ָ���ʽ����������ǲ����ķָ�Լ�д�ķָ��

    while (token != NULL) {
        if (isdigit(token[0])) { 
            // ����ֱ����ջ
            int num = atoi(token);//���atoi����Ҳ�ǲ�ģ��ַ�ת���ͣ���������num='token'-48��ת��������
            Push(&SaveStack, num);
        } else { 
            // ���������
            int a, b, result;
            if (Pop(&SaveStack, &b) == ERROR || Pop(&SaveStack, &a) == ERROR) {
                printf("���󣺱��ʽ���Ϸ������������㣡\n");
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
                        printf("��������Ϊ�㣡\n");
                        return -1;
                    }
                    result = a / b; // ��������
                    break;
                default:
                    printf("���������ȷ '%c'\n", token[0]);
                    return -1;
            }
            Push(&SaveStack, result); // �����ջ
        }
        token = strtok(NULL, " "); // �����ָ�
    }

    // ��֤���ս��
    int finalResult;
    if (Pop(&SaveStack, &finalResult) && StackEmpty(&SaveStack)) {
        printf("���ս��: %d\n", finalResult);
    } else {
        printf("���ʽ������\n");
    }

    return 0;
}