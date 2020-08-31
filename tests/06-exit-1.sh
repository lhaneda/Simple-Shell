source "${TEST_DIR}/lib/funcs.bash"

run_timeout=5

script=$(cat <<EOM
echo Easiest point...
echo ...ever!
exit
echo If the following prints, then exit
echo definitely isn't working!
ls
pwd
w
EOM
)

reference_run sh <(echo "${script}") 2> /dev/null

test_start "'exit' built-in command"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs

test_end
