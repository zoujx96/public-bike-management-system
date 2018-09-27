# public-bike-management-system
Source code for a C programming project: Public Bike Management System

There is a public bike service in a city which provides great convenience to the tourists from all over the world. One may rent a bike at any station and return it to any other stations in the city.

The Public Bike Management Center (PBMC) keeps monitoring the real-time capacity of all the stations. A station is said to be in perfect condition if it is exactly half-full. If a station is full or empty, PBMC will collect or send bikes to adjust the condition of that station to perfect. And more, all the stations on the way will be adjusted as well.

When a problem station is reported, PBMC will always choose the shortest path to reach that station. If there are more than one shortest path, the one that requires the least number of bikes sent from PBMC will be chosen.

The system is supposed to figure out an optimal path from a center to a certain destination station in a public bike management problem. 
 
To be specific, the following inputs are given:  (1) the maximum capacity of each station (2) N: the total number of stations (3) the index of the problem station (the stations are numbered from 1 to N, and the center is represented by the vertex 0) (4) M: the number of roads (5) N non-negative numbers (each is the current bike number of the station respectively) (6) the information about M roads(each contains the two stations it connects and the time taken from one to the other)  
 
We are supposed to find the shortest path from the center to the destination station and adjust the bike number of each stations along the path to the half of the maximum capacity by sending or collecting bikes. If the shortest path is not unique, we are supposed to choose the one that needs the least bikes to be sent. Furthermore, if we still have several qualified paths, we are supposed to choose the one that needs the least bikes to be taken back.

The user should first type in the capacity of vertex, number of vertexes, destination vertex and number of edges, then type in the current bike number of each vertex, and after that, type in the graph information.
As an example, the user can type in like this:  
15 4 3 4  
14  
7  
0  
9  
1 2 6  
2 3 12  
1 4 16  
4 3 2  

The program will return the best shortest path from the bike center to the destination vertex.  


