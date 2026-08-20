#!/bin/bash
# -------------------------------------------------------------
# Batch HG/LG inter-calibration — *Run250–Run289* だけを対象
# -------------------------------------------------------------
# 使い方:
#   ./hl_ratio_batch_run250_289.sh         # hl_ratio バイナリを同じ dir に置く
# -------------------------------------------------------------
set -euo pipefail

BASE_INPUT_DIR="/megraid01/users/data_beamtest/ECAL_data/analysed/2025/kek/decode"
BASE_OUTPUT_DIR="/megraid01/users/takatsu_t/ScECAL_KEK_Results"
EXECUTABLE="$(dirname "$0")/hl_ratio"   # 必要なら書き換え

if [[ ! -x "$EXECUTABLE" ]]; then
  echo "[ERROR] Executable not found or not executable: $EXECUTABLE" >&2
  exit 1
fi

echo "============================================================="
echo "  Batch inter-calib for Run250–Run289 only"
echo "============================================================="

time_start=$(date +%s)

find "$BASE_INPUT_DIR" -type f -name "*.root" | sort | while read -r INPUT_ROOT; do
    fname=$(basename "$INPUT_ROOT")
    # 期待フォーマット ..._Run<NNN>_YYYYMMDD_*.root
    if [[ $fname =~ _Run([0-9]{3})_ ]]; then
        run=${BASH_REMATCH[1]}
        if (( run >= 250 && run <= 289 )); then
            # 相対パス維持して出力先を作成
            rel_path="${INPUT_ROOT#${BASE_INPUT_DIR}/}"
            run_name="${rel_path%.root}"
            OUTPUT_ROOT="$BASE_OUTPUT_DIR/${run_name}_InterCalib.root"
            mkdir -p "$(dirname "$OUTPUT_ROOT")"

            echo "\n--- Processing Run $run ---"
            echo "Input : $INPUT_ROOT"
            echo "Output: $OUTPUT_ROOT"

            "$EXECUTABLE" "$INPUT_ROOT" "$OUTPUT_ROOT"
            echo "[OK] Run $run finished."
        fi
    fi
done

time_end=$(date +%s)
echo "============================================================="
echo "  Selected runs completed in $((time_end-time_start)) s"
echo "  Outputs are under: $BASE_OUTPUT_DIR"
echo "============================================================="
