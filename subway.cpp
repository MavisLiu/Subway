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
	int Line;       //地铁线路号
	int No;         //在该地铁线路中的站点号
	char name[20];       //站名
	ArcNode *firstarc;        
}VertexNode;

typedef struct{
	VertexNode vertex[MAXVEX];
	int vexnum;        //总站点数
	int arcnum;         
	int TotalLine;          //总线路数
	int Linenum[MAX];         //存储每条线路站点数
}AdjList;



typedef struct{
	char name[20];              //中转站站名
	int Location;           //该中转站在vertex[]中的位置
}MidStation;          //存储为中转站的站点信息

int MidStnum = 0;

//关于队列的函数
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
	
	q = malloc(sizeof(LQueue));   //申请头尾指针结点
	p = malloc(sizeof(QNode));    //申请链队头结点

	p -> next = NULL;
	q -> front  = q -> rear  = p;

	return q;
}

void EnterQueue(LQueue *q,datatype x)
{
	QNode *p;
	
	p = malloc(sizeof(QNode));   //申请新结点
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
		printf("空！");
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

//判断该站点是否为已经存储过的中转站	
int Compare(char name[],MidStation MidSt[])
{
	int i,k = 0;

	for(i = 1;i <= MidStnum;i ++)
		if(strcmp(MidSt[i].name,name) == 0)
			k = MidSt[i].Location;
	return k;
}

//查该站名在vertex[]中的位置
int Locate(AdjList *G,char name[],int min,int max)
{
	int i,k = 0;
	
	for(i = min;i <= max;i ++)
		if(strcmp(G->vertex[i].name,name) == 0)
			k = i;
	return k;
}

//用邻接表存储站点信息	
void Creat(AdjList *G,MidStation MidSt[])
{
	int i,j,t,k,pre,temp;
	char name[20],ch;
	ArcNode *p,*q;

	printf("\n\t输入总的线路数：");
	scanf("%d",&G->TotalLine);
	getchar();
//初始化
	G->vexnum = 0;
	printf("\n\t输入个线路的站点数：\n");
	for(i = 1;i <= G->TotalLine;i ++)
	{
		printf("Line %d:",i);
		scanf("%d",&G->Linenum[i]);
		getchar();
		G->vexnum += G->Linenum[i];
	}

	t = 0;          //用t来记录vertex的下标，以免中转站重复存储
	printf("\n\t按线路顺序依次输入各线路起始点至终点的站名：\n");
	for(i = 1;i <= G->TotalLine;i ++)
	{
		printf("\n\tLine %d:\n",i);
		printf("(1).");
		t++;
		gets(G->vertex[t].name);            //输入第i条线路的第一个站名
		G->vertex[t].No = 1;
		G->vertex[t].Line = i;
		G->vertex[i].firstarc = NULL;
		pre = t;         //记录前一个站点的Location，为了接下来邻接表创建
	
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
	
		printf("\n\t请输入Line %d中的中转站站名：\n",i);
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
			printf("是否要继续输入中转站？(Y/N)");
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

	//初始化
	temp = 0;
	pre = INFINITY;
	for(i = 0;i <= G->vexnum;i ++)
		visited[i] = 0;
	
	visited[start] = INFINITY;    //用visited[]数组来存储站点的前驱。
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
		printf("\n对不起。没找到相应路线点。\n");
	else
	{
		t = end;
		k = 1;
		Way[k] = t;      //Way[]可存储站点线路的逆序排列  从end到start
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
	printf("请输入信息：\n\n");
	printf("\t起始点：");
	gets(name);
	start = Locate(&G,name,1,t);
	printf("\t目的点：");
	gets(name);
	end = Locate(&G,name,1,t);
	
	Findway(&G,start,end,Way);
	
}
