#include <bits/stdc++.h>
using namespace std;
#define MAX 10


int n=4; //number of array size

//this says about the type of key value
struct Key_Type{
        string s;
};

//Node information
struct Node{
        struct Node* pointer[MAX+1]; //to save the address of the pointer
        struct Key_Type key[MAX+1]; //key value
        bool key_condition[MAX+1]; //saying if key is valid or not
        int tot_size; //how many nodes this have
        int type_of_node; // to identify this is a leaf node or not
};

struct Node* current_node; //current node with which I am working
struct Node* root;//this is from where I start traversing


//function to check if node is filled or not
bool node_fill_checking(struct Node* node){
	if(node->tot_size > (n-1)) {
		//this node has more element than n
		//need to split
		return true; 
	}
	return false; //element number has not overflowed
}

//function to initiate key condition
void init_key_condition(Node *temp){
	memset(temp->key_condition,false,sizeof(temp->key_condition));
}

//function to insert into B+ Tree
void insertion(Key_Type k){
      bool inserted = false; //this says if the value is already inserted or not
      Node *temp = root; //starting with root
      vector<Node*>V; //to save the traversing path
      while(true){
			
			if(inserted){
				cout<<"condition 1"<<endl;
				//if value is already inserted, need to check the conditions of parent nodes
				//first check if this node is overflowed or not
				temp=V[V.size()-1]; //the last node
				V.pop_back();//because dealing with last node is complete 
				bool overflow = node_fill_checking(temp);
				if(overflow) {
					//overflowed need to split
					if(temp->type_of_node == 1) {
						cout<<"spliting for leaf node " << endl;
						//leaf - leaf split
						//left split member number
						int left_side_val = ceil((double)(n-1)/2.0);
						//right split member number
						int right_side_val = temp->tot_size-left_side_val;
						
						//creating New right side 
						Node *notun = new Node();
						//node characteristic set
						notun->type_of_node = 1;//leaf node
						//tot size set
						notun->tot_size = right_side_val;
						//key and pointer set
						int next_start_val = 2*left_side_val-1 + 2;
						notun->pointer[0] = NULL; //leaf node nothing to point
						init_key_condition(notun); //key_condition array is initialized with being false
						for(int j=next_start_val,i=1; j<=2*temp->tot_size-1;j=j+2,i=i+2){
							//debug: cout<<"j= "<<j<<" i= "<<i<<endl;
							//key set
							notun->key[i]=temp->key[j];
							//key condition set
							notun->key_condition[i]=true;
							//pointer set : left side pointer copy
							notun->pointer[i-1]=temp->pointer[j-1];
						} //right side array done complete
						if(temp->pointer[2*n-2] == NULL) cout<<"YES"<<endl;
						notun->pointer[2*n-2]=temp->pointer[2*n-2]; //link to previous siblings
						
						//modify left side leaf
						//pointer set
						cout<<"creating Link to sibling " << endl;
						temp->pointer[2*n-2]=notun; //last pointer will point to the right sibling
						next_start_val = 2*left_side_val; //starting pointer
						for(int i=next_start_val;i<(2*n-2);i=i+2){
							temp->pointer[i]=NULL; //except the last pointer others should be null
						}
						//key set+key_condition set
						next_start_val = 2*left_side_val+1;
						for(int i=next_start_val;i<MAX;i=i+2){
							temp->key[i].s=""; //other keys of this node should be NULL because of split
							temp->key_condition[i]=false; //so they are false
						}
						//tot_size will change
						temp->tot_size=left_side_val;
						
						//now need to modify parent
						if(V.size() == 0) {
							//size is 0, it means there is no other parent
							//need to create parent
							Node *parent = new Node();
							//this is a non leaf
							parent->type_of_node = 2;
							//tot_size : just initiated
							parent->tot_size=1;
							//key condition set
							init_key_condition(parent);
							//key_val+key condition
							parent->key[1]=notun->key[1];
							parent->key_condition[1]=true;
							//pointer balance
							parent->pointer[0]=temp; //0 containse temp address:left
							parent->pointer[2]=notun;//right address
							V.push_back(parent);
							continue;
							
						}
						else {
							//there is parent
							Node *parent = V[V.size()-1];
							int i=1;
							bool go_right=false;
							Key_Type obj = notun->key[1];
							for(int cnt=1;cnt<=parent->tot_size;cnt++){
								if(parent->key[i].s <= obj.s){
									go_right=true; //need to go to right
									i=i+2; //increasing i value
									continue;
								}
								else {
									go_right=false;
									break;
								}
							}
							if(go_right){
								//normal set
								//tot_size increases
								parent->tot_size=parent->tot_size+1;
								//pointer set
								parent->pointer[i+1]=notun; //(i+1) pointer will point to new node
								if(parent->pointer[i-1] != temp){
									//this should never reach
									int value=0;
									assert(value != 1);
								}
								parent->key[i] = obj;
								parent->key_condition[i]=true;
								continue;
								
							}
							else {
								//need to sort things
								int last_index = parent->tot_size*2-1;
								//copying from last
								for(int j=last_index;j>=i;j=j-2){
									//pointer
									parent->pointer[j+3]=parent->pointer[j+1];
									//key set
									parent->key[j+2]=parent->key[j];
									//key condition set
									parent->key_condition[j+2]=true;
								}
								//set value
								parent->key[i]=obj; 
								//pointer
								parent->pointer[i+1]=notun; 
								if(parent->pointer[i-1] != temp){
									//this segment should never reach
									int value=0;
									assert(value != 1);
								}
								//tot_size
								parent->tot_size = parent->tot_size+1;
								//type of node unchange
								//modification in parent is complete
								continue;
							}
							
						}
					}
					else if(temp->type_of_node == 2){
						//non leaf
						int left_ptr_number = ceil((double)n/2.0);
						//creation of new node
						Node *notun = new Node();
						//type of node
						notun->type_of_node=2;//its a non leaf node
						notun->tot_size=0; //init
						int st_index=2*(left_ptr_number+1)-2; //here from pointer will start to be copied
						//copy pointer
						for(int j=0,cnt=left_ptr_number+1;cnt<=n;cnt++){
							notun->pointer[j]=temp->pointer[st_index];//copying the pointers
							j=j+2; //next pointer pos
							st_index=st_index+2;//next pointer
						}
						//key copy+key_condition
						int left_side_val = left_ptr_number-1; //value number will be 1 less than pointer
						int right_side_val = temp->tot_size-left_side_val-1;//one will go top and others will be on right
						//tot_size update
						notun->tot_size=right_side_val; //tot sizee initialized
						st_index=2*(left_ptr_number+1)-2+1; //start index of key to be copied
						for(int cnt=1,i=1;cnt<=right_side_val;cnt++){ //number of how much right side will have
							notun->key[i]=temp->key[st_index];
							notun->key_condition[i]=true;
							i=i+2;
							st_index=st_index+2;
						}
						
						//the index which will go up
						int index_to_go_up = 2*(left_ptr_number+1)-2-1; //imediate previous key value
						Key_Type obj = temp->key[index_to_go_up];
						
						//modify the left
						//pointer update
						st_index=2*(left_ptr_number+1) - 2;
						//these pointer are already copied to the new so here they will be null
						for(int i=st_index;i<=(2*n-2);i=i+2){
							temp->pointer[i]=NULL;
						}
						//end pointer will point to sibling
						//**temp->pointer[2*n-2] = notun;
						//key update
						for(int i=st_index-1;i<=2*(n-1)-1;i=i+2){
							//these values will be null
							temp->key[i].s="";
							temp->key_condition[i]=false;
						}
						//tot size updated
						temp->tot_size=left_side_val;
						//Now dealing with parent
						if(V.size() == 0) {
							//there is no parent
							//need to create parent
							Node *parent=new Node();
							//tot size
							parent->tot_size=1;
							//type_of_node
							parent->type_of_node=2;//non leaf node
							init_key_condition(parent); //initialization of key condtion
							//key update
							parent->key[1]=obj;
							//pointer update
							parent->pointer[0]=temp;
							parent->pointer[2]=notun;
							//insertion into vector
							V.push_back(parent);
							continue;
						}
						else {
							//parent already exists
							Node *parent=V[V.size()-1];
							int i=1;
							bool go_right=false;
							for(int cnt=1;cnt<=parent->tot_size;cnt++){
								if(parent->key[i].s<=obj.s){
									go_right=true; //go_right
									i=i+2;
								}
								else{
									//found place to sort
									go_right=false;
									break;
								}
							}
							if(go_right){
								//node is already sorted
								//just insert into last
								//tot_size
								parent->tot_size++;
								//pointer
								parent->pointer[i+1]=notun;
								if(parent->pointer[i-1] != temp){
									//this segment should never reach
									int value=0;
									assert(value !=1); 
								}
								//key
								parent->key[i]=obj;
								parent->key_condition[i]=true;
								//already in vector
								continue;
							}
							else{
								//contradiction occured
								int last_index=parent->tot_size*2-1;
								for(int j=last_index;j>=i;j=j-2){
									//key set
									parent->key[j+2]=parent->key[j];
									//key condition
									parent->key_condition[j+2]=true;
									//pointer
									parent->pointer[j+3]=parent->pointer[j+1];
								}
								//key
								parent->key[i]=obj;
								//pointer
								parent->pointer[i+1]=notun;
								if(parent->pointer[i-1] != temp){
									//this segemet should never reach
									int value=0;
									assert(value != 1);
								}
								parent->tot_size++;
								continue;
							}
							
						}
					}
					
					
				}
				else {
					//not overflowed
					//no need to split
					//initiate root
					if(V.size() == 0) {
						root=temp;
					}
					else {
						root = V[0];
					}
					break;
				}
			}
			else {
				cout<<"condition 2"<<endl;
				//if value is not inserted traverse
				int i=1;
				//cheking go right condition
				bool go_right = false; 
				//traversing through the members
				for(int cnt=1;cnt<=temp->tot_size;cnt++){
					if(temp->key[i].s<k.s) {
						//key value same go right 
						i=i+2;
						//making to say to go right
						go_right=true; 
						continue;
					}
					else {
						//contradiction found need to check left
						go_right=false;
						break;
					}
					
				}
				
				if(go_right){
					
					//saving the last pointer
					Node *save=NULL;
					if(temp->pointer[2*n-2] != NULL) {
						save=temp->pointer[2*n-2];
					}
					
					//I have been given larger number which is present at this moment at this node
					if(temp->type_of_node == 1) {
						//its a leaf node
						//left side pointer set
						temp->pointer[i-1]=NULL; 
						//key set
						temp->key[i]=k;
						//key condition set
						temp->key_condition[i]=true;
						//tot size 1 increase
						temp->tot_size=temp->tot_size+1;
						
						//saving the last pointer
						temp->pointer[2*n-2]=save;
						
						//insert into vector to check validation
						V.push_back(temp);
						//value inserted 
						inserted=true;
						continue; //now need to check if we have to split or not for overflow
					}
					else if(temp->type_of_node == 2) {
						// a non leaf node
						//need to go downwards to higher values
						V.push_back(temp);
						if(temp->pointer[i-1] != NULL) {
							temp=temp->pointer[i-1];
							continue;
						}
						else {
							//asserting because this segment should never reach
							int value = 0;
							assert(value != 1);
						}
					}
				}
				else {
					//condradiction found
					cout<<"NODE VAL " << endl;
					for(int cnt=1,p=1;cnt<=temp->tot_size;cnt++,p=p+2){
						cout<<temp->key[p].s<<" ";
					}
					cout<<endl;
					
					if(temp->type_of_node==1){
						//leaf node
						//denotes last index of present array
						int last_index=temp->tot_size * 2 - 1;
						//copying from last
						
						//last pointer save
						Node *save=NULL;
						if(temp->pointer[2*n-2] != NULL) {
							save=temp->pointer[2*n-2];
						}
						
						for(int j=last_index; j>=i;j=j-2){
							//jth index goes to (j+2)th index
							//key
							temp->key[j+2]=temp->key[j];
							//key condition
							temp->key_condition[j+2]=true;
							//pointer
							//*********changed from (j+3) to (j+1)
							temp->pointer[j+1]=temp->pointer[j-1];
						}
						//set into i
						//key
						temp->key[i]=k;
						//key condition
						temp->key_condition[i]=true;
						//pointer
						temp->pointer[i-1]=NULL;
						//size
						temp->tot_size=temp->tot_size+1;
						
						//balancing the sibling pointer
						temp->pointer[2*n-2]=save;
						
						//insert into vector
						V.push_back(temp);
						//inserted
						inserted=true;//now need to consider about split
						continue;
					}
					else if(temp->type_of_node==2){
						//non leaf node
						//insert parent node into vector
						V.push_back(temp);
						//go to down and left
						if(temp->pointer[i-1] != NULL){
							temp=temp->pointer[i-1];
							continue;
						}
						else {
							//asserting because this segment should never reach
							int value = 0;
							assert(value != 1);
						}
					}
				}				
			}
	  }
}

//initialization of a node
void initialization(Key_Type s){
        current_node = new Node(); //memory space allocated
        current_node->tot_size=1; //only one element inserted
        current_node->type_of_node=1; //1 means its a leaf node,2 means a non leaf node
        current_node->key[1]=s;//key saved
        current_node->pointer[0]=NULL; //pointer [0] should be null
        memset(current_node->key_condition,false,sizeof(current_node->key_condition)); //all are false
        current_node->key_condition[1]=true; //inserted into 1, this is true
        root = current_node; // this is root now
        return;
}

void print_sorted_full(Node *node){
	if(node->type_of_node==2){
		//non leaf
		if(node->pointer[0] != NULL) {
			cout<<"parent nodes :  ";
			int base=1;
			for(int i=1;i<=node->tot_size;i++){
				cout<<node->key[base].s<<" ";
				base=base+2;
			}
			cout<<endl;
			print_sorted_full(node->pointer[0]);
		}
		else{
			int value = 0;
			assert(value != 1);
		}
	}
	else if(node->type_of_node==1){
		//just print
		while(true){
			if(node == NULL) break;
			cout<<"( ";
			for(int i=1;i<=2*(node->tot_size)-1;i=i+2){
				cout<<node->key[i].s<<" ";
			}
			cout<<" ) " << endl;
			//sibling
			node=node->pointer[2*n-2];
		}
	}
}

void menu(){
	cout<<"1 to enter string"<<endl;
	cout<<"2 to delete string"<<endl;
	cout<<"3 to see tree"<<endl;
}

int main(void){
	int cnt=0;
	while(true){
		menu();
		int choice;
		cin>>choice;
		if(choice==1){
			//insertion
			string s;
			cin>>s;
			cnt++;
			Key_Type obj;
			obj.s=s;
			if(cnt==1){
				//beginning
				initialization(obj);
				print_sorted_full(root);
			}
			else {
				insertion(obj);
				print_sorted_full(root);
			}
		}
		else if(choice==2){
			
		}
		else {
			//showing
			print_sorted_full(root);
		}
	}
}

