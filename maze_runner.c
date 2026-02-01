#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// --- Configuration Constants ---
#define WIDTH 41
#define HEIGHT 21
#define MAX_QUEUE_SIZE (WIDTH * HEIGHT)

// Define maze cell states
#define WALL 0
#define PATH 1
#define START 2
#define EXIT 3
#define VISITED 4
#define SOLUTION 5

// --- Data Structure Definitions ---

// Structure to hold coordinates (used for both DFS and BFS)
typedef struct {
    int x;
    int y;
} Coordinate;

// Queue structure for BFS
typedef struct {
    Coordinate items[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// --- Utility Functions ---

// Initializes the queue
void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
}

// Checks if the queue is empty
bool isQueueEmpty(Queue *q) {
    return q->rear < q->front;
}

// Enqueues a coordinate
void enqueue(Queue *q, Coordinate c) {
    if (q->rear < MAX_QUEUE_SIZE - 1) {
        q->rear++;
        q->items[q->rear] = c;
    }
}

// Dequeues a coordinate
Coordinate dequeue(Queue *q) {
    if (isQueueEmpty(q)) {
        // Should not happen in a correctly implemented BFS
        Coordinate empty = {-1, -1};
        return empty;
    }
    Coordinate c = q->items[q->front];
    q->front++;
    return c;
}

// --- Maze Generation (Randomized DFS) ---

// Recursive DFS function for maze generation
void generateMaze(int maze[HEIGHT][WIDTH], int x, int y) {
    // 1. Mark the current cell as a path
    maze[y][x] = PATH;

    // 2. Define possible directions (x, y offsets)
    int dx[] = {2, -2, 0, 0};
    int dy[] = {0, 0, 2, -2};

    // 3. Create a random order of directions
    int order[] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = order[i];
        order[i] = order[r];
        order[r] = temp;
    }

    // 4. Try moving in the random order
    for (int i = 0; i < 4; i++) {
        int dir = order[i];
        int nx = x + dx[dir]; // New cell x
        int ny = y + dy[dir]; // New cell y
        int wx = x + dx[dir] / 2; // Wall x between current and new
        int wy = y + dy[dir] / 2; // Wall y between current and new

        // Check boundary conditions and if the new cell is still a wall
        if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && maze[ny][nx] == WALL) {
            // Break the wall
            maze[wy][wx] = PATH;
            // Recursively call DFS on the new cell
            generateMaze(maze, nx, ny);
        }
    }
}

// --- Maze Solving (BFS) ---

/**
 * Solves the maze using Breadth-First Search (BFS) to find the shortest path.
 * The path is marked in the maze grid using the SOLUTION constant.
 */
bool solveMaze(int maze[HEIGHT][WIDTH], Coordinate start, Coordinate end) {
    Queue q;
    initQueue(&q);
    
    // 2D array to store the parent/predecessor of each cell for path reconstruction
    Coordinate parent[HEIGHT][WIDTH];
    // 2D array to keep track of visited cells during BFS
    bool visited[HEIGHT][WIDTH] = {false};

    // Start BFS
    enqueue(&q, start);
    visited[start.y][start.x] = true;
    
    // Initialize parent for start to be itself, or a special marker
    Coordinate no_parent = {-1, -1};
    for(int i=0; i<HEIGHT; i++) {
        for(int j=0; j<WIDTH; j++) {
            parent[i][j] = no_parent;
        }
    }
    
    // Movement directions (up, down, left, right)
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    bool found = false;
    Coordinate current;

    while (!isQueueEmpty(&q)) {
        current = dequeue(&q);

        if (current.x == end.x && current.y == end.y) {
            found = true;
            break; // Exit found!
        }

        // Check 4 neighbors
        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            // Check boundaries and if it's a path and unvisited
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && 
                (maze[ny][nx] == PATH || maze[ny][nx] == EXIT) && !visited[ny][nx]) {
                
                visited[ny][nx] = true;
                parent[ny][nx] = current; // Record current as parent of neighbor
                
                Coordinate neighbor = {nx, ny};
                enqueue(&q, neighbor);
            }
        }
    }

    if (found) {
        // Reconstruct the path backwards from the end
        Coordinate trace = end;
        while (trace.x != start.x || trace.y != start.y) {
            if (maze[trace.y][trace.x] == PATH) {
                maze[trace.y][trace.x] = SOLUTION;
            }
            // Move to the parent coordinate
            trace = parent[trace.y][trace.x];
        }
        return true;
    }

    return false;
}

// --- Display Function ---

void printMaze(int maze[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            switch (maze[i][j]) {
                case WALL:
                    printf("██"); // Wall block
                    break;
                case PATH:
                    printf("  "); // Open path
                    break;
                case START:
                    printf("\033[1;32mS \033[0m"); // Green 'S'
                    break;
                case EXIT:
                    printf("\033[1;31mE \033[0m"); // Red 'E'
                    break;
                case SOLUTION:
                    printf("\033[1;34m**\033[0m"); // Blue Solution Path
                    break;
                default:
                    printf("??");
            }
        }
        printf("\n");
    }
}

// --- Main Program ---

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // 1. Initialize Maze Grid: All walls
    int maze[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            maze[i][j] = WALL;
        }
    }

    // 2. Generate the Maze using DFS
    // DFS works best starting on an odd coordinate (1, 1) and jumping by 2
    generateMaze(maze, 1, 1);

    // 3. Define Start and Exit Points
    Coordinate start = {1, 1}; // Always (1, 1)
    Coordinate end = {WIDTH - 2, HEIGHT - 2}; // Always (WIDTH-2, HEIGHT-2)

    // Mark the start and end in the maze grid
    maze[start.y][start.x] = START;
    maze[end.y][end.x] = EXIT;

    printf("===========================================\n");
    printf(" C MAZE RUNNER: Generated Maze \n");
    printf("===========================================\n");
    printf("S: Start | E: Exit\n\n");

    // Print the generated maze
    printMaze(maze);
    
    printf("\n");
    printf("===========================================\n");
    printf(" C MAZE RUNNER: Shortest Solution (BFS) \n");
    printf("===========================================\n");
    printf("**: Shortest Path Found\n\n");

    // 4. Solve the Maze using BFS
    if (solveMaze(maze, start, end)) {
        // Print the solved maze
        printMaze(maze);
    } else {
        printf("Error: Could not find a path! (This should not happen with DFS-generated mazes)\n");
    }


    return 0;
}
