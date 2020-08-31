source "${TEST_DIR}/lib/funcs.bash"

run_timeout=5

trap '# Interrupt' INT

script=$(cat <<EOM
sleep 1 &
sleep 2 &
sleep 500 &
sleep 100 &
sleep 450 &
sleep 3
jobs
${TEST_DIR}/inputs/scripts/kill-parent.sh
EOM
)

expected=$(cat <<EOM
sleep 100
sleep 450
sleep 500
EOM
)

test_start "Background job list"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

# We ignore:
# - pids (unique on each system)
# - & (optional to display in the job list)
processed_output=$(grep -o 'sleep\s*\([0-9]*\)' <<< "${program_output}" \
    | sort -n)

compare <(echo "${expected}") <(echo "${processed_output}")

test_end
