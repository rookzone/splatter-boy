#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
GBDK_DIR="${ROOT_DIR}/.gbdk"
EMU_DIR="${ROOT_DIR}/emu/mgba"

INSTALL_PACKAGES=1
INSTALL_GBDK=1
INSTALL_MGBA=1
FORCE=0

GBDK_RELEASE_API_URL="${GBDK_RELEASE_API_URL:-https://api.github.com/repos/gbdk-2020/gbdk-2020/releases/latest}"
MGBA_RELEASE_API_URL="${MGBA_RELEASE_API_URL:-https://api.github.com/repos/mgba-emu/mgba/releases/latest}"

log() {
    printf '[setup-linux] %s\n' "$*"
}

fail() {
    printf '[setup-linux] Error: %s\n' "$*" >&2
    exit 1
}

have() {
    command -v "$1" >/dev/null 2>&1
}

usage() {
    cat <<'EOF'
Usage: ./scripts/setup-linux.sh [options]

Downloads the Linux build dependencies used by this repo into ignored local
folders so a fresh machine can run `make` and `make run` quickly.

Options:
  --skip-packages  Do not install missing system packages with the distro package manager
  --skip-gbdk      Do not download/update the local GBDK toolchain
  --skip-mgba      Do not download/update the local mGBA emulator
  --force          Re-download and replace any existing local tool installs
  --help           Show this help text
EOF
}

while [ $# -gt 0 ]; do
    case "$1" in
        --skip-packages)
            INSTALL_PACKAGES=0
            ;;
        --skip-gbdk)
            INSTALL_GBDK=0
            ;;
        --skip-mgba)
            INSTALL_MGBA=0
            ;;
        --force)
            FORCE=1
            ;;
        --help|-h)
            usage
            exit 0
            ;;
        *)
            fail "Unknown option: $1"
            ;;
    esac
    shift
done

[ "$(uname -s)" = "Linux" ] || fail "This bootstrap script is for Linux only."

detect_arch() {
    case "$(uname -m)" in
        x86_64|amd64)
            printf 'x64\n'
            ;;
        aarch64|arm64)
            printf 'arm64\n'
            ;;
        *)
            fail "Unsupported CPU architecture: $(uname -m). Official bootstrap support is currently x86_64 and arm64."
            ;;
    esac
}

detect_package_manager() {
    if have apt-get; then
        printf 'apt-get\n'
    elif have dnf; then
        printf 'dnf\n'
    elif have pacman; then
        printf 'pacman\n'
    elif have zypper; then
        printf 'zypper\n'
    else
        return 1
    fi
}

run_as_root() {
    if [ "$(id -u)" -eq 0 ]; then
        "$@"
    elif have sudo; then
        sudo "$@"
    else
        fail "Need elevated privileges for package installation, but sudo is not available."
    fi
}

install_system_packages() {
    local package_manager
    package_manager="$(detect_package_manager || true)"
    [ -n "${package_manager}" ] || fail "No supported package manager found. Install make, curl, tar, gzip, xz, and ca-certificates manually."

    log "Installing missing system packages with ${package_manager}..."

    case "${package_manager}" in
        apt-get)
            run_as_root apt-get update
            run_as_root apt-get install -y make curl ca-certificates tar gzip xz-utils
            ;;
        dnf)
            run_as_root dnf install -y make curl ca-certificates tar gzip xz
            ;;
        pacman)
            run_as_root pacman -Sy --noconfirm --needed make curl ca-certificates tar gzip xz
            ;;
        zypper)
            run_as_root zypper --non-interactive install make curl ca-certificates tar gzip xz
            ;;
        *)
            fail "Unsupported package manager: ${package_manager}"
            ;;
    esac
}

json_first_string_value() {
    local key="$1"
    sed -n "s/^[[:space:]]*\"${key}\":[[:space:]]*\"\\([^\"]*\\)\".*/\\1/p" | head -n 1
}

json_download_urls() {
    sed -n 's/^[[:space:]]*"browser_download_url":[[:space:]]*"\([^"]*\)".*/\1/p'
}

require_base_tools() {
    local missing=()

    for command_name in make curl tar; do
        if ! have "${command_name}"; then
            missing+=("${command_name}")
        fi
    done

    if [ "${#missing[@]}" -gt 0 ]; then
        if [ "${INSTALL_PACKAGES}" -eq 1 ]; then
            install_system_packages
        else
            fail "Missing required commands: ${missing[*]}. Install them or rerun without --skip-packages."
        fi
    fi

    for command_name in make curl tar; do
        have "${command_name}" || fail "Required command still missing after setup: ${command_name}"
    done
}

download_release_asset() {
    local api_url="$1"
    local asset_pattern="$2"
    local output_file="$3"
    local tool_name="$4"
    local release_json
    local release_tag
    local asset_url

    log "Fetching latest ${tool_name} release metadata..."
    release_json="$(curl -fsSL "${api_url}")"
    release_tag="$(printf '%s\n' "${release_json}" | json_first_string_value tag_name)"
    asset_url="$(printf '%s\n' "${release_json}" | json_download_urls | grep -E "${asset_pattern}" | head -n 1 || true)"

    [ -n "${release_tag}" ] || fail "Could not read the latest ${tool_name} release version."
    [ -n "${asset_url}" ] || fail "Could not find a matching ${tool_name} download for this architecture."

    log "Downloading ${tool_name} ${release_tag}..."
    curl -fsSL -o "${output_file}" "${asset_url}"
}

install_gbdk() {
    local arch="$1"
    local asset_pattern
    local archive_path
    local extract_dir
    local found_lcc
    local extracted_root

    if [ -x "${GBDK_DIR}/bin/lcc" ] && [ "${FORCE}" -ne 1 ]; then
        log "GBDK already present at ${GBDK_DIR}; skipping. Use --force to refresh it."
        return
    fi

    case "${arch}" in
        x64)
            asset_pattern='gbdk-linux64\.tar\.gz$'
            ;;
        arm64)
            asset_pattern='gbdk-linux-arm64\.tar\.gz$'
            ;;
        *)
            fail "Unsupported GBDK architecture selector: ${arch}"
            ;;
    esac

    archive_path="${TMP_DIR}/gbdk.tar.gz"
    download_release_asset "${GBDK_RELEASE_API_URL}" "${asset_pattern}" "${archive_path}" "GBDK"

    extract_dir="${TMP_DIR}/gbdk-extract"
    rm -rf "${extract_dir}"
    mkdir -p "${extract_dir}"
    tar -xzf "${archive_path}" -C "${extract_dir}"

    found_lcc="$(find "${extract_dir}" -type f -path '*/bin/lcc' | head -n 1 || true)"
    [ -n "${found_lcc}" ] || fail "GBDK downloaded, but no bin/lcc was found in the extracted archive."

    extracted_root="$(dirname "$(dirname "${found_lcc}")")"
    [ -d "${extracted_root}" ] || fail "GBDK downloaded, but the extracted toolchain root could not be determined."

    rm -rf "${GBDK_DIR}"
    mkdir -p "${GBDK_DIR}"
    cp -a "${extracted_root}/." "${GBDK_DIR}/"

    [ -x "${GBDK_DIR}/bin/lcc" ] || fail "GBDK downloaded, but ${GBDK_DIR}/bin/lcc was not found."
    log "Installed GBDK to ${GBDK_DIR}"
}

install_mgba() {
    local arch="$1"
    local asset_pattern
    local appimage_path
    local wrapper_path

    if [ -x "${EMU_DIR}/mgba" ] && [ -f "${EMU_DIR}/mGBA.appimage" ] && [ "${FORCE}" -ne 1 ]; then
        log "mGBA already present at ${EMU_DIR}; skipping. Use --force to refresh it."
        return
    fi

    case "${arch}" in
        x64)
            asset_pattern='mGBA-.*-appimage-x64\.appimage$'
            ;;
        arm64)
            asset_pattern='mGBA-.*-appimage-arm64\.appimage$'
            ;;
        *)
            fail "Unsupported mGBA architecture selector: ${arch}"
            ;;
    esac

    mkdir -p "${EMU_DIR}"
    appimage_path="${EMU_DIR}/mGBA.appimage"
    wrapper_path="${EMU_DIR}/mgba"

    download_release_asset "${MGBA_RELEASE_API_URL}" "${asset_pattern}" "${appimage_path}" "mGBA"
    chmod +x "${appimage_path}"

    cat > "${wrapper_path}" <<'EOF'
#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
APPIMAGE="${SCRIPT_DIR}/mGBA.appimage"

[ -x "${APPIMAGE}" ] || {
    printf 'mGBA AppImage not found: %s\n' "${APPIMAGE}" >&2
    exit 1
}

# Extract-and-run avoids a separate FUSE dependency on many fresh Linux installs.
APPIMAGE_EXTRACT_AND_RUN=1 exec "${APPIMAGE}" "$@"
EOF
    chmod +x "${wrapper_path}"

    log "Installed mGBA to ${EMU_DIR}"
}

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "${TMP_DIR}"' EXIT

require_base_tools

ARCH="$(detect_arch)"

log "Repo root: ${ROOT_DIR}"
log "Detected architecture: ${ARCH}"

if [ "${INSTALL_GBDK}" -eq 1 ]; then
    install_gbdk "${ARCH}"
fi

if [ "${INSTALL_MGBA}" -eq 1 ]; then
    install_mgba "${ARCH}"
fi

log "Setup complete."
log "Compiler: ${GBDK_DIR}/bin/lcc"
log "Emulator wrapper: ${EMU_DIR}/mgba"
log "Next steps:"
log "  make doctor"
log "  make"
log "  make run"
