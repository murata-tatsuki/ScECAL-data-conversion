#!/bin/bash
# -------------------------------------------------------------
# Batch HG/LG inter-calibration for **all** ScECAL runs (2025/KEK)
#   - Input root files are searched under
#       /megraid01/users/data_beamtest/ECAL_data/analysed/2025/kek/decode
#   - Results are written to the same relative path beneath
#       /megraid01/users/takatsu_t/ScECAL_KEK_Results
# -------------------------------------------------------------
# Usage:
#   ./hl_ratio_batch_2025.sh        # assumes hl_ratio is in the same dir
# -------------------------------------------------------------
set -euo pipefail

BASE_INPUT_DIR="/megraid01/users/data_beamtest/ECAL_data/analysed/2025/kek/decode"
BASE_OUTPUT_DIR="/megraid01/users/takatsu_t/ScECAL_KEK_Results"
EXECUTABLE="$(dirname "$0")/hl_ratio"   # change if binary name differs

if [[ ! -x "$EXECUTABLE" ]]; then
  echo "[ERROR] Executable not found or not executable: $EXECUTABLE" >&2
  exit 1
fi

echo "============================================================="
echo "  Batch HG/LG inter-calibration — 2025 KEK analysed samples"
echo "  Input root base : $BASE_INPUT_DIR"
echo "  Output root base: $BASE_OUTPUT_DIR"
echo "============================================================="

time_start=$(date +%s)

# find ですべての .root を列挙（深いサブディレクトリも対象）
find "$BASE_INPUT_DIR" -type f -name "*.root" | sort | while IFS= read -r INPUT_ROOT; do
  # relative path below BASE_INPUT_DIR, without .root
  rel_path="${INPUT_ROOT#${BASE_INPUT_DIR}/}"
  run_name="${rel_path%.root}"

  OUTPUT_ROOT="$BASE_OUTPUT_DIR/${run_name}_InterCalib.root"
  mkdir -p "$(dirname "$OUTPUT_ROOT")"

  echo "\n--- Processing $run_name ---"
  echo "Input : $INPUT_ROOT"
  echo "Output: $OUTPUT_ROOT"

  "$EXECUTABLE" "$INPUT_ROOT" "$OUTPUT_ROOT"
  echo "[OK] $run_name finished."  

done

time_end=$(date +%s)
runtime=$((time_end - time_start))
echo "============================================================="
echo "  All runs completed in $runtime seconds."
echo "  Results are under: $BASE_OUTPUT_DIR"
echo "============================================================="
