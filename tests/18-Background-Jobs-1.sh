source "${TEST_DIR}/lib/funcs.bash"

run_timeout=1

script=$(cat <<EOM
${TEST_DIR}/inputs/scripts/sleeper.sh 500 &
${TEST_DIR}/inputs/scripts/sleeper.sh 100 &
${TEST_DIR}/inputs/scripts/sleeper.sh 85 &
${TEST_DIR}/inputs/scripts/sleeper.sh 450 &
echo Done
${TEST_DIR}/inputs/scripts/kill-parent.sh
EOM
)

test_start "Background job support"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare <(echo "Done") <(echo "${program_output}")

test_end
