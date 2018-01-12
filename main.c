#include<stdio.h>
#define MAXVertex 501
#define INFINITY 1000000
#pragma warning(disable:4996)
//Definition for ，表示到达节点i的第j种走法信息，便于后续回溯路径。每条记录包含以下信息：节点i的路由节点Nv、到达路由节点的走法序号Ne、到当前节点i为止需要从中心带走的车辆数Send和到当前节点i为止收集得到的车辆数Back 
struct GNode {
	int Nv;
	int Ne;
	int Send;
	int Back;
}GNode[MAXVertex][MAXVertex];
int Full, N, M, sp, Half,cur[MAXVertex];//定义所需输入变量：站点容量Full、站点容量一半Half、站点数N、边数M、目的站点sp、站点当前车辆数cur[] 
int G[MAXVertex][MAXVertex];//定义二维数组，用于存放任意两个站点之间互连需要的时间 
int dist[MAXVertex], known[MAXVertex], p[MAXVertex];//dijkstra算法需要用到的

void ShortestDist(int x);//实现一个dijkstra算法，同时记录下所有最短路径
void CheckGood();//从之前找的所有最短路径中选择最少的，并找到这个路径的每个点
int main() {
	int v, w, i, j, tem, path[512];
	scanf("%d %d %d %d", &Full, &N, &sp, &M);
	Half = Full / 2;
	for (i = 1; i <= N; i++) {  //记录每个点，并为了dijkstra初始化参数
		scanf("%d", &cur[i]);
		if (cur[i] > Full) {//对输入进行判断，尽量保证输入的合理性
			printf("ERROR INPUT\n");
			system("pause");
		}
		dist[i] = -1;
		known[i] = 0;
		p[i] = 0;
		for (j = 1; j <= N; j++) {
			G[i][j] = 0;
			GNode[i][j].Ne = GNode[i][j].Nv = 0;
			GNode[i][j].Back = GNode[i][j].Send = 0;
		}//初始化走法信息记录 
	}
	for (i = 0; i < M; i++) {
		scanf("%d %d %d", &v, &w, &tem);
		if (v > N || w > N || w==v||(G[v][w] != 0 && G[v][w] != tem)) {//对输入进行判断，尽量保证输入的合理性
			printf("ERROR INPUT\n");
			system("pause");
		}
		G[v][w] = G[w][v] = tem;    //双向图，v-w，w-v都是一样的
	}
	ShortestDist(sp);
	if (p[sp] == 0) {
		printf("Can't find one way to the target\n");
		system("pause");
	}
	CheckGood();//遴选出所有最短路径中Send或Back最少的路径 
	system("pause");
	return 0;
}

void ShortestDist(int x) {
	int v, w, i, j, k, len, temp;
	dist[0] = 0;//初始化一个0点，作为结束标记
	GNode[0][0].Nv=GNode[0][0].Ne= -1;
	GNode[0][0].Send = GNode[0][0].Back = 0;
	p[0] = 1;
	while (!known[x]) {
		len = INFINITY;
		v = -1;
		for (i = 0; i <= N; i++) {
			if (known[i]) continue;//已经经过该点，就跳过
			if (dist[i] >= 0 && dist[i] < len) {//寻找最短距离
				len = dist[i];
				v = i;
			}
		}//寻找当前最近节点 
		if (v == -1) break;
		known[v] = 1;//标记为已扩展 
		for (w = 1; w <=N; w++) {
			if (!G[v][w] || known[w]) continue; //经过或者无法到达，跳过
			if (dist[w] > 0 && dist[w] < dist[v] + G[v][w]) continue;//非更短的路径，跳过 
			else if (dist[w] == -1 || dist[w] >= dist[v] + G[v][w]) {
				if (dist[w] == -1 || dist[w] > dist[v] + G[v][w]) {//一条更短的路径
					p[w] = 0;  //之前找到的路径全部作废
				}
				dist[w] = dist[v] + G[v][w];//更新路径长度 
				temp = cur[w] > Half ? cur[w] - Half : Half - cur[w];
				for (i = 0, j = p[w]; i < p[v]; i++, j++) {
					GNode[w][j].Nv = v;
					GNode[w][j].Ne = i;
					if (cur[w] >= Half) { //经过的这个点w 车辆比一半要多 
						GNode[w][j].Back = GNode[v][i].Back + temp;  //需要送回的车辆变多
						GNode[w][j].Send = GNode[v][i].Send;  //送出的车辆不变
					}
					else {
						if (temp <= GNode[v][i].Back) {//比一半要少，且不会用完之前留下的车
							GNode[w][j].Back = GNode[v][i].Back - temp;
							GNode[w][j].Send = GNode[v][i].Send;
						}
						else {
							GNode[w][j].Send = GNode[v][i].Send + temp - GNode[v][i].Back;//增加送出的车辆
							GNode[w][j].Back = 0;
						}
					}
				}//更新通过扩展节点到达当前节点所有走法的记录信息 
				p[w] = j;//更新到达当前节点的最短路径条数 
			}
		}
	}
}

void CheckGood() {
	int i, j, k;
	int send, back, temp1,temp2,path[MAXVertex];
	send = back = INFINITY;
	for (i = 0; i < p[sp]; i++) {
		if (send > GNode[sp][i].Send || (send == GNode[sp][i].Send&&back > GNode[sp][i].Back)) {
			send = GNode[sp][i].Send;
			back = GNode[sp][i].Back;
			temp1 = GNode[sp][i].Nv;
			temp2 = GNode[sp][i].Ne;
		}
	}//比较不同最短路径，优先寻找send最少的路径，若不止一条，寻找back最少的路径 
	i = temp1;
	j = temp2;
	k = 0;
	while (i >= 0) {  //一个个通过结点找上去，回溯最佳路径 
		path[k++] = i;
		temp1 = GNode[i][j].Nv;
		temp2 = GNode[i][j].Ne;	
		i = temp1;
		j = temp2;
	}
	printf("%d ",send);
	for (--k; k >= 0; k--)
		printf("%d->", path[k]);
	printf("%d %d", sp, back);
	return;
}
