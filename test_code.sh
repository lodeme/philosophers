#!/bin/bash

# Initialize an associative array to hold exit code counts
declare -A exit_codes

# Run the program 100 times
for i in {1..100}; do
  ./philo 4 25 10 10 2
  exit_code=$?

  # Increment the count for this exit code
  if [[ -z ${exit_codes[$exit_code]} ]]; then
    exit_codes[$exit_code]=1
  else
    exit_codes[$exit_code]=$((exit_codes[$exit_code]+1))
  fi
done

# Summarize the results
echo "Exit Code Summary:"
for code in "${!exit_codes[@]}"; do
  echo "Exit Code $code: ${exit_codes[$code]} times"
done
