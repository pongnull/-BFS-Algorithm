#AHMAD ZAINAL ABQARI BIN MOHD ABD AZIZ
#CA21101 BEST FIRST SEARCH
import copy
from heapq import heappush, heappop

# Define the size of the sliding puzzle
N = 3

# Define the goal state
goal_state = [[1, 2, 3], [4, 5, 0], [6, 7, 8]]

# Define the moves for the empty tile (up, down, left, right)
row_moves = [-1, 1, 0, 0]
col_moves = [0, 0, -1, 1]

class PuzzleState:
    def __init__(self, board):
        self.board = board

    def __eq__(self, other):
        return self.board == other.board

    def __hash__(self):
        return hash(tuple(map(tuple, self.board)))

class Node:
    def __init__(self, state, parent, g_cost, h_cost):
        self.state = state
        self.parent = parent
        self.g_cost = g_cost
        self.h_cost = h_cost

    def __lt__(self, other):
        return (self.g_cost + self.h_cost) < (other.g_cost + other.h_cost)

def is_valid_move(row, col):
    return 0 <= row < N and 0 <= col < N

def get_neighbors(current_node):
    neighbors = []
    zero_row, zero_col = find_zero(current_node.state.board)

    for i in range(4):
        new_row, new_col = zero_row + row_moves[i], zero_col + col_moves[i]

        if is_valid_move(new_row, new_col):
            new_board = copy.deepcopy(current_node.state.board)
            new_board[zero_row][zero_col], new_board[new_row][new_col] = new_board[new_row][new_col], new_board[zero_row][zero_col]
            neighbor_state = PuzzleState(new_board)
            g_cost = current_node.g_cost + 1
            h_cost = calculate_heuristic(neighbor_state)
            neighbor_node = Node(neighbor_state, current_node, g_cost, h_cost)
            neighbors.append(neighbor_node)

    return neighbors

def calculate_heuristic(state):
    # Simple heuristic: count the number of misplaced tiles
    misplaced_tiles = sum(1 for i in range(N) for j in range(N) if state.board[i][j] != goal_state[i][j])
    return misplaced_tiles

def find_zero(board):
    for i in range(N):
        for j in range(N):
            if board[i][j] == 0:
                return i, j

def print_solution_path(node):
    path = []
    while node:
        path.append(node.state.board)
        node = node.parent
    for state in reversed(path):
        print_puzzle(state)
        print()

def print_puzzle(board):
    for row in board:
        print(" ".join(map(str, row)))
    print()

def a_star(start_state):
    start_node = Node(start_state, None, 0, calculate_heuristic(start_state))
    open_set = [start_node]
    closed_set = set()

    while open_set:
        current_node = heappop(open_set)

        if current_node.state == PuzzleState(goal_state):
            print("Goal state found!")
            print_solution_path(current_node)
            return current_node

        closed_set.add(current_node.state)

        neighbors = get_neighbors(current_node)
        for neighbor in neighbors: 
            if neighbor.state not in closed_set:
                if neighbor not in open_set:
                    heappush(open_set, neighbor)
                else:
                    existing_neighbor = next((n for n in open_set if n == neighbor), None)
                    if neighbor.g_cost < existing_neighbor.g_cost:
                        existing_neighbor.g_cost = neighbor.g_cost
                        existing_neighbor.parent = neighbor.parent

    print("No solution found.")
    return None

if __name__ == "__main__":
    # Set the start state
    start_state = PuzzleState([[4, 1, 3], [6, 2, 5], [7, 8, 0]])

    # Call the A* algorithm
    result_node = a_star(start_state)
