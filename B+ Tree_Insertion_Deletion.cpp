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
        int type_of_node; // to identify this is a leaf node or not [leaf node = 1, non leaf node = 2]
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
				//debug: cout<<"condition 1"<<endl;
				//if value is already inserted, need to check the conditions of parent nodes
				//first check if this node is overflowed or not
				temp=V[V.size()-1]; //the last node
				V.pop_back();//because dealing with last node is complete 
				bool overflow = node_fill_checking(temp);
				if(overflow) {
					//overflowed need to split
					if(temp->type_of_node == 1) {
						//cout:debug <<"spliting for leaf node " << endl;
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
						//cout<<"creating Link to sibling " << endl;
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
						int left_ptr_number = ceil((double)(n+1)/2.0);
						//creation of new node
						Node *notun = new Node();
						//type of node
						notun->type_of_node=2;//its a non leaf node
						notun->tot_size=0; //init
						int st_index=2*(left_ptr_number+1)-2; //here from pointer will start to be copied
						//copy pointer
						for(int j=0,cnt=left_ptr_number+1;cnt<=(n+1);cnt++){
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
				//cout<<"condition 2"<<endl;
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
					/*cout<<"NODE VAL " << endl;
					for(int cnt=1,p=1;cnt<=temp->tot_size;cnt++,p=p+2){
						cout<<temp->key[p].s<<" ";
					}
					cout<<endl;*/
					
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
			cout<<" ) " ;
			//sibling
			node=node->pointer[2*n-2];
		}
		cout<<endl;
	}
}

vector<Node*>saved_state[50];
int max_level = -1;
void print_function(Node *node,int level){
	//debug: printf("level %d\n",level);
	max_level = max(max_level,level);
	if(node == NULL) return;
	if(node->type_of_node == 1) {
		saved_state[level].push_back(node);
		for(int i=1;i<=2*(node->tot_size)-1;i=i+2) {
			cout<<node->key[i].s<<" ";
		}
		cout<<endl;
		if(node->pointer[2*n-2] == NULL) {
			//all nodes been traversed 
			/*for(int i=0;i<=level;i++){
				cout<<"level "<<i<<" : ";
				for(int j=0;j<(int)saved_state[i].size();j++){
					Node *temp = saved_state[i][j];
					cout<<" ( ";
					for(int k=1;k<=2*(temp->tot_size)-1;k=k+2){
						cout <<temp->key[k].s<<" ";
					}
					cout<<" ) ";
				}
				cout<<endl;
			}*/
		}
		return;
	}
	if(node->type_of_node == 2) {
		//non leaf node
		saved_state[level].push_back(node);
		for(int i=1;i<=(node->tot_size+1);i++){
			int idx=2*i-2;
			if(node->pointer[idx] != NULL) {
				print_function(node->pointer[idx],level+1);
			}
		}
		return;
	}
}

bool search(Node *node,Key_Type obj){
	bool go_right=false;
	
	if(node->type_of_node == 1) {
		//leaf node
		for(int i=1;i<=2*(node->tot_size);i=i+2){
			if(node->key[i].s<obj.s){
				go_right=true;
				continue;
			}
			else if(node->key[i].s == obj.s){
				return true;
			}
		}
		return false;
	}
	else {
		int i=1;
		for(i=1;i<=2*(node->tot_size);i=i+2){
			if(node->key[i].s<=obj.s) {
				go_right=true;
				continue;
			}
			else {
				go_right=false;
				break;
			}
		}
		if(node->pointer[i-1] != NULL) {
			return search(node->pointer[i-1],obj);
		}
		else{
			int value=0;
			assert(value != 1);
			return false;
		}
	}
}

/***B+ Tree Deletion**/
struct DeletionTree{
	struct Node *called_node; //the node with which we are working
	struct Node *parent_node; //parent node of this called_node
	int index; //the index through which we will move
	struct Node *right_sibling;
	struct Node *left_sibling; 
};

//function: save path,delete leaf and return vector
vector<DeletionTree*> deletion_part1(struct Node *node, struct Key_Type key){
	struct Node *temp = node; //base node to work with
	vector<DeletionTree*>V; //vector to return
	while(true){
		if(temp->type_of_node == 1) {
			//leaf node
			int i=0;
			for(i=1;i<=2*(temp->tot_size)-1;i=i+2){
				if(temp->key[i].s == key.s){
					break;
				}
			}
			//i is to delete
			struct Node *save = temp->pointer[2*n-2]; //sibling pointer
			//key and pointer set --basically shifted
			for(int j=i+2;j<=2*(temp->tot_size)-1;j=j+2){
				//key set
				temp->key[j-2]=temp->key[j]; 
				//pointer
				temp->pointer[j-3]=temp->pointer[j-1];
			}
			//making the last index(key) free
			temp->key[2*(temp->tot_size)-1].s = ""; //no value exists
			temp->key_condition[2*(temp->tot_size)-1]=false; //key condition set
			temp->pointer[2*(temp->tot_size)-2]=NULL;//pointer set /******doubt case *******/
			temp->tot_size--; //tot_size set
			temp->pointer[2*n-2] = save; //sibling maintained
			break;
		}
		else{
			//non leaf node
			//--- searching for the position to go down
			int i=0;
			for(i=1;i<=2*(temp->tot_size)-1;i=i+2){
				if(temp->key[i].s <= key.s) continue;
				else{
					break;
				}
			}
			//i says the position
			DeletionTree *variable = new DeletionTree();
			//called node
			variable->called_node = temp->pointer[i-1]; //the node through which we will go
			if(temp->pointer[i-1] == NULL) {
				//this shouldn't happen
				int value=0;
				assert(value != 1);
			}
			//parent node
			variable->parent_node = temp; 
			//index
			variable->index = i; // (i) is value (i-1) pointer through which we will move
			if((i-3)>=0 && temp->pointer[i-3] != NULL) {
				//left sibling exists
				variable->left_sibling = temp->pointer[i-3];
			}
			else{
				//left sibling doesn't exist
				variable->left_sibling = NULL; 
			}
			if((i+1)<=(2*(temp->tot_size+1)-1) && temp->pointer[i+1] != NULL) {
				//right sibling does exist
				variable->right_sibling = temp->pointer[i+1];
			}
			else{
				//right sibling doesn't exist
				variable->right_sibling = NULL; 
			}
			V.push_back(variable);
			temp=temp->pointer[i-1];
			continue;
			
		}
	}
	return V;
}


void print_vector(vector<DeletionTree*>V){
	for(int i=0;i<(int)V.size();i++){
		cout<<V[i]->parent_node->type_of_node<<" "<<V[i]->called_node->type_of_node<<" "<<V[i]->index<<endl;
		if(V[i]->called_node->type_of_node == 1) {
			printf("tot size = %d\n",V[i]->called_node->tot_size);
			for(int j=1;j<=2*(V[i]->called_node->tot_size)-1;j=j+2){
				cout<<V[i]->called_node->key[j].s<<" ";
			}
			cout<<endl;
		}
	}
}

//function to underflow checking in leaf node
bool underflow_leaf_node(struct Node *node){
	int sz = node->tot_size;
	if(sz<ceil((n-1)/2.0)) return true; //underflow node
	return false; //not underflowed node
}

//function to underflow checking in non leaf node
bool underflow_non_leaf_node(struct Node *node){
	int sz = node->tot_size;
	int val = ceil((n+1)/2.0)-1;
	if(sz<val) return true; //underflow
	return false;//not underflow
}


//case 1:
void leaf_node_not_underflows(DeletionTree *last){
	int i=last->index;
	if((i-2)>=1){
		last->parent_node->key[i-2]=last->called_node->key[1]; //because just immediate cell in parent should be equal to start of child
	}
	return;
}

//case 2: merge in leaf node with right [ called with be merged by taking elemets from right sibling]
void leaf_merge_with_right(DeletionTree *last){
	int start_for_called = 2*(last->called_node->tot_size)-1+2; //from this position copy will start
	cout<<"ASEEEE "<<last->index<<endl;
	//key pointer 
	for(int i=start_for_called,j=1;j<=2*(last->right_sibling->tot_size)-1;j=j+2,i=i+2){
		//key copy
		last->called_node->key[i]=last->right_sibling->key[j];
		cout<<last->called_node->key[i].s<<endl;
		//key condition
		last->called_node->key_condition[i]=true;
		//pointer copy
		last->called_node->pointer[i-1]=last->right_sibling->pointer[j-1];
	}
	
	
	//tot_size update
	last->called_node->tot_size = last->called_node->tot_size + last->right_sibling->tot_size;
	//last pointer correction
	last->called_node->pointer[2*(last->called_node->tot_size+1)-2]=last->right_sibling->pointer[2*(last->right_sibling->tot_size+1)-2];
	//leaf sibling pointer correction
	if(last->right_sibling->pointer[2*n-2] != NULL) {
		last->called_node->pointer[2*n-2] = last->right_sibling->pointer[2*n-2] ;
	}
	else{
		last->called_node->pointer[2*n-2]=NULL; 
	}
	//modification in parent 
	int index = last->index;
	cout<<"index = " << index<<" "<<last->parent_node->tot_size<<endl;
	for(int i=index+2;i<=2*(last->parent_node->tot_size)-1;i=i+2){
		//key set
		last->parent_node->key[i-2]=last->parent_node->key[i];
		cout<<last->parent_node->key[i-2].s<<endl;
		//key condition set
		last->parent_node->key_condition[i-2]=true;
		//pointer set
		last->parent_node->pointer[i-1] = last->parent_node->pointer[i+1]; 
		if(last->parent_node->pointer[i-1] != NULL) {
				cout<<"YES Valid"<<endl;
		} 
	}
	//size decrease in parent
	last->parent_node->tot_size--;
	//sanity saving
	//make pointer null
	for(int i=2*(last->parent_node->tot_size)-1+1+2;i<MAX;i=i+2){
		//they should be NULL
		last->parent_node->pointer[i]=NULL;
	}
	//make key empty
	for(int i=2*(last->parent_node->tot_size)-1+2;i<MAX;i=i+2){
		last->parent_node->key[i].s="";
		last->parent_node->key_condition[i]=false;
	}
	
	return;
}
//case 2: merge with left node left [ called with be merged with left sibling]
void leaf_merge_with_left(DeletionTree *last){
	int start_index_left_sibling = 2*(last->left_sibling->tot_size)-1+2;
	for(int i=start_index_left_sibling,j=1;j<=2*(last->called_node->tot_size)-1;j=j+2,i=i+2){
		//key
		last->left_sibling->key[i]=last->called_node->key[j];
		//key condition
		last->left_sibling->key_condition[i]=true;
		//pointer set
		last->left_sibling->pointer[i-1]=last->called_node->pointer[j-1];
	}
	//tot_size
	last->left_sibling->tot_size = last->left_sibling->tot_size + last->called_node->tot_size;
	//last pointer update
	last->left_sibling->pointer[2*(last->left_sibling->tot_size)-1+1] = last->called_node->pointer[2*(last->called_node->tot_size)-1+1];
	//leaf sibling pointer correction
	if(last->called_node->pointer[2*n-2] != NULL) {
		last->left_sibling->pointer[2*n-2]=last->called_node->pointer[2*n-2];
	}
	else{
		last->left_sibling->pointer[2*n-2]=NULL;
	}
	//modification in parent
	int index = last->index;
	for(int i=index;i<=2*(last->parent_node->tot_size)-1;i=i+2){
		//key set
		last->parent_node->key[i-2]=last->parent_node->key[i];
		//key condition set
		last->parent_node->key_condition[i-2]=true;
		//pointer set
		last->parent_node->pointer[i-1] = last->parent_node->pointer[i+1];  
	}
	//size decrease in parent
	last->parent_node->tot_size--;
	//sanity checking
	//pointer
	for(int i=2*(last->parent_node->tot_size)-1+1+2;i<MAX;i=i+2){
		last->parent_node->pointer[i]=NULL;
	}
	//key
	for(int i=2*(last->parent_node->tot_size)-1+2;i<MAX;i=i+2){
		last->parent_node->key[i].s="";
		last->parent_node->key_condition[i]=false;
	}
	return;
}

//case 3: redistribution into leaf: take from left
void redistribution_leaf_left(DeletionTree *last){
	int dorkar = ceil((n-1)/2.0) - last->called_node->tot_size;
	//shift dorkar times in right for called_node
	for(int i=2*(last->called_node->tot_size)-1 ; i>=1; i=i-2){
		//key shift
		last->called_node->key[i+2*dorkar]=last->called_node->key[i]; /******************change 1 **************************/
		//key condition
		last->called_node->key_condition[i+2*dorkar]=true;
		//pointer
		last->called_node->pointer[i+2*dorkar-1]=last->called_node->pointer[i-1];
	}
	//move from left_sibling
	//dorkar times element will move
	for(int i=2*(last->left_sibling->tot_size)-1,j=dorkar,k=2*dorkar-1;j>0;i=i-2,j--,k=k-2){
		//move
		//key
		last->called_node->key[k]=last->left_sibling->key[i];
		last->left_sibling->key[i].s="";
		//key condition
		last->called_node->key_condition[k]=true;
		last->left_sibling->key_condition[i]=false;
		//pointer set
		last->called_node->pointer[k-1]=last->left_sibling->pointer[i-1];
		last->left_sibling->pointer[i-1] = NULL; 
	}
	//tot_size
	last->left_sibling->tot_size -= dorkar;
	last->called_node->tot_size += dorkar; 
	//update in parent
	int index = last->index;
	if((index-2)>=1){
		last->parent_node->key[index-2]=last->called_node->key[1];
	}
	return;
}

//case 3: Redistribution into right: take from right
void redistribution_leaf_right(DeletionTree *last){
	int dorkar  = ceil((n-1)/2.0) - last->called_node->tot_size;
	//copy from right
	for(int i=2*(last->called_node->tot_size)-1+2,j=1,k=1;j<=dorkar;i=i+2,j=j+1,k=k+2){
		//key
		last->called_node->key[i]=last->right_sibling->key[k];
		//key condition
		last->called_node->key_condition[i]=true;
		//pointer
		last->called_node->pointer[i-1]=last->right_sibling->pointer[k-1];
	}
	//shift into right
	for(int i=2*(dorkar+1)-1,j=1;i<=2*(last->right_sibling->tot_size)-1;j=j+2,i=i+2){
			//key shifting
			last->right_sibling->key[j] = last->right_sibling->key[i];
			last->right_sibling->key[i].s="";
			//key condition shifting
			last->right_sibling->key_condition[j]=true;
			last->right_sibling->key_condition[i]=false;
			//pointer shifting
			last->right_sibling->pointer[j-1] = last->right_sibling->pointer[i-1];
			last->right_sibling->pointer[i-1] = NULL; 
	}
	//tot size
	last->called_node->tot_size += dorkar;
	last->right_sibling->tot_size -= dorkar;
	//modification into parent
	int index = last->index;
	last->parent_node->key[index]=last->right_sibling->key[1];
	return;	
}

//case 4: merge with right (taking everything from right)
void non_leaf_merge_with_right(DeletionTree *last){
	//bring from root, add with called node
	int index=last->index;
	last->called_node->key[2*(last->called_node->tot_size)-1+2]=last->parent_node->key[index]; 
	last->called_node->key_condition[2*(last->called_node->tot_size)-1+2]=true;
	last->called_node->tot_size++;
	//shifting from right
	for(int i=2*(last->called_node->tot_size)-1+2,j=1;j<=2*(last->right_sibling->tot_size)-1;j=j+2,i=i+2){
		//key
		last->called_node->key[i] = last->right_sibling->key[j];
		last->right_sibling->key[j].s="";
		//key condition
		last->called_node->key_condition[i]=true;
		last->right_sibling->key_condition[j]=false;
	}
	//pointer shift
	int ptr=last->right_sibling->tot_size+1;
	for(int i=2*(last->called_node->tot_size)-1+1,j=0;j<=2*(ptr)-2;j=j+2,i=i+2){
		last->called_node->pointer[i]=last->right_sibling->pointer[j];
		last->right_sibling->pointer[j]=NULL;
	}
	//tot_size
	last->called_node->tot_size += last->right_sibling->tot_size;
	//called_node complete
	//modification in parent
	for(int i=index+2;i<=2*(last->parent_node->tot_size)-1;i=i+2) {
		//key
		last->parent_node->key[i-2]=last->parent_node->key[i];
		last->parent_node->key[i].s = "";
		//key condition
		last->parent_node->key_condition[i-2]=true;
		last->parent_node->key_condition[i]=false;
		//pointer
		last->parent_node->pointer[i-1]=last->parent_node->pointer[i+1];
		last->parent_node->pointer[i+1]=NULL;
	}
	//tot_size
	last->parent_node->tot_size--;
	//sanity testing
	//pointer 
	for(int i=2*(last->parent_node->tot_size)-1+1+2;i<MAX;i=i+2){
		last->parent_node->pointer[i]=NULL;
	}
	//key
	for(int i=2*(last->parent_node->tot_size)-1+2;i<MAX;i=i+2){
		last->parent_node->key[i].s="";
		last->parent_node->key_condition[i]=false;
	}
	return;
}

//case 4: non leaf merge with left
void non_leaf_merge_with_left(DeletionTree *last){
	//bring from root and give to left
	int index = last->index;
	last->left_sibling->key[2*(last->left_sibling->tot_size)-1+2]=last->parent_node->key[index-2];
	last->left_sibling->key_condition[2*(last->left_sibling->tot_size)-1+2]=true;
	last->left_sibling->tot_size++;
	//bring from called_node 
	for(int i=2*(last->left_sibling->tot_size)-1+2,j=1;j<=2*(last->called_node->tot_size)-1;j=j+2,i=i+2){
		//key 
		last->left_sibling->key[i] = last->called_node->key[j];
		last->called_node->key[j].s="";
		//key_condition
		last->left_sibling->key_condition[i]=true;
		last->called_node->key_condition[j]=false;
	}
	//pointer shift
	int ptr = last->called_node->tot_size+1;
	for(int i=2*(last->left_sibling->tot_size)-1+1,j=0;j<=2*(ptr)-2;j=j+2,i=i+2){
		last->left_sibling->pointer[i]=last->called_node->pointer[j];
		last->called_node->pointer[j]=NULL; 
	}
	//tot size
	last->left_sibling->tot_size += last->called_node->tot_size;
	//modification in parent
	for(int i=index;i<=2*(last->parent_node->tot_size)-1;i=i+2){
		//key
		last->parent_node->key[i-2]=last->parent_node->key[i];
		last->parent_node->key[i].s="";
		//key condition
		last->parent_node->key_condition[i-2]=true;
		last->parent_node->key_condition[i]=false;
		//pointer
		last->parent_node->pointer[i-1]=last->parent_node->pointer[i+1];
		last->parent_node->pointer[i+1]=NULL;
	}
	//size
	last->parent_node->tot_size--;
	//sanity testing
	//pointer
	for(int i=2*(last->parent_node->tot_size)-1+1+2;i<MAX;i=i+2){
		last->parent_node->pointer[i]=NULL;
	}
	//key
	for(int i=2*(last->parent_node->tot_size)-1+2;i<MAX;i=i+2){
		last->parent_node->key[i].s="";
		last->parent_node->key_condition[i]=false;
	}
	return;
}

//case 5: redistribution bring from right in non leaf
void redistribution_non_leaf_right(DeletionTree *last){
	int index = last->index;
	//key setting
	last->called_node->key[2*(last->called_node->tot_size)-1+2]=last->parent_node->key[index];
	last->called_node->key_condition[2*(last->called_node->tot_size)-1+2]=true;
	int dorkar = ceil((n+1)/2.0) - (last->called_node->tot_size)-1; 
	//1 time already taken, remaining times need to be taken 
	//key
	for(int i=2*(last->called_node->tot_size)-1+4,j=1;j<=2*(dorkar-1)-1;j=j+2,i=i+2){
		last->called_node->key[i]=last->right_sibling->key[j];
		last->called_node->key_condition[i]=true;
	}
	//pointer set
	for(int i=2*(last->called_node->tot_size)-1+4-1,j=0;j<=2*(dorkar)-2;j=j+2,i=i+2){
		last->called_node->pointer[i]=last->right_sibling->pointer[j];
	}
	last->called_node->tot_size += dorkar; 
	//parent modification
	last->parent_node->key[index] = last->right_sibling->key[2*dorkar-1];
	//right sibling modification
	for(int i=1,j=2*(dorkar+1)-1;j<=2*(last->right_sibling->tot_size)-1;j=j+2,i=i+2){
		//key set
		last->right_sibling->key[i]=last->right_sibling->key[j];
		last->right_sibling->key[j].s="";
		//key condition 
		last->right_sibling->key_condition[i]=true;
		last->right_sibling->key_condition[j]=false;
	}
	for(int i=0,j=2*(dorkar+1)-1-1;j<=2*(last->right_sibling->tot_size+1)-2;j=j+2,i=i+2){
		//pointer set
		last->right_sibling->pointer[i]=last->right_sibling->pointer[j];
		last->right_sibling->pointer[j]=NULL;
	}
	last->right_sibling->tot_size -= dorkar; 
	return;
	
}

//case 5: redistribution bring from left
void redistribution_non_leaf_left(DeletionTree *last){
	int dorkar = ceil((n)/2.0) - (last->called_node->tot_size+1);
	//shifting of pointer
	for(int i=2*(last->called_node->tot_size+1)-2,cnt=1;i>=0;i=i-2,cnt++){
		//pointer
		last->called_node->pointer[i+dorkar*2]=last->called_node->pointer[i];
		last->called_node->pointer[i]=NULL;
	}
	//shifting of key
	for(int i=2*(last->called_node->tot_size)-1,cnt=1;i>=1;cnt++,i=i-2){
		//key
		last->called_node->key[i+dorkar*2]=last->called_node->key[i];
		last->called_node->key[i].s="";
		//key_condition
		last->called_node->key_condition[i+dorkar*2]=true;
		last->called_node->key_condition[i]=false;
	}
    //bring down parent
    int index = last->index; 
    last->called_node->key[1+dorkar*2-2] = last->parent_node->key[index-2];
    last->called_node->key_condition[1+dorkar*2-2]=true;
    //pointer move
    for(int i=1+dorkar*2-2-1,j=2*(last->left_sibling->tot_size+1)-2,cnt=1;cnt<=dorkar;cnt++,i=i-2,j=j-2){
		last->called_node->pointer[i]=last->left_sibling->pointer[j];
		last->left_sibling->pointer[j]=NULL;
	}
	//key move
	for(int i=1+dorkar*2-4,j=2*(last->left_sibling->tot_size)-1;i>=1;i=i-2,j=j-2){
		last->called_node->key[i]=last->left_sibling->key[j];
		last->left_sibling->key[j].s="";
		
		last->called_node->key_condition[i]=true;
		last->called_node->key_condition[j]=false;
	}
	//tot_size
	last->called_node->tot_size += dorkar;
	//parent update
	last->parent_node->key[last->index-2]=last->left_sibling->key[2*last->left_sibling->tot_size-1- 2*(dorkar-1)];
	last->left_sibling->key[2*last->left_sibling->tot_size-1- 2*(dorkar-1)].s="";
	last->left_sibling->key_condition[2*last->left_sibling->tot_size-1- 2*(dorkar-1)]=false;
	//tot size 
    last->left_sibling->tot_size -= dorkar;
	return;
}

//function to print node status
void print_node_status(struct Node *node){
	printf("type of node = %d\n",node->type_of_node);
	printf("tot size = %d\n",node->tot_size);
	for(int i=1;i<=2*(node->tot_size)-1;i=i+2){
		cout<<node->key[i].s<<" ";
	}
	cout<<endl;
	return;
}

//function to delete
void deletion(struct Node *node,struct Key_Type key){
	//bool search
	bool ok = search(root,key);
	if(!ok) return; //key can't be deleted
	//part 1: save path,delete leaf,return vector
	vector<DeletionTree*>V = deletion_part1(root,key);
	print_vector(V);
	//part 2: modification of the nodes
	while(V.size()>0){
		DeletionTree *last = V[V.size()-1];
		if(last->called_node->type_of_node == 1) {
			//leaf node
			//underflow checking
			bool ok=underflow_leaf_node(last->called_node); 
			if(ok) {
				//underflows --need to sort out
				if(last->right_sibling != NULL && (last->called_node->tot_size + last->right_sibling->tot_size)<=(n-1)){
					//merging with right sibling possible
					leaf_merge_with_right(last);
					V.pop_back();//working with the parent which just modified
					continue;
				}
				else if(last->left_sibling != NULL && (last->called_node->tot_size + last->left_sibling->tot_size) <= (n-1)){
					//merging with left possible
					leaf_merge_with_left(last);
					V.pop_back();
					continue;
				}
				else {
					//redistribution section
					int dorkar = ceil((n-1)/2.0) - last->called_node->tot_size; 
					if(last->left_sibling != NULL && (last->left_sibling->tot_size-dorkar) >= ceil((n-1)/2.0)){
						//even after redistribution enough key remains in left sibling
						redistribution_leaf_left(last);
						break; //redistribution never creates imbalance
					}
					else if(last->right_sibling != NULL && (last->right_sibling->tot_size-dorkar) >= ceil((n-1)/2.0)){
						//even after redistribution enough key remains in right sibling
						redistribution_leaf_right(last);
						break; //redistribution never creates imbalance
					}
					else{
						cout<<"shit no operation can occure in leaf"<<endl;
						break;/********change 2********/
					}
				}
			}
			else{
				//not underflowed
				//just change value in parent
				leaf_node_not_underflows(last);
				break;
			}
		}
		else {
			//non leaf node
			bool ok = underflow_non_leaf_node(last->called_node);
			if(ok) {
				//underflow in non leaf node
				//special case root will be handled outside loop
				//merge case
				if(last->right_sibling != NULL && (last->called_node->tot_size+1+last->right_sibling->tot_size+1)<=n){
					//meging taking from right
					non_leaf_merge_with_right(last);
					V.pop_back();
					continue;
				}
				else if(last->left_sibling != NULL && (last->called_node->tot_size+1+last->left_sibling->tot_size+1)<=n){
					
					//merging throwing to left
					non_leaf_merge_with_left(last);
					V.pop_back();
					continue;
				}
				else {
					if(last->right_sibling != NULL){
						int dorkar= ceil((n+1)/2.0) - (last->called_node->tot_size+1);
						if(dorkar == 0) break;
						if(((last->right_sibling->tot_size+1)-dorkar) >= ceil((n/2.0))) {
							//will take from right
							redistribution_non_leaf_right(last);
							break;
						}
					}
					else if(last->left_sibling != NULL){
						int dorkar = ceil(n/2.0) - (last->called_node->tot_size+1);
						if(dorkar == 0) break;
						if(((last->left_sibling->tot_size+1)-dorkar) >= ceil((n+1)/2.0)) {
							cout<<"DHUKE JAI"<<endl;
							redistribution_non_leaf_left(last);
							break;
						}
					}
					else {
						printf("shit nothing can be done in non leaf\n");
						break;/**********cahnge 2******************/
					}
				}
				
				
			}
			else{
				//no underflow
				//everything's okay
				break;
			}
		}
	}
	if(root->tot_size == 0){
		//root must be deleted
		int cnt=0;
		struct Node* save;
		for(int i=0;i<MAX;i=i+2){
			if(root->pointer[i] != NULL) {
				save = root->pointer[i];
				cnt++;
			}
		}
		if(cnt==1){
			root=save;
		}
		else{
			//things are problematic
			int value=0;
			assert(value != 1);
		}
	}
	return;
}



void menu(){
	cout<<"1 to enter string"<<endl;
	cout<<"2 to delete string"<<endl;
	cout<<"3 to see tree"<<endl;
	cout<<"4 to search"<<endl;
	cout<<"5 to exit"<<endl;
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
				//print_sorted_full(root);
			}
			else {
				insertion(obj);
				//print_sorted_full(root);
			}
		}
		else if(choice==2){
			cout<<"Give input to delete: ";
			string s;
			cin>>s;
			Key_Type key;
			key.s = s;
			deletion(root,key);
		}
		else if(choice == 3){
			//showing
			//print_sorted_full(root);
			for(int i=0;i<30;i++){
				saved_state[i].clear();
			}
			max_level = -1;
			print_function(root,0);
			for(int i=0;i<=max_level;i++){
				cout<<"level "<<i<<" : ";
				for(int j=0;j<(int)saved_state[i].size();j++){
					Node *temp = saved_state[i][j];
					cout<<" ( ";
					for(int k=1;k<=2*(temp->tot_size)-1;k=k+2){
						cout <<temp->key[k].s<<" ";
					}
					cout<<" ) ";
				}
				cout<<endl;
			}
		}
		else if(choice == 4) {
			//search
			string check_val;
			printf("Give check value: ");
			cin>>check_val;
			Key_Type obj;
			obj.s = check_val;
			bool ok = search(root,obj);
			if(ok) {
				cout<<"Found\n"<<endl;
			}
			else{
				cout<<"Not Found\n"<<endl;
			}
		}
		else if(choice == 5) {
			return 0;
		}
	}
}

