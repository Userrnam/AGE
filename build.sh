if [[ -d ".build" ]]; then {}
else
    mkdir .build
fi

cd .build
cmake ..
cmake --build .
