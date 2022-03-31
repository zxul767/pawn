#!/usr/bin/env python3
import fileinput
from functools import partial
from collections import defaultdict

# pip install rich
from rich.console import Console
console = Console()

# Parse the output of our unit test harness via piping, like this:
# test/unit/bin/pawn_unit_test --success | ./compact_reporter.py
#
# The original output looks like this, which is too verbose:
#
#-------------------------------------------------------------------------------
# ::bits
#   Random bitboards have few collisions
# -------------------------------------------------------------------------------
# test/unit/src/bitboard_test.cpp:16
# ...............................................................................

# test/unit/src/bitboard_test.cpp:29: PASSED:
#   REQUIRE( collisions < uint(TRIALS * SQUARES * COLLISIONS_TOLERANCE) )
# with expansion:
#   18 < 32

# -------------------------------------------------------------------------------
# ::bits
#   count_ones (random)
# -------------------------------------------------------------------------------
# test/unit/src/bitboard_test.cpp:34
# ...............................................................................

# test/unit/src/bitboard_test.cpp:47: PASSED:
#   REQUIRE( failures == 0 )
# with expansion:
#   0 == 0
#
# The goal is to remove all the redundant ::<suite-name> headers and produce a
# more compact and easier to read version, such as this one:
#
#::bits
#    🥝️ Random bitboards have few collisions
#    🥝️ count_ones (random)
#    🥝️ count_ones (smoke)
#    🥝️ lsb_position (smoke)
#    🍅️ msb_position (smoke)
# ::game::Board
#    🥝️ Can add a piece to the board
#    🥝️ Should return error on moving from empty square
#    🥝️ Can add and remove a piece to the board
#    🥝️ Can add multiple pieces to board
#    🥝️ Can move a knight from one square to another
#    🥝️ Can move a knight all around the board
#    🥝️ Can get locations for all pieces
#
# We need to do this because none of the reporters from our testing library
# (see https://github.com/catchorg/Catch2/blob/v2.x/docs/command-line.md)
# support a compact format as we want it
#
FAILED_TEST_EMOJI = ":tomato-emoji:"
PASSED_TEST_EMOJI = ":kiwi_fruit-emoji:"

def print_suite(suite_name, tests):
    console.print(f"[bold cyan]{suite_name}")
    for test in tests:
        indentation = 3*" "
        outcome = PASSED_TEST_EMOJI if len(test['failed-assertions']) == 0 else FAILED_TEST_EMOJI
        console.print(f"{indentation}{outcome} {test['name']}")


def parse_test_name(lines):
    suite_name, test_name = None, None
    line = next(lines, None)
    while line:
        line = line.strip()
        if line.startswith('::'):
            suite_name, test_name = line, next(lines, '').strip()
            # skip over the frame divider line so `parse_assertions`
            # works fine
            next(lines, None)
            break
        line = next(lines, None)
    return suite_name, test_name


def parse_assertions(lines):
    passed, failed = [], []
    line = next(lines, None)
    while line and not line.startswith('---'):
        if 'PASSED' in line:
            passed.append(line)
        elif 'FAILED' in line:
            failed.append(line)
        line = next(lines, None)
    return passed, failed


def parse_test_results(lines):
    suite_name, test_name = parse_test_name(lines)
    if not suite_name:
        return None, None, None
    passed, failed = parse_assertions(lines)
    return suite_name, test_name, (passed, failed)


if __name__ == '__main__':
    # read from standard input
    lines = fileinput.input()

    results_by_suite = defaultdict(list)
    while True:
        suite_name, test_name, assertions = parse_test_results(lines)
        if not suite_name:
            break
        results_by_suite[suite_name].append({
            'name': test_name,
            'passed-assertions': assertions[0],
            'failed-assertions': assertions[1],
        })

    for suite_name, tests in results_by_suite.items():
        print_suite(suite_name, tests)
