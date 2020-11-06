#!/usr/bin/env bash

set -e

function print_header() {
    echo -e "\n***** ${1} *****"
}

if [ "${2}" == "--local" ]; then
	CPPCHECK="linters/cppcheck/cppcheck"
else
	CPPCHECK="/usr/bin/cppcheck"
fi

print_header "RUN cppcheck"
${CPPCHECK} "$1"/ --enable=all  --error-exitcode=1 -I"$1"/include --suppress=missingIncludeSystem  #--check-config

print_header "RUN cpplint.py"
python2.7 ./linters/cpplint/cpplint.py "$1"/src/* "$1"/include/*

print_header "SUCCESS"
