#!/usr/bin/env bash

# Counts the number of lines in each file within a specified directory.
#
# Usage:
#   ./count-lines.sh /path/to/directory
#   ./count-lines.sh /path/to/directory -r
#   ./count-lines.sh /path/to/directory --recurse

set -uo pipefail

usage() {
    echo "Usage: $0 <directory> [-r|--recurse]"
    echo
    echo "Options:"
    echo "  -r, --recurse   Include subdirectories"
    echo "  -h, --help      Show this help message"
}

if [[ $# -lt 1 ]]; then
    usage >&2
    exit 1
fi

DIR="$1"
RECURSE=false

shift

while [[ $# -gt 0 ]]; do
    case "$1" in
        -r|--recurse)
            RECURSE=true
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Error: Unknown option '$1'" >&2
            usage >&2
            exit 1
            ;;
    esac
    shift
done

if [[ ! -d "$DIR" ]]; then
    echo "Error: '$DIR' is not a valid directory" >&2
    exit 1
fi

if [[ ! -r "$DIR" ]]; then
    echo "Error: Cannot read directory '$DIR'" >&2
    exit 1
fi

total_lines=0

printf "%-50s %10s\n" "FileName" "LineCount"
printf "%-50s %10s\n" "----------------------------------------" "----------"

while IFS= read -r -d '' file; do
    # Count newline characters and include a final unterminated line.
    line_count=$(awk 'END { print NR }' "$file" 2>/dev/null) || {
        echo "Warning: Could not read '$file'" >&2
        continue
    }

    filename=$(basename -- "$file")

    printf "%-50s %10s\n" "$filename" "$line_count"

    total_lines=$((total_lines + line_count))
done < <(
    if [[ "$RECURSE" == true ]]; then
        find "$DIR" -type f -print0
    else
        find "$DIR" -maxdepth 1 -type f -print0
    fi
)

echo "------------------------------------------------------------"
echo "Total lines: $total_lines"
