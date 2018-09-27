/*
!/usr/bin/env c
-*- coding: utf-8 -*-
**************************************
@Time    : 2018/9/27 1:02
@Author  : Jiaxu Zou
@File    : bike_manage.c 
**************************************
*/
#include<stdio.h>
#define MAXVertex 501
#define INFINITY 1000000

/*
Definition for a two-dimensional structure array. GNode[i][j] represents the routing information from Vertex i to Vertex j.
Nv: the preface vertex of Vertex i
Ne: the serial number of the path to Nv
Send: the total number of bikes to take from Center to reach Vertex i
Back: the total number of bikes collected from routing vertexes upon reaching Vertex i(including bikes collected from Vertex i)
*/
struct GNode {
	int Nv;
	int Ne;
	int Send;
	int Back;
}GNode[MAXVertex][MAXVertex];

/*
Definition for input variables:
Full: the capacity of each vertex
Half: the half of Full
N: the total number of vertexes
M: the total number of edges between two vertexes
sp: the destination vertex
cur[]: the current bike number at each vertex
*/

int Full, N, M, sp, Half,cur[MAXVertex];

/*Definition for a two-dimensional array. G[i][j] represents the length of the edge between adjacent vertex pairs i and j.*/

int G[MAXVertex][MAXVertex];

/*Definition for some intermediate variables in Dijkstra's Algorithm*/

int dist[MAXVertex], known[MAXVertex], p[MAXVertex];

/*Figuring out all the shortest paths using Dijkstra's Algorithm*/

void ShortestDist(int x);

/*Figuring out the optimal path among all the shortest paths found*/

void CheckGood();

int main() {
	int v, w, i, j, tem, path[512];
	/*Type in the capacity of vertex, number of vertexes, destination vertex and number of edges*/
	scanf("%d %d %d %d", &Full, &N, &sp, &M);
	Half = Full / 2;
	for (i = 1; i <= N; i++) {
		/*Type in the current bike number of each vertex*/
		scanf("%d", &cur[i]);
		if (cur[i] > Full) {
			printf("ERROR INPUT\n");
			system("pause");
		}
		dist[i] = -1;
		known[i] = 0;
		p[i] = 0;
		/*Initialization for path notes*/
		for (j = 1; j <= N; j++) {
			G[i][j] = 0;
			GNode[i][j].Ne = GNode[i][j].Nv = 0;
			GNode[i][j].Back = GNode[i][j].Send = 0;
		} 
	}
	for (i = 0; i < M; i++) {
		/*Type in the undirected graph information*/
		scanf("%d %d %d", &v, &w, &tem);
		if (v > N || w > N || w==v||(G[v][w] != 0 && G[v][w] != tem)) {
			printf("ERROR INPUT\n");
			system("pause");
		}
		G[v][w] = G[w][v] = tem;    
	}
	/*Find all the shortest paths*/
	ShortestDist(sp);
	
	if (p[sp] == 0) {
		printf("Can't find one way to the target\n");
		system("pause");
	}
	/*Choose the best shortest path with the least bikes to send or take back*/
	CheckGood();
	system("pause");
	return 0;
}

void ShortestDist(int x) {
	int v, w, i, j, k, len, temp;
	dist[0] = 0;
	GNode[0][0].Nv=GNode[0][0].Ne= -1;
	GNode[0][0].Send = GNode[0][0].Back = 0;
	p[0] = 1;
	while (!known[x]) {
		len = INFINITY;
		v = -1;
		for (i = 0; i <= N; i++) {
			/*Have reached the vertex already, so skip*/
			if (known[i]) continue;
			if (dist[i] >= 0 && dist[i] < len) {
				len = dist[i];
				v = i;
			}
		} 
		if (v == -1) break;
		/*Mark as reached*/
		known[v] = 1;
		for (w = 1; w <=N; w++) {
			/*Have reached or can't reach, so skip*/
			if (!G[v][w] || known[w]) continue; 
			/*Not a shorter path, so skip*/
			if (dist[w] > 0 && dist[w] < dist[v] + G[v][w]) continue;
			else if (dist[w] == -1 || dist[w] >= dist[v] + G[v][w]) {
				/*Have found a shorter path, so all the shortest paths found before are invalid*/
				if (dist[w] == -1 || dist[w] > dist[v] + G[v][w]) {
					p[w] = 0;  
				}
				/*Update the path length*/
				dist[w] = dist[v] + G[v][w];
				temp = cur[w] > Half ? cur[w] - Half : Half - cur[w];
				for (i = 0, j = p[w]; i < p[v]; i++, j++) {
					GNode[w][j].Nv = v;
					GNode[w][j].Ne = i;
					/*The number of bikes which go via the vertex is bigger than half the capacity*/
					if (cur[w] >= Half) { 
						/*The number of bikes to send back becomes more*/
						GNode[w][j].Back = GNode[v][i].Back + temp;  
						/*The number of bikes to send out remains the same*/
						GNode[w][j].Send = GNode[v][i].Send; 
					}
					else {
						/*The number of bikes is smaller than half and no more than the bikes left before*/
						if (temp <= GNode[v][i].Back) {
							GNode[w][j].Back = GNode[v][i].Back - temp;
							GNode[w][j].Send = GNode[v][i].Send;
						}
						/*Increase the bikes to send out*/
						else {
							GNode[w][j].Send = GNode[v][i].Send + temp - GNode[v][i].Back;
							GNode[w][j].Back = 0;
						}
					}
				}
				/*Update the number of shortest paths to the current vertex*/
				p[w] = j;
			}
		}
	}
}

void CheckGood() {
	int i, j, k;
	int send, back, temp1,temp2,path[MAXVertex];
	send = back = INFINITY;
	/*Compare between different shortest paths. Choose the path with least bikes to send out in priority. If there are multiple choices, then choose the one with least bikes to send back.*/
	for (i = 0; i < p[sp]; i++) {
		if (send > GNode[sp][i].Send || (send == GNode[sp][i].Send&&back > GNode[sp][i].Back)) {
			send = GNode[sp][i].Send;
			back = GNode[sp][i].Back;
			temp1 = GNode[sp][i].Nv;
			temp2 = GNode[sp][i].Ne;
		}
	}
	i = temp1;
	j = temp2;
	k = 0;
	/*Trace back the best shortest path and print it out*/
	while (i >= 0) { 
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
