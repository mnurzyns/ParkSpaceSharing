#!/bin/bash

# check if node js is installed and version is 18 or higher
if ! [ -x "$(command -v node)" ]; then
    echo 'Error: node is not installed.' >&2
    read -p "Dp you want to install it? <y/N> " prompt
    if [[ $prompt == "y" || $prompt == "Y" || $prompt == "yes" || $prompt == "Yes" ]]; then
        curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash - &&
            udo apt-get install -y nodejs
        echo "Node installed ✔️"
    else
        exit 0
    fi
else
    vo=$(node -v)
    v=${vo%.*.*}
    v=${v#v}
    if [ "$v" -lt 18 ]; then
        echo "Error: node version 18 or higher is required." >&2
        exit 1
    else
        echo "Node version is $vo ✔️"
    fi
fi

# check if npm is installed and version is 7 or higher
if ! [ -x "$(command -v npm)" ]; then
    echo 'Error: npm is not installed.' >&2
    exit 1
else
    vo=$(npm -v)
    v=${vo%.*.*}
    v=${v#v}
    if [ "$v" -lt 7 ]; then
        echo "Error: npm version 7 or higher is required." >&2
        exit 1
    else
        echo "Npm version is $vo ✔️"
        echo "Installing npm dependencies..."
        # check if npm i will return up to date
        if [[ $(pwd) = *"src"* ]]; then
            npo=$(npm i)
            echo $npo

            if [[ "$npo" = *"up to date"* ]]; then
                echo "Dependencies are up to date ✔️"
            else
                echo "There was an error installing dependiences... ❌"
            fi
        else
            echo "Current directory is not src ❌"
            echo "Please run this script from src directory (cd src && ./setup.sh)"
        fi
    fi
fi
