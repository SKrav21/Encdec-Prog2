#!/bin/bash

echo -e "\nTesting your code..."
echo -e "\t* successful compile gets you some points"
echo -e "\t* successful check of stdout gets you 4 points for each test"
echo -e "\t* successful check of stderr gets you 1 point  for each test"
echo -e "\t* successful check of rc     gets you 1 point  for each test"
echo -e "\t* (#, #, #) at end of each test indicates pass/fail for stdout, stderr, and rc check, respectively"
echo -e "\nRunning make clean"
make -f makefile clean
echo -e "\nRunning make chgTxt"
make encDec
rc=$?
if [[ $rc -eq 0 ]]; then
  echo -e "\nSuccessful compile"
else
  echo -e "\nCompilation failed"
fi
exit $rc
