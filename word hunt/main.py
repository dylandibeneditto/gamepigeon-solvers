import time
from pynput.mouse import Button, Controller

mouse = Controller()
mouse_delay = 0.12  # Slightly increased delay between mouse actions

class TrieNode:
    def __init__(self):
        self.children = {}
        self.is_end_of_word = False

class Trie:
    def __init__(self):
        self.root = TrieNode()

    def insert(self, word):
        """Insert a word into the trie."""
        node = self.root
        for letter in word:
            if letter not in node.children:
                node.children[letter] = TrieNode()
            node = node.children[letter]
        node.is_end_of_word = True

# Directions for moving in a 5x5 grid (horizontal, vertical, diagonal)
DIRECTIONS = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]

def load_words(file_path):
    """Load words from a text file and insert them into a trie."""
    trie = Trie()
    with open(file_path, 'r') as file:
        words = file.read().splitlines()
    for word in words:
        if len(word) >= 2:
            trie.insert(word.upper())
    return trie

def is_valid_position(x, y, visited):
    """Check if a position is within the 5x5 grid and not already visited."""
    return 0 <= x < 5 and 0 <= y < 5 and (x, y) not in visited

def find_words_from_position(board, trie, x, y, current_word, visited, results, node):
    """Recursively find valid words starting from a specific board position."""
    letter = board[x][y]
    
    if letter not in node.children:
        return  # Prune the search if the current path doesn't exist in the trie

    node = node.children[letter]
    current_word += letter
    visited.append((x, y))

    if node.is_end_of_word and current_word not in results:
        results[current_word] = visited[:]

    if len(current_word) > 25:
        return

    for direction in DIRECTIONS:
        new_x, new_y = x + direction[0], y + direction[1]
        if is_valid_position(new_x, new_y, visited):
            find_words_from_position(board, trie, new_x, new_y, current_word, visited, results, node)

    visited.pop()

def find_all_words(board_str, trie):
    """Find all valid words from a 5x5 board (flattened string) using a trie."""
    board = [list(board_str[i:i+5]) for i in range(0, 25, 5)]
    
    results = {}
    
    for x in range(5):
        for y in range(5):
            find_words_from_position(board, trie, x, y, "", [], results, trie.root)
    
    sorted_results = sorted(results.items(), key=lambda item: -len(item[0]))
    return sorted_results

def posToCoord(position):
    """Convert the grid position to screen coordinates."""
    return [(position[1]*50)+70, (position[0]*50)+485]

def getScore(x):
    if x == 3:
        return 100
    else:
        return (x-3)*400

def main():
    # Load words into a trie from dict.txt
    trie = load_words('../dict.txt')  # Assume load_words is correctly defined
    
    # Get the 5x5 board from user input as a flat string
    board_str = input("Enter a 25-character string (representing the 5x5 board): ").strip().upper()
    
    if len(board_str) != 25:
        print("Error: The input must be exactly 25 characters long.")
        return
    
    # Find all valid words
    words_with_positions = find_all_words(board_str, trie)  # Assume find_all_words is correctly defined
    
    # Perform initial click
    mouse.position = (70, 465)  # Move to the initial position
    time.sleep(mouse_delay)  # Add a small delay before clicking
    mouse.click(Button.left, 1)  # Click to start the drawing process
    time.sleep(mouse_delay)  # Add a small delay to ensure the click is registered

    if not words_with_positions:
        print("No words found.")
    else:
        lastLength = 25
        perfectScore = 0
        perfectWords = 0
        for word, positions in words_with_positions:
            wordLength = len(positions)
            if wordLength > 2:
                
                if wordLength < lastLength:
                    lastLength = wordLength
                    print(f"--- Word Length: {lastLength} ---")
                # Get the starting position
                start_coords = posToCoord(positions[0])
                
                # Move to the starting position and hold the mouse down
                mouse.position = (start_coords[0], start_coords[1])
                time.sleep(mouse_delay/5)
                mouse.press(Button.left)  # Hold down the left mouse button
                print(word)
                perfectScore += getScore(wordLength)
                perfectWords += 1
                
                
                # Move through all the positions while holding the mouse button down
                for position in positions[1:]:
                    coords = posToCoord(position)
                    
                    # Move to the new position without releasing the button
                    time.sleep(mouse_delay/6)
                    mouse.position = (coords[0], coords[1])
                    
                # Release the mouse after tracing the word
                mouse.release(Button.left)
                time.sleep(mouse_delay/5)  # Add delay between words to avoid fast actions
            else:
                print(f"{perfectScore}pts, {len(words_with_positions)} words")
                return

if __name__ == "__main__":
    main()