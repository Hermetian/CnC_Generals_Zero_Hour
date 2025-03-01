#!/bin/bash

# Build script for compiling GameSpy Replacement using Docker
# This script helps macOS users build the Windows DLL using a Docker container

echo "Building GameSpy Replacement using Docker..."
echo ""

# Check if Docker is installed
if ! command -v docker &> /dev/null; then
    echo "Error: Docker is not installed or not in PATH."
    echo "Please install Docker from https://docs.docker.com/get-docker/"
    exit 1
fi

# Check if Docker is running
if ! docker info &> /dev/null; then
    echo "Error: Docker daemon is not running."
    echo "Please start Docker and try again."
    exit 1
fi

# Create a Dockerfile
cat > Dockerfile << 'EOL'
FROM mcr.microsoft.com/windows/servercore:ltsc2019

# Install Chocolatey
RUN powershell -Command Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))

# Install build tools
RUN choco install -y cmake visualstudio2019buildtools visualstudio2019-workload-vctools

# Set up environment
WORKDIR /build

# Copy files
COPY . .

# Build command
ENTRYPOINT ["cmd.exe", "/C", "build.bat"]
EOL

echo "Docker configuration created."
echo "This will create a Windows Docker container to build the project."
echo "Note: This requires Docker with Windows containers support."
echo ""
echo "To build the project, run:"
echo "  docker build -t gsreplacement-builder ."
echo "  docker run --rm -v ${PWD}:/build gsreplacement-builder"
echo ""
echo "If you encounter issues with Windows containers, you might need to use a VM instead."
echo "You can use tools like VirtualBox or VMware to create a Windows VM for building."
echo ""
echo "Build script completed. Please review the Docker setup and run the commands manually."

chmod +x build-with-docker.sh 