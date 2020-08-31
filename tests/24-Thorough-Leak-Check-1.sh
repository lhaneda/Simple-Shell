source "${TEST_DIR}/lib/funcs.bash"

run_timeout=30

test_start "Thorough Memory Leak Check"

valgrind_output=$(valgrind \
    --trace-children=no \
    --child-silent-after-fork=yes \
    --leak-check=full \
    ./$SHELL_NAME < "${TEST_DIR}/inputs/scripts/leak.sh" 2>&1)

grep 'are definitely lost' <<< "${valgrind_output}"

if [[ $? -eq 0 ]]; then
    echo "Leaks detected! Showing valgrind output."

    echo "${valgrind_output}"

    test_end 1
else
    test_end 0
fi

