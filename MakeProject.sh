# rm -r build
# rm -r bin

cd Engine
cmake -S . -B ../build/Engine
cd ../build/Engine
make

cd ../../

cd Nuim
cmake -S . -B ../build/Nuim
cd ../build/Nuim
make

cd ../../

