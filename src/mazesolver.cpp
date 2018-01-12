#include <iostream>
#include <stdint.h>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <opencv2/opencv.hpp>
#include <opencv\highgui.h>

#include "node.h"
#include "edge.h"
#include "point.h"



// Create a map that STORES nodes based on their location
std::map<Point,Node> graph; 

// Add a node to a graph
Node insert_node(std::map<Point, Node>& g, Point& pnt, node_type_t type){
    Node n = Node(pnt,type);
    g[pnt] = n;
    return n;
}

// Link 2 nodes in a graph
bool link_nodes(std::map<Point, Node>& g, Point& pnt_a, Point& pnt_b){
    // Check that the nodes exist
    if (g.find(pnt_a) == g.end() || g.find(pnt_b) == g.end() ){
        return false;
    }

    // Grab node reference based on location
    Node& a = g[pnt_a];
    Node& b = g[pnt_b];
    // Use distance as edge cost
    double cost = pnt_a.dist_to(pnt_b);
    // Link is undirected so create edge for each node
    Edge a2b = Edge(&b,cost); // Create a pointer to node
    Edge b2a = Edge(&a,cost);
    // Add edge to nodes
    a.adj.push_back(a2b);
    b.adj.push_back(b2a);

    return true;
}


int main() {

    // read image
    cv::Mat image;

    image = cv::imread("maze.png", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        printf("Could not open or find the image\n");
        return -1;
    }

    // Grayscale matrix
    cv::Mat grayscale (image.size(), CV_8U);

    // Convert BGR to Gray
    cv::cvtColor( image, grayscale, CV_BGR2GRAY );

    // Binary image
    cv::Mat binary(grayscale.size(), grayscale.type());

    // Apply thresholding
    cv::threshold(grayscale, binary, 100, 255, cv::THRESH_BINARY);

    // Validate maze
    // Check for entrance and exit in corners
    if(binary.at<uint8_t>(1,0) != 255 || binary.at<uint8_t>(binary.rows-2,binary.cols-1) != 255){
        printf("Error: Unable to find entry or exit\n");
        return -1;
    }
    
    // Add the entry node
    insert_node(graph,Point(0,1),node_type_t::START);

    // Build graph of maze
    for(int y = 1; y < binary.rows-1; y++){
        for(int x = 1; x < binary.cols-1; x++){
            bool add_node = false;

            // only look at white pixels
            if(binary.at<uint8_t>(y,x) == 255){
            
                // Check for ONLY left edge
                if(binary.at<uint8_t>(y,x-1) == 0 && binary.at<uint8_t>(y,x+1) == 255){
                    add_node = true;
                }
                // Check for ONLY right edge
                if(binary.at<uint8_t>(y,x-1) == 255 && binary.at<uint8_t>(y,x+1) == 0){
                    add_node = true;
                }
                // Check for ONLY bottom opening
                if(binary.at<uint8_t>(y-1,x) == 0 && binary.at<uint8_t>(y+1,x) == 255 ){
                    add_node = true;
                }
                // Check for ONLY up opening
                if(binary.at<uint8_t>(y-1,x) == 255 && binary.at<uint8_t>(y+1,x) == 0 ){
                    add_node = true;
                }
                // Check for 4 way
                if(binary.at<uint8_t>(y-1,x) == 255 && binary.at<uint8_t>(y+1,x) == 255 &&
                     binary.at<uint8_t>(y,x-1) == 255 && binary.at<uint8_t>(y,x+1) == 255){
                    add_node = true;
                }
                if(add_node){

                    // insert a node
                    insert_node(graph,Point(x,y),node_type_t::NORMAL);
                    
                    // Try to link with leftmost node to the new node
                    for(int i = x-1; i >= 0; i--){
                        // Check for a wall
                        if(binary.at<uint8_t>(y,i) == 0){
                            break;
                        }
                        // Found an exisiting node
                        if(graph.find(Point(i,y)) != graph.end()){
                            bool ret = link_nodes(graph,Point(x,y),Point(i,y));
                            if(ret == false){
                                printf("WARNING: Failed to link nodes\n");
                            }
                            break;
                        }
                    }
                            
                    
                    // Try to link with uppermost node to the new node
                    for(int i = y-1; i >= 0; i--){
                        // Check for a wall
                        if(binary.at<uint8_t>(i,x) == 0){
                            break;
                        }
                        // Found an exisiting node
                        if(graph.find(Point(x,i)) != graph.end()){
                            bool ret = link_nodes(graph,Point(x,y),Point(x,i));
                            if(ret == false){
                                printf("WARNING: Failed to link nodes\n");
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    

    // Add exit node
    int x = binary.cols-1;
    int y = binary.rows-2;
    insert_node(graph,Point(x,y),node_type_t::END);

    // Link exit node
    for(int i = x-1; i >= 0; i--){
        // Check for a wall
        if(binary.at<uint8_t>(y,i) == 0){
            break;
        }
        // Found an exisiting node
        if(graph.find(Point(i,y)) != graph.end()){
            bool ret = link_nodes(graph,Point(x,y),Point(i,y));
            if(ret == false){
                printf("WARNING: Failed to link nodes\n");
            }
            break;
        }
    }
    /*
    // BFS Traverse the graph
    // Create output image
    cv::namedWindow( "BFS", cv::WINDOW_AUTOSIZE );
    cv::Mat bfs;
    // Track unvisited nodes
    std::queue<Node*> q;
    bool finished = false;
    // Visit root
    Node& root = graph[Point(0,1)];
    root.visited = true;
    q.push(&root);
    // Traverse until we've visited every node
    while(!q.empty() && !finished){
        // dequeue node
        Node* n = q.front();
        q.pop();
        
        // Check the neighbors of the current node
        for(std::vector<Edge>::iterator it = n->adj.begin(); it != n->adj.end(); it++){
            Node* nn = (*it).node;
            // Only look at it if it hasn't been visited
            if(!nn->visited){
                // Enqueue
                nn->visited = true;
                q.push(nn);
                // Visit node (render)
                line(image, cv::Point(n->loc.x,n->loc.y),cv::Point(nn->loc.x,nn->loc.y), cv::Scalar(255,0,0));
                image.at<cv::Vec3b>(n->loc.y,n->loc.x) = cv::Vec3b(0,255,0); // End point
                image.at<cv::Vec3b>(nn->loc.y,nn->loc.x) = cv::Vec3b(0,0,255); // End point
                cv::resize(image,bfs,cv::Size(800,800), 0,0,cv::INTER_AREA); //resize image
                cv::imshow( "BFS", bfs );
                cv::waitKey(1);

                if(nn->type == node_type_t::END){
                    finished = true; 
                    break;
                }

            }
        }
    }
    */

    // DFS Traverse the graph
    // Create output image
    cv::namedWindow( "DFS", cv::WINDOW_AUTOSIZE );
    cv::Mat dfs;
    // Track unvisited nodes
    std::stack<Node*> s;
    bool finished = false;
    // Visit root
    Node& root = graph[Point(0,1)];
    root.visited = true;
    s.push(&root);
    // Traverse until we've visited every node
    while(!s.empty() && !finished){
        // pop node
        Node* n = s.top();
        s.pop();
        
        // Check the neighbors of the current node
        for(std::vector<Edge>::iterator it = n->adj.begin(); it != n->adj.end(); it++){
            Node* nn = (*it).node;
            // Only look at it if it hasn't been visited
            if(!nn->visited){
                // Enqueue
                nn->visited = true;
                s.push(nn);
                // Visit node (render)
                line(image, cv::Point(n->loc.x,n->loc.y),cv::Point(nn->loc.x,nn->loc.y), cv::Scalar(255,0,0));
                image.at<cv::Vec3b>(n->loc.y,n->loc.x) = cv::Vec3b(0,255,0); // End point
                image.at<cv::Vec3b>(nn->loc.y,nn->loc.x) = cv::Vec3b(0,0,255); // End point
                cv::resize(image,dfs,cv::Size(800,800), 0,0,cv::INTER_AREA); //resize image
                cv::imshow( "DFS", dfs );
                cv::waitKey(1);

                if(nn->type == node_type_t::END){
                    finished = true; 
                    break;
                }
            }
        }
    }
    
    return 0;
}