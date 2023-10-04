#!/bin/bash

# Define the GitHub repository owner and name
REPO_OWNER="GloriousEggroll"
REPO_NAME="wine-ge-custom"
check_tar="$(command -v tar)"
check_jq="$(command -v jq)"
check_curl="$(command -v curl)"
check_pv="$(command -v pv)"

# Check if tar is installed
if [ -z "$check_tar" ]; then
    echo "Tar not installed!"
    exit 1
fi

# Check if jq is installed
if [ -z "$check_jq" ]; then
    echo "JQ not installed!"
    exit 1
fi

# Check if curl is installed
if [ -z "$check_curl" ]; then
    echo "Curl not installed!"
    exit 1
fi

# Check if pv is installed
if [ -z "$check_pv" ]; then
    echo "Pipe Viewer (pv) not installed! Install it to show extraction progress."
    exit 1
fi

# Use the GitHub API to fetch information about the latest release
API_URL="https://api.github.com/repos/$REPO_OWNER/$REPO_NAME/releases/latest"
RELEASE_INFO=$(curl -sSL "$API_URL")

# Parse the release tag from the JSON response
RELEASE_TAG=$(echo "$RELEASE_INFO" | jq -r '.tag_name')

# Check if the release tag is empty
if [ -z "$RELEASE_TAG" ]; then
    echo "Failed to retrieve the release tag. Please check the GitHub releases page."
    exit 1
fi

echo "The latest release tag is: $RELEASE_TAG"

# Define the installation directory for WineGE
INSTALL_DIR="$HOME/.local/share/ROL_dispatcher/WINE-GE/"

# Define the directory for extracted files
EXTRACTED_DIR="$HOME/.local/share/ROL_dispatcher/WINE-GE-EXT"

# Check if the installation directory and extracted directory already exist
if [ -d "$INSTALL_DIR" ] && [ -d "$EXTRACTED_DIR" ]; then
    echo "WineGE $RELEASE_TAG is already installed in $INSTALL_DIR and extracted in $EXTRACTED_DIR. No need to download."
    exit 0
fi

# Define the download link
DOWNLOAD_URL="https://github.com/GloriousEggroll/wine-ge-custom/releases/download/$RELEASE_TAG/wine-lutris-$RELEASE_TAG-x86_64.tar.xz"

# Define the filename for the downloaded archive
FILENAME="wine-lutris-$RELEASE_TAG-x86_64.tar.xz"

# Create the installation directory if it doesn't exist
mkdir -p "$INSTALL_DIR"

# Create the extracted directory if it doesn't exist
mkdir -p "$EXTRACTED_DIR"

# Download the latest release archive file with a progress bar
curl -# -L -o "$INSTALL_DIR/$FILENAME" "$DOWNLOAD_URL"

# Extract the downloaded archive into the extracted directory with a progress bar
tar -xf "$INSTALL_DIR/$FILENAME" -C "$EXTRACTED_DIR" | pv -p --timer --rate --bytes > /dev/null

echo "Download and extraction complete. WineGE $RELEASE_TAG has been downloaded and installed in $INSTALL_DIR, and extracted to $EXTRACTED_DIR."
