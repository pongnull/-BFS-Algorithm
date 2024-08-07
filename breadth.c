//AHMAD ZAINAL ABQARI BIN MOHD ABD AZIZ
//start state[4, 1, 3], [6, 2, 5], [7, 8, 0]
//goal state[1, 2, 3], [4, 5, 0], [6, 7, 8]
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N 3

// Structure to represent a puzzle state
typedef struct PuzzleState {
    int board[N][N];
    int zeroRow;
    int zeroCol;
} PuzzleState;

// Structure to represent a node in the search tree
typedef struct Node {
    PuzzleState state;
    struct Node* parent;
    struct Node* children[4]; // Up, Down, Left, Right
} Node;

// Queue structure for BFS
typedef struct QueueNode {
    Node* data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Function to initialize an empty queue
void initializeQueue(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

// Function to check if the queue is empty
bool isQueueEmpty(const Queue* queue) {
    return (queue->front == NULL);
}

// Function to enqueue a node in the queue
void enqueue(Queue* queue, Node* node) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = node;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Function to dequeue a node from the queue
Node* dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }

    QueueNode* frontNode = queue->front;
    Node* data = frontNode->data;

    queue->front = frontNode->next;
    free(frontNode);

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    return data;
}

// Function to print the puzzle board
void printPuzzle(const PuzzleState* puzzle) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (puzzle->board[i][j] == 0) {
                printf("   ");
            } else {
                printf("%2d ", puzzle->board[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Function to check if two puzzle states are equal
bool areStatesEqual(const PuzzleState* state1, const PuzzleState* state2) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (state1->board[i][j] != state2->board[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Function to check if a puzzle state is the goal state
bool isGoalState(const PuzzleState* state) {
    PuzzleState goalState = {
        .board = {{1, 2, 3}, {4, 5, 0}, {6, 7, 8}}
    };
    return areStatesEqual(state, &goalState);
}

// Function to perform a swap between two positions on the puzzle board
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to create a new puzzle state by moving the empty tile in a certain direction
PuzzleState move(const PuzzleState* current, int newZeroRow, int newZeroCol) {
    PuzzleState newState = *current;
    swap(&newState.board[current->zeroRow][current->zeroCol], &newState.board[newZeroRow][newZeroCol]);
    newState.zeroRow = newZeroRow;
    newState.zeroCol = newZeroCol;
    return newState;
}

// Function to print the solution path from the initial state to the goal state
void printSolutionPath(Node* goalNode) {
    if (goalNode == NULL) {
        printf("No solution found.\n");
        return;
    }

    Node* currentNode = goalNode;
    while (currentNode != NULL) {
        printPuzzle(&currentNode->state);
        currentNode = currentNode->parent;
    }
}

// Function to perform Breadth-First Search to solve the 8-puzzle problem
void bfs(const PuzzleState* startState) {
    Queue queue;
    initializeQueue(&queue);

    Node* startNode = (Node*)malloc(sizeof(Node));
    startNode->state = *startState;
    startNode->parent = NULL;
    for (int i = 0; i < 4; i++) {
        startNode->children[i] = NULL;
    }

    enqueue(&queue, startNode);

    while (!isQueueEmpty(&queue)) {
        Node* currentNode = dequeue(&queue);

        // Display the current state during BFS
        printf("Current State:\n");
        printPuzzle(&currentNode->state);

        if (isGoalState(&currentNode->state)) {
            // Goal state found, print the solution path
            printf("Goal State Found!\n");
            printSolutionPath(currentNode);
            return;
        }

        // Expand the current node
        for (int i = -1; i <= 1; i += 2) {
            // Try moving the empty tile up/down
            if (currentNode->state.zeroRow + i >= 0 && currentNode->state.zeroRow + i < N) {
                PuzzleState newState = move(&currentNode->state, currentNode->state.zeroRow + i, currentNode->state.zeroCol);
                if (!areStatesEqual(&newState, &currentNode->state)) {
                    Node* newNode = (Node*)malloc(sizeof(Node));
                    newNode->state = newState;
                    newNode->parent = currentNode;
                    for (int j = 0; j < 4; j++) {
                        newNode->children[j] = NULL;
                    }
                    currentNode->children[i == 1 ? 0 : 1] = newNode; // Up: children[0], Down: children[1]
                    enqueue(&queue, newNode);
                }
            }
            // Try moving the empty tile left/right
            if (currentNode->state.zeroCol + i >= 0 && currentNode->state.zeroCol + i < N) {
                PuzzleState newState = move(&currentNode->state, currentNode->state.zeroRow, currentNode->state.zeroCol + i);
                if (!areStatesEqual(&newState, &currentNode->state)) {
                    Node* newNode = (Node*)malloc(sizeof(Node));
                    newNode->state = newState;
                    newNode->parent = currentNode;
                    for (int j = 0; j < 4; j++) {
                        newNode->children[j] = NULL;
                    }
                    currentNode->children[i == 1 ? 2 : 3] = newNode; // Left: children[2], Right: children[3]
                    enqueue(&queue, newNode);
                }
            }
        }
    }
    // If the queue becomes empty and no solution is found
    printf("No solution found.\n");
}

// Function to display the tree recursively
void displayTree(Node* root, int depth) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    printPuzzle(&root->state);

    for (int i = 0; i < 4; i++) {
        displayTree(root->children[i], depth + 1);
    }
}

int main() {
    // Define the initial state
    PuzzleState startState = {
        .board = {{4, 1, 3}, {6, 2, 5}, {7, 8, 0}},
        .zeroRow = 2,
        .zeroCol = 2
    };

    printf("Initial State:\n");
    printPuzzle(&startState);

    // Call BFS with the initial state
    bfs(&startState);

    // Display the goal state
    PuzzleState goalState = {
        .board = {{1, 2, 3}, {4, 5, 0}, {6, 7, 8}},
        .zeroRow = 1,
        .zeroCol = 2
    };

    printf("Goal State:\n");
    printPuzzle(&goalState);

    return 0;
}
