source "${TEST_DIR}/lib/funcs.bash"

run_timeout=5

BASH_ENV="${TEST_DIR}/inputs/bashrc_history" HISTFILE=/dev/null HISTSIZE=100 reference_run \
    bash < "${TEST_DIR}/inputs/scripts/history-script-2.sh" 2> /dev/null

test_start "History List"

run ./$SHELL_NAME < "${TEST_DIR}/inputs/scripts/history-script-2.sh"

compare_outputs --ignore-all-space

test_end
