#!/bin/bash
# Try to convert all .cpp and .h files in this directory to UTF-8 using several common encodings.
# The script will back up the original file as filename.bak before conversion.

encodings=(GBK GB2312 GB18030 ISO-8859-1)

for file in *.cpp *.h; do
  echo "\nProcessing $file..."
  cp "$file" "$file.bak"
  success=0
  for enc in "${encodings[@]}"; do
    if iconv -f "$enc" -t UTF-8 "$file.bak" -o "$file.utf8" 2>/dev/null; then
      mv "$file.utf8" "$file"
      echo "[SUCCESS] Converted $file from $enc to UTF-8."
      success=1
      break
    fi
  done
  if [ $success -eq 0 ]; then
    echo "[FAILED] Could not convert $file with any known encoding. File left as original."
    mv "$file.bak" "$file"
  else
    rm -f "$file.bak"
  fi
  file -i "$file"
done
