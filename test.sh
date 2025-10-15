#!/bin/sh

# Robust test runner for 42 philosophers to detect deadlocks and data races
# Supports Valgrind Helgrind (default) or DRD via TOOL env var. Example:
#   TOOL=helgrind ./test.sh
#   TOOL=drd ./test.sh "5 800 200 200"  # run single case

set -u

TOOL="${TOOL-helgrind}"
TIMEOUT_SECS="${TIMEOUT-30}"
LOG_DIR="tests_logs"
PHILO_BIN="./philo"

mkdir -p "$LOG_DIR"

echo "Recompiling with debug symbols (-g) ..."
make clean >/dev/null 2>&1
make CFLAGS="-g -Wall -Wextra -Werror -pthread" || {
    echo "Compilation failed" >&2
    exit 1
}

if [ ! -x "$PHILO_BIN" ]; then
    echo "Binary $PHILO_BIN not found or not executable" >&2
    exit 1
fi

# Default test matrix unless a single test is provided as script arg
if [ "$#" -gt 0 ]; then
    # One test case passed as a single quoted string
    TEST_CASES="$1"
else
    TEST_CASES='
1 800 200 200
2 800 200 200
2 800 200 200 3
5 800 200 200
5 800 200 200 10
10 800 200 200
10 800 200 200 5
4 410 200 200
4 310 200 200
'
fi

# Build valgrind tool args
case "$TOOL" in
    helgrind)
        VG_TOOL_ARGS="--tool=helgrind --quiet --history-level=approx --track-lockorders=yes"
        ;;
    drd)
        VG_TOOL_ARGS="--tool=drd --quiet --segment-merging=yes"
        ;;
    *)
        echo "Unknown TOOL='$TOOL'. Supported: helgrind, drd" >&2
        exit 2
        ;;
esac

# Exit with non-zero code on any data race/deadlock detected by Valgrind
VG_COMMON="--error-exitcode=99"

echo "Starting tests with TOOL=$TOOL, timeout=${TIMEOUT_SECS}s"

PASS=0
FAIL=0
TIMEOUTS=0
CRASHES=0

run_case() {
    idx="$1"
    args="$2"
    log="$LOG_DIR/case_${idx}.log"
    echo "Testing [$idx]: $args"
    # timeout returns 124 if timed out
    timeout "${TIMEOUT_SECS}s" valgrind $VG_COMMON $VG_TOOL_ARGS "$PHILO_BIN" $args 2>"$log" 1>>"$log"
    rc=$?

    # Classify result
    if [ $rc -eq 124 ]; then
        echo "  -> Timed out (possible deadlock). Log: $log"
        TIMEOUTS=$((TIMEOUTS+1))
        return 124
    fi
    # Detect Helgrind/Valgrind internal assertion aborts
    if grep -q "Helgrind: .*Assertion 'found' failed" "$log" || grep -qi "valgrind.*aborted" "$log"; then
        echo "  -> Valgrind tool crashed (Helgrind assertion). Log: $log"
        CRASHES=$((CRASHES+1))
        # Try DRD fallback automatically if using helgrind
        if [ "$TOOL" = "helgrind" ]; then
            dlog="$LOG_DIR/case_${idx}_drd.log"
            echo "     Fallback to DRD..."
            timeout "${TIMEOUT_SECS}s" valgrind $VG_COMMON --tool=drd --quiet --segment-merging=yes "$PHILO_BIN" $args 2>"$dlog" 1>>"$dlog"
            drc=$?
            if [ $drc -eq 124 ]; then
                echo "     -> DRD timed out (possible deadlock). Log: $dlog"
                TIMEOUTS=$((TIMEOUTS+1))
                return 124
            elif [ $drc -eq 99 ] || grep -Ei "(conflicting memory|data race|Mutex.*locked.*destroyed|Thread.*still holds)" "$dlog" >/dev/null; then
                echo "     -> DRD FAIL: concurrency errors detected. Log: $dlog"
                FAIL=$((FAIL+1))
                return 99
            elif [ $drc -ne 0 ]; then
                echo "     -> DRD ERROR exit code $drc. Log: $dlog"
                FAIL=$((FAIL+1))
                return $drc
            else
                echo "     -> DRD OK"
                PASS=$((PASS+1))
                return 0
            fi
        fi
        return 125
    fi
    # Tool-reported errors (rc==99 due to --error-exitcode)
    if [ $rc -eq 99 ]; then
        # Try to extract a brief reason
        if [ "$TOOL" = "helgrind" ]; then
            reason=$(grep -E "(Possible data race|Lock.*order|deadlock|Exiting thread still holds)" "$log" | head -n 1)
        else
            reason=$(grep -E "(conflicting memory|data race|Mutex.*locked.*destroyed|Thread.*still holds)" "$log" | head -n 1)
        fi
        [ -z "$reason" ] && reason="Concurrency errors detected"
        echo "  -> FAIL: $reason. Log: $log"
        FAIL=$((FAIL+1))
        return 99
    fi
    # Even if rc==0, scan for suspicious patterns that some tools might not exit non-zero for
    if [ "$TOOL" = "helgrind" ]; then
        if grep -Eq "(Possible data race|deadlock|Exiting thread still holds|race condition)" "$log"; then
            echo "  -> FAIL: Patterns indicating races/deadlocks found. Log: $log"
            FAIL=$((FAIL+1))
            return 99
        fi
    else
        if grep -Ei "(conflicting memory|data race|Mutex.*locked.*destroyed|Thread.*still holds)" "$log" >/dev/null; then
            echo "  -> FAIL: Patterns indicating races/deadlocks found. Log: $log"
            FAIL=$((FAIL+1))
            return 99
        fi
    fi
    # Non-standard error code
    if [ $rc -ne 0 ]; then
        echo "  -> ERROR: program exited with code $rc. See $log"
        FAIL=$((FAIL+1))
        return $rc
    fi
    echo "  -> OK"
    PASS=$((PASS+1))
    return 0
}

i=1
while IFS= read -r line; do
    [ -z "$line" ] && continue
    run_case "$i" "$line"
    i=$((i+1))
done <<EOF
$TEST_CASES
EOF

echo ""
echo "Summary: PASS=$PASS, FAIL=$FAIL, TIMEOUTS=$TIMEOUTS, TOOL_CRASHES=$CRASHES"
echo "Logs saved in $LOG_DIR/"

# Exit non-zero if failures or timeouts
if [ $FAIL -gt 0 ] || [ $TIMEOUTS -gt 0 ]; then
    exit 1
fi
exit 0