source "${TEST_DIR}/lib/funcs.bash"

run_timeout=5

HISTFILE=/dev/null HISTSIZE=100  reference_run \
    bash <(cat "${TEST_DIR}/inputs/bashrc_history" "${TEST_DIR}/inputs/scripts/history-script-4.sh") 2> /dev/null


test_start "History Execution !prefix"

run ./$SHELL_NAME < "${TEST_DIR}/inputs/scripts/history-script-4.sh"

compare_outputs

test_end
