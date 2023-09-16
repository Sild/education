#!/bin/bash -e

target_task="${1}"

if [ "${target_task}" = "" ]; then
    echo "Usage: $0 {folder_name}"
    exit 1
fi
# remove slashes
target_task="$(echo ${target_task} | sed 's/\///')"

# precheck
cd ${target_task}
if ! cargo fmt --check; then
    echo "cargo fmt fail"
    exit 1
fi
if [ $(cargo clippy 2>&1 | grep "warning") ]; then
    echo "cargo clippy fail"
    exit 1
fi
cd -
zip -r ${target_task}.zip ${target_task} -x "${target_task}/target*" -x "${target_task}/.idea*"
exit 0
