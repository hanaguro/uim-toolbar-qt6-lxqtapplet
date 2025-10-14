#!/bin/bash
# update-uim.sh
# uim の qt4/toolbar 部分だけを軽量クローン／更新するスクリプト

set -e

UIM_DIR="extern/uim"
UIM_URL="https://github.com/uim/uim.git"
UIM_PATH="qt4/toolbar"

echo "=== uim submodule updater ==="

# --- 初回チェック ---
if [ ! -d "$UIM_DIR" ]; then
    echo "[INFO] uim submodule not found, cloning..."
    mkdir -p extern
    git clone --depth=1 --filter=blob:none "$UIM_URL" "$UIM_DIR"
    cd "$UIM_DIR"
    git sparse-checkout init --cone
    git sparse-checkout set "$UIM_PATH"
    cd ../..
    git submodule add "$UIM_URL" "$UIM_DIR"
    echo "[INFO] uim submodule added and initialized."
else
    echo "[INFO] uim submodule already exists, updating..."
    cd "$UIM_DIR"
    git fetch origin master --depth=1
    git checkout master
    git pull origin master
    git sparse-checkout init --cone
    git sparse-checkout set "$UIM_PATH"
    cd ../..
fi

# --- shallow クローン設定 ---
if ! grep -q "shallow = true" .gitmodules 2>/dev/null; then
    echo "[INFO] Enabling shallow clone in .gitmodules..."
    git config -f .gitmodules submodule."$UIM_DIR".shallow true
fi

echo "[INFO] uim submodule update complete."
echo "[INFO] Current version:"
(cd "$UIM_DIR" && git log -1 --oneline)

exit 0

