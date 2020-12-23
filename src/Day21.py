"""Day 21: Allergen Assessment

Find out through process of elimination which foreign ingredients
contain which allergens.
"""

from collections import Counter


def parse(filename):
    """Parse the input file into Python objects.

    Input file has lines like this:
        sdfsjdfk sjdkf lfke ljrl (contains dairy, soy)
    
    Parameters:
        filename (str): the name of the input file to open

    Returns:
        candidates (Dict[str, Set[str]]): a dict with keys that are the
            allergens (dairy, soy, etc.) and values that are the sets
            of all possible ingredients that could contain those
            allergens.
        counts (Counter): a count of how many lines each ingredient
            appears in.
    """

    candidates = dict()
    counts = Counter()
    with open(filename, "r") as f:
        for line in f:
            ingredients, _contains, allergens = line.partition(" (contains ")
            ingredients = set(ingredients.split())
            counts += Counter(ingredients)
            for allergen in allergens.strip(")\n").split(", "):
                if allergen in candidates:
                    candidates[allergen] &= ingredients
                else:
                    candidates[allergen] = ingredients.copy()

    return candidates, counts


def find_allergens(candidates):
    """Uses allergens that only have one possible ingredient that could
    contain them to rule that ingredient out for other allergens until
    all allergens have exactly one ingredient that could contain it.

    Returns a dict of allergen name to ingredient name to simplify things.
    """
    while not all(len(ingredients) == 1 for ingredients in candidates.values()):
        for allergen, ingredients in candidates.items():
            if len(ingredients) == 1:
                for other_al, other_ingredients in candidates.items():
                    if other_al != allergen:
                        other_ingredients -= ingredients
    return {allergen: ingredients.pop() for allergen, ingredients in candidates.items()}


def discard_unsafe_ingredient_counts(allergens, counts):
    """Given a set of decided allergens, removes each of these unsafe
    ingredients from a dict of counts so that only the counts of totally
    safe ingredients are retained.
    """
    for ingredient in allergens.values():
        del counts[ingredient]


def part1(counts):
    """Part 1 asks us to add up the number of occurrences of any totally
    safe ingredient.

    Assumes `counts` has already had its unsafe ingredients discarded.
    """
    return sum(counts.values())


def part2(allergens):
    """Part 2 wants the list of unsafe ingredients, sorted by their
    allergen alphabetically, joined together into one string by commas.
    """
    return ",".join(
        ingredient for (allergen, ingredient) in sorted(allergens.items())
    )

if __name__ == "__main__":
    test_candidates, test_counts = parse("data/day21_test.txt")
    test_allergens = find_allergens(test_candidates)
    discard_unsafe_ingredient_counts(test_allergens, test_counts)
    assert 5 == part1(test_counts)
    assert "mxmxvkd,sqjhc,fvjkl" == part2(test_allergens)
    print("All tests passed!")

    candidates, counts = parse("data/day21.txt")
    allergens = find_allergens(candidates)
    discard_unsafe_ingredient_counts(allergens, counts)
    print("Part 1:", part1(counts))
    print("Part 2:", part2(allergens))