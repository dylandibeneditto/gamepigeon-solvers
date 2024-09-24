import pyautogui
pyautogui.PAUSE = 0.03

def load_words(file_path):
    """Load words from a text file."""
    with open(file_path, 'r') as file:
        words = file.read().splitlines()
    return words

def can_form_word(input_str, word):
    """Check if a word can be formed from the input string (without using letters more than once)."""
    input_chars = list(input_str.upper())
    for letter in word:
        if letter in input_chars:
            input_chars.remove(letter)  # Remove the letter to prevent reuse
        else:
            return False
    return True

def find_anagrams(input_str, words_list):
    """Find words that can be formed from the input string in the provided words list."""
    anagrams = []
    for word in words_list:
        if can_form_word(input_str, word):
            anagrams.append(word)
    
    # Sort the anagrams by length from longest to shortest
    anagrams.sort(key=len, reverse=True)
    
    return anagrams

def get_letter_positions(input_str, anagram):
    """Return the positions of letters from input_str to match the anagram."""
    input_str = list(input_str.upper())
    positions = []
    
    for letter in anagram:
        index = input_str.index(letter)
        positions.append(index)
        input_str[index] = None  # Mark the used letter
    
    return positions

def main():
    # Load words from dict.txt
    words_list = load_words('../dict.txt')
    
    # Get input string from user
    input_str = input("Enter a string of characters: ").strip().lower()
    
    # Find anagrams (words that can be formed)
    anagrams = find_anagrams(input_str, words_list)
    
    if not anagrams:
        print("No anagrams found.")
    else:
        # If you want to automate clicks using pyautogui
        pyautogui.moveTo(40, 740)
        pyautogui.click()
        
        for anagram in anagrams:
            positions = get_letter_positions(input_str, anagram)
            print(f"Anagram: {anagram} -> Positions: {''.join(map(str, positions))}")
            
            for position in positions:
                pyautogui.moveTo(40 + (position * 45), 740)
                pyautogui.click()
                print(position)
            pyautogui.moveTo(165, 550)
            pyautogui.click()

if __name__ == "__main__":
    main()
