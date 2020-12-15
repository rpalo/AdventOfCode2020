"""Day 14: Docking Data

Interface with a docking computer doing bit masking to store values
in memory.
"""

from itertools import product
from typing import Generator, Union, List

class MaskCommand:
    """A command that parses and sets the mask for subsequent memory
    setting commands to be filtered through.

    Properties:
        bits (list(int)): The original bit-string of 1's, 0's, and X's.
        and_ (int): A value to be `&`-ed with memory values to set some
                    bits to 0 no matter what
        or_ (int): A value to be `|`-ed with memory values to set some
                    bits to 1 no matter what
        floating (list(int)): Indices where X's live, to help with
                              generating floating bits.
    """
    def __init__(self, s: str):
        self.bits = s
        self.and_ = 0
        self.or_ = 0
        self.floating: list(int) = []
        for i, c in enumerate(s):
            if c == '0':
                self.and_ |= (1 << (35 - i))
            elif c == '1':
                self.or_ |= (1 << (35 - i))
            elif c == 'X':
                self.floating.append(i)
        self.and_ = (~self.and_) & 0xFFFFFFFFF

    @staticmethod
    def generate_binary(digits: int) -> Generator[str, None, None]:
        """Generates strings of binary digits for all values between
        0 and 2**(digits)-1 (i.e. all possible sequences of 1's and 0's.)
        """
        for i in range(1 << digits):
            yield f"{i:0{digits}b}"

    def v2_addresses(self, command: Union['MaskCommand', 'SetCommand']) -> Generator[List[str], None, None]:
        """Generates addresses following V2 guidelines.
        
        Performs an OR between the command's address and the mask, and
        then generates every possible string made up of 1's and 0's in
        place of all the floating bits (X's).

        The OR assumes X's take precedence.

        e.g. 110X0X (after the OR) would yield 110000, 110001, 110100,
        and 110101.
        """ 
        merged = [c_bit if m_bit == '0' else m_bit
                  for (m_bit, c_bit) in zip(self.bits, command.bits)]
        for comb in MaskCommand.generate_binary(len(self.floating)):
            for index, bit in zip(self.floating, comb):
                merged[index] = bit
            yield merged


class SetCommand:
    """A command to set memory to some value.  Stores the address and
    value to be stored there.  Used for both V1 and V2, so includes both
    and integer version of the address and the bits for compatability.

    Properties:
        value (int): The value to be stored.
        address (int): The location in memory to store the value.
        bits (str): A string representation of the 0's and 1's that
                    make up the binary value for the address.  (36 bits wide).
    """
    def __init__(self, address: str, value: str):
        self.value = int(value)
        self.address = int(address)
        self.bits = f"{self.address:036b}"


def parse(filename: str) -> List[Union[MaskCommand, SetCommand]]:
    """Parse the input file into a list of commands."""
    commands = []
    with open(filename, "r") as f:
        for line in f:
            if line.startswith("mask"):
                commands.append(MaskCommand(line[7:]))
            else:
                parts = line.split()
                commands.append(SetCommand(parts[0][4:-1], parts[-1]))
    return commands


def part1(commands: List[Union[MaskCommand, SetCommand]]) -> int:
    """Find the sum of all values in memory after running all commands
    following the V1 spec.
    """
    memory = dict()
    mask = None
    for command in commands:
        if isinstance(command, MaskCommand):
            mask = command
        else:
            new_value = (command.value & mask.and_) | mask.or_
            memory[command.address] = new_value
    
    return sum(memory.values())


def part2(commands: List[Union[MaskCommand, SetCommand]]) -> int:
    """Find the sum of all values in memory after running all commands
    following the V2 spec.
    """
    memory = dict()
    mask = None
    for command in commands:
        if isinstance(command, MaskCommand):
            mask = command
        else:
            for address in mask.v2_addresses(command):
                memory["".join(address)] = command.value
    return sum(memory.values())


if __name__ == "__main__":
    assert 165 == part1(parse("data/day14_test.txt"))
    assert 208 == part2(parse("data/day14_test2.txt"))
    print("All tests passed.")
    commands = parse("data/day14.txt")
    print(part1(commands))
    print(part2(commands))