#!/bin/bash
# update-uim.sh
# uim の qt4/toolbar と関連ファイルだけを軽量クローン／更新するスクリプト

set -e

UIM_DIR="extern/uim"
UIM_URL="https://github.com/uim/uim.git"

echo "=== uim submodule updater ==="

# --- 初回セットアップ or 更新 ---
if [ ! -d "$UIM_DIR" ]; then
    echo "[INFO] uim not found, performing sparse clone..."
    mkdir -p extern
    git clone --depth=1 --filter=blob:none "$UIM_URL" "$UIM_DIR"
    cd "$UIM_DIR"
    git sparse-checkout init --cone
    git sparse-checkout set qt4 uim
    cd ../..
else
    echo "[INFO] uim already exists, updating..."
    cd "$UIM_DIR"
    git fetch origin master --depth=1
    git checkout master
    git pull origin master
    git sparse-checkout init --cone
    git sparse-checkout set qt4 uim
    cd ../..
fi

# --- uim/gettext.h の存在確認 ---
if [ ! -f "$UIM_DIR/uim/gettext.h" ]; then
    echo "[INFO] Fetching uim/gettext.h manually..."
    mkdir -p "$UIM_DIR/uim"
    curl -sSL https://raw.githubusercontent.com/uim/uim/master/uim/gettext.h \
        -o "$UIM_DIR/uim/gettext.h"
fi

# --- config.h の自動生成 ---
CONFIG_FILE="$UIM_DIR/config.h"
if [ ! -f "$CONFIG_FILE" ]; then
    echo "[INFO] Creating dummy config.h..."
    mkdir -p "$(dirname "$CONFIG_FILE")"
    cat > "$CONFIG_FILE" <<'EOF'
#ifndef UIM_CONFIG_H
#define UIM_CONFIG_H

/* Minimal dummy config for building embedded uim toolbar components */
#define ENABLE_NLS 1
#define PACKAGE "uim"
#define VERSION "1.9.6"

#endif /* UIM_CONFIG_H */
EOF
fi

# --- submodule 登録（存在しない場合のみ） ---
if ! git config --file .gitmodules --get-regexp path | grep -q "$UIM_DIR"; then
    echo "[INFO] Registering uim as submodule..."
    git submodule add "$UIM_URL" "$UIM_DIR"
else
    echo "[INFO] uim submodule already registered."
fi

echo "[INFO] uim submodule update complete."
echo "[INFO] Current version:"
(cd "$UIM_DIR" && git log -1 --oneline)

# --- Generate uim/version.h if missing ---
VERSION_IN="$UIM_DIR/uim/version.h.in"
VERSION_OUT="$UIM_DIR/uim/version.h"
if [ -f "$VERSION_IN" ] && [ ! -f "$VERSION_OUT" ]; then
    echo "[INFO] Generating uim/version.h..."
    sed -e 's/(@UIM_VERSION_MAJOR@)/1/' \
        -e 's/(@UIM_VERSION_MINOR@)/9/' \
        -e 's/(@UIM_VERSION_PATCHLEVEL@)/6/' \
        "$VERSION_IN" > "$VERSION_OUT"
fi

exit 0

