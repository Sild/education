#!/bin/bash

GREEN='\033[0;32m'
NC='\033[0m'

target_addr="${1}"

if [ "${target_addr}" = "" ]; then
    echo "Usage: ./run.sh {http://ip_addr:port}"
    exit 1
fi

USER_DIR="$(pwd)"
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd ${SCRIPT_DIR}


# git clone https://github.com/wg/wrk && cd wrk && make && cp wrk /usr/local/bin
pwd

echo "warming up..."
wrk -c 100 -d 5 -t 5 --latency --timeout=1s -s multiple-url-path.lua "${target_addr}" >/dev/null

run_wrk () {
    conns=$1
    threads=$2
    echo ""
    echo -e "${GREEN}running wrk with ${conns} connections, ${threads} threads:${NC}"
    wrk -c ${conns} -d 5 -t ${threads} --latency --timeout=1s -s multiple-url-path.lua "${target_addr}"
}

run_wrk 600 16
run_wrk 1200 32
run_wrk 1800 48

cd ${USER_DIR}
