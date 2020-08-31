source "${TEST_DIR}/lib/funcs.bash"

run_timeout=5

script=$(cat <<EOM
cd /etc
pwd
cd /this/is/invalid
pwd
cd /usr/bin
./pwd
cd ../..
ls
cd
pwd
EOM
)

reference_run sh <(echo "${script}") 2> /dev/null

test_start "'cd' built-in command"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs

test_end
