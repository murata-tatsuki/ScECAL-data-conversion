#!/bin/bash
# -------------------------------------------------------------
# High‑/Low‑gain inter‑calibration for ScECAL – KEK Run 171
# (Input: ECAL_Run171_20250605_162329.root)
# -------------------------------------------------------------
# Usage:  ./hl_ratio_Run171.sh
# NOTE:   Make sure the inter‑calibration executable (usually
#         called `hl_ratio` or `main`) is already compiled and
#         is in the same directory or in $PATH.
# -------------------------------------------------------------
set -e  # abort on first error

# ---------- User‑configurable section ----------
DATAFILE="ECAL_Run276_20250608_224843"
INPUT_DIR="/megraid01/users/data_beamtest/disk-BeamTestData1/Analyzed/decode/mipScan"
OUTPUT_DIR="/megraid01/users/takatsu_t/ScECAL_KEK_Results"
EXECUTABLE="$(dirname "$0")/hl_ratio"
# ---------- Derived paths ----------
INPUT_ROOT="${INPUT_DIR}/${DATAFILE}.root"
OUTPUT_ROOT="${OUTPUT_DIR}/${DATAFILE}_InterCalib.root"

# ---------- Sanity checks ----------
if [ ! -f "${INPUT_ROOT}" ]; then
  echo "[ERROR] Input file not found: ${INPUT_ROOT}" >&2
  exit 1
fi

mkdir -p "${OUTPUT_DIR}"

echo "============================================================="
echo "  ScECAL HG/LG inter‑calibration (Run 171)"
echo "  Input  : ${INPUT_ROOT}"
echo "  Output : ${OUTPUT_ROOT}"
echo "============================================================="

# ---------- Run the calibration ----------
"${EXECUTABLE}" "${INPUT_ROOT}" "${OUTPUT_ROOT}"

echo "[OK] Calibration finished. Results written to: ${OUTPUT_ROOT}"

