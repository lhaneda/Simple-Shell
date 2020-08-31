source "${TEST_DIR}/lib/funcs.bash"

run_timeout=5

script=$(cat <<EOM
# First, let's do an ls:
ls # whoohoo!
echo #Here's a comment
echo # Here's another
        # And another one! ####
echo Line 1 # This better not display
echo Line 2 #There's nothing here! #  echo No way
#bye :-)
##################
EOM
)

reference_run sh <(echo "${script}") 2> /dev/null

test_start "Comment support"

# ---------- Test Script ----------
echo "${script}"
# -------------- End --------------

run ./$SHELL_NAME < <(echo "${script}")

compare_outputs

test_end
