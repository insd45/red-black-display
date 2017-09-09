/*Ian Smith 12442396 insd45*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum nodeColor {
    RED,
    BLACK
};

struct rbNode {
    int data, color;
    struct rbNode *link[2];
};

struct rbNode *root = NULL;                               //Taken from example
struct rbNode *createNode(int data);                      //Taken from example
void insertion(int data);                                 //Taken from example
void printRBTreeLevelOrder(struct rbNode *root);                            //Prints the tree in a level order (via queue)
struct rbNode** makeQueue(int *front, int *rear);                           //Creates the queue to be used
struct rbNode* deQueue(struct rbNode **queue, int *front);                  //Shortens the queue
void enQueue(struct rbNode **queue, int *rear, struct rbNode *new);         //Adds an element to the queue
void freeMemory(struct rbNode *root);                                       //Free memory


/*
int main()                  <----------- How strtok works. I'm using this chunk of functioning code for reference
{
   char str[80] = "1,8,11,2,5,6";
   const char s[2] = ",";
   char *token;
   
   // get the first token //
   token = strtok(str, s);
   int b = 0;
   // walk through other tokens //
   while( token != NULL ) 
   {
      b = atoi(token);
      
      printf( " %d\n", b );
    
      token = strtok(NULL, s);
   }
   
   return(0);
}
*/

int main()
{
    //Scan in ints like 1,8,11,2,...
    printf("Please enter a series of ints to enter into a Red-Black tree separated by a comma: eg. 1,8,11,2, ....\n");
    char input[256];                                    //This will be the user's input string 
    const char s[100] = ",";                            //The delimeter to tokenize my strings, so thst way I can get a string that's just a number
    char *token;                                        //This will be the tokens (the ints) and I will use atoi to turn this string into an int (tok)
    int tok = 0;                                        //this int will hold the int data for the tokenized strings
    scanf("%s", input);                                 //Scan in the user input formatted int, int, int, int, ....
    
    //get the first token
    token = strtok(input, s);
    
    //Walk through other tokens, inserting them to the rbNode as we go
    while(token != NULL)
    {
        //Make this string an int
        tok = atoi(token);
        
        //Insert the int
        insertion(tok);
        
        //Continue with the next int inputted by the user
        token = strtok(NULL, s);
    }
    
    //All nodes entered by the user have been inputted into the RBTree
    //Print the Tree in a fashion similar to a heap
    
    printRBTreeLevelOrder(root);
    printf("\n");
    
    freeMemory(root);
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
    return(0);
}

struct rbNode * createNode(int data) {
    struct rbNode *newnode;
    newnode = (struct rbNode *)malloc(sizeof(struct rbNode));
    newnode->data = data;
    newnode->color = RED;
    newnode->link[0] = newnode->link[1] = NULL; /* link[0] - Left Child; link[1] - Right Child */
    return newnode;
}

void insertion (int data) {
    struct rbNode *stack[98], *ptr, *newnode, *xPtr, *yPtr;
    int dir[98], ht = 0, index;
    ptr = root;

    /* First node in the Red-Black Tree */
    if (!root) {
            root = createNode(data);
            root->color = BLACK;    /* Set inserted root node to black */
            return;
    }
    stack[ht] = root;
    dir[ht++] = 0;
    /* find the place to insert the new node */
    while (ptr != NULL) {
            if (ptr->data == data) {
                    printf("Duplicates Not Allowed!!\n");
                    return;
            }
            index = (data - ptr->data) > 0 ? 1 : 0;
            stack[ht] = ptr;
            ptr = ptr->link[index];
            dir[ht++] = index;
    }
    /* insert the new node */
    stack[ht - 1]->link[index] = newnode = createNode(data);
    while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
            if (dir[ht - 2] == 0) {
                    yPtr = stack[ht - 2]->link[1];

                    if (yPtr != NULL && yPtr->color == RED) {
                            /* Case 1
                             * Red node having red child. B- black, R-red
                             *     B                R
                             *    / \             /   \
                             *   R   R  =>     B     B
                             *  /               /
                             * R               R
                             */
                            stack[ht - 2]->color = RED;
                            stack[ht - 1]->color = yPtr->color = BLACK;
                            ht = ht -2;
                    } else {
                            if (dir[ht - 1] == 0) {
                                    yPtr = stack[ht - 1];
                            } else {
                                    /* Case 2
                                     * XR - node X with red color
                                     * YR - node Y with red color
                                     * Red node having red child
                                     *(do single rotation left b/w X and Y)
                                     *         B             B
                                     *        /             /
                                     *      XR     =>      YR
                                     *        \           /
                                     *         YR        XR
                                     * one more additional processing will be
                                     * performed after this else part.  Since
                                     * we have red node (YR) with red child(XR)
                                     */
                                    xPtr = stack[ht - 1];
                                    yPtr = xPtr->link[1];
                                    xPtr->link[1] = yPtr->link[0];
                                    yPtr->link[0] = xPtr;
                                    stack[ht - 2]->link[0] = yPtr;
                            }
                            /* Case 3
                             *  Red node(YR) with red child (XR) - single
                             *  rotation b/w YR and XR for height balance. Still,
                             *  red node (YR) is having red child.  So, change the
                             *  color of Y to black and Black child B to Red R
                             *          B           YR          YB
                             *         /           /  \        /  \
                             *        YR  =>   XR   B  =>  XR  R
                             *       /
                             *      XR
                             */
                            xPtr = stack[ht - 2];
                            xPtr->color = RED;
                            yPtr->color = BLACK;
                            xPtr->link[0] = yPtr->link[1];
                            yPtr->link[1] = xPtr;
                            if (xPtr == root) {
                                    root = yPtr;
                            } else {
                                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                            }
                            break;
                    }
            } else {
                    yPtr = stack[ht - 2]->link[0];
                    if ((yPtr != NULL) && (yPtr->color == RED)) {
                            /* Case 1:
                             * Red node with red child
                             *        B             R
                             *      /   \         /   \
                             *     R     R =>  B     B
                             *             \              \
                             *              R              R
                             *
                             */
                            stack[ht - 2]->color = RED;
                            stack[ht - 1]->color = yPtr->color = BLACK;
                            ht = ht - 2;
                    } else {
                            if (dir[ht - 1] == 1) {
                                    yPtr = stack[ht - 1];
                            } else {
                                    /* Case 2:
                                     * Red node(XR) with red child(YR)
                                     *   B          B
                                     *    \          \
                                     *     XR  => YR
                                     *    /            \
                                     *   YR             XR
                                     * Single rotation b/w XR(node x with red color) & YR
                                     */
                                    xPtr = stack[ht - 1];
                                    yPtr = xPtr->link[0];
                                    xPtr->link[0] = yPtr->link[1];
                                    yPtr->link[1] = xPtr;
                                    stack[ht - 2]->link[1] = yPtr;
                            }
                            /* Case 3:
                             *   B              YR          YB
                             *    \             /  \        /  \
                             *     YR  =>   B   XR => R    XR   R
                             *      \
                             *       XR
                             * Single rotation b/w YR and XR and change the color to
                             * satisfy rebalance property.
                             */
                            xPtr = stack[ht - 2];
                            yPtr->color = BLACK;
                            xPtr->color = RED;
                            xPtr->link[1] = yPtr->link[0];
                            yPtr->link[0] = xPtr;
                            if (xPtr == root) {
                                    root = yPtr;
                            } else {
                                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                            }
                            break;
                    }
            }
    }
    root->color = BLACK;
}

void printRBTreeLevelOrder(struct rbNode *root)
{
    int rear;  //Add something to end of queue
    int front;  //Front of queue
    struct rbNode **queue = makeQueue(&front, &rear);   //Double pointer so that it can be edited in void functions
    struct rbNode *temp_node = root;                    //temp_node starts with root ----> will traverse the tree without changing the original root position
    
    while(temp_node)                                    //While not at the end of the queue...
    {
        if(temp_node->color == 0) //node is red         //If red...
        {
            printf("%d-R ", temp_node->data);
        }
        else  //node is black                           //Or black...
        {
            printf("%d-B ", temp_node->data);
        }
        
        //Enqueue the left child
        if(temp_node->link[0])                          //When its children are added to the queue, they will be removed in level order
        {
            enQueue(queue, &rear, temp_node->link[0]);
        }
        //Enqueue right child
        if(temp_node->link[1])
        {
            enQueue(queue, &rear, temp_node->link[1]);
        }
        
        temp_node = deQueue(queue, &front);              //Remove the element we printed above from the queue, continue with the next nodes 
    }
    free(queue);
    free(temp_node);
}

struct rbNode** makeQueue(int *front, int *rear)
{
    struct rbNode **queue = malloc(sizeof(struct rbNode*) * 500);
    
    *front = 0;  //Clean slate, make front and rear 0
    *rear = 0;   //This will allow for the editing of the queue in the enQueue function
    return queue;
}

void enQueue(struct rbNode **queue, int *rear, struct rbNode *new)
{
    queue[*rear] = new;         //Add to end of queue
    (*rear)++;                  //Make the new node the new end
}

struct rbNode* deQueue(struct rbNode **queue, int *front)
{
    (*front)++;                 //Move the front up (changes the position of queue without deleting elements)
    return queue[*front - 1];   //Return the queue with the new position
}

void freeMemory(struct rbNode *root)
{
    if(!root)
    {
        return;
    }
    
    freeMemory(root->link[0]);
    freeMemory(root->link[1]);
    
    //Works all the way to the last one, frees from bottom up
    free(root);
}