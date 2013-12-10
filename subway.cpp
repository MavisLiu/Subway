#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXVEX 100
#define MAX 20
#define MAXSIZE 100
#define datatype int
#define INFINITY 32767
#define TRUE 1
#define FALSE 0
#define OVERFLOW 0
#define ERROR 0


typedef struct ArcNode{
	int adjvex;        
	int weight;
	struct ArcNode *nextarc;
}ArcNode;

typedef struct{
	int Line;       //������·��
	int No;         //�ڸõ�����·�е�վ���
	char name[20];       //վ��
	ArcNode *firstarc;        
}VertexNode;

typedef struct{
	VertexNode vertex[MAXVEX];
	int vexnum;        //��վ����
	int arcnum;         
	int TotalLine;          //����·��
	int Linenum[MAX];         //�洢ÿ����·վ����
}AdjList;



typedef struct{
	char name[20];              //��תվվ��
	int Location;           //����תվ��vertex[]�е�λ��
}MidStation;          //�洢Ϊ��תվ��վ����Ϣ

int MidStnum = 0;

//���ڶ��еĺ���
typedef struct node
{
	datatype data;
	struct node *next;
}QNode;
typedef struct 
{
	QNode *front;
	QNode *rear;
}LQueue;

LQueue *InitQueue()
{
	LQueue *q;
	QNode *p;
	
	q = malloc(sizeof(LQueue));   //����ͷβָ����
	p = malloc(sizeof(QNode));    //��������ͷ���

	p -> next = NULL;
	q -> front  = q -> rear  = p;

	return q;
}

void EnterQueue(LQueue *q,datatype x)
{
	QNode *p;
	
	p = malloc(sizeof(QNode));   //�����½��
	p -> data = x;
	p -> next = NULL;
	q -> rear -> next = p;
	q -> rear = p;

}

int Empty(LQueue *q)
{
	if(q -> front == q -> rear)
		return TRUE;
	else
		return 0;
}

int DeleteQueue(LQueue *q,datatype *x)
{
	QNode *p;
	
	if(Empty(q))
	{
		printf("�գ�");
		return FALSE;
	}
	else
	{
		p = q -> front -> next;
		q -> front -> next = p -> next;
		*x = p -> data;
		free(p);
		if(q -> front -> next == NULL)
			q -> rear = q -> front;
		return TRUE;
	}
}

//�жϸ�վ���Ƿ�Ϊ�Ѿ��洢������תվ	
int Compare(char name[],MidStation MidSt[])
{
	int i,k = 0;

	for(i = 1;i <= MidStnum;i ++)
		if(strcmp(MidSt[i].name,name) == 0)
			k = MidSt[i].Location;
	return k;
}

//���վ����vertex[]�е�λ��
int Locate(AdjList *G,char name[],int min,int max)
{
	int i,k = 0;
	
	for(i = min;i <= max;i ++)
		if(strcmp(G->vertex[i].name,name) == 0)
			k = i;
	return k;
}

//���ڽӱ�洢վ����Ϣ	
void Creat(AdjList *G,MidStation MidSt[])
{
	int i,j,t,k,pre,temp;
	char name[20],ch;
	ArcNode *p,*q;

	printf("\n\t�����ܵ���·����");
	scanf("%d",&G->TotalLine);
	getchar();
//��ʼ��
	G->vexnum = 0;
	printf("\n\t�������·��վ������\n");
	for(i = 1;i <= G->TotalLine;i ++)
	{
		printf("Line %d:",i);
		scanf("%d",&G->Linenum[i]);
		getchar();
		G->vexnum += G->Linenum[i];
	}

	t = 0;          //��t����¼vertex���±꣬������תվ�ظ��洢
	printf("\n\t����·˳�������������·��ʼ�����յ��վ����\n");
	for(i = 1;i <= G->TotalLine;i ++)
	{
		printf("\n\tLine %d:\n",i);
		printf("(1).");
		t++;
		gets(G->vertex[t].name);            //�����i����·�ĵ�һ��վ��
		G->vertex[t].No = 1;
		G->vertex[t].Line = i;
		G->vertex[i].firstarc = NULL;
		pre = t;         //��¼ǰһ��վ���Location��Ϊ�˽������ڽӱ���
	
		for(j = 2;j <= G->Linenum[i];j ++)
		{
			printf("(%d).",j);
			gets(name);
			temp = Compare(name,MidSt);
	
			if(temp == 0)
			{
				t++;
				G->vertex[t].No = j;
				G->vertex[t].Line = i;
				strcpy(G->vertex[t].name,name);
				G->vertex[i].firstarc = NULL;

				q = (ArcNode *)malloc(sizeof(ArcNode));
				q->adjvex = pre;
				q->nextarc = G->vertex[t].firstarc;
				G->vertex[t].firstarc = q;
	
				p = (ArcNode *)malloc(sizeof(ArcNode));
				p->adjvex = t;
				p->nextarc = G->vertex[pre].firstarc;
				G->vertex[pre].firstarc = p;
			
				pre = t;
			}
			else
			{
				p = (ArcNode *)malloc(sizeof(ArcNode));
				p->adjvex = pre;
				p->nextarc = G->vertex[temp].firstarc;
				G->vertex[temp].firstarc = p;
				
				q = (ArcNode *)malloc(sizeof(ArcNode));
				q->adjvex = temp;
				q->nextarc = G->vertex[pre].firstarc;
				G->vertex[pre].firstarc = q;

				pre = temp;
			}
		}
	
		printf("\n\t������Line %d�е���תվվ����\n",i);
		do
		{
			gets(name);
			k = Compare(name,MidSt);
			if(k == 0)
			{
				MidStnum ++;
				strcpy(MidSt[MidStnum].name,name);
				MidSt[MidStnum].Location = Locate(G,name,t-G->Linenum[i],t);
			}
			printf("�Ƿ�Ҫ����������תվ��(Y/N)");
			ch = getchar();
			getchar();
		}while(ch == 'Y' || ch == 'y');
	}
}


int FirstAdj(AdjList *G,int v)
{
	if(G->vertex[v].firstarc == NULL)
		return (-1);
	else
		return (G->vertex[v].firstarc->adjvex);
}

int NextAdj(AdjList *G,int w,int v)
{
	ArcNode *p;
	p = G->vertex[v].firstarc;
	
	while(p->adjvex != w)
		p = p->nextarc;
	
	if(p->nextarc == NULL)
		return (-1);
	else
		return (p->nextarc->adjvex);
}	


void Findway(AdjList *G,int start,int end,int Way[])
{
	int visited[MAXVEX];
	int i,temp,pre,k,v,w,t;
	LQueue *Q;

	//��ʼ��
	temp = 0;
	pre = INFINITY;
	for(i = 0;i <= G->vexnum;i ++)
		visited[i] = 0;
	
	visited[start] = INFINITY;    //��visited[]�������洢վ���ǰ����
	Q = InitQueue();
	EnterQueue(Q,start);

	while( !Empty(Q))
	{
		DeleteQueue(Q,&v);
		pre = v;
		w = FirstAdj(&G,v);
		while(w != -1 || w == end)
		{
			if(!visited[w])
			{
				visited[w] = pre;
				EnterQueue(Q,w);
			}
			w = NextAdj(&G,v,w);
		}
		if(w == end)
		{
			temp = 1;
			break;
		}
	}
	if(temp == 0)
		printf("\n�Բ���û�ҵ���Ӧ·�ߵ㡣\n");
	else
	{
		t = end;
		k = 1;
		Way[k] = t;      //Way[]�ɴ洢վ����·����������  ��end��start
		while(visited[t] != start)
		{
			k ++;
			t = visited[t];
			Way[k] = t;
		}
		k ++;
		Way[k] = start;
		for(i = k;i > 0;i --)
			printf("%s  ----->  ",G->vertex[Way[i]].name);
		printf(" ^ ^ \n\n");
	}
}

main()
{
	AdjList G;
	MidStation MidSt[MAX];
	int Way[MAXVEX];
	int start,end,t;
	char name[20];

	Creat(&G,MidSt);
	t = G.vexnum;
	printf("��������Ϣ��\n\n");
	printf("\t��ʼ�㣺");
	gets(name);
	start = Locate(&G,name,1,t);
	printf("\tĿ�ĵ㣺");
	gets(name);
	end = Locate(&G,name,1,t);
	
	Findway(&G,start,end,Way);
	
}
