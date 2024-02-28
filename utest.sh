#!/bin/sh -e
# μTest: modular testing framework written in POSIX sh
# Developed by Marco Cetica 2024 <email@marcocetica.com
#

### Assert functions ###
assert_eq() {
    [ "$1" = "$2" ] && return 0;

    printf "Assertion failed('%s == %s')\n" "$1" "$2"
    return 1
}

assert_neq() {
    [ "$1" != "$2" ] && return 0;

    printf "Assertion failed('%s != %s')\n" "$1" "$2"
    return 1
}

assert_t() {
    assert_eq "$1" true

    return "$?"
}

assert_f() {
    assert_eq "$1" false

    return "$?"
}

assert_gt() {
    [ "$1" -gt "$2" ] && return 0;

    printf "Assertion failed('%s > %s')\n" "$1" "$2"
    return 1
}

assert_ge() {
    [ "$1" -ge "$2" ] && return 0;

    printf "Assertion failed('%s >= %s')\n" "$1" "$2"
    return 1
}

assert_lt() {
    [ "$1" -lt "$2" ] && return 0;

    printf "Assertion failed('%s < %s')\n" "$1" "$2"
    return 1
}

assert_le() {
    [ "$1" -le "$2" ] && return 0;

    printf "Assertion failed('%s <= %s')\n" "$1" "$2"
    return 1
}
######

# Check whether tests directory is specified and exists
[ "$#" -ne 1 ] && { echo "Specify tests directory"; exit 1; } || TESTS_DIR="$1"
[ ! -d "$TESTS_DIR" ] && { echo "'$TESTS_DIR' directory not found"; exit 1; }

# Count number of test files
count_files() { echo "$#"; }
NUM_TESTS=$(count_files "$TESTS_DIR"/test_*)
IDX=1
# Execute all tests from $TESTS_DIR in ascending order
for test_file in $(printf "%s\n" "$TESTS_DIR"/test_* | sort -n -t '_' -k 2); do
    if [ -f "$test_file" ]; then
        printf "[%s/%s] - Running '%s'...\n" "$IDX" "$NUM_TESTS" "$test_file"
        # shellcheck source=tests/test_1
        . "$test_file"
        utest
        IDX=$((IDX+1))
    fi
done
# vim: ts=4 sw=4 softtabstop=4 expandtab:
