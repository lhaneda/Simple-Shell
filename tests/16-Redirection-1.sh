source "${TEST_DIR}/lib/funcs.bash"

run_timeout=5

script=$(cat <<EOM
rm -f ${TEST_DIR}/testfile1 ${TEST_DIR}/testfile2
echo Hello world this is a nice long message to save to a file > ${TEST_DIR}/testfile1
cat /etc/services > ${TEST_DIR}/testfile2
echo This will overwrite the first message > ${TEST_DIR}/testfile1
cat ${TEST_DIR}/testfile1 ${TEST_DIR}/testfile2 > ${TEST_DIR}/final_output.txt
cat ${TEST_DIR}/final_output.txt
${TEST_DIR}/inputs/scripts/get-stats.sh ${TEST_DIR}/final_output.txt
rm -f ${TEST_DIR}/final_output.txt
rm -f ${TEST_DIR}/testfile1 ${TEST_DIR}/testfile2
EOM
)

reference_run sh <(echo "${script}") 2> /dev/null

test_start "Output redirection" \
    "This test writes several strings to files and ensures their results are correct. It also checks to make sure the permissions are set up correctly with open() -- use 0666."

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs

test_end
