source "${TEST_DIR}/lib/funcs.bash"

script=$(cat <<EOM
ls /
cd
ls /
asdfghjklqprewopiqwualasdf # Bad Command!
# Comment only
pwd
EOM
)

test_start "Basic Memory Leak Check"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

valgrind_output=$(valgrind \
    --trace-children=no \
    --child-silent-after-fork=yes \
    --leak-check=full \
    ./$SHELL_NAME < <(echo "${script}") 2>&1)

grep 'are definitely lost' <<< "${valgrind_output}"

if [[ $? -eq 0 ]]; then
    echo "Leaks detected! Showing valgrind output."

    echo "${valgrind_output}"

    test_end 1
else
    test_end 0
fi

test_end
