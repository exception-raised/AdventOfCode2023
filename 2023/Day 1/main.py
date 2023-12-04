import re

input_file = open("Day 1/input.txt")

mapped = {
    "one": 1,
    "two": 2,
    "three": 3,
    "four": 4,
    "five": 5,
    "six": 6,
    "seven": 7,
    "eight": 8,
    "nine": 9
}

def get_data_from_string(word):
    matches = re.findall(r'(?=(0|1|2|3|4|5|6|7|8|9|one|two|three|four|five|six|seven|eight|nine))', word)
    if len(matches) != 0:
        return matches
    return []


def get_digit_from_word(word, position):
    value = get_data_from_string(word)[position]
    return int(mapped.get(value, value)) 

total = 0
for line in input_file:
    left    =  get_digit_from_word(line, 0)
    right   =  get_digit_from_word(line, -1)
    total += left*10 + right # Example: 8, 5 ; 8 * 10 = 80 ; 80 + 5 = 85

print(f"Answer: {total}")
input_file.close()