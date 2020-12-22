"""Day 22: Crab Combat

Play a crab in Combat (War) and calculate the score of the winner.
"""

from collections import deque
from copy import deepcopy

def parse(filename):
    """Parses the input file into a list of players numbers.  Input file
    has the format:
        Player 1:
        1
        2
        3

        Player 2:
        4
        5
        6

    Supports arbitrary numbers of players.
    """
    players = [deque()]
    current = 0
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()

            if line.isnumeric():
                players[current].append(int(line))
                continue
                
            if not line:
                current += 1
                players.append(deque())
    
    return players


def part1(players):
    """Simulate a game of war and then calculate the score of the
    winner.
    """
    while all(player for player in players):
        plays = [player.popleft() for player in players]
        winner = max((value, ind) for (ind, value) in enumerate(plays))
        players[winner[1]].extend(sorted(plays, reverse=True))

    return max(calculate_score(player) for player in players)


def recursive_war(p1, p2):
    """Recursive war follows these rules:

      1. If a particular configuration has been seen before in a game,
         Player 1 automatically wins.
      2. Otherwise, if either player's number is greater than the number
         of cards they have, the player with the highest number wins.
      3. Otherwise, the winner is the winner of a sub-game, where
         players play with the 1-nth cards in their deck where n is the
         value of their 0th card.

    Subgames do not affect parent games' decks (i.e. they are played
    with copies).

    Returns the calculated score of the winner of the root game's deck.
    Not designed for arbitrary #'s of players.  Only two.
    """
    seen = set()

    while p1 and p2:
        if str(p1) + str(p2) in seen:
            return 0, calculate_score(p1)
        else:
            seen.add(str(p1) + str(p2))
        v1, v2 = p1.popleft(), p2.popleft()

        if v1 <= len(p1) and v2 <= len(p2):
            winner, _score = recursive_war(deque(list(p1)[:v1]), deque(list(p2)[:v2]))
        else:
            winner = 0 if v1 > v2 else 1

        if winner == 0:
            p1.extend([v1, v2])
        else:
            p2.extend([v2, v1])

    if p1:
        return 0, calculate_score(p1)
    else:
        return 1, calculate_score(p2)


def calculate_score(player):
    """Calculate the "score" of a player's deck.  Their bottom card
    times 1 plus their second bottom card times 2 plus ...
    """
    return sum(val * ind
                for ind, val in enumerate(reversed(player), start=1))


def part2(players):
    """Calculate the score of a winner of recursive War."""
    winner, score = recursive_war(*players)
    return score


if __name__ == "__main__":
    players = parse("data/day22_test.txt")
    assert part1(deepcopy(players)) == 306
    assert part2(players) == 291
    print("All tests passed!")

    players = parse("data/day22.txt")
    print("Part 1: ", part1(deepcopy(players)))
    print("Part 2: ", part2(players))