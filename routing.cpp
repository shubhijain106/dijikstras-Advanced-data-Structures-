#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>

using namespace std;
class Edge;
enum Mark
{
    Visit, Uvisit, Seen, Noted
};
class Node                                               //defining node
{
public:
    int data;
    int key;
    int degree;
    Node * par;
    std::vector<Edge*> iedge;
    Node * lsibling;
    Node * rsibling;
    Node * ch;
    Node * previous;
    std::vector<Edge*> oedge;
    Mark condition;
    
    Node(int data, int key);
    Node();
    bool child(Node * node);
    bool sible(Node * node);
    bool del();
    Node* lsible();
    Node* rsible();
    void insert_iedge(Edge * edge);
    void insert_oedge(Edge * edge);
    
};

class Edge                                                   //defifning edge
{
    
public:
    Node* end;
    Node* head;
    int length;
    int cost;
    Edge(Node* head, Node* end, int length);
};
int dummy;
char filename1[100],filename2[100];

class Fib                                                   //defifning fibonacci heap
{
private:
    Node* degree_table[100];
    
    bool pairwise_combine(Node* root);
    Node * pointer;
    
public:
    
    Fib();
    Fib(Node *root);
    bool check_empty();
    bool addV(Node * node);
    void dec_key(int cost, Node* vertex);
    Node* find_minimum();
    Node* delete_minimum();
};

struct b_trie                                                   //defifning trie structure
{int leaf;
    struct b_trie *child[2];
};

struct b_trie *root=NULL;
Node::Node(int data, int key)
{
    condition = Uvisit;
    this->key = key;
    par = NULL;
    previous = NULL;
    ch = NULL;
    lsibling = NULL;
    this->data = data;
    rsibling = NULL;
    
    degree = 0;
    
}

Node::Node()
{
    par = NULL;
    ch = NULL;
    lsibling = NULL;
    rsibling = NULL;
    previous = NULL;
    degree = 0;
    condition =Uvisit;
}

Edge::Edge(Node* head, Node* end, int length)
{
    this->end = end;
    this->head = head;
    this->length = length;
}
std::vector<Node*> vertices;
long nodes_num,edge_num;



bool Node::sible(Node *n2)                                                   //function to add sibling to a node
{
    Node* t2 = rsible();
    if(t2 == NULL)
    {return false;}
    t2->rsibling = n2;
    n2->par = this->par;
    n2->rsibling = NULL;
    n2->lsibling = t2;
    if(par!=NULL)
    {par->degree+=1;}
    return true;
}


bool Node::del()                                                   //function to remove a node
{
    if(lsibling!=NULL)
    {lsibling->rsibling = rsibling;}
    if(rsibling!=NULL)
    {rsibling->lsibling = lsibling;}
    if(par!=NULL)
    {
        par->degree-=1;
        if(rsibling)
        {par->ch = rsibling;}
        else if(lsibling)
        {par->ch = lsibling;}
        else{
            par->ch = NULL;}
    }
    par = NULL;
    lsibling = NULL;
    rsibling = NULL;
    return true;
}

bool Node::child(Node *n1)                                                   //function to add child to a node
{
    if(ch == NULL)
    {
        ch = n1;
        degree = 1;
        n1->par = this;
    }
    else
    {ch->sible(n1);}
    return true;
}


vector<Edge*> edges;
void Node::insert_iedge(Edge * e1)                                                   //function to insert into vector containg incoming edges
{
    iedge.push_back(e1);
}

void Node::insert_oedge(Edge * e1)                                                   //function to insert into vector containg outgoing edges
{
    oedge.push_back(e1);
}


Node* Node::lsible()                                                   //function to return leftmost sibling of a node
{
    Node* t2 = this;
    if(this == NULL)
        return NULL;
    
    
    while(t2->lsibling!=NULL)
        t2 = t2->lsibling;
    return t2;
}

Node* Node::rsible()                                                   //function to return rightmost sibling of a node
{  Node* t2= this;
    if(this == NULL)
        return NULL;
    
    
    while(t2->rsibling!=NULL)
        t2 = t2->rsibling;
    return t2;
}

Fib::Fib()
{
    pointer = NULL;
}

Fib::Fib(Node *root)
{
    this->pointer = root;
    pointer->par = NULL;
    pointer->lsibling = NULL;
    pointer->rsibling = NULL;
    pointer->ch = NULL;
    pointer->degree = 0;
}


bool Fib::check_empty()                                                   //function to check if fibonacci heap is empty or not
{
    if(pointer != NULL)
        return false;
    else
        return true;
}

bool Fib::addV(Node * node)                                                   //function to insert a vertex in fibonacci heap
{
    if(node == NULL)
        return false;
    
    if(pointer != NULL){
        pointer->sible(node);
        if(pointer->key > node->key)
            pointer = node;
    }
    
    else
    {
        pointer = node;
    }
    return true;
}

Node* Fib::find_minimum()                                                   //function to find minimum element in fibonacci heap
{
    return pointer;
}

Node* Fib::delete_minimum()                                                   //function to delete the minimum element in fibonacci heap
{
    Node *nt = NULL;
    Node *t = pointer->ch;
    Node *t1= t->lsible();
    
    
    while(t1 != NULL)
    {
        nt = t1->rsibling;
        t1->del();
        pointer->sible(t1);
        t1 = nt;
    }
    
    t1 = pointer->lsible();
    if(t1 == pointer)
    {
        if(pointer->rsibling==NULL)
        {
            Node* result = pointer;
            pointer->del();
            pointer = NULL;
            return result;
        }
        
        else
        {
            
            t1 = pointer->rsibling;
        }
    }
    Node* result = pointer;
    pointer->del();
    pointer = t1;
    
    int i=0;
    while(i<nodes_num)
    {degree_table[i] = NULL;
        i++;
    }
    
    while(t1!=NULL)
    {
        if(pointer->key > t1->key)
        {
            pointer = t1;
        }
        
        nt = t1->rsibling;
        pairwise_combine(t1);
        t1 = nt;
    }
    
    return result;
}

bool Fib::pairwise_combine(Node* r1)          //function to pairwise combine two roots
{
    if(degree_table[r1->degree] != NULL)
    {
        Node* lv = degree_table[r1->degree];
        degree_table[r1->degree] = NULL;
        
        if( r1 == pointer||lv->key > r1->key)
        {
            lv->del();
            r1->child(lv);
            if(degree_table[r1->degree] == NULL)
                degree_table[r1->degree] = r1;
            else
                pairwise_combine(r1);
            
        }
        else
        {
            r1->del();
            lv->child(r1);
            if(degree_table[lv->degree] != NULL)
                pairwise_combine(lv);
            else
                degree_table[lv->degree] = lv;
            
        }
        return true;
        
        
    }
    else
    {
        
        degree_table[r1->degree] = r1;
        return false;
    }
}


void Fib::dec_key(int data, Node* v1)         //function to decrease key of a node in fibonacci heap
{
    v1->key = data;
    
    if(v1->par != NULL)
    {
        v1->del();
        pointer->sible(v1);
    }
    if( pointer->key > v1->key)
        pointer = v1;
}
int traverse(struct b_trie *temp,int *nwhop)                                                   //function to traverse the binary trie
{
    if(temp==NULL)
        return 1;
    if(temp->leaf!=-1)
    {
        *nwhop=temp->leaf;
        return 1;
    }
    int lhop,rhop;
    int leftch=traverse(temp->child[0],&lhop);
    int rtch=traverse(temp->child[1],&rhop);
    if(temp->child[0]==NULL&&temp->child[1]==NULL)
    {
        if(leftch&&rtch)
        {
            if(temp->child[0]==NULL)
            {
                temp->leaf=rhop;
                *nwhop=rhop;
                
            }
            else
            {
                temp->leaf=lhop;
                *nwhop=lhop;
            }
            free(temp->child[0]);
            free(temp->child[1]);
            temp->child[0]=NULL;
            temp->child[1]=NULL;
            return 1;
        }
        return 0;
    }
    else
    {
        if(leftch&&rtch&&lhop==rhop)
        {
            free(temp->child[0]);
            free(temp->child[1]);
            temp->child[0]=NULL;
            temp->child[1]=NULL;
            temp->leaf=lhop;
            *nwhop=lhop;
            return 1;
        }
        return 0;
        
    }
}

vector<char *> ip;
struct b_trie * newb_trie()
{
    struct b_trie *p=(struct b_trie*)malloc(sizeof(struct b_trie));
    p->child[0]=NULL;
    p->child[1]=NULL;
    p->leaf=-1;
    return p;
}
void insertb_trie(int dest,int nwhop)
{
    char *dip=ip[dest];
    
    if(root==NULL)
        root=newb_trie();
    struct b_trie *temp=root;
    int bit;
    while(*dip)
    {
        bit=*dip-'0';
        if(temp->child[bit]==NULL)
            temp->child[bit]=newb_trie();
        temp=temp->child[bit];
        dip++;
    }
    temp->leaf=nwhop;
}

int dijikstra(int src,int dest)           //function that implements dijikstra's algorithm
{
    
    long n,e;
    FILE * fp;
    fp=fopen(filename1,"r");
    int head,length,end;
    vector<Edge*> edges;
    vector<Node*> vertices;
    fscanf(fp,"%ld%ld",&n,&e);
    for(int j=0; j<=n-1 ; j++)
    {
        Node* v = new Node(j, -1);
        vertices.push_back(v);
    }
    
    while(e--)
    {
        
        fscanf(fp,"%d%d%d",&head,&end,&length);
        Edge* edge;
        edge = new Edge(vertices[head], vertices[end], length);
        edge->head->insert_iedge(edge);
        edge->end->insert_oedge(edge);
        edges.push_back(edge);
        edge = new Edge(vertices[end],vertices[head] , length);
        edge->head->insert_iedge(edge);
        edge->end->insert_oedge(edge);
        edges.push_back(edge);
    }
    fclose(fp);
    
    vertices[src]->key=0;
    vertices[src]->condition = Visit;
    
    
    
    Fib* heap = new Fib();
    
    heap->addV(vertices[src]);
    do
    {Node* v = heap->delete_minimum();
        
        v->condition = Seen;
        
        for(int i = 0; i < v->iedge.size(); i++)
        {
            
            Edge* ce = v->iedge[i];
            Node* h = ce->end;
            
            if(h->condition != Seen)
            {
                if(h->condition == Uvisit)
                {h->condition = Visit;
                    h->previous = v;
                    h->key = v->key + ce->length;
                    heap->addV(h);
                }
                else if(h->key > v->key + ce->length )
                {h->previous = v;
                    heap->dec_key(v->key + ce->length, h);
                }
            }
        }
    }
    while(!heap->check_empty());
    
    
    Node* temp = vertices[dest];
    
    if(!temp->previous)
    {
        printf("No path");
        return 0;
    }
    
    int ans=(int)vertices[dest]->key;
    if(src==dummy)
        printf("%d\n",ans);
    int nwhop,curhop;
    for(int i=0;i<=n-1;i++)
    {
        if(i==src)
            continue;
        curhop=i;
        nwhop=vertices[i]->previous->data;
        if(nwhop!=src)
        {while(nwhop!=src)
        {
            curhop=nwhop;
            nwhop=vertices[nwhop]->previous->data;
        }
            insertb_trie(i,curhop);
            
            
        }
        else{
            insertb_trie(i,i);
        }
    }
    
    int z;
    traverse(root,&z);
    
    char *dip=ip[dest];
	   struct b_trie *tem=root;
	   int bit;
    while(tem->leaf==-1)
    {
        bit=*dip-'0';
        printf("%d",bit);
        tem=tem->child[bit];
        dip++;
    }
    nwhop=tem->leaf;
    printf(" ");
    root=NULL;
    if(nwhop==src)
        return dest;
    return nwhop;
}

int main(int argc,char *argv[])
{
    int src,dest,nwhop;
    src=atoi(argv[3]);
    dest=atoi(argv[4]);
    strcpy(filename1,argv[1]);
    strcpy(filename2,argv[2]);
    dummy=src;
    FILE *bp;
    bp=fopen(filename1,"r");
    fscanf(bp,"%ld%ld",&nodes_num,&edge_num);
    fclose(bp);
    
    FILE * fp;
    char input_ip[33],binary[33],za[2];
    int num=0,count=0,p,l;
    fp=fopen(filename2,"r");
    for(int i=0;i<nodes_num;i++)
    {
        l=0;
        count=0;
        fscanf(fp,"%s",input_ip);
        while(input_ip[l]!='\0')
        {
            if(input_ip[l]>='0'&&input_ip[l]<='9')
            {
                num=num*10+(input_ip[l]-'0');
            }
            else
            {count++;
                p=8*count-1;
                while(num)
                {
                    int x=num%2;
                    binary[p--]=x+'0';
                    num=num/2;
                }
                while(p!=(8*count-9))
                    binary[p--]='0';
            }
            l++;
        }
        count++;
        p=8*count-1;
        while(num)
        {
            binary[p--]= num%2+'0';
            num=num/2;
        }
        while(p!=(8*count-9))
            binary[p--]='0';
        binary[32]='\0';
        char *t=(char *)malloc(sizeof(char)*33);
        strcpy(t,binary);
        ip.push_back(t);
    }
    fclose(fp);
    
    int nw=src;
    while(nw!=dest)
    {
        
        nw=dijikstra(nw,dest);
    }
    
    return 0;
}