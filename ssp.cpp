#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <vector>



class Node;

class Edge                                       //Edge declaration stores beginning and end nodes and distance between these nodes
{
public:
    Node* end;
    Node* head;
    double length;
    double cost;
    
    Edge(Node* end, Node* head, double length);
};

enum Mark
{
    Visit, Uvisit, Seen, Noted
};
class Node                                       //Node declaration stores parent,children,leftSibling,Rightsibling and key
{
public:
    Node * par;
    Node * lsibling, * rsibling;
    Node * ch;
    Node * previous;
    Node(int data, double key);
    int data;
    double key;
    int degree;
    std::vector<Edge*> iedge;
    std::vector<Edge*> oedge;
    Mark condition;
    Node();
    bool child(Node * n1);
    bool sible(Node * n1);
    bool del();
    Node* lsible();
    Node* rsible();
    void add_iedge(Edge * e1);
    void add_oedge(Edge * e2);
    
};

class Fib                                       //Fibonacci heap declaration stores circular linked list of roots and a pointer pointing to a minimum element
{
private:
    Node* degree_table[100];
    bool pairwise_combine(Node* r1);
    Node * minPointer;
    
public:
    Fib();
    Fib(Node *r1);
    bool check_empty();
    bool addV(Node * n1);
    void dec_key(double c1, Node* n1);
    Node* find_minimum();
    Node* delete_minimum();
};

Edge::Edge(Node* end, Node* head, double length)               //Constructor of edge class
{
    this->end = end;
    this->head = head;
    this->length = length;
}

Node::Node(int data, double key)               //Parameterized constructor of node class
{
    par = NULL;
    ch = NULL;
    this->data = data;
    lsibling = NULL;
    rsibling = NULL;
    this->key = key;
    previous = NULL;
    degree = 0;
    condition = Uvisit;
}

Node::Node()                                //Unparameterized constructor of node class
{
    par = NULL;
    previous = NULL;
    lsibling = NULL;
    rsibling = NULL;
    degree = 0;
    condition = Uvisit;
    ch = NULL;
}



bool Node::sible(Node *n1)                                //Function to create a sibling of a node
{
    Node* t2 = rsible();
    if(t2 == NULL)
        return false;
    
    t2->rsibling = n1;
    n1->par = this->par;
    n1->lsibling = t2;
    n1->rsibling = NULL;
    
    if(par!=NULL)
        par->degree+=1;
    
    return true;
}



void Node::add_iedge(Edge * e1)                              //Function to store incoming edges to a node
{
    iedge.push_back(e1);
}

void Node::add_oedge(Edge * e2)                              //Function to store outgoing edges from a node
{
    oedge.push_back(e2);
}


Node* Node::lsible()                                        //Function to return leftmost sibling of a node
{
    Node* t2 = this;
    if(this == NULL)
        return NULL;
    while(t2->lsibling!=NULL)
        t2 = t2->lsibling;
    return t2;
}

Node* Node::rsible()                                        //Function to return righttmost sibling of a node
{
    Node* t2 = this;
    if(this == NULL)
        return NULL;
    while(t2->rsibling)
        t2 = t2->rsibling;
    return t2;
}
bool Node::del()                                        //Function to remove a node
{
    
    
    if(par!=NULL)
    {
        par->degree-=1;
        if(lsibling!=NULL)
        {par->ch = lsibling;}
        else if(rsibling!=NULL)
        {par->ch = rsibling;}
        else
        {par->ch = NULL;}
    }
    
    if(lsibling!=NULL)
    {lsibling->rsibling = rsibling;}
    if(rsibling!=NULL)
    {rsibling->lsibling = lsibling;}
    rsibling = NULL;
    par = NULL;
    lsibling = NULL;
    return true;
}
bool Node::child(Node *n1)                                        //Function to add child to a node
{
    if(ch != NULL)
        ch->sible(n1);
    else
    {
        ch = n1;
        n1->par = this;
        degree = 1;
    }
    
    return true;
}

Fib::Fib()                                                           //Unparameterized constructor of fibonacci heap
{
    minPointer = NULL;
}

Fib::Fib(Node *r1)                                         //Parameterized constructor of fibonacci heap
{
    this->minPointer = r1;
    minPointer->par = NULL;
    minPointer->lsibling = NULL;
    minPointer->ch = NULL;
    minPointer->degree = 0;
    minPointer->rsibling = NULL;
    
}



bool Fib::check_empty()                                         //check if initially fibonacci heap is empty or not
{
    if(minPointer == NULL)
    {return true;}
    else
    {return false;}
}

bool Fib::addV(Node * n1)                                        //insert node into heap
{
    if(n1 == NULL)
    {return false;}
    
    if(minPointer != NULL)
    {   minPointer->sible(n1);
        if(minPointer->key > n1->key)
        {minPointer = n1;}
    }
    else
    {minPointer = n1;}
    return true;
}

Node* Fib::find_minimum()                                       //find miinimum element in heap
{
    return minPointer;
}

Node* Fib::delete_minimum()                                       //delete miinimum element from heap
{
    Node *t = minPointer->ch;
    Node *t1 = t->lsible();
    Node *nt = NULL;
    while(t1 != NULL)
    {
        nt = t1->rsibling;
        t1->del();
        minPointer->sible(t1);
        t1 = nt;
    }
    
    t1 = minPointer->lsible();
    if(t1 == minPointer)
    {
        if(!(minPointer->rsibling))
        {
           	Node* out = minPointer;
            minPointer->del();
            minPointer = NULL;
            return out;
        }
        
        
        else
        {t1 = minPointer->rsibling;}
    }
    Node* result = minPointer;
    minPointer->del();
    minPointer = t1;
    int h =0;
    
    while(h<100){
        
        degree_table[h] = NULL;
        h++;
        
    }
    while(t1)
    {if( minPointer->key > t1->key)
    {minPointer = t1;}
        
        nt = t1->rsibling;
        pairwise_combine(t1);
        t1 = nt;
    }
    
    return result;
}

bool Fib::pairwise_combine(Node* r1)                                       //do pairwise combine
{
    if(degree_table[r1->degree] != NULL)
    {   Node* pairwise_combineVertex = degree_table[r1->degree];
        degree_table[r1->degree] = NULL;
        
        if( pairwise_combineVertex->key > r1->key || r1 == minPointer)
        {
            pairwise_combineVertex->del();
            r1->child(pairwise_combineVertex);
            if(degree_table[r1->degree] == NULL)
                degree_table[r1->degree] = r1;
            
            else
            {pairwise_combine(r1);}
        }
        else
        {r1->del();
            pairwise_combineVertex->child(r1);
            if(degree_table[pairwise_combineVertex->degree] == NULL)
            {degree_table[pairwise_combineVertex->degree] = pairwise_combineVertex;}
            
            else
            {pairwise_combine(pairwise_combineVertex);}
        }
        return true;
        
    }
    else
    {degree_table[r1->degree] = r1;
        return false;
    }
}


void Fib::dec_key(double c1, Node* n1)                                       //do decrease key operation in heap
{
    n1->key = c1;
    
    if(n1->par != NULL)
    {
        n1->del();
        minPointer->sible(n1);
    }
    
    if(n1->key < minPointer->key)
    {minPointer = n1;}
}

char filename[100];
int main(int argc,char *argv[])
{
    
    int src,dest;
    double dist;
    src=atoi(argv[2]);
    dest=atoi(argv[3]);
    strcpy(filename,argv[1]);
    int we;
    std::vector<Node*> nodes;
    long num_node,num_edg;
    std::vector<Edge*> edges;
    FILE * fp;
    fp=fopen(filename,"r");
    
    fscanf(fp,"%ld%ld",&num_node,&num_edg);                    //scan no. of nodes,edges,src and dest.
    
    
    int j=0;
    while(j<num_node-1)                                       //store all the nodes
    {
        Node* vi = new Node(j, -1);
        nodes.push_back(vi);
        j++;
    }
    nodes.push_back(new Node(num_node-1, 0));
    nodes[num_node-1]->condition = Visit;
    int beg,end;
    
    while(num_edg--)                                      //store all the undirected edges for every node
    {
        
        fscanf(fp,"%d%d%lf",&beg,&end,&dist);
        Edge* e2;
        e2 = new Edge(nodes[end], nodes[beg], dist);
        e2->head->add_iedge(e2);
        e2->end->add_oedge(e2);
        edges.push_back(e2);
        e2 = new Edge(nodes[beg],nodes[end] , dist);
        e2->head->add_iedge(e2);
        e2->end->add_oedge(e2);
        edges.push_back(e2);
    }
    
    
    Fib* p = new Fib();
    p->addV(nodes[num_node-1]);                                    //add nodes to heap
    do                                                              //dijikstra's algorithm
    {
        Node* vy = p->delete_minimum();
        
        vy->condition = Seen;
        int l=0;
        while(l < vy->iedge.size())
        {
            Edge* g = vy->iedge[l];
            Node* hg = g->end;
            
            if(hg->condition != Seen)
            {
                if(hg->key > vy->key + g->length)
                {   hg->previous = vy;
                    p->dec_key(vy->key + g->length, hg);
                }
                else if(hg->condition == Uvisit)
                {   hg->condition = Visit;
                    hg->key = vy->key + g->length;
                    hg->previous = vy;
                    p->addV(hg);
                }
            }l++;
        }
    }
    while(!p->check_empty());
    Node* u = nodes[src];
    if(!u->previous)
    {
        printf("\n No path \n");
        fclose(fp);
        return 0;
    }
    we=(int)nodes[src]->key;
    printf("%d\n",we);
    while(u!=NULL)
    {
        printf("%d", u->data);
        u = u->previous;
        if(u!=NULL)
            printf(" ");
        if(u==nodes[src])
            break;
    }
    printf("\n");
    
    
    return 0;
}