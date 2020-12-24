from itertools import product, chain, takewhile

class Rule:
    """A Rule consists of all possible combinations of characters that
    match it.
    
    Properties:
        name (str): the ID of the rule, a positive integer as a string
        rules (List[List[str]]): a list of the initial raw rules that
            are available to match.  Each rule is a list of ID's pointing
            to another rule or a raw character
        resolved_values (Set[str]): concrete strings that this rule matches
    """
    def __init__(self, text):
        name, _colon, tail = text.partition(": ")
        self.name = name
        if '"' in tail:
            self.resolved_values = {tail.strip('"')}
            return

        parts = tail.split(" | ")
        self.rules = [part.split() for part in parts]
        self.resolved_values = None

    def resolve(self, rulebook):
        """A recursive function that resolves this rule and all children
        rules.  Caches its result in self.resolved_values.
        """
        if self.resolved_values:
            return self.resolved_values
        self.resolved_values = {
            "".join(p) 
            for rule in self.rules
            for p in product(*[rulebook[i].resolve(rulebook) for i in rule])
        }
        return self.resolved_values

    def match(self, line):
        """Returns true if this rule matches the line in one of its
        resolved_values."""
        return line in self.resolved_values


def parse(filename):
    """Parses an input file to a set of rules and a set of messages.

    Input files have this format:

        0: 1 4 3
        1: 2 | 5
        2: "a"
        3: "b"
        4: "c"
        5: "d"

        abbabacabacabacbbabbab
        bababababa
        baba
    """
    rules = dict()
    with open(filename, "r") as f:
        while (line := f.readline()) != "\n":
            rule = Rule(line.strip())
            rules[rule.name] = rule
        messages = f.read().splitlines()

    return rules, messages


def part1(rules, messages):
    """Return the number of messages that match rule 0."""
    return sum(rules["0"].match(message) for message in messages)


def chunk_message(message, size):
    """Chunk a string up into `size` size chunks."""
    for i in range(0, len(message), size):
        yield message[i:i + size]

def check_recursive(rules, message, chunksize):
    """Rules 8 and 11 are recursive.  Checks that the message matches
    the rule '8 11'.

    Rule 8: 42 | 42 8, which works out to one or more chunks matching
        rule 42.
    Rule 11: 42 31 | 42 11 31, which works out to one or more sets of
        rule 42 followed by an equal number of sets of rule 31.

    Together, these requirements mean that the message must follow the
    following requirements:
    
        1. The first and second chunk must match 42.
        2. All subsequent chunks must match 42 until:
        3. Chunks must then match one or more 31's.
        4. We have to have matched at least one more 42 than we do 31.

    It's not pretty but it does the job.  TODO: finite state machine.
    """
    chunks = chunk_message(message, chunksize)
    count_42 = 0
    past_42 = False
    count_31 = 0
    for chunk in chunks:
        if len(chunk) != chunksize:
            # Protect against strings that aren't an even multiple of
            # chunksize
            return False

        if not past_42 and chunk in rules["42"].resolved_values:
            # Match one or more 42's and nothing else.
            count_42 += 1
            continue
        
        past_42 = True  # Stop matching 42.

        if chunk not in rules["31"].resolved_values:
            # Match only 31's the rest of the way
            return False
        count_31 += 1

    # Ensure all requirements were met
    return count_42 >= 2 and count_31 >= 1 and count_42 >= 1 + count_31


def part2(rules, messages):
    """Modify Rule 8 and Rule 11 to be recursive.  Now how many
    messages match rule 0 exactly?
    """
    total = 0
    len42 = len(list(rules["42"].resolved_values)[0])
    len31 = len(list(rules["31"].resolved_values)[0])
    assert len42 == len31, "42 chunks don't equal 31 chunks"
    for message in messages:
        if check_recursive(rules, message, len42):
            total += 1
    
    return total


if __name__ == "__main__":
    rules, messages = parse("data/day19_test.txt")
    rules["0"].resolve(rules)
    assert 2 == part1(rules, messages)
    rules, messages = parse("data/day19_test2.txt")
    rules["42"].resolve(rules)
    rules["31"].resolve(rules)
    assert 12 == part2(rules, messages)
    print("All tests passed!")

    rules, messages = parse("data/day19.txt")
    rules["0"].resolve(rules)
    print("Part 1:", part1(rules, messages))
    print("Part 2:", part2(rules, messages))
